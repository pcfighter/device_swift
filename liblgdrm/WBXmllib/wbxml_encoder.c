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
 * @file wbxml_encoder.c
 * @ingroup wbxml_encoder
 *
 * @author Aymerick Jehanne <libwbxml@aymerick.com>
 * @date 11/11/02
 *
 * @brief WBXML Encoder - Encodes a WBXML Tree to WBXML or to XML
 *
 * @note Inspired from kannel WML Encoder (http://www.kannel.org)
 *
 * @note [OMA WV 1.1] : OMA-WV-CSP_WBXML-V1_1-20021001-A.pdf
 *
 * @todo Parse CDDATA
 * @todo Parse PI
 * @todo Handle Charsets Encoding
 * @todo Really generate ENTITY tokens
 * @todo Handle Namespaces !
 * @todo For canonical XML output: Sort the Attributes
 * @todo When adding string to String Table, check that this is not a Content Text that will be tokenized
 * @todo For Wireless-Village CSP :
 *              - Encode "Date and Time" in OPAQUE (OMA-WV-CSP_WBXML-V1_1-20021001-A.pdf - 6.6)
 *
 * @todo Review the canonical XML generation:
 *       - http://www.jclark.com/xml/canonxml.html
*        - http://www.w3.org/TR/2004/REC-xml-20040204/
 */

#include <ctype.h> /* For isdigit() */

#include "wbxml.h"


/**
 * Compilation Flag: WBXML_ENCODER_USE_STRTBL
 * -----------------
 * Do We Use String Table when Encoding to WBXML ?
 * (NOTE: We still use String Table for Unknown Public ID, even if this flag is not set)
 */

/* WBXML Header:    version     publicid    charset     length
 *                  u_int8      mb_u_int32  mb_u_int32  mb_u_int32
 *                  1 octet     5 octets    5 octets    5 octets   :  16 octets
 * mb_u_int32: 5 octets (to handle continuation bits)
 */
#define WBXML_HEADER_MAX_LEN 16

/* Memory management related defines */
#define WBXML_ENCODER_XML_DOC_MALLOC_BLOCK 5000
#define WBXML_ENCODER_WBXML_DOC_MALLOC_BLOCK 1000

#define WBXML_ENCODER_XML_HEADER_MALLOC_BLOCK 250
#define WBXML_ENCODER_WBXML_HEADER_MALLOC_BLOCK WBXML_HEADER_MAX_LEN

/* WBXML Default Charset: UTF-8 (106) */
#define WBXML_ENCODER_DEFAULT_CHARSET 0x6a

/* String Terminating NULL Char */
#define WBXML_STR_END '\0'

/* Minimum String Size needed for String Table - @note Set to '3' for Prov 1.0 */
#define WBXML_ENCODER_STRING_TABLE_MIN 3

/**
 * Default charset of the outputed WBXML document. Used only in this case :
 *  - No charset was indicated thanks to the function 'wbxml_encoder_set_output_charset()'
 *  - and the WBXML Tree field 'orig_charset' is set to WBXML_CHARSET_UNKNOWN (ie. charset
 *    information not found in original document)
 */
#define WBXML_ENCODER_WBXML_DEFAULT_CHARSET WBXML_CHARSET_UTF_8

/**
 * Default charset of the outputed XML document. Used only in this case :
 *  - No charset was indicated thanks to the function 'wbxml_encoder_set_output_charset()'
 *  - and the WBXML Tree field 'orig_charset' is set to WBXML_CHARSET_UNKNOWN (ie. charset
 *    information not found in original document)
 */
#define WBXML_ENCODER_XML_DEFAULT_CHARSET WBXML_CHARSET_UTF_8

/**
 * If defined, generate empty XML elements (eg: <foo />), else generate
 * full "end element" (eg: <foo></foo>)
 *
 * @todo This must be a 'WBXMLGenXMLParams' parameter
 */
//#define WBXML_ENCODER_XML_GEN_EMPTY_ELT	/* Lothson.20070905 Disable to genrate XC14N xml Doc */

/**
 * If defined, do not indent elements that have no element child (eg: <foo>bar</foo>),
 * else indent anyway (eg: <foo>
 *                           bar
 *                         </foo>)
 *
 * @todo This must be a 'WBXMLGenXMLParams' parameter
 */
#define WBXML_ENCODER_XML_NO_EMPTY_ELT_INDENT


/**
 * @warning For now 'current_tag' field is only used for WV Content Encoding. And for this use, it works.
 *          But this field is reset after End Tag, and as there is no Linked List mecanism, this is bad for
 *          cascading elements: we don't fill this field with parent Tag when parsing End Tag.
 *
 * @warning For now 'current_text_parent' field is only used for DRM REL Content Encoding. It should not be
 *          used for another purpose.
 */
struct WBXMLEncoder_s {
    WBXMLTree *tree;                        /**< WBXML Tree to Encode */
    const WBXMLLangEntry *lang;             /**< Language table to use */
    WBXMLBuffer *output;                    /**< The output (wbxml or xml) we are producing */
    WBXMLBuffer *output_header;             /**< The output header (used if Flow Mode encoding is activated) */
    const WBXMLTagEntry *current_tag;       /**< Current Tag (See The Warning For This Field !) */
    const WBXMLTreeNode *current_text_parent; /**< Text parent of current Node (See The Warning For This Field !) */
    const WBXMLAttrEntry *current_attr;     /**< Current Attribute */
    WB_UTINY tagCodePage;                   /**< Current Tag Code Page */
    WB_UTINY attrCodePage;                  /**< Current Attribute Code Page */
    WB_BOOL ignore_empty_text;              /**< Do we ignore empty text nodes (ie: ignorable whitespaces)? */
    WB_BOOL remove_text_blanks;             /**< Do we remove leading and trailing blanks in text nodes ? */
    WBXMLEncoderOutputType output_type;     /**< Output Type */
    WBXMLGenXMLType xml_gen_type;           /**< XML Generation Type */
    WB_UTINY indent_delta;                  /**< Indent Delta (number of spaces) */
    WB_UTINY indent;                        /**< Current Indent */
    WB_BOOL in_content;                     /**< We are in Content Text (used for indentation when generating XML output) */
    WB_BOOL in_cdata;                       /**< We are in a CDATA section (and so, content must be generaed "as is") */
    WBXMLBuffer *cdata;                     /**< Current CDATA Buffer */
#if defined( WBXML_ENCODER_USE_STRTBL )
    WBXMLList *strstbl;                     /**< String Table we are creating */
    WB_ULONG strstbl_len;                   /**< String Table Length */
    WB_BOOL use_strtbl;                     /**< Do we use String Table when generating WBXML output ? (default: YES) */
#endif /* WBXML_ENCODER_USE_STRTBL */
    WB_BOOL xml_encode_header;              /**< Do we generate XML Header ? */
    WBXMLVersion wbxml_version;             /**< WBXML Version to use (when generating WBXML output) */
    WBXMLCharsetMIBEnum output_charset;     /**< Output charset encoding */
    WB_BOOL flow_mode;                      /**< Is Flow Mode encoding activated ? */
    WB_ULONG pre_last_node_len;             /**< Output buffer length before last node encoding */
    WB_BOOL textual_publicid;               /**< Generate textual Public ID instead of token (when generating WBXML output) */

#if defined( WBXML_DRMEXT )
	WB_BOOL use_strtbl_literal;  /**< Use String Table only for literal (except wbxml string) (Default: FALSE) */
	WB_BOOL record_literals;		/**< Record name of tag and attribute start which are not included in code page (Default: FALSE) */
	WB_BOOL record_attrvalue_literals;	/**< Record attribute values which are not included in code page (Default: FALSE) */

	WB_LONG Tags;
	WBXMLNode *pstTag;

	WB_LONG Atts;
	WBXMLNode *pstAtt;

	WB_LONG AttValues;
	WBXMLNode *pstAttValues;
#endif
};

#if defined( WBXML_ENCODER_USE_STRTBL )
/**
 * @brief The WBXML String Table Element
 */
typedef struct WBXMLStringTableElement_t {
    WBXMLBuffer *string; /**< String */
    WB_ULONG offset;     /**< Offset of String in String Table */
    WB_ULONG count;      /**< Number of times this String is referenced in the XML Document */
    WB_BOOL stat;        /**< If set to TRUE, this is a static String that we must not destroy in wbxml_strtbl_element_destroy() function */
} WBXMLStringTableElement;
#endif /* WBXML_ENCODER_USE_STRTBL */

/**
 * @brief WBXML Value Element Context: In Content or in Attribute Value
 */
typedef enum WBXMLValueElementCtx_e {
    WBXML_VALUE_ELEMENT_CTX_CONTENT = 0,    /**< Text Content */
    WBXML_VALUE_ELEMENT_CTX_ATTR            /**< Attribute Value */
} WBXMLValueElementCtx;

/**
 * @brief WBXML Value Element Type: string / tableref / extension / opaque
 */
typedef enum WBXMLValueElementType_e {
    WBXML_VALUE_ELEMENT_STRING = 0, /**< Inline String */
    WBXML_VALUE_ELEMENT_EXTENSION,  /**< Extension Token */
    WBXML_VALUE_ELEMENT_OPAQUE,     /**< Opaque Buffer */
    WBXML_VALUE_ELEMENT_ATTR_TOKEN /**< Attribute Value Token */
#if defined( WBXML_ENCODER_USE_STRTBL )
    , WBXML_VALUE_ELEMENT_TABLEREF   /**< String Table Reference */
#endif /* WBXML_ENCODER_USE_STRTBL */
} WBXMLValueElementType;

/**
 * @brief WBXML Value Element Structure
 */
typedef struct WBXMLValueElement_t {
    WBXMLValueElementType type;     /**< Cf WBXMLValueElementType enum */
    union {
        WBXMLBuffer *str;                   /**< WBXML_VALUE_ELEMENT_STRING */
        const WBXMLExtValueEntry *ext;      /**< WBXML_VALUE_ELEMENT_EXTENSION */
        WBXMLBuffer *buff;                  /**< WBXML_VALUE_ELEMENT_OPAQUE */
        const WBXMLAttrValueEntry *attr;    /**< WBXML_VALUE_ELEMENT_ATTR_TOKEN */
#if defined( WBXML_ENCODER_USE_STRTBL )
        WB_ULONG    index;                  /**< WBXML_VALUE_ELEMENT_TABLEREF */
#endif /* WBXML_ENCODER_USE_STRTBL */
    } u;
} WBXMLValueElement;


/***************************************************
 *    Private Functions prototypes
 */

/*******************************
 * Common Functions
 */

#if 0
static WB_BOOL convert_char_to_ucs4(WB_UTINY ch, WB_ULONG *result);
#endif /* 0 */

static WBXMLEncoder *encoder_duplicate(WBXMLEncoder *encoder);
static WBXMLError encoder_encode_tree(WBXMLEncoder *encoder);
static WB_BOOL encoder_init_output(WBXMLEncoder *encoder);


/*******************************
 * WBXML Tree Parsing Functions
 */

static WBXMLError parse_node(WBXMLEncoder *encoder, WBXMLTreeNode *node, WB_BOOL enc_end);
static WBXMLError parse_element(WBXMLEncoder *encoder, WBXMLTreeNode *node, WB_BOOL has_content);
static WBXMLError parse_element_end(WBXMLEncoder *encoder, WBXMLTreeNode *node, WB_BOOL has_content);
static WBXMLError parse_attribute(WBXMLEncoder *encoder, WBXMLAttribute *attribute);
static WBXMLError parse_text(WBXMLEncoder *encoder, WBXMLTreeNode *node);
static WBXMLError parse_cdata(WBXMLEncoder *encoder, WBXMLTreeNode *node);
static WBXMLError parse_pi(WBXMLEncoder *encoder, WBXMLTreeNode *node);
static WBXMLError parse_tree(WBXMLEncoder *encoder, WBXMLTreeNode *node);


/*******************************
 * WBXML Output Functions
 */

/* Build WBXML Result */
static WBXMLError wbxml_build_result(WBXMLEncoder *encoder, WB_UTINY **wbxml, WB_ULONG *wbxml_len);
static WBXMLError wbxml_fill_header(WBXMLEncoder *encoder, WBXMLBuffer *header);

/* WBXML Encoding Functions */
static WBXMLError wbxml_encode_end(WBXMLEncoder *encoder);

static WBXMLError wbxml_encode_tag(WBXMLEncoder *encoder, WBXMLTreeNode *node, WB_BOOL has_content);
static WBXMLError wbxml_encode_tag_literal(WBXMLEncoder *encoder, WB_UTINY *tag, WB_UTINY mask);
static WBXMLError wbxml_encode_tag_token(WBXMLEncoder *encoder, WB_UTINY token, WB_UTINY page);

static WBXMLError wbxml_encode_attr(WBXMLEncoder *encoder, WBXMLAttribute *attribute);
static WBXMLError wbxml_encode_attr_start(WBXMLEncoder *encoder, WBXMLAttribute *attribute, WB_UTINY **value);
static WBXMLError wbxml_encode_value_element_buffer(WBXMLEncoder *encoder, WB_UTINY *value, WBXMLValueElementCtx ctx);
static WBXMLError wbxml_encode_value_element_list(WBXMLEncoder *encoder, WBXMLList *list);
static WBXMLError wbxml_encode_attr_start_literal(WBXMLEncoder *encoder, const WB_UTINY *attr);
static WBXMLError wbxml_encode_attr_token(WBXMLEncoder *encoder, WB_UTINY token, WB_UTINY page);

static WBXMLError wbxml_encode_inline_string(WBXMLEncoder *encoder, WBXMLBuffer *str);
static WBXMLError wbxml_encode_inline_integer_extension_token(WBXMLEncoder *encoder, WB_UTINY ext, WB_UTINY value);
#if 0
static WBXMLError wbxml_encode_entity(WBXMLEncoder *encoder, WB_ULONG value);
#endif /* 0 */
static WBXMLError wbxml_encode_opaque(WBXMLEncoder *encoder, WBXMLBuffer *buff);
#if defined( WBXML_ENCODER_USE_STRTBL )
static WBXMLError wbxml_encode_tableref(WBXMLEncoder *encoder, WB_ULONG offset);
#endif /* WBXML_ENCODER_USE_STRTBL */

static WBXMLValueElement *wbxml_value_element_create(void);
static void wbxml_value_element_destroy(WBXMLValueElement *elt);
static void wbxml_value_element_destroy_item(void *elt);

static WBXMLError wbxml_encode_tree(WBXMLEncoder *encoder, WBXMLTree *tree);

#if defined( WBXML_SUPPORT_DRMREL )
static WBXMLError wbxml_encode_drmrel_content(WBXMLEncoder *encoder, WB_UTINY *buffer);
#endif /* WBXML_SUPPORT_DRMREL */

#if defined( WBXML_ENCODER_USE_STRTBL )
/* WBXML String Table Functions */
static WBXMLStringTableElement *wbxml_strtbl_element_create(WBXMLBuffer *string, WB_BOOL is_stat);
static void wbxml_strtbl_element_destroy(WBXMLStringTableElement *element);
static void wbxml_strtbl_element_destroy_item(void *element);

static WBXMLError wbxml_strtbl_initialize(WBXMLEncoder *encoder, WBXMLTreeNode *root);
static void wbxml_strtbl_collect_strings(WBXMLEncoder *encoder, WBXMLTreeNode *node, WBXMLList *strings);
static WBXMLError wbxml_strtbl_collect_words(WBXMLList *elements, WBXMLList **result);
static WBXMLError wbxml_strtbl_construct(WBXMLBuffer *buff, WBXMLList *strstbl);
static WBXMLError wbxml_strtbl_check_references(WBXMLEncoder *encoder, WBXMLList **strings, WBXMLList **one_ref, WB_BOOL stat_buff);
static WB_BOOL wbxml_strtbl_add_element(WBXMLEncoder *encoder, WBXMLStringTableElement *elt, WB_ULONG *index, WB_BOOL *added);
#endif /* WBXML_ENCODER_USE_STRTBL */


/*******************************
 * XML Output Functions
 */

/** New Line */
#define WBXML_ENCODER_XML_NEW_LINE ((WB_UTINY *)"\n")

/* XML Header Macros */
#define WBXML_ENCODER_XML_HEADER "<?xml version=\"1.0\"?>"
#define WBXML_ENCODER_XML_DOCTYPE "<!DOCTYPE "
#define WBXML_ENCODER_XML_PUBLIC " PUBLIC \""
#define WBXML_ENCODER_XML_DTD "\" \""
#define WBXML_ENCODER_XML_END_DTD "\">"

/* Global vars for XML Normalization */
const WB_UTINY xml_lt[5]     = "&lt;";   /**< &lt; */
const WB_UTINY xml_gt[5]     = "&gt;";   /**< &gt; */
const WB_UTINY xml_amp[6]    = "&amp;";  /**< &amp; */
const WB_UTINY xml_quot[7]   = "&quot;"; /**< &quot; */
const WB_UTINY xml_apos[7]   = "&apos;"; /**< &apos; */
const WB_UTINY xml_slashr[6] = "&#13;";  /**< &#13; */
const WB_UTINY xml_slashn[6] = "&#10;";  /**< &#10; */
const WB_UTINY xml_tab[5]    = "&#9;";   /**< &#9; */

/* Build XML Result */
static WBXMLError xml_build_result(WBXMLEncoder *encoder, WB_UTINY **xml, WB_ULONG *xml_len);
static WBXMLError xml_fill_header(WBXMLEncoder *encoder, WBXMLBuffer *header);

/* XML Encoding Functions */
static WBXMLError xml_encode_tag(WBXMLEncoder *encoer, WBXMLTreeNode *node);
static WBXMLError xml_encode_end_tag(WBXMLEncoder *encoder, WBXMLTreeNode *node);

static WBXMLError xml_encode_attr(WBXMLEncoder *encoder, WBXMLAttribute *attribute);
static WBXMLError xml_encode_end_attrs(WBXMLEncoder *encoder, WBXMLTreeNode *node);

static WBXMLError xml_encode_text(WBXMLEncoder *encoder, WBXMLTreeNode *node);
static WB_BOOL xml_encode_new_line(WBXMLBuffer *buff);
static WB_BOOL xml_fix_text(WBXMLBuffer *buff, WB_BOOL normalize);

static WBXMLError xml_encode_cdata(WBXMLEncoder *encoder);
static WBXMLError xml_encode_end_cdata(WBXMLEncoder *encoder);

static WBXMLError xml_encode_tree(WBXMLEncoder *encoder, WBXMLTree *tree);


/***************************************************
 *    Public Functions
 */

WBXMLEncoder * wbxml_encoder_create_real(void)
{
    WBXMLEncoder *encoder = NULL;

    encoder = (WBXMLEncoder *) wbxml_malloc(sizeof(WBXMLEncoder));
    if (encoder == NULL) {
        return NULL;
    }

#if defined( WBXML_ENCODER_USE_STRTBL )
    if ((encoder->strstbl = wbxml_list_create()) == NULL) {
        wbxml_free(encoder);
        return NULL;
    }
    encoder->use_strtbl = TRUE;
    encoder->strstbl_len = 0;
#endif /* WBXML_ENCODER_USE_STRTBL */

    encoder->tree = NULL;
    encoder->lang = NULL;
    encoder->output = NULL;
    encoder->output_header = NULL;

    encoder->current_tag = NULL;
    encoder->current_text_parent = NULL;
    encoder->current_attr = NULL;

    encoder->tagCodePage = 0;
    encoder->attrCodePage = 0;

    encoder->ignore_empty_text = FALSE;
    encoder->remove_text_blanks = FALSE;

    encoder->output_type = WBXML_ENCODER_OUTPUT_WBXML;
    encoder->xml_gen_type = WBXML_GEN_XML_COMPACT;

    encoder->indent_delta = 1;
    encoder->indent = 0;
    encoder->in_content = FALSE;
    encoder->in_cdata = FALSE;
    encoder->cdata = NULL;

    encoder->xml_encode_header = FALSE;	// TRUE; Lothson.20070901 Disable

    /* Default Version: WBXML 1.3 */
    encoder->wbxml_version = WBXML_VERSION_13;

    /**
     * Default Output Charset Encoding is the one found in WBXML Tree,
     * so set it as 'unknown' for now.
     */
    encoder->output_charset = WBXML_CHARSET_UNKNOWN;
    
    encoder->flow_mode = FALSE;
    encoder->pre_last_node_len = 0;
    encoder->textual_publicid = FALSE;

#if defined( WBXML_DRMEXT )
	encoder->use_strtbl_literal = FALSE;
	encoder->record_literals = FALSE;
	encoder->record_attrvalue_literals = FALSE;

	encoder->Tags = 0;
	encoder->pstTag = NULL;
	
	encoder->Atts = 0;
	encoder->pstAtt = NULL;

	encoder->AttValues = 0;
	encoder->pstAttValues = NULL;
#endif

    return encoder;
}


void wbxml_encoder_destroy(WBXMLEncoder *encoder)
{
    if (encoder == NULL)
        return;

    wbxml_buffer_destroy(encoder->output);
    wbxml_buffer_destroy(encoder->output_header);
    wbxml_buffer_destroy(encoder->cdata);

#if defined( WBXML_ENCODER_USE_STRTBL )
    wbxml_list_destroy(encoder->strstbl, wbxml_strtbl_element_destroy_item);
#endif /* WBXML_ENCODER_USE_STRTBL */

#if defined( WBXML_DRMEXT )
	{
		WBXMLNode *pstNode;

		while(encoder->pstTag)
		{
			pstNode = encoder->pstTag;
			encoder->pstTag = encoder->pstTag->next;
			wbxml_free(pstNode->pszName);
			wbxml_free(pstNode);
		}
	
		while(encoder->pstAtt)
		{
			pstNode = encoder->pstAtt;
			encoder->pstAtt = encoder->pstAtt->next;
			wbxml_free(pstNode->pszName);
			wbxml_free(pstNode);
		}
		
		while(encoder->pstAttValues)
		{
			pstNode = encoder->pstAttValues;
			encoder->pstAttValues = encoder->pstAttValues->next;
			wbxml_free(pstNode->pszName);
			wbxml_free(pstNode);
		}
	}
#endif

    wbxml_free(encoder);
}


void wbxml_encoder_reset(WBXMLEncoder *encoder)
{
    if (encoder == NULL)
        return;

    encoder->tree = NULL;

    wbxml_buffer_destroy(encoder->output);
    encoder->output = NULL;
    
    wbxml_buffer_destroy(encoder->output_header);
    encoder->output_header = NULL;
    
    encoder->current_tag = NULL;
    encoder->current_attr = NULL;
    
    encoder->tagCodePage = 0;
    encoder->attrCodePage = 0;
    
    encoder->in_content = FALSE;
    encoder->in_cdata = FALSE;
    
    wbxml_buffer_destroy(encoder->cdata);
    encoder->cdata = NULL;
    
    encoder->pre_last_node_len = 0;

#if defined( WBXML_ENCODER_USE_STRTBL )
    wbxml_list_destroy(encoder->strstbl, wbxml_strtbl_element_destroy_item);
    encoder->strstbl = NULL;
    encoder->strstbl_len = 0;
#endif /* WBXML_ENCODER_USE_STRTBL */
}


void wbxml_encoder_set_ignore_empty_text(WBXMLEncoder *encoder, WB_BOOL set_ignore)
{
    if (encoder == NULL)
        return;

    encoder->ignore_empty_text = set_ignore;
}


void wbxml_encoder_set_remove_text_blanks(WBXMLEncoder *encoder, WB_BOOL set_remove)
{
    if (encoder == NULL)
        return;

    encoder->remove_text_blanks = set_remove;
}


void wbxml_encoder_set_output_charset(WBXMLEncoder *encoder, WBXMLCharsetMIBEnum charset)
{
  if (encoder == NULL)
    return;

  /* Tell which Output Charset Encoding to use (this overides the Charset Encoding found in WBXML Tree) */
  encoder->output_charset = charset;
}


void wbxml_encoder_set_use_strtbl(WBXMLEncoder *encoder, WB_BOOL use_strtbl)
{
#if defined( WBXML_ENCODER_USE_STRTBL )
    if (encoder == NULL)
        return;

    encoder->use_strtbl = use_strtbl;
#endif /* WBXML_ENCODER_USE_STRTBL */
}


void wbxml_encoder_set_wbxml_version(WBXMLEncoder *encoder, WBXMLVersion version)
{
    if (encoder == NULL)
        return;

    if (version != WBXML_VERSION_UNKNOWN)
        encoder->wbxml_version = version;
}


void wbxml_encoder_set_xml_gen_type(WBXMLEncoder *encoder, WBXMLGenXMLType gen_type)
{
    if (encoder == NULL)
        return;

    encoder->xml_gen_type = gen_type;
}


void wbxml_encoder_set_indent(WBXMLEncoder *encoder, WB_UTINY indent)
{
    if (encoder == NULL)
        return;

    encoder->indent_delta = indent;
}


void wbxml_encoder_set_tree(WBXMLEncoder *encoder, WBXMLTree *tree)
{
    if (encoder == NULL)
        return;

    encoder->tree = tree;
}


#if defined( WBXML_DRMEXT )
void wbxml_encoder_set_use_strtbl_literal(WBXMLEncoder *encoder, WB_BOOL use_strtbl_literal)
{
    if (encoder == NULL)
        return;

    encoder->use_strtbl_literal = use_strtbl_literal;
}

void wbxml_encoder_set_record_literals(WBXMLEncoder *encoder, WB_BOOL record_literals)
{
    if (encoder == NULL)
        return;

    encoder->record_literals = record_literals;
}

void wbxml_encoder_set_record_attrvalue_literals(WBXMLEncoder *encoder, WB_BOOL record_attrvalue_literals)
{
    if (encoder == NULL)
        return;

    encoder->record_attrvalue_literals = record_attrvalue_literals;
}

void wbxml_encoder_get_literal_nodes(WBXMLEncoder *encoder, WBXMLGenWBXMLParams *params)
{
    if (encoder == NULL)
        return;

	if (!encoder->record_literals)
		return;

	params->Tags = encoder->Tags;
	params->pstTag = encoder->pstTag;
	encoder->pstTag = NULL;
	
	params->Atts = encoder->Atts;
	params->pstAtt = encoder->pstAtt;
	encoder->pstAtt = NULL;
	
	params->AttValues = encoder->AttValues;
	params->pstAttValues = encoder->pstAttValues;
	encoder->pstAttValues = NULL;
}
#endif


WBXMLError wbxml_encoder_encode_tree_to_wbxml(WBXMLEncoder *encoder, WB_UTINY **wbxml, WB_ULONG *wbxml_len)
{
    WBXMLError ret = WBXML_OK;

    /* Check Parameters */
    if (encoder == NULL)
        return WBXML_ERROR_BAD_PARAMETER;
    
    /* Init ret values */
    *wbxml = NULL;
    *wbxml_len = 0;

    /* We output WBXML */
    wbxml_encoder_set_output_type(encoder, WBXML_ENCODER_OUTPUT_WBXML);

    /* Encode */
    if ((ret = encoder_encode_tree(encoder)) != WBXML_OK)
        return ret;

    /* Get result */
    return wbxml_encoder_get_output(encoder, wbxml, wbxml_len);
}


WBXMLError wbxml_encoder_encode_tree_to_xml(WBXMLEncoder *encoder, WB_UTINY **xml, WB_ULONG *xml_len)
{
    WBXMLError ret = WBXML_OK;

    /* Check Parameters */
    if (encoder == NULL)
        return WBXML_ERROR_BAD_PARAMETER;
    
    /* Init ret values */
    *xml = NULL;
    *xml_len = 0;

    /* We output WBXML */
    wbxml_encoder_set_output_type(encoder, WBXML_ENCODER_OUTPUT_XML);

    /* Encode */
    if ((ret = encoder_encode_tree(encoder)) != WBXML_OK)
        return ret;

    /* Get result */
    return wbxml_encoder_get_output(encoder, xml, xml_len);
}


WBXMLError wbxml_encoder_set_flow_mode(WBXMLEncoder *encoder, WB_BOOL flow_mode)
{
    if (encoder == NULL)
        return WBXML_ERROR_BAD_PARAMETER;
    
    encoder->flow_mode = TRUE;
    
    /* Don't use String Tables */
    wbxml_encoder_set_use_strtbl(encoder, FALSE);
    
    return WBXML_OK;
}


void wbxml_encoder_set_output_type(WBXMLEncoder *encoder, WBXMLEncoderOutputType output_type)
{
    if (encoder == NULL)
        return;
    
    encoder->output_type = output_type;
}


void wbxml_encoder_set_lang(WBXMLEncoder *encoder, WBXMLLanguage lang)
{
    if (encoder == NULL)
        return;

    encoder->lang = wbxml_tables_get_table(lang);
}


void wbxml_encoder_set_text_public_id(WBXMLEncoder *encoder, WB_BOOL gen_text)
{
    if (encoder == NULL)
        return;
    
    encoder->textual_publicid = gen_text;
}


WBXMLError wbxml_encoder_encode_node(WBXMLEncoder *encoder, WBXMLTreeNode *node)
{
    return wbxml_encoder_encode_node_with_elt_end(encoder, node, TRUE);
}


WBXMLError wbxml_encoder_encode_node_with_elt_end(WBXMLEncoder *encoder, WBXMLTreeNode *node, WB_BOOL enc_end)
{
    WB_ULONG   prev_len = 0;
    WBXMLError ret      = WBXML_OK;
    
    if ((encoder == NULL) || (node == NULL))
        return WBXML_ERROR_BAD_PARAMETER;
    
    if (encoder->flow_mode == FALSE) {
        WBXML_WARNING((WBXML_ENCODER, "You should NOT call wbxml_encoder_encode_node() if you are not in Flow Mode encoding ! (use wbxml_encoder_set_flow_mode(encoder, TRUE))"));
    }
    
    /* Check that language table has been set */
    if (encoder->lang == NULL)
        return WBXML_ERROR_BAD_PARAMETER;
    
    /* Init Output Buffer if needed */
    if (!encoder_init_output(encoder))
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;
    
    /* Backup length */
    prev_len = wbxml_buffer_len(encoder->output);
    
    /* Check if result header is not already built */
    if ((encoder->flow_mode == TRUE) && (encoder->output_header == NULL) &&
        !((encoder->xml_encode_header == FALSE) && (encoder->output_type == WBXML_ENCODER_OUTPUT_XML)))
    {
        /* Build result header */
        switch (encoder->output_type) {
        case WBXML_ENCODER_OUTPUT_XML:
            if ((encoder->output_header = wbxml_buffer_create("", 0, WBXML_ENCODER_XML_HEADER_MALLOC_BLOCK)) == NULL)
                ret = WBXML_ERROR_NOT_ENOUGH_MEMORY;
            else
                ret = xml_fill_header(encoder, encoder->output_header);
            break;
            
        case WBXML_ENCODER_OUTPUT_WBXML:
            if ((encoder->output_header = wbxml_buffer_create("", 0, WBXML_ENCODER_WBXML_HEADER_MALLOC_BLOCK)) == NULL)
                ret = WBXML_ERROR_NOT_ENOUGH_MEMORY;
            else
                ret = wbxml_fill_header(encoder, encoder->output_header);
            break;
            
        default:
            ret = WBXML_ERROR_BAD_PARAMETER;
            break;
        }
    }
    
    if (ret != WBXML_OK)
        return ret;
    
    if ((ret = parse_node(encoder, node, enc_end)) == WBXML_OK)
        encoder->pre_last_node_len = prev_len;
    
    return ret;
}


WBXMLError wbxml_encoder_encode_tree(WBXMLEncoder *encoder, WBXMLTree *tree)
{
    const WBXMLLangEntry *lang = NULL;
    WBXMLError            ret  = WBXML_OK;
    
    if ((encoder == NULL) || (tree == NULL))
        return WBXML_ERROR_BAD_PARAMETER;
    
    /* Backup encoder lang */
    lang = encoder->lang;
    
    /* Set Tree lang to encoder */
    encoder->lang = tree->lang;
    
    /* Encode root node */
    ret = wbxml_encoder_encode_node(encoder, tree->root);
    
    /* Revert encoder lang */
    encoder->lang = lang;
    
    return ret;
}


WBXMLError wbxml_encoder_encode_raw_elt_start(WBXMLEncoder *encoder, WBXMLTreeNode *node, WB_BOOL has_content)
{
    /* Init Output Buffer if needed */
    if (!encoder_init_output(encoder))
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;
    
    return parse_element(encoder, node, has_content);
}


WBXMLError wbxml_encoder_encode_raw_elt_end(WBXMLEncoder *encoder, WBXMLTreeNode *node, WB_BOOL has_content)
{
    /* Init Output Buffer if needed */
    if (!encoder_init_output(encoder))
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;
    
    return parse_element_end(encoder, node, has_content);
}


WBXMLError wbxml_encoder_get_output(WBXMLEncoder *encoder, WB_UTINY **result, WB_ULONG *result_len)
{
    if ((encoder == NULL) || (result == NULL) || (result_len == NULL))
        return WBXML_ERROR_BAD_PARAMETER;
    
    switch (encoder->output_type) {
    case WBXML_ENCODER_OUTPUT_XML:
        return xml_build_result(encoder, result, result_len);
        
    case WBXML_ENCODER_OUTPUT_WBXML:
        return wbxml_build_result(encoder, result, result_len);
        
    default:
        return WBXML_ERROR_BAD_PARAMETER;
    }
}


WB_ULONG wbxml_encoder_get_output_len(WBXMLEncoder *encoder)
{
    if (encoder == NULL)
        return 0;
    
    return wbxml_buffer_len(encoder->output_header) + wbxml_buffer_len(encoder->output);
}


void wbxml_encoder_delete_output_bytes(WBXMLEncoder *encoder, WB_ULONG nb)
{
    if (encoder == NULL)
        return;
    
    wbxml_buffer_delete(encoder->output, wbxml_buffer_len(encoder->output) - nb, nb);
}


void wbxml_encoder_delete_last_node(WBXMLEncoder *encoder)
{
    if (encoder == NULL)
        return;
    
    wbxml_encoder_delete_output_bytes(encoder, wbxml_buffer_len(encoder->output) - encoder->pre_last_node_len);
}


/***************************************************
 *    Private Functions
 */

/****************************
 * Common Functions
 */

#if 0
/**
 * @brief Convert a char to UCS-4
 * @param ch [in] The character to convert
 * @param result [out] The UCS-4 code
 * @return TRUE if convertion succeeded, FALSE otherwise
 */
static WB_BOOL convert_char_to_ucs4(WB_UTINY ch, WB_ULONG *result)
{
    /** @todo convert_char_to_ucs4() */

    return FALSE;
}
#endif /* 0 */

/**
 * @brief Duplicate a WBXML Encoder
 * @param encoder [in] The WBXML Encoder to Duplicate
 * @return The duplicated WBXML Encoder, or NULL if error
 * @note Only options (parameters) fields are duplicated, others are reset
 */
static WBXMLEncoder *encoder_duplicate(WBXMLEncoder *encoder)
{
    WBXMLEncoder *result = NULL;

    if ((result = wbxml_encoder_create()) == NULL)
        return NULL;

    result->ignore_empty_text = encoder->ignore_empty_text;
    result->remove_text_blanks = encoder->remove_text_blanks;

    result->output_type = encoder->output_type;
    result->xml_gen_type = encoder->xml_gen_type;

    result->indent_delta = encoder->indent_delta;
    result->indent = encoder->indent;

#if defined( WBXML_ENCODER_USE_STRTBL )
    result->use_strtbl = encoder->use_strtbl;
#endif /* WBXML_ENCODER_USE_STRTBL */

    /* Do NOT generate XML Header */
    result->xml_encode_header = FALSE;

    result->wbxml_version = encoder->wbxml_version;

    return result;
}


static WBXMLError encoder_encode_tree(WBXMLEncoder *encoder)
{
    WBXMLError ret = WBXML_OK;

    /* Check Parameters */
    if ((encoder == NULL) || (encoder->tree == NULL) || ((encoder->lang == NULL) && (encoder->tree->lang == NULL)) ||
        ((encoder->output_type != WBXML_ENCODER_OUTPUT_XML) && (encoder->output_type != WBXML_ENCODER_OUTPUT_WBXML)))
    {
        return WBXML_ERROR_BAD_PARAMETER;
    }
    
    if (encoder->lang == NULL)
        encoder->lang = encoder->tree->lang;

    /* Choose Output Charset */
    if (encoder->output_charset == WBXML_CHARSET_UNKNOWN) {
        /* User has not choosen the Output Charset Encoding */
        if (encoder->tree->orig_charset != WBXML_CHARSET_UNKNOWN) {
            /* Use the original Charset Encoding found when we have parsed the original document */
            encoder->output_charset = encoder->tree->orig_charset;
        }
        else {
            /* Use default charset */
            encoder->output_charset = WBXML_ENCODER_XML_DEFAULT_CHARSET;
        }
    }

    /* Init Output Buffer */
    if (!encoder_init_output(encoder)) {
        wbxml_encoder_destroy(encoder);
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;
    }
    
#if defined( WBXML_ENCODER_USE_STRTBL )
#if defined( WBXML_DRMEXT )
	if (!encoder->use_strtbl_literal)
	{
#endif
    if (encoder->output_type == WBXML_ENCODER_OUTPUT_WBXML) {
        /* Choose if we will use String Table */
        switch (encoder->lang->langID)
        {
        default:
            /* Use Default Value */
            break;
        }

        /* Init String Table */
        if (encoder->use_strtbl) {
            /**
             * @bug If 'output_charset' is different from UTF-8, the string table initialization
             *      also is erroneous !!!
             */
            if ((ret = wbxml_strtbl_initialize(encoder, encoder->tree->root)) != WBXML_OK)
                return ret;
        }
    }
#if defined( WBXML_DRMEXT )
	}
#endif    
#endif /* WBXML_ENCODER_USE_STRTBL */

    /* Let's begin WBXML Tree Parsing */
    return parse_node(encoder, encoder->tree->root, TRUE);
}


static WB_BOOL encoder_init_output(WBXMLEncoder *encoder)
{
    WB_ULONG malloc_block = 0;
    
    if (encoder == NULL)
        return FALSE;
    
    /* Check if output already inited */
    if (encoder->output != NULL)
        return TRUE;
    
    /* Get malloc block */
    if (encoder->output_type == WBXML_ENCODER_OUTPUT_WBXML)
        malloc_block = WBXML_ENCODER_WBXML_DOC_MALLOC_BLOCK;
    else
        malloc_block = WBXML_ENCODER_XML_DOC_MALLOC_BLOCK;

    /* Init Output Buffer */
    encoder->output = wbxml_buffer_create("", 0, malloc_block);
    if (encoder->output == NULL)
        return FALSE;
    
    return TRUE;
}

#if defined( WBXML_DRMEXT )
static WBXMLError encoder_add_attr_value(WBXMLEncoder *encoder, WB_UTINY *value)
{
	WBXMLNode *pstNew = encoder->pstAttValues;

	while (pstNew){
		if (!WBXML_STRCMP((const char *)pstNew->pszName, (const char *)value))
			/* Redundant node */
			return WBXML_OK;
		
		pstNew = pstNew->next;
	}
	
	pstNew = wbxml_malloc(sizeof(WBXMLNode));
	if (!pstNew) {
		 return WBXML_ERROR_NOT_ENOUGH_MEMORY;
	}
	pstNew->pszName = (unsigned char *)wbxml_strdup((const char*)value);
	if (!pstNew->pszName) {
		wbxml_free(pstNew);
		 return WBXML_ERROR_NOT_ENOUGH_MEMORY;
	}
	
	/* Add to head */
	pstNew->next = encoder->pstAttValues;
	encoder->pstAttValues = pstNew;
	encoder->AttValues++;

	return WBXML_OK;
}
#endif

/*********************************
 * WBXML Tree Parsing Functions
 */

/**
 * @brief Parse an XML Node
 * @param encoder The WBXML Encoder
 * @param node    The node to parse
 * @param enc_end If node is an element, do we encoded its end ?
 * @return WBXML_OK if parsing is OK, an error code otherwise
 * @note We have recurrency in this function
 */
static WBXMLError parse_node(WBXMLEncoder *encoder, WBXMLTreeNode *node, WB_BOOL enc_end)
{
    WBXMLError ret = WBXML_OK;

    /* Parse this node */
    switch (node->type) {
        case WBXML_TREE_ELEMENT_NODE:
            ret = parse_element(encoder, node, (WB_BOOL)(node->children != NULL));
            break;
        case WBXML_TREE_TEXT_NODE:
            ret = parse_text(encoder, node);
            break;
        case WBXML_TREE_CDATA_NODE:
            ret = parse_cdata(encoder, node);
            break;
        case WBXML_TREE_PI_NODE:
            ret = parse_pi(encoder, node);
            break;
        case WBXML_TREE_TREE_NODE:
            ret = parse_tree(encoder, node);
            break;
        default:
            return WBXML_ERROR_XML_NODE_NOT_ALLOWED;
    }

    if (ret != WBXML_OK)
        return ret;

    /* Check if node has children */
    if (node->children != NULL) {
        /* Parse Child */
        if ((ret = parse_node(encoder, node->children, TRUE)) != WBXML_OK)
            return ret;
    }

    /* Handle end of Element or CDATA section */
    switch (node->type) {
    case WBXML_TREE_ELEMENT_NODE:
        if (enc_end) {
            switch(encoder->output_type) {
            case WBXML_ENCODER_OUTPUT_XML:
#if defined( WBXML_ENCODER_XML_GEN_EMPTY_ELT )
                if (node->children != NULL) {
#endif /* WBXML_ENCODER_XML_GEN_EMPTY_ELT */

                    /* Encode end tag */
                    if ((ret = xml_encode_end_tag(encoder, node)) != WBXML_OK)
                        return ret;

                    WBXML_DEBUG((WBXML_ENCODER, "End Element"));

#if defined( WBXML_ENCODER_XML_GEN_EMPTY_ELT )
                }
#endif /* WBXML_ENCODER_XML_GEN_EMPTY_ELT */
                break;

            case WBXML_ENCODER_OUTPUT_WBXML:
                if (node->children != NULL) {
                    /* Add a WBXML End tag */
                    if ((ret = wbxml_encode_end(encoder)) != WBXML_OK)
                        return ret;

                    WBXML_DEBUG((WBXML_ENCODER, "End Element"));
                }
                break;

            default:
                /* hu ? */
                break;
            } /* switch */
        } /* if */
        break;

    case WBXML_TREE_CDATA_NODE:
        /* End of CDATA section */
        encoder->in_cdata = FALSE;

        WBXML_DEBUG((WBXML_ENCODER, "End CDATA"));

        switch(encoder->output_type) {
        case WBXML_ENCODER_OUTPUT_XML:
            /* Encode XML "End of CDATA section" */
            if ((ret = xml_encode_end_cdata(encoder)) != WBXML_OK)
                return ret;
            break;

        case WBXML_ENCODER_OUTPUT_WBXML:
            if (encoder->cdata == NULL) {
                /* Must never happen */
                return WBXML_ERROR_INTERNAL;
            }

            /* Encode CDATA Buffer into Opaque */
            if (wbxml_buffer_len(encoder->cdata) > 0) {
                if ((ret = wbxml_encode_opaque(encoder, encoder->cdata)) != WBXML_OK)
                    return ret;
            }

            /* Reset CDATA Buffer */
            wbxml_buffer_destroy(encoder->cdata);
            encoder->cdata = NULL;
            break;

        default:
            /* hu ? */
            break;
        } /* switch */
        break;

    default:
        /* NOP */
        break;
    }

    /* Reset Current Tag */
    encoder->current_tag = NULL;

    /* Parse next node */
    if (node->next != NULL)
        return parse_node(encoder, node->next, TRUE);
    else
        return WBXML_OK;
}


/**
 * @brief Parse an XML Element
 * @param encoder The WBXML Encoder
 * @param node The element to parse
 * @param has_content Does the element has content ?
 * @return WBXML_OK if parsing is OK, an error code otherwise
 */
static WBXMLError parse_element(WBXMLEncoder *encoder, WBXMLTreeNode *node, WB_BOOL has_content)
{
    WB_ULONG i = 0;
    WBXMLError ret = WBXML_OK;

    WBXML_DEBUG((WBXML_ENCODER, "Element: <%s>", wbxml_tag_get_xml_name(node->name)));

    /* Encode: Element Name */
    switch (encoder->output_type) {
    case WBXML_ENCODER_OUTPUT_WBXML:
        if ((ret = wbxml_encode_tag(encoder, node, has_content)) != WBXML_OK)
            return ret;
        break;

    case WBXML_ENCODER_OUTPUT_XML:
        if ((ret = xml_encode_tag(encoder, node)) != WBXML_OK)
            return ret;
        break;

    default:
        return WBXML_ERROR_INTERNAL;
    }

    /** @todo Check handling of Namespaces */

    /** @todo For Canonical XML Output: Attributes MUST be sorted */

    /* Parse: Attributes List */
    if (node->attrs != NULL)
    {
        for (i = 0; i < wbxml_list_len(node->attrs); i++) {
            /* Parse: Attribute */
            /* wbt : "wbxml_list_get(node->attrs, i)"could not be  NULL, through pre-checking "node->attrs != NULL" and "i < wbxml_list_len(node->attrs)" */
            if ((ret = parse_attribute(encoder, wbxml_list_get(node->attrs, i))) != WBXML_OK)
                return ret;
        }
    }

    /* Encode: End of attributes */
    switch (encoder->output_type) {
    case WBXML_ENCODER_OUTPUT_WBXML:
        /** @todo Check if the attributes will really be tokenized. There can be ignored attributes, and so NO attribute
         *        tokenized at all.
         */
        if ((node->attrs != NULL) && (encoder->lang->attrTable != NULL) /** @todo Fast patch for SyncML (change this) */) {
            if ((ret = wbxml_encode_end(encoder)) != WBXML_OK)
                return ret;

            WBXML_DEBUG((WBXML_ENCODER, "End Attributes"));
        }
        break;

    case WBXML_ENCODER_OUTPUT_XML:
        /* Encode end of attributes */
        if ((ret = xml_encode_end_attrs(encoder, node)) != WBXML_OK)
            return ret;

        WBXML_DEBUG((WBXML_ENCODER, "End Attributes"));
        break;

    default:
        return WBXML_ERROR_INTERNAL;
    }

    return WBXML_OK;
}


/**
 * @brief Parse an Element End
 * @param encoder The WBXML Encoder
 * @param node The element to parse
 * @param has_content Does the element has content ?
 * @return WBXML_OK if parsing is OK, an error code otherwise
 */
static WBXMLError parse_element_end(WBXMLEncoder *encoder, WBXMLTreeNode *node, WB_BOOL has_content)
{
    WBXMLError ret = WBXML_OK;
    
    if (encoder->output_type == WBXML_ENCODER_OUTPUT_XML) {
#if defined( WBXML_ENCODER_XML_GEN_EMPTY_ELT )
        if (has_content) {
#endif /* WBXML_ENCODER_XML_GEN_EMPTY_ELT */

            /* Encode end tag */
            ret = xml_encode_end_tag(encoder, node);

            WBXML_DEBUG((WBXML_ENCODER, "End Element"));

#if defined( WBXML_ENCODER_XML_GEN_EMPTY_ELT )
        }
#endif /* WBXML_ENCODER_XML_GEN_EMPTY_ELT */
    }
    else if (encoder->output_type == WBXML_ENCODER_OUTPUT_WBXML) {
        if (has_content) {
            /* Add a WBXML End tag */
            ret = wbxml_encode_end(encoder);

            WBXML_DEBUG((WBXML_ENCODER, "End Element"));
        }
    }
    
    return ret;
}


/**
 * @brief Parse an XML Attribute
 * @param encoder The WBXML Encoder
 * @param attribute The XML Attribute to parse
 * @return WBXML_OK if parsing is OK, an error code otherwise
 */
static WBXMLError parse_attribute(WBXMLEncoder *encoder, WBXMLAttribute *attribute)
{
    // WBT.20100428
    if (encoder == NULL || attribute == NULL)
        return WBXML_ERROR_NULL_PARSER;

    if (encoder->lang == NULL)
        return WBXML_ERROR_LANG_TABLE_UNDEFINED;

    if (encoder->lang->attrTable == NULL)
        return WBXML_OK;

    /* Check that this attribute has a name */
    if (attribute->name == NULL)
        return WBXML_ERROR_XML_NULL_ATTR_NAME;

    WBXML_DEBUG((WBXML_ENCODER, "Attribute: %s = %s", wbxml_attribute_get_xml_name(attribute), wbxml_attribute_get_xml_value(attribute)));

    /* Encode: Attribute */
    switch (encoder->output_type) {
    case WBXML_ENCODER_OUTPUT_WBXML:
        return wbxml_encode_attr(encoder, attribute);

    case WBXML_ENCODER_OUTPUT_XML:
        return xml_encode_attr(encoder, attribute);

    default:
        return WBXML_ERROR_INTERNAL;
    }
}


/**
 * @brief Parse an XML Text
 * @param encoder The WBXML Encoder
 * @param node The text to parse
 * @return WBXML_OK if parsing is OK, an error code otherwise
 */
static WBXMLError parse_text(WBXMLEncoder *encoder, WBXMLTreeNode *node)
{
    WBXMLError ret = WBXML_OK;
    
    /* Do not modify text inside a CDATA section */
    if (!encoder->in_cdata) {
        /* If Canonical Form: "Ignorable white space is considered significant and is treated equivalently to data" */
        if ((encoder->output_type != WBXML_ENCODER_OUTPUT_XML) || (encoder->xml_gen_type != WBXML_GEN_XML_CANONICAL)) {
            /* Ignore blank nodes */
            if ((encoder->ignore_empty_text) && (wbxml_buffer_contains_only_whitespaces(node->content)))
                return WBXML_OK;

            /* Strip Blanks */
            if (encoder->remove_text_blanks)
                wbxml_buffer_strip_blanks(node->content);
        }
    }

    WBXML_DEBUG((WBXML_ENCODER, "Text: <%s>", wbxml_buffer_get_cstr(node->content)));

    /* Encode Text */
    switch (encoder->output_type) {
    case WBXML_ENCODER_OUTPUT_WBXML:
        if (encoder->in_cdata) {
            if (encoder->cdata == NULL) {
                /* Must never happen */
                return WBXML_ERROR_INTERNAL;
            }

            /* Add text into CDATA Buffer */
            if (!wbxml_buffer_append(encoder->cdata, node->content))
                return WBXML_ERROR_ENCODER_APPEND_DATA;

            return WBXML_OK;
        }
        else {
            /* Encode text */
            encoder->current_text_parent = node->parent;
            ret = wbxml_encode_value_element_buffer(encoder, wbxml_buffer_get_cstr(node->content), WBXML_VALUE_ELEMENT_CTX_CONTENT);
            encoder->current_text_parent = NULL;
            return ret;
        }

    case WBXML_ENCODER_OUTPUT_XML:
        return xml_encode_text(encoder, node);

    default:
        return WBXML_ERROR_INTERNAL;
    }
}


/**
 * @brief Parse an XML CDATA
 * @param encoder The WBXML Encoder
 * @param node The CDATA to parse
 * @return WBXML_OK if parsing is OK, an error code otherwise
 */
static WBXMLError parse_cdata(WBXMLEncoder *encoder, WBXMLTreeNode *node)
{
    WBXML_DEBUG((WBXML_ENCODER, "CDATA Begin"));

    /* Keep in mind that we are in a CDATA section */
    encoder->in_cdata = TRUE;

    /* Encode CDATA */
    switch (encoder->output_type) {
    case WBXML_ENCODER_OUTPUT_WBXML:
        if (encoder->cdata != NULL) {
            /* Must never happend */
            return WBXML_ERROR_INTERNAL;
        }

        /* Create a new CDATA Buffer */
        if ((encoder->cdata = wbxml_buffer_create("", 0, 0)) == NULL) {
            return WBXML_ERROR_NOT_ENOUGH_MEMORY;
        }

        return WBXML_OK;
    case WBXML_ENCODER_OUTPUT_XML:
        return xml_encode_cdata(encoder);
    default:
        return WBXML_ERROR_INTERNAL;
    }
}


/**
 * @brief Parse an XML PI
 * @param encoder The WBXML Encoder
 * @param node The PI to parse
 * @return WBXML_OK if parsing is OK, an error code otherwise
 */
static WBXMLError parse_pi(WBXMLEncoder *encoder, WBXMLTreeNode *node)
{
    /** @todo parse_pi() */

    return WBXML_ERROR_NOT_IMPLEMENTED;
}


/**
 * @brief Parse a WBXML Tree
 * @param encoder The WBXML Encoder
 * @param node The WBXML Tree to parse
 * @return WBXML_OK if parsing is OK, an error code otherwise
 */
static WBXMLError parse_tree(WBXMLEncoder *encoder, WBXMLTreeNode *node)
{
    switch (encoder->output_type) {
    case WBXML_ENCODER_OUTPUT_WBXML:
        return wbxml_encode_tree(encoder, node->tree);
    case WBXML_ENCODER_OUTPUT_XML:
        return xml_encode_tree(encoder, node->tree);
    default:
        return WBXML_ERROR_INTERNAL;
    }
}


/*****************************************
 *  WBXML Output Functions
 */

/****************************
 * Build WBXML Result
 */

/**
 * @brief Build WBXML Result
 * @param encoder [in] The WBXML Encoder
 * @param wbxml [out] Resulting WBXML document
 * @param wbxml_len [out] The resulting WBXML document length
 * @return WBXML_OK if built is OK, an error code otherwise
 * @note WBXML Header = version publicid charset length
 */
static WBXMLError wbxml_build_result(WBXMLEncoder *encoder, WB_UTINY **wbxml, WB_ULONG *wbxml_len)
{
    WBXMLBuffer *header = NULL;
    WBXMLError ret = WBXML_OK;
    
    if (encoder->flow_mode == TRUE) {
        /* Header already built */
        header = encoder->output_header;
    }
    else {
        /* Create WBXML Header buffer */
        if ((header = wbxml_buffer_create("", 0, WBXML_ENCODER_WBXML_HEADER_MALLOC_BLOCK)) == NULL)
            return WBXML_ERROR_NOT_ENOUGH_MEMORY;
        
        /* Fill Header Buffer */
        if ((ret = wbxml_fill_header(encoder, header)) != WBXML_OK) {
            wbxml_buffer_destroy(header);
            return ret;
        }
    }

    /* Result Buffer Length */
    *wbxml_len = wbxml_buffer_len(header) + wbxml_buffer_len(encoder->output);

    /* Create Result Buffer */
    *wbxml = (WB_UTINY *) wbxml_malloc(*wbxml_len * sizeof(WB_UTINY));
    if (*wbxml == NULL) {
        if (encoder->flow_mode == FALSE)
            wbxml_buffer_destroy(header);
        
        *wbxml_len = 0;
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;
    }

    /* Copy WBXML Header */
    memcpy(*wbxml, wbxml_buffer_get_cstr(header), wbxml_buffer_len(header));

    /* Copy WBXML Buffer */
    memcpy(*wbxml + wbxml_buffer_len(header), wbxml_buffer_get_cstr(encoder->output), wbxml_buffer_len(encoder->output));

    if (encoder->flow_mode == FALSE)
        wbxml_buffer_destroy(header);

    return WBXML_OK;
}


static WBXMLError wbxml_fill_header(WBXMLEncoder *encoder, WBXMLBuffer *header)
{
    WBXMLBuffer *pid = NULL;
    WB_ULONG public_id = 0, public_id_index = 0, strstbl_len = 0;
    WB_BOOL pi_in_strtbl = FALSE;
    WBXMLError ret = WBXML_OK;

#if defined( WBXML_ENCODER_USE_STRTBL )
    WBXMLStringTableElement *elt = NULL;
    WB_BOOL added = FALSE;

    //strstbl_len = encoder->strstbl_len; WBT.20100128
#endif /* WBXML_ENCODER_USE_STRTBL */

    if ((encoder == NULL) || (encoder->lang == NULL) || (encoder->lang->publicID == NULL) || (header == NULL))
        return WBXML_ERROR_BAD_PARAMETER;

    // WBT.20100128
#if defined( WBXML_ENCODER_USE_STRTBL )
    strstbl_len = encoder->strstbl_len;
#endif /* WBXML_ENCODER_USE_STRTBL */

    /* WBXML Public ID */
    public_id = encoder->lang->publicID->wbxmlPublicID;

    /* Encode WBXML Version */
    if (!wbxml_buffer_append_char(header, (WB_UTINY) encoder->wbxml_version))
        return WBXML_ERROR_ENCODER_APPEND_DATA;

    /* Encode Public ID */
    /* If WBXML Public Id is '0x01' (unknown), or we forced it, add the XML Public ID in the String Table */
    if (encoder->textual_publicid || (public_id == WBXML_PUBLIC_ID_UNKNOWN))
    {
        if (encoder->lang->publicID->xmlPublicID != NULL)
        {
            if ((pid = wbxml_buffer_create(encoder->lang->publicID->xmlPublicID,
                                           WBXML_STRLEN(encoder->lang->publicID->xmlPublicID),
                                           WBXML_STRLEN(encoder->lang->publicID->xmlPublicID))) == NULL)
            {
                return WBXML_ERROR_NOT_ENOUGH_MEMORY;
            }

#if defined( WBXML_ENCODER_USE_STRTBL )
            if (encoder->use_strtbl) {
                if ((elt = wbxml_strtbl_element_create(pid, FALSE)) == NULL)
                {
                    wbxml_buffer_destroy(pid);
                    return WBXML_ERROR_NOT_ENOUGH_MEMORY;
                }

                if (!wbxml_strtbl_add_element(encoder,
                                              elt,
                                              &public_id_index,
                                              &added))
                {
                    wbxml_strtbl_element_destroy(elt);
                    return WBXML_ERROR_NOT_ENOUGH_MEMORY;
                }

                if (!added)
                    wbxml_strtbl_element_destroy(elt);

                strstbl_len = encoder->strstbl_len;
            }
            else {
#endif /* WBXML_ENCODER_USE_STRTBL */
                /* Length of String Table is length of XML Public ID */
                strstbl_len = wbxml_buffer_len(pid);

                /* There is only the XML Public ID in String Table */
                public_id_index = 0;
#if defined( WBXML_ENCODER_USE_STRTBL )
            }
#endif /* WBXML_ENCODER_USE_STRTBL */

            pi_in_strtbl = TRUE;
        }
    }

    /* publicid = mb_u_int32 | ( zero index ) */
    if (pi_in_strtbl) {
        /* Encode XML Public ID String Table index */
        if (!wbxml_buffer_append_char(header, 0x00) ||
            !wbxml_buffer_append_mb_uint_32(header, public_id_index))
        {
            return WBXML_ERROR_NOT_ENOUGH_MEMORY;
        }
    }
    else {
        /* Encode WBXML Public ID */
        if (!wbxml_buffer_append_mb_uint_32(header, public_id))
            return WBXML_ERROR_ENCODER_APPEND_DATA;
    }

    /* Encode Charset (default: UTF-8) and String Table Length */
    /** @todo Handle correctly the charset */
    if (!wbxml_buffer_append_mb_uint_32(header, WBXML_ENCODER_DEFAULT_CHARSET) ||
        !wbxml_buffer_append_mb_uint_32(header, strstbl_len))
    {
        return WBXML_ERROR_ENCODER_APPEND_DATA;
    }

    /* Copy WBXML String Table */
#if defined( WBXML_ENCODER_USE_STRTBL )
    if (encoder->use_strtbl) {
        if ((ret = wbxml_strtbl_construct(header, encoder->strstbl)) != WBXML_OK)
            return ret;
    }
    else {
#endif /* WBXML_ENCODER_USE_STRTBL */

        if (pid != NULL) {
            /* The append includes terminating NULL char */
            if (!wbxml_buffer_append(header, pid))
                return WBXML_ERROR_ENCODER_APPEND_DATA;
            
            /* Clean up */
            wbxml_buffer_destroy(pid);
        }
            
#if defined( WBXML_ENCODER_USE_STRTBL )
    }
#endif /* WBXML_ENCODER_USE_STRTBL */

    return WBXML_OK;
}


/****************************
 * WBXML Encoding Functions
 */

/**
 * @brief Encode a WBXML End Token
 * @param encoder The WBXML Encoder
 * @return WBXML_OK if encoding is OK, an error code otherwise
 */
static WBXMLError wbxml_encode_end(WBXMLEncoder *encoder)
{
    /* Append END */
    if (!wbxml_buffer_append_char(encoder->output, WBXML_END))
        return WBXML_ERROR_ENCODER_APPEND_DATA;

    return WBXML_OK;
}


/**
 * @brief Encode a WBXML Tag
 * @param encoder The WBXML Encoder
 * @param node The element to encode
 * @param has_content Does the element has content ?
 * @return WBXML_OK if encoding is OK, an error code otherwise
 */
static WBXMLError wbxml_encode_tag(WBXMLEncoder *encoder, WBXMLTreeNode *node, WB_BOOL has_content)
{
    const WBXMLTagEntry *tag = NULL;
    WB_UTINY token = 0x00, page = 0x00;

    if (node->name->type == WBXML_VALUE_TOKEN) {
        token = node->name->u.token->wbxmlToken;
        page = node->name->u.token->wbxmlCodePage;

        /* Current Tag */
        encoder->current_tag = node->name->u.token;
    }
    else {
        /* Search tag in Tags Table */
        if ((tag = wbxml_tables_get_tag_from_xml(encoder->lang, wbxml_tag_get_xml_name(node->name))) != NULL)
        {
            token = tag->wbxmlToken;
            page = tag->wbxmlCodePage;

            /* Current Tag */
            encoder->current_tag = tag;
        }
        else
            encoder->current_tag = NULL;
    }

    /* Check if this element has content */
    if (has_content)
        token |= WBXML_TOKEN_WITH_CONTENT;

    /* Check if this element has attributes */
    /** @todo Check if the attributes will really be tokenized. There can be ignored attributes, and so NO attribute
     *        tokenized at all.
     */
    if ((node->attrs != NULL) && (encoder->lang->attrTable != NULL) /** @todo Fast patch for SyncML (change this) */)
        token |= WBXML_TOKEN_WITH_ATTRS;

    /* Encode Token */
    if ((token & WBXML_TOKEN_MASK) == 0x00)
        return wbxml_encode_tag_literal(encoder, (WB_UTINY *) wbxml_tag_get_xml_name(node->name), token);
    else
        return wbxml_encode_tag_token(encoder, token, page);
}


/**
 * @brief Encode a WBXML Literal Token
 * @param encoder The WBXML Encoder
 * @param tag The literal tag to encode
 * @param mask The WBXML Mask for this tag
 * @return WBXML_OK if encoding is OK, an error code otherwise
 * @note    stag = (literalTag index)
 *          literalTag = LITERAL | LITERAL_A | LITERAL_C | LITERAL_AC
 *          index = mb_u_int32
 */
static WBXMLError wbxml_encode_tag_literal(WBXMLEncoder *encoder, WB_UTINY *tag, WB_UTINY mask)
{
#if defined( WBXML_ENCODER_USE_STRTBL )
    WBXMLStringTableElement *elt = NULL;
    WBXMLBuffer *buff = NULL;
    WB_ULONG index = 0;
    WB_BOOL added = FALSE;

    /* If String Table generation is disabled, we can't generate this Literal Tag */
    if (!encoder->use_strtbl)
        return WBXML_ERROR_STRTBL_DISABLED;

    /* Add tag in String Table */
    if (((buff = wbxml_buffer_create(tag, WBXML_STRLEN((const char *)tag), WBXML_STRLEN((const char *)tag))) == NULL) ||
        ((elt = wbxml_strtbl_element_create(buff, FALSE)) == NULL) ||
        (!wbxml_strtbl_add_element(encoder, elt, &index, &added)))
    {
        wbxml_strtbl_element_destroy(elt);
        wbxml_buffer_destroy(buff);
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;
    }

    /* If already exists in String Table: clean-up */
    if (!added)
        wbxml_strtbl_element_destroy(elt);
#if defined( WBXML_DRMEXT )
	else if (encoder->record_literals) {
		WBXMLNode *pstNew = wbxml_malloc(sizeof(WBXMLNode));
		if (!pstNew) {
			 wbxml_strtbl_element_destroy(elt);
			 return WBXML_ERROR_NOT_ENOUGH_MEMORY;
		}
		pstNew->pszName = (unsigned char*)wbxml_strdup((const char*)tag);
		if (!pstNew->pszName) {
			wbxml_free(pstNew);
			 wbxml_strtbl_element_destroy(elt);
			 return WBXML_ERROR_NOT_ENOUGH_MEMORY;
		}
		
		/* Add to head */
		pstNew->next = encoder->pstTag;
		encoder->pstTag = pstNew;
		encoder->Tags++;
	}
#endif

    /* Encode literalTag index */
    if ((!wbxml_buffer_append_char(encoder->output, (WB_UTINY) (WBXML_LITERAL | mask))) ||
        (!wbxml_buffer_append_mb_uint_32(encoder->output, index)))
    {
        return WBXML_ERROR_ENCODER_APPEND_DATA;
    }

    return WBXML_OK;
#else
    /* No String Table Support */
    return WBXML_ERROR_STRTBL_DISABLED;
#endif /* WBXML_ENCODER_USE_STRTBL */
}


/**
 * @brief Encode a WBXML Tag Token
 * @param encoder The WBXML Encoder
 * @param token The WBXML Tag Token to encode
 * @param page The WBXML CodePage for this Token
 * @return WBXML_OK if encoding is OK, an error code otherwise
 * @note    element = ([switchPage] stag)
 *          switchPage = SWITCH_PAGE pageindex
 *          stag = TAG
 *          pageindex = u_int8
 */
static WBXMLError wbxml_encode_tag_token(WBXMLEncoder *encoder, WB_UTINY token, WB_UTINY page)
{
    /* Switch Page if needed */
    if (encoder->tagCodePage != page)
    {
        if ((!wbxml_buffer_append_char(encoder->output, WBXML_SWITCH_PAGE)) ||
            (!wbxml_buffer_append_char(encoder->output, page)))
        {
            return WBXML_ERROR_ENCODER_APPEND_DATA;
        }

        encoder->tagCodePage = page;
    }

    /* Add Token */
    if (!wbxml_buffer_append_char(encoder->output, token))
        return WBXML_ERROR_ENCODER_APPEND_DATA;

    return WBXML_OK;
}


/**
 * @brief Encode a WBXML Attribute
 * @param encoder [in] The WBXML Encoder
 * @param attribute [in] The Attribute to encode
 * @return WBXML_OK if encoding is OK, an error code otherwise
 * @note    attribute = attrStart *attrValue
 */
static WBXMLError wbxml_encode_attr(WBXMLEncoder *encoder, WBXMLAttribute *attribute)
{
    WB_UTINY *value = NULL;
    WBXMLError ret = WBXML_OK;

    /* Encode Attribute Start */
    if ((ret = wbxml_encode_attr_start(encoder, attribute, &value)) != WBXML_OK)
        return ret;

    /* Encode Attribute Value */
    if (value != NULL) {
        if ((ret = wbxml_encode_value_element_buffer(encoder, value, WBXML_VALUE_ELEMENT_CTX_ATTR)) != WBXML_OK)
            return ret;
    }

    /* Reset Current Attribute */
    encoder->current_attr = NULL;

    return WBXML_OK;
}


/**
 * @brief Encode a WBXML Attribute Start
 * @param encoder [in] The WBXML Encoder
 * @param attribute [in] The Attribute
 * @param value [out] Pointer to the value to encode
 * @return WBXML_OK if encoding is OK, an error code otherwise
 * @note The 'value' result correspond to the value there is still to encode
 *       For example, in Wireless-Village, to encode:
 *              xmlns="http://www.wireless-village.org/CSP1.1"
 *       We first encode:
 *              xmlns="http://www.wireless-village.org/CSP (start token: 0x05)
 *       Then:
 *              "1.1" as an inline string
 */
static WBXMLError wbxml_encode_attr_start(WBXMLEncoder *encoder, WBXMLAttribute *attribute, WB_UTINY **value)
{
    const WBXMLAttrEntry *attr = NULL;
    WB_UTINY *value_left = NULL;
    WB_UTINY token = 0x00, page = 0x00;

    *value = wbxml_buffer_get_cstr(attribute->value);

    if (attribute->name->type == WBXML_VALUE_TOKEN) {
        /* We already have Token / Page pair for this Attribute Start */
        token = attribute->name->u.token->wbxmlToken;
        page = attribute->name->u.token->wbxmlCodePage;

        /* Current Attribute */
        encoder->current_attr = attribute->name->u.token;

        /* If there is a Start Value associated to the Attribute Name token... */
        if (attribute->name->u.token->xmlValue != NULL)
        {
            /* ... Check that we have it at start of full Attribute Value */
            if (WBXML_STRNCMP((const char *)wbxml_buffer_get_cstr(attribute->value),
                              (const char *)attribute->name->u.token->xmlValue,
                              WBXML_STRLEN(attribute->name->u.token->xmlValue)) == 0)
            {
                /* Check if you have still a part in the Attribute Value to encode */
                if (wbxml_buffer_len(attribute->value) > WBXML_STRLEN(attribute->name->u.token->xmlValue))
                {
                    /* There is still a part in the Value to encode */
                    *value = wbxml_buffer_get_cstr(attribute->value) + WBXML_STRLEN(attribute->name->u.token->xmlValue);
                }
                else
                    *value = NULL;
            }
            else {
                /** @todo Should we stop everything and generate an error ? */
                WBXML_WARNING((WBXML_ENCODER, "wbxml_encode_attr_start() => Attribute Value doesn't match Attribute Token"));

                /* Current Attribute */
                encoder->current_attr = NULL;

                /* Encode Attribute Literal */
                return wbxml_encode_attr_start_literal(encoder, wbxml_attribute_get_xml_name(attribute));
            }
        }

        /* Encode Attribute Token */
        return wbxml_encode_attr_token(encoder, token, page);
    }
    else {
        /* Search in Attribute table */
        if ((attr = wbxml_tables_get_attr_from_xml(encoder->lang,
                                                   (WB_UTINY *)attribute->name->u.token->xmlName,
                                                   wbxml_buffer_get_cstr(attribute->value),
                                                   &value_left)) != NULL)
        {
            token = attr->wbxmlToken;
            page = attr->wbxmlCodePage;

            /* Current Attribute */
            encoder->current_attr = attr;

            /* If there is still a part in Attribute Value to encode */
            *value = value_left;

            /* Encode Attribute Token */
            return wbxml_encode_attr_token(encoder, token, page);
        }
        else {
            /* Current Attribute */
            encoder->current_attr = NULL;

            /* Encode Attribute Literal */
            return wbxml_encode_attr_start_literal(encoder, wbxml_attribute_get_xml_name(attribute));
        }
    }
}


/**
 * @brief Encode a WBXML Attribute Value
 * @param encoder The WBXML Encoder
 * @param buffer The Value Element Buffer to encode
 * @param ctx Value Element Context (Attribute Value or Content)
 * @return WBXML_OK if encoding is OK, an error code otherwise
 * @note    attrStart = *attrValue
 *          attrValue = string | extension | entity | opaque
 *
 *   AND:   element = *content
 *          content = string | extension | entity | opaque
 */
static WBXMLError wbxml_encode_value_element_buffer(WBXMLEncoder *encoder, WB_UTINY *buffer, WBXMLValueElementCtx ctx)
{
    WBXMLList *lresult = NULL;
    WBXMLBuffer *buff = NULL;
    WBXMLValueElement *elt = NULL, *new_elt = NULL;
    WB_ULONG i = 0, j = 0, index = 0;
    WB_UTINY *the_buffer = buffer;
    WBXMLError ret = WBXML_OK;

#if defined( WBXML_ENCODER_USE_STRTBL )
    WBXMLStringTableElement *strtbl_elt = NULL;
#endif /* WBXML_ENCODER_USE_STRTBL */

    if ((buffer == NULL) || (*buffer == '\0'))
        return WBXML_OK;


    /*********************************************************
     *  Encoder Language Specific Attribute Values
     */

    if (ctx == WBXML_VALUE_ELEMENT_CTX_ATTR) {	
        switch (encoder->lang->langID) {
        default:
            break;
        }
    }


    /*********************************************************
     *  Encoder Language Specific Content Text Values
     */

    /* If this is a Text Content (not in a CDATA section) */
    if ((ctx == WBXML_VALUE_ELEMENT_CTX_CONTENT) && (!encoder->in_cdata))
    {
#if defined( WBXML_SUPPORT_DRMREL )
        /* If this is a DRMREL 1.0 document */
        if (encoder->lang->langID == WBXML_LANG_DRMREL10)
        {
            /* Here we try to encode Specific DRMREL Content. If this buffer is not a DRMREL Data Type buffer
             * this function returns WBXML_NOT_ENCODED.
             * If so, the buffer will be encoded as String latter.
             */
            if ((ret = wbxml_encode_drmrel_content(encoder, buffer)) != WBXML_NOT_ENCODED)
                return ret;
        }
#endif /* WBXML_SUPPORT_DRMREL */

    }


    /*********************************************************
     * @todo Search first for simple cases !
     */


    /*********************************************************
     *  We search the list of Value Elements that represents
     *  this Value buffer
     */

    /* Create Result List */
    if ((lresult = wbxml_list_create()) == NULL)
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;

    /* Create primary Buffer */
    if ((buff = wbxml_buffer_create_from_cstr((const WB_UTINY *)the_buffer)) == NULL) {
        wbxml_list_destroy(lresult, NULL);
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;
    }

    /* Create Value Element for this buffer */
    if ((elt = wbxml_value_element_create()) == NULL) {
        wbxml_buffer_destroy(buff);
        wbxml_list_destroy(lresult, NULL);
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;
    }

    elt->type = WBXML_VALUE_ELEMENT_STRING;
    elt->u.str = buff;

    /* Append this Buffer to Result List */
    if (!wbxml_list_append(lresult, elt)) {
        wbxml_value_element_destroy(elt);
        wbxml_list_destroy(lresult, NULL);
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;
    }

    /* If this is an Attribute Value */
    if (ctx == WBXML_VALUE_ELEMENT_CTX_ATTR)
    {
        /*********************************************************
         *  Search for Attribute Value Tokens
         */

        if (encoder->lang->attrValueTable != NULL) {
            /* For each Attribute Value Token */
            j = 0;
            while (encoder->lang->attrValueTable[j].xmlName != NULL)
            {
                /* For each Value Element */
                for (i = 0; i < wbxml_list_len(lresult); i++) {
                    if ((elt = (WBXMLValueElement *) wbxml_list_get(lresult, i)) == NULL)
                        continue;

                    if (elt->type != WBXML_VALUE_ELEMENT_STRING)
                        continue;

                    /* Is this Attribute Value contained in this Buffer ? */
                    if (wbxml_buffer_search_cstr(elt->u.str, (WB_UTINY *)encoder->lang->attrValueTable[j].xmlName, 0, &index)) {
                        /* Create new Value Element */
                        if ((new_elt = wbxml_value_element_create()) == NULL) {
                            wbxml_list_destroy(lresult, wbxml_value_element_destroy_item);
                            return WBXML_ERROR_NOT_ENOUGH_MEMORY;
                        }

                        new_elt->type = WBXML_VALUE_ELEMENT_ATTR_TOKEN;
                        new_elt->u.attr = &(encoder->lang->attrValueTable[j]);

                        /* Insert new Value Element in List */
                        if (!wbxml_list_insert(lresult, (void *) new_elt, i + 1)) {
                            wbxml_value_element_destroy(new_elt);
                            wbxml_list_destroy(lresult, wbxml_value_element_destroy_item);
                            return WBXML_ERROR_NOT_ENOUGH_MEMORY;
                        }

                        /* Check if there is still the end of the String to encode */
                        if (index + WBXML_STRLEN(encoder->lang->attrValueTable[j].xmlName) < wbxml_buffer_len(elt->u.str)) {
                            /* Create new Value Element */
                            if ((new_elt = wbxml_value_element_create()) == NULL) {
                                wbxml_list_destroy(lresult, wbxml_value_element_destroy_item);
                                return WBXML_ERROR_NOT_ENOUGH_MEMORY;
                            }

                            new_elt->type = WBXML_VALUE_ELEMENT_STRING;
                            new_elt->u.str = wbxml_buffer_create_from_cstr(wbxml_buffer_get_cstr(elt->u.str) + index + WBXML_STRLEN((const char *)encoder->lang->attrValueTable[j].xmlName));

                            /* Insert new Value Element in List */
                            if ((new_elt->u.str == NULL) ||
                                !wbxml_list_insert(lresult, (void *) new_elt, i + 2))
                            {
                                wbxml_value_element_destroy(new_elt);
                                wbxml_list_destroy(lresult, wbxml_value_element_destroy_item);
                                return WBXML_ERROR_NOT_ENOUGH_MEMORY;
                            }
                        }

                        /* Remove the Attribute Value found in Value Element */
                        wbxml_buffer_delete(elt->u.str, index, wbxml_buffer_len(elt->u.str) - index);
                    } /* if */
                } /* for */
                j++;
            } /* while */
        } /* if */
    }

    /* If this is a Text Content (not in a CDATA section) */
    if ((ctx == WBXML_VALUE_ELEMENT_CTX_CONTENT) && (!encoder->in_cdata))
    {
        /*********************************************************
         *  Search for Extension Tokens
         */

        /** @todo Finish Extension Tokens Search */

        if (encoder->lang->extValueTable != NULL) {
            /* For each Extension Token */
            j = 0;
            while (encoder->lang->extValueTable[j].xmlName != NULL)
            {
                /* For each Value Element */
                for (i = 0; i < wbxml_list_len(lresult); i++) {
                    if ((elt = (WBXMLValueElement *) wbxml_list_get(lresult, i)) == NULL)
                        continue;

                    if (elt->type != WBXML_VALUE_ELEMENT_STRING)
                        continue;

                    /* Ignores the "1 char Extension Tokens" */
                    if (WBXML_STRLEN(encoder->lang->extValueTable[j].xmlName) < 2)
                        continue;

                    /* Is this Extension Token contained in this Buffer ? */
                    if (wbxml_buffer_search_cstr(elt->u.str, (WB_UTINY *) encoder->lang->extValueTable[j].xmlName, 0, &index))
                    {
                        /* Create new Value Element */
                        if ((new_elt = wbxml_value_element_create()) == NULL) {
                            wbxml_list_destroy(lresult, wbxml_value_element_destroy_item);
                            return WBXML_ERROR_NOT_ENOUGH_MEMORY;
                        }

                        new_elt->type = WBXML_VALUE_ELEMENT_EXTENSION;
                        new_elt->u.ext = &(encoder->lang->extValueTable[j]);

                        /* Insert new Value Element in List */
                        if (!wbxml_list_insert(lresult, (void *) new_elt, i + 1)) {
                            wbxml_value_element_destroy(new_elt);
                            wbxml_list_destroy(lresult, wbxml_value_element_destroy_item);
                            return WBXML_ERROR_NOT_ENOUGH_MEMORY;
                        }

                        /* Check if there is still the end of the String to encode */
                        if (index + WBXML_STRLEN(encoder->lang->extValueTable[j].xmlName) < wbxml_buffer_len(elt->u.str)) {
                            /* Create new Value Element */
                            if ((new_elt = wbxml_value_element_create()) == NULL) {
                                wbxml_list_destroy(lresult, wbxml_value_element_destroy_item);
                                return WBXML_ERROR_NOT_ENOUGH_MEMORY;
                            }

                            new_elt->type = WBXML_VALUE_ELEMENT_STRING;
                            new_elt->u.str = wbxml_buffer_create_from_cstr(wbxml_buffer_get_cstr(elt->u.str) + index + WBXML_STRLEN(encoder->lang->extValueTable[j].xmlName));

                            /* Insert new Value Element in List */
                            if ((new_elt->u.str == NULL) ||
                                !wbxml_list_insert(lresult, (void *) new_elt, i + 2))
                            {
                                wbxml_value_element_destroy(new_elt);
                                wbxml_list_destroy(lresult, wbxml_value_element_destroy_item);
                                return WBXML_ERROR_NOT_ENOUGH_MEMORY;
                            }
                        }

                        /* Remove the Attribute Value found in Value Element */
                        wbxml_buffer_delete(elt->u.str, index, wbxml_buffer_len(elt->u.str) - index);
                    } /* if */
                } /* for */
                j++;
            } /* while */
        } /* if */
    }


#if defined( WBXML_ENCODER_USE_STRTBL )

    /***********************************************************************
     *  Search for String Table References
     *  (except if this is a Content string and we are in a CDATA section)
     */
#if defined( WBXML_DRMEXT )
	if (!encoder->use_strtbl_literal)
	{
#endif
    if (encoder->use_strtbl && !(encoder->in_cdata && (ctx == WBXML_VALUE_ELEMENT_CTX_CONTENT))) {
        /* For each String Table Element */
        for (j = 0; j < wbxml_list_len(encoder->strstbl); j++) {
            if ((strtbl_elt = (WBXMLStringTableElement *) wbxml_list_get(encoder->strstbl, j)) == NULL)
                continue;

            /* For each Value Element */
            for (i = 0; i < wbxml_list_len(lresult); i++) {
                if ((elt = (WBXMLValueElement *) wbxml_list_get(lresult, i)) == NULL)
                    continue;

                if (elt->type != WBXML_VALUE_ELEMENT_STRING)
                    continue;

                /* Is the String Table Element contained in this Buffer ? */
                if (wbxml_buffer_search(elt->u.str, strtbl_elt->string, 0, &index)) {
                    /* Create new Value Element */
                    if ((new_elt = wbxml_value_element_create()) == NULL) {
                        wbxml_list_destroy(lresult, wbxml_value_element_destroy_item);
                        return WBXML_ERROR_NOT_ENOUGH_MEMORY;
                    }

                    new_elt->type = WBXML_VALUE_ELEMENT_TABLEREF;
                    new_elt->u.index = strtbl_elt->offset;

                    /* Insert new Value Element in List */
                    if (!wbxml_list_insert(lresult, (void *) new_elt, i + 1)) {
                        wbxml_value_element_destroy(new_elt);
                        wbxml_list_destroy(lresult, wbxml_value_element_destroy_item);
                        return WBXML_ERROR_NOT_ENOUGH_MEMORY;
                    }

                    /* Check if there is still the end of the String to encode */
                    if (index + wbxml_buffer_len(strtbl_elt->string) < wbxml_buffer_len(elt->u.str)) {
                        /* Create new Value Element */
                        if ((new_elt = wbxml_value_element_create()) == NULL) {
                            wbxml_list_destroy(lresult, wbxml_value_element_destroy_item);
                            return WBXML_ERROR_NOT_ENOUGH_MEMORY;
                        }

                        new_elt->type = WBXML_VALUE_ELEMENT_STRING;
                        new_elt->u.str = wbxml_buffer_create_from_cstr(wbxml_buffer_get_cstr(elt->u.str) + index + wbxml_buffer_len(strtbl_elt->string));

                        /* Insert new Value Element in List */
                        if ((new_elt->u.str == NULL) ||
                            !wbxml_list_insert(lresult, (void *) new_elt, i + 2))
                        {
                            wbxml_value_element_destroy(new_elt);
                            wbxml_list_destroy(lresult, wbxml_value_element_destroy_item);
                            return WBXML_ERROR_NOT_ENOUGH_MEMORY;
                        }
                    }

                    /* Remove the Attribute Value found in Value Element */
                    wbxml_buffer_delete(elt->u.str, index, wbxml_buffer_len(elt->u.str) - index);
                } /* if */
            } /* for */
        } /* for */
    } /* if */
#if defined( WBXML_DRMEXT )
	}
#endif
#endif /* WBXML_ENCODER_USE_STRTBL */

    /*********************************************************
     *  Encode Value Element Buffer
     */

#if defined( WBXML_DRMEXT )
	if (encoder->record_attrvalue_literals && ctx == WBXML_VALUE_ELEMENT_CTX_ATTR)
	{
		WB_ULONG nIndx;
	    for (nIndx = 0; nIndx < wbxml_list_len(lresult); nIndx++) {
	        if ((elt = (WBXMLValueElement *) wbxml_list_get(lresult, nIndx)) == NULL)
	            continue;

			if (elt->type != WBXML_VALUE_ELEMENT_STRING)
				continue;
			if (wbxml_buffer_len(elt->u.str) <= 0)
				continue;
			if ((ret =encoder_add_attr_value(encoder, wbxml_buffer_get_cstr(elt->u.str))) != WBXML_OK) {
				/* Clean-up */
    			wbxml_list_destroy(lresult, wbxml_value_element_destroy_item);
				return ret;
			}
	    }	
	}
#endif

    ret = wbxml_encode_value_element_list(encoder, lresult);

    /* Clean-up */
    wbxml_list_destroy(lresult, wbxml_value_element_destroy_item);

    return ret;
}


/**
 * @brief Encode a WBXML Value Element List
 * @param encoder The WBXML Encoder
 * @param list The Value Element list
 * @return WBXML_OK if encoding is OK, an error code otherwise
 */
static WBXMLError wbxml_encode_value_element_list(WBXMLEncoder *encoder, WBXMLList *list)
{
    WBXMLValueElement *elt = NULL;
    WB_ULONG i = 0;
    WBXMLError ret = WBXML_OK;

    if (encoder == NULL)
        return WBXML_ERROR_INTERNAL;

    if (list == NULL)
        return WBXML_OK;

    for (i = 0; i < wbxml_list_len(list); i++) {
        if ((elt = (WBXMLValueElement *) wbxml_list_get(list, i)) == NULL)
            continue;

        switch (elt->type) {
        case WBXML_VALUE_ELEMENT_STRING:
            /* Inline String */
            if (wbxml_buffer_len(elt->u.str) > 0) {
                if ((ret = wbxml_encode_inline_string(encoder, elt->u.str)) != WBXML_OK)
                    return ret;
            }
            break;

#if defined( WBXML_ENCODER_USE_STRTBL )
        case WBXML_VALUE_ELEMENT_TABLEREF:
            /* String Table Reference */
            if ((ret = wbxml_encode_tableref(encoder, elt->u.index)) != WBXML_OK)
                return ret;
            break;
#endif /* WBXML_ENCODER_USE_STRTBL */

        case WBXML_VALUE_ELEMENT_EXTENSION:
            /* Encode Extension Token */
            if ((ret = wbxml_encode_inline_integer_extension_token(encoder, WBXML_EXT_T_0, elt->u.ext->wbxmlToken)) != WBXML_OK)
                return ret;
            break;

        case WBXML_VALUE_ELEMENT_OPAQUE:
            /** @todo Opaque */
            break;

        case WBXML_VALUE_ELEMENT_ATTR_TOKEN:
            /* Attribute Value Token */
            if ((ret = wbxml_encode_attr_token(encoder, elt->u.attr->wbxmlToken,  elt->u.attr->wbxmlCodePage)) != WBXML_OK)
                return ret;
            break;

        default:
            return WBXML_ERROR_INTERNAL;
        }
    }

    return WBXML_OK;
}


/**
 * @brief Encode a WBXML Literal Attribute Start
 * @param encoder The WBXML Encoder
 * @param attr The literal attr name to encode
 * @return WBXML_OK if encoding is OK, an error code otherwise
 * @note    attrStart = (LITERAL index)
 *          index = mb_u_int32
 */
static WBXMLError wbxml_encode_attr_start_literal(WBXMLEncoder *encoder, const WB_UTINY *attr)
{
#if defined( WBXML_ENCODER_USE_STRTBL )
    WBXMLStringTableElement *elt = NULL;
    WBXMLBuffer *buff = NULL;
    WB_ULONG index = 0;
    WB_BOOL added = FALSE;

    /* If String Table generation is disabled, we can't generate this Literal */
    if (!encoder->use_strtbl)
        return WBXML_ERROR_STRTBL_DISABLED;

    /* Add tag in String Table */
    if (((buff = wbxml_buffer_create(attr, WBXML_STRLEN((const char *)attr), WBXML_STRLEN((const char *)attr))) == NULL) ||
        ((elt = wbxml_strtbl_element_create(buff, FALSE)) == NULL) ||
        (!wbxml_strtbl_add_element(encoder, elt, &index, &added)))
    {
        wbxml_strtbl_element_destroy(elt);
        wbxml_buffer_destroy(buff);
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;
    }

    /* If already exists in String Table: clean-up */
    if (!added)
        wbxml_strtbl_element_destroy(elt);
#if defined( WBXML_DRMEXT )
	else if (encoder->record_literals) {
		WBXMLNode *pstNew = wbxml_malloc(sizeof(WBXMLNode));
		if (!pstNew) {
			 wbxml_strtbl_element_destroy(elt);
			 return WBXML_ERROR_NOT_ENOUGH_MEMORY;
		}
		pstNew->pszName = (unsigned char *)wbxml_strdup((const char*)attr);
		if (!pstNew->pszName) {
			wbxml_free(pstNew);
			 wbxml_strtbl_element_destroy(elt);
			 return WBXML_ERROR_NOT_ENOUGH_MEMORY;
		}
		
		/* Add to head */
		pstNew->next = encoder->pstAtt;
		encoder->pstAtt = pstNew;
		encoder->Atts++;
	}
#endif

    /* Encode LITERAL index */
    if ((!wbxml_buffer_append_char(encoder->output, WBXML_LITERAL)) ||
        (!wbxml_buffer_append_mb_uint_32(encoder->output, index)))
    {
        return WBXML_ERROR_ENCODER_APPEND_DATA;
    }

    return WBXML_OK;
#else
    /* No String Table Support */
    return WBXML_ERROR_STRTBL_DISABLED;
#endif /* WBXML_ENCODER_USE_STRTBL */
}


/**
 * @brief Encode a WBXML Attribute Token
 * @param encoder The WBXML Encoder
 * @param token The WBXML Attribute Token to encode
 * @param page The WBXML CodePage for this Token
 * @return WBXML_OK if encoding is OK, an error code otherwise
 * @note    attrStart = ([switchPage] ATTRSTART)
 *          switchPage = SWITCH_PAGE pageindex
 *          pageindex = u_int8
 *
 *  And:    attrValue = ([switchPage] ATTRVALUE)
 */
static WBXMLError wbxml_encode_attr_token(WBXMLEncoder *encoder, WB_UTINY token, WB_UTINY page)
{
    /* Switch Page if needed */
    if (encoder->attrCodePage != page)
    {
        if ((!wbxml_buffer_append_char(encoder->output, WBXML_SWITCH_PAGE)) ||
            (!wbxml_buffer_append_char(encoder->output, page)))
        {
            return WBXML_ERROR_ENCODER_APPEND_DATA;
        }

        encoder->attrCodePage = page;
    }

    /* Add Token */
    if (!wbxml_buffer_append_char(encoder->output, token))
        return WBXML_ERROR_ENCODER_APPEND_DATA;

    return WBXML_OK;
}


/**
 * @brief Encode a WBXML Inline String
 * @param encoder The WBXML Encoder
 * @param str The String to encode
 * @return WBXML_OK if encoding is OK, an error code otherwise
 */
static WBXMLError wbxml_encode_inline_string(WBXMLEncoder *encoder, WBXMLBuffer *str)
{
    /* Add STR_I */
    if (!wbxml_buffer_append_char(encoder->output, WBXML_STR_I))
        return WBXML_ERROR_ENCODER_APPEND_DATA;

    /* Add String */
    if (!wbxml_buffer_append(encoder->output, str))
        return WBXML_ERROR_ENCODER_APPEND_DATA;

    /* Add Null Termination */
    if (!wbxml_buffer_append_char(encoder->output, WBXML_STR_END))
        return WBXML_ERROR_ENCODER_APPEND_DATA;

    return WBXML_OK;
}


/**
 * @brief Encode a WBXML Inline Integer Extension Token
 * @param encoder The WBXML Encoder
 * @param ext Extension Type (WBXML_EXT_T_0, WBXML_EXT_T_1 or WBXML_EXT_T_2)
 * @param value The Extension Token Value
 * @return WBXML_OK if encoding is OK, an error code otherwise
 * @note  (WBXML 1.3 - 5.8.4.2) Inline integrer extension token = EXT_T* mb_u_int32
 */
static WBXMLError wbxml_encode_inline_integer_extension_token(WBXMLEncoder *encoder, WB_UTINY ext, WB_UTINY value)
{
    /* Add EXT_T* */
    if (!wbxml_buffer_append_char(encoder->output, ext))
        return WBXML_ERROR_ENCODER_APPEND_DATA;

    /* Add Value */
    if (!wbxml_buffer_append_mb_uint_32(encoder->output, (WB_ULONG) value))
        return WBXML_ERROR_ENCODER_APPEND_DATA;

    return WBXML_OK;
}


#if 0
/**
 * @brief Encode a WBXML Entity
 * @param encoder The WBXML Encoder
 * @param value The Entity to encode
 * @return WBXML_OK if encoding is OK, an error code otherwise
 * @note  (WBXML 1.3 - 5.8.4.3) entity = ENTITY entcode
 *                              entcode = mb_u_int32
 */
static WBXMLError wbxml_encode_entity(WBXMLEncoder *encoder, WB_ULONG value)
{
    /* Add ENTITY */
    if (!wbxml_buffer_append_char(encoder->output, WBXML_ENTITY))
        return WBXML_ERROR_ENCODER_APPEND_DATA;

    /* Add entcode */
    if (!wbxml_buffer_append_mb_uint_32(encoder->output, value))
        return WBXML_ERROR_ENCODER_APPEND_DATA;

    return WBXML_OK;
}
#endif /* 0 */


/**
 * @brief Encode a WBXML Opaque
 * @param encoder The WBXML Encoder
 * @param buff The Buffer to encode
 * @return WBXML_OK if encoding is OK, an error code otherwise
 * @note  opaque = OPAQUE length *byte
 *        length = mb_u_int32
 */
static WBXMLError wbxml_encode_opaque(WBXMLEncoder *encoder, WBXMLBuffer *buff)
{
    /* Add WBXML_OPAQUE */
    if (!wbxml_buffer_append_char(encoder->output, WBXML_OPAQUE))
        return WBXML_ERROR_ENCODER_APPEND_DATA;

    /* Add Length */
    if (!wbxml_buffer_append_mb_uint_32(encoder->output, wbxml_buffer_len(buff)))
        return WBXML_ERROR_ENCODER_APPEND_DATA;

    /* Add Buffer */
    if (!wbxml_buffer_append(encoder->output, buff))
        return WBXML_ERROR_ENCODER_APPEND_DATA;

    return WBXML_OK;
}


#if defined( WBXML_ENCODER_USE_STRTBL )
/**
 * @brief Encode a WBXML String Table Reference
 * @param encoder The WBXML Encoder
 * @param offset The String Table offset
 * @return WBXML_OK if encoding is OK, an error code otherwise
 * @note tableref = STR_T index
 */
static WBXMLError wbxml_encode_tableref(WBXMLEncoder *encoder, WB_ULONG offset)
{
    /* Add WBXML_STR_T */
    if (!wbxml_buffer_append_char(encoder->output, WBXML_STR_T))
        return WBXML_ERROR_ENCODER_APPEND_DATA;

    /* Add String */
    if (!wbxml_buffer_append_mb_uint_32(encoder->output, offset))
        return WBXML_ERROR_ENCODER_APPEND_DATA;

    return WBXML_OK;
}
#endif /* WBXML_ENCODER_USE_STRTBL */


/**
 * @brief Create a WBXMLValueElement structure
 * @return The newly created WBXMLValueElement structure, or NULL if not enough memory
 */
static WBXMLValueElement *wbxml_value_element_create(void)
{
    WBXMLValueElement *elt = NULL;

    if ((elt = (WBXMLValueElement*) wbxml_malloc(sizeof(WBXMLValueElement))) == NULL)
        return NULL;

    elt->type = WBXML_VALUE_ELEMENT_STRING;
    elt->u.str = NULL;

    return elt;
}


/**
 * @brief Destroy a WBXMLValueElement structure
 * @param elt The WBXMLValueElement structure to destroy
 */
static void wbxml_value_element_destroy(WBXMLValueElement *elt)
{
    if (elt == NULL)
        return;

    switch (elt->type) {
    case WBXML_VALUE_ELEMENT_STRING:
        wbxml_buffer_destroy(elt->u.str);
        break;
    case WBXML_VALUE_ELEMENT_OPAQUE:
        wbxml_buffer_destroy(elt->u.buff);
        break;
    default:
        /* Nothing to destroy */
        break;
    }

    wbxml_free((void*) elt);
}


/**
 * @brief Destroy a WBXMLValueElement structure (for wbxml_list_destroy() function)
 * @param elt The WBXMLValueElement structure to destroy
 */
static void wbxml_value_element_destroy_item(void *elt)
{
    wbxml_value_element_destroy((WBXMLValueElement *) elt);
}


/**
 * @brief Encode an encapsulated WBXML Tree to WBXML
 * @param encoder [in] The WBXML Encoder
 * @param tree    [in] The WBXML Tree to encode
 * @return WBXML_OK if encoding is OK, an error code otherwise
 */
static WBXMLError wbxml_encode_tree(WBXMLEncoder *encoder, WBXMLTree *tree)
{
    WBXMLEncoder *new_encoder = NULL;
    WB_UTINY *wbxml = NULL;
    WB_ULONG wbxml_len = 0;
    WBXMLError ret = WBXML_OK;

    if ((new_encoder = encoder_duplicate(encoder)) == NULL)
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;

    /* Set Tree */
    new_encoder->tree = tree;

    /* Encode to WBXML */
    if ((ret = wbxml_encoder_encode_tree_to_wbxml(new_encoder, &wbxml, &wbxml_len)) != WBXML_OK) {
        wbxml_encoder_destroy(new_encoder);
        return ret;
    }

    /* Clean-up */
    wbxml_encoder_destroy(new_encoder);

    /* Add WBXML_OPAQUE */
    if (!wbxml_buffer_append_char(encoder->output, WBXML_OPAQUE)) {
        wbxml_free(wbxml);
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;
    }

    /* Add Length */
    if (!wbxml_buffer_append_mb_uint_32(encoder->output, wbxml_len)) {
        wbxml_free(wbxml);
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;
    }

    /* Append wbxml to output */
    if (!wbxml_buffer_append_data(encoder->output, wbxml, wbxml_len)) {
        wbxml_free(wbxml);
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;
    }

    /* Clean-up */
    wbxml_free(wbxml);

    return WBXML_OK;
}


/****************************************
 * Language Specific Encoding Functions
 */


#if defined( WBXML_SUPPORT_DRMREL )

/*******************
 * DRMREL 1.0
 */

/**
 * @brief Encode a DRMREL Content buffer
 * @param encoder The WBXML Encoder
 * @param buffer The buffer to encode
 * @return WBXML_OK if encoded, WBXML_NOT_ENCODED if not encoded, another error code otherwise
 * @note This function encodes a Specific DRMREL content.
 *       If not found, this is not encoded... and it will be encoded latter as an Inline String
 *       in wbxml_encode_value_element_buffer().
 */
static WBXMLError wbxml_encode_drmrel_content(WBXMLEncoder *encoder, WB_UTINY *buffer)
{
    WB_UTINY *data = NULL;
    WB_LONG data_len = 0;
    const WBXMLTagEntry *current_tag = NULL;

    if ((encoder->current_text_parent != NULL) && 
        (encoder->current_text_parent->name != NULL) &&
        (encoder->current_text_parent->name->type == WBXML_VALUE_TOKEN))
    {
        current_tag = encoder->current_text_parent->name->u.token;
    }

    if (current_tag != NULL) 
    {
        if ((current_tag->wbxmlCodePage == 0x00) &&
            (current_tag->wbxmlToken == 0x0C))
        {
            /* <ds:KeyValue> content: "Encoded in binary format, i.e., no base64 encoding" */

            /* Decode Base64 */
            if ((data_len = wbxml_base64_decode(buffer, &data)) < 0)
                return WBXML_NOT_ENCODED;

            /* Add WBXML_OPAQUE */
            if (!wbxml_buffer_append_char(encoder->output, WBXML_OPAQUE))
                return WBXML_ERROR_ENCODER_APPEND_DATA;

            /* Add Data Length */
            if (!wbxml_buffer_append_mb_uint_32(encoder->output, (WB_ULONG) data_len))
                return WBXML_ERROR_ENCODER_APPEND_DATA;

            /* Add Data */
            if (!wbxml_buffer_append_data(encoder->output, data, data_len))
                return WBXML_ERROR_ENCODER_APPEND_DATA;

            /* Free Data */
            wbxml_free(data);

            return WBXML_OK;
        }
    }

    return WBXML_NOT_ENCODED;
}

#endif /* WBXML_SUPPORT_DRMREL */


#if defined( WBXML_ENCODER_USE_STRTBL )

/****************************
 * String Table Functions
 */

/**
 * @brief Create a String Table element
 * @param string The WBXMLBuffer containing the String
 * @param is_stat If set to TRUE, this Buffer won't be destroyed in wbxml_strtbl_element_destroy() function
 * @return The newly created String Table Element, or NULL if not enought memory
 */
static WBXMLStringTableElement *wbxml_strtbl_element_create(WBXMLBuffer *string, WB_BOOL is_stat)
{
    WBXMLStringTableElement *elt = NULL;

    if ((elt = (WBXMLStringTableElement *) wbxml_malloc(sizeof(WBXMLStringTableElement))) == NULL)
        return NULL;

    elt->string = string;
    elt->offset = 0;
    elt->count = 0;
    elt->stat = is_stat;

    return elt;
}


/**
 * @brief Destroy a String Table element
 * @param element The element to destroy
 */
static void wbxml_strtbl_element_destroy(WBXMLStringTableElement *element)
{
    if (element == NULL)
        return;

    if (!element->stat)
        wbxml_buffer_destroy(element->string);

    wbxml_free(element);
}


/**
 * @brief Destroy a String Table element (for wbxml_list_destroy())
 * @param element The element to destroy
 */
static void wbxml_strtbl_element_destroy_item(void *element)
{
    wbxml_strtbl_element_destroy((WBXMLStringTableElement *) element);
}


/**
 * @brief Initialize the String Table
 * @param encoder The WBXML Encoder
 * @param root The root element of LibXML Tree
 */
static WBXMLError wbxml_strtbl_initialize(WBXMLEncoder *encoder, WBXMLTreeNode *root)
{
    WBXMLList *strings = NULL, *one_ref = NULL;
    WBXMLError ret;

    if ((strings = wbxml_list_create()) == NULL)
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;

    /* Collect all Strings:
     * [out] 'strings' is the list of pointers to WBXMLBuffer. This Buffers must not be freed.
     */
    wbxml_strtbl_collect_strings(encoder, root, strings);

    /* Insert, in String Table, Strings that are referenced more than one time
     * [out] 'strings' is NULL
     *       'one_ref' is the list of strings referenced only ONE time (list of WBXMLStringTableElement*)
     *       Strings referenced more than one time are added to String Table
     */
    if ((ret = wbxml_strtbl_check_references(encoder, &strings, &one_ref, TRUE)) != WBXML_OK) {
        wbxml_list_destroy(strings, NULL);
        return ret;
    }

    /* 'strings' is destroyed after call of wbxml_strtbl_check_references() */

    /* Split Strings refered only one time in Words
     * [out] 'strings' is the list of words. This words (WBXMLBuffer) must be freed
     */
    if ((ret = wbxml_strtbl_collect_words(one_ref, &strings)) != WBXML_OK) {
        wbxml_list_destroy(one_ref, wbxml_strtbl_element_destroy_item);
        return ret;
    }

    /* Destroy References List */
    wbxml_list_destroy(one_ref, wbxml_strtbl_element_destroy_item);
    one_ref = NULL;

    /* Keep Strings referenced more than one time */
    if (strings != NULL)
        wbxml_strtbl_check_references(encoder, &strings, &one_ref, FALSE);

    /* 'strings' is destroyed after call of wbxml_strtbl_check_references() */

    /* Cleanup */
    wbxml_list_destroy(one_ref, wbxml_strtbl_element_destroy_item);

    return WBXML_OK;
}


/**
 * @brief Collect Strings in XML Document (in Text Content and Attribute Values)
 * @param encoder [in] The WBXML Encoder
 * @param node [in] The current element node of LibXML Tree
 * @param strings [out] List of WBXMLBuffer buffers corresponding to Collected Strings
 */
static void wbxml_strtbl_collect_strings(WBXMLEncoder *encoder, WBXMLTreeNode *node, WBXMLList *strings)
{
    const WBXMLAttrEntry *attr_entry = NULL;
    WBXMLAttribute *attr = NULL;
    WB_ULONG i = 0;
    WB_UTINY *value_left = NULL;

    switch (node->type)
    {
        case WBXML_TREE_TEXT_NODE:
            /* Ignore blank nodes */
            if (wbxml_buffer_contains_only_whitespaces(node->content))
                break;

            /** @todo Shrink / Strip Blanks */

            /* Only add this string if it is big enought */
            if (wbxml_buffer_len(node->content) > WBXML_ENCODER_STRING_TABLE_MIN) {
                wbxml_list_append(strings, node->content);
                WBXML_DEBUG((WBXML_ENCODER, "Strtbl - Collecting String: %s", wbxml_buffer_get_cstr(node->content)));
            }
            break;

        case WBXML_TREE_ELEMENT_NODE:
            /* Collect strings in Attributes Values too */
            if (node->attrs != NULL) {
                for (i = 0; i < wbxml_list_len(node->attrs); i++) {
                    /* Get attribute */
                    /* wbt : attr could not be set as NULL, through pre-checking "node->attrs != NULL" and "i < wbxml_list_len(node->attrs)" */
                    attr = wbxml_list_get(node->attrs, i);

                    /* Only add this string if it is big enought */
                    if (wbxml_buffer_len(attr->value) > WBXML_ENCODER_STRING_TABLE_MIN) {
                        /* This mustn't be a tokenisable Attribute Start */
                        attr_entry = wbxml_tables_get_attr_from_xml(encoder->lang,
                                                                   (WB_UTINY *) wbxml_attribute_get_xml_name(attr),
                                                                   (WB_UTINY *) wbxml_attribute_get_xml_value(attr),
                                                                   &value_left);

                        /* - If attr_entry is NULL: no Attribute Start found
                         * - If attr_entry is not NULL: and Attribute Start is found, but it can be the one with
                         *   no Attribute Value associated. So just check that the 'value_left' is the same than
                         *   the attribute value we where searching for
                         */
                        if ((attr_entry == NULL) || ((attr_entry != NULL) && (value_left == (WB_UTINY *) wbxml_attribute_get_xml_value(attr))))
                        {
                            /* It mustn't contain a tokenisable Attribute Value */
                            if (!wbxml_tables_contains_attr_value_from_xml(encoder->lang,
                                                                           (WB_UTINY *) wbxml_attribute_get_xml_value(attr)))
                            {
                                wbxml_list_append(strings, attr->value);
                                WBXML_DEBUG((WBXML_ENCODER, "Strtbl - Collecting String: %s", wbxml_buffer_get_cstr(attr->value)));
                            }
                        }
                    }
                }
            }
            break;

        default:
            /* NOOP */
            break;
    }

    if (node->children != NULL)
        wbxml_strtbl_collect_strings(encoder, node->children, strings);

    if (node->next != NULL)
        wbxml_strtbl_collect_strings(encoder, node->next, strings);
}


/**
 * @brief Split Strings into Words
 * @param elements [in] List of String Table Elements to split
 * @param result [out] Resulting list of Words
 * @return WBXML_OK is no error, another error code otherwise
 */
static WBXMLError wbxml_strtbl_collect_words(WBXMLList *elements, WBXMLList **result)
{
    WBXMLStringTableElement *elt = NULL;
    WBXMLList *list = NULL, *temp_list = NULL;
    WBXMLBuffer *word = NULL;
    WB_ULONG i = 0;

    *result = NULL;

    for (i = 0; i < wbxml_list_len(elements); i++)
    {
        /* wbt : elt could not set as NULL though pre-checking "i < wbxml_list_len(elements)" */
        elt = (WBXMLStringTableElement *) wbxml_list_get(elements, i);

        if (list == NULL) {
            if ((list = wbxml_buffer_split_words(elt->string)) == NULL) {
                wbxml_list_destroy(list, wbxml_buffer_destroy_item);
                return WBXML_ERROR_NOT_ENOUGH_MEMORY;
            }
        } else {
            if ((temp_list = wbxml_buffer_split_words(elt->string)) == NULL) {
                wbxml_list_destroy(list, wbxml_buffer_destroy_item);
                return WBXML_ERROR_NOT_ENOUGH_MEMORY;
            }

            while ((word = wbxml_list_extract_first(temp_list)) != NULL) {
                if (!wbxml_list_append(list, word)) {
                    wbxml_buffer_destroy(word);
                    wbxml_list_destroy(temp_list, wbxml_buffer_destroy_item);
                    wbxml_list_destroy(list, wbxml_buffer_destroy_item);
                    return WBXML_ERROR_NOT_ENOUGH_MEMORY;
                }
            }

            wbxml_list_destroy(temp_list, NULL);
        }
    }

    *result = list;

    return WBXML_OK;
}


/**
 * @brief Append the String Table to result Buffer
 * @param buff The Buffer to append the String Table to
 * @param strstbl The String Table to append
 */
static WBXMLError wbxml_strtbl_construct(WBXMLBuffer *buff, WBXMLList *strstbl)
{
    WBXMLStringTableElement *elt = NULL;
    WB_ULONG i = 0;

    if ((buff == NULL) || (strstbl == NULL))
        return WBXML_ERROR_BAD_PARAMETER;

    for (i = 0; i < wbxml_list_len(strstbl); i++)
    {
        if ((elt = wbxml_list_get(strstbl, i)) == NULL)
            continue;
        
        if (!wbxml_buffer_append(buff, elt->string))
            return WBXML_ERROR_ENCODER_APPEND_DATA;
            
        if (!wbxml_buffer_append_char(buff, WBXML_STR_END))
            return WBXML_ERROR_ENCODER_APPEND_DATA;
    }
    
    return WBXML_OK;
}


/**
 * @brief Check strings that have multiple references, add them to string table
 *        and return strings that have only one reference
 * @param encoder The WBXML Encoder
 * @param strings The List of Strings to check (List of WBXMLBuffer) : This list is freed by this function
 * @param one_ref List of strings that have only one reference (List of WBXMLStringTableElement)
 * @param stat_buff If set to TRUE, Buffers referenced by 'strings' must NOT be destroyed.
 * @return WBXML_OK if no error, another error code otherwise
 * @warning All elements of 'strings' list are removed from this list
 */
static WBXMLError wbxml_strtbl_check_references(WBXMLEncoder *encoder, WBXMLList **strings, WBXMLList **one_ref, WB_BOOL stat_buff)
{
    WBXMLList *referenced = NULL, *result = NULL;
    WBXMLBuffer *string = NULL;
    WBXMLStringTableElement *ref = NULL;
    WB_ULONG j = 0;
    WB_BOOL added = FALSE;

    if ((strings == NULL) || (one_ref == NULL))
        return WBXML_ERROR_INTERNAL;

    *one_ref = NULL;

    /* Create list of String References */
    if ((referenced = wbxml_list_create()) == NULL)
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;


    /*********************
     * Count References
     */

    while (wbxml_list_len(*strings) > 0)
    {
        string = (WBXMLBuffer *) wbxml_list_extract_first(*strings);

        /* Check if we have already found this String */
        for (j = 0; j < wbxml_list_len(referenced); j++)
        {
            /* wbt : ref could not be set as NULL, through pre-checking "if ((referenced = wbxml_list_create()) == NULL)" and "j < wbxml_list_len(referenced)" */
            ref = (WBXMLStringTableElement *) wbxml_list_get(referenced, j);

            if (wbxml_buffer_compare(ref->string, string) == 0)
            {
                if (!stat_buff)
                    wbxml_buffer_destroy(string);

                string = NULL;
                ref->count++;
                break;
            }
        }

        if (string != NULL)
        {
            /* New Reference Element */
            if ((ref = wbxml_strtbl_element_create(string, stat_buff)) == NULL)
            {
                wbxml_list_destroy(referenced, wbxml_strtbl_element_destroy_item);

                if (!stat_buff)
                    wbxml_list_destroy(*strings, wbxml_buffer_destroy_item);
                else
                    wbxml_list_destroy(*strings, NULL);

                *strings = NULL;
                return WBXML_ERROR_NOT_ENOUGH_MEMORY;
            }

            ref->count++;

            if (!wbxml_list_append(referenced, (void *) ref))
            {
                wbxml_list_destroy(referenced, wbxml_strtbl_element_destroy_item);

                if (!stat_buff)
                    wbxml_list_destroy(*strings, wbxml_buffer_destroy_item);
                else
                    wbxml_list_destroy(*strings, NULL);

                *strings = NULL;
                return WBXML_ERROR_NOT_ENOUGH_MEMORY;
            }
        }
    }


    wbxml_list_destroy(*strings, NULL);
    *strings = NULL;


    /***********************************************
     * Remove Strings that have only One reference
     */

    if ((result = wbxml_list_create()) == NULL) {
        wbxml_list_destroy(referenced, wbxml_strtbl_element_destroy_item);
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;
    }

    while (wbxml_list_len(referenced) > 0)
    {
        /* wbt : ref could not be set as NULL, through pre-checking "wbxml_list_len(referenced) > 0" */
        ref = (WBXMLStringTableElement *) wbxml_list_extract_first(referenced);
        if ((ref->count > 1) && (wbxml_buffer_len(ref->string) > WBXML_ENCODER_STRING_TABLE_MIN)) {
            /* Add Element to String Table */
            if (!wbxml_strtbl_add_element(encoder, ref, NULL, &added)) {
                wbxml_strtbl_element_destroy(ref);
                wbxml_list_destroy(referenced, wbxml_strtbl_element_destroy_item);
                wbxml_list_destroy(result, wbxml_strtbl_element_destroy_item);
                return WBXML_ERROR_NOT_ENOUGH_MEMORY;
            }

            if (!added) {
                wbxml_strtbl_element_destroy(ref);
            }
        }
        else {
            /* Add Element in resulting 'not added in String Table' list */
            if (!wbxml_list_append(result, (void *) ref)) {
                wbxml_strtbl_element_destroy(ref);
                wbxml_list_destroy(referenced, wbxml_strtbl_element_destroy_item);
                wbxml_list_destroy(result, wbxml_strtbl_element_destroy_item);
                return WBXML_ERROR_NOT_ENOUGH_MEMORY;
            }
        }
    }

    wbxml_list_destroy(referenced, wbxml_strtbl_element_destroy_item);

    *one_ref = result;

    return WBXML_OK;
}


/**
 * @brief Add a String in String table
 * @param encoder [in] The WBXML Encoder
 * @param elt [in] The Element to add
 * @param index [out] The index in String Table
 * @param added [out] TRUE if really added, or FALSe if already in String Table
 * @return TRUE if no error, FALSE is Memory Error
 */
static WB_BOOL wbxml_strtbl_add_element(WBXMLEncoder *encoder, WBXMLStringTableElement *elt, WB_ULONG *index, WB_BOOL *added)
{
    WBXMLStringTableElement *elt_tmp = NULL;
    WB_ULONG i = 0;

    if ((encoder == NULL) || (encoder->strstbl == NULL) || (elt == NULL) || (elt->string == NULL))
        return FALSE;

    *added = FALSE;

    /* Check if this element already exists in String Table */
    for (i = 0; i < wbxml_list_len(encoder->strstbl); i++)
    {
        if ((elt_tmp = wbxml_list_get(encoder->strstbl, i)) == NULL)
            continue;

        if ((wbxml_buffer_len(elt_tmp->string) == wbxml_buffer_len(elt->string)) &&
            (wbxml_buffer_compare(elt_tmp->string, elt->string) == 0))
        {
            /* The String already exists in the String Table */
            if (index != NULL)
                *index = elt_tmp->offset;
            return TRUE;
        }
    }

    /* Add this string to String Table */
    elt->offset = encoder->strstbl_len;

    if (!wbxml_list_append(encoder->strstbl, (void *) elt))
        return FALSE;

    /* Index in String Table */
    if (index != NULL)
        *index = encoder->strstbl_len;

    /* New String Table length */
    encoder->strstbl_len += wbxml_buffer_len(elt->string) + 1;

    *added = TRUE;

    return TRUE;
}

#endif /* WBXML_ENCODER_USE_STRTBL */


/*****************************************
 *  XML Output Functions
 */

/****************************
 * Build XML Result
 */

/**
 * @brief Build XML Result
 * @param encoder [in] The WBXML Encoder
 * @param xml     [out] Resulting XML document
 * @param xml_len [out] XML document length
 * @return WBXML_OK if built is OK, an error code otherwise
 */
static WBXMLError xml_build_result(WBXMLEncoder *encoder, WB_UTINY **xml, WB_ULONG *xml_len)
{
    WBXMLBuffer *header = NULL;
    WB_ULONG     len    = 0;
    WBXMLError   ret    = WBXML_OK;
    
    /* Init */
    //*xml_len = 0; WBT.20100128
    if (xml_len)
    {
    *xml_len = 0;
    }

    if (encoder->flow_mode == TRUE) {
        /* Header already built */
        header = encoder->output_header;
    }
    else {
        /* Create Header Buffer */
        if ((header = wbxml_buffer_create("", 0, WBXML_ENCODER_XML_HEADER_MALLOC_BLOCK)) == NULL)
            return WBXML_ERROR_NOT_ENOUGH_MEMORY;

        /* Fill Header Buffer */
        if (encoder->xml_encode_header) {
            if ((ret = xml_fill_header(encoder, header)) != WBXML_OK) {
                wbxml_buffer_destroy(header);
                return ret;
            }
        }
    }

    /* Result Buffer Length */
    len = wbxml_buffer_len(header) + wbxml_buffer_len(encoder->output);

    /* Create Result Buffer */
    *xml = (WB_UTINY *) wbxml_malloc((len + 1) * sizeof(WB_UTINY));
    if (*xml == NULL) {
        if (encoder->flow_mode == FALSE)
            wbxml_buffer_destroy(header);
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;
    }

    /** @todo Use the 'output_charset' field */

    /* Copy Header to Result */
    memcpy(*xml, wbxml_buffer_get_cstr(header), wbxml_buffer_len(header));

    /* Copy XML Document to Result */
    memcpy(*xml + wbxml_buffer_len(header), wbxml_buffer_get_cstr(encoder->output), wbxml_buffer_len(encoder->output));

    /** @todo Remove this NULL char if not needed by charset */

    /* NULL Terminated Buffer */
    (*xml)[len] = '\0';

    /* Set length */
    if (xml_len != NULL)
        *xml_len = len;

    /* Clean-up */
    if (encoder->flow_mode == FALSE)
        wbxml_buffer_destroy(header);

    return WBXML_OK;
}


/**
 * @brief Fill the XML Header
 * @param encoder The WBXML Encoder
 * @param header The Buffer to Fill
 * @return WBXML_OK if built is OK, an error code otherwise
 */
static WBXMLError xml_fill_header(WBXMLEncoder *encoder, WBXMLBuffer *header)
{
    if ((encoder == NULL) || (header == NULL))
        return WBXML_ERROR_BAD_PARAMETER;

    /** @todo Add 'encoding' info */

    /* <?xml version=\"1.0\"?> */
    if (!wbxml_buffer_append_cstr(header, WBXML_ENCODER_XML_HEADER))
        return WBXML_ERROR_ENCODER_APPEND_DATA;

    /* New Line */
    if (encoder->xml_gen_type == WBXML_GEN_XML_INDENT) {
        if (!xml_encode_new_line(header))
            return WBXML_ERROR_ENCODER_APPEND_DATA;
    }

    /* <!DOCTYPE */
    if (!wbxml_buffer_append_cstr(header, WBXML_ENCODER_XML_DOCTYPE))
        return WBXML_ERROR_ENCODER_APPEND_DATA;

    /* Root Element */
    if (!wbxml_buffer_append_cstr(header, encoder->lang->publicID->xmlRootElt))
        return WBXML_ERROR_ENCODER_APPEND_DATA;

    /*  PUBLIC " */
    if (!wbxml_buffer_append_cstr(header, WBXML_ENCODER_XML_PUBLIC))
        return WBXML_ERROR_ENCODER_APPEND_DATA;

    /* Public ID */
    if (!wbxml_buffer_append_cstr(header, encoder->lang->publicID->xmlPublicID))
        return WBXML_ERROR_ENCODER_APPEND_DATA;

    /* DTD */
    if (!wbxml_buffer_append_cstr(header, WBXML_ENCODER_XML_DTD))
        return WBXML_ERROR_ENCODER_APPEND_DATA;

    /* DTD */
    if (!wbxml_buffer_append_cstr(header, encoder->lang->publicID->xmlDTD))
        return WBXML_ERROR_ENCODER_APPEND_DATA;

    /* "> */
    if (!wbxml_buffer_append_cstr(header, WBXML_ENCODER_XML_END_DTD))
        return WBXML_ERROR_ENCODER_APPEND_DATA;

    /* New Line */
    if (encoder->xml_gen_type == WBXML_GEN_XML_INDENT) {
        if (!xml_encode_new_line(header))
            return WBXML_ERROR_ENCODER_APPEND_DATA;
    }

    return WBXML_OK;
}


/****************************
 * XML Encoding Functions
 */

/**
 * @brief Encode an XML Tag
 * @param encoder The WBXML Encoder
 * @param node The element to encode
 * @return WBXML_OK if encoding is OK, an error code otherwise
 */
static WBXMLError xml_encode_tag(WBXMLEncoder *encoder, WBXMLTreeNode *node)
{
    const WB_TINY *ns = NULL;
    WB_UTINY i;

    /* Set as current Tag */
    if (node->name->type == WBXML_VALUE_TOKEN)
        encoder->current_tag = node->name->u.token;
    else
        encoder->current_tag = NULL;

    /* Indent */
    if (encoder->xml_gen_type == WBXML_GEN_XML_INDENT) {
        for (i=0; i<(encoder->indent * encoder->indent_delta); i++) {
            if (!wbxml_buffer_append_char(encoder->output, ' '))
                return WBXML_ERROR_ENCODER_APPEND_DATA;
        }
    }

    /* Append < */
    if (!wbxml_buffer_append_char(encoder->output, '<'))
        return WBXML_ERROR_ENCODER_APPEND_DATA;

    /* Append Element Name */
    if (!wbxml_buffer_append_cstr(encoder->output, wbxml_tag_get_xml_name(node->name)))
        return WBXML_ERROR_ENCODER_APPEND_DATA;

    /* NameSpace handling: Check if Current Node Code Page is different than Parent Node Code Page */
    if ((encoder->lang->nsTable != NULL) &&
        ((node->parent == NULL) ||
         ((node->parent->type == WBXML_TREE_ELEMENT_NODE) &&
          (node->parent->name->type == WBXML_VALUE_TOKEN) &&
          (node->type == WBXML_TREE_ELEMENT_NODE) &&
          (node->name->type == WBXML_VALUE_TOKEN) &&
          (node->parent->name->u.token->wbxmlCodePage != node->name->u.token->wbxmlCodePage))))
    {
        if ((ns = wbxml_tables_get_xmlns(encoder->lang->nsTable, node->name->u.token->wbxmlCodePage)) != NULL)
        {
            /* Append xmlns=" */
            if (!wbxml_buffer_append_cstr(encoder->output, " xmlns=\""))
                return WBXML_ERROR_ENCODER_APPEND_DATA;

            /* Append NameSpace */
            if (!wbxml_buffer_append_cstr(encoder->output, ns))
                return WBXML_ERROR_ENCODER_APPEND_DATA;

            /* Append " */
            if (!wbxml_buffer_append_char(encoder->output, '"'))
                return WBXML_ERROR_ENCODER_APPEND_DATA;
        }
    }

    return WBXML_OK;
}


/**
 * @brief Encode an XML End Tag
 * @param encoder The WBXML Encoder
 * @param node Tag
 * @return WBXML_OK if encoding is OK, an error code otherwise
 */
static WBXMLError xml_encode_end_tag(WBXMLEncoder *encoder, WBXMLTreeNode *node)
{
    WB_UTINY i;

    if (encoder->xml_gen_type == WBXML_GEN_XML_INDENT) {

#if defined( WBXML_ENCODER_XML_NO_EMPTY_ELT_INDENT )
        if (wbxml_tree_node_have_child_elt(node)) {
#endif /* WBXML_ENCODER_XML_NO_EMPTY_ELT_INDENT */

            /* Add a New Line if there were content in this element */
            if (encoder->in_content) {
                if (!xml_encode_new_line(encoder->output))
                    return WBXML_ERROR_ENCODER_APPEND_DATA;
            }

            encoder->indent--;

            /* Indent End Element */
            for (i=0; i<(encoder->indent * encoder->indent_delta); i++) {
                if (!wbxml_buffer_append_char(encoder->output, ' '))
                    return WBXML_ERROR_ENCODER_APPEND_DATA;
            }

#if defined( WBXML_ENCODER_XML_NO_EMPTY_ELT_INDENT )
        }
#endif /* WBXML_ENCODER_XML_NO_EMPTY_ELT_INDENT */

    }

    /* Append </ */
    if (!wbxml_buffer_append_cstr(encoder->output, "</"))
        return WBXML_ERROR_ENCODER_APPEND_DATA;

    /* Append Element Name */
    if (!wbxml_buffer_append_cstr(encoder->output, wbxml_tag_get_xml_name(node->name)))
        return WBXML_ERROR_ENCODER_APPEND_DATA;

    /* Append > */
    if (!wbxml_buffer_append_char(encoder->output, '>'))
        return WBXML_ERROR_ENCODER_APPEND_DATA;

    /* New Line */
    if (encoder->xml_gen_type == WBXML_GEN_XML_INDENT) {
        if (!xml_encode_new_line(encoder->output))
            return WBXML_ERROR_ENCODER_APPEND_DATA;
    }

    /* No more in content */
    encoder->in_content = FALSE;

    return WBXML_OK;
}


/**
 * @brief Encode a XML Attribute
 * @param encoder [in] The WBXML Encoder
 * @param attribute [in] The Attribute to encode
 * @return WBXML_OK if encoding is OK, an error code otherwise
 */
static WBXMLError xml_encode_attr(WBXMLEncoder *encoder, WBXMLAttribute *attribute)
{
    /* Append a space */
    if (!wbxml_buffer_append_char(encoder->output, ' '))
        return WBXML_ERROR_ENCODER_APPEND_DATA;

    /* Append Attribute Name */
    if (!wbxml_buffer_append_cstr(encoder->output, wbxml_attribute_get_xml_name(attribute)))
        return WBXML_ERROR_ENCODER_APPEND_DATA;

    /* Append =" */
    if (!wbxml_buffer_append_cstr(encoder->output, "=\""))
        return WBXML_ERROR_ENCODER_APPEND_DATA;

    if (wbxml_attribute_get_xml_value(attribute) != NULL) {
        /* Fix Attribute Value text */
        WBXMLBuffer *tmp = NULL;

        /* Work with a temporary copy */
        if ((tmp = wbxml_buffer_create_from_cstr(wbxml_attribute_get_xml_value(attribute))) == NULL)
            return WBXML_ERROR_NOT_ENOUGH_MEMORY;

        /* Fix text */
        xml_fix_text(tmp, (WB_BOOL) (encoder->xml_gen_type == WBXML_GEN_XML_CANONICAL));

        /* Append Attribute Value */
        if (!wbxml_buffer_append(encoder->output, tmp)) {
            wbxml_buffer_destroy(tmp);
            return WBXML_ERROR_ENCODER_APPEND_DATA;
        }

        wbxml_buffer_destroy(tmp);
    }

    /* Append " */
    if (!wbxml_buffer_append_char(encoder->output, '"'))
        return WBXML_ERROR_ENCODER_APPEND_DATA;

    return WBXML_OK;
}


/**
 * @brief Encode a End of XML Attributes List
 * @param encoder [in] The WBXML Encoder
 * @param node    [in] Current node
 * @return WBXML_OK if encoding is OK, an error code otherwise
 */
static WBXMLError xml_encode_end_attrs(WBXMLEncoder *encoder, WBXMLTreeNode *node)
{
#if defined( WBXML_ENCODER_XML_GEN_EMPTY_ELT )

    if (node->children == NULL) {
        /* Append " />" */
        if (!wbxml_buffer_append_cstr(encoder->output, "/>"))
            return WBXML_ERROR_ENCODER_APPEND_DATA;

        /* New Line */
        if (encoder->xml_gen_type == WBXML_GEN_XML_INDENT) {
            if (!xml_encode_new_line(encoder->output))
                return WBXML_ERROR_ENCODER_APPEND_DATA;
        }
    }
    else {

#endif /* WBXML_ENCODER_XML_GEN_EMPTY_ELT */
            
        /* Append > */
        if (!wbxml_buffer_append_char(encoder->output, '>'))
            return WBXML_ERROR_ENCODER_APPEND_DATA;

        /* New Line */
        if (encoder->xml_gen_type == WBXML_GEN_XML_INDENT) {

#if defined( WBXML_ENCODER_XML_NO_EMPTY_ELT_INDENT )
            if (wbxml_tree_node_have_child_elt(node)) {
#endif /* WBXML_ENCODER_XML_NO_EMPTY_ELT_INDENT */

                if (!xml_encode_new_line(encoder->output))
                    return WBXML_ERROR_ENCODER_APPEND_DATA;

                /* Increment indentation */
                encoder->indent++;

#if defined( WBXML_ENCODER_XML_NO_EMPTY_ELT_INDENT )
            }
#endif /* WBXML_ENCODER_XML_NO_EMPTY_ELT_INDENT */

        }

#if defined( WBXML_ENCODER_XML_GEN_EMPTY_ELT )

    }

#endif /* WBXML_ENCODER_XML_GEN_EMPTY_ELT */

    return WBXML_OK;
}


/**
 * @brief Encode an XML Text
 * @param encoder The WBXML Encoder
 * @param node    The node containing XML Text to encode
 * @return WBXML_OK if encoding is OK, an error code otherwise
 */
static WBXMLError xml_encode_text(WBXMLEncoder *encoder, WBXMLTreeNode *node)
{
    WBXMLBuffer *str = node->content;
    WBXMLBuffer *tmp = NULL;
    WB_UTINY i = 0;

    if (encoder->in_cdata) {
        /* If we are in a CDATA section, do not modify the text to encode */
        if (!wbxml_buffer_append(encoder->output, str))
            return WBXML_ERROR_ENCODER_APPEND_DATA;
    }
    else {
        /* Work with a temporary copy */
        if ((tmp = wbxml_buffer_duplicate(str)) == NULL)
            return WBXML_ERROR_NOT_ENOUGH_MEMORY;

        /* Indent */
        if ((encoder->xml_gen_type == WBXML_GEN_XML_INDENT) &&
            (!encoder->in_content))
        {
#if defined( WBXML_ENCODER_XML_NO_EMPTY_ELT_INDENT )
            if (wbxml_tree_node_have_child_elt(node)) {
#endif /* WBXML_ENCODER_XML_NO_EMPTY_ELT_INDENT */

                /* Indent Content (only indent in first call to xml_encode_text()) */
                for (i=0; i<(encoder->indent * encoder->indent_delta); i++) {
                    if (!wbxml_buffer_append_char(encoder->output, ' ')) {
                        wbxml_buffer_destroy(tmp);
                        return WBXML_ERROR_ENCODER_APPEND_DATA;
                    }
                }

#if defined( WBXML_ENCODER_XML_NO_EMPTY_ELT_INDENT )
            }
#endif /* WBXML_ENCODER_XML_NO_EMPTY_ELT_INDENT */
        }

        /* Fix text */
        xml_fix_text(tmp, (WB_BOOL) (encoder->xml_gen_type == WBXML_GEN_XML_CANONICAL));

        /* Append Text */
        if (!wbxml_buffer_append(encoder->output, tmp)) {
            wbxml_buffer_destroy(tmp);
            return WBXML_ERROR_ENCODER_APPEND_DATA;
        }

        /* Clean-up */
        wbxml_buffer_destroy(tmp);
    }

    encoder->in_content = TRUE;

    return WBXML_OK;
}


/**
 * @brief Append a New Line to a Buffer
 * @param buff The Buffer
 * @return TRUE if added, FALSE otherwise
 */
static WB_BOOL xml_encode_new_line(WBXMLBuffer *buff)
{
    if (buff == NULL)
        return FALSE;

    return wbxml_buffer_append_data(buff, WBXML_ENCODER_XML_NEW_LINE, WBXML_STRLEN((const WB_TINY *)WBXML_ENCODER_XML_NEW_LINE));
}


/**
 * @brief Fix an XML text buffer (content text or attribute value)
 * @param buff The Buffer to fix
 * @param normalize Normalize text ?
 * @return WBXML_OK if ok, an Error Code otherwise
 * @note Reference: http://www.w3.org/TR/2004/REC-xml-20040204/#syntax
 */
static WB_BOOL xml_fix_text(WBXMLBuffer *buff, WB_BOOL normalize)
{
    WB_ULONG i = 0;
    WB_UTINY ch;

    for (i = 0; i < wbxml_buffer_len(buff); i++) {
        if (!wbxml_buffer_get_char(buff, i, &ch))
            continue;

        switch (ch) {
        case '<':
            /* Remove it */
            wbxml_buffer_delete(buff, i, 1);

            /* Write "&lt;" */
            if (!wbxml_buffer_insert_cstr(buff, (WB_UTINY *) xml_lt, i))
                return WBXML_ERROR_NOT_ENOUGH_MEMORY;

            break;

        case '>':
            /* Remove it */
            wbxml_buffer_delete(buff, i, 1);

            /* Write "&gt;" */
            if (!wbxml_buffer_insert_cstr(buff, (WB_UTINY *) xml_gt, i))
                return WBXML_ERROR_NOT_ENOUGH_MEMORY;

            break;

        case '&':
            /* Remove it */
            wbxml_buffer_delete(buff, i, 1);

            /* Write "&amp;" */
            if (!wbxml_buffer_insert_cstr(buff, (WB_UTINY *) xml_amp, i))
                return WBXML_ERROR_NOT_ENOUGH_MEMORY;

            break;

        case '"':
            /* Remove it */
            wbxml_buffer_delete(buff, i, 1);

            /* Write "&quot;" */
            if (!wbxml_buffer_insert_cstr(buff, (WB_UTINY *) xml_quot, i))
                return WBXML_ERROR_NOT_ENOUGH_MEMORY;

            break;

        case '\'':
            /* Remove it */
            wbxml_buffer_delete(buff, i, 1);

            /* Write "&apos;" */
            if (!wbxml_buffer_insert_cstr(buff, (WB_UTINY *) xml_apos, i))
                return WBXML_ERROR_NOT_ENOUGH_MEMORY;

            break;

        case '\r':
            if (normalize) {
                /* Remove it */
                wbxml_buffer_delete(buff, i, 1);

                /* Write "&#13;" */
                if (!wbxml_buffer_insert_cstr(buff, (WB_UTINY *) xml_slashr, i))
                    return WBXML_ERROR_NOT_ENOUGH_MEMORY;
            }
            break;

        case '\n':
            if (normalize) {
                /* Remove it */
                wbxml_buffer_delete(buff, i, 1);

                /* Write "&#10;" */
                if (!wbxml_buffer_insert_cstr(buff, (WB_UTINY *) xml_slashn, i))
                    return WBXML_ERROR_NOT_ENOUGH_MEMORY;
            }
            break;

        case '\t':
            if (normalize) {
                /* Remove it */
                wbxml_buffer_delete(buff, i, 1);

                /* Write "&#9;" */
                if (!wbxml_buffer_insert_cstr(buff, (WB_UTINY *) xml_tab, i))
                    return WBXML_ERROR_NOT_ENOUGH_MEMORY;
            }
            break;

        default:
            /* Do Nothing */
            break;
        }
    }

    return WBXML_OK;
}


/**
 * @brief Encode a begin of CDATA section
 * @param encoder The WBXML Encoder
 * @return WBXML_OK if encoding is OK, an error code otherwise
 */
static WBXMLError xml_encode_cdata(WBXMLEncoder *encoder)
{
    /* Append <![CDATA[ */
    if (!wbxml_buffer_append_cstr(encoder->output, "<![CDATA["))
        return WBXML_ERROR_ENCODER_APPEND_DATA;

    return WBXML_OK;
}


/**
 * @brief Encode an end of CDATA section
 * @param encoder The WBXML Encoder
 * @return WBXML_OK if encoding is OK, an error code otherwise
 */
static WBXMLError xml_encode_end_cdata(WBXMLEncoder *encoder)
{
    /* Append ]]> */
    if (!wbxml_buffer_append_cstr(encoder->output, "]]>"))
        return WBXML_ERROR_ENCODER_APPEND_DATA;

    return WBXML_OK;
}


/**
 * @brief Encode an encapsulated WBXML Tree to XML
 * @param encoder [in] The WBXML Encoder
 * @param tree    [in] The WBXML Tree to encode
 * @return WBXML_OK if encoding is OK, an error code otherwise
 */
static WBXMLError xml_encode_tree(WBXMLEncoder *encoder, WBXMLTree *tree)
{
    WBXMLEncoder *new_encoder = NULL;
    WB_UTINY     *xml         = NULL;
    WB_ULONG      xml_len     = 0;
    WBXMLError    ret         = WBXML_OK;

    if ((new_encoder = encoder_duplicate(encoder)) == NULL)
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;

    /* Set Tree */
    new_encoder->tree = tree;

    /* Encode to XML */
    if ((ret = wbxml_encoder_encode_tree_to_xml(new_encoder, &xml, &xml_len)) != WBXML_OK) {
        wbxml_encoder_destroy(new_encoder);
        return ret;
    }

    /* Clean-up */
    wbxml_encoder_destroy(new_encoder);

    /** @bug Handle output_charset ! */

    /* Append xml to output */
    if (!wbxml_buffer_append_cstr(encoder->output, xml)) {
        wbxml_free(xml);
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;
    }

    /* Clean-up */
    wbxml_free(xml);

    return WBXML_OK;
}
