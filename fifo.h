/*
 * Copyright (C), 1988-1999, Xxxxxx Tech. Co., Ltd.
 * FileName: fifo.h
 * Description: 环形存储
 * Change Logs:
  |Date          |Author       |Notes     			|version
  |2022-10-21    |满心欢喜     |Initial build     	|1.0.0
 */

/* @typedef */
typedef         unsigned char           uint8;
typedef         int                     int32;
typedef         unsigned int            uint32;

/* @define */
#define         FIFO_NULL               0x00
#define         FIFO_ERROR_SUCCESS      0x00
#define         FIFO_ERROR_NOTSPACE     0x01
#define         FIFO_ERROR_NOTDATA      0x02
#define         FIFO_ERROR_NOTEXIST     0x03
#define         FIFO_ERROR_OUTRANGE     0x04


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
uint32 fifo_getAvailable(fifo_cb cb);
uint32 fifo_getUsed(fifo_cb* cb);
uint8 fifo_pushbuf(fifo_cb* cb, uint8* dat, uint32 len);
uint8 fifo_popbuf(fifo_cb* cb, uint8* dat, uint32 len);
uint8 fifo_query(fifo_cb* cb, uint8* dat, uint32 index);