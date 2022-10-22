/*
 * Copyright (C), 1988-1999, Xxxxxx Tech. Co., Ltd.
 * FileName: fifo.c
 * Description: 环形存储
 * Change Logs:
  |Date          |Author       |Notes     			|version
  |2022-10-21    |满心欢喜     |Initial build     	|1.0.0
 */

/* @include */
#include <stdlib.h>
#include "fifo.h"

/**
    * @name		fifo_create
    * @brief  	创建fifo
  */
fifo_cb* fifo_create(uint32 size)
{
    fifo_cb* cb = (fifo_cb*)malloc(sizeof(fifo_cb));
    if(cb == FIFO_NULL)  return FIFO_NULL;
    cb->size = size + 1;
    cb->base = (uint8*)malloc(sizeof(uint8) * (size + 1));
    if(cb->base == FIFO_NULL) return FIFO_NULL;
    cb->head = 0;
    cb->tail = 0;
    return cb;
}

/**
    * @name		fifo_getAvailable
    * @brief  	获取可用空间
  */
uint32 fifo_getAvailable(fifo_cb* cb)
{
    if(cb == FIFO_NULL) return 0;
    return (cb->size - ((cb->tail - cb->head + cb->size) % cb->size)) - 1;
}

/**
    * @name		fifo_getUsed
    * @brief  	获取已用空间
  */
uint32 fifo_getUsed(fifo_cb* cb)
{
    if(cb == FIFO_NULL) return 0;
    return (cb->tail - cb->head + cb->size) % cb->size;
}

/**
    * @name		fifo_pushBuf
    * @brief  	写入buf
  */
fifo_err fifo_pushBuf(fifo_cb* cb, uint8* dat, uint32 len)
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
    * @name		fifo_popBuf
    * @brief  	读取buf
  */
fifo_err fifo_popBuf(fifo_cb* cb, uint8* dat, uint32 len)
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
    * @name		fifo_pushByte
    * @brief  	写入字节
  */
fifo_err fifo_pushByte(fifo_cb* cb, uint8 dat)
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
    * @name		fifo_popByte
    * @brief  	读取字节
  */
fifo_err fifo_popByte(fifo_cb* cb, uint8* dat)
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
    * @name		fifo_query
    * @brief  	查询队列内数据
  */
fifo_err fifo_query(fifo_cb* cb, uint8* dat, uint32 index)
{
    if(index >= fifo_getUsed(cb)) return FIFO_ERROR_OUTRANGE;
    *dat = *(cb->base + ((cb->head + index) % cb->size));
    return FIFO_ERROR_SUCCESS;
}

