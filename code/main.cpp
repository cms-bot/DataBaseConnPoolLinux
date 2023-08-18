/*
 * @Author: 快出来了哦
 * @Date: 2023-07-26 21:52:04
 * @LastEditors: 快出来了哦
 * @LastEditTime: 2023-08-18 17:25:21
 * @FilePath: /DataBaseConnPoolLinux/code/main.cpp
 * @Description:
 */
#include <iostream>
#include <thread>
#include "./connpool/connectionpool.h"
int main()
{
    std::cout << "插入之前" << std::endl;
    clock_t begin = clock();
    // std::thread t1([]()
    //                {
     ConnectionPoll *cp = ConnectionPoll::getConnPoll();
    for (int i = 0; i < 2500; ++i)
    {
        char sql[1024] = {0};
        sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
                "t1", 20, "male");
        std::shared_ptr<Connection> sp = cp->getConn();
        if (sp != nullptr)
        {
            sp->update(sql);
        }
      } //});

    clock_t end = clock();
    std::cout << (end - begin) << "ms" << std::endl;
    std::cout << "插入之后" << std::endl;
    getchar();
    return 0;
}