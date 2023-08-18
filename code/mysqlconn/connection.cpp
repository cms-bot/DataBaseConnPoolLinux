/*
 * @Author: 快出来了哦
 * @Date: 2023-07-26 23:05:18
 * @LastEditors: 快出来了哦
 * @LastEditTime: 2023-08-17 16:00:30
 * @FilePath: /DataBaseConnPoolLinux/code/mysqlconn/connection.cpp
 * @Description:
 */
#include "connection.h"
#include "../log/log.h"
Connection::Connection()
{
    conn_ = mysql_init(nullptr);
}
Connection::~Connection()
{
    if(conn_ != nullptr)
    {
        mysql_close(conn_);
        conn_ = nullptr;
    }
}
// 连接数据库
bool Connection::connect(std::string ip,
             unsigned int port,
             std::string user,
             std::string password,
             std::string dbname)
{
    if (!mysql_real_connect(conn_, ip.c_str(), user.c_str(), password.c_str(), dbname.c_str(), port, nullptr, 0))
    {
        LOG("数据库连接失败:"+mysql_errno(conn_));
        return false;
    }
    return true;
}
// 增删改
bool Connection::update(std::string sql)
{
    if(mysql_query(conn_,sql.c_str()) != 0)
    {
        LOG("数据库更新失败:"+mysql_errno(conn_));
        return false;
    }
    return true;
}
// 查询
MYSQL_RES *Connection::query(std::string sql)
{
    MYSQL_RES* result = mysql_store_result(conn_);
    if(!result)
    {
        LOG("数据库查询失败:"+mysql_errno(conn_));
        return nullptr;
    }
    return result;
}
// 刷新起始空闲时间
void Connection::refreshAliveTime()
{
    alivetime_ = clock();
}
// 返回存活时间
clock_t Connection::getAliveTime()
{
    return clock() - alivetime_;
}