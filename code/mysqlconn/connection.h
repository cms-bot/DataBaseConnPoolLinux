/*
 * @Author: 快出来了哦
 * @Date: 2023-07-26 23:04:57
 * @LastEditors: 快出来了哦
 * @LastEditTime: 2023-08-18 14:56:51
 * @FilePath: /DataBaseConnPoolLinux/code/mysqlconn/connection.h
 * @Description:
 */
#ifndef _CONNECTION_H
#define _CONNECTION_H
#include <mysql.h>
#include <string>
#include <ctime>
class Connection
{
public:
    // 初始化数据库连接
    Connection();
    // 释放数据库连接
    ~Connection();
    // 连接数据库
    bool connect(std::string ip,
                 unsigned int port,
                 std::string user,
                 std::string password,
                 std::string dbname);
    // 增删改
    bool update(std::string sql);
    // 查询
    MYSQL_RES* query(std::string sql);
    //刷新起始空闲时间
    void refreshAliveTime();
    //返回存活时间
    clock_t getAliveTime();
private:
    MYSQL* conn_;//数据的一条连接
    clock_t alivetime_;//进入空闲状态后的起始存活时间
};
#endif