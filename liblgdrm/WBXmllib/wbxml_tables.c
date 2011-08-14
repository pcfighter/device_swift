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
 * @file wbxml_tables.c
 * @ingroup wbxml_tables
 *
 * @author Aymerick Jehanne <libwbxml@aymerick.com>
 * @date 02/03/17
 *
 * @brief WBXML Tables
 */

#include "wbxml.h"

/** 
 * @brief If undefined, only the WML 1.3 tables are used for all WML versions (WML 1.0 / WML 1.1 / WML 1.2 / WML 1.3).
 *        It saves space, and, well, every handset must supports WML 1.3 right now.
 *        If defined, each version has its own exact tables.
 */
#undef WBXML_TABLES_SEPARATE_WML_VERSIONS


/**************************************
 * Languages Public IDs
 */

/* DRM */
#if defined( WBXML_SUPPORT_DRMREL )
const WBXMLPublicIDEntry sv_drmrel10_public_id = 
{ 
	WBXML_PUBLIC_ID_DRMREL10, 
	XML_PUBLIC_ID_DRMREL10,
	"o-ex:rights",
	"http://www.openmobilealliance.org/DTD/drmrel10.dtd"
};
#endif /* WBXML_SUPPORT_DRMREL */

#if defined( WBXML_SUPPORT_DRM2_1 )
const WBXMLPublicIDEntry sv_drm2_1_public_id =
{ 
	WBXML_PUBLIC_ID_DRM2_1, 
	XML_PUBLIC_ID_DRM2_1,
	"roap:roapTrigger",
	"urn:oma:bac:dldrm:roap-1.0"	/* No meaning */
};
#endif /* WBXML_SUPPORT_DRM2_1 */

#if defined( WBXML_SUPPORT_SRM )
const WBXMLPublicIDEntry sv_srm_public_id =
{ 
	WBXML_PUBLIC_ID_SRM, 
	XML_PUBLIC_ID_SRM,
	"oma-dd:roContainer",
	"http://www.openmobilealliance.com/oma-dd"	/* No meaning */
};
#endif /* WBXML_SUPPORT_SRM */


/**************************************
 * Languages Tables
 */

#if defined( WBXML_SUPPORT_DRMREL )

/*************************************************
 *    Rights Expression Language Version 1.0 ("OMA-Download-DRMREL-v1_0-20020913-a.pdf")
 */
 
const WBXMLTagEntry sv_drmrel10_tag_table[] = {
    { "o-ex:rights",			0x00, 0x05 },
    { "o-ex:context",			0x00, 0x06 },
    { "o-dd:version",			0x00, 0x07 },
    { "o-dd:uid",					0x00, 0x08 },
    { "o-ex:agreement",		0x00, 0x09 },
    { "o-ex:asset",				0x00, 0x0A },
    { "ds:KeyInfo",				0x00, 0x0B },
    { "ds:KeyValue",			0x00, 0x0C },
    { "o-ex:permission",	0x00, 0x0D },
    { "o-dd:play",				0x00, 0x0E },
    { "o-dd:display",			0x00, 0x0F },
    { "o-dd:execute",			0x00, 0x10 },
    { "o-dd:print",				0x00, 0x11 },
    { "o-ex:constraint",	0x00, 0x12 },
    { "o-dd:count",				0x00, 0x13 },
    { "o-dd:datetime",		0x00, 0x14 },
    { "o-dd:start",				0x00, 0x15 },
    { "o-dd:end",					0x00, 0x16 },
    { "o-dd:interval",		0x00, 0x17 },
		{ NULL,								0x00, 0x00 }
};

const WBXMLAttrEntry sv_drmrel10_attr_table[] = {
    { "xmlns:o-ex", NULL,	0x00, 0x05 },
    { "xmlns:o-dd",	NULL,	0x00, 0x06 },
    { "xmlns:ds",		NULL,	0x00, 0x07 },
    { NULL,					NULL,	0x00, 0x00 }
};

const WBXMLAttrValueEntry sv_drmrel10_attr_value_table[] = {
    { "http://odrl.net/1.1/ODRL-EX",		0x00, 0x85 },
    { "http://odrl.net/1.1/ODRL-DD",		0x00, 0x86 },
    { "http://www.w3.org/2000/09/xmldsig#/",	0x00, 0x87 },
		{ NULL,															0x00, 0x00 }
};

#endif /* WBXML_SUPPORT_DRMREL */


#if defined( WBXML_SUPPORT_DRM2_1 )

/*************************************************
 *    OMA DRM v2.1
 */
 
const WBXMLTagEntry sv_drm2_1_tag_table[] = {
	{"roap:roapTrigger",					0x00,	0x05},	
	{"registrationRequest",				0x00,	0x06},
	{"roAcquisition",							0x00,	0x07},
	{"joinDomain",								0x00,	0x08},
	{"leaveDomain",								0x00,	0x09},
	{"signature",									0x00,	0x0A},
	{"encKey",										0x00,	0x0B},
	{"riID",											0x00,	0x0C},
	{"riAlias",										0x00,	0x0D},
	{"nonce",											0x00,	0x0E},
	{"roapURL",										0x00,	0x0F},
	{"domainID",									0x00,	0x10},
	{"domainAlias",								0x00,	0x11},
	{"roap:domainID",							0x00,	0x12},
	{"roID",											0x00,	0x13},
	{"roAlias",										0x00,	0x14},
	{"contentID",									0x00,	0x15},
	{"roap:X509SPKIHash",					0x00,	0x16},
	{"keyIdentifier",							0x00,	0x17},
	{"hash",											0x00,	0x18},
	{"ds:SignedInfo",							0x00,	0x19},
	{"ds:SignatureValue",					0x00,	0x1A},
	{"ds:KeyInfo",								0x00,	0x1B},
	{"ds:CanonicalizationMethod",	0x00,	0x1C},
	{"ds:SignatureMethod",				0x00,	0x1D},
	{"ds:Reference",							0x00,	0x1E},
	{"ds:RetrievalMethod",				0x00,	0x1F},
	{"ds:Transforms",							0x00,	0x20},
	{"ds:DigestMethod",						0x00,	0x21},
	{"ds:DigestValue",						0x00,	0x22},
	{"ds:Transform",							0x00,	0x23},
	{"xenc:EncryptionMethod",			0x00,	0x24},
	{"xenc:CipherData",						0x00,	0x25},
	{"xenc:CipherValue",					0x00,	0x26},
	{"extendedTrigger",						0x00,	0x27},
	{"trgLeaveDomain",						0x00,	0x28},
	{"deviceID",									0x00,	0x29},
	{ NULL,             					0x00, 0x00 }
};

const WBXMLAttrEntry sv_drm2_1_attr_table[] = {
	{"xsi:type",			NULL,	0x00,	0x05},	
	{"xmlns:roap",		NULL,	0x00,	0x06},	
	{"xmlns:xsi",			NULL,	0x00,	0x07},	
	{"xmlns:xenc",		NULL,	0x00,	0x08},	
	{"xmlns:ds",			NULL,	0x00,	0x09},	
	{"xmlns:o-ex",		NULL,	0x00,	0x0A},	
	{"xmlns:o-dd",		NULL,	0x00,	0x0B},	
	{"xmlns:oma-dd",	NULL,	0x00,	0x0C},	
	{"version",				NULL,	0x00,	0x0D},	
	{"proxy",					NULL,	0x00,	0x0E},	
	{"id",						NULL,	0x00,	0x0F},	
	{"Id",						NULL,	0x00,	0x10},	
	{"algorithm",			NULL,	0x00,	0x11},	
	{"Algorithm",			NULL,	0x00,	0x12},	
	{"URI",						NULL,	0x00,	0x13},	
	{"type",					NULL,	0x00,	0x14},	
	{ NULL,						NULL,	0x00,	0x00 }
};

const WBXMLAttrValueEntry sv_drm2_1_attr_value_table[] = {
	{"urn:oma:bac:dldrm:roap-1.0",	0x00,	0x85},	
	{"http://odrl.net/1.1/ODRL-EX",	0x00,	0x86},	
	{"http://odrl.net/1.1/ODRL-DD",	0x00,	0x87},	
	{"http://www.openmobilealliance.com/oma-dd",	0x00,	0x88},	
	{"http://www.w3.org/2000/09/xmldsig#",				0x00,	0x89},	
	{"http://www.w3.org/2001/04/xmlenc#",					0x00,	0x8A},	
	{"http://www.w3.org/2001/XMLSchema-instance",	0x00,	0x8B},	
	{"roap:X509SPKIHash",	0x00,	0x8C},	
	{"http://www.w3.org/2000/09/xmldsig#sha1",			0x00,	0x8D},	
	{"http://www.w3.org/2001/10/xml-exc-c14n#",			0x00,	0x8E},	
	{"http://www.w3.org/2000/09/xmldsig#hmac-sha1",	0x00,	0x8F},	
	{"1.1",		0x00,	0x90},	
	{"2.0",		0x00,	0x91},	
	{"2.1",		0x00,	0x92},	
	{"identificationRequest",	0x00,	0x93},	
	{"roUploadRequest ",			0x00,	0x94},	
	{"meteringReport",				0x00,	0x95},	
	{"leaveDomain",						0x00,	0x96},	
	{ NULL,										0x00,	0x00 }
};

#endif /* WBXML_SUPPORT_DRM2_1 */



#if defined( WBXML_SUPPORT_SRM )

/*************************************************
 *    OMA SRM v1.0
 */
 
const WBXMLTagEntry sv_srm_tag_table[] = {
	// For Testing
	//{"o-ex:rights",						0x00,0x05},
	{"rights",								0x00,0x05},
	{"o-ex:context",					0x00,0x06},
	{"o-ex:agreement",				0x00,0x07},
	{"o-ex:asset",						0x00,0x08},
	{"o-ex:inherit",					0x00,0x09},
	{"o-ex:permission",				0x00,0x0A},
	{"o-ex:requirement",			0x00,0x0B},
	{"o-ex:constraint",				0x00,0x0C},
	{"o-ex:digest",						0x00,0x0D},
	{"o-dd:version",					0x00,0x0E},
	{"o-dd:uid",							0x00,0x0F},
	{"o-dd:play",							0x00,0x10},
	{"o-dd:display",					0x00,0x11},
	{"o-dd:execute",					0x00,0x12},
	{"o-dd:print",						0x00,0x13},
	{"o-dd:export",						0x00,0x14},
	{"o-dd:move",							0x00,0x15},
	{"o-dd:save",							0x00,0x16},
	{"o-dd:tracked",					0x00,0x17},
	{"o-dd:count",						0x00,0x18},
	{"o-dd:datetime",					0x00,0x19},
	{"o-dd:start",						0x00,0x1A},
	{"o-dd:end",							0x00,0x1B},
	{"o-dd:interval",					0x00,0x1C},
	{"o-dd:accumulated",			0x00,0x1D},
	{"o-dd:individual",				0x00,0x1E},
	{"oma-dd:timed-count",		0x00,0x1F},
	{"oma-dd:system",					0x00,0x20},
	{"oma-dd:access",					0x00,0x21},
	{"oma-dd:token-based",		0x00,0x22},
	{"oma-dd:token-constraint-count",				0x00,0x23},
	{"oma-dd:token-constraint-timed-count",	0x00,0x24},
	{"oma-dd:token-accumulated",						0x00,0x25},
	{"oma-dd:token-unit",			0x00,0x26},
	{"oma-dd:token-consumed",	0x00,0x27},
	// For Testing
	{"oma-dd:roContainer",			0x00,0x28},
	//{"roContainer",					0x00,0x28},
	{"xenc:EncryptedKey",				0x00,0x29},
	{"xenc:EncryptionMethod",		0x00,0x2A},
	{"xenc:CipherData",					0x00,0x2B},
	{"xenc:CipherValue",				0x00,0x2C},
	{"ds:DigestMethod",					0x00,0x2D},
	{"ds:DigestValue",					0x00,0x2E},
	{"ds:KeyInfo",							0x00,0x2F},
	{"ds:RetrievalMethod",			0x00,0x30},
	{"ds:SignedInfo",						0x00,0x31},
	{"ds:CanonicalizationMethod",0x00,0x32},
	{"ds:SignatureMethod",			0x00,0x33},
	{"ds:Reference",						0x00,0x34},
	{"ds:Transforms",						0x00,0x35},
	{"ds:Transform",						0x00,0x36},
	{"ds:SignatureValue",				0x00,0x37},
	{"roap:X509SPKIHash",				0x00,0x38},
	{"hash",										0x00,0x39},
	/// For testing
	//{"ds:signature",						0x00,0x3A},
	{"signature",								0x00,0x3A},
	{ NULL,             			0x00, 0x00 }
};

const WBXMLAttrEntry sv_srm_attr_table[] = {
	{"xmlns:o-ex",		NULL,	0x00,	0x05},
	{"xmlns:o-dd",		NULL,	0x00,	0x06},
	{"xmlns:ds",			NULL,	0x00,	0x07},
	{"xmlns:oma-dd",	NULL,	0x00,	0x08},
	{"xmlns:xenc",		NULL,	0x00,	0x09},
	{"o-ex:id",				NULL,	0x00,	0x0A},
	{"o-ex:idref",		NULL,	0x00,	0x0B},
	{"Algorithm",			NULL,	0x00,	0x0C},
	{"URI",						NULL,	0x00,	0x0D},
	{"oma-dd:onExpiredURL",NULL,	0x00,	0x0E},
	{"oma-dd:timer",	NULL,	0x00,	0x0F},
	{"oma-dd:mode",		NULL,	0x00,	0x10},
	{"oma-dd:timed",	NULL,	0x00,	0x11},
	{"oma-dd:contentAccessGranted",			NULL,	0x00,	0x12},
	{"oma-dd:token-timed-count-timer",	NULL,	0x00,	0x13},
	/// For testing
	/// { "xmlns:roap",       NULL,	0x00,	0x14 },
	{ NULL,						NULL,	0x00,	0x00 }
};

const WBXMLAttrValueEntry sv_srm_attr_value_table[] = {
	{"http://odrl.net/1.1/ODRL-EX",	0x00,	0x85},
	{"http://odrl.net/1.1/ODRL-DD",	0x00,	0x86},
	{"http://www.openmobilealliance.com/oma-dd",	0x00,	0x87},
	/*
		http://www.w3.org/2000/09/xmldsig#
		http://www.w3.org/2000/09/xmldsig#sha1

		Attribute value table searching is started from index 0.
		If xmldsig# is comes first, then xmldsig#sha1 will be detected encoded
		as attribue vale with inline string sha1.

		So, substring must come fist.
	*/
	{"http://www.w3.org/2000/09/xmldsig#sha1",	0x00,	0x8A},
	{"http://www.w3.org/2000/09/xmldsig#",			0x00,	0x88},
	{"http://www.w3.org/2001/04/xmlenc#kw-aes128",	0x00,	0x8B},
	{"http://www.w3.org/2001/04/xmlenc#",				0x00,	0x89},
	{"http://www.w3.org/2001/10/xml-exc-c14n#",	0x00,	0x8C},
	{"http://www.rsasecurity.com/rsalabs/pkcs/schemas/pkcs-1#rsa-pss-default",	0x00,	0x8D},
	{"#K_MAC_and_K_REK",0x00,0x8E},
	{"move",	0x00,	0x8F},
	{"copy",	0x00,	0x90},
	{"true",	0x00,	0x91},
	{"false",	0x00,	0x92},
	/// For Testing
	///{"urn:oma:bac:dldrm:roap-1.0",		0x00,	0x93},
	{ NULL,		0x00,	0x00 }
};

#endif /* WBXML_SUPPORT_SRM */


/******************************
 *    Main Table
 */

#if defined( WBXML_DYNAMIC_CODEPAGE )
WBXMLLangEntry sv_table_entry[] = {
#else
const WBXMLLangEntry sv_table_entry[] = {
#endif

#if defined( WBXML_SUPPORT_DRMREL )
    {	WBXML_LANG_DRMREL10,          
		&sv_drmrel10_public_id,
		sv_drmrel10_tag_table,
		NULL,
		sv_drmrel10_attr_table,
		sv_drmrel10_attr_value_table,
		NULL 
	},
#endif /* WBXML_SUPPORT_DRMREL */

#if defined( WBXML_SUPPORT_DRM2_1 )
    {	WBXML_LANG_OMADRM2_1,          
		&sv_drm2_1_public_id,
		sv_drm2_1_tag_table,
		NULL,
		sv_drm2_1_attr_table,
		sv_drm2_1_attr_value_table,
		NULL 
	},
#endif /* WBXML_SUPPORT_DRM2_1 */

#if defined( WBXML_SUPPORT_SRM )
    {	WBXML_LANG_OMASRM,          
		&sv_srm_public_id,
		sv_srm_tag_table,
		NULL,
		sv_srm_attr_table,
		sv_srm_attr_value_table,
		NULL 
	},
#endif /* WBXML_SUPPORT_SRM */

    {	WBXML_LANG_UNKNOWN,          
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL 
	}
};


/******************************
 * Public Functions
 */

/* Exported function to return pointer to WBXML Languages Main Table */
const WBXMLLangEntry * wbxml_tables_get_main(void)
{
#if defined( WBXML_DYNAMIC_CODEPAGE )
    return (const WBXMLLangEntry *)sv_table_entry;
#else
	return sv_table_entry;
#endif
}


const WBXMLLangEntry * wbxml_tables_get_table(WBXMLLanguage lang)
{
    const WBXMLLangEntry *main_table = NULL;
    WB_ULONG index = 0;
    
    /* Get main tables array*/
    if ((lang == WBXML_LANG_UNKNOWN) || ((main_table = wbxml_tables_get_main()) == NULL))
        return NULL;
    
    /* Search language table */
    while (main_table[index].langID != WBXML_LANG_UNKNOWN) {
        if (main_table[index].langID == lang)
            return &main_table[index];
        index++;
    }

    return NULL;
}


const WBXMLLangEntry * wbxml_tables_search_table(const WBXMLLangEntry *main_table,
                                                                const WB_UTINY *public_id, 
                                                                const WB_UTINY *system_id,
                                                                const WB_UTINY *root)
{
    WB_ULONG index;

    if (main_table == NULL)
        return NULL;

    /* Search by XML Public ID  */
    if (public_id != NULL) {
        index = 0;

        while (main_table[index].publicID != NULL) {
            if (main_table[index].publicID->xmlPublicID && WBXML_STRCASECMP((const WB_TINY *)main_table[index].publicID->xmlPublicID, (const WB_TINY *)public_id) == 0)
                return &main_table[index];
            index++;
        }
    }

    /* Search by XML System ID  */
    if (system_id != NULL) {
        index = 0;

        while (main_table[index].publicID != NULL) {
            if (main_table[index].publicID->xmlDTD && WBXML_STRCMP((const WB_TINY *)main_table[index].publicID->xmlDTD, (const WB_TINY *)system_id) == 0) 
                return &main_table[index];
            index++;
        }
    }

    /* Search by XML Root Element  */
    if (root != NULL) {
        index = 0;

        while (main_table[index].publicID != NULL) {
            if (main_table[index].publicID->xmlRootElt && WBXML_STRCMP((const WB_TINY *)main_table[index].publicID->xmlRootElt, (const WB_TINY *)root) == 0) 
                return &main_table[index];
            index++;
        }
    }

    return NULL;
}


WB_ULONG wbxml_tables_get_wbxml_publicid(const WBXMLLangEntry *main_table, WBXMLLanguage lang_id)
{
    WB_ULONG i = 0;

    if (main_table == NULL)
        return WBXML_PUBLIC_ID_UNKNOWN;

    while (main_table[i].langID != -1) {
        if (main_table[i].langID == lang_id) {
            if (main_table[i].publicID != NULL)
                return main_table[i].publicID->wbxmlPublicID;
            else
                return WBXML_PUBLIC_ID_UNKNOWN;
        }
        i++;
    }

    return WBXML_PUBLIC_ID_UNKNOWN;
}


const WBXMLTagEntry * wbxml_tables_get_tag_from_xml(const WBXMLLangEntry *lang_table,
                                                                   const WB_UTINY *xml_name)
{
    WB_ULONG i = 0;

    if ((lang_table == NULL) || (lang_table->tagTable == NULL) || (xml_name == NULL))
        return NULL;

    while (lang_table->tagTable[i].xmlName != NULL) {
        if (WBXML_STRCMP((const WB_TINY *)lang_table->tagTable[i].xmlName, (const WB_TINY *)xml_name) == 0)
            return &(lang_table->tagTable[i]);
        i++;
    }

    return NULL;
}


const WBXMLAttrEntry * wbxml_tables_get_attr_from_xml(const WBXMLLangEntry *lang_table,
                                                                     WB_UTINY *xml_name,
                                                                     WB_UTINY *xml_value,
                                                                     WB_UTINY **value_left)
{
    WB_ULONG i = 0;
    WB_ULONG found_index = 0, found_comp = 0;
    WB_BOOL found = FALSE;

    if ((lang_table == NULL) || (lang_table->attrTable == NULL) || (xml_name == NULL))
        return NULL;

    if (value_left != NULL)
        *value_left = xml_value;

    /* Iterate in Attribute Table */
    while (lang_table->attrTable[i].xmlName != NULL) {
        /* Search for Attribute Name */
        if (WBXML_STRCMP((const WB_TINY *)lang_table->attrTable[i].xmlName, (const WB_TINY *)xml_name) == 0) 
        {
            if (lang_table->attrTable[i].xmlValue == NULL) {
                /* This is the token with a NULL Attribute Value */
                if (xml_value == NULL) {
                    /* Well, we got it */
                    return &(lang_table->attrTable[i]);
                }
                else {
                    if (!found) {
                        /* We haven't found yet a better Attribute Token */
                        found = TRUE;
                        found_index = i;
                    }

                    /* Else: We already have found a better Attribute Token, so let's forget this one */
                }
            }
            else {
                /* Check the Attribute Value */
                if (xml_value != NULL)
                {
                    if (WBXML_STRCMP((const WB_TINY *)lang_table->attrTable[i].xmlValue, (const WB_TINY *)xml_value) == 0) 
                    {
                        /* We have found the EXACT Attribute Name / Value pair we are searching, well done boy */
                        if (value_left != NULL)
                            *value_left = NULL;

                        return &(lang_table->attrTable[i]);
                    }
                    else {
                        if ((WBXML_STRLEN((const WB_TINY *)lang_table->attrTable[i].xmlValue) < WBXML_STRLEN((const WB_TINY *)xml_value)) &&
                            (found_comp < WBXML_STRLEN(lang_table->attrTable[i].xmlValue)) &&
                            (WBXML_STRNCMP((const WB_TINY *)lang_table->attrTable[i].xmlValue, (const WB_TINY *)xml_value, WBXML_STRLEN((const WB_TINY *)lang_table->attrTable[i].xmlValue)) == 0))
                        {
                            /* We have found a better Attribute Value */
                            found = TRUE;
                            found_index = i;
                            found_comp = WBXML_STRLEN(lang_table->attrTable[i].xmlValue);
                        }
                    }
                }

                /* Else: We are searching for the Attribute Token with a NULL Attribute Value associated, so forget this one  */
            }
        }
        i++;
    }

    /* Attribute Name / Value pair not found, but an entry with this Attribute Name, 
     * and (maybe) start of this Attribute Value was found */
    if (found) {
        if (value_left != NULL)
            *value_left = xml_value + found_comp;

        return &(lang_table->attrTable[found_index]);
    }

    /* Attribute Name NOT found */
    return NULL;
}


const WBXMLExtValueEntry * wbxml_tables_get_ext_from_xml(const WBXMLLangEntry *lang_table,
                                                                        WB_UTINY *xml_value)
{
    WB_ULONG i = 0;

    if ((lang_table == NULL) || (lang_table->extValueTable == NULL) || (xml_value == NULL))
        return NULL;

    while (lang_table->extValueTable[i].xmlName != NULL) {
        if (WBXML_STRCMP((const WB_TINY *)lang_table->extValueTable[i].xmlName, (const WB_TINY *)xml_value) == 0)
            return &(lang_table->extValueTable[i]);
        i++;
    }

    return NULL;
}


WB_BOOL wbxml_tables_contains_attr_value_from_xml(const WBXMLLangEntry *lang_table,
                                                                 WB_UTINY *xml_value)
{
    WB_ULONG i = 0;

    if ((lang_table == NULL) || (lang_table->attrValueTable == NULL) || (xml_value == NULL))
        return FALSE;

    while (lang_table->attrValueTable[i].xmlName != NULL)
    {
        /* Is this Attribute Value contained in this XML Buffer ? */
        if (WBXML_STRSTR((const WB_TINY *)xml_value, lang_table->attrValueTable[i].xmlName) != NULL)
            return TRUE;

        i++;
    }

    return FALSE;
}


const WB_TINY * wbxml_tables_get_xmlns(const WBXMLNameSpaceEntry *ns_table, WB_UTINY code_page)
{
    WB_ULONG i = 0;

    if (ns_table == NULL)
        return NULL;

    while (ns_table[i].xmlNameSpace != NULL)
    {
        if (ns_table[i].wbxmlCodePage == code_page)
            return ns_table[i].xmlNameSpace;

        i++;
    }

    return NULL;
}

#if defined( WBXML_DYNAMIC_CODEPAGE )
WB_LONG wbxml_tables_get_num(WBXMLLanguage lang, WBXMLTableType type)
{
	WB_LONG count;
	WBXMLLangEntry *langEntry = (WBXMLLangEntry *)wbxml_tables_get_table(lang);

	if (!langEntry)	{
		return -1;
	}

	switch (type)
	{
	case eWBXML_TBLTYPE_TAG:
		if (!langEntry->tagTable)
			return 0;

		for (count=0; langEntry->tagTable[count].xmlName != NULL; count++) {
			;
		}
		return count;

	case eWBXML_TBLTYPE_NS:
		if (!langEntry->nsTable)
			return 0;

		for (count=0; langEntry->nsTable[count].xmlNameSpace != NULL; count++) {
			;
		}
		return count;

	case eWBXML_TBLTYPE_ATTR:
		if (!langEntry->attrTable)
			return 0;

		for (count=0; langEntry->attrTable[count].xmlName != NULL; count++) {
			;
		}
		return count;

	case eWBXML_TBLTYPE_ATTRVALUE:
		if (!langEntry->attrValueTable)
			return 0;

		for (count=0; langEntry->attrValueTable[count].xmlName != NULL; count++) {
			;
		}
		return count;

	case eWBXML_TBLTYPE_EXTVALUE:
		if (!langEntry->extValueTable)
			return 0;

		for (count=0; langEntry->extValueTable[count].xmlName != NULL; count++) {
			;
		}
		return count;
	}

	return -1;
}

WB_BOOL wbxml_tables_copy(void *dst, WBXMLLanguage lang, WBXMLTableType type, WB_LONG count)
{
	WBXMLLangEntry *langEntry = (WBXMLLangEntry *)wbxml_tables_get_table(lang);

	if (!dst || !langEntry || !count) {
		return FALSE;
	}

	switch (type)
	{
	case eWBXML_TBLTYPE_TAG:
		if (!langEntry->tagTable)
			return FALSE;

		memcpy(dst, langEntry->tagTable, count * sizeof(WBXMLTagEntry));
		return TRUE;
		
	case eWBXML_TBLTYPE_NS:
		if (!langEntry->nsTable)
			return FALSE;

		memcpy(dst, langEntry->nsTable, count * sizeof(WBXMLNameSpaceEntry));
		return TRUE;
		
	case eWBXML_TBLTYPE_ATTR:
		if (!langEntry->attrTable)
			return FALSE;
		
		memcpy(dst, langEntry->attrTable, count * sizeof(WBXMLAttrEntry));
		return TRUE;
		
	case eWBXML_TBLTYPE_ATTRVALUE:
		if (!langEntry->attrValueTable)
			return FALSE;

		memcpy(dst, langEntry->attrValueTable, count * sizeof(WBXMLAttrValueEntry));
		return TRUE;
		
	case eWBXML_TBLTYPE_EXTVALUE:
		if (!langEntry->extValueTable)
			return FALSE;

		memcpy(dst, langEntry->extValueTable, count * sizeof(WBXMLExtValueEntry));
		return TRUE;
	}

	return FALSE;
}

void *wbxml_tables_replace(void *src, WBXMLLanguage lang, WBXMLTableType type)
{
	void *old = NULL;
	WBXMLLangEntry *langEntry = (WBXMLLangEntry *)wbxml_tables_get_table(lang);

	if (!src || !langEntry) {
		return NULL;
	}

	switch (type)
	{
	case eWBXML_TBLTYPE_TAG:
		old = (void*)langEntry->tagTable;
		langEntry->tagTable = (WBXMLTagEntry*) src;
		break;
		
	case eWBXML_TBLTYPE_NS:
		old = (void*)langEntry->nsTable;
		langEntry->nsTable = (WBXMLNameSpaceEntry*) src;
		break;
		
	case eWBXML_TBLTYPE_ATTR:
		old = (void*)langEntry->attrTable;
		langEntry->attrTable = (WBXMLAttrEntry*) src;
		break;
		
	case eWBXML_TBLTYPE_ATTRVALUE:
		old = (void*)langEntry->attrValueTable;
		langEntry->attrValueTable = (WBXMLAttrValueEntry*) src;
		break;
		
	case eWBXML_TBLTYPE_EXTVALUE:
		old = (void*)langEntry->extValueTable;
		langEntry->extValueTable = (WBXMLExtValueEntry*) src;
		break;
	}

	return old;
}

#endif
