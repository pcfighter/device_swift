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
 * @file wbxml.h
 * @ingroup wbxml
 *
 * @author Aymerick Jehanne <libwbxml@aymerick.com>
 * @date 02/11/11
 *
 * @brief WBXML Library Main Header
 */

#ifndef WBXML_H
#define WBXML_H

#if defined( __SYMBIAN32__ )
/* For basic Symbian Types */
#include <e32def.h>
#endif /* __SYMBIAN32__ */

/*
 * This sytem includes are here instead of the *.c files because
 * we want them to be included AFTER 'e32def.h' on Symbian. If not so,
 * a lot of Warnings are displayed ('NULL' : macro redefinition)
 */
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/** @addtogroup wbxml
 *  @{ 
 */

/** WBXML Parser Lib Version */
#define WBXML_LIB_VERSION "0.9.2"

/** Backward compatibility flag */
#define WBXML_BACKWARD_COMPAT

/* WBXML Lib basic types redefinition */
#define WB_BOOL unsigned char
#define WB_UTINY unsigned char
#define WB_TINY char
#define WB_ULONG unsigned int
#define WB_LONG int

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL 0
#endif

/* Define NULL string */
#define WBXML_UTINY_NULL_STRING ((WB_UTINY *)"")

#if 0
/* WBXML Lib string functions */
#define WBXML_STRLEN(a) strlen((const WB_TINY*)a)
#define WBXML_STRCMP(a,b) strcmp((const WB_TINY*)a,(const WB_TINY*)b)
#define WBXML_STRNCMP(a,b,c) strncmp((const WB_TINY*)a,(const WB_TINY*)b,c)
#define WBXML_STRSTR(a,b) strstr((const WB_TINY*)a,(const WB_TINY*)b)
#if defined( WIN32 )
#define WBXML_STRCASECMP(a,b) _stricmp((const WB_TINY*)a,(const WB_TINY*)b)
#else
#define WBXML_STRCASECMP(a,b) strcasecmp((const WB_TINY*)a,(const WB_TINY*)b)
#endif /* WIN32 */

#define WBXML_ISDIGIT(a) isdigit(a)
#endif

/* WBXML Global Tokens */
#define WBXML_SWITCH_PAGE 0x00
#define WBXML_END         0x01
#define WBXML_ENTITY      0x02
#define WBXML_STR_I       0x03
#define WBXML_LITERAL     0x04
#define WBXML_EXT_I_0     0x40
#define WBXML_EXT_I_1     0x41
#define WBXML_EXT_I_2     0x42
#define WBXML_PI          0x43
#define WBXML_LITERAL_C   0x44
#define WBXML_EXT_T_0     0x80
#define WBXML_EXT_T_1     0x81
#define WBXML_EXT_T_2     0x82
#define WBXML_STR_T       0x83
#define WBXML_LITERAL_A   0x84
#define WBXML_EXT_0       0xC0
#define WBXML_EXT_1       0xC1
#define WBXML_EXT_2       0xC2
#define WBXML_OPAQUE      0xC3
#define WBXML_LITERAL_AC  0xC4

/* WBXML Tokens Masks */
#define WBXML_TOKEN_MASK              0x3F
#define WBXML_TOKEN_WITH_ATTRS        0x80
#define WBXML_TOKEN_WITH_CONTENT      0x40

/* WBXML Versions (XML text) */
#define WBXML_VERSION_TEXT_10   "1.0"   /**< WBXML 1.0 */
#define WBXML_VERSION_TEXT_11   "1.1"   /**< WBXML 1.1 */
#define WBXML_VERSION_TEXT_12   "1.2"   /**< WBXML 1.2 */
#define WBXML_VERSION_TEXT_13   "1.3"   /**< WBXML 1.3 */

/** WBXML Versions (WBXML tokens) */
typedef enum WBXMLVersion_e {
    WBXML_VERSION_UNKNOWN = -1, /**< Unknown WBXML Version */
    WBXML_VERSION_10 = 0x00,    /**< WBXML 1.0 Token */
    WBXML_VERSION_11 = 0x01,    /**< WBXML 1.1 Token */
    WBXML_VERSION_12 = 0x02,    /**< WBXML 1.2 Token */
    WBXML_VERSION_13 = 0x03     /**< WBXML 1.3 Token */
} WBXMLVersion;


/*
 * Possible Compilation Flags:
 * ---------------------------
 *
 *      WBXML_SUPPORT_WML
 *      WBXML_SUPPORT_WTA
 *      WBXML_SUPPORT_SI
 *      WBXML_SUPPORT_SL
 *      WBXML_SUPPORT_CO
 *      WBXML_SUPPORT_PROV
 *      WBXML_SUPPORT_EMN
 *      WBXML_SUPPORT_DRMREL
 *      WBXML_SUPPORT_OTA_SETTINGS
 *      WBXML_SUPPORT_SYNCML
 *      WBXML_SUPPORT_WV 
 *		 WBXML_SUPPORT_DRM2_1
 */
#define WBXML_SUPPORT_DRMREL	/* OMA DRM 1.0 */
#define WBXML_SUPPORT_DRM2_1	/* OMA DRM 2.1 */
#define WBXML_SUPPORT_SRM			/* OMA SRM */

//#define WBXML_LIB_VERBOSE			/* Logging feature */

/* Use srting table for wbxml literal */
#define WBXML_ENCODER_USE_STRTBL

/* wbxml encoder features for supporting DRM */
#if defined ( WBXML_ENCODER_USE_STRTBL )
#define WBXML_DRMEXT	/* Lothson.20081106 */
#endif

#define WBXML_DYNAMIC_CODEPAGE	/* Lothson.20081111 */

/** Supported WBXML Languages */
typedef enum WBXMLLanguage_e {
    WBXML_LANG_UNKNOWN = 0,     /**< Unknown / Not Specified */

#if defined( WBXML_SUPPORT_DRMREL )
    WBXML_LANG_DRMREL10,        /**< DRMREL 1.0 */
#endif /* WBXML_SUPPORT_DRMREL */

#if defined( WBXML_SUPPORT_DRM2_1 )
    WBXML_LANG_OMADRM2_1,        /**< OMA DRM 2.1 Trigger */
#endif /* WBXML_SUPPORT_DRM2_1 */

#if defined( WBXML_SUPPORT_SRM )
    WBXML_LANG_OMASRM,        /**< OMA SRM roContainer */
#endif /* WBXML_SUPPORT_SRM */


} WBXMLLanguage;



/**
 * @brief Type of XML Generation
 * @note Canonical Form is defined here: http://www.jclark.com/xml/canonxml.html
 */
typedef enum WBXMLGenXMLType_e {
    WBXML_GEN_XML_COMPACT = 0,  /**< Compact XML generation */
    WBXML_GEN_XML_INDENT,       /**< Indented XML generation */
    WBXML_GEN_XML_CANONICAL     /**< Canonical XML generation */
} WBXMLGenXMLType;


/**
 * @brief Parameters when generating an XML document
 */
typedef struct WBXMLGenXMLParams_s {
    WBXMLGenXMLType gen_type;    /**< WBXML_GEN_XML_COMPACT | WBXML_GEN_XML_INDENT | WBXML_GEN_XML_CANONICAL (Default: WBXML_GEN_XML_INDENT) */
    WBXMLLanguage lang;          /**< Force document Language (overwrite document Public ID) */
    WB_UTINY indent;             /**< Indentation Delta, when using WBXML_GEN_XML_INDENT Generation Type (Default: 0) */
    WB_BOOL keep_ignorable_ws;   /**< Keep Ignorable Whitespaces (Default: FALSE) */
} WBXMLGenXMLParams;

#if defined( WBXML_DRMEXT )
typedef struct WBXMLNode_s {
	WB_UTINY *pszName;
	struct WBXMLNode_s *next;
} WBXMLNode;
#endif


/**
 * @brief Parameters when generating a WBXML document
 */
typedef struct WBXMLGenWBXMLParams_s {
    WBXMLVersion wbxml_version; /**< WBXML Version */
    WB_BOOL keep_ignorable_ws;  /**< Keep Ignorable Whitespaces (Default: FALSE) */
    WB_BOOL use_strtbl;         /**< Generate String Table (Default: TRUE) */
#if defined( WBXML_DRMEXT )
	WB_BOOL use_strtbl_literal;  /**< Use String Table only for literal (except wbxml string) (Default: FALSE) */

	WB_BOOL record_literals;		/**< Record name of tag and attribute start which are not included in code page (Default: FALSE) */
	WB_BOOL record_attrvalue_literals;		/**< Record attribute values which are not included in code page (Default: FALSE) */

	WB_LONG Tags;
	WBXMLNode *pstTag;

	WB_LONG Atts;
	WBXMLNode *pstAtt;

	WB_LONG AttValues;
	WBXMLNode *pstAttValues;
#endif
} WBXMLGenWBXMLParams;


/** Generic macro to get number of elements in a table */
#define WBXML_TABLE_SIZE(table) ((WB_LONG)(sizeof(table) / sizeof(table[0])))

#if defined( WBXML_DYNAMIC_CODEPAGE )
typedef enum WBXMLTableType_e
{
	eWBXML_TBLTYPE_TAG = 0,
	eWBXML_TBLTYPE_NS,
	eWBXML_TBLTYPE_ATTR,
	eWBXML_TBLTYPE_ATTRVALUE,
	eWBXML_TBLTYPE_EXTVALUE
} WBXMLTableType;
#endif


#if defined( WBXML_BACKWARD_COMPAT )

/* 0.9.2 */
#define WBXML_ENCODER_XML_GEN_COMPACT   WBXML_GEN_XML_COMPACT
#define WBXML_ENCODER_XML_GEN_INDENT    WBXML_GEN_XML_INDENT
#define WBXML_ENCODER_XML_GEN_CANONICAL WBXML_GEN_XML_CANONICAL

#define WBXMLEncoderXMLGenType   WBXMLGenXMLType
#define WBXMLConvWBXML2XMLParams WBXMLGenXMLParams
#define WBXMLConvXML2WBXMLParams WBXMLGenWBXMLParams

// #define WBXMLTag      WBXMLTagName
// #define WBXMLTagEntry WBXMLTagNameEntry

#endif /* WBXML_BACKWARD_COMPAT */


/* We are good coders and we don't want to ignore Warnings :) */
#if 0//def WIN32
#pragma warning(error: 4001) /**< nonstandard extension 'single line comment' was used (disallow "//" C++ comments) */
#pragma warning(error: 4002) /**< too many actual parameters for macro 'identifier' */
#pragma warning(error: 4003) /**< not enough actual parameters for macro 'identifier' */
#pragma warning(error: 4004) /**< incorrect construction after 'defined' */
#pragma warning(error: 4005) /**< 'identifier' : macro redefinition */
#pragma warning(error: 4006) /**< #undef expected an identifier */
#pragma warning(error: 4009) /**< string too big; trailing characters truncated */
#pragma warning(error: 4013) /**< 'function' undefined; assuming extern returning int */
#pragma warning(error: 4015) /**< 'identifier' : type of bit field must be integral */
#pragma warning(error: 4016) /**< 'function' : no function return type; using int as default */
#pragma warning(error: 4018) /**< 'expression' : signed/unsigned mismatch */
#pragma warning(error: 4020) /**< 'function' : too many actual parameters */
#pragma warning(error: 4021) /**< 'function' : too few actual parameters */
#pragma warning(error: 4022) /**< 'function' : pointer mismatch for actual parameter 'number' */
#pragma warning(error: 4023) /**< 'symbol' : based pointer passed to unprototyped function : parameter number */
#pragma warning(error: 4024) /**< 'function' : different types for formal and actual parameter 'number' */
#pragma warning(error: 4025) /**< 'number' : based pointer passed to function with variable arguments: parameter number */
#pragma warning(error: 4026) /**< function declared with formal parameter list */
#pragma warning(error: 4027) /**< function declared without formal parameter list */
#pragma warning(error: 4028) /**< formal parameter 'number' different from declaration */
#pragma warning(error: 4029) /**< declared formal parameter list different from definition */
#pragma warning(error: 4030) /**< first formal parameter list longer than the second list */
#pragma warning(error: 4031) /**< second formal parameter list longer than the first list */
#pragma warning(error: 4033) /**< 'function' must return a value */
#pragma warning(error: 4035) /**< 'function' : no return value */
#pragma warning(error: 4036) /**< unnamed 'type' as actual parameter */
#pragma warning(error: 4045) /**< 'identifier' : array bounds overflow */
#pragma warning(error: 4047) /**< 'identifier1' : 'operator' : different levels of indirection from 'identifier2' */
#pragma warning(error: 4049) /**< compiler limit : terminating line number emission */
#pragma warning(error: 4051) /**< type conversion; possible loss of data */
#pragma warning(error: 4053) /**< one void operand for '?:' */
#pragma warning(error: 4054) /**< 'conversion' : from function pointer 'type1' to data pointer 'type2' */
#pragma warning(error: 4057) /**< 'operator' : 'identifier1' indirection to slightly different base types from 'identifier2' */
#pragma warning(error: 4059) /**< pascal string too big, length byte is length % 256 */
#pragma warning(error: 4061) /**< enumerate 'identifier' in switch of enum 'identifier' is not explicitly handled by a case label */
#pragma warning(error: 4063) /**< case 'identifier' is not a valid value for switch of enum 'identifier' */
#pragma warning(error: 4064) /**< switch of incomplete enum 'identifier' */
#pragma warning(error: 4071) /**< 'function' : no function prototype given */
#pragma warning(error: 4072) /**< 'function' : no function prototype on 'convention' function */
#pragma warning(error: 4078) /**< case constant 'value' too big for the type of the switch expression */
#pragma warning(error: 4081) /**< expected 'token1'; found 'token2' */
#pragma warning(error: 4087) /**< 'function' : declared with 'void' parameter list */
#pragma warning(error: 4088) /**< 'function' : pointer mismatch in actual parameter 'number', formal parameter 'number' */
#pragma warning(error: 4089) /**< 'function' : different types in actual parameter 'number', formal parameter 'number' */
#pragma warning(error: 4098) /**< 'function' : void function returning a value */
#pragma warning(error: 4113) /**< 'identifier1' differs in parameter lists from 'identifier2' */
#pragma warning(error: 4129) /**< 'character' : unrecognized character escape sequence */
#pragma warning(error: 4133) /**< 'type' : incompatible types - from 'type1' to 'type2' */
#pragma warning(error: 4150) /**< deletion of pointer to incomplete type 'type'; no destructor called */
#pragma warning(error: 4172) /**< returning address of local variable or temporary */
#pragma warning(error: 4221) /**< nonstandard extension used : 'identifier' : cannot be initialized using address of automatic variable */
#pragma warning(error: 4223) /**< nonstandard extension used : non-lvalue array converted to pointer */
#pragma warning(error: 4224) /**< nonstandard extension used : formal parameter 'identifier' was previously defined as a type */
#pragma warning(error: 4390) /**< ';' : empty controlled statement found; is this what was intended?" */
#pragma warning(error: 4508) /**< 'function' : function should return a value; void return type assumed */
#pragma warning(error: 4541) /**< 'identifier' used on polymorphic type 'type' with /GR-; unpredictable behavior may result */
#pragma warning(error: 4551) /**< function call missing argument list */
#pragma warning(error: 4553) /**< 'operator' : operator has no effect; did you intend 'operator'? */
#pragma warning(error: 4700) /**< local variable 'name' used without having been initialized */
#pragma warning(error: 4706) /**< assignment within conditional expression */
#pragma warning(error: 4715) /**< 'function' : not all control paths return a value */
#pragma warning(error: 4761) /**< integral size mismatch in argument : conversion supplied */
#endif /* WIN32 */


/* Includes */
#if defined( HAVE_EXPAT)
#include "expat_drm.h" //DRMHDRMERGE "../Expat/expat_drm.h"
#endif /* HAVE_EXPAT */

#if defined( HAVE_ICONV )
#include <iconv.h>
#endif /* HAVE_ICONV */

#include "wbxml_log.h"
#include "wbxml_errors.h"
#include "wbxml_mem.h"
#include "wbxml_lists.h"
#include "wbxml_buffers.h"
#include "wbxml_base64.h"
#include "wbxml_tables.h"
#include "wbxml_elt.h"
#include "wbxml_charset.h"
#include "wbxml_tree.h"
#include "wbxml_tree_clb_wbxml.h"
#include "wbxml_tree_clb_xml.h"
#include "wbxml_handlers.h"
#include "wbxml_parser.h"
#include "wbxml_encoder.h"
#include "wbxml_conv.h"

#if defined( WBXML_WRAPPERS )
#include "wbxml_wrap_syncml.h"
#endif /* WBXML_WRAPPERS */


/*******************************
	Added by lothson  for DRMv2.1 Porting
	20070901
********************************/

/* WBXML Lib memory functions */
extern wbxml_mallocFunc 		wbxml_malloc;
extern wbxml_reallocFunc 		wbxml_realloc;
extern wbxml_freeFunc 			wbxml_free;
extern wbxml_strdupFunc		wbxml_strdup;

/* WBXML Lib string functions */
extern WBXML_STRLENFunc WBXML_STRLEN;
extern WBXML_STRCMPFunc WBXML_STRCMP;
extern WBXML_STRNCMPFunc WBXML_STRNCMP;
extern WBXML_STRSTRFunc WBXML_STRSTR;
extern WBXML_STRCASECMPFunc WBXML_STRCASECMP;
extern WBXML_SPRINTFFunc WBXML_SPRINTFF;

#define WBXML_ISDIGIT(a) isdigit(a)

/** @} */

#endif /* WBXML_H */
