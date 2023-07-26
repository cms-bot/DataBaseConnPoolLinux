/*
 * @Author: 快出来了哦
 * @Date: 2023-07-26 23:05:18
 * @LastEditors: 快出来了哦
 * @LastEditTime: 2023-07-26 23:37:04
 * @FilePath: /DataBaseConnPoolLinux/code/mysqlconn/connection.cpp
 * @Description:
 */
#include "connection.h"
Connection::Connection()
{
}
Connection::~Connection()
{
}
// 连接数据库
bool Connection::connect(std::string ip,
                         unsigned short port,
                         std::string user,
                         std::string password,
                         std::string dbname)
{

}                      
// 增删改
bool Connection::update(std::string sql)
{
    return true;
}
// 查询
bool Connection::MYSQL_RES *query(std::string sql)
{
    return true;
}
// 刷新起始空闲时间
void Connection::refreshAliveTime()
{

}
// 返回存活时间
clock_t Connection::getAliveTime()
{
    return alivetime_;
}