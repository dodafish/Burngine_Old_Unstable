/*
 * Export.h
 *
 *  Created on: 05.09.2013
 *      Author: Dominik
 */

#ifndef EXPORT_H_
#define EXPORT_H_

#include <string>

#ifndef BURNGINE_STATIC

#ifndef BURNGINE_IMPORT
#define BURNGINE_API __declspec(dllexport)
#else
#define BURNGINE_API __declspec(dllimport)
#endif
#else
#define BURNGINE_API
#endif

namespace burn {
// All "common" platforms use the same size for char, short and int
// (basically there are 3 types for 3 sizes, so no other match is possible),
// we can use them without doing any kind of check

// 8 bits integer types
typedef signed char Int8;
typedef unsigned char Uint8;

// 16 bits integer types
typedef signed short Int16;
typedef unsigned short Uint16;

// 32 bits integer types
typedef signed int Int32;
typedef unsigned int Uint32;

// 64 bits integer types
#if defined(_MSC_VER)
typedef signed __int64 Int64;
typedef unsigned __int64 Uint64;
#else
typedef signed long long Int64;
typedef unsigned long long Uint64;
#endif
}

#endif /* EXPORT_H_ */
