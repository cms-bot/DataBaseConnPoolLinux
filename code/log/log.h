/*
 * @Author: 快出来了哦
 * @Date: 2023-07-27 22:49:36
 * @LastEditors: 快出来了哦
 * @LastEditTime: 2023-07-27 23:21:53
 * @FilePath: /DataBaseConnPoolLinux/code/log/log.h
 * @Description: 
 */
#ifndef _LOG_H
#define _LOG_H
#include <iostream>
#define LOG(str) \
	std::cout << __FILE__ << ":" << __LINE__ << " " << \
	__TIMESTAMP__ << " : " << str << std::endl;
#endif