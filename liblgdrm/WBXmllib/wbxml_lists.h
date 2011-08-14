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
 * @file wbxml_lists.h 
 * @ingroup wbxml_lists
 *
 * @author Aymerick Jehanne <libwbxml@aymerick.com>
 * @date 02/12/06
 *
 * @brief Generic Lists Functions
 */

#ifndef WBXML_LISTS_H
#define WBXML_LISTS_H


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/**
 * @brief WBXML Generic List
 */
typedef struct WBXMLList_s WBXMLList;


/** @addtogroup wbxml_lists  
 *  @{ 
 */

/**
 * @brief A List Element Cleaner Function prototype
 */
typedef void WBXMLListEltCleaner(void *item);

/**
 * @brief Create a List
 * @return The newly created List, or NULL if not enough memory
 * @warning Do NOT use this function directly, use wbxml_list_create() macro instead
 */
WBXMLList * wbxml_list_create_real(void);
#define wbxml_list_create() wbxml_mem_cleam(wbxml_list_create_real())

/**
 * @brief Destroy a List
 * @param list The List to destroy
 * @param destructor The function to destroy an element from list (if NULL, items are not destroyed from list)
 */
void wbxml_list_destroy(WBXMLList *list, WBXMLListEltCleaner *destructor);

/**
 * @brief Get list length
 * @param list The List
 * @return The List length
 */
WB_ULONG wbxml_list_len(WBXMLList *list);

/**
 * @brief Append an element at end of list
 * @param list The List
 * @param elt The element to append
 * @return TRUE if element appended, FALSE if not enough memory
 */
WB_BOOL wbxml_list_append(WBXMLList *list, void *elt);

/**
 * @brief Append an element to a list
 * @param list The List
 * @param elt The element to insert
 * @param pos The index where to insert this element
 * @return TRUE if element appended, FALSE if not enough memory
 */
WB_BOOL wbxml_list_insert(WBXMLList *list, void *elt, WB_ULONG pos);

/**
 * @brief Get an Element from list
 * @param list The List
 * @param index Index of element to get (index in start starts at '0')
 * @return The element, or NULL if not found
 */
void * wbxml_list_get(WBXMLList *list, WB_ULONG index);

/**
 * @brief Extract first element of a List
 * @param list The List
 * @return The element extracted, or NULL if not found
 * @note The element is removed from this list
 */
void * wbxml_list_extract_first(WBXMLList *list);

/** @} */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* WBXML_LISTS_H */
