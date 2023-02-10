# fifo

## 前言
本项目采用C语言开发不依赖外部工具，仅依赖C标准库《stdlib》进行内存空间malloc和free，完成环形队列（FIFO）数据结构。通过线性空间存储，它只允许在表的一端进行插入，另一端删除。这和我们日常生活中排队是一样的，最早进入队列的元素最早离开。在队列中，允许插入的一端叫做队尾，允许删除的一端则称为队头。

环形队列的关键是判断队列为空，还是为满。当tail追上head时，队列为满时，当head追上tail时，队列为空。但如何知道谁追上谁。还需要一些辅助的手段来判断.

如何判断环形队列为空，为满有两种判断方法。

 - 一.是附加一个标志位tag
当head赶上tail，队列空，则令tag = 0,
当tail赶上head，队列满，则令tag = 1,

 - 二.限制tail赶上head，即队尾结点与队首结点之间至少留有一个元素的空间。
队列空：   head == tail
队列满：   (tail + 1) % size == head

这里我们主要使用第二种方法,所以在我们申请队列空间时实际申请的空间为size + 1个字节

---

## 更新日志
 - 2022\10\22 08-57创建环形队列fifo驱动文件
 - 2022\10\22 09-14添加允许溢出写入功能
 - 2022\10\22 09-24新增fifo_err错误类型
 - 2022\10\22 09-29修复已知问题
 - 2022\10\22 09-57修改函数名称修复已知问题
 - 2022\10\22 10-18添加注释
 - 2022\11\25 15-49创建删除fifo函数
 - 2022\12\01 16-12添加清空fifo函数
 - 2023\02\10 09-51修改注释与数据类型typedef
 - 2023\02\10 10-00Create README.md

---

## 宏定义
```c
/* @define */
#define         FIFO_NULL               0x00    /* 空类型   */
#define         FIFO_ERROR_SUCCESS      0x00    /* 成功     */
#define         FIFO_ERROR_NOTSPACE     0x01    /* 空间不足 */
#define         FIFO_ERROR_NOTDATA      0x02    /* 数据不足 */
#define         FIFO_ERROR_NOTEXIST     0x03    /* 不存在   */
#define         FIFO_ERROR_OUTRANGE     0x04    /* 超出范围 */

/* fifo溢出使能，若开启此宏定义则在写入队列时不再判断队列是否为满队，溢出部分直接覆盖老数据 */
#define         FIFO_OVERFLOW_EN        0x00
```

## fifo控制块
```c
typedef struct
{
    fifo_int32 head;     /* 数据头 */

    fifo_int32 tail;     /* 数据尾 */

    fifo_int32 size;     /* 空间大小 */

    fifo_uint8* base;    /* 数据地址 */
}fifo_cb;
```
### head 数据头
表示头指针在数据基地址基础上的数据偏移位置
### tail 数据尾
表示尾指针在数据基地址基础上的数据偏移位置
### size 空间大小
表示fifo空间大小，这里的空间大小为用户申请的空间大小+1
### base 数据地址
表示fifo线性空间首地址（基地址）

## 创建fifo
```c
/**
 * @brief 创建fifo
 * 
 * @param size 空间大小
 * @return fifo_cb* 对象指针 若为FIFO_NULL 则创建失败
 */
fifo_cb* fifo_create(fifo_uint32 size)
```

## 删除fifo
```c
/**
 * @brief 删除fifo
 * 
 * @param cb 对象指针
 */
void fifo_delete(fifo_cb* cb)
```

## 获取可用空间
```c
/**
 * @brief 获取可用空间
 * 
 * @param cb 对象指针
 * @return fifo_uint32 可用空间大小
 */
fifo_uint32 fifo_getAvailable(fifo_cb* cb)
```

## 获取已用空间
```c
/**
 * @brief 获取已用空间
 * 
 * @param cb 对象指针
 * @return fifo_uint32 已用空间大小
 */
fifo_uint32 fifo_getUsed(fifo_cb* cb)
```

## 写入buf
```c
/**
 * @brief 写入buf
 * 
 * @param cb 对象指针
 * @param dat 数据地址
 * @param len 数据长度
 * @return fifo_err 错误码
 */
fifo_err fifo_pushBuf(fifo_cb* cb, fifo_uint8* dat, fifo_uint32 len)
```

## 读取buf
```c
/**
 * @brief 读取buf
 * 
 * @param cb 对象指针
 * @param dat 数据地址
 * @param len 数据长度
 * @return fifo_err 错误码
 */
fifo_err fifo_popBuf(fifo_cb* cb, fifo_uint8* dat, fifo_uint32 len)
```

## 写入字节
```c
/**
 * @brief 写入字节
 * 
 * @param cb 对象指针
 * @param dat 数据
 * @return fifo_err 错误码
 */
fifo_err fifo_pushByte(fifo_cb* cb, fifo_uint8 dat)
```

## 读取字节
```c
/**
 * @brief 读取字节
 * 
 * @param cb 对象指针
 * @param dat 数据地址
 * @return fifo_err 错误码
 */
fifo_err fifo_popByte(fifo_cb* cb, fifo_uint8* dat)
```

## 查询队列内数据
```c
/**
 * @brief 查询队列内数据
 * 
 * @param cb 对象指针
 * @param dat 数据地址
 * @param index 数据序号
 * @return fifo_err 错误码
 */
fifo_err fifo_query(fifo_cb* cb, fifo_uint8* dat, fifo_uint32 index)
```

## 清空队列
```c
/**
 * @brief 清空队列
 * 
 * @param cb 对象指针
 * @return fifo_err 错误码
 */
fifo_err fifo_clean(fifo_cb* cb)
```