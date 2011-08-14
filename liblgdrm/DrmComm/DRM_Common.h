
/******************************************************************************
* Developed by	: LGE
* Security			: Confidential
* Module				: LG DRM
* Filename			: DRM_Common.h
* Purpose				: LG DRM Common Definitions
* Author(s)			: Lothson
* e-mail				: lothson@lge.com
* Creation date	: 5th July 2006
* Edit History	: Modification description(s)
Date				Name					Version			Description
---------		-------				------			--------------
2006.07.05	Lothson				v1.00				Initial Creation
******************************************************************************/

/*
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	THIS FILE CONTAINS VARIOUS KINDS OF ENUM AND STRUCTURE DEFINITIONS.
	THESE DEFINITIONS WILL NOT BE EXPOSED TO OUTSIDE(MODULE).
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
*/

#ifndef _DRM_COMMON_H_
#define _DRM_COMMON_H_


/*
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
		INCLUDE
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
*/
#include "DRM.h" //DRMHDRMERGE "../Include/DRM.h"

/*
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
		DEFINE
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
*/

/* 收收收收收收收收收收 [DRM Object Process] 收收收收收收收收收收 */

typedef enum _DRMOpenType
{
	eDRM_OPEN_NOTDRM=0,		/* Normal file */
	eDRM_OPEN_OMA,				/* OMA DRM */
	eDRM_OPEN_CFM,				/* CFM DRM */
	eDRM_OPEN_LS,					/* Lockstream DRM */
	eDRM_OPEN_WM,					/* MS WMDRM */
	eDRM_OPEN_DIVX,				/* Divx DRM */
	eDRM_OPEN_PLAYREADY		/* MS PlayReady DRM */
} enDRMOpenType;


/* DRM Handle for Object processing and file I/O */
typedef struct _DRMHnd
{
	void *pvHnd;								/* Solution Handle */	
	enDRMSolutionType eSLType;	/* Solution type of Object processing */
	enDRMOpenType		eOpenType;	/* File I/O type */
	enDRMContentType	eCntType;	/* DRM File type */
} stDRMHnd;

typedef enum _DRMSLSesType
{
	eDRM_SLSES_OMA2 = 1,	/* OMA DRM 2.0 Session */
	eDRM_SLSES_WM,				/* WM DRM Session */
	eDRM_SLSES_PLAYREADY	/* PLAYREADY Session */
} enDRMSLSesType;

/* 收收收收收收收收收收 [DRM Time] 收收收收收收收收收收 */

typedef struct _DRMTime
{
	int tm_sec;			/* seconds after the minute [0, 59] */
	int tm_min;			/* minutes after the hour [0, 59] */
	int tm_hour;		/* hours since midnight [0, 23] */
	int tm_mday;		/* day of the month [1, 31] */
	int tm_mon;			/* months since January [0, 11] */
	int tm_year;		/* years since 1900 */
	int tm_wday;		/* days since Sunday [0, 6] */
	
	int tm_yday;		/* days since January 1 [0, 365] */
	int tm_isdst;		/* Daylight Saving Time flag */	
} stDRMTime;


/*
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	Send HTTP Request to Module or Task
	
	- Which module (or task) ask for getting HTTP Response
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
*/

typedef enum   _DRMROAPCallerType {
	eDRMROAP_CALLER_UNKNOWN = -1,
	eDRMROAP_CALLER_WAP,					/* Wap Browser */
	eDRMROAP_CALLER_ROAPCLIENT,		/* OMA DRM Roap Client */
	eDRMROAP_CALLER_BCASTCLIENT,	/* OMA DRM Bcast Profile Client */
	eDRMROAP_CALLER_DLA						/* WM DRM Direct License Acquisition Client */
} enDRMROAPCallerType;


/*
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	Type of mutex
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
*/

typedef enum _DRMMutexType
{
	eDRM_MUTEX_NONE = 0,
	eDRM_MUTEX_NORMAL,		/* Allocate default mutex */
	eDRM_MUTEX_RECURSIVE	/* Allocate recursive mutex */
} enDRMMutexType;


/*
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	ETC
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
*/

/* 收收收收收收收收收收 [Attribute of forward-lock content] 收收收收收收收收收收 */

typedef enum _DRMFLAttrType
{
	eDRM_FLATTR_NONE = 0,
	eDRM_FLATTR_NO_RINGTONE = 0x01,
	eDRM_FLATTR_NO_SCREEN = 0x02,
	eDRM_FLATTR_PRELOAD = 0x04
} enDRMFLAttrType;

#endif

