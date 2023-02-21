/**
 * @file fifo.h
 * @author 满心欢喜
 * @brief 环形队列存储
 * @version 0.1
 * @date 2023-02-10
 * 
 * @copyright Copyright (c) 2023
 * struct
 */

#ifndef __FIFO_H_
#define __FIFO_H_

/* @typedef */
typedef         unsigned char           fifo_uint8;
typedef         int                     fifo_int32;
typedef         unsigned int            fifo_uint32;
typedef         fifo_uint8              fifo_err;

/* @define */
/* @define */
#define         FIFO_MALLOC             malloc      /* 空间申请接口 */
#define         FIFO_FREE               free        /* 空间释放接口 */

#define         FIFO_NULL               NULL        /* 空类型   */
#define         FIFO_ERROR_SUCCESS      0x00        /* 成功     */
#define         FIFO_ERROR_NOTSPACE     0x01        /* 空间不足 */
#define         FIFO_ERROR_NOTDATA      0x02        /* 数据不足 */
#define         FIFO_ERROR_NOTEXIST     0x03        /* 不存在   */
#define         FIFO_ERROR_OUTRANGE     0x04        /* 超出范围 */

/* fifo溢出使能，若开启此宏定义则在写入队列时不再判断队列是否为满队，溢出部分直接覆盖老数据 */
#define         FIFO_OVERFLOW_EN        0x00


/* @struct */
#pragma pack(1) // 单字节对齐

/**
 * @brief 队列控制块
 * 
 */
typedef struct
{
    fifo_int32 head;        /* 数据头 */

    fifo_int32 tail;        /* 数据尾 */

    fifo_int32 size;        /* 空间大小 */

    fifo_uint8 base[];      /* 数据地址 */
}fifo_cb_t;

#pragma pack() // 恢复默认字节对齐

/* @Function declaration */
fifo_cb_t* fifo_create(fifo_uint32 size);
void fifo_delete(fifo_cb_t* cb);
fifo_uint32 fifo_getAvailable(fifo_cb_t* cb);
fifo_uint32 fifo_getUsed(fifo_cb_t* cb);
fifo_err fifo_pushBuf(fifo_cb_t* cb, fifo_uint8* dat, fifo_uint32 len);
fifo_err fifo_popBuf(fifo_cb_t* cb, fifo_uint8* dat, fifo_uint32 len);
fifo_err fifo_pushByte(fifo_cb_t* cb, fifo_uint8 dat);
fifo_err fifo_popByte(fifo_cb_t* cb, fifo_uint8* dat);
fifo_err fifo_query(fifo_cb_t* cb, fifo_uint8* dat, fifo_uint32 index);
fifo_err fifo_clean(fifo_cb_t* cb);

#endif
