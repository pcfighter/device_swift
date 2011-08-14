
/******************************************************************************
* Developed by	: LGE
* Security			: Confidential
* Module				: LG DRM
* Filename			: DRM_Port_Comm.h
* Purpose				: Common porting layer
* Author(s)			: Lothson
* e-mail				: lothson@lge.com
* Creation date	: 13th Nov 2009
* Edit History	: Modification description(s)
Date				Name					Version			Description
---------		-------				------			--------------
2009.11.13	Lothson				v1.00				Initial Creation
******************************************************************************/

#ifndef __DRM_PORT_COMM_H
#define __DRM_PORT_COMM_H

/*
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
		INCLUDE
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
*/

#include "DRM_Config.h"


/*
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
		DEFINE
收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
*/

/* 收收收收收收收收收收 [STRICMP] 收收收收收收收收收收 */

extern const unsigned char DRM_casemap[256];


/* 收收收收收收收收收收 [OPERATOR CONFIGURATION] 收收收收收收收收收收 */

/** 
  @ingroup	Port_DT
  @brief	  	Type of configurations */
typedef enum _enDRMOpIntCfg
{
	eDRM_OPINTCFG_DRM = 0,		
	/**< Supported DRM Solutions */
	eDRM_OPINTCFG_RIGHT_XML_PROLOGUE,
	/**< Support stripping xml prolgue */
	eDRM_OPINTCFG_RIGHT_COUNT_WITH_TIME,	
	/**< If time based constraint is not supported, remove it and leave only count constraint */
	eDRM_OPINTCFG_AUTO_LOADING_CERT,
	/**< Automatically load test certificates */
	eDRM_OPINTCFG_LOADING_TEST_CERT,
	/**< Load test certificates */
	eDRM_OPINTCFG_TIMEBASED_CONST,
	/**< Support time based constraint */
	eDRM_OPINTCFG_MAX
} enDRMOpIntCfg;

typedef enum _enDRMOpStrCfg
{
	eDRM_OPSTRCFG_CERTTYPE = 0,
	eDRM_OPSTRCFG_MAX
} enDRMOpStrCfg;

 
/** 
  @ingroup	Port_DT
  @brief	  Operator type */ 
typedef	enum _DRMOperator
{
	eDRM_FLEX_OP_NONE = -1,			/**< Unknow operator */
	eDRM_FLEX_OP_VDF,
	eDRM_FLEX_OP_H3G,
	eDRM_FLEX_OP_ORG,
	eDRM_FLEX_OP_TMO,
	eDRM_FLEX_OP_TME,
	eDRM_FLEX_OP_TIM,
	eDRM_FLEX_OP_TNR,
	eDRM_FLEX_OP_MTN,
	eDRM_FLEX_OP_MOBITEL,
	eDRM_FLEX_OP_TMN,
	eDRM_FLEX_OP_CELLCOM,
	eDRM_FLEX_OP_OPEN,
	eDRM_FLEX_OP_GLOBE,
	eDRM_FLEX_OP_SMART,
	eDRM_FLEX_OP_O2,
	eDRM_FLEX_OP_VIVO,
	eDRM_FLEX_OP_CLARO,
	eDRM_FLEX_OP_MOV,		//Movistar
	eDRM_FLEX_OP_CMCC,	//China mobile
	eDRM_FLEX_OP_CUGSM,	//China Unicom
	eDRM_FLEX_OP_BRT,
	eDRM_FLEX_OP_OI,
	eDRM_FLEX_OP_TELEMIG,
	eDRM_FLEX_OP_DIGITEL,
	eDRM_FLEX_OP_MOVILNET,
	eDRM_FLEX_OP_COMCEL,
	eDRM_FLEX_OP_CTI,
	eDRM_FLEX_OP_PERSONAL,
	eDRM_FLEX_OP_ENTEL,
	eDRM_FLEX_OP_PORTA,
	eDRM_FLEX_OP_KPN,
	eDRM_FLEX_OP_SWC,
	eDRM_FLEX_OP_ATT,
	eDRM_FLEX_OP_VZW,
	eDRM_FLEX_OP_METROPCS,
	eDRM_FLEX_OP_MAX,
} enDRMOperator;

typedef enum _DRMRegion
{
	eDRM_FLEX_REGION_NONE = 0,
	eDRM_FLEX_REGION_EUR,		/* Europe */
	eDRM_FLEX_REGION_CIS,		/* Commonwealth of Independent States */
	eDRM_FLEX_REGION_LATIN,	/* Latin America */
	eDRM_FLEX_REGION_US,		/* North America */
	eDRM_FLEX_REGION_ASIA,	/* Asia */
	eDRM_FLEX_REGION_MEA,		/* Middle of Asia */
	eDRM_FLEX_REGION_AUS,		/* Australian */
	eDRM_FLEX_REGION_AFRICA,/* Africa */
	eDRM_FLEX_REGION_MAX
} enDRMRegion;

typedef enum _DRMFlexInfo
{
	eDRM_FLEX_OPERATOR = 0,
	eDRM_FLEX_REGION
} enDRMFlexInfo;

typedef struct _stDRMOpIntCfg
{
	enDRMOpIntCfg	item;
	int intValue;
} stDRMOpIntCfg;

typedef struct _stDRMOpStrCfg
{
	enDRMOpStrCfg	item;
	char *strValue;
}stDRMOpStrCfg;


#endif

