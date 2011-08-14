
/******************************************************************************
* Developed by	: LGE
* Security			: Confidential
* Module				: LG DRM
* Filename			: DRM_Config.h
* Purpose				: Configure LG DRM Setting
* Author(s)			: Lothson
* e-mail				: lothson@lge.com
* Creation date	: 5th July 2006
* Edit History	: Modification description(s)
Date				Name					Version			Description
---------		-------				------			--------------
2006.07.05	Lothson				v1.00				Initial Creation
2006.09.07	Lothson				v1.01				Change logging feature
******************************************************************************/

#ifndef __DRM_CONFIG_H
#define __DRM_CONFIG_H

/*
����������������������������������������������������������������������������������������������
		THIS FILE CONTAINS VARIOUS KINDS OF COMPILATION OPTIONS (DEBUGGING)
		
			CHANGING THESE VALUES COULD AFFECT TO LGE DRM CORE.
����������������������������������������������������������������������������������������������
*/

/*
����������������������������������������������������������������������
		INCLUDE
����������������������������������������������������������������������
*/

#include "DRM_Define.h"

/*
����������������������������������������������������������������������
		DEFINE
����������������������������������������������������������������������
*/

/* �������������������� [SUPPORTED SOLUTION SOURCE] �������������������� */

/*
	Define whether Current Engine has source codes for each solution
*/
 
/** 
  @ingroup	Common_CFG
  @brief	  DRM Agent has OMA DRM solution source code */ 
#define FEATURE_OMA_DRM

/** 
  @ingroup	Common_CFG
  @brief	  DRM Agent has CFM DRM solution source code */ 
#define FEATURE_CFM_DRM

#if defined(FEATURE_CFM_DRM) && !defined(FEATURE_OMA_DRM)
	#error "Must enable OMA to use CFM"
#endif

/** 
  @ingroup	Common_CFG
  @brief	  DRM Agent has Windoes Media DRM solution source code */ 
//#define FEATURE_WM_DRM

/** 
  @ingroup	Common_CFG
  @brief	  DRM Agent has PlayReady DRM solution source code */ 
//#define FEATURE_PLAYREADY

#if defined(FEATURE_WM_DRM) && defined(FEATURE_PLAYREADY)
	#error "LGDRM BUILD ERROR : WMDRM and PLAYREADY features are not available in concurrence"
#endif

#if defined(FEATURE_WM_DRM) && DRM_SUPPORT_DLA
/*
	DRM_SUPPORT_DLA is Compilation Directive of WMDRM PD.
	Enabling DLA features from LGE WMDRM Interface is managed by DRM_SUPPORT_DLA.

	Lothson.20080304 Don't change DRM_SUPPORT_DLA as defined(DRM_SUPPORT_DLA).
	DRM_SUPPORT_DLA is always on but has value 0 or 1.

	Lothson.20090415
	Disable Camese permanently.
	Camese will not be used anymore, but some of the DLA features are used in VZW operators.
*/
//#define FEATURE_WM_DRM_CAMESE	/* WMDRM PD Janus + Camese */
#endif

/** 
  @ingroup	Common_CFG
  @brief	  DRM Agent has DivX DRM solution source code of DivX SDK version 1.2 */
//#define FEATURE_DIVX_DRM12

/** 
  @ingroup	Common_CFG
  @brief	  DRM Agent has DivX DRM solution source code of DivX SDK version 1.3 */
//#define FEATURE_DIVX_DRM13

#if defined(FEATURE_DIVX_DRM12) || defined(FEATURE_DIVX_DRM13)
#define FEATURE_DIVX_DRM	/* DIVX DRM Solution */
#endif

/** 
  @ingroup	Common_CFG
  @brief	  DRM Agent has Broadcast DRM Profile solution source code */ 
//#define FEATURE_BCAST_DRM	/* OMA DRM v2.0 Extension for Broadcast Support */

/** 
  @ingroup	Common_CFG
  @brief	  DRM Agent has OMA SRM solution source code */
//#define FEATURE_OMA_SRM


/* �������������������� [SUPPORTED PLATFORM] �������������������� */
 
/** 
  @ingroup	Common_CFG
  @brief	  Base platform is EMP */
//#define DRM_PLATFORM_EMP

/** 
  @ingroup	Common_CFG
  @brief	  Base platform is QCT EURO */
//#define DRM_PLATFORM_QCT_EURO

/** 
  @ingroup	Common_CFG
  @brief	  Base platform is QCT */
//#define DRM_PLATFORM_QCT_US

/** 
  @ingroup	Common_CFG
  @brief	  Base platform is Limo */
//#define DRM_PLATFORM_LIMO

/** 
  @ingroup	Common_CFG
  @brief	  Base platform is Android */
#define DRM_PLATFORM_ANDROID

#if defined(DRM_PLATFORM_LIMO) || defined(DRM_PLATFORM_ANDROID)
#define DRM_PLATFORM_LINUX
#endif

/** 
  @ingroup	Common_CFG
  @brief	  Base platform is Windows Mobile */ 
//#define DRM_PLATFORM_WNDM

/** 
  @ingroup	Common_CFG
  @brief	  Base platform is ASDP */ 
//#define DRM_PLATFORM_ASDP

/** 
  @ingroup	Common_CFG
  @brief	  Base platform is Infineon */ 
//#define DRM_PLATFORM_INFI

/** 
  @ingroup	Common_CFG
  @brief	  Base platform is MediaTek (MTK) */
//#define DRM_PLATFORM_MTK

/** 
  @ingroup	Common_CFG
  @brief	  Base platform is Symphony */ 
/*
	This define might be used over platforms.
	So it will be used with other platform defines.
*/
//#define DRM_PLATFORM_SYM

/** 
  @ingroup	Common_CFG
  @brief	  Base platform is Windows PC */ 
/*
	This define might be used over platforms.
	So it will be used with other platform defines.
*/
//#define DRM_PLATFORM_WIN_PC

/** 
  @ingroup	Common_CFG
  @brief	  Base platform is WISE 1.2 */
//#define DRM_WISE_12

/** 
  @ingroup	Common_CFG
  @brief	  Platform OS is Qualcomm L4 */
#if defined(DRM_PLATFORM_QCT_EURO) || defined(DRM_PLATFORM_QCT_US)
//#define DRM_OS_L4
#endif

/** 
  @ingroup	Common_CFG
  @brief	  Select low level I/O function */
#if defined(DRM_PLATFORM_QCT_EURO) || defined(DRM_PLATFORM_QCT_US)
//#define DRM_PAL_FS
#endif

/** 
  @ingroup	Common_CFG
  @brief	  DRM agent is running under multiple process environment */
#if defined(DRM_PLATFORM_WNDM)
#define DRM_MP_ENV
#endif

/** 
  @ingroup	Common_CFG
  @brief	  DRM agent is running under multiple thread environment */
#if defined(DRM_PLATFORM_WNDM) || defined(DRM_PLATFORM_ANDROID)
#define DRM_MT_ENV
#endif


/* �������������������� [NOTIFY DRM RIGHTS UPDATE] �������������������� */
 
/** 
  @ingroup	Common_CFG
  @brief	  When there are changes in rights status, notfity related application through an event */ 
#if !(defined(DRM_PLATFORM_LIMO) || defined(DRM_PLATFORM_WNDM) || defined(DRM_PLATFORM_ASDP))
/* Limo, Windows Mobile, ASDP does not support Notification */
#define DRM_NOTIFY_RIGHTS
#endif


/* �������������������� [KEY ENCRYPT] �������������������� */
 
/** 
  @ingroup	Common_CFG
  @brief	  DRM Agent sends encrypted CEK to Multimedia Codec */
#define DRM_KEY_ENCRYPT

/** 
  @ingroup	Common_CFG
  @brief	  DRM Agent sends additional information with decryption information to Multimedia Codec */ 
#define DRM_DEC_META_INFO


/* �������������������� [UNICODE FUNCTION] �������������������� */

/** 
  @ingroup	Common_CFG
  @brief	  Platform supports unicode string functions */
//#define DRM_USE_PLATFORM_UNIFUNC


/* �������������������� [SOLUTION DETECTION BY EXTENSION] �������������������� */

/* == Apply strict extesion check == */
/** 
  @ingroup	Common_CFG
  @brief	  Detect DRM solution based on file extension */
#if !defined(DRM_PLATFORM_WNDM)
#define DRM_APPLY_STRICT_EXT
#endif


/* �������������������� [FILE SYSTEM ADOPTATION] �������������������� */
 
/** 
  @ingroup	Common_CFG
  @brief	  Initial position */ 
typedef enum {
	DRM_FS_SEEK_SET = 0,	/**< Beginning of file */
	DRM_FS_SEEK_CUR,			/**< Current position of DRM file handle */
	DRM_FS_SEEK_END				/**< End of file */
} drm_fs_seek_orign_type;

/** 
  @ingroup	Common_CFG
  @brief	  File access mode */ 
#if !defined(DRM_PLATFORM_EMP)
/*
	According to platform supports, some of the open mode will not be supported.
	To support drm engine, at least, the following modes must be supported.
		Read/Write, Read only, Write only, Create.

	Append mode should not be used inside DRM Engine and application.

	Preview modes are reserved for future use.
*/
typedef enum _DRMOpenMode{
	DRM_FS_READWRITE = 0,	/**< Open file for reading and writing */
	DRM_FS_READONLY,			/**< Open file for reading only */
	DRM_FS_WRITEONLY,			/**< Open file for write only */
 	DRM_FS_APPEND,				/**< Obsolete */
	DRM_FS_CREATE,				/**< Create file */
	DRM_FS_PREVIEW				/**< Reserved */
} enDRMOpenMode;
#else
typedef enum _DRMOpenMode{
	DRM_FS_READWRITE	= 0x0001,
	DRM_FS_READONLY		= 0x0002,
	DRM_FS_WRITEONLY	= 0x0004,
	DRM_FS_APPEND			= 0x0008,
	DRM_FS_CREATE			= 0x0010,
	DRM_FS_PREVIEW 		= 0x0100
} enDRMOpenMode;
#endif


/* �������������������� [Content storage root path] �������������������� */

#if defined(DRM_PLATFORM_EMP)

/**
	@ingroup	Common_CFG
	@brief		Root path of removable media card */
#define DRM_MMC_ROOT_PATH		DRM_T("/card")

#elif defined(DRM_PLATFORM_QCT_EURO) || defined(DRM_PLATFORM_QCT_US)
#define DRM_MMC_ROOT_PATH		DRM_T("/mmc1")

#elif defined(DRM_PLATFORM_LIMO)
/* Lothson. Exceptional case. Don't refer this code */
#ifndef LIMO_DRM_EXTERNAL_STORAGE_ROOT_DIR
#error LIMO_DRM_EXTERNAL_STORAGE_ROOT_DIR is not defined!
#endif
#define DRM_MMC_ROOT_PATH		DRM_T(LIMO_DRM_EXTERNAL_STORAGE_ROOT_DIR)

#elif defined(DRM_PLATFORM_ANDROID)
/*
	Cupcake ~ Eclair
	 - Mount position of MMC is /sdcard.
	 - DRM_MMC_ROOT_PATH			DRM_T("/sdcard")
	 - DRM_MMC_ROOT_LINK_PATH	disable
	Froyo ~
	 - Mount position of MMC is /mnt/sdcard and /sdcard is symbolic link to /mnt/sdcard.
	 - DRM_MMC_ROOT_PATH			DRM_T("/mnt/sdcard")
	 - DRM_MMC_ROOT_LINK_PATH	DRM_T("/sdcard")
*/
/* Real mount position. */
#define DRM_MMC_ROOT_PATH				DRM_T("/sdcard")
/* Symbolic link position */
//#define DRM_MMC_ROOT_LINK_PATH	DRM_T("/sdcard")

#elif defined(DRM_PLATFORM_WNDM)
#define DRM_MMC_ROOT_PATH		DRM_T("/Storage Card")

#elif defined(DRM_PLATFORM_ASDP)
#define DRM_MMC_ROOT_PATH		DRM_T("mmc1")

#elif defined(DRM_PLATFORM_INFI)
#define DRM_MMC_ROOT_PATH		DRM_T("/mmc1")

#elif defined(DRM_PLATFORM_MTK)
#define DRM_MMC_ROOT_PATH		DRM_T("/mmc1")
#endif


/* �������������������� [LGDRM Root path] �������������������� */

#if defined(DRM_PLATFORM_EMP) || defined(DRM_PLATFORM_QCT_EURO) || defined(DRM_PLATFORM_QCT_US)
/**
	@ingroup	Common_CFG
	@brief		Internal storage path which will be used by DRM agent */

/* If support S/W upgrade through FOTA, use inerasable path instead */
//#define DRM_INERASABLE_FOTA_PATH

#if defined(DRM_INERASABLE_FOTA_PATH)
#define DRM_ROOT_PATH	"/LGAPP/Application/DRM"
#else
#define DRM_ROOT_PATH	"/LGAPP/DRM"
#endif //DRM_INERASABLE_FOTA_PATH

#elif defined(DRM_PLATFORM_LIMO)
/* Lothson. Exceptional case. Don't refer this code */
#ifndef LIMO_DRM_ROOT_DIR
#error LIMO_DRM_ROOT_DIR is not defined!
#endif
#define DRM_ROOT_PATH	DRM_T(LIMO_DRM_ROOT_DIR"/lgapp.drm")

#elif defined(DRM_PLATFORM_ANDROID)
#define DRM_ROOT_PATH	"/data/lgdrm"

#elif defined(DRM_PLATFORM_WNDM)
#define DRM_ROOT_PATH	"/Windows/DRM"

#elif defined(DRM_PLATFORM_ASDP)
#define DRM_ROOT_PATH	"LGDRM"

#elif defined(DRM_PLATFORM_INFI)
#define DRM_ROOT_PATH	"/LGAPP/DRM"

#elif defined(DRM_PLATFORM_MTK)
#define DRM_ROOT_PATH	"/LGAPP/DRM"
#endif

#define DRM_ROOT_UNI(A)	DRM_T(DRM_ROOT_PATH) DRM_T(A)
#define DRM_ROOT(A)			DRM_ROOT_PATH A


/* �������������������� [Certificates path] �������������������� */

#if defined(DRM_PLATFORM_EMP)
/**
	@ingroup	Common_CFG
	@brief		Product certificate path of OMA DRM v2.x */
#define DRM_CERT_PROD_PATH	"/LGDRM"			/* Files which are loaded during mass-production */

/**
	@ingroup	Common_CFG
	@brief		Addoitiona information path of certificates */
#define DRM_CERT_INFO_PATH	"/LGAPP/DRM"	/* Files which are pre-loaded, supplied by image download */

#elif defined(DRM_PLATFORM_QCT_EURO) || defined(DRM_PLATFORM_QCT_US)
/*
	If DRM_CERTLOAD_DRMPARTITION is enabled,
	then DRM_CERT_PROD_PATH will be used to support auto loading and test certificates.
*/
#define DRM_CERT_PROD_PATH	"/LGAPP/DRM/CERT/CERT"
#define DRM_CERT_INFO_PATH	"/LGAPP/DRM"

#elif defined(DRM_PLATFORM_LIMO)
// TODO:
#define DRM_CERT_PROD_PATH	"/LGAPP/DRM/CERT/CERT"
#define DRM_CERT_INFO_PATH	"/LGAPP/DRM"

#elif defined(DRM_PLATFORM_ANDROID)
#define DRM_CERT_PROD_PATH	"/lgdrm"
#define DRM_CERT_INFO_PATH	"/system/media/lgdrm"

#elif defined(DRM_PLATFORM_WNDM)
// TODO:
#define DRM_CERT_PROD_PATH	"/Windows/DRM/CERT/CERT"
#define DRM_CERT_INFO_PATH	"/Windows/DRM"

#elif defined(DRM_PLATFORM_ASDP)
#define DRM_CERT_PROD_PATH	"LGDRM"
#define DRM_CERT_INFO_PATH	"LGDRM"

#elif defined(DRM_PLATFORM_INFI)
#define DRM_CERT_PROD_PATH	"/LGAPP/DRM/CERT/CERT"
#define DRM_CERT_INFO_PATH	"/LGAPP/DRM"

#elif defined(DRM_PLATFORM_MTK)
#define DRM_CERT_PROD_PATH	"/LGAPP/DRM/CERT/CERT"
#define DRM_CERT_INFO_PATH	"/LGAPP/DRM"
#endif

#define DRM_CERT_PROD_UNI(A)	DRM_T(DRM_CERT_PROD_PATH) DRM_T(A)
#define DRM_CERT_RROD(A)			DRM_CERT_PROD_PATH A

#define DRM_CERT_INFO_UNI(A)	DRM_T(DRM_CERT_INFO_PATH) DRM_T(A)
#define DRM_CERT_INFO(A)			DRM_CERT_INFO_PATH A


/* �������������������� [DYNAMIC CONFIGURATION OF OPERATOR AND SOLUTION] �������������������� */

#define DRM_SOLUTION_CFG_PATH	DRM_ROOT_UNI("/Solution.cfg")
#define DRM_OPERATOR_PATH			DRM_ROOT_UNI("/Operator.cfg")

#define DRM_CERT_DBRESET_UTF8	DRM_ROOT("/DRM_DBOP_RESET.CMD")
#define DRM_CERT_DBRESET			DRM_ROOT_UNI("/DRM_DBOP_RESET.CMD")


/* �������������������� [DEBUG CONFIGURATION (HEAP TESTING)] �������������������� */
 
/** 
  @ingroup	Common_DBG_CFG
  @brief	  Enable debugging feature for heap memory */
//#define DRM_DEBUG_HEAP

#ifdef DRM_DEBUG_HEAP
//#define DRM_DEBUG_HEAP_SSL
//#define DRM_DEBUG_HEAP_LIBXML
//#define DRM_DEBUG_HEAP_ISAM
//#define DRM_DEBUG_HEAP_LIBWBXML

/* Free openssl and libxml's static heap */
#define DRM_DEBUG_HEAP_FREESSL

/* Microsoft DRM heap debug */
//#define DRM_DEBUG_HEAP_WM
//#define DRM_DEBUG_HEAP_MSDRM

#define DRM_DEBUG_HEAP_SQLITE
#define DRM_DEBUG_HEAP_EXT
#endif


/* �������������������� [DEBUG  Performance] �������������������� */
 
/** 
  @ingroup	Common_DBG_CFG
  @brief	  Enable debugging log for performance */
#define DRM_DEBUG_APIPERFORMANCE


/* �������������������� [Debug Configuration] �������������������� */
 
/** 
  @ingroup	Common_DBG_CFG
  @brief	  Enable logging */ 
#define DRM_LOG_ENABLE

/** 
  @ingroup	Common_DBG_CFG
  @brief	  Enable error logging */ 
#define DRM_ERRORLOG_ENABLE

/** 
  @ingroup	Common_DBG_CFG
  @brief	  Enable all debugging features @n If disable this, all debugging features are disable except logging */
#define DRM_SUPPORT_DBG


/* ==== WMDRM Start ==== */
#define DRM_HIDDEN_ERROR_LOG		/* write error code - 2007.10.04. kybae */
//#define DRM_DEBUG_USEDLA			DRM_ROOT_UNI("/ALL_DLA.txt")	/* WMDRM Not use WAP for download */
/* ==== WMDRM End ==== */

#ifdef DRM_SUPPORT_DBG
#define DRM_SEED_DUMPFILE			DRM_ROOT_UNI("/SEED.txt")
#define DRM_IMSI_DUMPFILE			DRM_ROOT_UNI("/IMSI.txt")

/* MTP, BT Debugging */
//#define DRM_DEBUG_FILELOG			DRM_ROOT_UNI("/DRMLOG.txt")

//#define DRM_DEBUG_BY_PERF		/* Performance log on a file */
#define DRM_PERF_FILE					DRM_ROOT_UNI("/Perf.dat")
#endif // DRM_SUPPORT_DBG


//3 			Only for supporting PC Emulator
/* �������������������� [DRM SECURE STORAGE PATH & PREFIX CONFIGURATION] �������������������� */

#define DRM_ID_PATH 					DRM_ROOT("/ID")
#define DRM_IMSI_FILE					"IMSI"
#define DRM_IMEI_FILE					"IMEI"


/* �������������������� [Preloaded content on MMC] �������������������� */

/**
	@ingroup	Common_CFG
	@brief		Support preloaded forward-lock content */
#ifdef FEATURE_OMA_DRM
//#define DRM_SUPPORT_PRELOAD
#endif

/**
	@ingroup	Common_CFG
	@brief		Support packaging a content as preloaded form */
#ifdef DRM_SUPPORT_PRELOAD
#define DRM_SUPPORT_PRELOAD_PKG
#endif

/**
	@ingroup	Common_CFG
	@brief		Bind preloaded content on a specific model */
#ifdef DRM_SUPPORT_PRELOAD
#define DRM_PRELOAD_BIND
#endif

#endif

