#pragma once

//void* Alloc(size_t size, unsigned char dimensions, ...);
//
//#define PG_Alloc(restultType,basicType,dimensions, ...) \
//    (restultType)Alloc(sizeof(basicType,dimessions,...))

#define VA_LIST_GET(number,type,out_pointer)        \
    {                                               \
        va_list list;                               \
        va_start(list, number);                     \
        for (int i = 0; i < number; i++)            \
        {                                           \
            out_pointer[i] = va_arg(list, type);    \
        }                                           \
        va_end(list);                               \
     }                                              \
