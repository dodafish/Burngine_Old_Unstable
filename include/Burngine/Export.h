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
#define BURNGINE_API __declspec(dllexport)
#else
#define BURNGINE_API
#endif

#endif /* EXPORT_H_ */
