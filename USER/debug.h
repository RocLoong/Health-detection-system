/*
 * @Author: Rocloong 63897185+RocLoong@users.noreply.github.com
 * @Date: 2023-03-19 13:41:55
 * @LastEditors: Rocloong 63897185+RocLoong@users.noreply.github.com
 * @LastEditTime: 2023-03-19 14:13:49
 * @FilePath: \USER\debug.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __DEBUG__H_
#define __DEBUG__H_


#define DEBUG

#ifdef DEBUG

#define debug(fmt, ...) printf("[debug] %s  %d:\r\n"fmt, __FILE__, __LINE__,##__VA_ARGS__)

#else

#define debug(fmt, ...)

#endif

#endif


