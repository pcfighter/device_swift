
/******************************************************************************
* Developed by	: LGE
* Security			: Confidential
* Module				: LG DRM
* Filename			: DRM_Port.c
* Purpose				: LG DRM Porting layer
* Author(s)			: Lothson
* e-mail				: lothson@lge.com
* Creation date	: 5th July 2006
* Edit History	: Modification description(s)
Date				Name					Version			Description
---------		-------				------			--------------
2006.07.05	Lothson				v1.00				Initial Creation
******************************************************************************/

/*
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
		INCLUDE
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
*/

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "DRM_Port.h"
#include "DRM_Error.h" //DRMHDRMERGE "../../DrmComm/DRM_Error.h"
#include "DRM_Common.h" //DRMHDRMERGE "../../DrmComm/DRM_Common.h"

/* ━━━━━━━━━━ [Platform Check Start] ━━━━━━━━━━ */
#if !defined(DRM_PLATFORM_LINUX)
#error DRM_Port.c --> Select proper porting interface or create new one
#endif
/* ━━━━━━━━━━ [Platform Check End] ━━━━━━━━━━ */

#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <linux/limits.h>

/* String */
#include <string.h>
#include <ctype.h>          /* isdigit, isxxxx ... */
#ifdef DRM_UNICODE_VERSION
#include <wchar.h>
#endif

/* File */
#include <fcntl.h>
#include <sys/stat.h>       /* fstat */
#include <dirent.h>     /* directory handling */
/* File system */
#include <sys/vfs.h>

/* Time */
#include <sys/time.h>
#include <time.h>

#if defined(DRM_PLATFORM_LIMO)
/* syslog */
#include <syslog.h>

/* DRM FW utilities */
#include <drm_util.h>
/* MMC Status Check */
#include <devmgr/devmgr_mmc.h>
#endif

/* Thread */
#include <pthread.h>


#if defined(DRM_PLATFORM_ANDROID)

#ifndef DRM_JNI_TEST

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "LGDRM"
/* 0 means enable */
#define LOG_NDEBUG	0	// LOGV
#define LOG_NDDEBUG	0 // LOGD
#define LOG_NIDEBUG	0	// LOGI

#include <cutils/log.h>
#include <cutils/properties.h>
#include <sys/system_properties.h>
#include <private/android_filesystem_config.h>
#else
#define LOGI	printf
#define LOGD	printf
#define LOGV	printf
#define LOGE	printf
#endif // DRM_JNI_TEST

/* Flex file path */
#define DRM_FLEX_XML DRM_T("/system/etc/flex/flex.xml")

#endif


/*
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
		DEFINE
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
*/

/*
	DRM Settings 
*/
int g_DRM_Settings = 0x00000000;

/*
	Debugging configuration
*/
int g_DRM_Debug = eDRM_DBG_LOG_HIGH;


/* Linux API returns 0 on sucess, -1 on errors. */
#define DRM_RV_CONV_LINUX2DRM(x) (x+1)

typedef struct _DRMMutex
{
	enDRMMutexType eType;
	pthread_mutex_t mutex;
	pthread_t owner;
	int nRef;
} stDRMMutex;


/* ━━━━━━━━━━ [DEFAULT OPERATOR CONFIGURATION] ━━━━━━━━━━ */

//1 Default configuration
const stDRMOpIntCfg drmDefaultIntCfg[] = { \
 { eDRM_OPINTCFG_DRM, 
	eDRM_Solution_OMA_DRM_v1 
	//|eDRM_Solution_OMA_DRM_v2 
	//|eDRM_Solution_OMA_DRM_v2_1
	//|eDRM_Solution_DIVXDRM 
	|eDRM_Solution_CFM
	//|eDRM_Solution_BCASTDRM
	//|eDRM_Solution_WMDRM
	//|eDRM_CERT_CMLA 
	//|eDRM_CERT_CMLA_DEVROOT
 },
 { eDRM_OPINTCFG_RIGHT_XML_PROLOGUE, 1},
 { eDRM_OPINTCFG_RIGHT_COUNT_WITH_TIME, 0},
 { eDRM_OPINTCFG_AUTO_LOADING_CERT, 0},
 { eDRM_OPINTCFG_LOADING_TEST_CERT, 0},
 { eDRM_OPINTCFG_TIMEBASED_CONST, 0},
};

const stDRMOpStrCfg drmDefaultStrCfg[] = { \
 { eDRM_OPSTRCFG_CERTTYPE, NULL}, 
};


/*
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
		GLOBAL FUNCTION DEFINITION
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
*/

/* ━━━━━━━━━━ [Memory] ━━━━━━━━━━ */

 
/**   
	@ingroup		Memory
	@brief			This interface returns free size of a heap.
	@param		void : (None)  
	@return
				@li	0		:	No more free memory exists.
				@li	> 0	:	Size of free memory in byte.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/12]
 */ 
unsigned int drmLibHeapGetFreeSize(void)
{
	/*
		Lothson.20090420
		
		This interface will not used anymore.
		It is really hard to detect free heap memory space under linux.
		Never try to use /proc information to achieve it.
		It can be only enabled if platform team supports that.
	*/
	return 0;
}


#ifndef DRM_DEBUG_HEAP
/**   
	@ingroup		Memory
	@brief			This interface allocates memory blocks.
	@param		uSize  	[IN] Bytes to allocate.
	@return
				@li	NULL	:	On failure.
				@li	>0 		:	Address of the allocated space.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/12]
 */ 
void *drmLibMalloc(unsigned int uSize)
{
	if (!uSize) 
	{
		return NULL;
	}
	return malloc(uSize);
}

 
/**   
	@ingroup		Memory
	@brief			This interface allocates an array in memory with elements initialized to 0.
	@param		uSize  	[IN] Bytes to allocate.
	@return
				@li	NULL	:	On failure.
				@li	> 0		:	Address of the allocated space.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/12]
 */ 
void *drmLibCalloc(unsigned int uSize)
{
	if (!uSize) 
	{
		return NULL;
	}
	/* According to F.Phone's convention. */
	return calloc(1, uSize);
}

 
/**   
	@ingroup		Memory
	@brief			This interface deallocates or frees a memory block.
	@param		pvPtr  	[IN] Previously allocated memory block to be freed. 
	@return
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/12]
 */ 
void drmLibFree(void *pvPtr)
{
	if (! pvPtr) 
	{
		return;
	}
	free(pvPtr);
}   

 
/**   
	@ingroup		Memory
	@brief			This interface reallocates memory blocks.
	@param		pvPtr  [OUT] Pointer to previously allocated memory block. 
	@param		uSize  [IN] New size in bytes. 
	@return
				@li	NULL	:	On failure.
				@li	> 0		:	Address of the reallocated memory block.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/12]
 */ 
void *drmLibRealloc(void *pvPtr, unsigned int uSize)
{
	if (!uSize) 
	{
		return NULL;
	}
	return realloc(pvPtr, uSize);
}


/**   
	@ingroup		Memory
	@brief			This interface deallocates or frees a memory block.
	@param		pvPtr  	[IN] Previously allocated memory block to be freed. 
	@return
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
void drmLibExtFree(void *pvPtr)
{
	if (! pvPtr) 
	{
		return;
	}
	return free(pvPtr);
}

 
/**   
	@ingroup		Memory
	@brief			This interface allocates memory blocks.
	@param		uSize  	[IN] Bytes to allocate.
	@return
				@li	NULL	:	On failure.
				@li	>0 		:	Address of the allocated space.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
void *drmLibExtMalloc(unsigned int uSize)
{
	if (!uSize) 
	{
		return NULL;
	}
	return malloc(uSize);
}

 
/**   
	@ingroup		Memory
	@brief			This interface allocates an array in memory with elements initialized to 0.
	@param		uSize  	[IN] Bytes to allocate.
	@return
				@li	NULL	:	On failure.
				@li	> 0		:	Address of the allocated space.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
void *drmLibExtCalloc(unsigned int uSize)
{
	if (!uSize) 
	{
		return NULL;
	}
	return calloc(1, uSize);
}

#endif	// end of DRM_DEBUG_HEAP

 
/**   
	@ingroup		Memory
	@brief			This interface reallocates memory blocks.
	@param		pvPtr	[OUT] Pointer to previously allocated memory block. 
	@param		uSize  	[IN] New size in bytes. 
	@return
				@li	NULL	:	On failure.
				@li	> 0		:	Address of the reallocated memory block.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/12]
 */ 
void *drmLibExtRealloc(void *pvPtr, unsigned int uSize)
{
	if (!uSize) 
	{
		return NULL;
	}
	return realloc(pvPtr, uSize);
}

 
/**   
	@ingroup		Memory
	@brief			This interface sets buffers to a specified value.
	@param		pvPtr  	[OUT] Pointer to destination. 
	@param		nValue	[IN] Character to set. 
	@param		uSize  	[IN] Number of characters. 
	@return
				@li	NULL	:	On failure.
				@li	> 0		:	Address of the destination.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
void *drmLibMemset(void *pvPtr, int nValue, unsigned int uSize)
{
	if (!pvPtr || !uSize) 
	{
		return NULL;
	}
	return memset(pvPtr, nValue, uSize);
}

 
/**   
	@ingroup		Memory
	@brief			This interface copies memory block for given size.
	@param		pvTarget  	[OUT] New buffer.
	@param		pvSource  	[IN] Buffer to copy from.
	@param		uSize  			[IN] Nummer of bytes to copy.
	@return
				@li	NULL	:	On failure.
				@li	> 0		:	Address of the new buffer.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
void *drmLibMemcpy(void *pvTarget, const void *pvSource, unsigned int uSize)
{
	if (!pvTarget ||!pvSource ||!uSize) 
	{
		return NULL;
	}
	return memcpy(pvTarget, pvSource, uSize);
}


/**   
	@ingroup		Memory
	@brief			This interface moves one buffer to another.
	@param		pvTarget  	[OUT] Destination object.
	@param		pvSource  	[IN] Source object.
	@param		uSize		  	[IN] Number of bytes of characters to copy.
	@return
				@li	NULL	:	On failure.
				@li	> 0		:	Address of the destinatino object.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
void *drmLibMemmove(void *pvTarget, const void *pvSource, unsigned int uSize)
{
	if (!pvTarget ||!pvSource ||!uSize) 
	{
		return NULL;
	}
	return memmove(pvTarget, pvSource, uSize);
}

 
/**   
	@ingroup		Memory
	@brief			This interface compare byte data in two buffers.
	@param		pvTarget  	[IN] First buffer.
	@param		pvSource  	[IN] Second buffer.
	@param		uSize		  	[IN] Number of bytes.
	@return
				@li	< 0	:	pTarget less than pSource.
				@li	0		:	pTarget identical to pSource.
				@li	> 0	:	pTarget greater than pSource.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
int drmLibMemcmp(const void *pvTarget, const void *pvSource, unsigned int uSize)
{
	if (!pvTarget||!pvSource ||!uSize) 
	{
		return 1;
	}
	return memcmp(pvTarget, pvSource, uSize);
}



/* ━━━━━━━━━━ [String] ━━━━━━━━━━ */


#ifndef DRM_DEBUG_HEAP
/**   
	@ingroup		String
	@brief			This interface duplicate strings.
	@param		pszStr 	[IN] Null-terminated source string. 
	@return
				@li	NULL	:	On failure.
				@li	> 0		:	Address of the copied string.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
char *drmLibStrdup(const char *pszStr)
{
	if (!pszStr) 
	{
		return NULL;
	}
	return strdup(pszStr);
}

 
/**   
	@ingroup		String
	@brief			This interface duplicate strings.
	@param		pszStr 	[IN] Null-terminated source string. 
	@return
				@li	NULL	:	On failure.
				@li	> 0		:	Address of the copied string.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
char *drmLibExtStrdup(const char *pszStr)
{
	if (!pszStr) 
	{
		return NULL;
	}
	return strdup(pszStr);
}


/**   
	@ingroup		String
	@brief			This interface duplicate strings.
	@param		pszStr  	[IN] Source string. 
	@param		uSize		  	[IN] Size of byte to dumplicate.
	@return
				@li	NULL	:	On failure.
				@li	> 0		:	Address of the null terminated string.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/10/9]
 */ 
char *drmLibStrndup(const char *pszStr, unsigned int uSize)
{
	if (!pszStr)
	{
		return NULL;
	}

	return strndup(pszStr, uSize);
}
#endif 

 
/**   
	@ingroup		String
	@brief			This interface copies a string.
	@param		pszTarget  	[OUT] Destination string. 
	@param		pszSource  	[IN] Null-terminated source string. 
	@return
				@li	NULL	:	On failure.
				@li	> 0		:	Address of the destination string.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
char *drmLibStrcpy(char *pszTarget, const char *pszSource)
{
	if (!pszTarget ||!pszSource) 
	{
		return NULL;
	}
	return strcpy((char *)pszTarget, (char *)pszSource);
}

 
/**   
	@ingroup		String
	@brief			This interface copies characters of one string to another.
	@param		pszTarget  	[OUT] Destination string.
	@param		pszSource  	[IN] Source string.
	@param		uSize		  	[IN] Number of characters to be copied.
	@return
				@li	NULL	:	On failure.
				@li	> 0		:	Address of the destination string.
	@warning	
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
char *drmLibStrncpy(char *pszTarget, const char *pszSource, unsigned int uSize)
{
	if (!pszTarget ||!pszSource||! uSize) 
	{
		return NULL;
	}
	return strncpy(pszTarget, pszSource, uSize);
}

 
/**   
	@ingroup		String
	@brief			This interface appends a string.
	@param		pszTarget  	[OUT] Null-terminated destination string. 
	@param		pszSource  	[IN] Null-terminated source string. 
	@return
				@li	NULL	:	On failure.
				@li	> 0		:	Address of the destination string.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
char *drmLibStrcat(char *pszTarget, const char *pszSource)
{
	if (!pszTarget ||!pszSource) 
	{
		return NULL;
	}
	return strcat((char *)pszTarget, (char *)pszSource);
}


/**   
	@ingroup		String
	@brief			This interface appends a string.
	@param		pszTarget  	[OUT] Null-terminated destination string. 
	@param		pszSource  	[IN] Null-terminated source string. 
	@param		uSize		  	[IN] Number of characters to be appended.
	@return
				@li	NULL	:	On failure.
				@li	> 0		:	Address of the destination string.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
char* drmLibStrncat(char *pszTarget, const char *pszSource, unsigned int uSize)
{
	if (!pszTarget ||!pszSource||!uSize) 
	{
		return NULL;
	}
	return strncat((char *)pszTarget, (char *)pszSource, uSize);
}

 
/**   
	@ingroup		String
	@brief			This interface compares strings.
	@param		pszTarget  	[IN] Null-terminated strings to compare. 
	@param		pszSource  	[IN] Null-terminated strings to compare. 
	@return
				@li	< 0	:	pTarget less than pSource.
				@li	0		:	pTarget identical to pSource.
				@li	> 0	:	pTraget greater than pSource.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
int drmLibStrcmp(const char *pszTarget, const char *pszSource)
{
	if (!pszTarget ||!pszSource) 
	{
		return 1;
	}
	return strcmp((char *)pszTarget, (char *)pszSource);
}

 
/**   
	@ingroup		String
	@brief			This interface compares characters of two strings.
	@param		pszTarget  	[IN] Strings to compare. 
	@param		pszSource  	[IN] Strings to compare. 
	@param		uSize		  	[IN] Number of characters to compare. 
	@return
				@li	< 0	:	pTarget less than pSource.
				@li	0		:	pTarget identical to pSource.
				@li	> 0	:	pTraget greater than pSource.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
int drmLibStrncmp(const char *pszTarget, const char *pszSource, unsigned int uSize)
{
	if (!pszTarget ||!pszSource||!uSize) 
	{
		return 1;
	}
	return strncmp((char *)pszTarget, (char *)pszSource, uSize);
}

 
/**   
	@ingroup		String
	@brief			This interface performs a lowercase comparison of strings.
	@param		pszTarget  	[IN] Null-terminated strings to compare.
	@param		pszSource  	[IN] Null-terminated strings to compare.
	@return
				@li	< 0	:	pTarget less than pSource.
				@li	0		:	pTarget identical to pSource.
				@li	> 0	:	pTarget greater than pSource.
	@warning	
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
int drmLibStricmp(const char *pszTarget, const char *pszSource)
{
	if (pszTarget == pszSource) return(0);
	if (pszTarget == NULL) return(-1);
	if (pszSource == NULL) return(1);

	return strcasecmp(pszTarget, pszSource);
}

 
/**   
	@ingroup		String
	@brief			This interface finds a character in a string.
	@param		pszStr  [IN] Null-terminated source string.
	@param		nChr  	[IN] Character to be located.
	@return
				@li	NULL	:	On failure.
				@li	> 0		:	Address of the first occurrence of cCharacter in pString.
	@warning			
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
char *drmLibStrchr (const char *pszStr, int nChr)
{
	if (!pszStr ||!nChr) 
	{
		return NULL;
	}
	return strchr((char *)pszStr, nChr);
}


/**   
	@ingroup		String
	@brief			This interface finds a character in a string from the end.
	@param		pszStr  [IN] Null-terminated source string.
	@param		nChr  	[IN] Character to be located.
	@return
				@li	NULL	:	On failure.
				@li	> 0		:	Address of the first occurrence of cCharacter in pString.
	@warning
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2009/7/3]
 */ 
char *drmLibStrrchr(const char *pszStr, int nChr)
{
	if (!pszStr ||!nChr) 
	{
		return NULL;
	}

	return strrchr(pszStr, nChr);
}


/**   
	@ingroup		String
	@brief			This interface returns a pointer to the first occurrence of a search string in a string.
	@param		pszStr  		[IN] Null-terminated string to search. 
	@param		pszSubStr  	[IN] Null-terminated string to search for. 
	@return
				@li	NULL 	:	On failure or there is no search string in a string.
				@li	> 0		:	Address of the first occurrence of pSubString in pString.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM AM [Modify date : 2008/6/13]
 */ 
char *drmLibStrstr (const char *pszStr, const char *pszSubStr)
{
	if (!pszStr ||!pszSubStr) 
	{
		return NULL;
	}
	return strstr((char *)pszStr, pszSubStr);
}


/**   
	@ingroup		String
	@brief			This interface returns a pointer to the first occurrence of a search string in a string.   
	@param		pszStr  	[IN] Null-terminated string to search. 
	@param		pszSubStr [IN] Null-terminated string to search for. 
	@param		nStrLen  	[IN] Length of string.
	@return
				@li	NULL 	:	On failure or there is no search string in a string.
				@li	> 0		:	Address of the first occurrence of pSubString in pString.
	@warning
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2009/7/3]
 */ 
char* drmLibStrnstr(const char* pszStr, const char* pszSubStr, int nStrLen)
{
	unsigned char *q = (unsigned char*)pszSubStr;
	unsigned char *p = (unsigned char*)pszStr;
	unsigned char *pStrt = NULL;
	int found = 0;

	/* Check for valid input values */
	if(0 >= nStrLen || p == NULL || pszSubStr == NULL || drmLibStrlen(pszSubStr) == 0)
	{
		return NULL;
	}

	while(nStrLen--)
	{
		/* Check if the starting point is found */
		if(!found)
		{
			if(*p++ == *q)
			{
				found = 1;
				pStrt = p - 1; /* pointer "p" was already incremeted, so (p-1) is starting point.*/
				q++;
				if(*q == 0) /* If matching string reached end */
					break;
			}
		}
		else
		{
			if(*p++ == *q++)				
			{
				if(*q == 0)  /* If matching string reached end */
					break;			
			}
			else /* If the current character did not match */
			{
				pStrt = NULL;
				found = 0;
				q = (unsigned char*)pszSubStr;
			}
		}
	}
	
	/* This condition checks, though the starting point is found, but 
	   the search buffer reached end before ending point could be found.
	 */
	if(nStrLen < 0 && *q != 0) 
	{
		pStrt = NULL;
	}

	return pStrt;
}


/**   
	@ingroup		String
	@brief			This interface gets the length of a string.
	@param		pszStr  	[IN] Null-terminated string.
	@return
				@li	0		:	On failure.
				@li	> 0	:	The number of characters in pString.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
unsigned int drmLibStrlen (const char *pszStr)
{
	if (!pszStr) 
	{
		return 0;
	}
	return strlen((char *)pszStr);
}

 
/**   
	@ingroup		String
	@brief			This interface finds the next token in a string.
	@deprecated	Use drmLibStrtok_r() interface instead.
							@n It doesn't support reentrance and also safe for multiple thread environment.
	@param		pszStr  		[IN] String containing token or tokens.
	@param		pszDelim  	[IN] Set of delimiter characters.
	@return
				@li	NULL 	:	On failure.
				@li	> 0		:	Address of the next token found in pString.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
char *drmLibStrtok(char *pszStr, const char *pszDelim)
{
	if (!pszDelim) 
	{
		return NULL;
	}
	return strtok(pszStr, pszDelim);
}

/**
	@ingroup	String
	@brief		This interface is reentrant version of strtok.
	@param		pszStr			[IN] Source string
	@param		pszDelims		[IN] Set of delimiter characters.
	@param		ppszSaveptr	[IN]/[OUT] Saved pointer.
	@return
				@li	NULL 	:	On failure.
				@li	> 0		:	Address of the next token found in pszStr.
	@warning
	@par PreCondition
	@par PostCondition
	@par Example
				@code
				@endcode
	@sa
	@author		LGE DRM TEAM [Modify date : 2009/9/29]
*/
char *drmLibStrtok_r(char *pszStr, const char *pszDelims, char **ppszSaveptr)
{
	if (!pszDelims) 
	{
		return NULL;
	}
	return strtok_r(pszStr , pszDelims, ppszSaveptr);
}
 
/**   
	@ingroup		String
	@brief			This interface writes formatted data to a string.
	@param		pszBuf  		[IN] Storage location for output.
	@param		pszFormat	[IN] Format-control string.
	@param		...  				[IN] Optional arguments.
	@return
				@li	<= 0		:	On failure.
				@li	> 0		:	The number of characters written.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
				drmLibVsprintf()
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
int drmLibSprintf(char *pszBuf, const char *pszFormat, ...)
{
	int iRet;
	va_list		VaList;

	va_start(VaList, pszFormat);
	iRet = drmLibVsprintf(pszBuf,pszFormat,VaList);
	va_end(VaList);

	return iRet;
}

 
/**   
	@ingroup		String
	@brief			This interface writes formatted output using a pointer to a list of arguments.
	@param		pszBuf  		[IN] Storage location for output.
	@param		pszFormat	[IN] Format specification.
	@param		pstArgs  		[IN] Pointer to list of arguments.
	@return
				@li	<= 0		:	On failure.
				@li	> 0		:	The number of characters written.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
int drmLibVsprintf(char *pszBuf, const char *pszFormat, va_list pstArgs)
{
	if (!pszBuf ||!pszFormat) 
	{
		return 0;
	}
	return vsprintf(pszBuf,pszFormat,pstArgs);
}

 
/**   
	@ingroup		String
	@brief			This interface writes formatted data to a string.
	@param		pszBuf  		[IN] Storage location for output.
	@param		uBufLen  		[IN] Maximum number of characters to write.
	@param		pszFormat 	[IN] Format-control string.
	@param		...  				[IN] Optional arguments.
	@return
				@li	<= 0	:	On failure.
				@li	> 0	:	The number of characters written.
	@warning
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
				drmLibVsnprintf()
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
int drmLibSnprintf(char *pszBuf, unsigned int uBufLen, const char *pszFormat, ...)
{
	int iRet;
	va_list		VaList;

	va_start(VaList, pszFormat);
	iRet = drmLibVsnprintf(pszBuf,uBufLen,pszFormat,VaList);
	va_end(VaList);

	return iRet;
}

 
/**   
	@ingroup		String
	@brief			This interface writes formatted output using a pointer to a list of arguments.
	@param		pszBuf  		[IN] Storage location for ouput.
	@param		uBufLen  		[IN] Maximum number of characters to write.
	@param		pszFormat	[IN] Format specification.
	@param		pstArgs  		[IN] Pointer to list of arguments.
	@return
				@li	<= 0		:	On failure.
				@li	> 0		:	The number of characters written.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
int drmLibVsnprintf(char *pszBuf, unsigned int uBufLen, const char *pszFormat, va_list pstArgs)
{
#ifdef WIN32
	return _vsnprintf(pszBuf,uBufLen,pszFormat,pstArgs);
#else
	return vsnprintf(pszBuf,uBufLen,pszFormat,pstArgs);
#endif
}


/* ━━━━━━━━━━ [Unicode String] ━━━━━━━━━━ */

#if !defined(DRM_UNICODE_VERSION) || defined(DRM_USE_PLATFORM_UNIFUNC)
/**   
	@ingroup		Unicode_String
	@brief			This interface copies a UCS-2 string.
	@param		pusTarget  	[OUT] Destination string. 
	@param		pusSource  	[IN] Null-terminated source string. 
	@return
				@li	NULL	:	On failure.
				@li	> 0		:	Address of the destination string.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
DRM_TSTR drmLibUniStrcpy(DRM_TSTR pusTarget, DRM_TCSTR pusSource)
{
	if (!pusTarget ||!pusSource) 
	{
		return NULL;
	}

#ifdef DRM_UNICODE_VERSION
	return wcscpy(pusTarget, pusSource);
#else
	return strcpy((char*)pusTarget, (const char*)pusSource);
#endif
}

 
/**   
	@ingroup		Unicode_String
	@brief			This interface copies characters of one UCS-2 string to another.
	@param		pusTarget  	[OUT] Destination string.
	@param		pusSource  	[IN] Source string.
	@param		uSize  			[IN] Number of characters to be copied.
	@return
				@li	NULL 	:	On failure.
				@li	> 0		:	Address of the destination string.
	@warning	
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
DRM_TSTR drmLibUniStrncpy(DRM_TSTR pusTarget, DRM_TCSTR pusSource, unsigned int uSize)
{
	if (!pusTarget ||!pusSource||!uSize) 
	{
		return NULL;
	}

#ifdef DRM_UNICODE_VERSION
	return wcsncpy(pusTarget, pusSource, uSize);
#else
	return strncpy((char*)pusTarget, (const char*)pusSource, uSize);
#endif
}


/**   
	@ingroup		Unicode_String
	@brief			This interface appends a UCS-2 string.
	@param		pusTarget  	[OUT] Null-terminated destination string. 
	@param		pusSource  	[IN] Null-terminated source string. 
	@return
				@li	NULL	:	On failure.
				@li	> 0		:	Address of the destination string.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */  
DRM_TSTR drmLibUniStrcat(DRM_TSTR pusTarget, DRM_TCSTR pusSource)
{
	if (!pusTarget ||!pusSource) 
	{
		return NULL;
	}
	
#ifdef DRM_UNICODE_VERSION
	return wcscat(pusTarget, pusSource);
#else
	return strcat((char*)pusTarget, (const char*)pusSource);
#endif
}

 
/**   
	@ingroup		Unicode_String
	@brief			This interface compares UCS-2 strings.
	@param		pTarget  	[IN] Null-terminated strings to compare. 
	@param		pSource  	[IN] Null-terminated strings to compare. 
	@return
				@li	< 0	:	pTarget less than pSource.
				@li	0		:	pTarget identical to pSource.
				@li	> 0	:	pTraget greater than pSource.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
int drmLibUniStrcmp(DRM_TCSTR pusTarget, DRM_TCSTR pusSource)
{
	if (!pusTarget ||!pusSource) 
	{
		return 1;
	}
	
#ifdef DRM_UNICODE_VERSION
	return wcscmp(pusTarget, pusSource);
#else
	return strcmp((const char*)pusTarget, (const char*)pusSource);
#endif
}

 
/**   
	@ingroup		Unicode_String
	@brief			This interface compares characters of two UCS-2 strings.
	@param		pTarget  	[IN] Strings to compare. 
	@param		pSource  	[IN] Strings to compare. 
	@param		count  		[IN] Number of characters to compare. 
	@return
				@li	< 0	:	pTarget less than pSource.
				@li	0		:	pTarget identical to pSource.
				@li	> 0	:	pTraget greater than pSource.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
int drmLibUniStrncmp(DRM_TCSTR pusTarget, DRM_TCSTR pusSource, unsigned int uSize)
{
	if (!pusTarget ||!pusSource || !uSize) 
	{
		return 1;
	}

#ifdef DRM_UNICODE_VERSION
	return wcsncmp(pusTarget, pusSource, uSize);				
#else
	return strncmp((const char*)pusTarget, (const char*)pusSource, uSize);
#endif
}

 
/**   
	@ingroup		Unicode_String
	@brief			This interface gets the length of a UCS-2 string.
	@param		pusStr  	[IN] Null-terminated string.
	@return
				@li	0		:	On failure.
				@li	> 0	:	The number of characters in pString.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date :	2008/6/13]
 */ 
int drmLibUniStrlen(DRM_TCSTR pusStr)
{
	if (!pusStr) 
	{
		return 0;
	}
	
#ifdef DRM_UNICODE_VERSION
	return wcslen(pusStr);
#else
	return strlen((const char*)pusStr);
#endif
}
#endif

#ifndef DRM_DEBUG_HEAP
/**   
	@ingroup		Unicode_String
	@brief			This interface duplicate UCS-2 strings.
	@param		pusStr  	[IN] Null-terminated source string. 
	@return
				@li	NULL	:	On failure.
				@li	> 0		:	Address of the copied string.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
DRM_TSTR drmLibUniStrdup(DRM_TCSTR pusStr)
{
	if (!pusStr) 
	{
		return NULL;
	}
	
	return strdup((const char*)pusStr);
}

 
/**   
	@ingroup		Unicode_String
	@brief			This interface duplicate UCS-2 strings located.
	@param		pusStr  	[IN] Null-terminated source string. 
	@return
				@li	NULL	:	On failure.
				@li	> 0		:	Address of the copied string.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
DRM_TSTR drmLibExtUniStrdup(DRM_TCSTR pusStr)
{
	if (!pusStr) 
	{
		return NULL;
	}
	
	return strdup((const char*)pusStr);
}

#endif


#if !defined(DRM_UNICODE_VERSION) || defined(DRM_USE_PLATFORM_UNIFUNC)
/**   
	@ingroup		Unicode_String
	@brief			This interface returns a pointer to the first occurrence of a search UCS-2 string in a UCS-2 string.
	@param		pusStr  		[IN] Null-terminated string to search. 
	@param		pusSubStr  	[IN] Null-terminated string to search for. 
	@return
				@li	NULL 	:	On failure.
				@li	> 0		:	Address of the first occurrence of pSubString in pString.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
DRM_TSTR drmLibUniStrstr(DRM_TCSTR pusStr, DRM_TCSTR pusSubStr)
{
	if (!pusStr ||!pusSubStr) 
	{
		return NULL;
	}
	
#ifdef DRM_UNICODE_VERSION
	return wcsstr(pusStr, pusSubStr);
#else
	return strstr((const char*)pusStr, (const char*)pusSubStr);
#endif
}

/*
	2006.03.06 Lothson. 
	If the PDK does not support UniLib_Vsprintf, use this method.
*/
#ifdef DRM_UNICODE_VERSION
#error need to do something!
#else
drmLibUniSprintfFunc drmLibUniSprintf 		= (drmLibUniSprintfFunc) sprintf;
#endif

#if 0
 
/**   
	@ingroup		Unicode_String
	@brief			This interface writes formatted data to a UCS-2 string.
	@param		pusBuf  			[IN] Storage location for output.
	@param		pusFormat		[IN] Format-control string.
	@param		...  					[IN] Optional arguments.
	@return
				@li	<= 0		:	On failure.
				@li	> 0		:	The number of characters written.
	@warning
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/18]
 */ 
int drmLibUniSprintf(DRM_TSTR pusBuf, DRM_TCSTR pusFormat, ...)
{
	va_list args;
	int i;
	
	va_start(args, pusFormat);
	i = UniLib_Vsprintf(pusBuf, pusFormat, args);
	va_end(args);
	
	return i;
}
#endif

 
/**   
	@ingroup		Unicode_String
	@brief			This interface finds a character in a UCS-2 string.
	@param		pusStr  [IN] Null-terminated source string.
	@param		nChr		[IN] Character to be located.
	@return
				@li	NULL	:	On failure.
				@li	> 0		:	Address of the first occurrence of ch in pString.
	@warning			
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
DRM_TSTR drmLibUniStrchr(DRM_TCSTR pusStr, int nChr)
{
	if (!pusStr ||!nChr) 
	{
		return NULL;
	}
	
#ifdef DRM_UNICODE_VERSION
	return wcschr(pusStr, (DRM_UCHAR)nChr);
#else
	return strchr((const char*)pusStr, nChr);
#endif
}


/**   
	@ingroup		Unicode_String
	@brief			This interface performs a lowercase comparison of UCS-2 strings.
	@param		pTarget  	[IN] Null-terminated strings to compare.
	@param		pSource  	[IN] Null-terminated strings to compare.
	@return
				@li	< 0	:	pTarget less than pSource.
				@li	0		:	pTarget identical to pSource.
				@li	> 0	:	pTarget greater than pSource.
	@warning	
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
int drmLibUniStricmp(DRM_TCSTR pusTarget, DRM_TCSTR pusSource)
{
	if (!pusTarget || !pusSource)
	{
		return 1;
	}
	
  return strcasecmp((const char*)pusTarget, (const char*)pusSource);
}


/* ━━━━━━━━━━ [Conversion function] ━━━━━━━━━━ */

 
/**   
	@ingroup		String_Conversion
	@brief			This interface converts a UTF-8 string to UCS-2 string.
	@param		pszSource  	[IN] String to be converted.
	@param		pusTarget  	[IN] Destination string.
	@return
				@li	<= 0		:	On failure.
				@li	> 0		:	The number of characters converted.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
int drmLibUTF8ToUCS2(const char *pszSource, DRM_TSTR pusTarget)
{
	int nCount = 0;
	
	if(!pszSource || !pusTarget)
	{
		return eDRM_FAILURE;
	}

	nCount = drmLibStrlen(pszSource);
	if (nCount <= 0)
	{
		return eDRM_FAILURE;
	}		

	if (!drmLibStrcpy(pusTarget, pszSource))
	{
		return eDRM_FAILURE;
	}
	return nCount;
}

 
/**   
	@ingroup		String_Conversion
	@brief			This interface converts a UCS-2 string to UTF-8 string.
	@param		pusSource 	[IN] String to be converted.
	@param		pszTarget  	[IN] Destination string.
	@return
				@li	<= 0		:	On failure.
				@li	> 0		:	The number of characters converted.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
int drmLibUCS2ToUTF8(DRM_TCSTR pusSource, char *pszTarget)
{
	int nCount = 0;
	
	if(!pusSource || !pszTarget)
	{
		return eDRM_FAILURE;
	}

	nCount = drmLibStrlen(pusSource);
	if (nCount <= 0)
	{
		return eDRM_FAILURE;
	}		

	if (!drmLibStrcpy(pszTarget, pusSource))
	{
		return eDRM_FAILURE;
	}
	return nCount;
}
#endif


/**   
	@ingroup		Unicode_String
	@brief			This interface gets the length of a UCS-2 string.
	@param		string  	[IN] Null-terminated string.
	@return
				@li	0		:	On failure.
				@li	> 0	:	The number of characters in string.
	@warning		
				If #DRM_UCHAR is UTF-8 character, not UCS-2,
				all unicode functions will performs exactly the same functionalities with UTF-8.
				But this interface is not affected by #DRM_UCHAR data type.
				It always works as UCS-2 function.
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
unsigned int drmLibUniFixedStrlen(const unsigned short* pucStr)
{
	unsigned short uChar;
	unsigned char *pStr = (unsigned char*)pucStr;
	unsigned int CharCount	= 0;

	if (!pucStr)
	{
		return 0;
	}

	while(1)
	{
		uChar	= *pStr + (*(pStr+1) << 8);
		pStr   += sizeof(unsigned short);

		if (uChar == 0x0000)
		{
			return CharCount;
		}

		CharCount++;
	}
}


/******************************************************************************
* Function name	:	
* Description		:	Local function to convert UTF8 to UCS2
* Pre-Condition	:	
* Post-Condition:	
* Return Type		:	
* Argument			:	
*			[IN]			:	
*			[OUT] 		:	 
*			[IN/OUT]	:	
* Globals Affected:	
******************************************************************************/
int Cvrt_UTF8_To_UCS2(const unsigned char *utf8, unsigned short *ucs2)
{
	int		n;

	if		((*utf8 & 0xf8) == 0xf0) 	{ n = 4; } /* 11110XXXX */
	else if	((*utf8 & 0xf0) == 0xe0) 	{ n = 3; } /* 1110XXXXX */
	else if	((*utf8 & 0xe0) == 0xc0)   { n = 2; } /* 110XXXXXX */
	else if	((*utf8 & 0x80) == 0x00)   { n = 1; } /* 0XXXXXXXX */
	else 							{ n = 0; } /* Error */


	switch (n)
	{
		case 1: 
			*ucs2 = (*utf8);
			break;
		case 2: 
			*ucs2 = (((*utf8)&0x1f)<< 6)  		/* 110XXXXX */
				| ((*(utf8+1)) &0x3f); 		/* 10XXXXXX */
			break;
		case 3: 
			*ucs2 = (((*utf8)     &0x0f) << 12) 	/* 1110XXXX */
				| (((*(utf8+1)) &0x3f) << 6 ) 	/* 10XXXXXX */
				| (( *(utf8+2)) &0x3f);		/* 10XXXXXX */
			break;
		case 4: 
			*ucs2 = (((*utf8)     &0x07) << 18) 	/* 11110XXX */
				| (((*(utf8+1)) &0x3f) << 12) 	/* 10XXXXXX */
				| (((*(utf8+2)) &0x3f) <<  6)	/* 10XXXXXX */
				| (( *(utf8+3)) &0x3f);		/* 10XXXXXX */
			break;
		default:
			return 0;
	}

	return n;
}


 /******************************************************************************
* Function name	:	
* Description		:	Local function to convert UCS2 to UTF8
* Pre-Condition	:	
* Post-Condition:	
* Return Type		:	
* Argument			:	
*			[IN]			:	
*			[OUT] 		:	 
*			[IN/OUT]	:	
* Globals Affected:	
******************************************************************************/
int Cvrt_UCS2_To_UTF8(unsigned short ucs2, unsigned char *utf8)
{
	int n;

	if (ucs2 < 0x00000080)
	{
		n = 1;
	}
	else if (ucs2 < 0x00000800)
	{
		n = 2;
	}
	else
	{
		n = 3;
	}

	switch (n)
	{
	case 1:
		*(utf8)		= (unsigned char)ucs2;
		break;
	case 2:
		*(utf8)		= 0xc0 | ((ucs2 >> 6) & 0x1f);
		*(utf8+1)	= 0x80 | ((ucs2 >> 0) & 0x3f);
		break;
	case 3:
		*(utf8)		= 0xe0 | ((ucs2 >>12) & 0x0f);
		*(utf8+1)	= 0x80 | ((ucs2 >> 6) & 0x3f);
		*(utf8+2)	= 0x80 | ((ucs2 >> 0) & 0x3f);
		break;
	}

	return n;
}

 
/**   
	@ingroup		String_Conversion
	@brief			This interface converts a UTF-8 string to UCS-2 string.
	@param		pszSource 	[IN] String to be converted.
	@param		pusTarget  	[IN] Destination string.
	@return
				@li	<= 0		:	On failure.
				@li	> 0		:	The number of characters converted.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
int drmLibFixedUTF8ToUCS2(const char *pszSource, unsigned short *pusTarget)
{ 
	int 	length_utf8;
	int 	length_ucs2;
	unsigned char*	pointer_utf8;
	unsigned short*	pointer_ucs2;

	if(!pszSource || !pusTarget)
	{
		return eDRM_FAILURE;
	}
	
	pointer_utf8 	= (unsigned char*)pszSource;
	length_utf8 	= strlen(pszSource)+1;
	length_ucs2 	= 0;
	pointer_ucs2 = pusTarget;

	while(length_utf8>0)	// 방어코드 삽입
	{
		int pass_count;
		pass_count = Cvrt_UTF8_To_UCS2(pointer_utf8, pointer_ucs2);
		if(pass_count == 0) 
			break;
		
		length_utf8 	-= pass_count;
		pointer_utf8 	+= pass_count;
		length_ucs2++;
		pointer_ucs2++;
	}
	return length_ucs2;
}


/**   
	@ingroup		String_Conversion
	@brief			This interface converts a UCS-2 string to UTF-8 string.
	@param		pusSource  	[IN] String to be converted.
	@param		pszTarget  	[IN] Destination string.
	@return
				@li	<= 0		:	On failure.
				@li	> 0		:	The number of characters converted.
	@warning		
				If #DRM_UCHAR is UTF-8 character, not UCS-2,
				all unicode functions will performs exactly the same functionalities with UTF-8.
				But this interface is not affected by #DRM_UCHAR data type.
				It always works as UCS-2 function.	
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
int drmLibFixedUCS2ToUTF8(const unsigned short *pusSource, char *pszTarget)
{
	int 	length_ucs2;
	int 	length_utf8;
	const unsigned short *pointer_ucs2;
	unsigned char*	pointer_utf8;
	int 	pass_count;

	if(!pszTarget || !pusSource)
	{
		return eDRM_FAILURE;
	}
	
	length_ucs2 	= drmLibUniFixedStrlen(pusSource)+1;//UniLib_Strlen(ucs2_string)+1;
	length_utf8 	= 0;
	pointer_ucs2 	= pusSource;
	pointer_utf8 	= pszTarget;
	while(length_ucs2>0)	// 방어코드 삽입
	{
		pass_count = Cvrt_UCS2_To_UTF8(*pointer_ucs2,pointer_utf8);
		if(pass_count == 0)
			break;
		length_ucs2--;
		pointer_utf8 += pass_count;
		length_utf8	 += pass_count;//[20041027]lima
		pointer_ucs2++;
	}
	
	/* ysch 20041104  NULL 문자는 길이에 더하지 말것 */
	return (length_utf8 -1);
}


/**   
	@ingroup		String_Conversion
	@brief			This interface converts a string to integer.
	@param		pszStr  	[IN] String to be converted.
	@return
				@li	< 0	:	The int value produced by interpreting the input characters as a number.
				@li	0		:	The input cannot be converted to a int value.
				@li	> 0	:	The int value produced by interpreting the input characters as a number.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
int drmLibAtoi(const char *pszStr)
{
	if(!pszStr)
	{
		return eDRM_FAILURE;
	}
	return atoi(pszStr);
}



/* ━━━━━━━━━━ [System ID] ━━━━━━━━━━ */

/**   
	@ingroup		System_ID
	@brief			This interface returns unique identifer for current subscriber. (USIM ID).
	@param		pucSIMId  	[OUT] Holds the subscriber id.
	@return
				@li	eDRM_FAILURE 	:	On failure.
				@li   eDRM_SUCCESS	:	On success.
	@warning	
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
int drmLibGetSIMId(unsigned char *pucSIMId)
{
#ifdef DRM_PC_EMULATOR
	{
		void *fp;

		unsigned char szFullPath[DRM_FILE_PATH_LEN+1] = {0};
		DRM_UCHAR ausFullPath[DRM_FILE_PATH_LEN+1] = {0};
	
		/* File Check */
		drmLibSprintf(szFullPath, "%s/%s", DRM_ID_PATH, DRM_IMSI_FILE);
		drmLibUTF8ToUCS2(szFullPath, ausFullPath);
		if (!drmLibFsFileExist(ausFullPath))
		{
			fp = drmLibFsOpen(ausFullPath, DRM_FS_CREATE);	/* Defual IMEI file creation */
			if (fp != NULL)
			{
				drmLibFsWrite(fp, "123456789012345", sizeof("123456789012345")-1);
				drmLibStrncpy(pucSIMId, "123456789012345", sizeof("123456789012345"));
			}
			else
			{
				return eDRM_FAILURE;
			}
		}
		else
		{
			unsigned char szIDs[DRM_IMSI_LEN+1] = {0};
			int nBufLen = 0;

			fp = drmLibFsOpen(ausFullPath, DRM_FS_READONLY);
			if (fp != NULL)
			{
				drmLibFsRead(fp, szIDs, DRM_IMSI_LEN);
				drmLibStrncpy(pucSIMId, szIDs, DRM_IMSI_LEN);
			}
			else
			{
				return eDRM_FAILURE;
			}
		}

		if (fp != NULL)
		{
			drmLibFsClose(fp);
		}
	}
#elif defined(DRM_PLATFORM_LIMO)
	{
		// TODO:
    DRM_LOG_LOW("%s: Not implemented yet!", __FUNCTION__);
    drmLibStrncpy((char*)pucSIMId, "123456789012345", sizeof("123456789012345"));
	}
#elif defined(DRM_PLATFORM_ANDROID)
	{
		DRM_ERROR("[DRMERR] drmLibGetSIMId() : Not implemented yet",0,0,0);
		drmLibStrncpy((char*)pucSIMId, "123456789012345", sizeof("123456789012345"));
	}
#endif

#ifdef DRM_SUPPORT_DBG
	if (g_DRM_Debug & eDRM_DBG_DUMP_IMSI)	
	{
		static int snSeed = 0;

		if (!snSeed)
		{
			void *fp = NULL;
			
			if (!drmLibFsFileExist(DRM_IMSI_DUMPFILE))
			{
				fp = drmLibFsOpen(DRM_IMSI_DUMPFILE, DRM_FS_CREATE);

				if (fp)
				{
					drmLibFsWrite(fp, pucSIMId, DRM_IMSI_LEN);
					drmLibFsClose(fp);
					fp = NULL;
				}
			}
			snSeed = 1;
		}
	}
#endif

	//DRM_LOG(("[DRM] drmLibGetSIMId(): Success"));

	return eDRM_SUCCESS;
}


/**   
	@ingroup		System_ID
	@brief			This interface gets process information.
	@param		eType        [OUT] Type of request.
	@param		pucSystemID  [IN/OUT] Information.
	@return
				@li	-1 	:	On failure.
				@li	0 	:	Not exist requsted information.
				@li	1 	:	On success.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2010/2/5]
 */ 
int drmLibGetProcessInfo(enDRMProcInfo eType, void *pvData)
{
	return 0;
}


/* ━━━━━━━━━━ [FILE] ━━━━━━━━━━ */

#ifdef DRM_SUPPORT_DBG
int nIOCount = 0;
#endif


/**   
	@ingroup		File
	@brief			This interface opens/creats a file.
	@param		pusFilename		[IN] Filename.
	@param		nAccessMode  	[IN] Type of access permitted. 
	@return
				@li	NULL 	:	On failure.
				@li	> 0		:	Address of the created/opened file.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
void *drmLibFsOpen(DRM_TSTR pusFilename, int nAccessMode)
{
	int fd = -1;
	
	if(!pusFilename)
	{
		return NULL;
	}
	/*
		Lothson.20090412
		If use stream file i/o, ex fopen, 
		cache buffer in c-lib layer should be controlled carefully using fflush and fclose.
		
		But current core implementation does not control flush.
		So use open, not fopen.

		Kernel space cache buffer will not be considered for now, ex fsync, O_SYNC.
	*/
	switch(nAccessMode)
	{
	case DRM_FS_CREATE:
		fd = open((const char *)pusFilename, O_RDWR | O_CREAT | O_EXCL, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
		#if defined(DRM_PLATFORM_ANDROID) && !defined(DRM_JNI_TEST)
		if (fd != -1)
		{
			/* Only for internal memory */
			if (0 != strncmp((const char *)pusFilename, DRM_MMC_ROOT_PATH, strlen(DRM_MMC_ROOT_PATH))
#ifdef DRM_MMC_ROOT_LINK_PATH
				&& 0 != strncmp((const char *)pusFilename, DRM_MMC_ROOT_LINK_PATH, strlen(DRM_MMC_ROOT_LINK_PATH))
#endif
				)
			{
				#ifndef DRM_MP_ENV
				/* Change owner and group to be AID_LGDRM and AID_LGDRM_ACC */
				fchown(fd, AID_LGDRM, AID_LGDRM_ACC);
				#else
				/* Change group to be AID_LGDRM_ACC */
				fchown(fd, (uid_t)-1, AID_LGDRM_ACC);
				#endif
			}
		}
		#endif
		break;

	case DRM_FS_READWRITE:
		fd = open((const char *)pusFilename, O_RDWR);
		break;

	case DRM_FS_READONLY:
		fd = open((const char *)pusFilename, O_RDONLY);
		break;

	default:
		fd = open((const char *)pusFilename, O_RDWR);
		break;
	}

	if (fd == -1)
	{
#ifdef DRM_SUPPORT_DBG
		if (errno == EACCES)
		{
			DRM_ERROR("[DRMERR] drmLibFsOpen() : Has no proper permission",0,0,0);
		}
		else if (errno == EEXIST)
		{
			DRM_ERROR("[DRMERR] drmLibFsOpen() : Already exist",0,0,0);
		}
		else
		{
			DRM_ERROR("[DRMERR] drmLibFsOpen() : errno = %u", errno,0,0);
		}
#endif
		return NULL;
	}

	/*
		Basic precondition
		Standard input, output and error will not be used.
		So fd 0~2 will not be in here.
	*/
#ifdef DRM_SUPPORT_DBG
	if ((g_DRM_Debug & eDRM_DBG_FSCOUNT))
	{
		nIOCount++;
	}
#endif

	return (void*)fd;
}


/**   
	@ingroup		File
	@brief			This interface gets the current file position.
	@param		hFile  	[IN] Filename. 
	@return
				@li	-1 		:	On failure.
				@li	>= 0		:	File position.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
int drmLibFsTell(void* hFile)
{
	if (!hFile)
	{
		return -1;
	}
	return (int)lseek((int)hFile, 0, SEEK_CUR);
}


/**   
	@ingroup		File
	@brief			This interface sets the file position for a opened file.
	@param		hFile  		[IN] Filename. 
	@param		nOffset  	[IN] Position to seek.
	@param		nWhence 	[IN] From where to seek.
	@return
				@li	0 	:	On failure.
				@li	1	:	On Success.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
int drmLibFsSeek(void* hFile, int nOffset, int nWhence)
{
	int nRetVal;
	
	if (hFile == NULL)
	{
		return 0;
	}
	
	nRetVal = lseek((int)hFile, nOffset, nWhence);
	if (nRetVal == -1)
	{
		return 0;
	}

	return 1;
}


/**   
	@ingroup		File
	@brief			This interface closes an opened file.
	@param		hFile  	[IN] Filename.
	@return
				@li	0	:	On failure.
				@li   1	:	On success.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
int drmLibFsClose(void* hFile)
{
	int nRetVal = 0;
	
	if (!hFile)
	{
		return 0;
	}

#ifdef DRM_SUPPORT_DBG
	if (g_DRM_Debug & eDRM_DBG_FSCOUNT)
	{
		nIOCount--;
		DRM_LOG_LOW("[DRM] IO Debug Count = %d", nIOCount, 0, 0);
	}
#endif

	nRetVal = close((int)hFile);
	
	return DRM_RV_CONV_LINUX2DRM(nRetVal);
}


/**   
	@ingroup		File
	@brief			This interface reads data from an opened file.
	@param		hFile  		[IN] Filename.
	@param		pucBuf  	[OUT] Storage location for data. 
	@param		uSize  		[IN] Item size in bytes.
	@return
				@li	-1	:	On failure.
				@li	0		:	End of file.
				@li	> 0	:	Number of bytes actually read.
	@warning		
	@par PreCondition	
				File should be opened.
	@par PostCondition	
				Read content is available in the buffer pointer "cpBuffer".
	@par Example
				@code
				@endcode 
	@sa 	
				
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
int drmLibFsRead(void* hFile, unsigned char *pucBuf, unsigned int uSize)
{
	if(!hFile||!pucBuf||!uSize)
	{
		return -1;
	}
	return read((int)hFile, pucBuf, uSize);
}


/**   
	@ingroup		File
	@brief			This interface writes data to an opened file.
	@param		hFile  		[IN] Filename.
	@param		pucBuf  	[IN] Pointer to data to be written. 
	@param		uSize  		[IN] Item size in bytes. 
	@return
				@li	<= 0		:	On failure.
				@li	> 0		:	Number of bytes actually writeen.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
int drmLibFsWrite(void* hFile, unsigned char *pucBuf, unsigned int uSize)
{
	int nRetVal;
	
	if(!hFile||!pucBuf||!uSize)
	{
		return 0;
	}

	//return write((int)hFile, pucBuf, uSize);
	nRetVal = write((int)hFile, pucBuf, uSize);
	
#if defined(DRM_PLATFORM_ANDROID)
	/*
		Lothson.20090708
		Android doesn't support flushing write buffer to a file without explicit fsync().
		But actuall fsync can't also trust whether the buffer really reached to disk.
		It should be ensured by platform team.
	*/
	if (nRetVal != -1)
	{
		if (-1 == fsync((int)hFile))
		{
			DRM_ERROR("[DRMERR] drmLibFsWrite() : Fail to fsync()",0,0,0);
		}
	}
#endif

	return nRetVal;
}


/**   
	@ingroup		File
	@brief			This interface gets the size of a file based on given file name.
	@param		pusFilename  	[IN] Filename.
	@return
				@li	-1		:	On failure.
				@li	>= 0 	:	Size of the file.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
unsigned int drmLibFsFileSize(DRM_TSTR pusFilename)
{
	struct stat stStat = {0};
	int nRetVal;
	
	if (!pusFilename)
	{
		return (unsigned int)-1;
	}

	/* Does not support symbolic link */
	nRetVal = lstat((const char*)pusFilename, &stStat);
	if (nRetVal == -1)
	{
		return (unsigned int)-1;
	}
	
	return stStat.st_size;
}


/**   
	@ingroup		File
	@brief			This interface checks for the existence of a file.
	@param		pusFilename  	[IN] Filename.
	@return
				@li	0	:	Not exist.
				@li	1	:	Exist.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
int drmLibFsFileExist(DRM_TSTR pusFilename)
{
	if(!pusFilename)
	{
		return 0;
	}	
	return DRM_RV_CONV_LINUX2DRM(access((const char*)pusFilename, F_OK));
}

 
/**   
	@ingroup		File
	@brief			This interface removes a file .
	@param		pusFilename  	[IN] Filename.
	@return
				@li	0	:	On failure.
				@li	1	:	On success.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
int drmLibFsRemove(DRM_TSTR pusFilename)
{
	if(!pusFilename)
	{
		return 0;
	}
	return DRM_RV_CONV_LINUX2DRM(remove((const char*)pusFilename));
}

 
/**   
	@ingroup		File
	@brief			This interface renames a file.
 	@param		cpOldName  		[IN] Pointer to old name. 
	@param		cpNewName  	[IN] Pointer to new name. 
	@return
				@li	0	:	On failure.
				@li	1	:	On success.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
int drmLibFsRename(DRM_TSTR pusOldFName, DRM_TSTR pusNewFName)
{
	if(!pusOldFName||!pusNewFName)
	{
		return 0;
	}
	return DRM_RV_CONV_LINUX2DRM(rename((const char*)pusOldFName, (const char*)pusNewFName));
}

 
/**   
	@ingroup		File
	@brief			This interface truncates a file.
	@param		hFile  	[IN] Filename.
	@param		uSize  	[IN] The byte length to which the hFile should be truncated.
	@return
				@li	0	:	On failure.
				@li	1	:	On success.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
int drmLibFsTruncate(void* hFile, unsigned int uSize)
{
	if(!hFile)
	{
		return 0;
	}
	return DRM_RV_CONV_LINUX2DRM(ftruncate((int)hFile, uSize));
}


/**   
	@ingroup		File
	@brief			This interface flush a file.
	@param			hFile  	[IN] File handle.
	@return
				@li	1	:	On success.
				@li	0	:	On failure.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2009/6/16]
 */ 
int drmLibFsFlush(void* hFile)
{
	return DRM_RV_CONV_LINUX2DRM(fsync((int)hFile));
}


/**   
	@ingroup		File
	@brief			This interface gets the size of a file based on file handle.
	@param		hFile  	[IN] Filename.
	@return
			@li	-1		:	On failure.
			@li	>= 0 	:	Size of the file.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
unsigned int drmLibFsGetSize (void *hFile)
{
	struct stat stStat = {0};
	int nRetVal;
	
	if(!hFile)
	{
		return (unsigned int)-1;
	}
	
	nRetVal = fstat((int)hFile, &stStat);
	if (nRetVal == -1)
	{
		return (unsigned int)-1;
	}
	
	return stStat.st_size;
}


/**   
	@ingroup		File
	@brief			This interface gets the current file position.
	@param		hFile  	[IN] Filename.
	@return
				@li	-1 		:	On failure.
				@li	>= 0		:	File position.
 	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
int drmLibFsGetPos(void *hFile)
{
	if (!hFile)
	{
		return -1;
	}
	return (int)lseek((int)hFile, 0, SEEK_CUR);
}


/**   
	@ingroup		File
	@brief			This interface copies a file.
	@param		pusOldFName  		[IN] Source file.
	@param		pusNewFName  	[IN] Destination file.
	@return
				@li	0	:	On failure.
				@li	1	:	On success.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/13]
 */ 
int drmLibFsCopyFile(DRM_TSTR pusOldFName, DRM_TSTR pusNewFName)
{
	unsigned char *pucData;
	void *pOldFile;
	void *pNewFile;
	int nSize;
	
	if(!pusOldFName||!pusNewFName)
	{
		return 0;
	}
	if(!drmLibFsFileExist(pusOldFName))
	{
		return 0;
	}

	pucData = drmLibMalloc(1024);
	if( !pucData )
	{
		return 0;
	}

	pOldFile = drmLibFsOpen(pusOldFName, DRM_FS_READONLY);
	if( !pOldFile )
	{
		drmLibFree(pucData);
		return 0;
	}
	
	pNewFile = drmLibFsOpen(pusNewFName, DRM_FS_CREATE);
	if( !pNewFile )
	{
		drmLibFree(pucData);
		drmLibFsClose(pOldFile);
		return 0;
	}

	while((nSize = drmLibFsRead(pOldFile, pucData, 1024)) > 0)
	{
		drmLibFsWrite(pNewFile, pucData, nSize);
	}

	drmLibFsClose(pNewFile);
	drmLibFsClose(pOldFile);
	drmLibFree(pucData);

	return 1;
}


/**   
	@ingroup		File
	@brief			This interface moves a file.
	@param		pusOldFName  		[IN] The current name of the file.
	@param		pusNewFName  	[IN] The new name for the file.
	@return
				@li	0	:	On failure.
				@li	1	:	On success.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/16]
 */ 
int drmLibFsMoveFile(DRM_TSTR pusOldFName, DRM_TSTR pusNewFName)
{
	if(!pusOldFName||!pusNewFName)
	{
		return 0;
	}
	return DRM_RV_CONV_LINUX2DRM(rename((const char*)pusOldFName, (const char*)pusNewFName));
}

 
/**   
	@ingroup		File
	@brief			This interface opens directory stream and read first file entry.
	@param		pusDirPath  	[IN]  Directory name.
	@param		pnIterator  	[OUT] Pointer to store iterator.
	@param		FStat  		[OUT] Pointer to structure that stores results.
	@return
				@li	0	:	On failure.
				@li	1	:	On success.
	@warning		
				This inteface returns Iterator to read next entry from directory stream.
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/16]
 */ 
int drmLibFsFindFirstFile(DRM_TSTR pusDirPath, int *pnIterator, DRMTFStat *pstFStat)
{
	DIR *d;
	struct dirent *de;

	if(!pusDirPath || !pnIterator || !pstFStat)
	{
		return 0;
	}

	d = opendir((const char*)pusDirPath);
	if (!d)
	{
		DRM_ERROR("[DRMERR] drmLibFsFindFirstFile() : Fail to open Dir",0,0,0);
		return 0;
	}

	de = readdir (d);
	if (!de)
	{
		DRM_ERROR("[DRMERR] drmLibFsFindFirstFile() : Fail to read Dir",0,0,0);
		closedir(d);
		return 0;
	}
	
	*pnIterator = (int)d;

	drmLibUniStrcpy(pstFStat->szName, (DRM_TCSTR)de->d_name);
	pstFStat->bFile = (de->d_type == DT_REG)? 1 : 0;
	//pstFStat->iSize = drmLibFsFileSize((DRM_TSTR)de->d_name);
	pstFStat->iSize = 1; // TODO: Lothson.20090413 Not used at all. What to do for performance ??
	
	return 1;
}

 
/**   
	@ingroup		File
	@brief			This interface reads next file entry from directory stream.
	@param		nIterator  	[IN] Interator from drmLibFsFindFirstFile()
	@param		pstFStat  		[OUT] Pointer to structure that stores results.
	@return
				@li	0	:	On failure.
				@li	1	:	On success.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
				drmLibFsFindFirstFile()
	@author		LGE DRM TEAM [Modify date : 2008/6/16]
 */ 
int drmLibFsFindNextFile(int nIterator, DRMTFStat *pstFStat)
{
	DIR *d;
	struct dirent *de;

	d = (DIR *)nIterator;
	if (!d)
	{
		return 0;
	}
	
	de = readdir(d);
	if (!de)
	{
		return 0;
	}

	drmLibUniStrcpy((DRM_TSTR)pstFStat->szName, (DRM_TCSTR)de->d_name);
	pstFStat->bFile = (de->d_type == DT_REG)? 1 : 0;
	//pstFStat->iSize = drmLibFsFileSize((DRM_TSTR)de->d_name);
	pstFStat->iSize = 1; // TODO: Lothson.20090413 Not used at all. What to do for performance ??
	
	return 1;
}

 
/**   
	@ingroup		File
	@brief			This interface closes directory stream.
	@param		Iterator  	[IN] Interator from drmLibFsFindFirstFile() or drmLibFsFindFirstDir().
	@return
	@return
				@li	0	:	On failure.
				@li	1	:	On success.
	@warning	
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
				drmLibFsFindFirstFile(), drmLibFsFindFirstDir()
	@author		LGE DRM TEAM [Modify date : 2008/6/16]
 */ 
int drmLibFsFindClose(int nIterator)
{
	if(!nIterator)
	{
		return 0;
	}
	return DRM_RV_CONV_LINUX2DRM(closedir((DIR *)nIterator));
}

 
/**   
	@ingroup		File
	@brief			This interface checks for the existence of a directory.
	@param		pusDirname  	[IN] Directory name.
	@return
				@li	0	:	Not exist.
				@li	1	:	Exist.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/18]
 */ 
int drmLibFsSearchDir(DRM_TSTR pusDirname)
{
	if(!pusDirname)
	{
		return 0;
	}
	return DRM_RV_CONV_LINUX2DRM(access((const char*)pusDirname, F_OK));
}

 
/**   
	@ingroup		File
	@brief			This interface creates a directory.
	@param		pusDirname  	[IN] Directory name.
	@return
				@li	0	:	On failure.
				@li	1	:	On success.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/18]
 */ 
int drmLibFsCreateDir(DRM_TSTR pusDirname)
{
	if(!pusDirname)
	{
		return 0;
	}
	
#if defined(DRM_PLATFORM_LIMO) || defined(DRM_JNI_TEST)
	return DRM_RV_CONV_LINUX2DRM(mkdir((const char*)pusDirname, S_IRWXU|S_IRWXG));
#elif defined(DRM_PLATFORM_ANDROID)
	if (0 == mkdir((const char*)pusDirname, S_IRWXU|S_IRWXG))
	{
		/* Only for internal memory */
		if (0 != strncmp((const char *)pusDirname, DRM_MMC_ROOT_PATH, strlen(DRM_MMC_ROOT_PATH))
#ifdef DRM_MMC_ROOT_LINK_PATH
				&& 0 != strncmp((const char *)pusDirname, DRM_MMC_ROOT_LINK_PATH, strlen(DRM_MMC_ROOT_LINK_PATH))
#endif
			)
		{
			#ifndef DRM_MP_ENV
			/* Change owner and group to be AID_LGDRM and AID_LGDRM_ACC */
			chown((const char*)pusDirname, AID_LGDRM, AID_LGDRM_ACC);
			#else
			/* Change group to be AID_LGDRM_ACC */
			chown((const char*)pusDirname, (uid_t)-1, AID_LGDRM_ACC);
			#endif
		}
		return 1;
	}
	return 0;
#endif
}


 
/**   
	@ingroup		File
	@brief			This interface opens directory stream and read first directory entry.
	@param		pusDirPath	[IN] Directory name.
	@param		pnIterator  	[OUT] Pointer to store iterator.
	@param		pstFStat  		[OUT] Pointer to structure that stores results.
	@return
				@li	0	:	On failure.
				@li	1	:	On success.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/18]
 */ 
int drmLibFsFindFirstDir(DRM_TSTR pusDirPath, int *pnIterator, DRMTFStat *pstFStat)
{
	return drmLibFsFindFirstFile(pusDirPath, pnIterator, pstFStat);
}

/**   
	@ingroup		File
	@brief			This interface reads next directory entry from directory stream.
	@param		nIterator  	[IN] Interator from drmLibFsFindFirstDir()
	@param		pstFStat  		[OUT] Pointer to structure that stores results.
	@return
				@li	0	:	On failure.
				@li	1	:	On success.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
				drmLibFsFindFirstDir()
	@author		LGE DRM TEAM [Modify date : 2008/6/18]
 */ 
int drmLibFsFindNextDir(int nIterator, DRMTFStat *pstFStat)
{
	return drmLibFsFindNextFile(nIterator, pstFStat);
}

 
/**   
	@ingroup		File
	@brief			This interface get free size of file system.
	@param		pusMountPath  	[IN] Mount(file system) path
	@return
				@li	<= 0		:	No more free space exists.
				@li	> 0		:	Size of free space in byte.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/18]
 */ 
DRMUINT64 drmLibFsGetFreeSpace(DRM_TSTR pusMountPath)
{
	DRMUINT64 uFree;
	struct statfs stStatFs = {0};

	if (-1 == statfs((const char *)pusMountPath, &stStatFs))
	{
		return 0;
	}

	uFree = (DRMUINT64)(stStatFs.f_bfree * stStatFs.f_bsize);
	return uFree;
}

/**   
	@ingroup		File
	@brief			This interface get free size of given file system type.
	@param		eFSType  	[IN] File system type
	@return
				@li	<= 0		:	No more free space exists.
				@li	> 0		:	Size of free space in byte.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/18]
 */ 
DRMUINT64 drmLibFsGetFreeSpaceByType(enDRMFSType eFSType)
{
	if (eFSType == eDRM_FSTYPE_UNKNOW)
	{
		return 0;
	}

	if (eFSType == eDRM_FSTYPE_INT)
	{
#if defined(DRM_PLATFORM_LIMO)
		return drmLibFsGetFreeSpace((DRM_TSTR)DRM_T(LIMO_DRM_INTERNAL_STORAGE_ROOT_DIR));
#elif defined(DRM_PLATFORM_ANDROID)
		// TODO: Lothson.20090413 double check again on target.
		return drmLibFsGetFreeSpace((DRM_TSTR)"/data");
#endif
	}

	return drmLibFsGetFreeSpace((DRM_TSTR)DRM_MMC_ROOT_PATH);
}

/* ━━━━━━━━━━ [MMC] ━━━━━━━━━━ */
 
/**   
	@ingroup		Memory_Card
	@brief			This interface checks the existence of external memory card.
	@param		pusStatus  	[OUT] Status of external memory card.
	@return
	@warning		
				If external memory card was attached, Status is 1.
				@n If not, Status is 0.
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/18]
 */ 
void drmLibMMC_GetState(unsigned short *pusStatus)
{
	if (!pusStatus)
	{
		return;
	}

#ifdef DRM_PC_EMULATOR
	if (drmLibFsSearchDir(DRM_MMC_ROOT_PATH))
	{
		*pusStatus = 1;
	}
	else
	{
		*pusStatus = 0;
	}
	return;
#endif

#if defined(DRM_PLATFORM_LIMO)
	{
		int nStatus = 0;
		devmgr_mmc_get_status((int *)&nStatus);
		*pusStatus = (unsigned short)nStatus;
	}
#elif defined(DRM_PLATFORM_ANDROID)
	{
		/*
			Lothson.20090414
			Read /proc/mounts is the robust way to check mount status.
			Checking the statfs' f_type for vfat can be another solution.
			But I'm not sure whether f_type alway has same value for various kinds of mmc cards.
		*/
		FILE *fp;
		unsigned char buf[512]; /* Will not exceed 512 Byte */

		*pusStatus = 0;
		
		fp = fopen("/proc/mounts", "r");
		if (fp == NULL)
		{
			DRM_ERROR("[DRMERR] drmLibMMC_GetState() : Fail to open /proc/mounts",0,0,0);
			*pusStatus = 0;
			return;
		}
		
		while (fgets(buf, sizeof(buf), fp))
		{
			/* Check mount path. Double check /system/etc/mountd.conf again */
			if (strstr(buf, DRM_MMC_ROOT_PATH))
			{
				*pusStatus = 1;
				break;
			}
		}
		fclose(fp);
	}
#endif
}

 
/**   
	@ingroup		Memory_Card
	@brief			This interface get total size of external memory card.
   
	@param		pllMMC_size  	[OUT] Size in byte.
	@return
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/18]
 */ 
void drmLibMMC_GetSize(DRMUINT64 *pllMMC_size)
{
	if (!pllMMC_size)
	{
		return;
	}

#if defined(DRM_PLATFORM_LIMO)
	/* TODO - model dependant */
	DRM_ERROR("[DRMERR] drmLibMMC_GetSize() : Not implemented yet",0,0,0);
	*pllMMC_size = 0;
#elif defined(DRM_PLATFORM_ANDROID)
	*pllMMC_size = drmLibFsGetFreeSpace((DRM_TSTR)DRM_MMC_ROOT_PATH);
#endif
}

/* ━━━━━━━━━━ [TIME] ━━━━━━━━━━ */


/**   
	@ingroup		Time
	@brief			This interface get current user time and return it as structure.
	@param		pvTime  	[OUT]  Void pointer to structure that stores results. 
	@return
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/18]
 */ 
void drmLibUserTime_GetST(void *pvTime)
{
	stDRMTime *pstTime = (stDRMTime *)pvTime;
	time_t utcSec;
	struct tm *tm;

	if (!pvTime)
	{
		return;
	}
	
	utcSec = time(NULL);
	if (utcSec == -1)
	{
		DRM_ERROR("[DRMERR] drmLibUserTime_GetST() : Failed to get time",0,0,0);
		return;
	}
	
	tm = localtime(&utcSec);
	if (!tm)
	{
		DRM_ERROR("[DRMERR] drmLibUserTime_GetST() : Failed to get localtime",0,0,0);
		return;
	}
	
	pstTime->tm_year = tm->tm_year+1900;
	pstTime->tm_mon = tm->tm_mon+1;
	pstTime->tm_mday = tm->tm_mday;
	pstTime->tm_wday = tm->tm_wday >= 1 ? (tm->tm_wday -1) : 6;

	pstTime->tm_hour = tm->tm_hour;
	pstTime->tm_min = tm->tm_min;
	pstTime->tm_sec = tm->tm_sec;

	return;
}

/**   
	@ingroup		Time
	@brief			This interface get current user time and convert it as GMT seconds.
	@param		void : (None)  
	@return
				@li	0		:	On failure.
				@li	> 0	:	GMT seconds.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/18]
 */ 
unsigned int drmLibUserTime_GetUTCSec(void)
{
	stDRMTime stTime = {0};

	drmLibUserTime_GetST(&stTime);

	return drmLibTime_ST2UTCSec(&stTime);
}


/**   
	@ingroup		Time
	@brief			This interface converts given time to GMT seconds.
	@param		pvTime  	[IN] Time. 
	@return
				@li	0		:	On failure.
				@li	> 0	:	GMT seconds.
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/18]
 */ 
unsigned int drmLibTime_ST2UTCSec(void *pvTime)
{
	stDRMTime *pstTime = (stDRMTime *)pvTime;
	unsigned int  uTmpNum =0;
	int  nLeapYears =0;
	int  nTmpYear =0;

	if (!pvTime)
	{
		return 0;
	}

	uTmpNum = (pstTime->tm_year - DRM_EPOCH_YEAR) * (DRM_YEAR_DAYS * DRM_SECONDS_IN_DAY);

	/* If we haven't had this year's leap day yet, pretend it's
	* the previous year. 
	*/
	nTmpYear = pstTime->tm_year;

	if (pstTime->tm_mon <= DRM_MONTH_FEB)
	{
		nTmpYear--;
	}

	/* Add leap years since 1970.  The magic number 477 is the value
	* this formula would give for 1970 itself.  Notice the extra
	* effort we make to keep it correct for the year 2100. 
	*/
	nLeapYears = DRM_NO_OF_LEAP_YEARS (nTmpYear);

	uTmpNum += nLeapYears * DRM_SECONDS_IN_DAY;

	uTmpNum += DRM_DAYSUM_OF_MONTH[pstTime->tm_mon - 1] * DRM_SECONDS_IN_DAY;
	//uTmpNum += DAYSUM_OF_MONTH[nMonth] * DRM_SECONDS_IN_DAY;

	uTmpNum += (pstTime->tm_mday - 1) * DRM_SECONDS_IN_DAY;
	uTmpNum += pstTime->tm_hour * DRM_SECONDS_IN_HOUR;
	uTmpNum += pstTime->tm_min * DRM_SECONDS_IN_MINUTE;
	uTmpNum += pstTime->tm_sec;

	return uTmpNum;
}


/**   
	@ingroup		Time
	@brief			This interface converts given GMT seconds as structure format.
   
	@param		uUTCSec  	[IN]  GMT seconds.
	@param		pvTime  	[OUT]  Void pointer to structure that stores converted time. 
	@return
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/18]
 */ 
void drmLibTime_UTCSec2ST(unsigned int uUTCSec, void *pvTime)
{
	stDRMTime *timep = (stDRMTime *)pvTime;

	unsigned int time = uUTCSec;

	unsigned long dayclock, dayno;
	int year = EPOCH_YR;

	if (!pvTime || !uUTCSec)
	{
		return;
	}
	
	dayclock = (unsigned long)time % SECS_DAY;
	dayno = (unsigned long)time / SECS_DAY;

	timep->tm_sec = dayclock % 60;
	timep->tm_min = (dayclock % 3600) / 60;
	timep->tm_hour = dayclock / 3600;
	timep->tm_wday = (dayno + 4) % 7;       /* day 0 was a thursday */
	while (dayno >= (unsigned long)YEARSIZE(year))
	{
		dayno -= YEARSIZE(year);
		year++;
	}
	timep->tm_year = year - YEAR0;
	timep->tm_yday = dayno;
	timep->tm_mon = 0;
	while (dayno >= (unsigned long)_ytab[LEAPYEAR(year)][timep->tm_mon])
	{
		dayno -= _ytab[LEAPYEAR(year)][timep->tm_mon];
		timep->tm_mon++;
	}
	timep->tm_mday = dayno + 1;

	/* Lothson.20070723 Covert to Normal format */
	timep->tm_year += YEAR0;
	timep->tm_mon++;
}


/**   
	@ingroup		Time
	@brief			This interface prints out GMT seconds as readable string.
	@param		uUTCSec  		[IN] GMT seconds.
	@param		pszPrefix  	[IN] Prefix value to prefix readable string.
	@return
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/18]
 */ 
void drmLibTime_PrintUTCSec(unsigned int uUTCSec, unsigned char *pszPrefix)
{
	stDRMTime timep = {0};

	unsigned int time = uUTCSec;

	unsigned long dayclock, dayno;
	int year = EPOCH_YR;

	if (!uUTCSec || !pszPrefix)
	{
		return;
	}
	
	dayclock = (unsigned long)time % SECS_DAY;
	dayno = (unsigned long)time / SECS_DAY;

	timep.tm_sec = dayclock % 60;
	timep.tm_min = (dayclock % 3600) / 60;
	timep.tm_hour = dayclock / 3600;
	timep.tm_wday = (dayno + 4) % 7;       /* day 0 was a thursday */
	while (dayno >= (unsigned long)YEARSIZE(year))
	{
		dayno -= YEARSIZE(year);
		year++;
	}
	timep.tm_year = year - YEAR0;
	timep.tm_yday = dayno;
	timep.tm_mon = 0;
	while (dayno >= (unsigned long)_ytab[LEAPYEAR(year)][timep.tm_mon])
	{
		dayno -= _ytab[LEAPYEAR(year)][timep.tm_mon];
		timep.tm_mon++;
	}
	timep.tm_mday = dayno + 1;

	/* Lothson.20070723 Covert to Normal format */
	timep.tm_year += YEAR0;
	timep.tm_mon++;

	drmLibWriteLog("[DRM] UTC of %s : %4d:%2d:%2d T %2d:%2d:%2d", pszPrefix, 
		timep.tm_year, timep.tm_mon, timep.tm_mday, timep.tm_hour, timep.tm_min,timep.tm_sec);
}


/**   
	@ingroup		Time
	@brief			This interface get tick count of system.
	@param		void : (None)  
	@return
				@li	> 0	:	Tick counts.
	@warning		Desc
	@par PreCondition	
				Desc
	@par PostCondition	
				Desc
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/18]
 */ 
unsigned int drmLibGetTickCount(void)
{
	/* 
		kybae 2009.03.06 
		using system time instead of time tick count,
		because this API is used performance checking only
	*/
	unsigned int uMSTime = 0;
	
	struct timeval tv;

	if (gettimeofday(&tv, NULL) == -1)
	{
		DRM_ERROR("[DRMERR] drmLibGetTickCount() : Failed to gettimeofday",0,0,0);
		return 0;
	}

	uMSTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;

	return uMSTime;
}

/* ━━━━━━━━━━ [ETC] ━━━━━━━━━━ */


/**   
	@ingroup		Etc
	@brief			This interface prints out data as readable string to debugging console.
	@param		pszFormat  	[IN] Format-control string.
	@param		...  					[IN] Optional arguments.
	@return
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/18]
 */ 
void drmLibWriteLog(char* pszFormat, ...)
{
	char szBuf[DRM_LOG_BUF_LEN+1];
	va_list VaList;

	va_start(VaList, pszFormat);
	vsnprintf(szBuf, DRM_LOG_BUF_LEN-1, pszFormat, VaList);
	va_end(VaList);

#if defined(DRM_PLATFORM_LIMO) && !defined(DRM_PC_EMULATOR)
	strcat(szBuf, "\n");
#elif defined(DRM_PLATFORM_ANDROID) && defined(DRM_JNI_TEST)
	strcat(szBuf, "\n");
#endif

#if defined(DRM_PLATFORM_LIMO)
	//syslog(LOG_INFO, szBuf);
	printf(szBuf);
#elif defined(DRM_PLATFORM_ANDROID)
	LOGI(szBuf);
#endif
}


/**
	@ingroup	Etc
	@brief		This interface prints out data as readable string to debugging console with additiona information.
	@param		eLevel	[IN] Level of log.
	@param		pszFile	[IN] File name of caller.
	@param		nLine		[IN] Line number of caller.
	@param		pszFormat	[IN] Format-control string.
	@param		arg1	[IN] Argument 1.
	@param		arg2	[IN] Argument 2.
	@param		arg2	[IN] Argument 3.
	@return
	@warning
	@par PreCondition
	@par PostCondition
	@par Example
				@code
				@endcode
	@sa
	@author		LGE DRM TEAM [Modify date : 2009/9/28]
*/
void drmLibWriteLevelLog(enDRMLOGLV eLevel, char *pszFile, int nLine, char* pszFormat, int arg1, int arg2, int arg3)
{
	char szBuf[DRM_LOG_BUF_LEN+1];

	snprintf(szBuf, DRM_LOG_BUF_LEN-1, pszFormat, arg1, arg2, arg3);

#if defined(DRM_PLATFORM_LIMO) && !defined(DRM_PC_EMULATOR)
	strcat(szBuf, "\n");
#elif defined(DRM_PLATFORM_ANDROID) && defined(DRM_JNI_TEST)
	strcat(szBuf, "\n");
#endif

#if defined(DRM_PLATFORM_LIMO)
	switch (eLevel)
	{
	case eDRM_LOGLV_LOW:
		//syslog(LOG_INFO, szBuf);
		printf(szBuf);
		break;
	case eDRM_LOGLV_MID:
		//syslog(LOG_NOTICE, szBuf);
		printf(szBuf);
		break;
	case eDRM_LOGLV_HIGH:
		//syslog(LOG_CRIT, szBuf);
		printf(szBuf);
		break;
	case eDRM_LOGLV_ERR:
		//syslog(LOG_EMERG, szBuf);
		printf(szBuf);
		break;
	}
#elif defined(DRM_PLATFORM_ANDROID)
	switch (eLevel)
	{
	case eDRM_LOGLV_LOW:
		LOGV(szBuf);
		break;
	case eDRM_LOGLV_MID:
		LOGD(szBuf);
		break;
	case eDRM_LOGLV_HIGH:
		LOGI(szBuf);
		break;
	case eDRM_LOGLV_ERR:
		LOGE(szBuf);
		break;
	}
#endif

	if (g_DRM_Debug & eDRM_DBG_FILE_LOG)
	{
		drmLibFileLog(pszFormat, arg1, arg2, arg3);
	}
}


/**   
	@ingroup		Etc
	@brief			This interface prints out UCS-2 string to debugging console.
	@param		pszData  	[IN] UCS-2 string.
	@return
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/18]
 */ 
void drmLibWriteLogUni(DRM_TSTR pusData)
{
	unsigned char pBuf[DRM_MAX_BUF_FULLPATHLEN+1]={0,};
	int iBufLen;
	
	if(!pusData)
	{
		return;
	}

	iBufLen = drmLibUniStrlen(pusData);
	if(iBufLen < 1 || iBufLen > DRM_FILE_PATH_LEN-1)
	{
		return;
	}
	drmLibUCS2ToUTF8(pusData,pBuf);
	pBuf[iBufLen] = 0;

	drmLibWriteLog("[DRM] UNI_STR: %s", pBuf);
	
	if (g_DRM_Debug & eDRM_DBG_FILE_LOG)
	{
		drmLibFileLog("[DRM] UNI_STR: %s", pBuf);
	}
}


/**   
	@ingroup		Etc
	@brief			This interface prints out data as readable string to a file.
	@param		pszFormat  	[IN] Format-control string.
	@param		...  			[IN] Optional arguments.
	@return
	@warning		
	@par PreCondition	
	@par PostCondition	
	@par Example
				@code
				@endcode 
	@sa 	
	@author		LGE DRM TEAM [Modify date : 2008/6/18]
 */ 
void drmLibFileLog(char *pszFormat, ...)
{
#ifdef DRM_DEBUG_FILELOG
	static int bRecursive = 0;
	void *fp = NULL;
	int nLen;

	char szBuf[DRM_LOG_BUF_LEN+1];
	va_list VaList;

	if (!pszFormat)
	{
		return;
	}

	if (bRecursive == 1)
	{
		/*
			DRM_LOG() -> drmLibFileLog -> DRM_LOG() -> Fault.
			To prevent recursive call, use this flag.
		*/
		return;
	}
	bRecursive = 1;

	va_start(VaList, pszFormat);
	vsnprintf(szBuf, DRM_LOG_BUF_LEN, pszFormat, VaList);
	va_end(VaList);

	nLen = drmLibStrlen(szBuf);
	if(!nLen)
	{
		bRecursive = 0;
		return;
	}

	fp = drmLibFsOpen(DRM_DEBUG_FILELOG, DRM_FS_READWRITE);
	if (!fp)
	{
		fp = drmLibFsOpen(DRM_DEBUG_FILELOG, DRM_FS_CREATE);
		if (!fp)
		{
			bRecursive = 0;
			return;
		}
	}

	drmLibFsSeek(fp, 0, DRM_FS_SEEK_END);
	drmLibFsWrite(fp, szBuf, nLen);	
	drmLibFsWrite(fp, "\r\n", 2);
	drmLibFsClose(fp);
	bRecursive = 0;
#endif
}


#if defined(DRM_PLATFORM_ANDROID)
static int readFlex(char *pszStrIndx, char *pszFlexValue)
{
	void *fp;
	int nSize;
	int nCount;
	unsigned char *pszBuf;
	unsigned char *pszStart;
	
	if (!pszStrIndx || !pszFlexValue)
	{
		DRM_ERROR("[DRMERR] readFlex() : Invalid argument",0,0,0);
		return eDRM_FAILURE;
	}

	/* Read flex file */
	fp = drmLibFsOpen(DRM_FLEX_XML, DRM_FS_READONLY);
	if(!fp)
	{
		DRM_ERROR("[DRMERR] readFlex() : Fail to open flex",0,0,0);
		return eDRM_FAILURE;
	}

	nSize = drmLibFsGetSize(fp);
	if (nSize <= 0)
	{
		DRM_ERROR("[DRMERR] readFlex() : File size error",0,0,0);
		drmLibFsClose(fp);
		return eDRM_FAILURE;
	}

	pszBuf = drmLibCalloc(nSize+1);
	if (!pszBuf)
	{
		DRM_ERROR("[DRMERR] readFlex() : Malloc failure (%d line)",__LINE__,0,0);
		drmLibFsClose(fp);
		return eDRM_FAILURE;
	}

	if (nSize != drmLibFsRead(fp, pszBuf, nSize))
	{
		DRM_ERROR("[DRMERR] readFlex() : read failure",0,0,0);
		drmLibFsClose(fp);
		drmLibFree(pszBuf);
		return eDRM_FAILURE;
	}
	drmLibFsClose(fp);

	/* Find attribute */
	pszStart = drmLibStrstr(pszBuf, pszStrIndx);
	if (!pszStart)
	{
		DRM_ERROR("[DRMERR] readFlex() : Not exist specified name",0,0,0);
		drmLibFree(pszBuf);
		return eDRM_FAILURE;
	}

	/* Skip attribute name */
	pszStart = pszStart + drmLibStrlen(pszStrIndx);
	nSize -= (pszStart - pszBuf);

	/* Skip =" */
	for (; nSize > 0; nSize--)
	{
		if (pszStart[0] == ' ' || pszStart[0] == '=' || pszStart[0] == '"')
		{
			pszStart += 1;
		}
		else
		{
			break;
		}
	}

	/* Skip " */
	for (nCount = 0; nSize > 0; nSize--, nCount++)
	{
		if (pszStart[nCount] == '"')
		{
			pszStart[nCount] = 0;
			break;
		}
	}

	/* Copy value */
	drmLibStrcpy(pszFlexValue, pszStart);
	drmLibFree(pszBuf);
	
	return eDRM_SUCCESS;
}
#endif

/**
	@ingroup	Solution_Configuration
	@brief		This interface return flex information from the system.
	@param		eType		[IN] Information type.
	@param		pvData	[OUT] Pointer to store information.
	@return
				@li	eDRM_FAILURE	:	On failure.
				@li	eDRM_SUCCESS	:	On success.
	@warning
	@par PreCondition
	@par PostCondition
	@par Example
				@code
				@endcode
	@sa
	@author		LGE DRM TEAM [Modify date : 2009/11/11]
*/
int drmLibGetFlexInfo(enDRMFlexInfo eType, void *pvData)
{
	if (!pvData)
	{
		return eDRM_FAILURE;
	}

	if (eType == eDRM_FLEX_OPERATOR)
	{
		int nOpCode = eDRM_FLEX_OP_NONE;
		
#if defined(DRM_PLATFORM_ANDROID)
		char szInfo[50]  = {0,};

		if (eDRM_SUCCESS != readFlex("FLEX_OPERATOR_CODE", szInfo))
		{
			DRM_ERROR("[DRMERR] drmLibGetFlexInfo() : Fail to read OpCode",0,0,0);
			return eDRM_FAILURE;
		}
		
		DRM_LOG_MID("[DRM] drmLibGetFlexInfo() : OP code (%s)", szInfo,0,0);

		if (!drmLibStrcmp("VIV", szInfo))
		{
			nOpCode = eDRM_FLEX_OP_VIVO;
		}
#else
		// TODO:
#endif

		*((int*)pvData) = nOpCode;
		return eDRM_SUCCESS;
	}
	else if (eType == eDRM_FLEX_REGION)
	{
		char szInfo[50]  = {0,};
		int nInfo = 0;

		*((int*)pvData) = eDRM_FLEX_REGION_NONE;
		
		if (eDRM_SUCCESS != readFlex("FLEX_REGION_CODE", szInfo))
		{
			DRM_ERROR("[DRMERR] drmLibGetFlexInfo() : Fail to read FLEX_REGION_CODE",0,0,0);
			return eDRM_FAILURE;
		}

		DRM_LOG_MID("[DRM] drmLibGetFlexInfo() : FLEX_REGION_CODE (%s)", szInfo,0,0);

		nInfo = drmLibAtoi(szInfo);
		switch (nInfo)
		{
		/* 0:EUR, 1:LATIN, 2:ASIA, 3:AUS, 4:AFRICA, 5:CIS, 6:MEA, 7:US */
		case 0: *((int*)pvData) = eDRM_FLEX_REGION_EUR;		break;
		case 1: *((int*)pvData) = eDRM_FLEX_REGION_LATIN;	break;
		case 2: *((int*)pvData) = eDRM_FLEX_REGION_ASIA;	break;
		case 3: *((int*)pvData) = eDRM_FLEX_REGION_AUS;		break;
		case 4: *((int*)pvData) = eDRM_FLEX_REGION_AFRICA;break;
		case 5: *((int*)pvData) = eDRM_FLEX_REGION_CIS;		break;
		case 6: *((int*)pvData) = eDRM_FLEX_REGION_MEA;		break;
		case 7: *((int*)pvData) = eDRM_FLEX_REGION_US;		break;
		default:
			return eDRM_FAILURE;
		}
		
		return eDRM_SUCCESS;
	}

	return eDRM_FAILURE;
}


/* ━━━━━━━━━━ [MUTEX] ━━━━━━━━━━ */

/**
	@ingroup	Mutex
	@brief		This interface allocates mutex of given type.
	@param		nType			[IN] Type of mutex.
	@return
				@li	NULL	:	On failure.
				@li	> 0		:	Address of the mutex.
	@warning
	@par PreCondition
	@par PostCondition
	@par Example
				@code
				@endcode
	@sa
	@author		LGE DRM TEAM [Modify date : 2009/8/27]
*/
void *drmLibMutexAlloc(int nType)
{
	int nRetVal;
	stDRMMutex *pstMutex = drmLibCalloc(sizeof(stDRMMutex));
	
	if (!pstMutex)
	{
		DRM_ERROR("[DRMERR] drmLibMutexAlloc() : Malloc failure (%d line)",__LINE__,0,0);
		return NULL;
	}
	
	/* Use default mutex */
	if (nType == eDRM_MUTEX_NORMAL)
	{
		nRetVal = pthread_mutex_init(&pstMutex->mutex, NULL);
		if (nRetVal != 0)
		{
			DRM_ERROR("[DRMERR] drmLibMutexAlloc() : Fail to init mutex (Err=%d)",nRetVal,0,0);
			drmLibFree(pstMutex);
			return NULL;
		}
	}
	else if (nType == eDRM_MUTEX_RECURSIVE)
	{
		/* Use a recursive mutex */
		pthread_mutexattr_t recursiveAttr;
		pthread_mutexattr_init(&recursiveAttr);
		pthread_mutexattr_settype(&recursiveAttr, PTHREAD_MUTEX_RECURSIVE);

		nRetVal = pthread_mutex_init(&pstMutex->mutex, &recursiveAttr);
		pthread_mutexattr_destroy(&recursiveAttr);
		
		if (nRetVal != 0)
		{
			DRM_ERROR("[DRMERR] drmLibMutexAlloc() : Fail to init recursize mutex (Err=%d)",nRetVal,0,0);
			drmLibFree(pstMutex);
			return NULL;
		}
	}
	
	pstMutex->eType = nType;
	
	return (void*)pstMutex;
}


/**
	@ingroup	Mutex
	@brief		This interface destory mutex.
	@param		pvMutex	[IN] Mutex object.
	@return
				@li	1	:	On success.
				@li	0	:	On failure.
	@warning
	@par PreCondition
	@par PostCondition
	@par Example
				@code
				@endcode
	@sa
	@author		LGE DRM TEAM [Modify date : 2009/8/27]
*/
int drmLibMutexDestroy(void *pvMutex)
{
	int nRetVal;
	
	if (!pvMutex)
	{
		DRM_ERROR("[DRMERR] drmLibMutexDestroy() : Invalid argument",0,0,0);
		return 0;
	}
	
	nRetVal = pthread_mutex_destroy(&(((stDRMMutex *)pvMutex)->mutex));
	drmLibFree(pvMutex);
	
	if (nRetVal != 0)
	{
		DRM_ERROR("[DRMERR] drmLibMutexDestroy() : Fail to destroy mutex (Err=%d)",nRetVal,0,0);
		return 0;
	}

	return 1;
}


/**
	@ingroup	Mutex
	@brief		This interface lock mutex of given type.
	@param		pvMutex	[IN] Mutex object.
	@return
				@li	1	:	On success.
				@li	0	:	On failure.
	@warning
	@par PreCondition
	@par PostCondition
	@par Example
				@code
				@endcode
	@sa
	@author		LGE DRM TEAM [Modify date : 2009/8/27]
*/
int drmLibMutexEnter(void *pvMutex)
{
	int nRetVal;
	stDRMMutex *pstMutex = (stDRMMutex *)pvMutex;
	
	if (!pvMutex)
	{
		DRM_ERROR("[DRMERR] drmLibMutexEnter() : Invalid argument",0,0,0);
		return 0;
	}
	
	nRetVal = pthread_mutex_lock(&pstMutex->mutex);
	if (nRetVal != 0)
	{
		DRM_ERROR("[DRMERR] drmLibMutexEnter() : Fail to lock (Err=%d)",nRetVal,0,0);
		return 0;
	}
	
	pstMutex->owner = pthread_self();
	pstMutex->nRef += 1;
	
	return 1;
}


/**
	@ingroup	Mutex
	@brief		This interface tries to lock mutex of given type.
	@param		pvMutex	[IN] Mutex object.
	@return
				@li	1	:	Calling thread acquired mutex.
				@li	0	:	Mutex was already locked.
	@warning
	@par PreCondition
	@par PostCondition
	@par Example
				@code
				@endcode
	@sa
	@author		LGE DRM TEAM [Modify date : 2009/8/27]
*/
int drmLibMutexTryEnter(void *pvMutex)
{
	int nRetVal;
	stDRMMutex *pstMutex = (stDRMMutex *)pvMutex;
	
	if (!pvMutex)
	{
		DRM_ERROR("[DRMERR] drmLibMutexEnter() : Invalid argument",0,0,0);
		return 0;
	}
	
	nRetVal = pthread_mutex_trylock(&pstMutex->mutex);
	if (nRetVal != 0)
	{
		DRM_LOG_LOW("[DRM] drmLibMutexTryEnter() : Already locked",0,0,0);
		return 0;
	}
	
	pstMutex->owner = pthread_self();
	pstMutex->nRef += 1;

	return 1;
}


/**
	@ingroup	Mutex
	@brief		Unlock mutex.
	@param		pvMutex	[IN] Mutex object.
	@return
				@li	1	:	On success.
				@li	0	:	On failure.
	@warning
	@par PreCondition
	@par PostCondition
	@par Example
				@code
				@endcode
	@sa
	@author		LGE DRM TEAM [Modify date : 2009/8/27]
*/
int drmLibMutexLeave(void *pvMutex)
{
	int nRetVal;
	stDRMMutex *pstMutex = (stDRMMutex *)pvMutex;

	if (!pvMutex)
	{
		DRM_ERROR("[DRMERR] drmLibMutexLeave() : Invalid argument",0,0,0);
		return 0;
	}
	
	if (pstMutex->owner == 0)
	{
		DRM_ERROR("[DRMERR] drmLibMutexLeave() : No owner was given",0,0,0);
		return 0;
	}
	
	if (pstMutex->nRef <= 0)
	{
		DRM_ERROR("[DRMERR] drmLibMutexLeave() : Abnormal reference count",0,0,0);
		return 0;
	}

	pstMutex->nRef -= 1;
	if (pstMutex->nRef == 0)
	{
		pstMutex->owner = 0;
		//DRM_LOG_LOW("[DRM] drmLibMutexLeave() : Release mutex",0,0,0);
	}
	
	nRetVal = pthread_mutex_unlock(&pstMutex->mutex);
	if (nRetVal != 0)
	{
		DRM_ERROR("[DRMERR] drmLibMutexLeave() : Fail to unlock (Err=%d)",nRetVal,0,0);
		return 0;
	}

	return 1;
}

