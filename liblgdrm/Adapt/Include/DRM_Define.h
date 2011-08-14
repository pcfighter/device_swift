
/******************************************************************************
* Developed by	: LGE
* Security			: Confidential
* Module				: LG DRM
* Filename			: DRM_Define.h
* Purpose				: LG DRM Data types and constant definitions
* Author(s)			: Lothson
* e-mail				: lothson@lge.com
* Creation date	: 5th July 2006
* Edit History	: Modification description(s)
Date				Name					Version			Description
---------		-------				------			--------------
2006.07.05	Lothson				v1.00				Initial Creation
2008.02.09	Lothson				v2.00				Release OMA DRM v2.1
******************************************************************************/

#ifndef _DRM_DEFINE_H
#define _DRM_DEFINE_H

/*
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
				THIS FILE CONTAINS VARIOUS KINDS OF FIXED CONSTANTS.
		
	FIXED CONSTANTS MUST BE BASED ON DRM SPECIFICATION AND NEVER BE CHANGED.

	CHANGING THESE VALUES COULD BE DISASTEROUS TO LGE DRM CORE
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
*/

/*
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
		INCLUDE
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
*/

/*
	Include to setup DRM_FILE_PATH_LEN value.

	Except Lab4, all the others projects should exclude the following include and 
	setup proper value for DRM_FILE_PATH_LEN define
*/
//#include "File.h" //DRMHDRMERGE "../../../BasicApiLib/Include/File.h"


/*
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
		DEFINE
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
*/


/* 收收收收收收收收收收 [Data types] 收收收收收收收收收收 */

 
/** 
  @ingroup	Global_DT
  @brief	  If platform supports UCS-2 unicode, then enable this configuration */
//#define DRM_UNICODE_VERSION

#ifdef DRM_UNICODE_VERSION

/**
	@ingroup	Global_DT
	@brief		Data type of UCS-2 character. If selected platform doesn't support UCS-2, it will be unsigned char. */
	typedef unsigned short	DRM_UCHAR;
	//typedef wchar_t					DRM_UCHAR;	/* Linux */
	
	#define __DRM_T(s)		L##s
	#define DRM_T(s)		__DRM_T(s) /* for argument prescan */

#else
	typedef unsigned char		DRM_UCHAR;

	#define DRM_T(s)		s
#endif	// DRM_UNICODE_VERSION

 
/** 
  @ingroup	Global_DT
  @brief	  String */ 
typedef DRM_UCHAR*		DRM_TSTR;
 
/** 
  @ingroup	Global_DT
  @brief	  Const string */ 
typedef const DRM_UCHAR*	DRM_TCSTR;

#ifndef NULL						  /* a NULL pointer */
  #define NULL (void*)	0
#endif


#define DRMTRUE		1
#define DRMFALSE	0


#if defined(WIN32)
typedef		unsigned __int64			DRMUINT64;
typedef						 __int64			DRMINT64;
#elif defined(__arm) || defined(__arm__)
typedef		unsigned long long		DRMUINT64;
typedef						 long long		DRMINT64;
#elif defined(__TMS470__) 
typedef		unsigned long					DRMUINT64;
typedef						 long					DRMINT64;
#elif defined(__ICCARM__) 
typedef		unsigned long long		DRMUINT64;
typedef						 long long		DRMINT64;
#else
typedef		unsigned long					DRMUINT64;
typedef						 long					DRMINT64;
#endif


/* 收收收收收收收收收收 [BYTE ORDERING] 收收收收收收收收收收 */

#define DRM_LITTLE_ENDIAN

#if defined (_WIN32) && !defined(ASDP_WINNT)
#include <windows.h>
#endif

#if defined(DRM_BIG_ENDIAN) && !defined(DRM_LITTLE_ENDIAN)

#define drm_htons(A)  (A)
#define drm_htonl(A)  (A)
#define drm_ntohs(A)  (A)
#define drm_ntohl(A)  (A)

#elif defined(DRM_LITTLE_ENDIAN) && !defined(DRM_BIG_ENDIAN)


#define drm_htons(A)	((((unsigned short)(A) & 0xff00) >> 8)| \
                   		(((unsigned short)(A) & 0x00ff) << 8))
#define drm_htonl(A)	((((unsigned int)(A) & 0xff000000) >> 24)| \
											(((unsigned int)(A) & 0x00ff0000) >> 8)| \
											(((unsigned int)(A) & 0x0000ff00) << 8)| \
											(((unsigned int)(A) & 0x000000ff) << 24)) 
#define drm_ntohs     drm_htons
#define drm_ntohl     drm_htonl

#else

#error "Either BIG_ENDIAN or LITTLE_ENDIAN must be #defined, but not both."

#endif /* BIG_ENDIAN & LITTLE_ENDIAN */


/* 收收收收收收收收收收 [TIME] 收收收收收收收收收收 */

/* Value in seconds */
#define DRM_SECONDS_IN_MINUTE	60
#define DRM_SECONDS_IN_HOUR		(60 * DRM_SECONDS_IN_MINUTE)
#define DRM_SECONDS_IN_DAY		(24 * DRM_SECONDS_IN_HOUR)

#define DRM_DAYS_IN_CENTURY		36524
#define DRM_YEARS_IN_CENTURY	100
#define DRM_MONTH_IN_YEAR			12
#define DRM_YEAR_DAYS					365
#define DRM_MONTH_DAYS				30
#define DRM_EPOCH_YEAR				1970
#define DRM_EPOCH_MONTH				1
#define DRM_EPOCH_DAY					1
#define DRM_MONTH_FEB					2


#define DRM_NO_OF_LEAP_YEARS(X) (X / 4) - (X / 100) + (X / 400) - 477;

/* The starting day of each month, if there's not a leap year.
 * January 1 is day 0, December 31 is day 365.  
 */
static const int	DRM_DAYSUM_OF_MONTH[14] = {
	0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365, 396 
};

static const int	DRM_DAYSUM_OF_MONTH_LEAP[14] = {
	0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366, 397 
};

static const int	DRM_DAYSUM_OF_YEAR[4] = {
	0, 366, 731, 1096
};


#define ONEYEAR		31536000L
#define YEAR0			1900                    /* the first year */
#define EPOCH_YR	1970
#define SECS_DAY	(24L * 60L * 60L)
#define LEAPYEAR(year)		(!((year) % 4) && (((year) % 100) || !((year) % 400)))
#define YEARSIZE(year)		(LEAPYEAR(year) ? 366 : 365)
#define FIRSTSUNDAY(timp)	(((timp)->tm_yday - (timp)->tm_wday + 420) % 7)
#define FIRSTDAYOF(timp)	(((timp)->tm_wday - (timp)->tm_yday + 420) % 7)
#define TIME_MAX	ULONG_MAX
#define ABB_LEN		3

static const int _ytab[2][12] = {
	{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
	{ 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};


/* 收收收收收收收收收收 [DRM MIME] 收收收收收收收收收收 */

/* == OMA DRM == */
#define DRM_MIME_ORT 	"application/vnd.oma.drm.roap-trigger+xml"	/* roap trigger (.ort) */
#define DRM_MIME_ORU 	"application/vnd.oma.drm.roap-pdu+xml" 		/* roap pdu (.oru) */
#define DRM_MIME_ORO 	"application/vnd.oma.drm.ro+xml"					/* protectedRO (.oro) */
#define DRM_MIME_ODF 	"application/vnd.oma.drm.dcf" 						/* ODF (.odf) */
#define DRM_MIME_DR 	"application/vnd.oma.drm.rights+xml" 			/* REL1.0 (.dr) */
#define DRM_MIME_DRC 	"application/vnd.oma.drm.rights+wbxml" 		/* REL1.0 (.drc) */
#define DRM_MIME_DM 	"application/vnd.oma.drm.message" 				/* DM (.dm) */
#define DRM_MIME_DCF 	"application/vnd.oma.drm.content" 				/* DCFv1.0 (.dcf) */
#define DRM_MIME_MP		"multipart/related"											/* Mutipart related */
#define DRM_MIME_WBORT "application/vnd.oma.drm.roap-trigger+wbxml"	/* compact roap trigger (.ort) */
#define DRM_MIME_DD		"application/vnd.oma.dd+xml"

/* == WMDRM == */
#define DRM_MIME_LIC_REQ			"application/vnd.ms-wmdrm.lic-chlg-req"
#define DRM_MIME_LIC_RESP			"application/vnd.ms-wmdrm.lic-resp"
#define DRM_MIME_METER_REQ		"application/vnd.ms-wmdrm.meter-chlg-req"
#define DRM_MIME_METER_RESP		"application/vnd.ms-wmdrm.meter-resp"
#define DRM_MIME_TEXTHTML			"text/html"
#define DRM_MIME_WM_WMA				"audio/x-ms-wma"
#define DRM_MIME_WM_WMV				"video/x-ms-wmv"
/* == OMA SRM == */
#define DRM_MIME_SRM					"application/vnd.oma.drm.srm-pdu"

/* == PlayReady == */
#define DRM_MIME_PR_INITIATOR	"application/vnd.ms-playready.initiator+xml" /* PlayReady Initiator */
#define DRM_MIME_PR_PYA				"audio/vnd.ms-playready.media.pya"			/* PlayReady PMF audio */
#define DRM_MIME_PR_PYV				"video/vnd.ms-playready.media.pyv"			/* PlayReady PMF video */
#define DRM_MIME_PR_TEXTXML		"text/xml" /* PlayReady SOAP v1.1 Message always have text/xml mimetype */
#define DRM_MIME_PR_SOAPv12		"application/soap+xml" /* PlayReady SOAP v1.2 Message always have application/soap+xml mimetype */


/* 收收收收收收收收收收 [DRM EXTENSION] 收收收收收收收收收收 */

/* == OMA DRM == */
#define DRM_EXTENSION_ODF 	DRM_T("odf")			/* ODF (.odf) */
#define DRM_EXTENSION_O4A 	DRM_T("o4a")			/* ODF (.odf) */
#define DRM_EXTENSION_O4V 	DRM_T("o4v")			/* ODF (.odf) */
#define DRM_EXTENSION_DM 		DRM_T("dm")			/* DM (.dm) */
#define DRM_EXTENSION_DCF 	DRM_T("dcf")			/* DCFv1.0 (.dcf) */

/* == CFM DRM == */
#define DRM_EXTENSION_CFM  	DRM_T("cfm")			/* H3G CFM content */

/* == WM DRM == */
#define DRM_EXTENSION_WMA		DRM_T("wma")		/* MS WMDRM PD */
#define DRM_EXTENSION_WMV		DRM_T("wmv")		/* MS WMDRM PD */

/* == DIVX DRM == */
#define DRM_EXTENSION_DIVX_AVI		DRM_T("avi")			/* Divx AVI files */
#define DRM_EXTENSION_DIVX_DMF		DRM_T("divx")			/* Divx Media Format(DMF) */

/* == PLAYREADY == */
#define DRM_EXTENSION_PR_ENVELOP	DRM_T("eny")		/* PlayReady Enveloped File Format */
#define DRM_EXTENSION_PR_AUDIO		DRM_T("pya")		/* Audio ASF format packaged with PlayReady */
#define DRM_EXTENSION_PR_VIDEO		DRM_T("pyv")		/* Video ASF format packaged with PlayReady */


/* 收收收收收收收收收收 [File path & Name Length] 收收收收收收收收收收 */

/** 
  @ingroup	Global_DT
  @brief	  Maximum byte length of file name with path */ 
#define DRM_FILE_PATH_LEN					260 //MAX_FULLPATH //261		/* Max File path length */ (must add NULL when you use) (A_CHAR)
#define DRM_MAX_BUF_FULLPATHLEN		(DRM_FILE_PATH_LEN*3)		//must set this value for UTF8 when call USC2ToUTF8


/* 收收收收收收收收收收 [Key Length] 收收收收收收收收收收 */
 
/** 
  @ingroup	Global_DT
  @brief	  Byte length of content encryption key */ 
#define DRM_CEK_LEN				16
#define DRM_HW_IDLEN			28			/* B64 SHA-1 Hash of this device(IMEI) */


/**
	@ingroup	Global_DT
	@brief		Byte length of DivX base key */
#define DRM_DIVX_BASEKEY_LEN	65

/**
	@ingroup	Global_DT
	@brief		Byte length of Diev user key */
#define DRM_DIVX_USERKEY_LEN	16


/* 收收收收收收收收收收 [Info Length] 收收收收收收收收收收 */
 
/** 
  @ingroup	Global_DT
  @brief	  Maximum XML ANYURI data type length */ 
#define DRM_ANYURI_LEN		1024		/* XML ANYURI Data Type Length */
 
/** 
  @ingroup	Global_DT
  @brief	  Maximum identifer data type length */ 
#define DRM_ID_LEN				192		/* XML ID Length */

/* OMA DRM 2.1 */
/**
	@ingroup	Global_DT
	@brief		RI, RO Alias length, roap:String80 */
#define DRM_STR80_LEN			80
#define DRM_FQDN_LEN			80			/* DRM_STR80_LEN */


/* 收收收收收收收收收收 [MS DRM Key ID] 收收收收收收收收收收 */

/**
	@ingroup	Global_DT
	@brief		Byte length of WM and PR's content encryption object Key ID (Max KID[25] + Prefix[4]) */
#define DRM_KID_LEN				29

/**
	@ingroup	Global_DT
	@brief		Prefix of Key ID */
#define DRM_KID_PREFIX		"kid:"

/**
	@ingroup	Global_DT
	@brief		Byte length of Key ID Prefix */
#define DRM_KID_PREFIX_LEN	4


/* 收收收收收收收收收收 [MIME/EXT] 收收收收收收收收收收 */

/** 
  @ingroup	Global_DT
  @brief	  Maximum length of mime type */ 
#define DRM_MIME_TYPE_LEN		64
 
/** 
  @ingroup	Global_DT
  @brief	  Maximum length of extension */ 
#define DRM_EXT_LEN					8


/* 收收收收收收收收收收 [Rights Information] 收收收收收收收收收收 */
 
/** 
  @ingroup	Global_DT
  @brief	  Maximum cound of supported individual name */ 
#define DRM_INDV_CONSTRAINT_COUNT		3	/* <-- 10 Lothson.20080806 There is no case to use individual constraint more than 1 */
 
/** 
  @ingroup	Global_DT
  @brief	  Maximum length of IMSI */ 
#define DRM_IMSI_LEN						30
 
/** 
  @ingroup	Global_DT
  @brief	  Maximum cound of supported system name */ 
#define DRM_SYS_CONSTRAINT_COUNT		3	/* <-- 10 Lothson.20080806 There is no case to use system constraint more than 1 */
#define DRM_SYS_NAME_LEN			20
 
/** 
  @ingroup	Global_DT
  @brief	  Maximum length of system name */ 
#define DRM_OMNA_NAME_LEN		DRM_ID_LEN
 
/** 
  @ingroup	Global_DT
  @brief	  Maximum length of rights information */ 
#define DRM_INT_ROINFO_LEN 		60	

/**
	@ingroup	Global_DT
	@brief		[OMA] Length of Rights issuer identifier */
#define DRM_RIID_LEN					28


/* 收收收收收收收收收收 [DRM Support] 收收收收收收收收收收 */
 
/** 
  @ingroup	Global_DT
  @brief	  Maximum length of DRM agent version */ 
#define DRM_DRMLIBVERSION_LEN	5

/** 
  @ingroup	Global_DT
  @brief	  Maximum length of modification information */ 
#define DRM_MODTIME_LEN			20


/* 收收收收收收收收收收 [OMA DRMv2.0 ROAP DOMAIN] 收收收收收收收收收收 */

#define DRM_DOMAINID_LEN				20	/* maximum size of domain-name */

/**
	@ingroup	Global_DT
	@brief		maximum size of domain-name */
#define DRM_DOMAINBASEID_LEN		17
#define DRM_DOMAINID_GENNUM_LEN	3		/* fixed generation digits */
#define DRM_DKEY_LEN						16


/* 收收收收收收收收收收 [Filesystem] 收收收收收收收收收收 */

/* File system type */
typedef enum _DRMFSType
{
	eDRM_FSTYPE_UNKNOW = 0,	/* Unknown */
	eDRM_FSTYPE_INT,				/* Internal memory */
	eDRM_FSTYPE_MMC					/* External memory */
} enDRMFSType;

#endif /* _DRM_DEFINE_H */

