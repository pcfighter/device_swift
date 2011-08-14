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
 * @file wbxml_buffers.c
 * @ingroup wbxml_buffers
 *
 * @author Aymerick Jehanne <libwbxml@aymerick.com>
 * @date 02/03/12
 *
 * @brief Generic Buffers Functions
 *
 * @note Original idea: Kannel Project (http://www.kannel.org/)
 */

#include <limits.h>
#include <ctype.h>

#include "wbxml.h"


/* Memory management define */
#define WBXML_BUFFER_SPLIT_BLOCK 20

/**
 * The Generic Buffer type
 */
struct WBXMLBuffer_s
{
    WB_UTINY *data;             /**< The data */
    WB_ULONG  len;              /**< Length of data in buffer */
    WB_ULONG  malloced;         /**< Length of buffer */
    WB_ULONG  malloc_block;     /**< Malloc Block Size */
    WB_BOOL   is_static;        /**< Is it a static buffer ?  */
};


static WB_BOOL grow_buff(WBXMLBuffer *buffer, WB_ULONG size);
static WB_BOOL insert_data(WBXMLBuffer *buffer, WB_ULONG pos, const WB_UTINY *data, WB_ULONG len);



/**********************************
 *    Public functions
 */


WBXMLBuffer * wbxml_buffer_create_real(const WB_UTINY *data, WB_ULONG len, WB_ULONG malloc_block)
{
    WBXMLBuffer *buffer = NULL;

    buffer = (WBXMLBuffer *) wbxml_malloc(sizeof(WBXMLBuffer));
    if (buffer == NULL)
        return NULL;
        
    buffer->malloc_block = malloc_block;
    buffer->is_static    = FALSE;

    if ((len <= 0) || (data == NULL)) {        
        buffer->malloced = 0;
        buffer->len = 0;
        buffer->data = NULL;
    } 
    else {               
        if (len + 1 > malloc_block + 1)
            buffer->malloced = len + 1 + malloc_block;
        else
            buffer->malloced = malloc_block + 1;
        
        buffer->data = (WB_UTINY *) wbxml_malloc(buffer->malloced * sizeof(WB_UTINY));
        if (buffer->data == NULL) {
            wbxml_free(buffer);
            return NULL;
        }

        buffer->len = len;
        memcpy(buffer->data, data, len);
        buffer->data[len] = '\0';
    }

    return buffer;
}


WBXMLBuffer * wbxml_buffer_sta_create_real(const WB_UTINY *data, WB_ULONG len)
{
    WBXMLBuffer *buffer = NULL;
  
    buffer = (WBXMLBuffer *) wbxml_malloc(sizeof(WBXMLBuffer));
    if (buffer == NULL) {
        return NULL;
    }

    buffer->malloc_block = 0;
    buffer->is_static    = TRUE;
    buffer->data         = (WB_UTINY *) data;
    buffer->len          = len;

    return buffer;
}


void wbxml_buffer_destroy(WBXMLBuffer *buffer)
{
    if (buffer != NULL) {
        if (!buffer->is_static) {
            /* Free dynamic data */
            wbxml_free(buffer->data);
        }

        /* Free structure */
        wbxml_free(buffer);
    }
}


void wbxml_buffer_destroy_item(void *buff)
{
    wbxml_buffer_destroy((WBXMLBuffer *) buff);
}


WBXMLBuffer * wbxml_buffer_duplicate(WBXMLBuffer *buff)
{
    WBXMLBuffer *result = NULL;

    if (buff == NULL)
        return NULL;

    result = wbxml_buffer_create_real(wbxml_buffer_get_cstr(buff),
                                      wbxml_buffer_len(buff),  
                                      wbxml_buffer_len(buff));

    return result;
}


WB_ULONG wbxml_buffer_len(WBXMLBuffer *buffer)
{
    if (buffer == NULL)
        return 0;
        
    return buffer->len;
}


WB_BOOL wbxml_buffer_get_char(WBXMLBuffer *buffer, WB_ULONG pos, WB_UTINY *result)
{
    //if ((buffer == NULL) || (pos >= buffer->len) || (pos < 0)) WBT.20100128
    if ((buffer == NULL) || (pos >= buffer->len) || (((int)pos) < 0))
        return FALSE;
        
    *result = buffer->data[pos];
    return TRUE;
}


void wbxml_buffer_set_char(WBXMLBuffer *buffer, WB_ULONG pos, WB_UTINY ch)
{
    if ((buffer != NULL) && !buffer->is_static && (pos < buffer->len))
        buffer->data[pos] = ch;
}


WB_UTINY * wbxml_buffer_get_cstr(WBXMLBuffer *buffer)
{
    if ((buffer == NULL) || (buffer->len == 0))
        return WBXML_UTINY_NULL_STRING;
        
    return buffer->data;
}


WB_BOOL wbxml_buffer_insert(WBXMLBuffer *to, WBXMLBuffer *buffer, WB_ULONG pos)
{
    if ((to != NULL) && (buffer != NULL) && !to->is_static)
        return insert_data(to, pos, buffer->data, buffer->len);

    return FALSE;
}


WB_BOOL wbxml_buffer_insert_cstr(WBXMLBuffer *to, WB_UTINY *str, WB_ULONG pos)
{
    if ((to != NULL) && (str != NULL) && !to->is_static)
        return insert_data(to, pos, str, WBXML_STRLEN((const char *)str));

    return FALSE;
}


WB_BOOL wbxml_buffer_append(WBXMLBuffer *dest, WBXMLBuffer *buff)
{
    if ((dest == NULL) || dest->is_static)
        return FALSE;

    if (buff == NULL)
        return TRUE;

    return wbxml_buffer_append_data(dest, wbxml_buffer_get_cstr(buff), wbxml_buffer_len(buff));
}


WB_BOOL wbxml_buffer_append_data_real(WBXMLBuffer *buffer, const WB_UTINY *data, WB_ULONG len)
{
    if ((buffer == NULL) || buffer->is_static)
        return FALSE;

    if ((data == NULL) || (len == 0))
        return TRUE;

    return insert_data(buffer, buffer->len, data, len);
}

WB_BOOL wbxml_buffer_append_cstr_real(WBXMLBuffer *buffer, const WB_UTINY *data)
{
    if ((buffer == NULL) || buffer->is_static) {
        return FALSE;
    }

    if (data == NULL)
        return TRUE;

    return wbxml_buffer_append_data(buffer, data, WBXML_STRLEN((const char *)data));
}


WB_BOOL wbxml_buffer_append_char(WBXMLBuffer *buffer, WB_UTINY ch)
{
    WB_UTINY c = ch;

    if ((buffer == NULL) || buffer->is_static)
        return FALSE;

    return insert_data(buffer, buffer->len, &c, 1);
}


WB_BOOL wbxml_buffer_append_mb_uint_32(WBXMLBuffer *buffer, WB_ULONG value)
{
    /**
     * A uintvar is defined to be up to 32 bits large
     * so it will fit in 5 octets (to handle continuation bits) 
     */
    WB_UTINY octets[5];
    WB_LONG i = 0, start = 0;

    if ((buffer == NULL) || buffer->is_static)
        return FALSE;

    /**
     * Handle last byte separately; it has no continuation bit,
     * and must be encoded even if value is 0. 
     */
    octets[4] = (WB_UTINY) (value & 0x7f);
    value >>= 7;

    for (i = 3; value > 0 && i >= 0; i--) {
        octets[i] = (WB_UTINY) (0x80 | (value & 0x7f));
        value >>= 7;
    }
    start = i + 1;

    return wbxml_buffer_append_data(buffer, octets + start, 5 - start);
}


void wbxml_buffer_delete(WBXMLBuffer *buffer, WB_ULONG pos, WB_ULONG len)
{
    if ((buffer == NULL) || buffer->is_static)
        return;

    if (pos > buffer->len)
        pos = buffer->len;
        
    if (pos + len > buffer->len)
        len = buffer->len - pos;
        
    if (len > 0) {
        memmove(buffer->data + pos, buffer->data + pos + len,
                buffer->len - pos - len);
                
        buffer->len -= len;
        buffer->data[buffer->len] = '\0';
    }
}


void wbxml_buffer_shrink_blanks(WBXMLBuffer *buffer)
{
    WB_ULONG i = 0, j = 0, end = 0;
    WB_UTINY ch = 0;
    
    if ((buffer == NULL) || buffer->is_static)
        return;
        
    end = wbxml_buffer_len(buffer);

    for (i = 0; i < end; i++) 
    {
        if (wbxml_buffer_get_char(buffer, i, &ch) && isspace(ch)) 
        {
            /* Replace space by a whitespace */
            if (ch != ' ')
                wbxml_buffer_set_char(buffer, i, ' ');           

            /* Remove all following spaces */
            j = i = i + 1;
            while (wbxml_buffer_get_char(buffer, j, &ch) && isspace(ch))
                j++;

            if (j - i > 1)
                wbxml_buffer_delete(buffer, i, j - i);
        }
    }
}


void wbxml_buffer_strip_blanks(WBXMLBuffer *buffer)
{
    WB_ULONG start = 0, end = 0, len = 0;
    WB_UTINY ch = 0;

    if ((buffer == NULL) || buffer->is_static)
        return;

    /* Remove whitespaces at beginning of buffer... */
    while (wbxml_buffer_get_char(buffer, start, &ch) && 
           isspace(ch) && 
           start <= wbxml_buffer_len(buffer))
    {
        start ++;
    }

    if (start > 0)
        wbxml_buffer_delete(buffer, 0, start);

    /* ... and at the end */
    if ((len = wbxml_buffer_len(buffer)) > 0) {
        end = len = len - 1;
        while (wbxml_buffer_get_char(buffer, end, &ch) &&
            isspace(ch) && 
            //end >= 0) WBT.20100128
            ((int)end) >= 0)
        {
            end--;
        }
        wbxml_buffer_delete(buffer, end + 1, len - end);
    }
}


WB_LONG wbxml_buffer_compare(WBXMLBuffer *buff1, WBXMLBuffer *buff2)
{
    WB_LONG ret = 0, len = 0;

    if ((buff1 == NULL) || (buff2 == NULL)) {
        if ((buff1 == NULL) && (buff2 == NULL))
            return 0;

        if (buff1 == NULL)
            return -1;
        else
            return 1;
    }

    if (buff1->len < buff2->len)
        len = buff1->len;
    else
        len = buff2->len;

    if (len == 0) 
    {
        if (buff1->len == 0 && buff2->len > 0)
            return -1;
        if (buff1->len > 0 && buff2->len == 0)
            return 1;
        return 0;
    }

    if ((ret = memcmp(buff1->data, buff2->data, len)) == 0) 
    {
        if (buff1->len < buff2->len)
            ret = -1;
        else {
            if (buff1->len > buff2->len)
                ret = 1;
        }
    }

    return ret;
}


WB_LONG wbxml_buffer_compare_cstr(WBXMLBuffer *buff, const WB_TINY *str)
{
    WB_LONG ret = 0, len = 0;

    if ((buff == NULL) || (str == NULL)) {
        if ((buff == NULL) && (str == NULL))
            return 0;

        if (buff == NULL)
            return -1;
        else
            return 1;
    }

    if (buff->len < WBXML_STRLEN(str))
        len = buff->len;
    else
        len = WBXML_STRLEN(str);

    if (len == 0) 
    {
        if (buff->len == 0 && WBXML_STRLEN(str) > 0)
            return -1;
        if (buff->len > 0 && WBXML_STRLEN(str) == 0)
            return 1;
        return 0;
    }

    if ((ret = memcmp(buff->data, str, len)) == 0) 
    {
        if (buff->len < WBXML_STRLEN(str))
            ret = -1;
        else {
            if (buff->len > WBXML_STRLEN(str))
                ret = 1;
        }
    }

    return ret;
}


WBXMLList * wbxml_buffer_split_words_real(WBXMLBuffer *buff)
{
    WB_UTINY *p = NULL;
    WBXMLList *list = NULL;
    WBXMLBuffer *word = NULL;
    WB_ULONG i = 0, start = 0, end = 0;

    if ((list = wbxml_list_create()) == NULL)
        return NULL;

    p = buff->data;
    i = 0;
    while (TRUE)
    {
        while (i < buff->len && isspace(*p)) {
            ++p;
            ++i;
        }
        start = i;

        while (i < buff->len && !isspace(*p)) {
            ++p;
            ++i;
        }
        end = i;

        if (start == end)
            break;

        if((word = wbxml_buffer_create(buff->data + start, end - start, WBXML_BUFFER_SPLIT_BLOCK)) == NULL) {
            wbxml_list_destroy(list, wbxml_buffer_destroy_item);
            return NULL;
        }

        wbxml_list_append(list, word);
    }

    return list;
}


WB_BOOL wbxml_buffer_search_char(WBXMLBuffer *to, WB_UTINY ch, WB_ULONG pos, WB_ULONG *result)
{
    WB_UTINY *p = NULL;

    if (to == NULL)
        return FALSE;

    if (pos >= to->len)
        return FALSE;

    if ((p = (WB_UTINY *) memchr(to->data + pos, ch, to->len - pos)) == NULL)
        return FALSE;

    if (result != NULL)
        *result = p - to->data;

    return TRUE;
}


WB_BOOL wbxml_buffer_search(WBXMLBuffer *to, WBXMLBuffer *search, WB_ULONG pos, WB_ULONG *result)
{
    WB_UTINY first = 0;

    if ((to == NULL) || (search == NULL))
        return FALSE;

    if (result != NULL)
        *result = 0;

    /* Always "find" an empty string */
    if (search->len == 0)
        return TRUE;

    /* Check if 'search' is greater than 'to' */
    if (search->len > to->len)
        return FALSE;

    /* We are searching for one char */
    if (search->len == 1)
        return wbxml_buffer_search_char(to, search->data[0], pos, result);

    /* For each occurrence of search's first character in to, then check if the rest of needle follows.
     * Stop if there are no more occurrences, or if the rest of 'search' can't possibly fit in 'to'. */
    first = search->data[0];
    while ((wbxml_buffer_search_char(to, first, pos, &pos)) && 
           (to->len - pos >= search->len)) 
    {
        if (memcmp(to->data + pos, search->data, search->len) == 0) {
            if (result != NULL)
                *result = pos;
            return TRUE;
        }
        pos++;
    }

    return FALSE;    
}


WB_BOOL wbxml_buffer_search_cstr(WBXMLBuffer *to, WB_UTINY *search, WB_ULONG pos, WB_ULONG *result)
{
    WB_UTINY first = 0;

    if ((to == NULL) || (search == NULL))
        return FALSE;

    if (result != NULL)
        *result = 0;

    /* Always "find" an empty string */
    if (WBXML_STRLEN((const char *)search) == 0)
        return TRUE;

    /* Check if 'search' is greater than 'to' */
    if (WBXML_STRLEN((const char *)search) > to->len)
        return FALSE;

    /* We are searching for one char */
    if (WBXML_STRLEN((const char *)search) == 1)
        return wbxml_buffer_search_char(to, search[0], pos, result);

    /* For each occurrence of search's first character in to, then check if the rest of needle follows.
     * Stop if there are no more occurrences, or if the rest of 'search' can't possibly fit in 'to'. */
    first = search[0];
    while ((wbxml_buffer_search_char(to, first, pos, &pos)) && 
           (to->len - pos >= WBXML_STRLEN((const char *)search))) 
    {
        if (memcmp(to->data + pos, search, WBXML_STRLEN((const char *)search)) == 0) {
            if (result != NULL)
                *result = pos;
            return TRUE;
        }
        pos++;
    }

    return FALSE;
}


WB_BOOL wbxml_buffer_contains_only_whitespaces(WBXMLBuffer *buffer)
{
    WB_ULONG i = 0;

    if (buffer == NULL)
        return FALSE;

    for (i=0; i<buffer->len; i++) {
        if (!isspace(*(buffer->data + i)))
            return FALSE;
    }

    return TRUE;
}


void wbxml_buffer_hex_to_binary(WBXMLBuffer *buffer)
{
    WB_UTINY *p = NULL;
    WB_ULONG i = 0, len = 0;

    if ((buffer == NULL) || buffer->is_static)
        return;

    p = buffer->data;
    len = wbxml_buffer_len(buffer);

    /* Convert ascii data to binary values */
    for (i = 0; i < len; i++, p++) {
        if (*p >= '0' && *p <= '9')
            *p -= '0';
        else if (*p >= 'a' && *p <= 'f')
            *p = (WB_UTINY) (*p - 'a' + 10);
        else if (*p >= 'A' && *p <= 'F')
            *p = (WB_UTINY) (*p - 'A' + 10);
        else {
            /* Bad Bad ! There should be only digits in the buffer ! */
            *p = 0;
        }
    }

    /* De-hexing will compress data by factor of 2 */
    len = buffer->len / 2;

    for (i = 0; i < len; i++)
        buffer->data[i] = (WB_UTINY) (buffer->data[i * 2] * 16 | buffer->data[i * 2 + 1]);

    buffer->len = len;
    buffer->data[len] = '\0';
}


WB_BOOL wbxml_buffer_binary_to_hex(WBXMLBuffer *buffer, WB_BOOL uppercase)
{
    WB_UTINY *hexits = NULL;
    WB_LONG i = 0;

    if ((buffer == NULL) || buffer->is_static)
        return FALSE;

    if (wbxml_buffer_len(buffer) == 0)
        return TRUE;

    hexits = (WB_UTINY *)(uppercase ? "0123456789ABCDEF" : "0123456789abcdef");

    /* Grows the Buffer size by 2 */
    grow_buff(buffer, buffer->len * 2);

    /* In-place modification must be done back-to-front to avoid
     * overwriting the data while we read it.  Even the order of
     * the two assignments is important, to get i == 0 right. 
     */
    for (i = buffer->len - 1; i >= 0; i--) {
        buffer->data[i * 2 + 1] = hexits[buffer->data[i] % 16];
        buffer->data[i * 2] = hexits[(buffer->data[i] / 16) & 0xf];
    }

    buffer->len = buffer->len * 2;
    buffer->data[buffer->len] = '\0';

    return TRUE;
}


void wbxml_buffer_remove_trailing_zeros(WBXMLBuffer **buffer)
{
    WB_UTINY ch = 0;

    if ((buffer == NULL) || (*buffer == NULL))
        return;

    while ((*buffer)->len > 0) {
        if (wbxml_buffer_get_char(*buffer, wbxml_buffer_len(*buffer) - 1, &ch) && (ch == '\0'))
            wbxml_buffer_delete(*buffer, wbxml_buffer_len(*buffer) - 1, 1);
        else
            return;
    }
}


/**********************************
 *    Private functions
 */

/**
 * @brief Add space for at least 'size' octets
 * @param buffer The buffer
 * @param size The size to add
 * @return TRUE is space successfully reserved, FALSE is size was negative, buffer was NULL or if not enough memory
 */
static WB_BOOL grow_buff(WBXMLBuffer *buffer, WB_ULONG size)
{
    //if ((buffer == NULL) || buffer->is_static || (size < 0)) WBT.20100128
    if ((buffer == NULL) || buffer->is_static || (((int)size) < 0))
        return FALSE;
        
    /* Make room for the invisible terminating NUL */
    size++; 

    if ((buffer->len + size) > buffer->malloced) {
        if ((buffer->malloced + buffer->malloc_block) < (buffer->len + size))
            buffer->malloced = buffer->len + size + buffer->malloc_block;
        else
            buffer->malloced = buffer->malloced + buffer->malloc_block;
            
        buffer->data = wbxml_realloc(buffer->data, buffer->malloced);
        if (buffer->data == NULL)
            return FALSE;
    }

    return TRUE;
}


/**
 * @brief Insert data into a Generic Buffer
 * @param buffer The Generic Buffer
 * @param pos Position in Generic Buffer where to insert data
 * @param data Data to insert
 * @param len Data length
 * @return TRUE is data inserted, FALSE if not
 */
static WB_BOOL insert_data(WBXMLBuffer *buffer, WB_ULONG pos, const WB_UTINY *data, WB_ULONG len)
{
    if ((buffer == NULL) || buffer->is_static || (len == 0) || (pos > buffer->len))
        return FALSE;

    if (!grow_buff(buffer, len))
        return FALSE;

    if (buffer->len > pos) {    
        /* Only if neccessary */
        memmove(buffer->data + pos + len, buffer->data + pos, buffer->len - pos);
    }

    memcpy(buffer->data + pos, data, len);
    buffer->len += len;
    buffer->data[buffer->len] = '\0';

    return TRUE;
}
