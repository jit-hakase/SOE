#ifndef DEF_H
#define DEF_H

#define OFFSET_OF(type, member) (size_t)&(((type*)0)->member)

#define CONTAINER_OF(ptr, type, member) ({ \
     const typeof( ((type *)0)->member ) *__mptr = (ptr); \
     (type *)( (char *)__mptr - offsetof(type,member) );})
     
#define ARRAY_SIZE_OF(array) sizeof(array) / sizeof(array[0])

#endif
