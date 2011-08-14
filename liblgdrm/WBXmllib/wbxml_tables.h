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
 * @file wbxml_tables.h
 * @ingroup wbxml_tables
 *
 * @author Aymerick Jehanne <libwbxml@aymerick.com>
 * @date 02/03/17
 *
 * @brief WBXML Tables
 */

#ifndef WBXML_TABLES_H
#define WBXML_TABLES_H


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @addtogroup wbxml_tables 
 *  @{ 
 */

/****************************************************
 *    WBXML Public IDs (http://www.wapforum.org/wina)
 */

#define WBXML_PUBLIC_ID_UNKNOWN   	0x01

/* DRM */
#define WBXML_PUBLIC_ID_DRMREL10  	0x0E
#define WBXML_PUBLIC_ID_DRM2_1  		0x13
#define WBXML_PUBLIC_ID_SRM  				0x14

#define XML_PUBLIC_ID_DRMREL10  	"-//OMA//DTD DRMREL 1.0//EN"
#define XML_PUBLIC_ID_DRM2_1  		"-//OMA//DRM 2.1//EN"
#define XML_PUBLIC_ID_SRM			  		"-//OMA//SRM 1.0//EN"

/****************************************************
 *    WBXML Tables Structures
 */

/**
 * @brief WBXML Public ID structure
 */
typedef struct WBXMLPublicIDEntry_s
{
    WB_ULONG       wbxmlPublicID; /**< WBXML Public ID */
    const WB_TINY *xmlPublicID;   /**< XML Public ID */
    const WB_TINY *xmlRootElt;    /**< XML Root Element */
    const WB_TINY *xmlDTD;        /**< XML DTD */

} WBXMLPublicIDEntry;


/**
 * @brief WBXML Application Token structure: Tag token
 */
typedef struct WBXMLTagEntry_s
{    
    const WB_TINY *xmlName;       /**< XML Tag Name */    
    WB_UTINY       wbxmlCodePage; /**< WBXML Code Page */
    WB_UTINY       wbxmlToken;    /**< WBXML Tag Token */
} WBXMLTagEntry;


/**
 * @brief Name Space
 * @note For SyncML, where a WBXML Code Page is associated to an XML Name Space
 */
typedef struct WBXMLNameSpaceEntry_s
{    
    const WB_TINY *xmlNameSpace;  /**< XML Name Space */   
    WB_UTINY       wbxmlCodePage; /**< WBXML Code Page */
} WBXMLNameSpaceEntry;


/**
 * @brief WBXML Application Token structure: Attribute token
 */
typedef struct WBXMLAttrEntry_s
{
    const WB_TINY *xmlName;       /**< XML Attribute Name */
    const WB_TINY *xmlValue;      /**< XML Attribute Value (may be NULL) */
    WB_UTINY       wbxmlCodePage; /**< WBXML Code Page */
    WB_UTINY       wbxmlToken;    /**< WBXML Attribute Token */
} WBXMLAttrEntry;


/**
 * @brief WBXML Application Token structure: Attribute Value token
 */
typedef struct WBXMLAttrValueEntry_s
{
    const WB_TINY *xmlName;       /**< XML Attribute Value */
    WB_UTINY       wbxmlCodePage; /**< WBXML Code Page */
    WB_UTINY       wbxmlToken;    /**< WBXML Attribute Value Token */
} WBXMLAttrValueEntry;


/**
 * @brief WBXML Application Token structure: Extension Value token
 * @note For Wireless-Village, the content can be tokenized with Extension Tokens
 */
typedef struct WBXMLExtValueEntry_s
{
    const WB_TINY *xmlName;    /**< XML Extension Value */
    WB_UTINY       wbxmlToken; /**< WBXML Extension Value Token */
} WBXMLExtValueEntry;


/**
 * @brief Language structure
 */
typedef struct WBXMLLangEntry_s
{
    WBXMLLanguage              langID;         /**< Language ID */
    const WBXMLPublicIDEntry  *publicID;       /**< Public ID */
    const WBXMLTagEntry       *tagTable;       /**< Tags Table */
    const WBXMLNameSpaceEntry *nsTable;        /**< NameSpaces Table */
    const WBXMLAttrEntry      *attrTable;      /**< Attributes Table*/
    const WBXMLAttrValueEntry *attrValueTable; /**< Attributes Values Table */
    const WBXMLExtValueEntry  *extValueTable;  /**< Extensions Values Table */
} WBXMLLangEntry;


/**
 * @brief Get Main Table
 * @return The main array of WBXML Language Tables
 */
const WBXMLLangEntry * wbxml_tables_get_main(void);

/**
 * @brief Get a Language Table
 * @param lang Language to get
 * @return The Language Table, or NULL if unknown Language
 */
const WBXMLLangEntry * wbxml_tables_get_table(WBXMLLanguage lang);

/**
 * @brief Search for a Language Table
 * @param main_table Main Table Array to search in
 * @param public_id The Public ID to search [can be NULL]
 * @param system_id The System ID to search [can be NULL]
 * @param root The Root Element to search [can be NULL]
 * @return The Language Table found, or NULL if none found
 * @note This function try to find the correct Language Table thanks to the XML Public ID, then (if not found) by
 *       the XML System ID, and finally (if not found) by the Root XML Element
 */
const WBXMLLangEntry * wbxml_tables_search_table(const WBXMLLangEntry *main_table,
                                                                const WB_UTINY *public_id, 
                                                                const WB_UTINY *system_id,
                                                                const WB_UTINY *root);

/**
 * @brief Get the WBXML Public ID corresponding to given WBXML Language
 * @param main_table The Main Languages Table to search in
 * @param lang_id The Language ID
 * @return The WBXML Public ID (can be WBXML_PUBLIC_ID_UNKNOWN if Language doesn't have one, or if Language not found)
*/
WB_ULONG wbxml_tables_get_wbxml_publicid(const WBXMLLangEntry *main_table,
                                                        WBXMLLanguage lang_id);

/**
 * @brief Search for a Tag Entry in Language Table, given the XML Name of the Tag
 * @param lang_table The Language Table to search in
 * @param xml_name The XML Name of the Tag to search
 * @return The Tag Entry of this XML Name in Language Table, or NULL if not found
 */
const WBXMLTagEntry * wbxml_tables_get_tag_from_xml(const WBXMLLangEntry *lang_table,
                                                                   const WB_UTINY *xml_name);

/**
 * @brief Search for an Attribute Entry in Language Table, given the XML Name and Value of the Attribute
 * @param lang_table The Language Table to search in
 * @param xml_name The XML Name of the Attribute to search
 * @param xml_value The XML Value of the Attribute to search
 * @param value_left Is the WBXMLAttrEntry returned EXACTLY the Attribute we are searching ? (ie: is the Attribute Value
 *                   found matching the one we were looking for ?). If Yes, then this is NULL. If not, then this is the
 *                   attribute value part that we still have to encode.
 * @return The Attribute Entry of this XML Attribute Name in Language Table, or NULL if not found
 * @note Has the Attribut Value can be expressed in many ways in WBXML, this function is focused on
 *       searching for the ATTRIBUTE NAME !
 *       Thus, when Attribute Name is found in Table, we search for an Entry with the same Attribute Name / Attribute Value
 *       pair. If found the 'value_left' parameter is set to NULL. If not, we still return an Entry matching the Attribute Name,
 *       but the 'value_left' parameter is the Attribute Value part that is not included in the Attrbute Token.
 */
const WBXMLAttrEntry * wbxml_tables_get_attr_from_xml(const WBXMLLangEntry *lang_table,
                                                                     WB_UTINY *xml_name,
                                                                     WB_UTINY *xml_value,
                                                                     WB_UTINY **value_left);

/**
 * @brief Search for an Extension Token Entry in Language Table, given the XML Value of the Extension
 * @param lang_table The Language Table to search in
 * @param xml_value The XML Value of the Extension to search
 * @return The Extension Token Entry of this XML Value in Language Table, or NULL if not found
 */
const WBXMLExtValueEntry * wbxml_tables_get_ext_from_xml(const WBXMLLangEntry *lang_table,
                                                                        WB_UTINY *xml_value);

/**
 * @brief Check if an XML Attribute Value contains at least one Attribute Value defined in Language Attribute Values Table
 * @param lang_table The Language Table to search in
 * @param xml_value The XML Attribute Value to check
 * @return TRUE if this value contains an Attribute Value, FALSE otherwise
 */
WB_BOOL wbxml_tables_contains_attr_value_from_xml(const WBXMLLangEntry *lang_table,
                                                                 WB_UTINY *xml_value);

/**
 * @brief Get an XML NameSpace, given a WBXML Code Page
 * @param ns_table  The NameSpace Table
 * @param code_page The WBXML Code Page
 * @return The XML NameSpace, or NULL if not found
 */
const WB_TINY * wbxml_tables_get_xmlns(const WBXMLNameSpaceEntry *ns_table,
                                                      WB_UTINY code_page);

#if defined( WBXML_DYNAMIC_CODEPAGE ) 
WB_LONG wbxml_tables_get_num(WBXMLLanguage lang, WBXMLTableType type);
WB_BOOL wbxml_tables_copy(void *dst, WBXMLLanguage lang, WBXMLTableType type, WB_LONG count);
void *wbxml_tables_replace(void *src, WBXMLLanguage lang, WBXMLTableType type);
#endif

/** @} */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* WBXML_TABLES_H */
