#ifndef __NOP_TYPE_H__
#define __NOP_TYPE_H__

#include <stddef.h>

typedef signed long long int64_t;
typedef signed int       int32_t;
typedef signed short     int16_t;
typedef signed char      int8_t;

typedef unsigned long long uint64_t;
typedef unsigned int       uint32_t;
typedef unsigned short     uint16_t;
typedef unsigned char      uint8_t;

// we need a signed 33-bit integer, but that doesn't exist so we have to do this...
typedef int64_t off_t;

#endif
