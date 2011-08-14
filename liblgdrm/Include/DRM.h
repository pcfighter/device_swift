//1 			Source Editing Rule		
/*
	1. For category:
	收收收收收收收收收收 [Data base Identifier] 收收收收收收收收收收
	use 20 收 symbols for pre & postfix
	use square bracket for each category
	
	2. For section delimeter
	use 70 收 symbols for Section Category 
	收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	
*/

/* 收收收收收收收收收收 [Data] 收收收收收收收收收收 */


/*
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
		INCLUDE
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
*/

/*
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
		DEFINE
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
*/

/*
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
		GLOBAL FUNCTION PROTOTYPE
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
*/

/*
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
		INTERNAL FUNCTION PROTOTYPE
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
*/

/*
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
		GLOBAL FUNCTION DEFINITION
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
*/

/*
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
		INTERNAL FUNCTION DEFINITION
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
*/

/******************************************************************************
* Developed by	: LGE
* Security			: Confidential
* Module				: LG DRM
* Filename			: DRM.h
* Purpose				: LG DRM Interfaces
* Author(s)			: Lothson
* e-mail				: lothson@lge.com
* Creation date	: 5th July 2006
* Edit History	: Modification description(s)
Date				Name					Version			Description
---------		-------				------			--------------
2006.07.05	Lothson				v1.00				Initial Creation
2007.06.11	Lothson				v2.00				Apply WM DRM
2008.02.08	Lothson				v2.10				Apply OMA DRM v2.1
******************************************************************************/

#ifndef _DRM_H_
#define _DRM_H_

/*
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	THIS FILE CONTAINS VARIOUS KINDS OF ENUM AND STRUCTURE DEFINITIONS.
	THESE DEFINITIONS WILL BE USED FOR LG DRM INTERFACE.
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
*/


/*
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
		INCLUDE
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
*/

#include "DRM_Config.h" //DRMHDRMERGE "../Adapt/Include/DRM_Config.h"
#include "DRM_Error.h" //DRMHDRMERGE "../DrmComm/DRM_Error.h"

/*
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
		DEFINE
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
*/

/* 收收收收收收收收收收 [DRM Solution] 收收收收收收收收收收 */

typedef enum _DRMCFG
{
	eDRM_Solution_OMA_DRM_v1	= 0x00000001,		/* OMA DRM v1.0 */
	eDRM_Solution_OMA_DRM_v2	= 0x00000002,		/* OMA DRM v2.0 */
	eDRM_Solution_CFM					= 0x00000004,		/* CFM */
	eDRM_Solution_WMDRM				= 0x00000008,		/* WMDRM PD Janus */
	eDRM_Solution_WMDRMCAMESE	= 0x00000010,		/* WMDRM PD Janus + Camese */
	eDRM_Solution_OMA_DRM_v2_1= 0x00000020,		/* OMA DRM v2.1 */
	eDRM_Solution_DIVXDRM			= 0x00000040,		/* Divx DRM */
	eDRM_Solution_BCASTDRM		= 0x00000080,		/* OMA BCAST DRM Profile */
	eDRM_CERT_CMLA						= 0x00000100,		/* [OMA] Support CMLA certificate */
	eDRM_CERT_FranceTelecom		= 0x00000200,		/* [OMA] Support ORG certificate */
	eDRM_CERT_VeriSign				= 0x00000400,		/* [OMA] Support VeriSign certificate */
	eDRM_CERT_CMLA_DEVROOT		= 0x00000800,		/* [OMA] Support CMLA development root certificate */
	eDRM_CERT_LGE							= 0x00001000,		/* [OMA] Support LGE certificate */
	eDRM_Solution_PLAYREADY		= 0x00002000,		/* PLAYREADY */
	eDRM_Solution_SRM					= 0x00004000,		/* OMA SRM */
} enDRMCFG;


typedef enum _DRMSolutionType
{
	eDRM_SL_UNKNOWN =0,	/* Unknown */	
	eDRM_SL_OMA1,				/* OMA DRM v1.0 */
	eDRM_SL_OMA2,				/* OMA DRM v2.x */
	eDRM_SL_CFM,				/* CFM DRM */
	eDRM_SL_LS,					/* Lockstream DRM */
	eDRM_SL_WM,					/* MS WMDRM */
	eDRM_SL_DIVX,				/* Divx DRM */
	eDRM_SL_PLAYREADY,	/* MS PlayReady DRM */
	eDRM_SL_SRM,				/* OMA SRM */
	eDRM_SL_ALL					/* All DRM agent */
} enDRMSolutionType;


/* 收收收收收收收收收收 [ROAP Request through ROAP CLIENT] 收收收收收收收收收收 */

/* Request from OMA2 Agent to ROAP CLIENT*/
typedef enum _DRM_LocalProcReq
{
	eDRM_LOCPROC_REQ_NONE			= 0x0000,
	eDRM_LOCPROC_REQ_ROAP			= 0x0001,
	eDRM_LOCPROC_REQ_PREVIEW	= 0x0002,
	eDRM_LOCPROC_REQ_SILENT 	= 0x0004,
	eDRM_LOCPROC_REQ_GET 			= 0x0008,
	eDRM_LOCPROC_REQ_PENDRO		= 0x0010,
	eDRM_LOCPROC_REQ_EMBEDRO	= 0x0020,
	eDRM_LOCPROC_REQ_ROUPLOAD = 0x0100,
	eDRM_LOCPROC_REQ_ONEXPIRED= 0x0200
} enDRM_LocalProcReq;


/* 收收收收收收收收收收 [DRM OBJECT PROCESSING] 收收收收收收收收收收 */

/* == DRM_ProcessInit() Attributes */
/* 0x0001xxxx, right 2byte xxxx is used by Browser. Therefore we only have to use left 2byte */
/* For multiple attributes, the attribute value is exclusive for each other */

/** 
  @ingroup	Object_Process_DT
  @brief		Attributes of current download session */ 
typedef enum _DRMProcAttr
{
	/* == OMA CFM START == */
	eDRM_PROC_ATTR_PRELOAD		= 0x00000001,		/**< Package a content to preload on MMC */
	eDRM_PROC_ATTR_NO_FW			= 0x00010000,		/**< [CFM] Don't forward download content */
	eDRM_PROC_ATTR_NO_RINGTONE= 0x00020000,		/**< [CFM] Don't use downloaded content as rigntone */
	eDRM_PROC_ATTR_NO_SCREEN	= 0x00040000,		/**< [CFM] Don't use downloaded content as wallpaper */
	eDRM_PROC_ATTR_NO_ENCRYPT	= 0x00080000,		/**< Obsolete */
	eDRM_PROC_ATTR_MULTIPART	= 0x00100000,		/**< Obsolete */
	eDRM_PROC_ATTR_IMPORT			= 0x00200000,		/**< Obsolete */
	/* == OMA CFM END == */
	
	/* == MSDRM & OMA START == */
	eDRM_PROC_ATTR_WAP				= 0x00400000,		/**< [WM,OMA,PR] Object will be downloaded through Download agent of Browser */
	eDRM_PROC_ATTR_WAPPUSH		= 0x00800000,		/**< [WM,OMA,PR] Object will be downloaded through WAP Push */
	/* == MSDRM & OMA START == */
	
	/* == MSDRM START == */
	eDRM_PROC_ATTR_SOAP				= 0x01000000,		/**< [PR] SOAP message */
	eDRM_PROC_ATTR_DLA				= 0x02000000,		/**< [WM,PR] Object will be downloaded through Direct License Acquisition client */
	eDRM_PROC_ATTR_CLKSYNCPET	= 0x04000000,		/**< [WM,PR] Response for petition url request */
	eDRM_PROC_ATTR_CLKSYNCRESP= 0x08000000,		/**< [WM,PR] Response for secure clock response */
	/* == MSDRM END == */
	
	/* == OMA DRM v2.0 START == */		/* Should Be Matched enDRMROAPCallerType!!! */
	eDRM_PROC_ATTR_DLD				= 0x10000000,		/**< [OMA] Object will be downloaded through ROAP Client */
	eDRM_PROC_ATTR_BCAST			= 0x40000000		/**< [BCAST] Object will be downloaded through BCAST Client */
	/* == OMA DRM v2.0 END == */
} enDRMProcAttr;


/* == DRM_ProcessStatus() USER CONSENT == */
/** 
  @ingroup	Object_Process_DT
  @brief		Detailed reason for requesting user's confirmation */ 
typedef enum _DRMUserConsent
{
	/* Registration */
	eDRM_CONSENT_REGISTRATION = 1,	/**< [OMA] Device registration */

	/* Domain Related */
	eDRM_CONSENT_DOMAIN_NOTMEMBER,	/**< [OMA] Not a domain member */
	eDRM_CONSENT_DOMAIN_UPGRADE,		/**< [OMA] Domain upgrade */
	eDRM_CONSENT_DOMAIN_RENEWAL,		/**< [OMA] Domain renewal */
	eDRM_CONSENT_LEAVED_NOTMEMBER,	/**< [OMA] Not a domain member */

	/* Silent or Preview */
	eDRM_CONSENT_SILENT,						/**< [OMA] Unreliable silent url */
	eDRM_CONSENT_PREVIEW,						/**< [OMA] Unreliable preview url */

	/* OMA DRM 2.1 Extend LeavDomain Trigger */
	eDRM_CONSENT_EXT_LEAVED,				/**< [OMA] Extended leave domain */
	eDRM_CONSENT_EXT_ROUPLOAD				/**< [OMA] Extended ro uploading */
} enDRMUserConsent;


/* == DRM_ProcessEnd() User's response */
/** 
  @ingroup	Object_Process_DT
  @brief		Status of user's approval */ 
typedef enum _DRMProcStatus
{
	/* User response */
	eDRM_PROC_COMMIT = 0x00,	/**< No error. Apply all changes */
	eDRM_PROC_AGREED,					/**< User agreed internal processing of DRM Agent */
	eDRM_PROC_DISAGREED,			/**< User disagreed internal processing of DRM Agent */

	/* Failure */
	eDRM_PROC_CANCEL,					/**< Failure was occurred during object processing */
	eDRM_PROC_MAX
} enDRMProcStatus;


/* == Precise failure reason of stDRMProcFail */
/** 
  @ingroup	Object_Process_DT
  @brief		Detailed reason of object processing failure */ 
typedef enum _DrmProcFail
{
	eDRM_PROCFAIL_UNKNOWN = 0,		/**< Unknown failure */
	eDRM_PROCFAIL_DOMAINFULL,			/**< Can't add more device in device network */
	eDRM_PROCFAIL_ROAP,						/**< ROAP failure. Show status or error message of #stDRMProcFail */
	eDRM_PROCFAIL_RO_CORRUPT,			/**< Corrouped rights object. Only ONA DRM 1.0 */
	/* MSDRM */
	eDRM_PROCFAIL_SOAPFAULT,			/**< PlayReady SOAPFault case. Show error message or Redirect URL of #stDRMProcFail */
	eDRM_PROCFAIL_OUT_OF_STORAGE,	/**< No more available space in file system to store */
} enDrmProcFail;


/* == Precise failure reason of DRM_ProcessEnd() */
/** 
  @ingroup	Object_Process_DT
  @brief		Detailed reason of object processing failure */
typedef struct _DRMProcFail
{
	enDrmProcFail eReason;		/**< Failure reason */
	
	/* Only for roap messge */
	unsigned char szStatus[DRM_ANYURI_LEN+1];			/**< [OMA, PR] Reason of failure */
	unsigned char szErrorMsg[DRM_ANYURI_LEN+1];		/**< [OMA, PR] Detailed description of failure */
	unsigned char szRedirectURL[DRM_ANYURI_LEN+1];/**< [OMA, PR] Redirect URL for getting more information */
} stDRMProcFail;

/* == DRM_ProcessEnd()'s return value */
/** 
  @ingroup	Object_Process_DT
  @brief		Return value of DRM_ProcessEnd() */ 
typedef enum _DRMPEndStatus
{
	eDRM_PROCEND_ERROR			= -1,
	/**< Failure */

	/* == Success condition == */
	eDRM_PROCEND_END 				= 0,
	/**< Complete object downloading @n - If have rights, try to render */

	/* ============== Only OMA DRM START ============== */
	eDRM_PROCEND_ROAP				= 5,
	/**< [OMA] Complete ROAP (Registration, Join domain, Member Leave domain ) @n - Show only Completed Popup*/
	eDRM_PROCEND_NOTDMEMBER	= 7,
	/**< [OMA] Successful Leave domain but user is not domain member @n - Show Not Device Network Member Popup */	
	eDRM_PROCEND_RO					= 10,
	/**< [OMA] End of RO download @n - If filename is available, show rendering popup */

	//eDRM_PROCEND_PREVIEWRO		= 15,
	/* End of preview RO download - Terminate download agent */
	/* ============== Only OMA DRM END ============== */
	
	eDRM_PROCEND_WAITREQ		= 20,	
	/**< Wait next request event from DRM Agent to download agent @n - Show waiting popup */
	
	eDRM_PROCEND_WAITCNTREQ	= 30,
	/**< [WM,PR] Wait a request for content download from DRM Agent to download agent */
	eDRM_PROCEND_WAITALBUMARTREQ	= 40
	/**< [WM,PR] Wait a request for album art download from DRM Agent to download agent */
} eDRMPEndStatus;


/* 收收收收收收收收收收 [Drm Type] 收收收收收收收收收收 */

/** 
  @ingroup	Object_Process_DT
  @brief		Category of DRM object */ 
typedef enum  _DRMFormatType
{	
	eDRM_FORMAT_UNKOWN = -1,	/**< Failure */
	eDRM_FORMAT_NOTDRM, /**< Not DRM */
	eDRM_FORMAT_FL,			/**< [OMA, 1.0] Forward locked Content*/
	eDRM_FORMAT_FLSD,		/**< [OMA, 1.0] Forward locked separate delivery content */
	eDRM_FORMAT_CD,			/**< [OMA, 1.0] Conbined Delivery content */
	eDRM_FORMAT_DCF,		/**< [OMA, 1.0] Separate Delivery content (DCF) */
	eDRM_FORMAT_DR,			/**< [OMA, 1.0] Rights Object encoded into XML */
	eDRM_FORMAT_DRC,		/**< [OMA, 1.0] Rights Object encoded into WBXML */
	eDRM_FORMAT_ODF,		/**< [OMA, 2.0] DCF Content */
	eDRM_FORMAT_ORO,		/**< [OMA, 2.0] Protected Rights Object */
	eDRM_FORMAT_ROAP,		/**< [OMA, 2.0] ROAP PDU */
	eDRM_FORMAT_DRM2,		/**< [OMA, 2.0] Object wrapped in multipart/related */
	eDRM_FORMAT_CFM			/**< Obsolete. CFM content */
}enDRMFormatType; 

/** 
  @ingroup	Object_Process_DT
  @brief		Type of DRM object information */ 
typedef enum _DRMObjectInfo
{
	eDRM_OBJINFO_MIME = 1,	/**< Mandatory. Mime type of media object */
	eDRM_OBJINFO_FILENAME		/**< Optional. Filename parameter */
} enDRMObjectInfo;


/* 收收收收收收收收收收 [Modue RO Waiting] 收收收收收收收收收收 */
 
/** 
  @ingroup	Object_Process_DT
  @brief		Application identifier. */ 
typedef enum _DRMModuleID
{
	/* eMODULE_DRMAGENT is loop control value. Do not change init value */
	eMODULE_DRMAGENT =1,/**< DRM Agent */
	eMODULE_IV,					/**< Image viewer */
	eMODULE_FM,					/**< File manager */
	eMODULE_MYMUSIC,		/**< Music Library */
	eMODULE_WAP,				/**< Browser */
	eMODULE_JAVA,				/**< Java */
	eMODULE_MMS_DOWN, 	/**< MMS CommonComposer */
	eMODULE_MMS_VIEW, 	/**< MMS MediaManager */
	eMODULE_MM,					/**< Multimedia player */
	eMODULE_EMAIL,			/**< E-Mail */
	eMODULE_BT,					/**< Bluetooth */
	eMODULE_MTP,				/**< MTP application */
	eMODULE_PCSYNC,			/**< PC Sync */
	eMODULE_GALLERY,		/**< Gallery */
	eMODULE_FMUM,				/**< Update manager of file manager */
	eMODULE_MAX					/* Used for loop control value. Do not remove */
} enModuleID;

 
/** 
  @ingroup	Object_Process_DT
  @brief		Waiting type */ 
typedef enum _DRMWaitingType
{
	eDRM_WAIT_ROPUSH,		/**< Waiting for WAP Push of OMA DRM 1.0 Rights object */
	eDRM_WAIT_ROAP,			/**< OMA DRM 2.0 Roap waiting */
	eDRM_WAIT_PREVIEW,	/**< OMA DRM 2.0 Preview Right */
	eDRM_WAIT_ROUPLOAD,	/**< OMA DRM 2.1 RO Upload */
	eDRM_WAIT_TIMESYNC,	/**< DRM Time Synchronization */
	eDRM_WAIT_ONEXPIRED,/**< OMA DRM 2.1 OnExpiredURL */
	
	eDRM_WAIT_RIGHTS_MOVE,			/**< SRM Rights Move to SRM Agent */
	eDRM_WAIT_RIGHTS_RETRIEVE,	/**< SRM Rights Retrieval from SRM Agent */
	eDRM_WAIT_RIGHTS_LOCAL,			/**< SRM Local Rights Consumption */
	eDRM_WAIT_MAX
} enDRMWaitingType;

 
/** 
  @ingroup	Object_Process_DT
  @brief		Result of waiting */ 
typedef enum _DRMWaitResult
{
	eDRM_WAIT_SUCCESS = 1,			/**< Success */
	eDRM_WAIT_CONNERR,					/**< Failure because of connection error */
	eDRM_WAIT_FAIL,							/**< Unknow or internal error */
	eDRM_WAIT_INSUFFICIENT_MEM	/**< No free space in file system */
} enDRMWaitResult;

/* Event for DRM_SetModuleWaiting() */
 
/** 
  @ingroup	Object_Process_DT
  @brief		Waiting result of additional(background) processing by DRM Agent */ 
typedef struct _DRMModuleResult
{
	enDRMWaitResult eResult;		/**< Waiting result */
	enDRMWaitingType eWaitType;	/**< Waiting type */
	
	int nRequestID;							/**< Unique id for this waiting */
	DRM_UCHAR ausFilename[DRM_FILE_PATH_LEN+1];	/**< Related file name*/
	int nIndx;									/**< [OMA] Index of container */

	/* If failure of ROAP (only for OMA DRM2.0) */
	unsigned char szStatus[DRM_ANYURI_LEN+1];			/**< [OMA] Reason of failure */
	unsigned char szErrorMsg[DRM_ANYURI_LEN+1];		/**< [OMA] Detailed description of failure */
	unsigned char szRedirectURL[DRM_ANYURI_LEN+1];/**< [OMA] Redirect URL for getting more information */
} stDRMModuleResult, *pstDRMModuleResult;


/* 收收收收收收收收收收 [ROAP Request through DLD] 收收收收收收收收收收 */
 
/** 
  @ingroup	Object_Process_DT
  @brief		HTTP Request Type. */ 
typedef enum _DRMHttpMethod
{
	eDRM_HTTP_GET  = 0x10,		/**< Do http GET throguth Download agent */
	eDRM_HTTP_POST = 0x20			/**< Do http POST throguth Download agent */
} enDRMHttpMethod;

 
/** 
  @ingroup	Object_Process_DT
  @brief		Detailed state of current request. */ 
typedef enum _DRMDLDState
{
	eDRM_REQ_SEND							= 0x00000001,	/**< Need http interaction */
	eDRM_REQ_END							= 0x00000002,	/**< Received final message (Obselete) */

	/* ==== START WM DRM PD ==== */
	eDRM_REQ_LICENSE					= 0x00000004,	/**< [WM] License request */
	eDRM_REQ_CNTDWN						= 0x00000008,	/**< [WM] Content download */
	eDRM_REQ_ALBMART					= 0x00000010,	/**< [WM] Album art download */
	eDRM_REQ_LICNOTIFY				= 0x00000020,	/**< [WM] Install notify for License */
	eDRM_REQ_ACTIVATION				= 0x00000040,	/**< [WM] Activation of a content */
	/* Internal only */
	eDRM_REQ_METER						= 0x00000100,	/**< [WM, DLA Only] Metering request */
	eDRM_REQ_CLKSYNCPET				= 0x00000200,	/**< [WM, DLA Only] Petition URL request for Clock sync */
	eDRM_REQ_CLKSYNCRESP			= 0x00000400,	/**< [WM, DLA Only] Requst Secure clock response */
	eDRM_REQ_WAPPUSH_SL				= 0x00000800,	/**< [WM, DLA Only] Push URL for Silent renewal or metering */
	/* ==== END WM DRM PD ==== */

	/* ==== START PLAYREADY ==== */
	eDRM_DLA_REQ_INITIATOR		= 0x00001000,	/**< [PR] Not Used*/
	eDRM_DLA_REQ_LICENSE_ACQ	= 0x00002000,	/**< [PR] License Acquisition */
	eDRM_DLA_REQ_LICENSE_ACK	= 0x00004000,	/**< [PR] License Acknowledge */
	eDRM_DLA_REQ_JOINDOMAIN		= 0x00008000,	/**< [PR] Join Domain */
	eDRM_DLA_REQ_LEAVEDOMAIN	= 0x00010000,	/**< [PR] Leave Domain */
	eDRM_DLA_REQ_METERING			= 0x00020000,	/**< [PR] Metering Report */
	eDRM_DLA_REQ_METERCERT		= 0x00040000,	/**< [PR] Metering certificate */
	eDRM_DLA_REQ_ACTIVATION		= 0x00080000,	/**< [PR] Activation */
	eDRM_DLA_REQ_ALBUMART			= 0x00100000,	/**< [PR] Albumart */
	eDRM_DLA_REQ_CNTDWN				= 0x00200000,	/**< [PR] Content download */
	/* ==== END PLAYREADY ==== */
} enDRMDLDState;

/** 
  @ingroup	Object_Process_DT
  @brief		Event to send Download Agent. */ 
typedef struct _DrmDLDReq {
	enDRMSolutionType eSLType;	/**< Solution type */
	enDRMDLDState eState;				/**< Request type */
	void *pvUsrData;	/**< User data of Download agent */
	void *pvDrmSes;		/**< DRM Session Information */

	/* == For WMDRM Activation START == */
	enModuleID eModuleID;		/**< [WM, PR] Application id which requested content activation */
	DRM_TSTR pusFilename;		/**< [WM, PR] Full path of a DRM content which will be activated */
	/* == For WMDRM Activation END == */
	
	enDRMHttpMethod eHttpMethod;	/**< HTTP Method */	
	unsigned char *pszURL;				/**< Request URL */
	unsigned char *pszSendBuf;		/**< Data for HTTP Post */
	unsigned int uBufLen;					/**< Data length in byte */
} stDRMDLDReq;

/** 
  @ingroup	Object_Process_DT
  @brief		Failure reason form session destroy. */
typedef enum _DRMReqError
{
	eDRM_REQERR_NONE = eDRM_PROC_MAX + 1,//=0, Lothson.20070721 changed. To receive exact reason form DLA, ROAP Client
	eDRM_REQERR_HTTP404,	/**< Http 404 Not Found */
	eDRM_REQERR_CONNECT,	/**< Http Errors except 404 */
	eDRM_REQERR_INTERNAL, /**< Internal processing failure */
	eDRM_REQERR_CLEAN			/**< Clean all drm sessions when Error page, (x)HTML, was delivered in the middle of download */
} enDRMReqError;

/* Internal activation data of DRM agent */
typedef struct _DRMActivationReq
{
	DRM_UCHAR ausURL[DRM_ANYURI_LEN+1];
} stDRMActivationReq;

/* Internal object processing data of DRM agent. */
typedef struct _DRMUserData
{
	unsigned char szReceiver[DRM_ID_LEN+1];
	stDRMModuleResult *pstWaitResult;
	stDRMDLDReq *pstNextReq;
} stDRMUserData;


/* 收收收收收收收收收收 [Drm Content Type] 收收收收收收收收收收 */

/** 
  @ingroup	Content_Usage_DT
  @brief		Content type */ 
typedef enum  _DRMContentType
{	
	eDRM_CONTENT_NOTDRM	= 0,						/**< Not DRM */

	/* Property */
	eDRM_CONTENT_NOFW 	= 0x00000001,		/**< Property : can't forward this content */
	eDRM_CONTENT_WARNFW	= 0x00000002,		/**< Property : can forward with user warning */
	
	eDRM_CONTENT_DM			= 0x00000010,		/**< Category : DRM message format (DM) */
	eDRM_CONTENT_FL 		= 0x00000031,		/**< Foward lock */
	eDRM_CONTENT_CD			= 0x00000051,		/**< Combined delivery */

	eDRM_CONTENT_DCF		= 0x00000100,		/**< Category : DRM content format version 1 (DCF) */
	eDRM_CONTENT_SD			= 0x00000300,		/**< Separate delivery */
	eDRM_CONTENT_SDWARN	= 0x00000302, 	/**< Separate delivery without rights issuer url */
	eDRM_CONTENT_FLSD		= 0x00000301,		/**< Separate delivery wrapped in DRM message foramt */

	eDRM_CONTENT_ODF		= 0x00001000,		/**< Category : DRM Content Format Version 2 (DCFv2 or PDCF) */
	eDRM_CONTENT_DCFV2	= 0x00003000,		/**< DCFv2 */
	eDRM_CONTENT_PDCF		= 0x00005000,		/**< PDCF */

	eDRM_CONTENT_CFM 		= 0x00008001,		/**< Obsolete. CFM DRM */

	eDRM_CONTENT_WMDRM	= 0x00010000,		/**< Category : Windows Media Drm */
	eDRM_CONTENT_WMA		= 0x00030000,		/**< Windows media Audio Content */
	eDRM_CONTENT_WMV		= 0x00050000,		/**< Windows media Video Content */

	eDRM_CONTENT_DIVX		= 0x00080000,		/**< DivX DRM */
	
	eDRM_CONTENT_PR_PMF	= 0x00100000,		/**< Category : PlayReady Media Format (PMF) */
	eDRM_CONTENT_PR_PYA	= 0x00300000,		/**< PlayReady Media Format Audio Content */
	eDRM_CONTENT_PR_PYV	= 0x00500000,		/**< PlayReady Media Format Video Content */
	eDRM_CONTENT_PR_ENY	= 0x00800000		/**< PlayReady Enveloped File Format Content */
}enDRMContentType;	


/* 收收收收收收收收收收 [Rights Related] 收收收收收收收收收收 */

/* 
	Permission 
	Whenever change enDRMPermission (add new, change sequence), 
	enDRMPermissionSeq must be also changed properly.
*/
/** 
  @ingroup	Content_Usage_DT
  @brief		Permission type */ 
typedef enum  _DRMPermission
{
	eDRM_PERMISSION_NONE 				= 0x00,		/* For child RO (internal only) */
	eDRM_PERMISSION_PLAY 				= 0x01,		/**< Permission to play */
	eDRM_PERMISSION_DISPLAY 		= 0x02,		/**< Permission to display */
	eDRM_PERMISSION_EXECUTE 		= 0x04,		/**< Permission to execute */
	eDRM_PERMISSION_PRINT 			= 0x08,		/**< Permission to print */
	eDRM_PERMISSION_EXPORT_MOVE	= 0x10,		/**< Permission to export (moving) */
	eDRM_PERMISSION_EXPORT_COPY	= 0x20,		/**< Permission to export (copying) */
	eDRM_PERMISSION_EXPORT 			= 0x30,		/**< Permission to export */
	eDRM_PERMISSION_ACCESS 			= 0x40,		/**< [BCAST] Permission to access */
	eDRM_PERMISSION_MOVE 				= 0x80,		/**< [SRM] Permission to move */
	eDRM_PERMISSION_ANY 				= 0xFF		/* Used for DB query only (internal only) */
}enDRMPermission;


/** 
  @ingroup	Content_Usage_DT
  @brief		Remnant rights information */ 
typedef struct _DRMRemnantInfo
{
	unsigned int uRemCount;	/**< Remnant count */
	unsigned int uOrgCount;	/**< Original count */
	unsigned int uRemSecs;	/**< [OMA] Remnant acculated constraint in seconds */
	unsigned int uOrgSecs;	/**< Original acculated constraint in seconds */

	/* OMA DRM 2.1 */
	unsigned char szRiID[DRM_RIID_LEN+1];			/**< [OMA] RiID to update Metering consent DB */
	unsigned char szRiAlias[DRM_STR80_LEN+1];	/**< [OMA] RiAlias or FQDN of RiURL. To request metering consent */
}stDRMRemnantInfo;


typedef struct _WMDRMModuleData
{
	int bSuccess;
	DRM_TSTR pusFilename;
}stWMDRMModuleData;

/** 
  @ingroup	Content_Usage_DT
  @brief		Rights Status */ 
typedef enum _DRMROStatus
{
	eDRM_RIGHTS_MEMSTOREFULL = -3,
	/**< No more space in file system to store information. Only for DRM_ConsumeRights() */
	eDRM_RIGHTS_EXPIRED = -2,
	/**< Rights was expired in the middle of consuming. Only for DRM_ConsumeRights() */
	eDRM_RIGHTS_VALID = 0,
	/**< Valid Rights */
	eDRM_RIGHTS_INVALID = 1,
	/**< No Rights */
	eDRM_RIGHTS_INVALIDDRO = 2,
	/**< [OMA] Expired Domain Rights */
	eDRM_RIGHTS_INVALIDSIM = 3,
	/**< [OMA] Rights is bound to another USIM */
	eDRM_RIGHTS_NOTREADY = 4,
	/**< Not ready to use time based constraint */
	
	/* Preview or Silent */
	eDRM_RIGHTS_TRUSTEDURL = 5,
	/**< [OMA] Trusted url for preview or silent */
	eDRM_RIGHTS_UNRELIABLEURL = 6,
	/**< [OMA] Unreliable URL for preview or silent */
	eDRM_RIGHTS_ROAMINGURL = 7,
	/**< [OMA] Preview or silent url exist. But device is nunder roaming network */

	eDRM_RIGHTS_SYNCCLK = 8,
	/**< Need secure clock synchronization */
	eDRM_RIGHTS_METERCONSENT = 9,
	/**< [OMA] To use this content, need user's consent for metering */
	eDRM_RIGHTS_METERCONSENTORUSE = 10,
	/**< [OMA]  Content can be used with metering consent. @n But without consent, it can be rendered */
	eDRM_RIGHTS_ONEXPIREDURL = 11,
	/**< [OMA] OnExpiredURL exist. */
	
	eDRM_RIGHTS_UNAUTHORIZED = 12,
	/**< [DivX] Content is not yet authorized */

	eDRM_RIGHTS_CORRUPTED = 13,
	/**< [WM, PR] HDS file was corrupted. Need to delete the HDS file */
	eDRM_RIGHTS_HDS_MISMATCH = 14,
	/**< [WM, PR] HDS file mismatches with the current certificate file. */
	eDRM_RIGHTS_HDS_NOT_EXIST = 15
	/**< [WM, PR] HDS file doesn't exist */
} enDRMROStatus;

/* For DRM_GetRightInfo() */
/** 
  @ingroup	Managements_DT
  @brief		Rights information */ 
typedef struct _DRMROInfo
{
	/* Summary of rights information */
	int bUnlimited;		/**< Unlimited constraint flag */
	unsigned char szUseLeft[DRM_INT_ROINFO_LEN+1];		/**< uRemantCount / uTotalCount (uSec for time count) */
	unsigned char szValidFor[DRM_INT_ROINFO_LEN+1];		/**< uDay day uHour hour uMin min uSec sec */
	
	/*Rights informaiton in detail */
	enDRMPermission ePermType;	/**< Permission */

	int bMetering;				/**< Metering flag */
	unsigned char szMID[DRM_RIID_LEN+1]; /**< Unique metering ID */
	
	int bPreview;					/**< Preview flag */
	int bUnConstrainted;	/**< Unlimited Constraint Flag */
	unsigned char szCount[DRM_INT_ROINFO_LEN+1];				/**< uCount; uRemnantCount */
	unsigned char szTimedCount[DRM_INT_ROINFO_LEN+1];		/**< uTimed; uTimedCount; uRemnantTimedCount  */
	unsigned char szDateTime[DRM_INT_ROINFO_LEN+1];			/**< uDatetimeBegin; uDatetimeEnd */
	unsigned char szInterval[DRM_INT_ROINFO_LEN+1];			/**< uInterval; uIntervalBegin; uIntervalEnd */
	unsigned char szAccumulated[DRM_INT_ROINFO_LEN+1];	/**< uAccumulated; nRemnantAccumulated */
	unsigned char szIndv[DRM_INDV_CONSTRAINT_COUNT*(DRM_IMSI_LEN+1)];			/**< Individual */
	unsigned char szSys[DRM_SYS_CONSTRAINT_COUNT*(DRM_OMNA_NAME_LEN+1)];	/**< System */
} stDRMROInfo;

/* General structure of LG DRM Constraint (Rights List Management) */
 
/** 
  @ingroup	Managements_DT
  @brief		Constraint information */ 
typedef struct _DRMConstraintNode
{
	int iConstraintType;							/**< Constraint type of this record */
	
	unsigned int uCount;							/**< Original count */
	unsigned int uRemnantCount;				/**< Current count which is not consumed yet */

	unsigned int uDatetimeBegin;			/**< Start time of Datetime */
	unsigned int uDatetimeEnd;				/**< End time of Datetime */

	unsigned int uInterval;						/**< Original Interval time(sec) */
	unsigned int uIntervalBegin;			/**< Start time of Interval constraint */
	unsigned int uIntervalEnd;				/**< End time of Interval constraint */

	unsigned int uTimed;							/**< Time(sec) of  Timedcount */
	unsigned int uTimedCount;					/**< Original count of Timedcount */
	unsigned int uRemnantTimedCount;	/**< Current count of Timedcount which is not consumed yet */

	unsigned int uAccumulated;				/**< Original Accumulated time(sec) */
	int nRemnantAccumulated;					/**< Accumulated time which is not consumed yet */

	unsigned char aszIndividual[DRM_INDV_CONSTRAINT_COUNT][DRM_IMSI_LEN+1];		/**< individual */
	unsigned char aszSystem[DRM_SYS_CONSTRAINT_COUNT][DRM_OMNA_NAME_LEN+1];		/**< system */
} stDRMConstraintNode;

/* General structure of LG DRM Right (Rights List Management) */
 
/** 
  @ingroup	Managements_DT
  @brief		Rights information */ 
typedef struct _DRMRightNode
{
	int nRightID;			/**< Unique Identifier for Rights DB */
	int bPreview;			/**< Flag for preview (Obsolete) */
	
	char bDomainRO;		/**< [OMA] Flag for domain rights */
	unsigned char szDBaseID[DRM_DOMAINBASEID_LEN+1];	/**< [OMA] Domain Base ID */

	unsigned char szID[DRM_ID_LEN+1];					/**< Unique identifier for matched content (ex, ContentID, GroupID or SubscriptionID ) */
	unsigned char szInheritID[DRM_ID_LEN+1];	/**< Parent ID (ex, SubscriptionID ) */
	unsigned char szSIMID[DRM_IMSI_LEN+1];		/**< IMSI which bount to this right */

	int iPermissionType;								/**< Permission type */
	stDRMConstraintNode stConstraints;	/**< Detailed information about constraint */
} stDRMRightNode;


/** 
  @ingroup	Managements_DT
  @brief		Import result */
typedef enum _DRMImportStatus
{
	eDRM_IMPORT_UNKNOWN = 0,			/**< Unknow status */
	eDRM_IMPORT_SUCCESS,					/**< Succeed to import */
	eDRM_IMPORT_CONTEXT_REQUIRED,	/**< Context required to process Rights objects */
	eDRM_IMPORT_PROCESSED_NOTHING	/**< Nothing to process */
} enDRMImportStatus;


/* 收收收收收收收收收收 [DRM Content Usage] 收收收收收收收收收收 */

/* DRM Ecryption method */
typedef enum  _DRMEncMethod
{
	eDRM_ENC_NULL							= 0x00,
	eDRM_ENC_AES128CBC				= 0x01, 	// padding RFC2630
	eDRM_ENC_AES128CBC_PLAIN	= 0x02,
	eDRM_ENC_AES128CTR 				= 0x03,		// padding none
	eDRM_ENC_AES128ECB				= 0x04,		// padding zeroing	
	eDRM_ENC_BLOWFISH					= 0X05
}enDRMEnc;

/** 
  @ingroup	Content_Usage_DT
  @brief		Decryption information */
typedef struct _LGDrm_Info
{
	int nDrmType;											/**< DRM Type. */
	unsigned int uDataStartPos;				/**< Object Start Position */
	
	unsigned int uOrgCntSize;					/**< Plain Text Length */
	unsigned int uEncCntSize;					/**< Encrypted Content Size */

	int nEncMethod;										/**< Encryption method */
	unsigned char Key [DRM_CEK_LEN];	/**< Encryption key */

#ifdef DRM_KEY_ENCRYPT
	unsigned char ucSystemId[DRM_CEK_LEN];	/**< Key Encrpypt & Decrpypt */
#endif
	/* lothson 2006.09.05 To support cmx and Qtv with minimum change */
	unsigned char ucBuf[DRM_CEK_LEN];		/**< Remnant buffer for block handling */
	unsigned int uBufLen;								/**< Remnant buffer length*/
	unsigned char ucVEC[DRM_CEK_LEN];		/**< Initialization vector */	

#ifdef DRM_DEC_META_INFO
	unsigned char szMimeType[DRM_MIME_TYPE_LEN+1];	/**< Mime type of media object */
	DRM_UCHAR pcMediaExtension[DRM_EXT_LEN+1];			/**< Extension of media object */
#endif
}stLGDrmInfo;

/** 
  @ingroup	Content_Usage_DT
  @brief		Decryption information */
typedef struct _LGDivXDrm_Info
{
	unsigned char	ucBaseKey[DRM_DIVX_BASEKEY_LEN];	/**< Base Key */
	unsigned char ucUserKey[DRM_DIVX_USERKEY_LEN];	/**< User key */	
}stLGDivXDrmInfo;

typedef struct _LGZoranDrm_Info
{
	int nDrmType;
	void *pstKey;
}stLGZoranDrmInfo;

/* kybae 2009.03.06 - WMDRM structure size for decryption info, refer to WMDrm_Interface.c  */
#ifdef FEATURE_WM_DRM
/**
  @ingroup	Content_Usage_DT
  @brief		Byte length of Windoes Media DRM's decryption information */
extern int DRM_WM_DEC_INFO_SIZE;
#endif

/* yanggak 2009.06.11 - MSDRM structure size for decryption info, refer to PlayReady_Interface.c  */
#ifdef FEATURE_PLAYREADY
/**
  @ingroup	Content_Usage_DT
  @brief		Byte length of PlayReady DRM's decryption information */
extern int DRM_MS_DEC_INFO_SIZE;
#endif

/* Only for Java */
typedef struct _stDRMJavaHnd
{
	/* Never change following sequenct for flag and file handle */
	unsigned char bIsDRMfile;		/* (0, normal), (1, DRM), (2, PRGP) */
	void *fileHandle;
} stDRMJavaHnd;


/* 收收收收收收收收收收 [DRM Content Info] 收收收收收收收收收收 */
 
/** 
  @ingroup	Content_Usage_DT
  @brief		Maximum content info in an array */ 
#define DRM_MAX_CNT_INFO_LIST	12	
 
/** 
  @ingroup	Content_Usage_DT
  @brief		Maximum length of each content information */ 
#define DRM_MAX_CNT_INFO	DRM_ID_LEN

/** 
  @ingroup	Content_Usage_DT
  @brief		Type of additional information about a DRM content 
					@n _S_ : Support only single request. Casting as DRM_TSTR
					@n _M_ : Support multiple request. Casting as #stDRMCntInfoList */ 
typedef enum _DRMCntInfoType
{
	eDRM_S_CNTINF_CID 				= 0x00001,		/**< [OMA] Content ID */
	eDRM_S_CNTINF_EXT					= 0x00002,		/**< Extension */
	eDRM_S_CNTINF_MIME				= 0x00003,		/**< MIME */
	eDRM_S_CNTINF_RIURL				= 0x00004,		/**< [OMA] Right Issuer URL */
	eDRM_S_CNTINF_CNTURL			= 0x00005,		/**< [OMA] DCFv2 Content URL */
	eDRM_S_CNTINF_TTID				= 0x00006,		/**< [OMA] Transaction ID */
	eDRM_S_CNTINF_MEDIATYPE		= 0x00007,		/**< Media type. Refer #enDRMMediaType */
	eDRM_S_CNTINF_CGMSASIGNAL	= 0x00008,		/**< [DivX] Only for DIVX DRM */	
	eDRM_S_CNTINF_KID					= 0x00009,		/**< [WM,PR] Key ID of DRM Header */
	eDRM_S_CNTINF_CHKPREVIEW	= 0x00010,		/**< [OMA] Check preview and silent status */
	eDRM_S_CNTINF_MAX,											/* Dummy value. Separator */
	
	/* Meta Info */
	eDRM_M_CNTINF_TITLE				= 0x00020,		/**< [OMA] DCFv1 Content-Name, DCFv2 Tile */
	eDRM_M_CNTINF_DESC				= 0x00040,		/**< [OMA] Description */
	eDRM_M_CNTINF_COPYR				= 0x00080,		/**< [OMA] DCFv1 Content Vendor, DCFv2 Copy Rights */
	eDRM_M_CNTINF_PERF				= 0x00100,		/**< [OMA] DCFv2 Performer or artist */
	eDRM_M_CNTINF_AUTHOR			= 0x00200,		/**< [OMA] DCFv2 Author */
	eDRM_M_CNTINF_GNRE				= 0x00400,		/**< [OMA] DCFv2 Category and style */	
	eDRM_M_CNTINF_CLSF				= 0x00800,		/**< [OMA] DCFv2 Classification */
	eDRM_M_CNTINF_ALBM				= 0x01000,		/**< [OMA] DCFv2 Album title */
	eDRM_M_CNTINF_YRRC				= 0x02000,		/**< [OMA] DCFv2 recording year for the media */
	eDRM_M_CNTINF_COVERURI		= 0x04000,		/**< [OMA] DCFv2 CoverURI */
	eDRM_M_CNTINF_LYRICS			= 0x08000,		/**< [OMA] DCFv2 Lyrics */
	eDRM_M_CNTINF_TRACK				= 0x10000,		/**< [OMA] DCFv2 Track number */
#if 0 /* MM will not support this. Lothson. 2006.12.18 */
	eDRM_M_CNTINF_RTNG				= 0x70,				/**< [OMA] DCFv2 media rating */
	eDRM_M_CNTINF_KYWD				= 0x90,				/**< [OMA] DCFv2 media keyword */	
	eDRM_M_CNTINF_LOCI				= 0xA0,				/**< [OMA] DCFv2 location information */
#endif
	eDRM_M_CNTINF_BASIC				= 0x20000,		/**< Internal only. Retrive basic information at once */
	eDRM_M_CNTINF_MAX,											/* Dummy value. Separator */

	eDRM_U_CNTINF_UPDATE			= 0x40000,		/**< [OMA] Update meta data */
	eDRM_U_CNTINF_CLEAN				= 0x80000,		/**< [OMA] Clean user edited meta data */
	eDRM_U_CNTINF_MAX												/* Dummy value. Separator */
} enDRMCntInfo;

 
/** 
  @ingroup	Content_Usage_DT
  @brief		Content information */ 
typedef struct _DRMCntInfo
{
	enDRMCntInfo eType;	/**< Content information Type */
	DRM_UCHAR ausCntInfo[DRM_MAX_CNT_INFO+1];	/**< Content information */
} stDRMCntInfo;

 
/** 
  @ingroup	Content_Usage_DT
  @brief		Content information about multiple request */ 
typedef struct _DRMCntInfoList
{
	int nCount;	/**< Totoal number of content info in astCntInfo array */
	stDRMCntInfo astCntInfo[DRM_MAX_CNT_INFO_LIST]; 	/**< Array of content information */
} stDRMCntInfoList;

/*
	- !! CAUTION !! -
	Data size is 5KB. Must not use STACK VARIABLE for this structure

	Not support metadata editing for CVRU, LRCU, CLSF, CPRT
*/ 
/** 
  @ingroup	Content_Usage_DT
  @brief		Meta data updaing */ 
typedef struct _DRMMetaData
{
	DRM_UCHAR ausTitle[DRM_MAX_CNT_INFO+1];				/**< Content name or title */
	DRM_UCHAR ausDescription[DRM_MAX_CNT_INFO+1];	/**< Description */
	DRM_UCHAR ausPerformer[DRM_MAX_CNT_INFO+1];		/**< Performer or artist */
	DRM_UCHAR ausAuthor[DRM_MAX_CNT_INFO+1];			/**< Author */
	DRM_UCHAR ausGnre[DRM_MAX_CNT_INFO+1];				/**< Category and style */
	DRM_UCHAR ausAlbm[DRM_MAX_CNT_INFO+1];				/**< Album title */
	DRM_UCHAR ausYrrc[DRM_MAX_CNT_INFO+1];				/**< Recording year */
	DRM_UCHAR ausTracknum[DRM_MAX_CNT_INFO+1];		/**< Track number */
} stDRMMetaData;

/** 
  @ingroup	Content_Usage_DT
  @brief		Content information about preview and silent */ 
typedef enum _DRMCntPreviewInfo
{
	eDRM_CNT_PREVIEW_INSTANCE	= 0x01,		/**< [OMA] Preview instance */
	eDRM_CNT_PREVIEW_URL			= 0x02,		/**< [OMA] Preview rights url */
	eDRM_CNT_SILENT_ONDEMNAD	= 0x04,		/**< [OMA] Silent on-demand url */
	eDRM_CNT_SILENT_INADVANCE	= 0x08		/**< [OMA] Silent in-advance url */
} enDRMCntPreviewInfo;


/** 
  @ingroup	Content_Usage_DT
  @brief		Basic content's information */ 
typedef struct _DRMCntInfoBasic
{
	DRM_UCHAR ausExt[DRM_EXT_LEN+1];				/**< Extension of media object */
	DRM_UCHAR ausMime[DRM_MIME_TYPE_LEN+1];	/**< Mime type of media object */
	int nMediaType;	/* Media type of media object */
} stDRMCntInfoBasic;

/* 收收收收收收收收收收 [DRM Content List in OMA DRMv2.0 DCFv2] 收收收收收收收收收收 */

/** 
  @ingroup	Content_Usage_DT
  @brief		Maximum Media objects(Original content) in a DCFv2 content */ 
#define DRM_MAX_MOS_IN_ODF	5
 
/** 
  @ingroup	Content_Usage_DT
  @brief		Maximum length of Media object name */ 
#define DRM_MAX_MO_NAME			128

/* DCFv2 content list */
 
/** 
  @ingroup	Content_Usage_DT
  @brief		Media object information */ 
typedef struct _DRMMediaObject
{
	int nIndex; 			/**< Index inside DCFv2 content */
	int nMediaType;		/**< Media object type */
	DRM_UCHAR ausMime[DRM_MIME_TYPE_LEN+1];			/**< Mime type */
	DRM_UCHAR ausExt[DRM_EXT_LEN+1];						/**< Extension */	// 2006.09.07 lothson. By module's request
	DRM_UCHAR ausMediaName[DRM_MAX_MO_NAME+1];	/**< Media object name (Original content name) */
} stDRMMediaObject;
 
/** 
  @ingroup	Content_Usage_DT
  @brief		List of media object information */ 
typedef struct _DRMMOList
{
	int nCount;		/**< Totoal number of Media objects in astMO array */
	stDRMMediaObject astMO[DRM_MAX_MOS_IN_ODF];		/**< Array of Media object information */
} stDRMMOList;


/* 收收收收收收收收收收 [Auto Rendering Type] 收收收收收收收收收收 */

/** 
  @ingroup	Managements_DT
  @brief		Action type for DRM_IsSettingsAvailable() */ 
typedef enum _DRMAutoRender
{
	eDRM_SETTING_FORWARD = 1,	/**< Can forward */
	eDRM_SETTING_RINGTONE,		/**< Can be used for ringtone */
	eDRM_SETTING_SCREEN,			/**< Can be used for wallpaper */
	eDRM_SETTING_TVOUT				/**< Can be used for video output */
} enDRMAutoRender;


/* 收收收收收收收收收收 [DRM Support Information] 收收收收收收收收收收 */

/** 
  @ingroup	Managements_DT
  @brief		Information about DRM agent */ 
typedef struct _DRMSupport
{
	unsigned char szDrmLibVersion[DRM_DRMLIBVERSION_LEN+1];	/**< S/W version of DRM agent */
	unsigned char szModtime[DRM_MODTIME_LEN+1];	/**< Date and Time of last modification */

	/* OMA DRM */
	unsigned int uOmaVersion;									/**< [OMA] OMA version (v1.0, 1), (v2.0, 2), (v2.1, 3) */
	unsigned char bForwardLockSupport;				/**< [OMA] True if Forward Lock is supported */
	unsigned char bCombinedDeliverySupport;  	/**< [OMA] True if Combined Delivery is supported */
	unsigned char bSeparateDeliverySupport;		/**< [OMA] True if Separate Delivery is supported */
	unsigned char bDCFSupport;								/**< [OMA] True if Version 2.0 DCF is supported */
	unsigned char bForwardLockIconVisible;		/**< [OMA] True if Forward Lock's DRM icon should be visible */
	
	/* OMA BCAST DRM Profile */
	unsigned char bBCASTSupport;							/**< [BCAST] True if OMA BCAST is supported */

	/* CFM DRM */
	unsigned char bCFMSupport;								/**< [CFM] True if CFM  is supported */

	/* DIVX DRM */
	unsigned char bDIVXSupport;								/**< [DivX] True if DivX DRM  is supported */

	/* Windows Media DRM */
	unsigned int uWMVersion;									/**< [WM] WMDRM version. (Janus, 1), (Camese, 2) */
	unsigned char bWMDRMSupport;							/**< [WM] True if Version of WMDRM  is supported */

	/* PlayReady DRM */
	unsigned int	uPRVersion;									/**< [PR] PLAYREADY version. */
	unsigned char  bPRDRMSupport;							/**< [PR] True if Version of PLAYREADY is supported */

	/* OMA SRM */
	unsigned char bSRMSupport;								/**< [OMA SRM] True if OMA SRM is supported */
}stDRMSupport;


/* 收收收收收收收收收收 [General DRM DB] 收收收收收收收收收收 */

/** 
  @ingroup	Managements_DT
  @brief		Database table type */ 
typedef enum _DRMDBTblType
{
	eDRM_DBT_RIGHTS,		/**< [OMA] Rights table */
	eDRM_DBT_ROWAIT,		/**< [OMA] RO waiting table */
	eDRM_DBT_RICTX,			/**< [OMA] Rights issuer context table */
	eDRM_DBT_DOMAINCTX,	/**< [OMA] Domain context table */
	eDRM_DBT_CERT,			/**< [OMA] Certificate table */
	eDRM_DBT_ROID,			/**< [OMA] Rights objects ID table */
	eDRM_DBT_REPLAY,		/**< [OMA] Replay protection table */
	eDRM_DBT_PRO,				/**< [OMA] Protected rights object table */
	eDRM_DBT_PENDRO,		/**< [OMA] Pended rights object table */
	eDRM_DBT_DWLIST,		/**< [OMA] Domain name white list table */
	
	/* OMA DRM 2.0 */
	eDRM_DBT_METCNST,		/**< [OMA] Metering consent table */
	eDRM_DBT_MET,				/**< [OMA] Metering table */
	/* OMA BCAST DRM Profile */
	eDRM_DBT_BCASTRO,		/**< [OMA] OMA BCAST DRM Profile table */

	/* OMA SRM */
	eDRM_DBT_SRMSAC,		/**< [SRM] Secure Authenticated Channel table */
	eDRM_DBT_SRMRMETA,	/**< [SRM] Rights Meta Data table */
	eDRM_DBT_OMAMAX,
	
	/* WMDRM */
	eDRM_DBT_HDS,				/**< [WM,PR] WMDRM license table - HDS */
	eDRM_DBT_MAX
} enDRMDBTblType;

 
/** 
  @ingroup	Managements_DT
  @brief		Operation code for DRM_ManageDB() */ 
typedef enum _DRMDBManage
{
	eDRM_DBOP_RESET,								/**< Reset all DB. (Factory reset). */
	eDRM_DBOP_DCTX_ALL_DELETE,			/**< [OMA] Reset domain context DB */
	eDRM_DBOP_RICTX_ALL_DELETE,			/**< [OMA] Reset rights issuer context DB */
	eDRM_DBOP_RIGHTS_ALL_DELETE,		/**< [OMA] Reset DBs which are related to rights object */
	eDRM_DBOP_METER_ALL_RESET,			/**< [OMA] Reset DBs which are related to rights object and metering */
	eDRM_DBOP_BCASTRO_ALL_DELETE,		/**< [BCAST] Delete rights which are bound to USIM */
	eDRM_DBOP_BCASTRO_EXPIRED_DEL,	/**< [BCAST] Delete expired bcast rights */
	eDRM_DBOP_INIT_HDS,							/**< [WM,PR] Check HDS Initialize */
	eDRM_DBOP_CLOSE_HDS,						/**< [WM,PR] Close HDS */
	eDRM_DBOP_CLOSE_ALL_DB,					/**< Close all DB */

	/* OMA DRM 2.1 */
	eDRM_DBOP_METER_OK,							/**< [OMA] Enable metering for this RI */
	eDRM_DBOP_METER_NO,							/**< [OMA] Disable metering for this RI */
	eDRM_DBOP_METER_LIST,						/**< [OMA] Get metering consent list */
	eDRM_DBOP_METER_CHECK,					/**< [OMA] Check whether metering is enabled or not */

	eDRM_DBOP_MAX
} enDRMDBManage;

/** 
  @ingroup	Managements_DT
  @brief		DBMS type */ 
typedef enum _DRMDBMSType
{
	eDRM_DBMS_NONE = 0,
	eDRM_DBMS_ISAM,		/**< PBL ISAM DBMS */
	eDRM_DBMS_LGE,		/**< LGE DBMS */
	eDRM_DBMS_SQL,		/**< SQL DBMS */
	eDRM_DBMS_MAX
} enDRMDBMSType;

/*
	2006.12.15 lothson
	!!!!! should remove this part form L650V
	!!!!! EMP should not merge this part
*/
/* Y.G L650v DRMv2 Porting 2006.11.12 (dependency - lg_oem_snd_java.c) */

typedef enum
{
	eDRM_ERROR		= -1,	// error
	eDRM_NONDRM		= 0,	// not DRM content, maybe normal content to play
	eDRM_INVALID	= 1,	// LG DRM content, Invalid content
	eDRM_VALID		= 2		// LG DRM content, Valid content
}DRMIDENTITY;


/* 收收收收收收收收收收 [ODF Manager WParam] 收收收收收收收收收收 */

typedef enum _DRMODFMngrCmd
{
	eDRM_ODFMNGR_REQ_IMPORT = 0
} enDRMODFMngrCmd;


/* 收收收收收收收收收收 [Drm Media Type] 收收收收收收收收收收 */
 
/** 
  @ingroup	Content_Usage_DT
  @brief		Category of media object */ 
typedef enum  _DRMMediaType
{	
	eDRM_MEDIA_UNKNOWN = 0,
	eDRM_MEDIA_IMAGE,	/**< Image content */
	eDRM_MEDIA_AUDIO,	/**< Audio content */
	eDRM_MEDIA_VIDEO,	/**< Video content */
	eDRM_MEDIA_GAME,	/**< Game content */
	eDRM_MEDIA_FLASH,	/**< Flash content */
	eDRM_MEDIA_SVG		/**< SVG content */
}enDRMMediaType;	


/* 收收收收收收收收收收 [Drm Init OpCode] 收收收收收收收收收收 */

/** 
  @ingroup	Initialization_DT
  @brief		Operation code for DRM_Init(). */ 
typedef enum  _DRMInitOpcode
{
	eDRM_INIT_AGENT,	/**< Initialize DRM agent during boot-up process */
	eDRM_INIT_DEINIT,	/**< De-initialize DRM agent */
	eDRM_INIT_SELF,		/**< Initialize DRM agent by Agent itself */
	eDRM_INIT_MAX
}enDRMInitOpcode;


/* 收收收收收收收收收收 [Drm Manage Cert OpCode] 收收收收收收收收收收 */
 
/** 
  @ingroup	Managements_DT
  @brief		Operation code for DRM_ManageCERT() */ 
typedef enum  _DRMManageCERTOpcode
{
	eDRM_CERT_MANAGE_CHECK = 0,				/**< Validate certificate chain and return issuer's organization name of certificate (At command) */
	eDRM_CERT_MANAGE_ERASE,						/**< Erase all certificates and previate key (At command) */
	eDRM_CERT_MANAGE_WRITE_ROOT,			/**< Save root certificate in secure area (At command) */
	eDRM_CERT_MANAGE_WRITE_DEVCA,			/**< Save certificate authority certificate in secure area (At command) */
	eDRM_CERT_MANAGE_WRITE_DEV,				/**< Save device certificate in secure area (At command) */
	eDRM_CERT_MANAGE_WRITE_DEVPKEY,		/**< Save device private key in secure area (At command) */
	eDRM_CERT_MANAGE_IS_TESTCERT,			/**< Check whether loaded certficiate set is for testing */
	eDRM_CERT_MANAGE_GET_ISSUERNAME,	/**< Get issuer's organization name of loaded certficate */
	eDRM_CERT_MANAGE_GET_CERTTYPE,		/**< Get intended issuer's organization name of current SW version, not loaded certificate (At command) */
	eDRM_CERT_MANAGE_LOADING_CERT_FROM_INDEX, /**< Match IMEI and load certificates from selected index */
	eDRM_CERT_MANAGE_SET_INDEX,				/**< Store index information of certificates (At command) */
	eDRM_CERT_MANAGE_GET_INDEX,				/**< Get index information of certificates (At command) */
	eDRM_CERT_MANAGE_MAX
}enDRMManageCERTOpcode;


/* 收收收收收收收收收收 [Metering User Consent List] 收收收收收收收收收收 */

/**
	@ingroup	Managements_DT
	@brief		Maximum list of metering consent list. */
#define DRM_MAX_METERCONSENT 5

/**
	@ingroup	Managements_DT
	@brief		Metering consent information. */
typedef struct _DRMMeterCnst
{
	int bEnable; /**< Flag for enabled metering */
	unsigned char szRiID[DRM_RIID_LEN+1]; 		/**< Rights issuer identifier */
	unsigned char szRiAlias[DRM_STR80_LEN+1];	/**< Alias of Rights issuer identifier */
}stDRMMeterCnst;

/**
	@ingroup	Managements_DT
	@brief		Metering consent list. */
typedef struct _DRMMeterCnstLst
{
	int nCount;	/**< Totoal number of consent info in astConsents array */
	stDRMMeterCnst astConsents[DRM_MAX_METERCONSENT]; /**< Array of Metering consent information */
} stDRMMeterCnstLst;


/* 收收收收收收收收收收 [Metering User Consent List] 收收收收收收收收收收 */
 
/** 
  @ingroup	Object_Process_DT
  @brief		[OMA] Type of roap request */ 
typedef enum _DRMRoapMode
{
	eDRM_ROAP_PREVIEW,		/**< Start roap processing for getting preview or silent rights object */
	eDRM_ROAP_ROUPLOAD,		/**< Start roap processing for uploading rights object for selected content */
	eDRM_ROAP_ONEXPIRED		/**< Start roap processing for reporting metering data */
} enDRMRoapMode;


/* 收收收收收收收收收收 [DEBUGGIN FEATURE] 收收收收收收收收收收 */

/** 
  @ingroup	Managements_DT
  @brief		Operation code for DRM_Debug(). */
typedef enum _DRMDebugOpcode
{
	eDRM_DBG_CFG,								/**< Manage debugging configuration */
	eDRM_DBG_PROFILE_START,			/**< Start Profiling */
	eDRM_DBG_PROFILE_STOP,			/**< Stop Profiling */
	eDRM_DBG_DB_RIGHTS_DUMP,		/**< Dump Rights DB */
	eDRM_DBG_DB_RIGHTS_RESTORE,	/**< Restore Rights DB */
	eDRM_DBG_DB_CHANGE_DBMS,		/**< Change DBMS */
	eDRM_DBG_DB_TR_START,				/**< Start transaction */
	eDRM_DBG_DB_TR_END,					/**< End transaction */
	eDRM_DBG_CONNECT_URL,				/**< Connect test url using HTTP get */
	eDRM_DBG_HEAP_USAGE,				/**< Print out heap usage */
	eDRM_DBG_PLAYREADY_TC,			/**< Start PlayReady testing */
	eDRM_DBG_MAX
}enDRMDebugOpcode;

typedef enum _DRMDebugCfg
{
	eDRM_DBG_UNKNOWN			= 0x00000000,
	eDRM_DBG_CLEAR				= 0x00000001,		/* Clear Debug Flag */
	eDRM_DBG_LOG_LOW			= 0x00000002,		/* Enable DRM_LOG_LOW */
	eDRM_DBG_LOG_MID			= 0x00000004,		/* Enable DRM_LOG_MID */
	eDRM_DBG_LOG_HIGH			= 0x00000008,		/* Enable DRM_LOG_HIGH */
	eDRM_DBG_PERF_API 		= 0x00000010,		/* Enable performance log of API */
	eDRM_DBG_PERF_SEC			= 0x00000020,		/* Enable performance log of Seucrity */
	eDRM_DBG_PERF_DBMS		= 0x00000040,		/* Enable performance log of DBMS */
	eDRM_DBG_PERF_XML			= 0x00000080,		/* Enable performance log of XML */
	eDRM_DBG_PERF_XMLWR		= 0x00000100,		/* Enable performance log of XML writer */
	eDRM_DBG_PERF_WBXML		= 0x00000200,		/* Enable performance log of WBXML */
	
	eDRM_DBG_DUMP_ROAP		= 0x00000400,		/* Dump OMA DRM v2.0 Roap */
	eDRM_DBG_DUMP_RELV1		= 0x00000800,		/* Dump OMA DRM v1.0 REL */
	eDRM_DBG_DUMP_DM			= 0x00001000,		/* Dump OMA DRM v1.0 DM */
	eDRM_DBG_DUMP_SEED		= 0x00002000,		/* Dump IMEI */
	eDRM_DBG_DUMP_IMSI		= 0x00004000,		/* Dump IMSI */
	eDRM_DBG_DUMP_SRM			= 0x00008000,		/* Dump OMA SRM Messages */
	
	eDRM_DBG_MKROAMING		= 0x00010000,		/* Make roaming */
	eDRM_DBG_LEAVEMETDB		= 0x00020000,		/* Do not delete Metering DB */
	eDRM_DBG_MULTI_OCSP		= 0x00040000,		/* Support multiple ocsp for VF */
	eDRM_DBG_TRTRACKING		= 0x00080000,		/* Enable transaction tracking */
	eDRM_DBG_FETCH_SILENT	= 0x00100000,		/* Fetch Silent on-demand URL according to OMA TS */

	eDRM_DBG_FSCOUNT			= 0x00200000,		/* Count drmLibFsopen and close */
	eDRM_DBG_FILE_LOG			= 0x00400000,		/* Enable File Log */
} enDRMDebugCfg;


/* 收收收收收收收收收收 [DivX DRM] 收收收收收收收收收收 */

typedef struct _stDRM_DivXMediaSpec
{
	int dwMicroSecPerFrame;	/* set to 0 ? */
	int dwTotalFrames;			/* number of frames in first RIFF 'AVI ' chunk */
	int dwWidth;						/* width of video frame */
	int dwHeight;						/* height of the video frame */
} stDRM_DivXMediaSpec;


/* 收收收收收收收收收收 [Secure Removable Media] 收收收收收收收收收收 */

typedef enum _SrmDeviceType
{
	eSRM_DEV_UNKNOWN = -1,
	/* Access SRM Agent on Universal Subscriber Identity Module */
	eSRM_DEV_USIM,
	/* Access SRM Agent on Secure Digital card */
	eSRM_DEV_SD,
	/* Access SRM Agent over HTTP */
	eSRM_DEV_HTTP,

	eSRM_DEV_MAX
} enSrmDeviceType;

/* Action type */
typedef enum _SrmActionType
{
	eSRM_ACT_NONE = 0,
	// TODO: 
	/* Retrieve Rights List in SRM Agent */
	eSRM_ACT_RIGHTS_LIST = 1,
	/* Rights Move from Device to SRM */
	eSRM_ACT_RIGHTS_MOVE,
	/* Rights Move from SRM to Device */
	eSRM_ACT_RIGHTS_RETRIEVE,
	/* Local Rights Consumption */
	eSRM_ACT_RIGHTS_LOCAL,
	/* Rights Enablement */
	eSRM_ACT_RIGHTS_ENABLE,
	/* Change SAC */
	eSRM_ACT_CHANGE_SAC,
	/* Rights Information For TESTFEST#25 */
	eSRM_ACT_RIGHTS_INFO,
	/* RI Certificates Removal For TESTFEST#26 */
	eSRM_ACT_RICERTS_REMOVE
} enSrmActionType;

typedef struct _SrmRequest {
	enSrmDeviceType eSRMDevType;

	unsigned char *pucData;
	unsigned int uDataLen;

	void *pvData;
} stSrmRequest;


/* 收收收收收收收收收收 [Secure clock sync status] 收收收收收收收收收收 */
// should sync with Pal and Here
typedef enum _DRM_SECURE_CLOCK_SYNC_STATUS
{
	eDRM_SECURE_CLOCK_SYNC_NOT_YET = 0,
	eDRM_SECURE_CLOCK_NITZ_SYNC_DONE,
	eDRM_SECURE_CLOCK_OCSP_SYNC_DONE
} enDRM_SECURE_CLOCK_SYNC_STATUS;


/* 收收收收收收收收收收 [Windoes Mobile] 收收收收收收收收收收 */

#ifdef DRM_PLATFORM_WNDM
/*
	Unique Window Message name which will be sent to waiting module.
*/
#define DRM_REGISTER_MSG_STRING (_T("ROAPProcessEndMsg"))
#endif

#endif //_DRM_H_

