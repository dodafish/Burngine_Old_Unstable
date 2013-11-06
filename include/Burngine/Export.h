//////////////////////////////////////////////////////////////////////////////
//
// Burngine is distributed under the GNU GPL v2 License
// ====================================================
//
//    Copyright (C) 2013 Dominik David (frischer-hering@gmx.de)
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//////////////////////////////////////////////////////////////////////////////

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
