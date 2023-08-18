/*
 * @Author: 快出来了哦
 * @Date: 2023-08-15 20:28:36
 * @LastEditors: 快出来了哦
 * @LastEditTime: 2023-08-18 17:14:56
 * @FilePath: /DataBaseConnPoolLinux/code/connpool/connectionpool.cpp
 * @Description:
 */
#include <thread>
#include <functional>
#include <chrono>
#include "connectionpool.h"
#include "../log/log.h"
ConnectionPoll::ConnectionPoll()
{
    if (!loadConfig())
    {
        LOG("加载配置文件出错");
        return;
    }
    // 分配初始连接
    for (int i = 0; i < initSize_; ++i)
    {
        Connection *p = new Connection();
        p->connect(ip_, port_, username_, password_, dbname_);
        p->refreshAliveTime();
        connQue_.push(p);
        curConnSize_++;
    }
    // 生产连接
    std::thread producer(std::bind(&ConnectionPoll::produceConn, this));
    // 回收连接
    std::thread scanner(std::bind(&ConnectionPoll::scannConn, this));
    producer.detach();
    scanner.detach();
}
ConnectionPoll::~ConnectionPoll()
{
    m_cv_.notify_all();
}
std::shared_ptr<Connection> ConnectionPoll::getConn()
{
    std::unique_lock<std::mutex> lock(m_mutex_);
    while (connQue_.empty())
    {
        if (std::cv_status::timeout == m_cv_.wait_for(lock, std::chrono::microseconds(connTimeout_)))
        {
            if (connQue_.empty())
            {
                LOG("连接超时...");
                return nullptr;
            }
        }
    }
    // 自定义析构
    std::shared_ptr<Connection> sp(connQue_.front(), std::function<void(Connection* ptr)>([&](Connection* ptr)
        {
        std::unique_lock<std::mutex> userlock(m_mutex_);
        ptr->refreshAliveTime();
        connQue_.push(ptr); 
        }));
    connQue_.pop();
    m_cv_.notify_all();//消费完成后通知一下
    return sp;
}
ConnectionPoll *ConnectionPoll::getConnPoll()
{
    static ConnectionPoll pool;
    return &pool;
}
bool ConnectionPoll::loadConfig()
{

    FILE *pf = fopen("../configfile/connpool.ini", "r");
    if (pf == nullptr)
    {
        LOG("connpool.ini file is not exist!");
        return false;
    }

    while (!feof(pf))
    {
        char line[1024] = {0};
        fgets(line, 1024, pf);
        std::string str = line;
        int idx = str.find('=', 0);
        if (idx == -1) // 无效的配置项
        {
            continue;
        }
        // password=123456\n
        int endidx = str.find('\n', idx);
        std::string key = str.substr(0, idx);
        std::string value = str.substr(idx + 1, endidx - idx - 1);

        if (key == "ip")
        {
            ip_ = value;
        }
        else if (key == "port")
        {
            port_ = atoi(value.c_str());
        }
        else if (key == "username")
        {
            username_ = value;
        }
        else if (key == "password")
        {
            password_ = value;
        }
        else if (key == "dbname")
        {
            dbname_ = value;
        }
        else if (key == "initSize")
        {
            initSize_ = atoi(value.c_str());
        }
        else if (key == "maxSize")
        {
            maxSize_ = atoi(value.c_str());
        }
        else if (key == "maxIdleTime")
        {
            maxIdleTime_ = atoi(value.c_str());
        }
        else if (key == "connTimeOut")
        {
            connTimeout_ = atoi(value.c_str());
        }
    }
    return true;
}
void ConnectionPoll::scannConn()
{
    for (;;)
    {
        std::this_thread::sleep_for(std::chrono::microseconds(maxIdleTime_*1000));
         std::unique_lock<std::mutex> lock(m_mutex_);
         while(curConnSize_ > initSize_)
         {
            if(connQue_.empty()) break;
            auto ptr = connQue_.front();
            if(ptr &&ptr->getAliveTime() >= (maxIdleTime_*1000))
            {
                connQue_.pop();
                curConnSize_--;
                delete ptr;
                ptr = nullptr;
                LOG("释放连接");
            }else{
                break;
            }
         } 
    }
}
void ConnectionPoll::produceConn()
{
    for (;;)
    {
        std::unique_lock<std::mutex> lock(m_mutex_);
        m_cv_.wait(lock,[&]()
        {
            return connQue_.empty();
        });
        if (curConnSize_ < maxSize_)
        {
            Connection *p = new Connection();
            p->connect(ip_, port_, username_, password_, dbname_);
            p->refreshAliveTime();
            connQue_.push(p);
            curConnSize_++;
            LOG("产生新连接");
        }
        m_cv_.notify_all();
    }
}
