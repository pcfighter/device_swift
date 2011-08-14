
/******************************************************************************
* (c)Copyright 2002. All Rights Reserved
* LG Soft India
* Cherry Hills, 
* Embassy Golf Links Business Park
* Bangalore - 560071
* India.
*
* Project Name	: DRM R&D
* Developed by 	: LGE-MSL
* Security			: Confidential
* Module				: LG DRM
* Filename			: DRM_Error.h
* Purpose				: Definations of error flags
* Author(s)			: Jaya Prakash
* e-mail				: jayaprakash@lgsoftindia.com
* Creation date	: 01th Nov 2003
* Edit History	: Modification description(s)
Date				Name					Version			Description
---------		-------				------			--------------
2003.11.04	Jaya Prakash	v1.00				Initial Defination
2003.11.22  Anish Raj			v1.00				More Error codes related to DD Validator
2003.12.08  Anish Raj			v1.00				Added an error code for unsupported DD Version
2006.07.05  Lothson				v1.01				Clean unused error values
2009.0709		Lothson				v2.00				Remove unused historical values
******************************************************************************/

#ifndef _DRM_ERROR_H_
#define _DRM_ERROR_H_


/**
	@ingroup	Global_DT
	@brief		Common error code of external interface */
typedef enum _DRMError
{
	eDRM_UNSUPPORTED_RIGHT = -3,
	/**< Unsupported Rights object was received */
	eDRM_OUT_OF_STORAGE = -2,  
	/**< No more available space in file system to store rights object or media files */
	eDRM_FAILURE = -1,
	/**< Generic failure */
	eDRM_SUCCESS,
	/**< Success */
	eUNSPECIFIED_ERROR,
	/**< Unknown error */
	eDRM_CORRUPT_RIGHT,
	/**< Failure during parsing the license */
	eDRM_MANAGE_CERT_VERIFY_FAILED,
	/**< Error while verifying certificates */
	eDRM_MANAGE_CERT_MISMATCH,
	/**< Error while verifying certificates */
	eDRM_MANAGE_DB_MSDRM_CLOCK_NOT_SYNCHED,
	/**< MSDRM Clock Not synched*/
	eDRM_MANAGE_DB_MSDRM_HDS_CORRUPTED,
	/**< MSDRM HDS is corrupted */
	eDRM_END_COMMDRM_ERROR
	/**< End of Comm Error */
}enDRMError;

#endif
