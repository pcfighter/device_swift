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
 * @file wbxml_tree_clb_wbxml.c
 * @ingroup wbxml_tree
 *
 * @author Aymerick Jehanne <libwbxml@aymerick.com>
 * @date 03/02/22
 *
 * @brief WBXML Tree Callbacks for WBXML Parser
 */

#include "wbxml.h"


/***************************************************
 *  Public Functions
 */

void wbxml_tree_clb_wbxml_start_document(void *ctx, WBXMLCharsetMIBEnum charset, const WBXMLLangEntry *lang)
{
    WBXMLTreeClbCtx *tree_ctx = (WBXMLTreeClbCtx *) ctx;

    if (tree_ctx->error != WBXML_OK)
        return;

    tree_ctx->tree->lang = lang;
    tree_ctx->tree->orig_charset = charset;
}


void wbxml_tree_clb_wbxml_end_document(void *ctx)
{
    WBXMLTreeClbCtx *tree_ctx = (WBXMLTreeClbCtx *) ctx;

    if (tree_ctx->error != WBXML_OK)
        return;
}


void wbxml_tree_clb_wbxml_start_element(void *ctx, WBXMLTag *element, WBXMLAttribute **attrs, WB_BOOL empty)
{
    WBXMLTreeClbCtx *tree_ctx = (WBXMLTreeClbCtx *) ctx;

    if (tree_ctx->error != WBXML_OK)
        return;

    /* Add a new Node to tree */
    tree_ctx->current = wbxml_tree_add_elt_with_attrs(tree_ctx->tree,
                                                      tree_ctx->current,
                                                      element,
                                                      attrs);
    if (tree_ctx->current == NULL) {
        tree_ctx->error = WBXML_ERROR_INTERNAL;
    }
}


void wbxml_tree_clb_wbxml_end_element(void *ctx, WBXMLTag *element, WB_BOOL empty)
{
    WBXMLTreeClbCtx *tree_ctx = (WBXMLTreeClbCtx *) ctx;

    if (tree_ctx->error != WBXML_OK)
        return;

    if (tree_ctx->current == NULL) {
        tree_ctx->error = WBXML_ERROR_INTERNAL;
        return;
    }

    if (tree_ctx->current->parent == NULL) {
        /* This must be the Root Element */
        if (tree_ctx->current != tree_ctx->tree->root) {
            tree_ctx->error = WBXML_ERROR_INTERNAL;
        }
    }
    else {

        /* Go back one step upper in the tree */
        tree_ctx->current = tree_ctx->current->parent;
    }
}


void wbxml_tree_clb_wbxml_characters(void *ctx, WB_UTINY *ch, WB_ULONG start, WB_ULONG length)
{
    WBXMLTreeClbCtx *tree_ctx = (WBXMLTreeClbCtx *) ctx;

    if (tree_ctx->error != WBXML_OK)
        return;


    /* Add Text Node */
    if (wbxml_tree_add_text(tree_ctx->tree,
                            tree_ctx->current,
                            (const WB_UTINY*) ch + start,
                            length) == NULL)
    {
        tree_ctx->error = WBXML_ERROR_INTERNAL;
    }
}


void wbxml_tree_clb_wbxml_pi(void *ctx, const WB_UTINY *target, WB_UTINY *data)
{
    /** @todo wbxml_tree_clb_pi() */
}
