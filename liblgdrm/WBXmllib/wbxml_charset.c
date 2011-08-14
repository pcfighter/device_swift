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
 * @file wbxml_charset.c
 * @ingroup wbxml_charset
 *
 * @author Aymerick Jehanne <libwbxml@aymerick.com>
 * @date 04/03/24
 *
 * @brief Charset Functions
 */

#include "wbxml.h"


/* Structures */

/** WBXML Charset */
typedef struct WBXMLCharsetEntry_s {
    const WB_TINY       *name;     /**< Charset Name */
    WBXMLCharsetMIBEnum  mib_enum; /**< Charset MIBEnum Value */
} WBXMLCharsetEntry;


/* Globals */

/**
 * @brief Charset table
 * @note  From http://www.iana.org/assignments/character-sets
 */
static const WBXMLCharsetEntry wbxml_charset_entries[] =
{
    { "US-ASCII",        WBXML_CHARSET_US_ASCII        },
    { "ISO-8859-1",      WBXML_CHARSET_ISO_8859_1      },
    { "ISO-8859-2",      WBXML_CHARSET_ISO_8859_2      },
    { "ISO-8859-3",      WBXML_CHARSET_ISO_8859_3      },
    { "ISO-8859-4",      WBXML_CHARSET_ISO_8859_4      },
    { "ISO-8859-5",      WBXML_CHARSET_ISO_8859_5      },
    { "ISO-8859-6",      WBXML_CHARSET_ISO_8859_6      },
    { "ISO-8859-7",      WBXML_CHARSET_ISO_8859_7      },
    { "ISO-8859-8",      WBXML_CHARSET_ISO_8859_8      },
    { "ISO-8859-9",      WBXML_CHARSET_ISO_8859_9      },
    { "Shift_JIS",       WBXML_CHARSET_SHIFT_JIS       },
    { "UTF-8",           WBXML_CHARSET_UTF_8           },
    { "ISO-10646-UCS-2", WBXML_CHARSET_ISO_10646_UCS_2 },
    { "UTF-16",          WBXML_CHARSET_UTF_16          },
    { "Big5",            WBXML_CHARSET_BIG5            }
};


/* Private Functions Prototypes */
static WB_BOOL binary_search(const WB_TINY *in_buf,
                             WB_ULONG       in_buf_len,
                             const WB_TINY *in_seq,
                             WB_ULONG       in_seq_len,
                             WB_ULONG      *out_pos);


/***************************************************
 *    Public Functions
 */

WB_BOOL wbxml_charset_get_mib(const WB_TINY       *name,
                                             WBXMLCharsetMIBEnum *mib_enum)
{
    WB_ULONG i = 0;
  
    for (i = 0; i < WBXML_TABLE_SIZE(wbxml_charset_entries); i++) {
        if (WBXML_STRCASECMP(name, wbxml_charset_entries[i].name) == 0) {
            if (mib_enum != NULL) {
                *mib_enum = wbxml_charset_entries[i].mib_enum;
            }

            return TRUE;
        }
    }

    return FALSE;
}


WB_BOOL wbxml_charset_get_name(WBXMLCharsetMIBEnum   mib_enum,
                                              const WB_TINY       **name)
{
    WB_ULONG i = 0;
  
    for (i = 0; i < WBXML_TABLE_SIZE(wbxml_charset_entries); i++) {
        if (mib_enum == wbxml_charset_entries[i].mib_enum) {
            if (name != NULL) {
                *name = wbxml_charset_entries[i].name;
            }
      
            return TRUE;
        }
    }
  
    return FALSE;
}


WBXMLError wbxml_charset_conv(const WB_TINY        *in_buf,
                                             WB_ULONG             *io_bytes,
                                             WBXMLCharsetMIBEnum   in_charset,
                                             WBXMLBuffer         **out_buf,
                                             WBXMLCharsetMIBEnum   out_charset)
{
    /**************************************************
     * First, check for simple US-ASCII / UTF-8 cases
     */

    /* Are we dealing with US-ASCII or UTF-8 ? */
    if (((in_charset  == WBXML_CHARSET_US_ASCII) || (in_charset  == WBXML_CHARSET_UTF_8)) &&
        ((out_charset == WBXML_CHARSET_US_ASCII) || (out_charset == WBXML_CHARSET_UTF_8)))
    {
        /* Create a static buffer */
        if ((*out_buf = wbxml_buffer_sta_create_from_cstr(in_buf)) == NULL) {
            return WBXML_ERROR_NOT_ENOUGH_MEMORY;
        }

        /* US-ASCII and UTF-8 are NULL terminated */
        *io_bytes -= WBXML_STRLEN(in_buf) + 1;
    
        return WBXML_OK;
    }
  
    /**************************************
     * Ok guys, we really have to convert
     */
  
#if defined( HAVE_ICONV )
  
    {
        /**********************
         * The iconv way
         */
    
        const WB_TINY * charset_to   = NULL;
        const WB_TINY * charset_from = NULL;
        WB_TINY       * tmp_buf      = NULL;
        WB_TINY       * tmp_ptr      = NULL;
        WB_ULONG        tmp_buf_len  = 0;
        WB_ULONG        tmp_len_left = 0;
        WBXMLError      ret          = WBXML_OK;
        iconv_t         cd           = 0;
        WB_UTINY        last_char    = 0;
    
        /* Get Charsets names */
        if (!wbxml_charset_get_name(in_charset, &charset_from)) {
            return WBXML_ERROR_CHARSET_UNKNOWN;
        }
  
        if (!wbxml_charset_get_name(out_charset, &charset_to)) {
            return WBXML_ERROR_CHARSET_UNKNOWN;
        }
    
        /* Init iconv */
        if ((cd = iconv_open(charset_to, charset_from)) == (iconv_t)(-1))
        {
            /* Init failed */
            return WBXML_ERROR_CHARSET_CONV_INIT;
        }
    
        /* Allocate maximum result buffer (4 bytes unicode) */
        tmp_len_left = tmp_buf_len = 4 * (sizeof(WB_TINY) * (*io_bytes));
    
        if ((tmp_buf = (WB_TINY *) wbxml_malloc(tmp_buf_len)) == NULL) {
            iconv_close(cd);
            return WBXML_ERROR_NOT_ENOUGH_MEMORY;
        }

        tmp_ptr = tmp_buf;
    
        /* Convert ! */
        (void) iconv(cd,
                     &in_buf,
                     (size_t*)io_bytes,
                     &tmp_buf,
                     (size_t*)&tmp_len_left);

        /** @todo Check errno (but it doesn't seems to work on windows) */

        if (tmp_buf_len > tmp_len_left) {
            /* Create result buffer */
            if ((*out_buf = wbxml_buffer_create(tmp_ptr,
                                                tmp_buf_len - tmp_len_left,
                                                tmp_buf_len - tmp_len_left)) == NULL)
            {
                /* Not enough memory */
                ret = WBXML_ERROR_NOT_ENOUGH_MEMORY;
            }

            /* Remove trailing NULL char */
            wbxml_buffer_remove_trailing_zeros(out_buf);
        }
        else
        {
            /* Not converted */
            ret = WBXML_ERROR_CHARSET_CONV;
        }
    
        /* Shutdown iconv */
        iconv_close(cd);
    
        /* Clean-up */
        wbxml_free(tmp_ptr);

        return ret;
    }
  
#else
  
    {
        /***************************************************
         * Add your own charset conversion function here !
         */
    
        return WBXML_ERROR_NO_CHARSET_CONV;
    }
  
#endif /* HAVE_ICONV */
}


WBXMLError wbxml_charset_conv_term(const WB_TINY        *in_buf,
                                                  WB_ULONG             *io_bytes,
                                                  WBXMLCharsetMIBEnum   in_charset,
                                                  WBXMLBuffer         **out_buf,
                                                  WBXMLCharsetMIBEnum   out_charset)
{
    WB_ULONG   buf_len  = 0;
    WB_ULONG   new_len  = 0;
    WB_ULONG   term_len = 0;
    WBXMLError ret      = WBXML_OK;
  
    /* Find length of input buffer */
    switch (in_charset)
    {
    case WBXML_CHARSET_ISO_10646_UCS_2 :
    case WBXML_CHARSET_UTF_16 :
        /* Terminated by two NULL char ("\0\0") */
        term_len = 2;

        if (!binary_search(in_buf, *io_bytes, "\0\0", 2, &buf_len)) {
            return WBXML_ERROR_CHARSET_STR_LEN;
        }

        /* Add termination bytes length */
        buf_len += term_len;
        break;
    
    default :
        /* Terminated by a simple NULL char ('\0') */
        term_len = 1;

        buf_len = WBXML_STRLEN(in_buf) + term_len;
        break;
    }

    /* Check length found */
    if (buf_len > *io_bytes) {
        return WBXML_ERROR_CHARSET_STR_LEN;
    }

    /* Use a temporary length var (because it is decreased) */
    new_len = buf_len;
  
    /* Convert ! */
    ret = wbxml_charset_conv(in_buf, 
                             &new_len,
                             in_charset,
                             out_buf,
                             out_charset);
  
    /* Set input buffer length */           
    *io_bytes = buf_len;
  
    return ret;
}


/***************************************************
 *    Private Functions
 */

/**
 * Binary search of a sequence of bytes into a buffer
 *
 * @param in_buf     Buffer to search in
 * @param in_buf_len Length of input buffer
 * @param in_seq     Sequence to search
 * @param in_seq_len Length of sequence
 * @param out_pos    Index of Sequence into Buffer
 * @return TRUE if found, FALSE otherwise
 */
static WB_BOOL binary_search(const WB_TINY *in_buf,
                             WB_ULONG       in_buf_len,
                             const WB_TINY *in_seq,
                             WB_ULONG       in_seq_len,
                             WB_ULONG      *out_pos)
{
    /** @todo binary_search() / See wbxml_buffer_search() */

    return FALSE;
}
