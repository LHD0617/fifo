/**
 * @file fifo.c
 * @author 满心欢喜
 * @brief 环形队列存储
 * @version 0.1
 * @date 2023-02-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */

/* @include */
#include <stdlib.h>
#include "fifo.h"

/**
 * @brief 创建fifo
 * 
 * @param size 空间大小
 * @return fifo_cb_t* 对象指针 若为FIFO_NULL 则创建失败
 */
fifo_cb_t* fifo_create(fifo_uint32 size)
{
    fifo_cb_t* cb = (fifo_cb_t*)FIFO_MALLOC(sizeof(fifo_cb_t) + size + 1);
    if(!cb)  return FIFO_NULL;
    cb->size = size + 1;
    cb->head = 0;
    cb->tail = 0;
    return cb;
}

/**
 * @brief 删除fifo
 * 
 * @param cb 对象指针
 */
void fifo_delete(fifo_cb_t* cb)
{
    FIFO_FREE(cb);
    cb = FIFO_NULL;
}

/**
 * @brief 获取可用空间
 * 
 * @param cb 对象指针
 * @return fifo_uint32 可用空间大小
 */
fifo_uint32 fifo_getAvailable(fifo_cb_t* cb)
{
    if(cb == FIFO_NULL) return 0;
    return (cb->size - ((cb->tail - cb->head + cb->size) % cb->size)) - 1;
}

/**
 * @brief 获取已用空间
 * 
 * @param cb 对象指针
 * @return fifo_uint32 已用空间大小
 */
fifo_uint32 fifo_getUsed(fifo_cb_t* cb)
{
    if(cb == FIFO_NULL) return 0;
    return (cb->tail - cb->head + cb->size) % cb->size;
}

/**
 * @brief 写入buf
 * 
 * @param cb 对象指针
 * @param dat 数据地址
 * @param len 数据长度
 * @return fifo_err 错误码
 */
fifo_err fifo_pushBuf(fifo_cb_t* cb, fifo_uint8* dat, fifo_uint32 len)
{
    if(cb == FIFO_NULL) return FIFO_ERROR_NOTEXIST;
#if FIFO_OVERFLOW_EN
    while(len--)
    {
        *(cb->base + cb->tail) = *(dat++);
        cb->tail = (cb->tail + 1) % cb->size;
        if(cb->tail == cb->head) cb->head = (cb->head + 1) % cb->size;
    }
    return FIFO_ERROR_SUCCESS;
#else
    if(fifo_getAvailable(cb) >= len)
    {
        while(len--)
        {
            *(cb->base + cb->tail) = *(dat++);
            cb->tail = (cb->tail + 1) % cb->size;
        }
        return FIFO_ERROR_SUCCESS;
    }
    return FIFO_ERROR_NOTSPACE;
#endif
}

/**
 * @brief 读取buf
 * 
 * @param cb 对象指针
 * @param dat 数据地址
 * @param len 数据长度
 * @return fifo_err 错误码
 */
fifo_err fifo_popBuf(fifo_cb_t* cb, fifo_uint8* dat, fifo_uint32 len)
{
    if(cb == FIFO_NULL) return FIFO_ERROR_NOTEXIST;
    if(fifo_getUsed(cb) >= len)
    {
        while(len--)
        {
            *(dat++) = *(cb->base + cb->head);
            cb->head = (cb->head + 1) % cb->size;
        }
        return FIFO_ERROR_SUCCESS;
    }
    return FIFO_ERROR_NOTDATA;
}

/**
 * @brief 写入字节
 * 
 * @param cb 对象指针
 * @param dat 数据
 * @return fifo_err 错误码
 */
fifo_err fifo_pushByte(fifo_cb_t* cb, fifo_uint8 dat)
{
    if(cb == FIFO_NULL) return FIFO_ERROR_NOTEXIST;
#if FIFO_OVERFLOW_EN
    *(cb->base + cb->tail) = dat;
    cb->tail = (cb->tail + 1) % cb->size;
    if(cb->tail == cb->head) cb->head = (cb->head + 1) % cb->size;
    return FIFO_ERROR_SUCCESS;
#else
    if(fifo_getAvailable(cb) >= 1)
    {
        *(cb->base + cb->tail) = dat;
        cb->tail = (cb->tail + 1) % cb->size;
        return FIFO_ERROR_SUCCESS;
    }
    return FIFO_ERROR_NOTSPACE;
#endif
}

/**
 * @brief 读取字节
 * 
 * @param cb 对象指针
 * @param dat 数据地址
 * @return fifo_err 错误码
 */
fifo_err fifo_popByte(fifo_cb_t* cb, fifo_uint8* dat)
{
    if(cb == FIFO_NULL) return FIFO_ERROR_NOTEXIST;
    if(fifo_getUsed(cb) >= 1)
    {
        *dat = *(cb->base + cb->head);
        cb->head = (cb->head + 1) % cb->size;
        return FIFO_ERROR_SUCCESS;
    }
    return FIFO_ERROR_NOTDATA;
}

/**
 * @brief 查询队列内数据
 * 
 * @param cb 对象指针
 * @param dat 数据地址
 * @param index 数据序号
 * @return fifo_err 错误码
 */
fifo_err fifo_query(fifo_cb_t* cb, fifo_uint8* dat, fifo_uint32 index)
{
    if(cb == FIFO_NULL) return FIFO_ERROR_NOTEXIST;
    if(index >= fifo_getUsed(cb)) return FIFO_ERROR_OUTRANGE;
    *dat = *(cb->base + ((cb->head + index) % cb->size));
    return FIFO_ERROR_SUCCESS;
}

/**
 * @brief 清空队列
 * 
 * @param cb 对象指针
 * @return fifo_err 错误码
 */
fifo_err fifo_clean(fifo_cb_t* cb)
{
    if(cb == FIFO_NULL) return FIFO_ERROR_NOTEXIST;
    cb->tail = cb->head;
    return FIFO_ERROR_SUCCESS;
}
