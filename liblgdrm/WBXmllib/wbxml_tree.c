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
 * @file wbxml_tree.c
 * @ingroup wbxml_tree
 *
 * @author Aymerick Jehanne <libwbxml@aymerick.com>
 * @date 03/02/18
 *
 * @brief WBXML Tree
 */

#include "wbxml.h"


/***************************************************
 *    Public Functions
 */

WBXMLError wbxml_tree_from_wbxml(WB_UTINY *wbxml, WB_ULONG wbxml_len, WBXMLLanguage lang, WBXMLTree **tree)
{
    WBXMLParser *wbxml_parser = NULL;
    WB_LONG error_index = 0;
    WBXMLTreeClbCtx wbxml_tree_clb_ctx;
    WBXMLError ret = WBXML_OK;
    WBXMLContentHandler wbxml_tree_content_handler = 
        {
            wbxml_tree_clb_wbxml_start_document,
            wbxml_tree_clb_wbxml_end_document,
            wbxml_tree_clb_wbxml_start_element,
            wbxml_tree_clb_wbxml_end_element,
            wbxml_tree_clb_wbxml_characters,
            wbxml_tree_clb_wbxml_pi
        };

    if (tree != NULL)
        *tree = NULL;

    /* Create WBXML Parser */
    if((wbxml_parser = wbxml_parser_create()) == NULL) {
        WBXML_ERROR((WBXML_PARSER, "Can't create WBXML Parser"));
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;
    }

    /* Init context */
    wbxml_tree_clb_ctx.error = WBXML_OK;
    wbxml_tree_clb_ctx.current = NULL;
    if ((wbxml_tree_clb_ctx.tree = wbxml_tree_create(WBXML_LANG_UNKNOWN, WBXML_CHARSET_UNKNOWN)) == NULL) {
        wbxml_parser_destroy(wbxml_parser);
        WBXML_ERROR((WBXML_PARSER, "Can't create WBXML Tree"));
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;
    }
    
    /* Set Handlers Callbacks */
    wbxml_parser_set_user_data(wbxml_parser, &wbxml_tree_clb_ctx);
    wbxml_parser_set_content_handler(wbxml_parser, &wbxml_tree_content_handler);

    /* Give the user the possibility to force Document Language */
    if (lang != WBXML_LANG_UNKNOWN)
        wbxml_parser_set_language(wbxml_parser, lang);

    /** @todo Use wbxml_parser_set_meta_charset() */

    /* Parse the WBXML document to WBXML Tree */
    ret = wbxml_parser_parse(wbxml_parser, wbxml, wbxml_len);
    if ((ret != WBXML_OK) || (wbxml_tree_clb_ctx.error != WBXML_OK)) 
    {
        error_index = wbxml_parser_get_current_byte_index(wbxml_parser);
        WBXML_ERROR((WBXML_PARSER, "WBXML Parser failed at %ld - token: %x (%s)", 
                                   error_index,
                                   wbxml[error_index],
                                   ret != WBXML_OK ? wbxml_errors_string(ret) : wbxml_errors_string(wbxml_tree_clb_ctx.error)));
        
        wbxml_tree_destroy(wbxml_tree_clb_ctx.tree);
    }
    else {
        *tree = wbxml_tree_clb_ctx.tree;
    }

    /* Clean-up */
    wbxml_parser_destroy(wbxml_parser);

    if (ret != WBXML_OK)
        return ret;
    else
        return wbxml_tree_clb_ctx.error;
}


WBXMLError wbxml_tree_to_wbxml(WBXMLTree *tree,
                                              WB_UTINY **wbxml,
                                              WB_ULONG  *wbxml_len,
                                              WBXMLGenWBXMLParams *params)
{
    WBXMLEncoder *wbxml_encoder = NULL;
    WBXMLError ret = WBXML_OK;

    /* Encode WBXML Tree to WBXML Document */
    if ((wbxml_encoder = wbxml_encoder_create()) == NULL) {
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;
    }

    /* Set the WBXML Tree to encode */
    wbxml_encoder_set_tree(wbxml_encoder, tree);

    /* Set encoder parameters */
    if (params == NULL) {
        /* Default Parameters */

        /* Ignores "Empty Text" Nodes */
        wbxml_encoder_set_ignore_empty_text(wbxml_encoder, TRUE);

        /* Remove leading and trailing whitespaces in "Text Nodes" */
        wbxml_encoder_set_remove_text_blanks(wbxml_encoder, TRUE);

        /* Use String Table */
        wbxml_encoder_set_use_strtbl(wbxml_encoder, TRUE);
    }
    else {
        /* WBXML Version */
        wbxml_encoder_set_wbxml_version(wbxml_encoder, params->wbxml_version);

        /* Keep Ignorable Whitespaces ? */
        if (!params->keep_ignorable_ws) {
            /* Ignores "Empty Text" Nodes */
            wbxml_encoder_set_ignore_empty_text(wbxml_encoder, TRUE);

            /* Remove leading and trailing whitespaces in "Text Nodes" */
            wbxml_encoder_set_remove_text_blanks(wbxml_encoder, TRUE);
        }

        /* String Table */
        wbxml_encoder_set_use_strtbl(wbxml_encoder, params->use_strtbl);

#if defined( WBXML_DRMEXT )
		/* Only literals use string table */
		wbxml_encoder_set_use_strtbl_literal(wbxml_encoder, params->use_strtbl_literal);

		/* Record all literals */
		wbxml_encoder_set_record_literals(wbxml_encoder, params->record_literals);

		/* Record attribute values */
		wbxml_encoder_set_record_attrvalue_literals(wbxml_encoder, params->record_attrvalue_literals);
#endif

        /** @todo Add parameter to call : wbxml_encoder_set_output_charset() */
    }

    /* Encode WBXML */
    ret = wbxml_encoder_encode_to_wbxml(wbxml_encoder, wbxml, wbxml_len);

#if defined( WBXML_DRMEXT )
	if (ret == WBXML_OK && params)
	{
		wbxml_encoder_get_literal_nodes(wbxml_encoder, params);
	}
#endif

    /* Clean-up */
    wbxml_encoder_destroy(wbxml_encoder);

    return ret;
}


WBXMLError wbxml_tree_from_xml(WB_UTINY *xml, WB_ULONG xml_len, WBXMLTree **tree)
{
#if defined( HAVE_EXPAT )

    const XML_Feature *feature_list = NULL;
    XML_Parser_drm         xml_parser   = NULL;
    WBXMLError         ret          = WBXML_OK;
    WB_BOOL            expat_utf16  = FALSE;
    WBXMLTreeClbCtx    wbxml_tree_clb_ctx;

    /* First Check if Expat is outputing UTF-16 strings */
    feature_list = (const XML_Feature *)XML_GetFeatureList_drm();

    if ((feature_list != NULL) && (feature_list[0].value != sizeof(WB_TINY))) {
#if !defined( HAVE_ICONV )
        /* Ouch, can't convert from UTF-16 to UTF-8 */
        return WBXML_ERROR_XMLPARSER_OUTPUT_UTF16;
#else
        /* Expat returns UTF-16 encoded strings in its callbacks */
        expat_utf16 = TRUE;
#endif /* !HAVE_ICONV */
    }

    if (tree != NULL)
        *tree = NULL;

    /* Create Expat XML Parser */
    if ((xml_parser = XML_ParserCreate_drm(NULL)) == NULL)
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;
    
    /* Init context */
    wbxml_tree_clb_ctx.current = NULL;
    wbxml_tree_clb_ctx.error = WBXML_OK;
    wbxml_tree_clb_ctx.skip_lvl = 0;
    wbxml_tree_clb_ctx.skip_start = 0;
    wbxml_tree_clb_ctx.xml_parser = xml_parser;
    wbxml_tree_clb_ctx.input_buff = xml;
    wbxml_tree_clb_ctx.expat_utf16 = expat_utf16;

    /* Create WBXML Tree */
    if ((wbxml_tree_clb_ctx.tree = wbxml_tree_create(WBXML_LANG_UNKNOWN, WBXML_CHARSET_UNKNOWN)) == NULL) {
        XML_ParserFree_drm(xml_parser);
        WBXML_ERROR((WBXML_PARSER, "Can't create WBXML Tree"));
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;
    }
    
    /* Set Handlers Callbacks */
    XML_SetXmlDeclHandler_drm(xml_parser, wbxml_tree_clb_xml_decl);
    XML_SetStartDoctypeDeclHandler_drm(xml_parser, wbxml_tree_clb_xml_doctype_decl);
    XML_SetElementHandler_drm(xml_parser, wbxml_tree_clb_xml_start_element, wbxml_tree_clb_xml_end_element);
    XML_SetCdataSectionHandler_drm(xml_parser, wbxml_tree_clb_xml_start_cdata, wbxml_tree_clb_xml_end_cdata);
    XML_SetProcessingInstructionHandler_drm(xml_parser , wbxml_tree_clb_xml_pi);
    XML_SetCharacterDataHandler_drm(xml_parser, wbxml_tree_clb_xml_characters);
    XML_SetUserData_drm(xml_parser, (void*)&wbxml_tree_clb_ctx);

    /* Parse the XML Document to WBXML Tree */
    if (XML_Parse_drm(xml_parser, (WB_TINY*) xml, xml_len, TRUE) == 0)
    {
        WBXML_ERROR((WBXML_CONV, "xml2wbxml convertion failed - expat error %i\n"
            "\tdescription: %s\n"
            "\tline: %i\n"
            "\tcolumn: %i\n"
            "\tbyte index: %i\n"
            "\ttotal bytes: %i\n%s",
            XML_GetErrorCode_drm(xml_parser), 
            XML_ErrorString_drm(XML_GetErrorCode_drm(xml_parser)), 
            XML_GetCurrentLineNumber_drm(xml_parser), 
            XML_GetCurrentColumnNumber_drm(xml_parser), 
            XML_GetCurrentByteIndex_drm(xml_parser), 
            XML_GetCurrentByteCount_drm(xml_parser), xml));

        wbxml_tree_destroy(wbxml_tree_clb_ctx.tree);

        ret = WBXML_ERROR_XML_PARSING_FAILED;
    }
    else {
        if ((ret = wbxml_tree_clb_ctx.error) != WBXML_OK)
            wbxml_tree_destroy(wbxml_tree_clb_ctx.tree);
        else
            *tree = wbxml_tree_clb_ctx.tree;
    }

    /* Clean-up */
    XML_ParserFree_drm(xml_parser);

    return ret;

#else /* HAVE_EXPAT */

#if defined( HAVE_LIBXML )

    /** @todo Use LibXML2 SAX interface ! */
    return WBXML_ERROR_NO_XMLPARSER;

#else /* HAVE_LIBXML */
    
    /** @note You can add here another XML Parser support */
    return WBXML_ERROR_NO_XMLPARSER;

#endif /* HAVE_LIBXML */

#endif /* HAVE_EXPAT */
}


WBXMLError wbxml_tree_to_xml(WBXMLTree *tree,
                                            WB_UTINY **xml,
                                            WB_ULONG  *xml_len,
                                            WBXMLGenXMLParams *params)
{
    WBXMLEncoder *wbxml_encoder = NULL;
    WBXMLError ret = WBXML_OK;

    /* Create WBXML Encoder */
    if ((wbxml_encoder = wbxml_encoder_create()) == NULL) {
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;
    }

    /* Set the WBXML Tree to encode */
    wbxml_encoder_set_tree(wbxml_encoder, tree);

    /* Set encoder parameters */
    if (params == NULL) {
        /* Default Values */

        /* Set XML Generation Type */
        wbxml_encoder_set_xml_gen_type(wbxml_encoder, WBXML_GEN_XML_INDENT);

        /* Set Indent */
        wbxml_encoder_set_indent(wbxml_encoder, 0);

        /* Skip Ignorable Whitespaces */
        wbxml_encoder_set_ignore_empty_text(wbxml_encoder, TRUE);
        wbxml_encoder_set_remove_text_blanks(wbxml_encoder, TRUE);
    }
    else {
        /* Set XML Generation Type */
        wbxml_encoder_set_xml_gen_type(wbxml_encoder, params->gen_type);

        /* Set Indent */
        if (params->gen_type == WBXML_GEN_XML_INDENT)
            wbxml_encoder_set_indent(wbxml_encoder, params->indent);

        /* Ignorable Whitespaces */
        if (params->keep_ignorable_ws) {
            wbxml_encoder_set_ignore_empty_text(wbxml_encoder, FALSE);
            wbxml_encoder_set_remove_text_blanks(wbxml_encoder, FALSE);
        }
        else {
            wbxml_encoder_set_ignore_empty_text(wbxml_encoder, TRUE);
            wbxml_encoder_set_remove_text_blanks(wbxml_encoder, TRUE);
        }

        /** @todo Add parameter to call : wbxml_encoder_set_output_charset() */
    }

    /* Encode WBXML Tree to XML */
    ret = wbxml_encoder_encode_tree_to_xml(wbxml_encoder, xml, xml_len);

    /* Clean-up */
    wbxml_encoder_destroy(wbxml_encoder);

    return ret;
}


#if defined( HAVE_LIBXML )

WBXMLError wbxml_tree_from_libxml_doc(xmlDocPtr libxml_doc,
                                                     WBXMLTree **tree)
{
    /** @todo wbxml_tree_from_libxml_doc() */
    return WBXML_ERROR_NOT_IMPLEMENTED;
}


WBXMLError wbxml_tree_to_libxml_doc(WBXMLTree *tree,
                                                   xmlDocPtr *libxml_doc)
{
    /** @todo wbxml_tree_to_libxml_doc() */
    return WBXML_ERROR_NOT_IMPLEMENTED;
}

#endif /* HAVE_LIBXML */


WBXMLTreeNode * wbxml_tree_node_create(WBXMLTreeNodeType type)
{
    WBXMLTreeNode *result = NULL;
    
    if ((result = (WBXMLTreeNode *) wbxml_malloc(sizeof(WBXMLTreeNode))) == NULL)
        return NULL;

    result->type = type;
    result->name = NULL;
    result->attrs = NULL;
    result->content = NULL;
    result->tree = NULL;

    result->parent = NULL;
    result->children = NULL;
    result->next = NULL;
    result->prev = NULL;

    return result;
}


void wbxml_tree_node_destroy(WBXMLTreeNode *node)
{
    if (node == NULL)
        return;

    wbxml_tag_destroy(node->name);
    wbxml_list_destroy(node->attrs, wbxml_attribute_destroy_item);
    wbxml_buffer_destroy(node->content);
    wbxml_tree_destroy(node->tree);

    wbxml_free(node);
}


void wbxml_tree_node_destroy_item(void *node)
{
    wbxml_tree_node_destroy((WBXMLTreeNode *)node);
}


void wbxml_tree_node_destroy_all(WBXMLTreeNode *node)
{
    WBXMLTreeNode *parent_node   = NULL;
    WBXMLTreeNode *current_node  = NULL;
    WBXMLTreeNode *previous_node = NULL;
    WBXMLTreeNode *tmp_node      = NULL;
    WB_BOOL        end_of_walk   = FALSE;

    if (node == NULL)
        return;

    /* Let's go through the sub-tree (iteratively) to free all the nodes */
    current_node = node;
    parent_node  = node->parent;

    while (!end_of_walk)
    {
        if (current_node == NULL) {
            /* Leaf reached */
            if (previous_node == NULL) {
                end_of_walk = TRUE;
                break;
            }
            else {
                if (previous_node->parent == parent_node) {
                    /* End of parsing, we have parsed the last child of node */
                    end_of_walk = TRUE;
                    break;
                }
                else {
                    /* Let's parse next child of parent node */                    
                    current_node = previous_node->next;
                    tmp_node = previous_node->parent;

                    /* Destroy this node (leaf) */
                    wbxml_tree_node_destroy(previous_node);

                    previous_node = tmp_node;
                }
            }
        }
        else {
            /* Go deeper in sub-tree */
            previous_node = current_node;
            current_node = current_node->children;        
        }
    }

    wbxml_tree_node_destroy(node);
}


WBXMLTreeNode * wbxml_tree_node_create_xml_elt(const WBXMLLangEntry *lang_table,
                                                              const WB_UTINY *name)
{
    const WBXMLTagEntry *tag_entry = NULL;
    WBXMLTreeNode *node = NULL;
    WBXMLTag *tag = NULL;
    
    /* Search for XML Tag Name in Table */
    if ((tag_entry = wbxml_tables_get_tag_from_xml(lang_table, name)) != NULL) {
        /* Found : token tag */
        tag = wbxml_tag_create_token(tag_entry);
    }
    else {
        /* Not found : literal tag */
        tag = wbxml_tag_create_literal((WB_UTINY *)name);
    }

    if (tag == NULL)
        return NULL;

    /* Create a new Node */
    if ((node = wbxml_tree_node_create(WBXML_TREE_ELEMENT_NODE)) == NULL) {
        wbxml_tag_destroy(tag);
        return NULL;
    }
    
    /* Set Node Tag */
    node->name = tag;
    
    return node;
}


WBXMLTreeNode * wbxml_tree_node_create_xml_elt_with_text(const WBXMLLangEntry *lang_table,
                                                                        const WB_UTINY *name,
                                                                        const WB_UTINY *text,
                                                                        WB_ULONG len)
{
    WBXMLTreeNode *node = NULL;
    WBXMLTreeNode *text_node = NULL;
    
    /* Create element node */
    if ((node = wbxml_tree_node_create_xml_elt(lang_table, name)) == NULL)
        return NULL;
    
    /* Create text node */
    if ((text_node = wbxml_tree_node_create_text(text, len)) == NULL) {
        wbxml_tree_node_destroy(node);
        return NULL;
    }
    
    /* Add text node to element node */
    if (!wbxml_tree_node_add_child(node, text_node)) {
        wbxml_tree_node_destroy(node);
        wbxml_tree_node_destroy(text_node);
        return NULL;
    }
    
    return node;
}


WBXMLTreeNode * wbxml_tree_node_create_text(const WB_UTINY *text,
                                                           WB_ULONG len)
{
    WBXMLTreeNode *node = NULL;

    /* Create a new Node */
    if ((node = wbxml_tree_node_create(WBXML_TREE_TEXT_NODE)) == NULL) {
        return NULL;
    }

    /* Set Content */
    if ((node->content = wbxml_buffer_create(text, len, len)) == NULL) {
        wbxml_tree_node_destroy(node);
        return NULL;
    }

    return node;
}


WBXMLTreeNode * wbxml_tree_node_create_cdata(const WB_UTINY *text,
                                                            WB_ULONG len)
{
    WBXMLTreeNode *node = NULL;
    WBXMLTreeNode *text_node = NULL;

    /* Create a new node */
    if ((node = wbxml_tree_node_create(WBXML_TREE_CDATA_NODE)) == NULL) {
        return NULL;
    }

    /* Create a text node */
    if ((text_node = wbxml_tree_node_create_text(text, len)) == NULL) {
        wbxml_tree_node_destroy(node);
        return NULL;
    }
    
    /* Add text node to cdata */
    if (!wbxml_tree_node_add_child(node, text_node)) {
        wbxml_tree_node_destroy_all(node);
        node = NULL;
    }

    return node;
}


WBXMLTreeNode * wbxml_tree_node_create_tree(WBXMLTreeNode *root,
                                                           WBXMLLanguage lang,
                                                           WBXMLCharsetMIBEnum orig_charset)
{
    WBXMLTreeNode* result = NULL;
    WBXMLTree*     tree   = NULL;
    
    if ((root == NULL) || (lang == WBXML_LANG_UNKNOWN))
        return NULL;
    
    /* Create Tree */
    if ((tree = wbxml_tree_create(lang, orig_charset)) == NULL)
        return NULL;
    
    /* Fill Tree */
    tree->root = root;
    
    /* Create Tree Node */
    if ((result = wbxml_tree_node_create(WBXML_TREE_TREE_NODE)) == NULL) {
        wbxml_tree_destroy(tree);
        return NULL;
    }
    
    /* Fill Tree Node */
    result->tree = tree;
    
    return result;
}


WB_BOOL wbxml_tree_node_add_child(WBXMLTreeNode *parent,
                                                 WBXMLTreeNode *node)
{
    WBXMLTreeNode *tmp = NULL;

    if ((parent == NULL) || (node == NULL))
        return FALSE;

    /* Set parent to new node */
    node->parent = parent;    

    /* Search for previous sibbling element */
    if (parent->children != NULL) {
        /* Add this Node to end of Sibbling Node list of Parent */
        tmp = parent->children;

        while (tmp->next != NULL)
            tmp = tmp->next;
        
        node->prev = tmp;
        tmp->next = node;
    }
    else {
        /* No previous sibbling element */
        parent->children = node;
    }

    return TRUE;
}


WBXMLError wbxml_tree_node_add_attr(WBXMLTreeNode *node,
                                                   WBXMLAttribute *attr)
{
    WBXMLAttribute *new_attr = NULL;

    if ((node == NULL) || (attr == NULL)) {
        return WBXML_ERROR_BAD_PARAMETER;
    }

    /* Create list if needed */
    if (node->attrs == NULL) {
        if ((node->attrs = wbxml_list_create()) == NULL) {
            return WBXML_ERROR_NOT_ENOUGH_MEMORY;
        }
    }

    /* Duplicate Attribute */
    if ((new_attr = wbxml_attribute_duplicate(attr)) == NULL)
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;
    
    /* Add attribute to list */
    if (!wbxml_list_append(node->attrs, new_attr)) {
        wbxml_attribute_destroy(attr);
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;
    }

    return WBXML_OK;
}


WBXMLError wbxml_tree_node_add_attrs(WBXMLTreeNode *node,
                                                    WBXMLAttribute **attrs)
{
    WB_ULONG i = 0;

    if ((node == NULL) || (attrs == NULL)) {
        return WBXML_ERROR_BAD_PARAMETER;
    }

    while (attrs[i] != NULL) {
        /* Add attribute */
        if (wbxml_tree_node_add_attr(node, attrs[i]) != WBXML_OK)
            return WBXML_ERROR_NOT_ENOUGH_MEMORY;

        i++;
    }

    return WBXML_OK;
}


WBXMLError wbxml_tree_node_add_xml_attr(const WBXMLLangEntry *lang_table,
                                                       WBXMLTreeNode *node,
                                                       const WB_UTINY *name,
                                                       const WB_UTINY *value)
{
    WBXMLAttribute *attr = NULL;
    const WBXMLAttrEntry *attr_entry = NULL;

    /* Create list if needed */
    if (node->attrs == NULL) {
        if ((node->attrs = wbxml_list_create()) == NULL) {
            return WBXML_ERROR_NOT_ENOUGH_MEMORY;
        }
    }

    /* Create Attribute */
    if ((attr = wbxml_attribute_create()) == NULL)
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;

    /* Set Attribute Name */
    if ((attr_entry = wbxml_tables_get_attr_from_xml(lang_table, (WB_UTINY *)name, (WB_UTINY *)value, NULL)) != NULL)
        attr->name = wbxml_attribute_name_create_token(attr_entry);
    else
        attr->name = wbxml_attribute_name_create_literal((WB_UTINY *)name);

    if (attr->name == NULL) {
        wbxml_attribute_destroy(attr);
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;
    }

    /* Set Attribute Value */
    attr->value = wbxml_buffer_create_real(value, WBXML_STRLEN((const WB_TINY *)value), WBXML_STRLEN((const WB_TINY *)value));
    if (attr->value == NULL) {
        wbxml_attribute_destroy(attr);
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;
    }

    /* Add attribute to list */
    if (!wbxml_list_append(node->attrs, attr)) {
        wbxml_attribute_destroy(attr);
        return WBXML_ERROR_NOT_ENOUGH_MEMORY;
    }

    return WBXML_OK;
}


WBXMLError wbxml_tree_node_add_xml_attrs(const WBXMLLangEntry *lang_table,
                                                        WBXMLTreeNode *node,
                                                        const WB_UTINY **attrs)
{
    const WB_UTINY **p = attrs;

    if ((lang_table == NULL) || (node == NULL) || (attrs == NULL))
        return WBXML_ERROR_BAD_PARAMETER;

    while (p && *p) {
        /* Add attribute */
        if (wbxml_tree_node_add_xml_attr(lang_table, node, *p, *(p+1)) != WBXML_OK)
            return WBXML_ERROR_NOT_ENOUGH_MEMORY;

        p += 2;
    }

    return WBXML_OK;
}


WBXMLTreeNode * wbxml_tree_node_elt_get_from_name(WBXMLTreeNode *node, const char *name, WB_BOOL recurs)
{
    WBXMLTreeNode *current_node = NULL;
    WB_BOOL node_found = FALSE;

    if ((node == NULL) || (name == NULL))
        return NULL;

    /** @todo Handle 'recurs' TRUE */

    /* Let's go through the tree */
    current_node = node;

    while (current_node != NULL)
    {
        /* Is this the Node we searched ? */
        if ((current_node->type == WBXML_TREE_ELEMENT_NODE) && 
            (WBXML_STRCMP((const WB_TINY *)wbxml_tag_get_xml_name(current_node->name), (const WB_TINY *)name) == 0))
        {
            node_found = TRUE;
            break;
        }
        else {
            /* Go to next Sibbling Node */
            current_node = current_node->next;
        }
    }

    if (node_found)
        return current_node;

    return NULL;
}


WB_BOOL wbxml_tree_node_have_child_elt(WBXMLTreeNode *node)
{
    WBXMLTreeNode *current = NULL;

    if (node != NULL) {
        /* Get first child */
        current = node->children;

        while (current != NULL) {
            if (current->type == WBXML_TREE_ELEMENT_NODE) {
                /* Element Node found ! */
                return TRUE;
            }

            /* Check next child */
            current = current->next;
        }
    }

    return FALSE;
}


WBXMLList* wbxml_tree_node_get_all_children(WBXMLTreeNode *node)
{
    WBXMLList* result = NULL;
    
    if ( node == NULL )
        return NULL;
    
    node = node->children;
    
    while ( node != NULL ) {
        /* Create result list if not already done */
        if ( result == NULL )
            result = wbxml_list_create();
        
        /* Append node to result */
        wbxml_list_append(result, node);
        
        /* Go to next node */
        node = node->next;
    } 
    
    return result;
}


WBXMLTree * wbxml_tree_create(WBXMLLanguage lang,
                                             WBXMLCharsetMIBEnum orig_charset)
{
    WBXMLTree *result = NULL;
    
    if ((result = (WBXMLTree *) wbxml_malloc(sizeof(WBXMLTree))) == NULL)
        return NULL;

    result->lang = wbxml_tables_get_table(lang);
    result->root = NULL;
    result->orig_charset = orig_charset;

    return result;
}


void wbxml_tree_destroy(WBXMLTree *tree)
{
    if (tree != NULL) {
        /* Destroy root node and all its children */
        wbxml_tree_node_destroy_all(tree->root);

        /* Free tree */
        wbxml_free(tree);
    }
}


/** @todo Rewrite this function (use wbxml_tree_node_* functions) */
WB_BOOL wbxml_tree_add_node(WBXMLTree *tree, WBXMLTreeNode *parent, WBXMLTreeNode *node)
{
    WBXMLTreeNode *tmp = NULL;

    if ((tree == NULL) || (node == NULL))
        return FALSE;

    /* Set parent to new node */
    node->parent = parent;    

    /* Check if this is the Root Element */
    if (parent != NULL) {
        /* This is not the Root Element... search for previous sibbling element */
        if (parent->children != NULL) {
            /* Add this Node to end of Sibbling Node list of Parent */
            tmp = parent->children;

            while (tmp->next != NULL)
                tmp = tmp->next;
            
            node->prev = tmp;
            tmp->next = node;
        }
        else {
            /* No previous sibbling element */
            parent->children = node;
        }
    }
    else {
        /* We do NOT allow replacement of an existing Tree Node */
        if (tree->root != NULL)
            return FALSE;

        /* This is the Root Element */
        tree->root = node;
    }

    return TRUE;
}


/** @todo Rewrite this function (use wbxml_tree_node_* functions) */
WBXMLError wbxml_tree_extract_node(WBXMLTree *tree,
                                                  WBXMLTreeNode *node)
{
    if ((tree == NULL) || (node == NULL))
        return WBXML_ERROR_BAD_PARAMETER;

    /* Parent link */
    if (node->parent != NULL) {
        if (node->parent->children == node) {
            /* Update parent children */
		    node->parent->children = node->next;
        }

        /* No more parent */
        node->parent = NULL;
    }
    else {
        /* Root removed ! */
        tree->root = node->next;
    }

    /* Next link */
    if (node->next != NULL) {
        /* Link next node to previous node */
        node->next->prev = node->prev;
        node->next = NULL;
    }

    /* Previous link */
    if (node->prev != NULL) {
        /* Link previous node to next node */
        node->prev->next = node->next;
        node->prev = NULL;
    }

    return WBXML_OK;
}


/** @todo Rewrite this function (use wbxml_tree_node_* functions) */
WBXMLTreeNode * wbxml_tree_add_elt(WBXMLTree *tree,
                                                  WBXMLTreeNode *parent,
                                                  WBXMLTag *tag)
{
    WBXMLTreeNode *node = NULL;

    /* Create a new Node */
    if ((node = wbxml_tree_node_create(WBXML_TREE_ELEMENT_NODE)) == NULL) {
        return NULL;
    }

    /* Set Element */
    if ((node->name = wbxml_tag_duplicate(tag)) == NULL) {
        wbxml_tree_node_destroy(node);
        return NULL;
    }

    /* Add this Node to Tree  */
    if (!wbxml_tree_add_node(tree, parent, node)) {
        wbxml_tree_node_destroy(node);
        return NULL;
    }

    return node;
}


/** @todo Rewrite this function (use wbxml_tree_node_* functions) */
WBXMLTreeNode * wbxml_tree_add_elt_with_attrs(WBXMLTree *tree,
                                                             WBXMLTreeNode *parent,
                                                             WBXMLTag *tag,
                                                             WBXMLAttribute **attrs)
{
    WBXMLTreeNode *node = NULL;

    /* Add element */
    if ((node = wbxml_tree_add_elt(tree, parent, tag)) == NULL) {
        return NULL;
    }

    /* Add attributes to element */
    if ((attrs != NULL) && (*attrs != NULL)) {
        if (wbxml_tree_node_add_attrs(node, attrs) != WBXML_OK) {
            /* Remove node from Tree */
            wbxml_tree_extract_node(tree, node);
            wbxml_tree_node_destroy(node);
            return NULL;
        }
    }

    return node;
}


/** @todo Rewrite this function (use wbxml_tree_node_* functions) */
WBXMLTreeNode * wbxml_tree_add_xml_elt(WBXMLTree *tree,
                                                      WBXMLTreeNode *parent,
                                                      WB_UTINY *name)
{
    const WBXMLTagEntry *tag_entry = NULL;
    WBXMLTreeNode *node = NULL;
    WBXMLTag *tag = NULL;
    
    /* Search for XML Tag Name in Table */
    if ((tag_entry = wbxml_tables_get_tag_from_xml(tree->lang, (const WB_UTINY *) name)) != NULL) {
        /* Found : token tag */
        tag = wbxml_tag_create_token(tag_entry);
    }
    else {
        /* Not found : literal tag */
        tag = wbxml_tag_create_literal(name);
    }

    if (tag == NULL)
        return NULL;

    /* Create a new Node */
    if ((node = wbxml_tree_node_create(WBXML_TREE_ELEMENT_NODE)) == NULL) {
        wbxml_tag_destroy(tag);
        return NULL;
    }
    
    /* Set Node Tag */
    node->name = tag;

    /* Add this Node to Tree  */
    if (!wbxml_tree_add_node(tree, parent, node)) {
        wbxml_tree_node_destroy(node);
        return NULL;
    }
    
    return node;
}


/** @todo Rewrite this function (use wbxml_tree_node_* functions) */
WBXMLTreeNode * wbxml_tree_add_xml_elt_with_attrs(WBXMLTree *tree,
                                                                 WBXMLTreeNode *parent,
                                                                 WB_UTINY *name,
                                                                 const WB_UTINY **attrs)
{
    WBXMLTreeNode *node = NULL;

    /* Add element */
    if ((node = wbxml_tree_add_xml_elt(tree, parent, name)) == NULL) {
        return NULL;
    }

    /* Add attributes to element */
    if ((attrs != NULL) && (*attrs != NULL)) {
        if (wbxml_tree_node_add_xml_attrs(tree->lang, node, attrs) != WBXML_OK) {
            /* Remove node from Tree */
            wbxml_tree_extract_node(tree, node);
            wbxml_tree_node_destroy(node);
            return NULL;
        }
    }

    return node;
}


/** @todo Rewrite this function (use wbxml_tree_node_* functions) */
WBXMLTreeNode * wbxml_tree_add_text(WBXMLTree *tree,
                                                   WBXMLTreeNode *parent,
                                                   const WB_UTINY *text,
                                                   WB_ULONG len)
{
    WBXMLTreeNode *node = NULL;

    /* Create a new Node */
    if ((node = wbxml_tree_node_create(WBXML_TREE_TEXT_NODE)) == NULL) {
        return NULL;
    }

    /* Set Content */
    if ((node->content = wbxml_buffer_create(text, len, len)) == NULL) {
        wbxml_tree_node_destroy(node);
        return NULL;
    }

    /* Add this Node to Tree  */
    if (!wbxml_tree_add_node(tree, parent, node)) {
        wbxml_tree_node_destroy(node);
        return NULL;
    }

    return node;
}


/** @todo Rewrite this function (use wbxml_tree_node_* functions) */
WBXMLTreeNode * wbxml_tree_add_cdata(WBXMLTree *tree,
                                                    WBXMLTreeNode *parent)
{
    WBXMLTreeNode *node = NULL;

    /* Create a new Node */
    if ((node = wbxml_tree_node_create(WBXML_TREE_CDATA_NODE)) == NULL) {
        return NULL;
    }

    /* Add this Node to Tree  */
    if (!wbxml_tree_add_node(tree, parent, node)) {
        wbxml_tree_node_destroy(node);
        return NULL;
    }

    return node;
}


/** @todo wbxml_tree_add_cdata_with_text() */


/** @todo Rewrite this function (use wbxml_tree_node_* functions) */
WBXMLTreeNode * wbxml_tree_add_tree(WBXMLTree *tree,
                                                   WBXMLTreeNode *parent,
                                                   WBXMLTree *new_tree)
{
    WBXMLTreeNode *node = NULL;

    /* Create a new Node */
    if ((node = wbxml_tree_node_create(WBXML_TREE_TREE_NODE)) == NULL) {        
        return NULL;
    }

    /* Add this Node to Tree  */
    if (!wbxml_tree_add_node(tree, parent, node)) {
        wbxml_tree_node_destroy(node);
        return NULL;
    }

    /* Set Tree */
    node->tree = new_tree;

    return node;
}


/** @todo Rewrite this function (use wbxml_tree_node_* functions) */
WBXMLTreeNode * wbxml_tree_add_xml_elt_with_attrs_and_text(WBXMLTree *tree,
                                                                          WBXMLTreeNode *parent,
                                                                          WB_UTINY *name,
                                                                          const WB_UTINY **attrs,
                                                                          const WB_UTINY *text,
                                                                          WB_ULONG len)
{
    WBXMLTreeNode *new_node = NULL;
    
    /* Add XML node */
    if ((new_node = wbxml_tree_add_xml_elt_with_attrs(tree, parent, name, attrs)) == NULL)
        return NULL;
    
    /* Add text node */
    if ((text != NULL) && (len > 0)) {
        if (wbxml_tree_add_text(tree, new_node, text, len) == NULL) {
            wbxml_tree_node_destroy(new_node);
            return NULL;
        }
    }
    
    return new_node;
}
