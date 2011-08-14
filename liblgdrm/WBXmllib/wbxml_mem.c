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
 * @file wbxml_mem.c
 * @ingroup wbxml_mem
 *
 * @author Aymerick Jehanne <libwbxml@aymerick.com>
 * @date 02/11/24
 *
 * @brief Memory Functions
 */

#include "wbxml.h"

#include "DRM_Port.h"

/***************************************************
 *    Public Functions
 */

#ifdef DRM_DEBUG_HEAP_LIBWBXML
wbxml_freeFunc wbxml_free = (wbxml_freeFunc) drmLibWbXmlFree;
wbxml_mallocFunc wbxml_malloc = (wbxml_mallocFunc) drmLibWbXmlMalloc;
wbxml_reallocFunc wbxml_realloc = (wbxml_reallocFunc) drmLibWbXmlRealloc;
wbxml_strdupFunc wbxml_strdup = (wbxml_strdupFunc) drmLibWbXmlStrdup;
#else
wbxml_freeFunc wbxml_free = (wbxml_freeFunc) drmLibFree;
wbxml_mallocFunc wbxml_malloc = (wbxml_mallocFunc) drmLibMalloc;
wbxml_reallocFunc wbxml_realloc = (wbxml_reallocFunc) drmLibRealloc;
wbxml_strdupFunc wbxml_strdup = (wbxml_strdupFunc) drmLibStrdup;
#endif

WBXML_STRLENFunc WBXML_STRLEN = (WBXML_STRLENFunc) drmLibStrlen;
WBXML_STRCMPFunc WBXML_STRCMP = (WBXML_STRCMPFunc) drmLibStrcmp;
WBXML_STRNCMPFunc WBXML_STRNCMP = (WBXML_STRNCMPFunc) drmLibStrncmp;
WBXML_STRSTRFunc WBXML_STRSTR = (WBXML_STRSTRFunc) drmLibStrstr;
WBXML_STRCASECMPFunc WBXML_STRCASECMP = (WBXML_STRCASECMPFunc) drmLibStricmp;
WBXML_SPRINTFFunc WBXML_SPRINTFF = (WBXML_SPRINTFFunc) drmLibSprintf;

#if 0
void * wbxml_malloc(size_t size)
{
#ifdef WBXML_USE_LEAKTRACKER
    return lt_malloc(size);
#else
    return malloc(size);
#endif
}


void wbxml_free(void *memblock)
{
#ifdef WBXML_USE_LEAKTRACKER
    lt_free(memblock);
#else
    free(memblock);
#endif
}


void * wbxml_realloc(void *memblock, size_t size)
{
#ifdef WBXML_USE_LEAKTRACKER
    return lt_realloc(memblock, size);
#else
    return realloc(memblock, size);
#endif
}


char * wbxml_strdup(const char *str)
{
#ifdef WBXML_USE_LEAKTRACKER
    return lt_strdup(str);
#else
    return strdup(str);
#endif
}

#endif
