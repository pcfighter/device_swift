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
 * @file wbxml_encoder.h
 * @ingroup wbxml_encoder
 *
 * @author Aymerick Jehanne <libwbxml@aymerick.com>
 * @date 11/11/02
 *
 * @brief WBXML Encoder - Encodes a WBXML Tree to WBXML or to XML
 */

#ifndef WBXML_ENCODER_H
#define WBXML_ENCODER_H


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @addtogroup wbxml_encoder
 *  @{ 
 */

/**
 * @brief WBXML Encoder
 */
typedef struct WBXMLEncoder_s WBXMLEncoder;

/**
 * @brief WBXML Encoder Output Type
 */
typedef enum WBXMLEncoderOutputType_e {
    WBXML_ENCODER_OUTPUT_WBXML = 0,
    WBXML_ENCODER_OUTPUT_XML
} WBXMLEncoderOutputType;


/**
 * @brief Create a WBXML Encoder
 * @result Return the newly created WBXMLEncoder, or NULL if not enough memory
 * @warning Do NOT use this function directly, use wbxml_encoder_create() macro instead
 */
WBXMLEncoder * wbxml_encoder_create_real(void);
#define wbxml_encoder_create() wbxml_mem_cleam(wbxml_encoder_create_real())

/**
 * @brief Destroy a WBXML Encoder
 * @param encoder The WBXMLEncoder to free
 */
void wbxml_encoder_destroy(WBXMLEncoder *encoder);

/**
 * @brief Reset a WBXML Encoder
 * @param encoder The WBXMLEncoder to reset
 */
void wbxml_encoder_reset(WBXMLEncoder *encoder);


/**
 * @brief Set the WBXML Encoder to ignore empty texts (ie: ignorable Whitespaces) [Default: FALSE]
 * @param encoder [in] The WBXML Encoder
 * @param set_ignore [in] TRUE if ignore, FALSE otherwise
 * @warning This behaviour can me overriden by the WBXML_GEN_XML_CANONICAL mode (set by wbxml_encoder_set_xml_gen_type())
 */
void wbxml_encoder_set_ignore_empty_text(WBXMLEncoder *encoder, WB_BOOL set_ignore);

/**
 * @brief Set the WBXML Encoder to remove leading and trailing blanks in texts [Default: FALSE]
 * @param encoder [in] The WBXML Encoder
 * @param set_remove [in] TRUE if remove, FALSE otherwise
 * @warning This behaviour can me overriden by the WBXML_GEN_XML_CANONICAL mode (set by wbxml_encoder_set_xml_gen_type())
 */
void wbxml_encoder_set_remove_text_blanks(WBXMLEncoder *encoder, WB_BOOL set_remove);

/**
 * @brief Set output document Charset Encoding
 * @param encoder [in] The WBXML Encoder
 * @param charset [in] The Charset to set
 * @note - This overrides the Charset Encoding found in WBXML Tree.
 *       - If not set with this function, the 'orig_charset' field of the WBXML Tree is used
 *         (ie the Charset Encoding of the original document).
 *       - If no 'orig_charset' is set in WBXML Tree, the default charset is used : 
 *          - 'WBXML_ENCODER_WBXML_DEFAULT_CHARSET' when encoding to XML
 *          - 'WBXML_ENCODER_XML_DEFAULT_CHARSET' when encoding to WBXML
 */
void wbxml_encoder_set_output_charset(WBXMLEncoder *encoder, WBXMLCharsetMIBEnum charset);


/**
 * @brief Set if we use String Table when Encoding into WBXML [Default: TRUE]
 * @param encoder [in] The WBXML Encoder
 * @param use_strtbl [in] TRUE if we use String Table, FALSE otherwise
 * @note This function has no effect if WBXML_ENCODER_USE_STRTBL compilation flag is not set
 */
void wbxml_encoder_set_use_strtbl(WBXMLEncoder *encoder, WB_BOOL use_strtbl);

/**
 * @brief Set the WBXML Version of the output document, when generating WBXML [Default: 'WBXML_VERSION_TOKEN_13' (1.3)]
 * @param encoder [in] The WBXML Encoder
 * @param version [in] The WBXML Version
 */
void wbxml_encoder_set_wbxml_version(WBXMLEncoder *encoder, WBXMLVersion version);


/**
 * @brief Set the WBXML Encoder XML Generation Type, when generating XML [Default: WBXML_GEN_XML_COMPACT]
 * @param encoder [in] The WBXML Encoder
 * @param gen_type [in] Generation Type (cf. WBXMLEncoderXMLGen enum)
 */
void wbxml_encoder_set_xml_gen_type(WBXMLEncoder *encoder, WBXMLGenXMLType gen_type);

/**
 * @brief Set the WBXML Encoder indent, when generating XML in WBXML_GEN_XML_INDENT mode [Default: 0]
 * @param encoder [in] The WBXML Encoder
 * @param indent [in] If 'WBXML_GEN_XML_INDENT' type is used, this is the number of spaces for indent
 */
void wbxml_encoder_set_indent(WBXMLEncoder *encoder, WB_UTINY indent);


/**
 * @brief Set the WBXML Tree to encode
 *
 * Use this method before calling wbxml_encoder_encode_tree_to_wbxml() or wbxml_encoder_encode_tree_to_xml().
 *
 * @param encoder [in] The WBXML Encoder to use
 * @param tree [in] The WBXML Tree to encode
 */
void wbxml_encoder_set_tree(WBXMLEncoder *encoder, WBXMLTree *tree);

#if defined( WBXML_DRMEXT )
void wbxml_encoder_set_use_strtbl_literal(WBXMLEncoder *encoder, WB_BOOL use_strtbl_literal);
void wbxml_encoder_set_record_literals(WBXMLEncoder *encoder, WB_BOOL record_literals);
void wbxml_encoder_set_record_attrvalue_literals(WBXMLEncoder *encoder, WB_BOOL record_attrvalue_literals);
void wbxml_encoder_get_literal_nodes(WBXMLEncoder *encoder, WBXMLGenWBXMLParams *params);
#endif

/**
 * @brief Encode the WBXML Tree attached to this encoder into WBXML
 *
 * Call wbxml_encoder_set_tree() before using this method.
 *
 * @param encoder [in] The WBXML Encoder to use
 * @param wbxml [out] Resulting WBXML document
 * @param wbxml_len [out] The resulting WBXML document length
 * @return Return WBXML_OK if no error, an error code otherwise
 * @warning The 'encoder->tree' WBXMLLib Tree MUST be already set with a call to wbxml_encoder_set_tree() function
 */
WBXMLError wbxml_encoder_encode_tree_to_wbxml(WBXMLEncoder *encoder, WB_UTINY **wbxml, WB_ULONG *wbxml_len);

/* BC */
#define wbxml_encoder_encode_to_wbxml(a,b,c) wbxml_encoder_encode_tree_to_wbxml(a,b,c)

/**
 * @brief Encode the WBXML Tree attached to this encoder into XML
 *
 * Call wbxml_encoder_set_tree() before using this method.
 *
 * @param encoder [in] The WBXML Encoder to use
 * @param xml     [out] Resulting XML document
 * @param xml_len [out] XML document length
 * @return Return WBXML_OK if no error, an error code otherwise
 * @warning The 'encoder->tree' WBXMLLib Tree MUST be already set with a call to wbxml_encoder_set_tree() function
 */
WBXMLError wbxml_encoder_encode_tree_to_xml(WBXMLEncoder *encoder, WB_UTINY **xml, WB_ULONG *xml_len);

/* BC */
#define wbxml_encoder_encode_to_xml(a,b,c) wbxml_encoder_encode_tree_to_xml(a,b,c)


/**
 * @brief Set the encoder into 'Flow Mode' (to encode nodes directly)
 *
 * Use this method to the set the encoder in 'Flow Mode'. This permits to encode WBXML Nodes 'on the fly', without
 * having to encode a whole WBXML Tree.
 *
 * You should use this function (with TRUE parameter) before calling wbxml_encoder_encode_node().
 *
 * @param encode    [in] The WBXML Encoder to use
 * @param flow_mode [in] Set Flow Mode ?
 * @return Return WBXML_OK if no error, an error code otherwise
 */
WBXMLError wbxml_encoder_set_flow_mode(WBXMLEncoder *encoder, WB_BOOL flow_mode);

/**
 * @brief Set the output type (WBXML_ENCODER_OUTPUT_XML | WBXML_ENCODER_OUTPUT_WBXML)
 * @param encoder [in] The WBXML Encoder
 * @param output_type [in] The output type
 */
void wbxml_encoder_set_output_type(WBXMLEncoder *encoder, WBXMLEncoderOutputType output_type);

/**
 * @brief Set the language to use
 * @param encoder [in] The WBXML Encoder
 * @param lang    [in] The language to use
 */
void wbxml_encoder_set_lang(WBXMLEncoder *encoder, WBXMLLanguage lang);

/**
 * @brief Generate textual Public ID instead of token
 * @param encoder  [in] The WBXML Encoder
 * @param gen_text [in] Set or unset
 */
void wbxml_encoder_set_text_public_id(WBXMLEncoder *encoder, WB_BOOL gen_text); 

/**
 * @brief Encode a WBXML Tree Node
 *
 * This function directly encode a WBXMLTreeNode. So you can use if you don't want to encode a whole WBXML Tree.
 *
 * You should call wbxml_encoder_set_flow_mode(TRUE), wbxml_encoder_set_output_type() before using this function.
 * You must call wbxml_encoder_set_lang() before using this function.
 *
 * @param encode [in] The WBXML Encoder to use
 * @param node   [in] The WBXML Tree Node to encode
 * @return Return WBXML_OK if no error, an error code otherwise
 */
WBXMLError wbxml_encoder_encode_node(WBXMLEncoder *encoder, WBXMLTreeNode *node);

/**
 * @brief Encode node, but if node is an Element, choose the encode 'end' tag or not
 *
 * @param encoder [in] The WBXML Encoder to use
 * @param tree    [in] The WBXML Tree to encode
 * @param enc_end [in] Encoded element 'end' ?
 * @return Return WBXML_OK if no error, an error code otherwise
 */
WBXMLError wbxml_encoder_encode_node_with_elt_end(WBXMLEncoder *encoder, WBXMLTreeNode *node, WB_BOOL enc_end);

/**
 * @brief Encode a WBXML Tree
 *
 * You should call wbxml_encoder_set_flow_mode(TRUE), wbxml_encoder_set_output_type() before using this function.
 *
 * @param encoder [in] The WBXML Encoder to use
 * @param tree    [in] The WBXML Tree to encode
 * @return Return WBXML_OK if no error, an error code otherwise
 */
WBXMLError wbxml_encoder_encode_tree(WBXMLEncoder *encoder, WBXMLTree *tree);

/**
 * @brief Encode a raw element start
 * @param encoder     [in] The WBXML Encoder to use
 * @param node        [in] The WBXML Tree Node representing the element start to encode
 * @param has_content [in] Does the element has content ?
 * @return Return WBXML_OK if no error, an error code otherwise
 */
WBXMLError wbxml_encoder_encode_raw_elt_start(WBXMLEncoder *encoder, WBXMLTreeNode *node, WB_BOOL has_content);

/**
 * @brief Encode a raw element end
 * @param encoder     [in] The WBXML Encoder to use
 * @param node        [in] The WBXML Tree Node representing the element end to encode
 * @param has_content [in] Does the element has content ?
 * @return Return WBXML_OK if no error, an error code otherwise
 */
WBXMLError wbxml_encoder_encode_raw_elt_end(WBXMLEncoder *encoder, WBXMLTreeNode *node, WB_BOOL has_content);

/**
 * @brief Get currently encoded buffer
 * @param encoder    [in] The WBXML Encoder to use
 * @param result     [out] Resulting buffer
 * @param result_len [out] Resulting buffer length
 * @return Return WBXML_OK if no error, an error code otherwise
 */
WBXMLError wbxml_encoder_get_output(WBXMLEncoder *encoder, WB_UTINY **result, WB_ULONG *result_len);

/**
 * @brief Get currently encoded buffer length
 * @param encoder [in] The WBXML Encoder to use
 * @return Return WBXML_OK if no error, an error code otherwise
 */
WB_ULONG wbxml_encoder_get_output_len(WBXMLEncoder *encoder);

/**
 * @brief Delete bytes from output buffer (from end of buffer)
 * @param encoder [in] The WBXML Encoder to use
 * @param nb      [in] Number of bytes to delete
 */
void wbxml_encoder_delete_output_bytes(WBXMLEncoder *encoder, WB_ULONG nb);

/**
 * @brief Delete last encoded node
 * @param encoder [in] The WBXML Encoder to use
 */
void wbxml_encoder_delete_last_node(WBXMLEncoder *encoder);

/** @} */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* WBXML_ENCODER_H */
