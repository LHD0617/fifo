/*
 * Copyright (C), 1988-1999, Xxxxxx Tech. Co., Ltd.
 * FileName: fifo.h
 * Description: 环形存储
 * Change Logs:
  |Date          |Author       |Notes     			|version
  |2022-10-21    |满心欢喜     |Initial build     	|1.0.0
 */

#ifndef __FIFO_H_
#define __FIFO_H_

/* @typedef */
typedef         unsigned char           uint8;
typedef         int                     int32;
typedef         unsigned int            uint32;
typedef         uint8                   fifo_err;

/* @define */
#define         FIFO_NULL               0x00    /* 空类型   */
#define         FIFO_ERROR_SUCCESS      0x00    /* 成功     */
#define         FIFO_ERROR_NOTSPACE     0x01    /* 空间不足 */
#define         FIFO_ERROR_NOTDATA      0x02    /* 数据不足 */
#define         FIFO_ERROR_NOTEXIST     0x03    /* 不存在   */
#define         FIFO_ERROR_OUTRANGE     0x04    /* 超出范围 */

/* fifo溢出使能，若开启此宏定义则在写入队列时不再判断队列是否为满队，溢出部分直接覆盖老数据 */
#define         FIFO_OVERFLOW_EN        0x00


/* @struct */
typedef struct
{
    int32 head;     /* 数据头 */

    int32 tail;     /* 数据尾 */

    int32 size;     /* 空间大小 */

    uint8* base;    /* 数据地址 */
}fifo_cb;

/* @Function declaration */
fifo_cb* fifo_create(uint32 size);
uint32 fifo_getAvailable(fifo_cb* cb);
uint32 fifo_getUsed(fifo_cb* cb);
fifo_err fifo_pushBuf(fifo_cb* cb, uint8* dat, uint32 len);
fifo_err fifo_popBuf(fifo_cb* cb, uint8* dat, uint32 len);
fifo_err fifo_pushByte(fifo_cb* cb, uint8 dat);
fifo_err fifo_popByte(fifo_cb* cb, uint8* dat);
fifo_err fifo_query(fifo_cb* cb, uint8* dat, uint32 index);


#endif

