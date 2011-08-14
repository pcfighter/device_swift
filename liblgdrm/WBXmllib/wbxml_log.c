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
 * @file wbxml_log.c
 * @ingroup wbxml_log
 *
 * @author Aymerick Jehanne <libwbxml@aymerick.com>
 * @date 02/12/04
 *
 * @brief Log Functions
 */

#include "wbxml.h"


#if defined( WBXML_LIB_VERBOSE ) && !defined( WBXML_USE_LEAKTRACKER )

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "DRM_Port.h"

/** Length of Log Buffer */
#define WBXML_LOG_FORMAT_SIZE 1024


/* Private Functions Prototypes */
static void format_log_message(WB_TINY *buf, WB_UTINY type, const WB_TINY *fmt);
static const WB_TINY *get_type(WB_UTINY type);


/***************************************************
 *    Public Functions
 */

void wbxml_log_debug(WB_UTINY type, const WB_TINY *fmt, ...)
{
    char buf[WBXML_LOG_FORMAT_SIZE];
	int nLen;
	
    va_list args;
    
	if (type == WBXML_PARSER)
	{
		drmLibStrcpy(buf, "[DRM] WB Parser : ");
	}
	else if (type == WBXML_ENCODER)
	{
		drmLibStrcpy(buf, "[DRM] WB Encoder : ");
	}
	else if (type == WBXML_CONV)
	{
		drmLibStrcpy(buf, "[DRM] WB Coverter : ");
	}
	else
	{
		drmLibStrcpy(buf, "[DRM] WB Unknow : ");
	}
	nLen = drmLibStrlen(buf);

    va_start(args, fmt);
	vsprintf(buf+nLen,fmt,args);
    va_end(args);

	drmLibStrcat(buf, "\n");
	drmLibWriteLog(buf);
}

void wbxml_log_warning(WB_UTINY type, const WB_TINY *fmt, ...)
{
    char buf[WBXML_LOG_FORMAT_SIZE];
	int nLen;
	
    va_list args;
    
	if (type == WBXML_PARSER)
	{
		drmLibStrcpy(buf, "[DRMWARN] WB Parser : ");
	}
	else if (type == WBXML_ENCODER)
	{
		drmLibStrcpy(buf, "[DRMWARN] WB Encoder : ");
	}
	else if (type == WBXML_CONV)
	{
		drmLibStrcpy(buf, "[DRMWARN] WB Coverter : ");
	}
	else
	{
		drmLibStrcpy(buf, "[DRMWARN] WB Unknow : ");
	}
	nLen = drmLibStrlen(buf);

    va_start(args, fmt);
	vsprintf(buf+nLen,fmt,args);
    va_end(args);

	drmLibStrcat(buf, "\n");
	drmLibWriteLog(buf);
}

void wbxml_log_error(WB_UTINY type, const WB_TINY *fmt, ...)
{
    char buf[WBXML_LOG_FORMAT_SIZE];
	int nLen;
	
    va_list args;
    
	if (type == WBXML_PARSER)
	{
		drmLibStrcpy(buf, "[DRMERR] WB Parser : ");
	}
	else if (type == WBXML_ENCODER)
	{
		drmLibStrcpy(buf, "[DRMERR] WB Encoder : ");
	}
	else if (type == WBXML_CONV)
	{
		drmLibStrcpy(buf, "[DRMERR] WB Coverter : ");
	}
	else
	{
		drmLibStrcpy(buf, "[DRMERR] WB Unknow : ");
	}
	nLen = drmLibStrlen(buf);

    va_start(args, fmt);
	vsprintf(buf+nLen,fmt,args);
    va_end(args);

	drmLibStrcat(buf, "\n");
	drmLibWriteLog(buf);
}


/***************************************************
 *    Private Functions
 */

/**
 * @brief Format a Log Message
 * @param buf [out] Resulting formated buffer
 * @param type [in] Type of Message
 * @param fmt [int] Message to format
 */
static void format_log_message(WB_TINY *buf, WB_UTINY type, const WB_TINY *fmt)
{
    WB_TINY *p, prefix[WBXML_LOG_FORMAT_SIZE];
    
    p = prefix;
    
    sprintf(p, "%s> ", get_type(type));
    
    if (WBXML_STRLEN(prefix) + WBXML_STRLEN(fmt) > WBXML_LOG_FORMAT_SIZE / 2) {
        sprintf(buf, "(LOG MESSAGE TOO LONG !)\n");
        return;
    }
    
    sprintf(buf, "%s%s\n", prefix, fmt);    
}

/**
 * @brief Get a Message Type string
 * @param type [in] Type of Message
 * @return The string representation of the Message Type
 */
static const WB_TINY *get_type(WB_UTINY type) 
{
    switch (type)
    {
        case WBXML_PARSER:
            return "WBXML Parser";
        
        case WBXML_ENCODER:
            return "WBXML Encoder";

        case WBXML_CONV:
            return "WBXML Converter";
        
        default:
            return "WBXML Unknown";
    }
}

#endif /* WBXML_LIB_VERBOSE */
