
/******************************************************************************
* Developed by	: LGE
* Security			: Confidential
* Module				: LG DRM
* Filename			: DRM_Port.h
* Purpose				: LG DRM Porting layer
* Author(s)			: Lothson
* e-mail				: lothson@lge.com
* Creation date	: 5th July 2006
* Edit History	: Modification description(s)
Date				Name					Version			Description
---------		-------				------			--------------
2006.07.05	Lothson				v1.00				Initial Creation
******************************************************************************/

#ifndef __DRMPORT_H
#define __DRMPORT_H

/*
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
		INCLUDE
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
*/
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>

#include "DRM_Config.h"
#include "DRM_Port_Comm.h"


/*
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
		DEFINE
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
*/

/*
	DRM Settings 
*/
extern int g_DRM_Settings;

/*
	Debugging configuration
*/
extern int g_DRM_Debug;

/* Load DRM solution config info */
#if defined(DRM_MP_ENV) || defined(DRM_MT_ENV)
#define DRM_LOAD_CFG() if (!g_DRM_Settings) {drmLibGetSolutionCFG();}
#else
#define DRM_LOAD_CFG()
#endif

/* 收收收收收收收收收收 [LOG HDR] 收收收收收收收收收收 */

#if defined(DRM_LOG_ENABLE) || defined(DRM_ERRORLOG_ENABLE)
	#if !defined(DRM_PC_EMULATOR) && (defined(DRM_PLATFORM_QCT_EURO) || defined(DRM_PLATFORM_QCT_US))
		#include "msg.h"
	#endif
#endif


/* 收收收收收收收收收收 [GENERIC LOG] 收收收收收收收收收收 */

#define DRM_LOG_BUF_LEN		1024 /* Maximum size of log */

typedef enum _DRMLOGLV
{
	eDRM_LOGLV_LOW = 0,
	eDRM_LOGLV_MID,
	eDRM_LOGLV_HIGH,
	eDRM_LOGLV_ERR
} enDRMLOGLV;

#ifdef DRM_LOG_ENABLE
	#if !defined(DRM_PC_EMULATOR) && (defined(DRM_PLATFORM_QCT_EURO) || defined(DRM_PLATFORM_QCT_US))
		/*
			QCT platform uses macro to print out log with file name and line.
			So, using the default macro is the best way.
		*/
		#define DRM_LOG(X_f,a,b,c)			if (g_DRM_Debug&eDRM_DBG_LOG_MID) {LGE_MSG_MED(13,X_f,a,b,c); drmLibFileLog(X_f,a,b,c);}
		#define DRM_LOG_HIGH(X_f,a,b,c)	if (g_DRM_Debug&eDRM_DBG_LOG_HIGH){LGE_MSG_HIGH(13,X_f,a,b,c);drmLibFileLog(X_f,a,b,c);}
		#define DRM_LOG_MID(X_f,a,b,c)	if (g_DRM_Debug&eDRM_DBG_LOG_MID) {LGE_MSG_MED(13,X_f,a,b,c); drmLibFileLog(X_f,a,b,c);}
		#define DRM_LOG_LOW(X_f,a,b,c)	if (g_DRM_Debug&eDRM_DBG_LOG_LOW) {LGE_MSG_LOW(13,X_f,a,b,c); drmLibFileLog(X_f,a,b,c);}
	#else
		#define DRM_LOG(X_f,a,b,c)			if (g_DRM_Debug&eDRM_DBG_LOG_MID) drmLibWriteLevelLog(eDRM_LOGLV_MID,	__FILE__, __LINE__,X_f,(int)a,(int)b,(int)c)
		#define DRM_LOG_HIGH(X_f,a,b,c)	if (g_DRM_Debug&eDRM_DBG_LOG_HIGH)drmLibWriteLevelLog(eDRM_LOGLV_HIGH,__FILE__, __LINE__,X_f,(int)a,(int)b,(int)c)
		#define DRM_LOG_MID(X_f,a,b,c)	if (g_DRM_Debug&eDRM_DBG_LOG_MID) drmLibWriteLevelLog(eDRM_LOGLV_MID,	__FILE__, __LINE__,X_f,(int)a,(int)b,(int)c)
		#define DRM_LOG_LOW(X_f,a,b,c)	if (g_DRM_Debug&eDRM_DBG_LOG_LOW) drmLibWriteLevelLog(eDRM_LOGLV_LOW,	__FILE__, __LINE__,X_f,(int)a,(int)b,(int)c)
	#endif
		#define DRM_LOGUNI(x)						if (g_DRM_Debug&eDRM_DBG_LOG_LOW) drmLibWriteLogUni(x)
#else	// DRM_LOG_ENABLE
	#define DRM_LOG(X_f,a,b,c)
	#define DRM_LOG_HIGH(X_f,a,b,c)
	#define DRM_LOG_MID(X_f,a,b,c)
	#define DRM_LOG_LOW(X_f,a,b,c)
	#define DRM_LOGUNI(x)
#endif // DRM_LOG_ENABLE


/* 收收收收收收收收收收 [ERROR LOG] 收收收收收收收收收收 */

#ifdef DRM_ERRORLOG_ENABLE
	#if !defined(DRM_PC_EMULATOR) && (defined(DRM_PLATFORM_QCT_EURO) || defined(DRM_PLATFORM_QCT_US))
		#define DRM_ERROR(X_f,a,b,c)			{LGE_MSG_ERROR(13,X_f,a,b,c); drmLibFileLog(X_f,a,b,c);}
	#else
		#define DRM_ERROR(X_f,a,b,c)			drmLibWriteLevelLog(eDRM_LOGLV_ERR,__FILE__, __LINE__,X_f,(int)a,(int)b,(int)c)
	#endif
#else	// DRM_ERRORLOG_ENABLE
	#define DRM_ERROR(X_f,a,b,c)
#endif	// DRM_ERRORLOG_ENABLE


/* 收收收收收收收收收收 [PERFORMANCE LOG] 收收收收收收收收收收 */

#ifdef DRM_DEBUG_APIPERFORMANCE
	#define DRM_TLOG_START(TYPE)				unsigned int uUnitTime = 0; if (g_DRM_Debug&(TYPE)) uUnitTime = drmLibGetTickCount()

	#if !defined(DRM_PC_EMULATOR) && (defined(DRM_PLATFORM_QCT_EURO) || defined(DRM_PLATFORM_QCT_US))
		#define DRM_TLOG_END(TYPE,msg)		if (g_DRM_Debug&(TYPE)) LGE_MSG_LOW(13,"[DRM] " #msg " : Took \t%d \tms", drmLibGetTickCount() - uUnitTime, 0, 0)
		#define DRM_TLOG_TICK(TYPE,msg)		if (g_DRM_Debug&(TYPE)) LGE_MSG_LOW(13,"[DRM] " #msg " : Tick \t%u", drmLibGetTickCount(), 0, 0)
	#else
		#define DRM_TLOG_END(TYPE,msg)		if (g_DRM_Debug&(TYPE)) drmLibWriteLevelLog(eDRM_LOGLV_LOW,__FILE__, __LINE__, "[DRM] " #msg " : Took \t%d \tms", drmLibGetTickCount() - uUnitTime, 0, 0)
		#define DRM_TLOG_TICK(TYPE,msg)		if (g_DRM_Debug&(TYPE)) drmLibWriteLevelLog(eDRM_LOGLV_LOW,__FILE__, __LINE__, "[DRM] " #msg " : Tick \t%u", drmLibGetTickCount(), 0, 0)
	#endif

#else	// DRM_DEBUG_APIPERFORMANCE
	#define DRM_TLOG_START(TYPE)
	#define DRM_TLOG_END(TYPE,msg)
	#define DRM_TLOG_TICK(TYPE,msg)
#endif	// DRM_DEBUG_APIPERFORMANCE


/* 收收收收收收收收收收 [LOG FILE] 收收收收收收收收收收 */

/* File logging */
#ifdef DRM_DEBUG_FILELOG
	#define DRM_LOG_FILE(X)		drmLibFileLog(X)
#else
	#define DRM_LOG_FILE(X)
#endif


/** 
  @ingroup	Port_DT
  @brief	  Result of stat */
typedef struct
{
	DRM_UCHAR szName[DRM_FILE_PATH_LEN+1];
	/**< Name */
	unsigned char bFile;
	/**< File(TRUE), Directory(FALSE) */
	int iSize;
	/* File Or Directory Size */
} DRMTFStat;

#ifdef DRM_DEBUG_HEAP
typedef enum
{
	DRM_HEAPDBG_RESET 						= -999,
	DRM_HEAPDBG_FREE_RECORD_START = -888,
	DRM_HEAPDBG_FREE_RECORD_END 	= -777,
	DRM_HEAPDBG_FREE_DBG					= -666
} drm_heap_dbg;
#endif

typedef enum _DRMMemType
{
	eDRM_MEM_COMM = 0,
	eDRM_MEM_SSL,
	eDRM_MEM_XML,
	eDRM_MEM_ISAM,
	eDRM_MEM_WM,
	eDRM_MEM_WBXML,
	eDRM_MEM_MSDRM,
	eDRM_MEM_SQLITE,
	eDRM_MEM_EXT
} enDRMMemType;

typedef enum _DRMSystemID
{
	/* Get current system ID value */
	eDRM_SYSID_GET = 0,
	/* Whether the caller should read system ID again or not */
	eDRM_SYSID_IS_UPDATE_REQUIRED,
	/* Cache system ID */
	eDRM_SYSID_CACHE,
	/* Force to update system ID value */
	eDRM_SYSID_FORCE_UPDATE
} enDRMSystemID;

typedef enum _DRMProcInfo
{
	/* Check whether the current process (or task) name is matched with given one */
	eDRM_PINFO_NAME_IS_MATCHED = 0,
	/* Get current process (or task) name */
	eDRM_PINFO_NAME_GET
} enDRMProcInfo;

/*
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
		GLOBAL FUNCTION PROTOTYPE
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
*/


/* 收收收收收收收收收收 [Memory] 收收收收收收收收收收 */

unsigned int drmLibHeapGetFreeSize(void);
void *drmLibRealloc(void *pvPtr, unsigned int uSize);
void drmLibFree(void *pvPtr);
void *drmLibMemset(void *pvPtr, int nValue, unsigned int uSize);
void *drmLibMemcpy(void *pvTarget, const void *pvSource, unsigned int uSize);
void *drmLibMemmove(void *pvTarget, const void *pvSource, unsigned int uSize);
int drmLibMemcmp(const void *pvTarget, const void *pvSource, unsigned int uSize);
void *drmLibMalloc(unsigned int uSize);
void *drmLibCalloc(unsigned int uSize);

void drmLibExtFree(void *pvPtr);
void *drmLibExtMalloc(unsigned int uSize);
void *drmLibExtCalloc(unsigned int uSize);
void *drmLibExtRealloc(void *pvPtr, unsigned int uSize);

#ifdef DRM_DEBUG_HEAP_SSL
void *drmLibSSLRealloc(void *pvPtr, unsigned int uSize);
void drmLibSSLFree(void *pvPtr);
void *drmLibSSLMalloc(unsigned int uSize);
#endif

#ifdef DRM_DEBUG_HEAP_LIBXML
void *drmLibXmlRealloc(void *pvPtr, unsigned int uSize);
void drmLibXmlFree(void *pvPtr);
void *drmLibXmlMalloc(unsigned int uSize);
void *drmLibXmlCalloc(unsigned int uSize);
char *drmLibXmlStrdup (const char *pszStr);
#endif

#ifdef DRM_DEBUG_HEAP_ISAM
void *drmLibIsamRealloc(void *pvPtr, unsigned int uSize);
void drmLibIsamFree(void *pvPtr);
void *drmLibIsamMalloc(unsigned int uSize);
char *drmLibIsamStrdup(const char *pszStr);
#endif

#ifdef DRM_DEBUG_HEAP_WM
void* drmLibWMRealloc(void *pvPtr, unsigned int uSize);
void drmLibWMFree(void *pvPtr);
void *drmLibWMMalloc(unsigned int uSize);
#endif

#ifdef DRM_DEBUG_HEAP_MSDRM
void *drmLibMSRealloc(void *pvPtr, unsigned int uSize);
void drmLibMSFree(void *pvPtr);
void *drmLibMSMalloc(unsigned int uSize);
#endif

#ifdef DRM_DEBUG_HEAP_LIBWBXML
void *drmLibWbXmlRealloc(void *pvPtr, unsigned int uSize);
void drmLibWbXmlFree(void *pvPtr);
void *drmLibWbXmlMalloc(unsigned int uSize);
void *drmLibWbXmlCalloc(unsigned int uSize);
char *drmLibWbXmlStrdup(const char *pszStr);
#endif

#ifdef DRM_DEBUG_HEAP_SQLITE
void *drmLibSQLiteRealloc(void *pvPtr, unsigned int uSize);
void drmLibSQLiteFree(void *pvPtr);
void *drmLibSQLiteMalloc(unsigned int uSize);
#endif

#ifdef DRM_DEBUG_HEAP
void drmLibHeapDebug(int nSrcMemType, void *pvSource, int nDstMemType);
int drmLibAdjustHeap(int n, int nType);
void drmLibHeapUsage(void);
#else
#define drmLibHeapDebug
#endif


/* 收收收收收收收收收收 [String] 收收收收收收收收收收 */

char *drmLibStrdup(const char *pszStr);
char *drmLibExtStrdup(const char *pszStr);
char *drmLibStrndup(const char *pszStr, unsigned int uSize);
char *drmLibStrcpy(char *pszTarget, const char *pszSource);
char *drmLibStrncpy(char *pszTarget, const char *pszSource, unsigned int uSize);
char *drmLibStrcat(char *pszTarget, const char *pszSource);
int drmLibStrcmp(const char *pszTarget, const char *pszSource);
int drmLibStrncmp(const char *pszTarget, const char *pszSource, unsigned int uSize);
int drmLibStricmp(const char *pszTarget, const char *pszSource);

char *drmLibStrchr(const char *pszStr, int nChr);
char *drmLibStrrchr(const char *pszStr, int nChr);
char *drmLibStrstr(const char *pszStr, const char *pszSubStr);
char* drmLibStrnstr(const char* pszStr, const char* pszSubStr, int nStrLen);
unsigned int drmLibStrlen(const char *pszStr);
#ifdef DRM_PLATFORM_EMP
unsigned int drmLibStrtoul(const char *pszStr, char **ppszEndStr, int nBase);
#endif

char *drmLibStrtok(char *pszStr, const char *pszDelim);
char *drmLibStrtok_r(char *pszStr, const char *pszDelims, char **ppszSaveptr);
int drmLibSprintf(char *pszBuf, const char *pszFormat, ...);
int drmLibVsprintf(char *pszBuf, const char *pszFormat, va_list pstArgs);
int drmLibSnprintf(char *pszBuf, unsigned int uBufLen, const char *pszFormat, ...);
int drmLibVsnprintf(char *pszBuf, unsigned int uBufLen, const char *pszFormat, va_list pstArgs);



/* 收收收收收收收收收收 [Unicode String] 收收收收收收收收收收 */

DRM_TSTR drmLibUniStrcpy(DRM_TSTR pusTarget, DRM_TCSTR pusSource);
DRM_TSTR drmLibUniStrncpy(DRM_TSTR pusTarget, DRM_TCSTR pusSource, unsigned int uSize);
DRM_TSTR drmLibUniStrcat(DRM_TSTR pusTarget, DRM_TCSTR pusSource);

int drmLibUniStrcmp(DRM_TCSTR pusTarget, DRM_TCSTR pusSource);
int drmLibUniStrncmp(DRM_TCSTR pusTarget, DRM_TCSTR pusSource, unsigned int uSize);
int drmLibUniStrlen(DRM_TCSTR pusStr);

DRM_TSTR drmLibUniStrdup(DRM_TCSTR pusStr);
DRM_TSTR drmLibExtUniStrdup(DRM_TCSTR pusStr);
DRM_TSTR drmLibUniStrstr(DRM_TCSTR pusStr, DRM_TCSTR pusSubStr);
DRM_TSTR drmLibUniStrchr(DRM_TCSTR pusStr, int nChr);

#if !defined(DRM_PLATFORM_WNDM)
/*
	2006.03.06 Lothson. 
	If the PDK does not support UniLib_Vsprintf, use this method.
*/
typedef  int (*drmLibUniSprintfFunc)(DRM_TSTR pusBuf, DRM_TCSTR pusFormat, ...);
extern drmLibUniSprintfFunc drmLibUniSprintf;

#else
int	drmLibUniSprintf(DRM_TSTR buf, DRM_TCSTR format, ...);
#endif

int drmLibUniStricmp(DRM_TCSTR pusTarget, DRM_TCSTR pusSource);
unsigned int drmLibUniFixedStrlen(const unsigned short* pucStr);


/* 收收收收收收收收收收 [Conversion function] 收收收收收收收收收收 */
int drmLibAtoi(const char *pszStr);
int drmLibUTF8ToUCS2(const char *pszSource, DRM_TSTR pusTarget);
int drmLibUCS2ToUTF8(DRM_TCSTR pusSource, char *pszTarget);
int drmLibFixedUTF8ToUCS2(const char *pszSource, unsigned short *pusTarget);
int drmLibFixedUCS2ToUTF8(const unsigned short *pusSource, char *pszTarget);


/* 收收收收收收收收收收 [System ID] 收收收收收收收收收收 */

int drmLibGetSIMId(unsigned char *pucSIMId);
int drmLibGetSystemId(enDRMSystemID eType, unsigned char *pucSystemID);
int drmLibGetSystemId_PC(unsigned char *pucSystemID);
int drmLibGetProcessInfo(enDRMProcInfo eType, void *pvData);

/* 收收收收收收收收收收 [FILE] 收收收收收收收收收收 */

void *drmLibFsOpen(DRM_TSTR pusFilename, int nAccessMode);
int drmLibFsClose(void* hFile);
int drmLibFsRead(void* hFile, unsigned char *pucBuf, unsigned int uSize);
int drmLibFsWrite(void* hFile, unsigned char *pucBuf, unsigned int uSize);
int drmLibFsSeek(void* hFile, int nOffset, int nWhence);
int drmLibFsRemove(DRM_TSTR pusFilename);
int drmLibFsTell(void* hFile);
unsigned int	drmLibFsFileSize(DRM_TSTR pusFilename);
int drmLibFsFileExist(DRM_TSTR pusFilename);
int drmLibFsRename(DRM_TSTR pusOldFName, DRM_TSTR pusNewFName);
int drmLibFsCopyFile(DRM_TSTR pusOldFName, DRM_TSTR pusNewFName);
int drmLibFsMoveFile(DRM_TSTR pusOldFName, DRM_TSTR pusNewFName);
unsigned int drmLibFsGetSize (void *hFile);
int drmLibFsGetPos(void *hFile);
int drmLibFsTruncate(void* hFile, unsigned int uSize);
int drmLibFsFlush(void *hFile);
int drmLibFsFindFirstFile(DRM_TSTR pusDirPath, int *pnIterator, DRMTFStat *pstFStat);
int drmLibFsFindNextFile(int nIterator, DRMTFStat *pstFStat);
int drmLibFsFindClose(int nIterator);
int drmLibFsSearchDir(DRM_TSTR pusDirname);
int drmLibFsCreateDir(DRM_TSTR pusDirname);
int drmLibFsFindFirstDir(DRM_TSTR pusDirPath, int *pnIterator, DRMTFStat *pstFStat);
int drmLibFsFindNextDir(int nIterator, DRMTFStat *pstFStat);
DRMUINT64 drmLibFsGetFreeSpace(DRM_TSTR pusMountPath);
DRMUINT64 drmLibFsGetFreeSpaceByType(enDRMFSType eFSType);


/* 收收收收收收收收收收 [MMC] 收收收收收收收收收收 */

void drmLibMMC_GetState(unsigned short *pusStatus);
void drmLibMMC_GetSize(DRMUINT64 *pllMMC_size);



/* 收收收收收收收收收收 [TIME] 收收收收收收收收收收 */

void drmLibUserTime_GetST(void *pvTime);
unsigned int drmLibUserTime_GetUTCSec(void);
unsigned int drmLibTime_ST2UTCSec(void *pvTime);
void drmLibTime_UTCSec2ST(unsigned int uUTCSec, void *pvTime);
void drmLibTime_PrintUTCSec(unsigned int uUTCSec, unsigned char *pszPrefix);
unsigned int drmLibGetTickCount(void);


/* 收收收收收收收收收收 [ETC] 收收收收收收收收收收 */

DRM_TSTR drmLibGetContentExtension(unsigned char *pszMimeType);
int drmLibGetMediaType(unsigned char *pszMimeType);
#ifdef DRM_PLATFORM_ANDROID
char *drmLibGetAndroidMimeType(unsigned char *pszMimeType);
#endif
DRM_TSTR drmLibGetStoragePath(unsigned char *pszMimeType);

void drmLibWriteLog(char* pszFormat, ...);
void drmLibWriteLevelLog(enDRMLOGLV eLevel, char *pszFile, int nLine, char* pszFormat, int arg1, int arg2, int arg3);
void drmLibWriteLogUni(DRM_TSTR pusData);
void drmLibFileLog(char *pszFormat, ...);


/* 收收收收收收收收收收 [CFG] 收收收收收收收收收收 */

void drmLibGetSolutionCFG(void);
int drmLibGetOperatorCode(void);
int drmLibGetCFGIntValue(enDRMOpIntCfg eItem);
const char * drmLibGetCFGStrValue(enDRMOpStrCfg eItem);
int drmLibGetFlexInfo(enDRMFlexInfo eType, void *pvData);
int drmLibForwardLockIconVisible(void);

/* 收收收收收收收收收收 [MUTEX] 收收收收收收收收收收 */

void *drmLibMutexAlloc(int nType);
int drmLibMutexDestroy(void *pvMutex);
int drmLibMutexEnter(void *pvMutex);
int drmLibMutexTryEnter(void *pvMutex);
int drmLibMutexLeave(void *pvMutex);


#endif


