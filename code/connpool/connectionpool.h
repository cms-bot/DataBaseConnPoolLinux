/*
 * @Author: 快出来了哦
 * @Date: 2023-08-15 19:12:24
 * @LastEditors: 快出来了哦
 * @LastEditTime: 2023-08-16 13:52:57
 * @FilePath: /DataBaseConnPoolLinux/code/connpool/connectionpool.h
 * @Description:
 */
#ifndef _CONNECTIONPOOL_H
#define _CONNECTIONPOOL_H
#include <string>
#include <memory>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <queue>
#include "../mysqlconn/connection.h"
class ConnectionPoll
{
public:
    static ConnectionPoll* getConnPoll();//获得连接池
    std::shared_ptr<Connection> getConn();//获取一个连接
private:
    ConnectionPoll();
    ConnectionPoll(const ConnectionPoll&) =  delete;
    ConnectionPoll& operator =(const ConnectionPoll&) = delete;
    ~ConnectionPoll();
    bool loadConfig();//加载配置
    void produceConn();//生产连接
    void scannConn();//扫描，清理多余的空闲连接
private:
    std::string ip_;
    unsigned int port_;
    std::string username_;
    std::string password_;
    std::string dbname_;
    int initSize_;//初始连接数量
    int maxSize_;//最大连接数量
    int maxIdleTime_;//最大空闲时间
    int connTimeout_;//连接超时时间
    std::atomic_int curConnSize_;//当前连接数量
    std::mutex m_mutex_;//互斥量
    std::condition_variable m_cv_;//信号量
    std::queue<Connection*> connQue_;//连接队列
};
#endif