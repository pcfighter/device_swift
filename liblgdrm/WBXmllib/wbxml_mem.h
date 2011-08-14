/*
 * libwbxml, the WBXML Library.
 * Copyright (C) 2002-2005 Aymerick Jehanne <aymerick@jehanne.org>
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 * LGPL v2.1: http://www.gnu.org/copyleft/lesser.txt
 * 
 * Contact: libwbxml@aymerick.com
 * Home: http://libwbxml.aymerick.com
 */
 
/**
 * @file wbxml_mem.h
 * @ingroup wbxml_mem
 *
 * @author Aymerick Jehanne <libwbxml@aymerick.com>
 * @date 02/07/01
 *
 * @brief Memory Functions
 */

#ifndef WBXML_MEM_H
#define WBXML_MEM_H


#ifdef WBXML_USE_LEAKTRACKER
#include "leaktrack.h"
#include "lt_log.h"
#define wbxml_mem_cleam(ptr) (lt_claim_area(ptr))
#else  /* WBXML_USE_LEAKTRACKER */
#define wbxml_mem_cleam(ptr) (ptr)
#endif /* WBXML_USE_LEAKTRACKER */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @addtogroup wbxml_mem  
 *  @{ 
 */

/**
 * @brief Alloc a Memory Block
 * @param size Size of Memory to alloc
 * @return The newly mlloced Memory Block, or NULL if not enough memory
 */
//void * wbxml_malloc(size_t size);
typedef void *( *wbxml_mallocFunc)(size_t size);

/**
 * @brief Free a Memory Block
 * @param memblock The Memory Block to free
 */
//void wbxml_free(void *memblock);
typedef void ( *wbxml_freeFunc)(void *mem);

/**
 * @brief Realloc a Memory Block
 * @param memblock The Memory Block to realloc
 * @param size Size of Memory to realloc
 * @return The newly realloced Memory Block, or NULL if not enough memory
 */
//void * wbxml_realloc(void *memblock, size_t size);
typedef void *( *wbxml_reallocFunc)(void *mem, size_t size);

/**
 * @brief Duplicate a C String
 * @param str The C String to duplicate
 * @return The newly duplicated C String, or NULL if not enough memory
 */
//char * wbxml_strdup(const char *str);
typedef char *( *wbxml_strdupFunc)(const char *str);


/* String Function */
typedef unsigned int 	(*WBXML_STRLENFunc)(const char *pString);
typedef unsigned int 	(*WBXML_STRCMPFunc)(const char *pTarget, const char *pSource);
typedef unsigned int 	(*WBXML_STRNCMPFunc)(const char *pTarget, const char *pSource, int iCount);
typedef char* 				(*WBXML_STRSTRFunc)(const char *pString, const char *pSubString);
typedef int 					(*WBXML_STRCASECMPFunc)(const char *pTarget, const char *pSource);

typedef int 					(*WBXML_SPRINTFFunc)(char *pcBuf,char *pszFormat, ...);
	

/** @} */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* WBXML_MEM_H */
