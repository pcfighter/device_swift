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
 * @file wbxml_parser.c
 * @ingroup wbxml_parser
 *
 * @author Aymerick Jehanne <libwbxml@aymerick.com>
 * @date 02/03/12
 *
 * @brief WBXML Parser - Parse a WBXML document and call user defined Callbacks
 *
 * @todo Handle correctly charset
 *
 * @note WBXML Versions Differences:
 *            - WBXML 1.2: - No differences with WBXML 1.3, except a clarification in BNF for 'LITERAL' handling
 *            - WBXML 1.1: - No Switch Page mecanism
 *                         - An Attribute value can't be "opaque"
 *            - WBXML 1.0: - No 'charset' handling
 *                         - No 'opaque' support
 *                         - A strange 'body' rule description in BNF (just forget it).
 */

#include "wbxml.h"


/* Memory management related defines */
#define WBXML_PARSER_MALLOC_BLOCK 5000
#define WBXML_PARSER_STRING_TABLE_MALLOC_BLOCK 200
#define WBXML_PARSER_ATTR_VALUE_MALLOC_BLOCK 100

/** Set it to '1' for Best Effort mode */
#define WBXML_PARSER_BEST_EFFORT 0 // 1 Lothson.20081113

/** For unknown Tag Name or Attribute Name (in Best Effort Mode) */
#define WBXML_PARSER_UNKNOWN_STRING ((WB_UTINY *)"unknown")

/** If you want to modify this define, change the 'entcode' variable length in parse_entity() too please */
#define WBXML_PARSER_MAX_ENTITY_CODE 999999


/**
 * @brief The WBXML Application Token types
 */
typedef enum WBXMLTokenType_e {
    WBXML_TAG_TOKEN,        /**< Tag token */
    WBXML_ATTR_TOKEN        /**< Attribute token */
} WBXMLTokenType;


/**
 * @brief The WBXML Parser
 * @warning For now 'current_tag' field is only used for WV Content Parsing. And for this use, it works.
 *          But this field is reset after End Tag, and as there is no Linked List mecanism, this is bad for
 *          cascading elements: we don't fill this field with parent Tag when parsing End Tag.
 */
struct WBXMLParser_s {
    void                 *user_data;       /**< User Data */
    WBXMLContentHandler  *content_hdl;     /**< Content Handlers Callbacks */
    WBXMLBuffer          *wbxml;           /**< The wbxml we are parsing */    
    WBXMLBuffer          *strstbl;         /**< String Table specified in WBXML document */
    const WBXMLLangEntry *langTable;       /**< Current document Language Table */
    const WBXMLLangEntry *mainTable;       /**< Main WBXML Languages Table */
    const WBXMLTagEntry  *current_tag;     /**< Current Tag */
  
    WBXMLLanguage         lang_forced;     /**< Language forced by User */
    WB_ULONG              public_id;       /**< Public ID specified in WBXML document */    
    WB_LONG               public_id_index; /**< If Public ID is a String Table reference,
                                                this is the index defined in the strtbl */
    WBXMLCharsetMIBEnum   charset;         /**< Charset of WBXML document */
    WBXMLCharsetMIBEnum   meta_charset;    /**< Meta-info provided by user: only used if
                                                Charset is not specified in WBXML document */
    WB_ULONG              pos;             /**< Position of parsing curser in wbxml */    
    WBXMLVersion          version;         /**< WBXML Version field specified in WBXML document */
    WB_UTINY              tagCodePage;     /**< Current Tag Code Page */
    WB_UTINY              attrCodePage;    /**< Current Attribute Code Page */
};



/***************************************************
 *    Private Functions prototypes
 */

/* WBXML Parser functions */
static void wbxml_parser_reinit(WBXMLParser *parser);

/* Check functions */
static WB_BOOL is_token(WBXMLParser *parser, WB_UTINY token);
static WB_BOOL is_literal(WBXMLParser *parser);
static WB_BOOL is_attr_value(WBXMLParser *parser);
static WB_BOOL is_string(WBXMLParser *parser);
static WB_BOOL is_extension(WBXMLParser *parser);
static WB_BOOL check_public_id(WBXMLParser *parser);

/* Parse functions */
static WBXMLError parse_version(WBXMLParser *parser);
static WBXMLError parse_publicid(WBXMLParser *parser);
static WBXMLError parse_charset(WBXMLParser *parser);
static WBXMLError parse_strtbl(WBXMLParser *parser);
static WBXMLError parse_body(WBXMLParser *parser);

static WBXMLError parse_pi(WBXMLParser *parser);
static WBXMLError parse_element(WBXMLParser *parser);
static void free_attrs_table(WBXMLAttribute **attrs);

static WBXMLError parse_switch_page(WBXMLParser *parser, WBXMLTokenType  code_space);
static WBXMLError parse_stag(WBXMLParser *parser, WB_UTINY *tag, WBXMLTag **element);
static WBXMLError parse_tag(WBXMLParser *parser, WB_UTINY *tag, WBXMLTag **element);
static WBXMLError parse_attribute(WBXMLParser *parser, WBXMLAttribute **attr);
static WBXMLError parse_content(WBXMLParser *parser, WBXMLBuffer **result);

static WBXMLError parse_string(WBXMLParser *parser, WBXMLBuffer **result);
static WBXMLError parse_extension(WBXMLParser *parser, WBXMLTokenType code_space, WBXMLBuffer **result);
static WBXMLError parse_entity(WBXMLParser *parser, WBXMLBuffer **result);
static WBXMLError parse_opaque(WBXMLParser *parser, WBXMLBuffer **result);

static WBXMLError parse_literal(WBXMLParser *parser, WB_UTINY *tag, WBXMLBuffer **result);

static WBXMLError parse_attr_start(WBXMLParser *parser, WBXMLAttributeName **name, const WB_UTINY **value);
static WBXMLError parse_attr_value(WBXMLParser *parser, WBXMLBuffer **result);

static WBXMLError parse_termstr(WBXMLParser *parser, WBXMLBuffer **result);
static WBXMLError parse_inline(WBXMLParser *parser, WBXMLBuffer **result);
static WBXMLError parse_tableref(WBXMLParser *parser, WBXMLBuffer **result);
static WBXMLError parse_entcode(WBXMLParser *parser, WB_ULONG *result);

static WBXMLError get_strtbl_reference(WBXMLParser *parser, WB_ULONG index, WBXMLBuffer **result);

/* Basic Types Parse functions */
static WBXMLError parse_uint8(WBXMLParser *parser, WB_UTINY *result);
static WBXMLError parse_mb_uint32(WBXMLParser *parser, WB_ULONG *result);

/* Language Specific Decoding Functions */
static WBXMLError decode_opaque_content(WBXMLParser *parser, WBXMLBuffer **data);

#if defined( WBXML_SUPPORT_DRMREL )
static WBXMLError decode_drmrel_keyvalue(WBXMLBuffer **data);
#endif /* WBXML_SUPPORT_DRMREL */

/* Macro for error handling */
#define CHECK_ERROR if (ret != WBXML_OK) return ret;


/***************************************************
 *    Public Functions
 */

WBXMLParser * wbxml_parser_create(void)
{
    WBXMLParser *parser = NULL;

    parser = (WBXMLParser *) wbxml_malloc(sizeof(WBXMLParser));
    if (parser == NULL) {
        return NULL;
    }
    
    parser->wbxml = NULL;
    parser->user_data = NULL;
    parser->content_hdl = NULL;
    parser->strstbl = NULL;
    parser->langTable = NULL;

    /* Default Main WBXML Languages Table */
    parser->mainTable = wbxml_tables_get_main();

    parser->current_tag = NULL;

    parser->lang_forced = WBXML_LANG_UNKNOWN;
    parser->public_id = WBXML_PUBLIC_ID_UNKNOWN;    
    parser->public_id_index = -1;
    parser->charset = WBXML_CHARSET_UNKNOWN;
    parser->meta_charset = WBXML_CHARSET_UNKNOWN;
    parser->version = WBXML_VERSION_UNKNOWN;    
    
    parser->pos = 0;
    parser->tagCodePage = 0;
    parser->attrCodePage = 0;

    return parser;
}


void wbxml_parser_destroy(WBXMLParser *parser)
{
    if (parser == NULL)
        return;
    
    wbxml_buffer_destroy(parser->wbxml);
    wbxml_buffer_destroy(parser->strstbl);

    wbxml_free(parser);
}


WBXMLError wbxml_parser_parse(WBXMLParser *parser, WB_UTINY *wbxml, WB_ULONG wbxml_len)
{
    WBXMLError ret = WBXML_OK;

    if (parser == NULL)
        return WBXML_ERROR_NULL_PARSER;

    if ((wbxml == NULL) || (wbxml_len <= 0))
        return WBXML_ERROR_EMPTY_WBXML;

    /* Reinitialize WBXML Parser */
    wbxml_parser_reinit(parser);

    parser->wbxml = wbxml_buffer_create(wbxml, wbxml_len, WBXML_PARSER_MALLOC_BLOCK);
    if (parser->wbxml == NULL)
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;

    /* WBXML Version */
    ret = parse_version(parser);
    CHECK_ERROR

    if ((WB_UTINY)parser->version > WBXML_VERSION_13)
        WBXML_WARNING((WBXML_PARSER, "This library only supports WBXML " WBXML_VERSION_TEXT_13));

    /* WBXML Public ID */
    ret = parse_publicid(parser);
    CHECK_ERROR

    /* Ignore Document Public ID if user has forced use of another Public ID */
    if (parser->lang_forced != WBXML_LANG_UNKNOWN)
        parser->public_id = wbxml_tables_get_wbxml_publicid(wbxml_tables_get_main(), parser->lang_forced);

    /* No charset in WBXML 1.0 */
    if (parser->version != WBXML_VERSION_10) {
        ret = parse_charset(parser);
        CHECK_ERROR
    }

    /* Check charset */
    if (parser->charset == WBXML_CHARSET_UNKNOWN) {
        if (parser->meta_charset != WBXML_CHARSET_UNKNOWN) {
            /* Use meta-information provided by user */
            parser->charset = parser->meta_charset;
      
            WBXML_DEBUG((WBXML_PARSER,
                        "Using provided meta charset: %ld",
                        parser->meta_charset));
        }
        else {
            /* Default Charset Encoding: UTF-8 */
            parser->charset = WBXML_PARSER_DEFAULT_CHARSET;
      
            WBXML_WARNING((WBXML_PARSER,
                           "No charset information found, using default : %s",
                           WBXML_PARSER_DEFAULT_CHARSET));
        }
    }

    /* WBXML String Table */
    ret = parse_strtbl(parser);
    CHECK_ERROR

    /* Now that we have parsed String Table, we can check Public ID */
    if (!check_public_id(parser)) {
        WBXML_ERROR((WBXML_PARSER, "PublicID not found"));
        return WBXML_ERROR_UNKNOWN_PUBLIC_ID;
    }

    /* Call to WBXMLStartDocumentHandler */
    if ((parser->content_hdl != NULL) && (parser->content_hdl->start_document_clb != NULL))
        parser->content_hdl->start_document_clb(parser->user_data, parser->charset, parser->langTable);

    /* WBXML Body */
    ret = parse_body(parser);
    CHECK_ERROR

    /* Call to WBXMLEndDocumentHandler */
    if ((parser->content_hdl != NULL) && (parser->content_hdl->end_document_clb != NULL))
        parser->content_hdl->end_document_clb(parser->user_data);

    return ret;
}


void wbxml_parser_set_user_data(WBXMLParser *parser, void *user_data)
{
    if (parser != NULL)
        parser->user_data = user_data;
}


void wbxml_parser_set_content_handler(WBXMLParser *parser, WBXMLContentHandler *content_handler)
{
    if (parser != NULL)
        parser->content_hdl = content_handler;
}


void wbxml_parser_set_main_table(WBXMLParser *parser, const WBXMLLangEntry *main_table)
{
    if (parser != NULL)
        parser->mainTable = main_table;
}


WB_BOOL wbxml_parser_set_language(WBXMLParser *parser, WBXMLLanguage lang)
{
    if (parser != NULL) {
        parser->lang_forced = lang;
        return TRUE;
    }

    return FALSE;
}


WB_BOOL wbxml_parser_set_meta_charset(WBXMLParser *parser,
                                                     WBXMLCharsetMIBEnum charset)
{
    if ( parser != NULL ) {
        parser->meta_charset = charset;
        return TRUE;
    }
  
    return FALSE;
}


WB_ULONG wbxml_parser_get_wbxml_public_id(WBXMLParser *parser)
{
    if ((parser != NULL) && (parser->langTable != NULL) && (parser->langTable->publicID != NULL))
        return parser->langTable->publicID->wbxmlPublicID;
    else
        return WBXML_PUBLIC_ID_UNKNOWN;
}


const WB_UTINY * wbxml_parser_get_xml_public_id(WBXMLParser *parser)
{
    if ((parser != NULL) && (parser->langTable != NULL) && (parser->langTable->publicID != NULL))
        return (const WB_UTINY *) parser->langTable->publicID->xmlPublicID;
    else
        return NULL;
}


WBXMLVersion wbxml_parser_get_wbxml_version(WBXMLParser *parser)
{
    if (parser != NULL)
        return parser->version;
    else
        return WBXML_VERSION_UNKNOWN;
}


WB_LONG wbxml_parser_get_current_byte_index(WBXMLParser *parser)
{
    if (parser != NULL)
        return parser->pos - 1;
    else
        return 0;
}


/***************************************************
 *    Private Functions
 */

/**************************
 * WBXML Parser functions
 */

/**
 * @brief Reinitialize a WBXML Parser
 * @param parser The WBXMLParser to reinitialize
 * @note Only reinitialize internal fields of parser, and so keep User Data
 *         and Content Handler pointers.
 */
static void wbxml_parser_reinit(WBXMLParser *parser)
{
    if (parser == NULL)
        return;

    wbxml_buffer_destroy(parser->wbxml);
    parser->wbxml           = NULL;
  
    wbxml_buffer_destroy(parser->strstbl);
    parser->strstbl         = NULL;
  
    parser->langTable       = NULL;
    parser->current_tag     = NULL;
  
    parser->public_id       = WBXML_PUBLIC_ID_UNKNOWN;    
    parser->public_id_index = -1;
    parser->charset         = WBXML_CHARSET_UNKNOWN;
    parser->meta_charset    = WBXML_CHARSET_UNKNOWN;
    parser->version         = WBXML_VERSION_UNKNOWN;    
  
    parser->pos             = 0;
    parser->tagCodePage     = 0;
    parser->attrCodePage    = 0;    
}


/******************
 * Check functions
 */

/**
 * @brief Check if current byte a specified WBXML token
 * @param parser The WBXML Parser
 * @param token The WBXML token
 * @return TRUE is current byte is the specified token, FALSE otherwise
 */
static WB_BOOL is_token(WBXMLParser *parser, WB_UTINY token)
{
    WB_UTINY result;

    if (!wbxml_buffer_get_char(parser->wbxml, parser->pos, &result))
        return FALSE;

    return (WB_BOOL) (result == token);
}


/**
 * @brief Check if current byte is a WBXML literalTag token
 * @param parser The WBXML Parser
 * @return TRUE is current byte is a literalTag token, FALSE otherwise
 */
static WB_BOOL is_literal(WBXMLParser *parser)
{
    WB_UTINY result;

    if (!wbxml_buffer_get_char(parser->wbxml, parser->pos, &result))
        return FALSE;

    return (WB_BOOL) ((result == WBXML_LITERAL) || (result == WBXML_LITERAL_A) || (result == WBXML_LITERAL_C) || (result == WBXML_LITERAL_AC));
}


/**
 * @brief Check if next token to parse is an Attribute Value
 * @param parser The WBXML Parser
 * @return TRUE if next token to parse is an Attribute Value, FALSE otherwise
 * @note attrValue    = ([switchPage] ATTRVALUE | string | extension | entity | opaque)
 */
static WB_BOOL is_attr_value(WBXMLParser *parser)
{
    WB_UTINY cur_byte, next_byte;

    /* Get current byte */
    if (!wbxml_buffer_get_char(parser->wbxml, parser->pos, &cur_byte))
        return FALSE;

    /* If current byte is a switch page, check that following token is an Attribute Value token */
    if (is_token(parser, WBXML_SWITCH_PAGE)) {
        if (!wbxml_buffer_get_char(parser->wbxml, parser->pos + 2, &next_byte))
            return FALSE;

        /* Attribute Value is greater than or equal to 128 */
        if ((next_byte & 0x80) == 0x80)
            return TRUE;
    }

    /* Else, check current byte is an Attribute Value, a string, an extension, an entity or an opaque */
    if (((cur_byte & 0x80) == 0x80) ||
        (is_string(parser)) ||
        (is_extension(parser)) ||
        (is_token(parser, WBXML_ENTITY)) ||
        (is_token(parser, WBXML_OPAQUE)))
        return TRUE;

    return FALSE;
}


/**
 * @brief Check if current byte is a string
 * @param parser The WBXML Parser
 * @return TRUE if current byte is a string, FALSE otherwise
 */
static WB_BOOL is_string(WBXMLParser *parser)
{
    return (WB_BOOL) (is_token(parser, WBXML_STR_I) || is_token(parser, WBXML_STR_T));
}


/**
 * @brief Check if current byte is an extension
 * @param parser The WBXML Parser
 * @return TRUE if current byte is an extension, FALSE otherwise
 */
static WB_BOOL is_extension(WBXMLParser *parser)
{
    WB_UTINY cur_byte;

    /* If current byte is a switch page, check the following token */
    if (is_token(parser, WBXML_SWITCH_PAGE)) {
        if (!wbxml_buffer_get_char(parser->wbxml, parser->pos + 2, &cur_byte))
            return FALSE;
    }
    else {
        if (!wbxml_buffer_get_char(parser->wbxml, parser->pos, &cur_byte))
            return FALSE;
    }

    return (WB_BOOL) ((cur_byte == WBXML_EXT_I_0) || (cur_byte == WBXML_EXT_I_1) || (cur_byte == WBXML_EXT_I_2) ||
            (cur_byte == WBXML_EXT_T_0) || (cur_byte == WBXML_EXT_T_1) || (cur_byte == WBXML_EXT_T_2) ||
            (cur_byte == WBXML_EXT_0)   || (cur_byte == WBXML_EXT_1)   || (cur_byte == WBXML_EXT_2));
}


/**
 * @brief Check the Public ID
 * @param parser The WBXML Parser
 * @return TRUE if Public ID is found, FALSE otherwise
 */
static WB_BOOL check_public_id(WBXMLParser *parser)
{
    WBXMLBuffer *public_id = NULL;
    WB_LONG          index = 0;

    WBXML_DEBUG((WBXML_PARSER, "\t  Checking PublicID"));

    /* First check if we can figure out the Public ID */
    if ((parser->lang_forced == WBXML_LANG_UNKNOWN) && 
        (parser->public_id == WBXML_PUBLIC_ID_UNKNOWN) && 
        (parser->public_id_index == -1))
    {
        return FALSE;
    }


    /********************************************************
     * Case 1: Language is forced by user
     */

    if (parser->lang_forced != WBXML_LANG_UNKNOWN) {
        /* Search Language Entry */
        while (parser->mainTable[index].langID != -1) {
            if (parser->mainTable[index].langID == parser->lang_forced) {
                parser->langTable = &(parser->mainTable[index]);

                WBXML_DEBUG((WBXML_PARSER, "\t  Language Forced - PublicID : '%s'", parser->mainTable[index].publicID->xmlPublicID));

                return TRUE;
            }

            index++;
        }
    }


    /********************************************************
     * Case 2: Public ID is a normal token 
     *         (found in WBXML Document, or forced by user)
     */

    if (parser->public_id != WBXML_PUBLIC_ID_UNKNOWN) {
        WBXML_DEBUG((WBXML_PARSER, "\t  PublicID token: 0x%X", parser->public_id));

        /* Search Public ID Table */
        while (parser->mainTable[index].publicID != NULL) {
            if (parser->mainTable[index].publicID->wbxmlPublicID == parser->public_id) {
                parser->langTable = &(parser->mainTable[index]);

                WBXML_DEBUG((WBXML_PARSER, "\t  PublicID : '%s'", parser->mainTable[index].publicID->xmlPublicID));

                return TRUE;
            }

            index++;
        }
    }


    /********************************************************
     * Case 3: Public ID referenced in String Table 
     */
    if (parser->public_id_index != -1) {
        WBXML_DEBUG((WBXML_PARSER, "\t  PublicID is in String Table (index: 0x%X)", parser->public_id_index));

        if (get_strtbl_reference(parser, (WB_ULONG) parser->public_id_index, &public_id) != WBXML_OK) {
            WBXML_ERROR((WBXML_PARSER, "Bad publicID reference in string table"));
            return FALSE;
        }

        WBXML_DEBUG((WBXML_PARSER, "\t  PublicID : '%s'", wbxml_buffer_get_cstr(public_id)));

        /* Search Public ID Table */
        while (parser->mainTable[index].publicID != NULL)
        {
            if ((parser->mainTable[index].publicID->xmlPublicID != NULL) && 
                (WBXML_STRCASECMP((const WB_TINY *)parser->mainTable[index].publicID->xmlPublicID, (const WB_TINY *)wbxml_buffer_get_cstr(public_id)) == 0))
            {
                parser->langTable = &(parser->mainTable[index]);
                /* parser->public_id = parser->mainTable[index].publicID->wbxmlPublicID; */

                wbxml_buffer_destroy(public_id);
                return TRUE;
            }

            index++;
        }

        /* Clean up */
        wbxml_buffer_destroy(public_id);
    }

    /* Public ID not found in Tables */
    return FALSE;
}



/***************************
 *    WBXML Parse functions
 */

/**
 * @brief Parse WBXML version
 * @param parser The WBXML Parser
 * @return WBXML_OK if parsing is OK, an error code otherwise
 * @note version = u_int8
 */
static WBXMLError parse_version(WBXMLParser *parser)
{   
    WBXMLError ret = WBXML_OK;
    
    /* Initialize version: 1.0 
     *
     * Do NOT keep 'WBXML_VERSION_UNKNOWN' (0xffffffff) because only one byte will change.
     * (for example, if the version is 0x02, then parser->version will be 0xffffff02)
     */
    parser->version = WBXML_VERSION_10;
    
    if ((ret = parse_uint8(parser, (WB_UTINY*) &parser->version)) != WBXML_OK)
        return ret;

    WBXML_DEBUG((WBXML_PARSER, "(%d) Parsed version: '0x%X'", parser->pos - 1, (WB_TINY) parser->version));

    return WBXML_OK;
}


/**
 * @brief Parse WBXML public id
 * @param parser The WBXML Parser
 * @return WBXML_OK if parsing is OK, an error code otherwise
 * @note publicid = mb_u_int32 | ( zero index )
 * @note index = mb_u_int32
 */
static WBXMLError parse_publicid(WBXMLParser *parser)
{
    WB_UTINY public_id;

    if (!wbxml_buffer_get_char(parser->wbxml, parser->pos, &public_id))
        return WBXML_ERROR_END_OF_BUFFER;

    WBXML_DEBUG((WBXML_PARSER, "(%d) Parsed publicid: '0x%X'", parser->pos, public_id));

    if (public_id == 0x00) {
        parser->pos++;

        /* Get index (we will retreive the Public ID latter) */
        return parse_mb_uint32(parser, (WB_ULONG *)&parser->public_id_index);
    }
    else {
        /* Get Public ID */
        return parse_mb_uint32(parser, &parser->public_id);        
    }
}


/**
 * @brief Parse WBXML charset
 * @param parser The WBXML Parser
 * @return WBXML_OK if parsing is OK, an error code otherwise
 * @note charset = mb_u_int32
 * @note "The binary XML format contains a representation of the XML document character encoding.
 *        This is the WBXML equivalent of the XML document format encoding attribute,
 *        which is specified in the ?xml processing instruction. The character set is encoded as
 *        a multi-byte positive integer value, representing the IANA-assigned MIB number for
 *        a character set. A value of zero indicates an unknown document encoding. In the case of
 *        an unknown encoding, transport meta-information should be used to determine the character
 *        encoding. If transport meta-information is unavailable, the default encoding of UTF-8
 *        should be assumed."
 */
static WBXMLError parse_charset(WBXMLParser *parser)
{
#if defined( WBXML_LIB_VERBOSE )
    WB_ULONG startpos = parser->pos;
#endif /* WBXML_LIB_VERBOSE */

    WBXMLError ret = parse_mb_uint32(parser, (WB_ULONG *)&parser->charset);

    WBXML_DEBUG((WBXML_PARSER, "(%d) Parsed charset: '0x%X'", startpos, parser->charset));

    return ret;
}


/**
 * @brief Parse WBXML string table
 * @param parser The WBXML Parser
 * @return WBXML_OK if parsing is OK, an error code otherwise
 * @note strtbl = length *byte
 * @note length = mb_u_int32
 */
static WBXMLError parse_strtbl(WBXMLParser *parser)
{
    WB_UTINY  *data       = NULL;
    WB_ULONG   strtbl_len = 0;
    WB_UTINY   end_char   = 0;
    WBXMLError ret        = WBXML_OK;

    WBXML_DEBUG((WBXML_PARSER, "(%d) Parsing strtbl", parser->pos));

    /* Get String Table Length */
    ret = parse_mb_uint32(parser, &strtbl_len);
    if (ret != WBXML_OK)
        return WBXML_ERROR_END_OF_BUFFER;

    if (strtbl_len > 0) {
        /* Check this string table length */
        if (parser->pos + strtbl_len > wbxml_buffer_len(parser->wbxml))
            return WBXML_ERROR_STRTBL_LENGTH;

        /* Get String Table */
        data = wbxml_buffer_get_cstr(parser->wbxml);
        parser->strstbl = wbxml_buffer_create(data + parser->pos, strtbl_len, WBXML_PARSER_STRING_TABLE_MALLOC_BLOCK);
        if (parser->strstbl == NULL)
            return WBXML_ERROR_NOT_ENOUGH_MEMORY;

        /** @todo Damned ! Check the charset ! This may not be a simple NULL terminated string ! */

        /* Some phones doesn't terminate the String Table with a null char... let's correct this */
        if (!wbxml_buffer_get_char(parser->strstbl,
                                   wbxml_buffer_len(parser->strstbl) - 1,
                                   &end_char))
        {
            return WBXML_ERROR_INTERNAL;
        }

        if (end_char != '\0') {
            /* Append NULL char to end of String Table */
            if (!wbxml_buffer_append_char(parser->strstbl, '\0'))
                return WBXML_ERROR_NOT_ENOUGH_MEMORY;
        }

        parser->pos = parser->pos + strtbl_len;
    }

    return WBXML_OK;
}


/**
 * @brief Parse WBXML body
 * @param parser The WBXML Parser
 * @return WBXML_OK if parsing is OK, an error code otherwise
 * @note body = *pi element *pi
 */
static WBXMLError parse_body(WBXMLParser *parser)
{
    WBXMLError ret = WBXML_OK;

    WBXML_DEBUG((WBXML_PARSER, "(%d) Parsing body", parser->pos));

    while (is_token(parser, WBXML_PI)) {
        if ((ret = parse_pi(parser)) != WBXML_OK)
            return ret;
    }

    if ((ret = parse_element(parser)) != WBXML_OK)
        return ret;

    while (is_token(parser, WBXML_PI)) {
        if ((ret = parse_pi(parser)) != WBXML_OK)
            return ret;
    }

    return WBXML_OK;
}


/**
 * @brief Parse WBXML pi
 * @param parser The WBXML Parser
 * @return WBXML_OK if parsing is OK, an error code otherwise
 * @note pi = PI attrStart *attrValue END
 */
static WBXMLError parse_pi(WBXMLParser *parser)
{
    WBXMLAttributeName *attr_name    = NULL;
    const WB_UTINY     *start_value  = NULL;
    WBXMLBuffer        *attr_value   = NULL;
    WBXMLBuffer        *tmp_value    = NULL;
    WBXMLError          ret          = WBXML_OK;
  
    WBXML_DEBUG((WBXML_PARSER, "(%d) Parsing PI", parser->pos));
  
    /* Skip PI */
    parser->pos++;    
      
    /* Parse attrStart */
    if ((ret = parse_attr_start(parser,
                                &attr_name,
                                &start_value)) != WBXML_OK)
    {
        return ret;
    }
      
    if (start_value != NULL ) {
        /* Create a buffer from attribute start value */
        attr_value = wbxml_buffer_create(start_value,
                                         WBXML_STRLEN((const WB_TINY *)start_value),
                                         WBXML_PARSER_ATTR_VALUE_MALLOC_BLOCK);
    }
    else {
        /* Create an empty buffer */
        attr_value = wbxml_buffer_create(NULL,
                                         0,
                                         WBXML_PARSER_ATTR_VALUE_MALLOC_BLOCK);
    }
      
    if (attr_value == NULL) {
        /* Memory error */
        wbxml_attribute_name_destroy(attr_name);
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;
    } 
    
    
    /* Parse *attrValue */
    while (!is_token(parser, WBXML_END)) {
        /* Parse attrValue */
        if ((ret = parse_attr_value(parser, &tmp_value)) != WBXML_OK) {
            wbxml_attribute_name_destroy(attr_name);
            wbxml_buffer_destroy(attr_value);
            return ret;
        }
    
        /* Append to main attribute value buffer */
        if (!wbxml_buffer_append(attr_value, tmp_value)) {
            wbxml_attribute_name_destroy(attr_name);
            wbxml_buffer_destroy(attr_value);
            wbxml_buffer_destroy(tmp_value);
            return WBXML_ERROR_NOT_ENOUGH_MEMORY;
        }
        
        wbxml_buffer_destroy(tmp_value);
        tmp_value = NULL;
    }
      
    /* Skip END */
    parser->pos++;
      
    /* Append NULL char to attr value */
    if (wbxml_buffer_len(attr_value) > 0) {
        if (!wbxml_buffer_append_char(attr_value, '\0')) {
            wbxml_attribute_name_destroy(attr_name);
            wbxml_buffer_destroy(attr_value);

            return WBXML_ERROR_NOT_ENOUGH_MEMORY;
        }
    }
      
    /* Callback WBXMLProcessingInstructionHandler */
    if ((parser->content_hdl != NULL) &&
        (parser->content_hdl->pi_clb != NULL))
    {
        parser->content_hdl->pi_clb(parser->user_data,
                                    wbxml_attribute_name_get_xml_name(attr_name),
                                    wbxml_buffer_get_cstr(attr_value));
    }
        
    wbxml_attribute_name_destroy(attr_name);
    wbxml_buffer_destroy(attr_value);
      
    return WBXML_OK;
}


/**
 * @brief Parse WBXML element
 * @param parser The WBXML Parser
 * @return WBXML_OK if parsing is OK, an error code otherwise
 * @note element = ([switchPage] stag) [ 1*attribute END ] [ *content END ]
 */
static WBXMLError parse_element(WBXMLParser *parser)
{
    WBXMLTag        *element        = NULL;
    WBXMLAttribute  *attr           = NULL;
    WBXMLAttribute **attrs          = NULL;
    WBXMLBuffer     *content        = NULL;
  
    WB_ULONG         attrs_nb       = 0;
    WBXMLError       ret            = WBXML_OK;
    WB_UTINY         tag            = 0;
    WB_BOOL          is_empty       = FALSE;
  
    WBXML_DEBUG((WBXML_PARSER, "(%d) Parsing element", parser->pos));
  
    if (is_token(parser, WBXML_SWITCH_PAGE)) {
        if ((ret = parse_switch_page(parser, WBXML_TAG_TOKEN)) != WBXML_OK) {
            return ret;
        }
    }
  
    /* Parse Tag */
    if ((ret = parse_stag(parser, &tag, &element)) != WBXML_OK ) {
        return ret;
    }
  
    /* Set Current Tag */
    if (element->type == WBXML_VALUE_TOKEN) {
        parser->current_tag = element->u.token;
    }
  
    /* Parse Attributes */
    if (tag & WBXML_TOKEN_WITH_ATTRS) {
        WBXML_DEBUG((WBXML_PARSER, "(%d) Parsing attributes", parser->pos));
    
        /* There must be at least one attribute */
        do {
            /* Parse attribute */
            if ((ret = parse_attribute(parser, &attr)) != WBXML_OK) {
                wbxml_tag_destroy(element);
                free_attrs_table(attrs);
                return ret;
            }
          
            /* Append this attribute in WBXMLAttribute **attrs table */
            attrs_nb++;
    
            if ((attrs = wbxml_realloc(attrs,
                                        (attrs_nb + 1) * sizeof(*attrs))) == NULL)
            {
                /* Clean-up */
                wbxml_tag_destroy(element);
                wbxml_attribute_destroy(attr);
                free_attrs_table(attrs);
                return WBXML_ERROR_NOT_ENOUGH_MEMORY;
            }
    
            attrs[(attrs_nb - 1)] = attr;
            attrs[attrs_nb] = NULL;
        } while ( !is_token(parser, WBXML_END) );
        
        /* Skip END */
        parser->pos++;
    }
    
    /* Is it an empty element ? */
    is_empty = (WB_BOOL) !(tag & WBXML_TOKEN_WITH_CONTENT);
      
    /* Callback WBXMLStartElementHandler */
    if ((parser->content_hdl != NULL) &&
        (parser->content_hdl->start_element_clb != NULL))
    {
        parser->content_hdl->start_element_clb(parser->user_data,
                                               element,
                                               attrs,
                                               is_empty);
    }
      
    /* Free Attributes */
    free_attrs_table(attrs);
    
      
    /* Parse *content */
    if (!is_empty) {
        /* There can be NO content */
        while (!is_token(parser, WBXML_END)) {
            /* Parse content */
            if ((ret = parse_content(parser, &content)) != WBXML_OK)
            {
                wbxml_tag_destroy(element);
                return ret;
            }
    
            /* Callback WBXMLCharactersHandler if content is not NULL */
            if ((content != NULL) &&
                (wbxml_buffer_len(content) != 0) &&
                (parser->content_hdl != NULL) &&
                (parser->content_hdl->characters_clb != NULL))
            {
                parser->content_hdl->characters_clb(parser->user_data,
                                                    wbxml_buffer_get_cstr(content),
                                                    0,
                                                    wbxml_buffer_len(content));
            }
          
            /* Free content */
            wbxml_buffer_destroy(content);
            content = NULL;
        }
        
        WBXML_DEBUG((WBXML_PARSER, "(%d) End of Element", parser->pos));
        
        /* Skip END */
        parser->pos++;
    }
      
    /* Callback WBXMLEndElementHandler */
    if ((parser->content_hdl != NULL) &&
        (parser->content_hdl->end_element_clb != NULL))
    {
        parser->content_hdl->end_element_clb(parser->user_data,
                                             element,
                                             is_empty);
    }
      
    /* Free Tag */
    wbxml_tag_destroy(element);
      
    /* Reset Current Tag */
    parser->current_tag = NULL;
      
    return WBXML_OK;
}


/**
 * @brief Free a (WBXMLAttribute *) table
 * @param attrs The table to ree
 */
static void free_attrs_table(WBXMLAttribute **attrs)
{
    WB_LONG i = 0;

    if (attrs != NULL) {
        while (attrs[i] != NULL) {
            /* Free attribute */
            wbxml_attribute_destroy(attrs[i++]);
        }
        wbxml_free(attrs);
    }
}


/**
 * @brief Parse WBXML switchPage
 * @param parser The WBXML Parser
 * @param code_space The token code space
 * @return WBXML_OK if parsing is OK, an error code otherwise
 * @note switchPage = SWITCH_PAGE pageindex
 */
static WBXMLError parse_switch_page(WBXMLParser *parser, WBXMLTokenType code_space)
{
    WBXML_DEBUG((WBXML_PARSER, "(%d) Parsing switchPage", parser->pos));

    if ((WB_UTINY) parser->version < (WB_UTINY) WBXML_VERSION_12)
        WBXML_WARNING((WBXML_PARSER, "No Switch Page mecanism possible in WBXML < %s", WBXML_VERSION_TEXT_12));

    /* Skip SWITCH_PAGE token */
    parser->pos++;

    /* Change Code Page in correct Code Space */
    if (code_space == WBXML_TAG_TOKEN)
        return parse_uint8(parser, &parser->tagCodePage);
    else
        if (code_space == WBXML_ATTR_TOKEN)
            return parse_uint8(parser, &parser->attrCodePage);
        else
            return WBXML_ERROR_INTERNAL;
}


/**
 * @brief Parse WBXML stag
 * @param parser The WBXML Parser
 * @param tag The parsed tag token
 * @param element The parsed element corresponding to token
 * @return WBXML_OK if parsing is OK, an error code otherwise
 * @note stag = TAG | (literalTag index)
 */
static WBXMLError parse_stag(WBXMLParser *parser, WB_UTINY *tag, WBXMLTag **element)
{
    WBXMLBuffer *name = NULL;
    WBXMLError   ret  = WBXML_OK;
  
    WBXML_DEBUG((WBXML_PARSER, "(%d) Parsing stag", parser->pos));
  
    if (is_literal(parser)) {
        /* Parse '(literalTag index)' */
        if ((ret = parse_literal(parser, tag, &name)) != WBXML_OK) {
            return ret;
        }
    
        /* Create Element Tag */
        if ((*element = wbxml_tag_create_literal(wbxml_buffer_get_cstr(name))) == NULL) {
            ret = WBXML_ERROR_NOT_ENOUGH_MEMORY;
        }

        wbxml_buffer_destroy(name);
        return ret;
    }

    /* Parse 'TAG' */
    return parse_tag(parser, tag, element);
}


/**
 * @brief Parse WBXML Application Token (tag)
 * @param parser The WBXML Parser
 * @param tag The parsed token tag
 * @param element The parsed element (the text element corresponding to token)
 * @return WBXML_OK if parsing is OK, an error code otherwise
 */
static WBXMLError parse_tag(WBXMLParser *parser, WB_UTINY *tag, WBXMLTag **element)
{
    WB_ULONG index = 0;
    WB_UTINY token;
    WBXMLError ret = WBXML_OK;

    /* Parse UINT8 */
    ret = parse_uint8(parser, tag);
    if (ret != WBXML_OK)
        return ret;

    /* Remove ATTR and CONTENT bits */
    token = (WB_UTINY) (*tag & WBXML_TOKEN_MASK);

    /* Search tag in Tags Table */
    if (parser->langTable == NULL)
        return WBXML_ERROR_LANG_TABLE_UNDEFINED;

    if (parser->langTable->tagTable == NULL)
        return WBXML_ERROR_TAG_TABLE_UNDEFINED;

    
    while ((parser->langTable->tagTable[index].xmlName != NULL) &&
           ((parser->langTable->tagTable[index].wbxmlToken != token) ||
            (parser->langTable->tagTable[index].wbxmlCodePage != parser->tagCodePage))) {
        index++;
    }


    if (parser->langTable->tagTable[index].xmlName == NULL) {
#if WBXML_PARSER_BEST_EFFORT
        /* Create "unknown" Tag Element */
        if ((*element = wbxml_tag_create_literal(WBXML_PARSER_UNKNOWN_STRING)) == NULL)
            return WBXML_ERROR_NOT_ENOUGH_MEMORY;

        return WBXML_OK;
#else
        return WBXML_ERROR_UNKNOWN_TAG;
#endif /* WBXML_PARSER_BEST_EFFORT */
    }

    if ((*element = wbxml_tag_create(WBXML_VALUE_TOKEN)) == NULL)
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;

    (*element)->u.token = &(parser->langTable->tagTable[index]);

    WBXML_DEBUG((WBXML_PARSER, "(%d) Token: 0x%X", parser->pos - 1, token));
    
    return WBXML_OK;
}


/**
 * @brief Parse WBXML attribute
 * @param parser The WBXML Parser
 * @param attr The resulting attribute parsed
 * @return WBXML_OK if parsing is OK, an error code otherwise
 * @note attribute = attrStart *attrValue
 * @warning The attr_value parameter MUST be freed by caller
 */
static WBXMLError parse_attribute(WBXMLParser *parser, WBXMLAttribute **attr)
{
    WBXMLAttributeName *attr_name    = NULL;
    const WB_UTINY     *start_value  = NULL;
    WBXMLBuffer        *attr_value   = NULL;
    WBXMLBuffer        *tmp_value    = NULL;
    WBXMLError          ret          = WBXML_OK;
  
    WBXML_DEBUG((WBXML_PARSER, "(%d) Parsing attribute", parser->pos));
  
    /* Parse attrStart */
    if ((ret = parse_attr_start(parser,
                                &attr_name,
                                &start_value)) != WBXML_OK)
    {
        return ret;
    }
  
    if ( start_value != NULL ) {
        /* Create a buffer from attribute start value */
        attr_value = wbxml_buffer_create(start_value,
                                         WBXML_STRLEN((const WB_TINY *)start_value),
                                         WBXML_PARSER_ATTR_VALUE_MALLOC_BLOCK);
    }
    else {
        /* Create an empty buffer */
        attr_value = wbxml_buffer_create(NULL,
                                         0,
                                         WBXML_PARSER_ATTR_VALUE_MALLOC_BLOCK);
    }
  
    if (attr_value == NULL) {
        /* Memory error */
        wbxml_attribute_name_destroy(attr_name);        
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;
    }
  
    /* Construct Attribute Value */
    while (is_attr_value(parser)) {
        /* Parse attrValue */
        if ((ret = parse_attr_value(parser, &tmp_value)) != WBXML_OK) {
            wbxml_attribute_name_destroy(attr_name);
            wbxml_buffer_destroy(attr_value);
            return ret;
        }
    
        if (!wbxml_buffer_append(attr_value, tmp_value)) {
            wbxml_attribute_name_destroy(attr_name);
            wbxml_buffer_destroy(attr_value);
            wbxml_buffer_destroy(tmp_value);
            return WBXML_ERROR_NOT_ENOUGH_MEMORY;
        }
    
        wbxml_buffer_destroy(tmp_value);
        tmp_value = NULL;
    }
  
    if ((wbxml_buffer_len(attr_value) > 0) &&
        (attr_name->type == WBXML_VALUE_TOKEN)) 
    {
        /* Handle Language Specific Attribute Values */
        switch (parser->langTable->langID) {    
        default:
          break;
        }
    }
  
    /* Append NULL char to attr value */
    if (wbxml_buffer_len(attr_value) > 0) {
        if (!wbxml_buffer_append_char(attr_value, '\0')) {
            wbxml_attribute_name_destroy(attr_name);
            wbxml_buffer_destroy(attr_value);

            return WBXML_ERROR_NOT_ENOUGH_MEMORY;
        }
    }
  
    if ((*attr = wbxml_attribute_create()) == NULL) {
        wbxml_attribute_name_destroy(attr_name);
        wbxml_buffer_destroy(attr_value);

        return WBXML_ERROR_NOT_ENOUGH_MEMORY;
    }
  
    (*attr)->name = attr_name;
    (*attr)->value = attr_value;
  
    return WBXML_OK;
}


/**
 * @brief Parse WBXML content
 * @param parser The WBXML Parser
 * @param result Resulting parsed content, if content is not an Element
 * @return WBXML_OK if parsing is OK, an error code otherwise
 * @note content = element | string | extension | entity | pi | opaque
 */
static WBXMLError parse_content(WBXMLParser *parser, WBXMLBuffer **result)
{
    WB_UTINY cur_byte;
    WBXMLError ret = WBXML_OK;

    /* Debug */
    if (!wbxml_buffer_get_char(parser->wbxml, parser->pos, &cur_byte))
        return WBXML_ERROR_END_OF_BUFFER;

    WBXML_DEBUG((WBXML_PARSER, "(%d) Parsing content: '0x%X'", parser->pos, cur_byte));

    /* extension */
    if (is_extension(parser))
        return parse_extension(parser, WBXML_TAG_TOKEN, result);

    /* entity */
    if (is_token(parser, WBXML_ENTITY))
        return parse_entity(parser, result);

    /* string */
    if (is_string(parser))
        return parse_string(parser, result);

    /* opaque */
    if (is_token(parser, WBXML_OPAQUE)) {
        if ((ret = parse_opaque(parser, result)) != WBXML_OK)
            return ret;

        return decode_opaque_content(parser, result);
    }

    /* pi */
    if (is_token(parser, WBXML_PI))
        return parse_pi(parser);
        
    /** 
     * @note Non standard behaviour because of a Nokia 6600 bug
     *       that generate switch pages in wrong places.
     *
     * Thanks to Balaji Alasyam for finding this bug.
     *
     * Example : 02 9F 53 6A 00 6D 6C 71 C3 03 31 2E 31 01 72 C3 0A 53 79 6E
     *           63 4D 4C 2F 31 2E 31 01 65 C3 01 34 01 5B C3 01 31 01 6E 57
     *           C3 31 68 74 74 70 3A 2F 2F 32 31 30 2E 32 31 34 2E 31 36 31
     *           2E 31 37 32 3A 38 30 38 30 2F 74 65 73 74 2F 53 79 6E 63 4D
     *           4C 3F 75 73 65 72 3D 62 61 6C 75 01 01 67 57 C3 14 49 4D 45
     *           49 3A 33 35 31 35 34 36 30 30 35 33 39 34 31 39 39 01 01 5A
     *           00 01 4C C3 05 31 30 30 30 30 01 00 00 01 01 6B 46 4B C3 ...
     *                                            ^^^^^
     */
   
    /* switchPage */
    if ( is_token(parser, WBXML_SWITCH_PAGE) )
      return parse_switch_page(parser, WBXML_TAG_TOKEN);

    /** @note We have recurrency here ! */
    return parse_element(parser);
}


/**
 * @brief Parse WBXML string
 * @param parser [in]  The WBXML Parsertatic
 * @param result [out] The resulting parsed string
 * @return WBXML_OK if parsing is OK, an error code otherwise
 * @note string = inline | tableref
 */
static WBXMLError parse_string(WBXMLParser *parser, WBXMLBuffer **result)
{
    WBXML_DEBUG((WBXML_PARSER, "(%d) Parsing string", parser->pos));

    if (is_token(parser, WBXML_STR_I))
        return parse_inline(parser, result);

    if (is_token(parser, WBXML_STR_T))
        return parse_tableref(parser, result);

    return WBXML_ERROR_STRING_EXPECTED;
}


/**
 * @brief Parse WBXML extension
 * @param parser     The WBXML Parser
 * @param code_space The token code space
 * @param result     Resulting parsed extension
 * @return WBXML_OK if parsing is OK, an error code otherwise
 * @note extension = [switchPage] (( EXT_I termstr ) | ( EXT_T index ) | EXT)
 * @note 5.8.4.2 - The effect of a switchPage preceding an extension will depend upon where the extension appears.
 *                   If switchPage appears in content, it will change the tag code page. Is switchPage appears in
 *                   an attribute list, it will change the attribute code page.
 * @note Extensions tokens are explained in WML Specifications (WAP-191-WML-20000219-a.pdf - 14.1.1 & 14.3)
 * @warning The resulting ext paramater MUST be freed by caller !
 */
static WBXMLError parse_extension(WBXMLParser *parser, WBXMLTokenType code_space, WBXMLBuffer **result)
{
    WB_UTINY   *ext   = NULL;
    WB_ULONG    len   = 0;
    WBXMLError  ret   = WBXML_OK;
    WB_UTINY    token = 0;

    WBXML_DEBUG((WBXML_PARSER, "(%d) Parsing extension", parser->pos));
  
    /* Parse switchPage */
    if (is_token(parser, WBXML_SWITCH_PAGE)) {
        if ((ret = parse_switch_page(parser, code_space)) != WBXML_OK) {
          return ret;
        }
    }
  
    /* Get Extension Token */
    if ((ret = parse_uint8(parser, &token)) != WBXML_OK) {
        return ret;
    }

#if 0
    /* Language specific treatment */
    switch (parser->langTable->langID) {
   
    default:
        WBXML_ERROR((WBXML_PARSER, "Extension tokens not allowed with this Document !"));
    }
#endif

    /* Set result */
    if (ext == NULL) {
        *result = NULL;
    }
    else {
        if ((*result = wbxml_buffer_create(ext, len, len)) == NULL) {
            return WBXML_ERROR_NOT_ENOUGH_MEMORY;
        }

        /** @todo Replace this local var by the direct creation of the result Buffer */
        wbxml_free(ext);
    }
  
    return WBXML_OK;
}


/**
 * @brief Parse WBXML entity
 * @param parser The WBXML Parser
 * @param result The resulting parsed entity
 * @return WBXML_OK if parsing is OK, an error code otherwise
 * @note entity = ENTITY entcode
 * @note http://www.w3.org/TR/wbxml/ : 
 *         "The character entity token (ENTITY) encodes a numeric character entity. This has the same semantics
 *          as an XML numeric character entity (eg, &#32;). The mb_u_int32 refers to a character in the UCS-4
 *          character encoding. All entities in the source XML document must be represented using either a string
 *          token (eg, STR_I or the ENTITY token."
 * @warning The resulting entity paramater MUST be freed by caller !
 */
static WBXMLError parse_entity(WBXMLParser *parser, WBXMLBuffer **result)
{
    WB_TINY    entity[10];
    WB_ULONG   code   = 0;
    WBXMLError ret    = WBXML_OK;
  
    WBXML_DEBUG((WBXML_PARSER, "(%d) Parsing entity", parser->pos));
  
    /* Skip ENTITY */
    parser->pos++;
  
    if ( (ret = parse_entcode(parser, &code)) != WBXML_OK ) {
        return ret;
    }
  
    /* Build Entity */
    if ( code > WBXML_PARSER_MAX_ENTITY_CODE )  {
        return WBXML_ERROR_ENTITY_CODE_OVERFLOW;
    }
  
    /**
     * WARNING: If you change the entity variable length (10 chars), change too
     *          'WBXML_PARSER_MAX_ENTITY_CODE' defined in this file !
     */
    WBXML_SPRINTFF(entity, "&#%u;", code);

    /* Create result buffer */
    if ( (*result = wbxml_buffer_create_from_cstr(entity)) == NULL ) {
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;
    }
  
  return WBXML_OK;
}


/**
 * @brief Parse WBXML opaque
 * @param parser The WBXML Parser
 * @param result Resulting opaque data parsed
 * @return WBXML_OK if parsing is OK, an error code otherwise
 * @note opaque = OPAQUE length *byte
 * @note length = mb_u_int32
 */
static WBXMLError parse_opaque(WBXMLParser *parser, WBXMLBuffer **result)
{
    WB_ULONG   len = 0;
    WBXMLError ret = WBXML_OK;
  
    WBXML_DEBUG((WBXML_PARSER, "(%d) Parsing opaque", parser->pos));
  
    if (parser->version < WBXML_VERSION_11)
        WBXML_WARNING((WBXML_PARSER, "No 'opaque' support in WBXML < %s", WBXML_VERSION_TEXT_11));
  
    /* Skip OPAQUE */
    parser->pos++;
  
    if ((ret = parse_mb_uint32(parser, &len)) != WBXML_OK) {
        return ret;
    }

    /* Check that length specified in OPAQUE doesn't overflow wbxml length */
    if (parser->pos + len > wbxml_buffer_len(parser->wbxml)) {
        return WBXML_ERROR_BAD_OPAQUE_LENGTH;
    }

    /**
     * Create result buffer (don't create a static buffer, because this can be
     * modified while trying to decode this content)
     */
    *result = wbxml_buffer_create(wbxml_buffer_get_cstr(parser->wbxml) + parser->pos, len, len);
    if (*result == NULL) {
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;
    }
  
    parser->pos += len;
  
    return ret;
}


/**
 * @brief Parse WBXML literalTag
 * @param parser The WBXML Parser
 * @param mask   Resulting tag mask (WBXML_TOKEN_MASK          |
 *                                   WBXML_TOKEN_WITH_CONTENT  |
 *                                   WBXML_TOKEN_WITH_ATTRS    |
 *                                   (WBXML_TOKEN_WITH_CONTENT || WBXML_TOKEN_WITH_ATTRS))
 * @param result The resulting parsed literal
 * @return WBXML_OK if parsing is OK, an error code otherwise
 * @note    result = ( literalTag index )
 *            literalTag = LITERAL | LITERAL_A | LITERAL_C | LITERAL_AC
 */
static WBXMLError parse_literal(WBXMLParser  *parser,
                                WB_UTINY     *mask,
                                WBXMLBuffer **result)
{
    WBXMLError ret     = WBXML_OK;
    WB_UTINY   token   = 0;
    WB_ULONG   index   = 0;
  
    WBXML_DEBUG((WBXML_PARSER, "(%d) Parsing literalTag", parser->pos));
  
    /* Parse literalTag */
    if ( (ret = parse_uint8(parser, &token)) != WBXML_OK ) {
        return ret;
    }
  
    /* Parse index */
    if ( (ret = parse_mb_uint32(parser, &index)) != WBXML_OK ) {
        return ret;
    }
  
    /* Get string */
    if ( (ret = get_strtbl_reference(parser, index, result)) != WBXML_OK ) {
        return ret;
    }

    /* Build Tag Mask */
    switch(token) {
    case WBXML_LITERAL:
        *mask = WBXML_TOKEN_MASK;
        break;

    case WBXML_LITERAL_C:
        *mask = WBXML_TOKEN_WITH_CONTENT;
        break;

    case WBXML_LITERAL_A:
        *mask = WBXML_TOKEN_WITH_ATTRS;
        break;

    case WBXML_LITERAL_AC:
        *mask = ( WBXML_TOKEN_WITH_CONTENT | WBXML_TOKEN_WITH_ATTRS );
        break;

    default:
        return WBXML_ERROR_INTERNAL;
    }
  
    return WBXML_OK;
}


/**
 * @brief Parse WBXML attrStart
 * @param parser The WBXML Parser
 * @param name The Attribute Name parsed
 * @param value The Attribute Value associated, if any
 * @return WBXML_OK if parsing is OK, an error code otherwise
 * @note attrStart = ([switchPage] ATTRSTART) | ( LITERAL index )
 */
static WBXMLError parse_attr_start(WBXMLParser         *parser,
                                   WBXMLAttributeName **name,
                                   const WB_UTINY     **value)
{
    WBXMLBuffer *literal_str = NULL;
    WB_UTINY     literal     = 0;    
    WB_UTINY     tag         = 0;
    WBXMLError   ret         = WBXML_OK;
    WB_ULONG     index       = 0;

    WBXML_DEBUG((WBXML_PARSER, "(%d) Parsing attrStart", parser->pos));
  
  
    /**************************
     * Case: ( LITERAL index ) 
     */
  
    if (is_token(parser, WBXML_LITERAL)) {
        if ((ret = parse_literal(parser, &literal, &literal_str)) != WBXML_OK) {
            return ret;
        }
    
        if ((*name = wbxml_attribute_name_create_literal(wbxml_buffer_get_cstr(literal_str))) == NULL) {
            ret = WBXML_ERROR_NOT_ENOUGH_MEMORY;
        }
    
        /**
         * @todo Return Warning if 'literal' is different from 'WBML_TOKEN_MASK' (because it MUST be a 'LITERAL' token, not
         *       LITERAL_A, nor LITERAL_C, nor LITERAL_AC
         */

        wbxml_buffer_destroy(literal_str);
        return WBXML_OK;
    }
  
  
    /***********************************
     * Case: ( [switchPage] ATTRSTART )
     */
  
    /* Parse switchPage */
    if (is_token(parser, WBXML_SWITCH_PAGE)) {
        if ((ret = parse_switch_page(parser, WBXML_ATTR_TOKEN)) != WBXML_OK) {
            return ret;
        }
    }
  
    /* Parse UINT8 */
    if ((ret = parse_uint8(parser, &tag)) != WBXML_OK) {
        return ret;
    }
  
    WBXML_DEBUG((WBXML_PARSER, "\tToken: 0x%X", tag));
  
    /* Search tag in Tags Table */
    if (parser->langTable == NULL) {
        return WBXML_ERROR_LANG_TABLE_UNDEFINED;
    }
  
    if (parser->langTable->attrTable == NULL) { 
        return WBXML_ERROR_ATTR_TABLE_UNDEFINED;
    }

    while ((parser->langTable->attrTable[index].xmlName != NULL) &&
           ((parser->langTable->attrTable[index].wbxmlToken != tag) ||
            (parser->langTable->attrTable[index].wbxmlCodePage != parser->attrCodePage)))
    {
        index++;
    }
  
    if (parser->langTable->attrTable[index].xmlName == NULL) {
#if WBXML_PARSER_BEST_EFFORT
        /* Create "unknown" Attribute Name */
        if ((*name = wbxml_attribute_name_create_literal(WBXML_PARSER_UNKNOWN_STRING)) == NULL) {
            return WBXML_ERROR_NOT_ENOUGH_MEMORY;
        }
    
        return WBXML_OK;
#else
        return WBXML_ERROR_UNKNOWN_ATTR;
#endif /* WBXML_PARSER_BEST_EFFORT */
    }
  
    /* Create Token Attribute Name */
    if ((*name = wbxml_attribute_name_create(WBXML_VALUE_TOKEN)) == NULL) {
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;
    }
  
    (*name)->u.token = &(parser->langTable->attrTable[index]);
  
    /* Get Attribute start value (if any) */
    if (parser->langTable->attrTable[index].xmlValue != NULL) {
        *value = (const WB_UTINY *) parser->langTable->attrTable[index].xmlValue;
    }
  
    return WBXML_OK;
}


/**
 * @brief Parse WBXML attrValue
 * @param parser [in]  The WBXML Parser
 * @param result [out] The resulting Value parsed
 * @return WBXML_OK if parsing is OK, an error code otherwise
 * @note attrValue = ([switchPage] ATTRVALUE) | string | extension | entity | opaque
 */
static WBXMLError parse_attr_value(WBXMLParser  *parser,
                                   WBXMLBuffer **result)
{
    WB_ULONG   index = 0;
    WB_UTINY   tag   = 0;
    WBXMLError ret   = WBXML_OK;
  
    WBXML_DEBUG((WBXML_PARSER, "(%d) Parsing attrValue", parser->pos));
  
    /* Parse extension */
    if (is_extension(parser)) {
        return parse_extension(parser, WBXML_ATTR_TOKEN, result);
    }
  
    /* Parse entity */
    if (is_token(parser, WBXML_ENTITY)) {
        return parse_entity(parser, result);
    }
  
    /* Parse string */
    if (is_string(parser)) {
        return parse_string(parser, result);
    }

    /* Parse opaque */
    if (is_token(parser, WBXML_OPAQUE)) {
        if (parser->version < WBXML_VERSION_12) {
            WBXML_ERROR((WBXML_PARSER, "An Attribute value can't be 'opaque' in WBXML version < %s", WBXML_VERSION_TEXT_12));
        }
        
        return parse_opaque(parser, result);
    }
  
  
    /*****************************
     *    ([switchPage] ATTRVALUE)
     */
  
    /* Parse switchPage */
    if (is_token(parser, WBXML_SWITCH_PAGE)) {
        ret = parse_switch_page(parser, WBXML_ATTR_TOKEN);
        if (ret != WBXML_OK) {
            return ret;
        }
    }
  
    /* Parse UINT8 */
    ret = parse_uint8(parser, &tag);
    if (ret != WBXML_OK) {
        return ret;
    }
  
    /* Search tag in Tags Table */
    if (parser->langTable == NULL) {
        return WBXML_ERROR_LANG_TABLE_UNDEFINED;
    }
  
    if (parser->langTable->attrValueTable == NULL) {
        return WBXML_ERROR_ATTR_VALUE_TABLE_UNDEFINED;
    }
  
    while ((parser->langTable->attrValueTable[index].xmlName != NULL) &&
           ((parser->langTable->attrValueTable[index].wbxmlToken != tag) ||
            (parser->langTable->attrValueTable[index].wbxmlCodePage != parser->attrCodePage)))
    {
        index++;
    }
  
    if (parser->langTable->attrValueTable[index].xmlName == NULL) {
        return WBXML_ERROR_UNKNOWN_ATTR_VALUE;
    }

    *result = wbxml_buffer_sta_create_from_cstr(parser->langTable->attrValueTable[index].xmlName);

    return WBXML_OK;
}


/**
 * @brief Parse WBXML termstr
 * @param parser [in]  The WBXML Parser
 * @param result [out] The resulting parsed string
 * @return WBXML_OK if parsing is OK, an error code otherwise
 * @note termstr = charset-dependent string with termination
 */
static WBXMLError parse_termstr(WBXMLParser  *parser,
                                WBXMLBuffer **result)
{
    WB_ULONG   max_len  = 0;
    WBXMLError ret      = WBXML_OK;
  
    WBXML_DEBUG((WBXML_PARSER, "(%d) Parsing termstr", parser->pos));

    /* Get max possible string length */
    max_len = wbxml_buffer_len(parser->wbxml) - parser->pos;
  
    /* Convert to UTF-8 Buffer */
    if ((ret = wbxml_charset_conv_term((const WB_TINY *) (wbxml_buffer_get_cstr(parser->wbxml) + parser->pos),
                                       &max_len,
                                       parser->charset,
                                       result,
                                       WBXML_CHARSET_UTF_8)) != WBXML_OK) {
        return ret;
    }

    parser->pos += max_len;
  
    WBXML_DEBUG((WBXML_PARSER, "(%d) termstr: %s", parser->pos, wbxml_buffer_get_cstr(*result)));
  
    return WBXML_OK;
}


/**
 * @brief Parse WBXML inline
 * @param parser [in]  The WBXML Parser
 * @param result [out] The resulting parsed string
 * @return WBXML_OK if parsing is OK, an error code otherwise
 * @note inline = STR_I termstr
 */
static WBXMLError parse_inline(WBXMLParser  *parser,
                               WBXMLBuffer **result)
{
    WBXML_DEBUG((WBXML_PARSER, "(%d) Parsing inline", parser->pos));
  
    /* Skip STR_I */
    parser->pos++;
  
    return parse_termstr(parser, result);
}


/**
 * @brief Parse WBXML tableref
 * @param parser The WBXML Parser
 * @param result The resulting parsed string
 * @return WBXML_OK if parsing is OK, an error code otherwise
 * @note tableref = STR_T index
 * @note index = mb_u_int32
 */
static WBXMLError parse_tableref(WBXMLParser  *parser,
                                 WBXMLBuffer **result)
{
    WB_ULONG index;
    WBXMLError ret = WBXML_OK;
  
    WBXML_DEBUG((WBXML_PARSER, "(%d) Parsing tableref", parser->pos));
  
    /* Skip STR_T */
    parser->pos++;
  
    /* Parse index */
    if ((ret = parse_mb_uint32(parser, &index)) != WBXML_OK) {
        return ret;
    }
  
    return get_strtbl_reference(parser, index, result);
}


/**
 * @brief Parse WBXML entcode
 * @param parser [in] The WBXML Parser
 * @param result [out] The entcode parsed
 * @return WBXML_OK if parsing is OK, an error code otherwise
 * @note entcode = mb_u_int32 // UCS-4 character code
 */
static WBXMLError parse_entcode(WBXMLParser *parser,
                                WB_ULONG    *result)
{
    WBXML_DEBUG((WBXML_PARSER, "(%d) Parsing entcode", parser->pos));
  
    return parse_mb_uint32(parser, result);
}


/**
 * @brief Get a string from String Table
 * @param parser The WBXML Parser
 * @param index  Index of string in String Table
 * @param result The resulting parsed string
 * @return WBXML_OK if OK, an error code otherwise
 */
static WBXMLError get_strtbl_reference(WBXMLParser  *parser,
                                       WB_ULONG      index,
                                       WBXMLBuffer **result)
{
    WB_ULONG   max_len = 0;
    WBXMLError ret     = WBXML_OK;
  
    /* Check if strtbl is NULL */
    if (parser->strstbl == NULL) {
        return WBXML_ERROR_NULL_STRING_TABLE;
    }
  
    if (index >= wbxml_buffer_len(parser->strstbl)) {
        return WBXML_ERROR_INVALID_STRTBL_INDEX;
    }

    /* Get max possible string length */
    max_len = wbxml_buffer_len(parser->strstbl) - index;
  
    /* Convert to UTF-8 Buffer */
    if ((ret = wbxml_charset_conv_term((const WB_TINY *) (wbxml_buffer_get_cstr(parser->strstbl) + index),
                                       &max_len,
                                       parser->charset,
                                       result,
                                       WBXML_CHARSET_UTF_8)) != WBXML_OK) {
        return ret;
    }
  
    WBXML_DEBUG((WBXML_PARSER, "(%d) String Table Reference: %s", parser->pos, wbxml_buffer_get_cstr(*result)));
  
    return WBXML_OK;
}


/********************************
 *    Basic Types Parse functions
 */

/**
 * @brief Parse UINT8
 * @param parser [in] The WBXML Parser
 * @param result [out] Parsed UINT8
 * @return WBXML_OK if parsing is OK, an error code otherwise
 * @note u_int8 = 8 bit unsigned integer
 */
static WBXMLError parse_uint8(WBXMLParser *parser, WB_UTINY *result)
{
    if (parser == NULL)
        return WBXML_ERROR_NULL_PARSER;

    if (result == NULL)
        return WBXML_ERROR_BAD_PARAMETER;

    if (parser->pos == wbxml_buffer_len(parser->wbxml))
        return WBXML_ERROR_END_OF_BUFFER;

    if (!wbxml_buffer_get_char(parser->wbxml, parser->pos, result))
        return WBXML_ERROR_END_OF_BUFFER;

    parser->pos++;

    return WBXML_OK;
}


/**
 * @brief Parse a MultiByte UINT32
 * @param parser The WBXML Parser
 * @param result The parsed MultiByte
 * @return WBXML_OK if parsing is OK, an error code otherwise
 * @note mb_u_int32 = 32 bit unsigned integer, encoded in multi-byte format
 */
static WBXMLError parse_mb_uint32(WBXMLParser *parser, WB_ULONG *result)
{
    WB_ULONG uint = 0, byte_pos;
    WB_UTINY cur_byte;

    if (parser == NULL)
        return WBXML_ERROR_NULL_PARSER;

    if (result == NULL)
        return WBXML_ERROR_BAD_PARAMETER;

    /* It's a 32bit integer, and so it fits to a maximum of 4 bytes */
    for (byte_pos = 0; byte_pos < 5; byte_pos++) {
        /* Get current byte */
        if (!wbxml_buffer_get_char(parser->wbxml, parser->pos, &cur_byte))
            return WBXML_ERROR_END_OF_BUFFER;

        /* Move to next byte */
        parser->pos++;

        /* Update uint value */
        uint = (uint << 7) | ((WB_UTINY)cur_byte & 0x7F);

        /* Check first bit, and stop if value is zero */
        if (!((WB_UTINY)cur_byte & 0x80)) {
            *result = uint;
            return WBXML_OK;
        }
    }

    return WBXML_ERROR_UNVALID_MBUINT32;
}


/****************************************
 * Language Specific Decoding Functions 
 */


/**
 * @brief Decode an Opaque Content buffer
 * @param parser The WBXML Parser
 * @param data The Opaque data buffer
 * @return WBXML_OK if OK, another error code otherwise
 * @note Used for:
 *      - WV 1.1 / 1.2
 */
static WBXMLError decode_opaque_content(WBXMLParser  *parser,
                                        WBXMLBuffer **data)
{
    switch (parser->langTable->langID) 
    {
#if defined( WBXML_SUPPORT_DRMREL )

    case WBXML_LANG_DRMREL10:
        if ((parser->current_tag->wbxmlCodePage == 0x00) &&
            (parser->current_tag->wbxmlToken == 0x0C))
        {
            WBXMLError ret = WBXML_OK;
            
            /* Decode <KeyValue> */
            if ((ret = decode_drmrel_keyvalue(data)) != WBXML_OK)
                return ret;

            return WBXML_OK;
        }
        break;

#endif /* WBXML_SUPPORT_DRMREL */    
    
    default:
        /* NOP */
        break;
    } /* switch */
  
    return WBXML_OK;
}



#if defined( WBXML_SUPPORT_DRMREL )

/**************************************
 * DRMREL 1.0
 */

/**
 * @brief Decode a DRMREL 'KeyValue'
 * @param data The KeyValue to decode
 * @return WBXML_OK if OK, another error code otherwise
 * @note Used for:
 *      - DRMREL 1.0
 * @note [OMA-Download-DRMREL-v1_0-20020913-C.PDF] - 7.1:
 *       "The content of the <KeyValue> element MUST be in binary format using the �opaque?token."
 */
static WBXMLError decode_drmrel_keyvalue(WBXMLBuffer **data)
{
    WB_UTINY   *result = NULL;
    WBXMLError  ret    = WBXML_OK;
  
    if ((data == NULL) || (*data == NULL)) {
        return WBXML_ERROR_INTERNAL;
    }
  
    if ((result = wbxml_base64_encode((const WB_UTINY *) wbxml_buffer_get_cstr(*data),
                                      wbxml_buffer_len(*data))) == NULL)
    {
        return WBXML_ERROR_B64_ENC;
    }

    /* Reset buffer */
    wbxml_buffer_delete(*data, 0, wbxml_buffer_len(*data));

    /* Set data */
    if (!wbxml_buffer_append_cstr(*data, result)) {
        ret = WBXML_ERROR_NOT_ENOUGH_MEMORY;
    }

    wbxml_free(result);
  
    return ret;
}

#endif /* WBXML_SUPPORT_DRMREL */
