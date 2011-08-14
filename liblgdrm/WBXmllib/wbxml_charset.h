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
 * @file wbxml_charset.h
 * @ingroup wbxml_charset
 *
 * @author Aymerick Jehanne <libwbxml@aymerick.com>
 * @date 04/03/24
 *
 * @brief Charset Functions
 */

#ifndef WBXML_CHARSET_H
#define WBXML_CHARSET_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
  
/** @addtogroup wbxml_charset 
 *  @{ 
 */

/** WBXML Charsets MIBEnum */
typedef enum WBXMLCharsetMIBEnum_e {
  WBXML_CHARSET_UNKNOWN         = 0,       /**< Unknown Charset */
  WBXML_CHARSET_US_ASCII        = 3,       /**< US-ASCII */
  WBXML_CHARSET_ISO_8859_1      = 4,       /**< ISO-8859-1 */
  WBXML_CHARSET_ISO_8859_2      = 5,       /**< ISO-8859-2 */
  WBXML_CHARSET_ISO_8859_3      = 6,       /**< ISO-8859-3 */
  WBXML_CHARSET_ISO_8859_4      = 7,       /**< ISO-8859-4 */
  WBXML_CHARSET_ISO_8859_5      = 8,       /**< ISO-8859-5 */
  WBXML_CHARSET_ISO_8859_6      = 9,       /**< ISO-8859-6 */
  WBXML_CHARSET_ISO_8859_7      = 10,      /**< ISO-8859-7 */
  WBXML_CHARSET_ISO_8859_8      = 11,      /**< ISO-8859-8 */
  WBXML_CHARSET_ISO_8859_9      = 12,      /**< ISO-8859-9 */
  WBXML_CHARSET_SHIFT_JIS       = 17,      /**< Shift_JIS */
  WBXML_CHARSET_UTF_8           = 106,     /**< UTF-8 */
  WBXML_CHARSET_ISO_10646_UCS_2 = 1000,    /**< ISO-10646-UCS-2 */
  WBXML_CHARSET_UTF_16          = 1015,    /**< UTF-16 */
  WBXML_CHARSET_BIG5            = 2026     /**< Big5 */
} WBXMLCharsetMIBEnum;


/**
 * @brief Get a Charset MIBEnum given a Charset Name
 * @param name     [in]  Charset Name to search
 * @param mib_enum [out] MIBEnum if found
 * @return Return TRUE if Charset found, FALSE otherwise
 */
WB_BOOL wbxml_charset_get_mib(const WB_TINY       *name,
                                             WBXMLCharsetMIBEnum *mib_enum);

/**
 * @brief Get a Charset Name given a Charset MIBEnum
 * @param mib_enum [in]  MIBEnum to search
 * @param name     [out] Charset Name if found
 * @return Return TRUE if Charset found, FALSE otherwise
 */
WB_BOOL wbxml_charset_get_name(WBXMLCharsetMIBEnum   mib_enum,
                                              const WB_TINY       **name);
  
/**
 * Charset Convertion function
 *
 * Input is a normal pointer to buffer to convert.
 *
 * Result is a WBXML Buffer. If 'in_charset' and 'out_charset' are
 * identical, and are even ASCII or UTF-8, this will be a static
 * buffer pointing to input buffer.
 *
 * The 'io_bytes' parameter is decremented each time a byte is correctly
 * converted from 'in_buf', so that it reflects the number of bytes that
 * have been converted from 'in_buf'.
 *
 * @param in_buf      Buffer to convert
 * @param io_bytes    Number of bytes in buffer
 * @param in_charser  Original charset
 * @param out_buf     Resulting converted Buffer
 * @param out_charset Destination charset
 */
WBXMLError wbxml_charset_conv(const WB_TINY        *in_buf,
                                             WB_ULONG             *io_bytes,
                                             WBXMLCharsetMIBEnum   in_charset,
                                             WBXMLBuffer         **out_buf,
                                             WBXMLCharsetMIBEnum   out_charset);

/**
 * Charset Convertion function, for unknown length strings
 *
 * This is a wrapper around wbxml_charset_conv(), but to convert
 * a buffer that have an unknown length. This function first try to find
 * the buffer length, by finding its charset specific termination code
 * (eg: '\0', '\0\0'), then it calls the wbxml_charset_conv() function.
 *
 * Set the maximum possible length of input buffer into 'io_bytes'. A check
 * is done if length found is higher than input 'io_bytes' value.
 *
 * WARNING : 'io_bytes' is then set to the real length of input buffer (this is
 * a different behaviour than with wbxml_charset_conv()).
 *
 * @param in_buf      Buffer to convert
 * @param io_bytes    Number of bytes in buffer
 * @param in_charser  Original charset
 * @param out_buf     Resulting converted Buffer
 * @param out_charset Destination charset
 */
WBXMLError wbxml_charset_conv_term(const WB_TINY        *in_buf,
                                                  WB_ULONG             *io_bytes,
                                                  WBXMLCharsetMIBEnum   in_charset,
                                                  WBXMLBuffer         **out_buf,
                                                  WBXMLCharsetMIBEnum   out_charset);

/** @} */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* WBXML_CHARSET_H */
