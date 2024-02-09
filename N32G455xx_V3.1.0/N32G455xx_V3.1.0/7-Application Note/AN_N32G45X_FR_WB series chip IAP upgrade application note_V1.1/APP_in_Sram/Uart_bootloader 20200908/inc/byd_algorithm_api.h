#ifndef BYD_ALGORITHM_API_H
#define BYD_ALGORITHM_API_H

#include <stdio.h>
#include <stdint.h>

#include "n32g4fr.h"




#define FPS_5926_EN			     1
#define FPS_5325AS_EN			   0



#define RT_OK                    0x00 //success
#define RT_FAIL                  0x01 //fail

typedef enum {FALSE = 0, TRUE = !FALSE} BOOL;


typedef enum
{
	 FPS_TempNomal,
   FPS_TempFull,	
   FPS_TempEmpty,
   FPS_FinerIDExist
}FPS_TemplateSates;



/* FPS operation fail code */
typedef enum 
{
	FPS_Success = 0,			//Success
	FPS_TempletFull,			//finger templet full
	FPS_NoFinger,				//no finger on fps while enrolling
	FPS_SPIDataError,			//dsp read fps data error
	FPS_IDExist,				//finger id is existed
	FPS_LessFeaturePoints,		//feature points too less
	FPS_MultipleFeaturePoints,	//feature points too multiple
	FPS_IDOutRange,				//finger id is out of set range
	FPS_FingerSlide,			//finger slide when enroll
	FPS_FingerExist,			//current finger has enrolled
	FPS_PositionOverlapped,		//current finger position and previous overlapped
	FPS_AreaSamll,				//finger area is too samll on the fps
	FPS_ImageBlurring,			//finger image is blurring	
	FPS_TempletNull,			//finger templet is null 
	FPS_TempletMismatch,		//finger templet match fail
	FPS_ImageAbnormal,			//finger image abnormal
	FPS_IDNonExistent,			//finger id is non-existent
	FPS_IDAbnormal,				//finger id abnormal	
	FPS_OtherErr          //other error
} FPS_FailCode;

/* Finger Id */
typedef enum
{
	ID0 = 0,
	ID1,
	ID2,
	ID3,
	ID4,
	ID5,
	ID6,
	ID7,
	ID8,
	ID9,
	ID10,
	ID11,
	ID12,
	ID13,
	ID14,
	ID15,
	ID16,
	ID17,
	ID18,
  ID19,
	ID20,
	ID21,
	ID22,
	ID23,
	ID24,
	ID25,
	ID26,
	ID27,
	ID28,
  ID29,
	ID30,
	ID31,
	ID32,
	ID33,
	ID34,
	ID35,
	ID36,
	ID37,
	ID38,
  ID39,
	ID40,
	ID41,
	ID42,
	ID43,
	ID44,
	ID45,
	ID46,
	ID47,
	ID48,
  //ID49
  ID49,
	ID50,
	ID51,
	ID52,
	ID53,
	ID54,
	ID55,
	ID56,
	ID57,
	ID58,
  ID59,
	ID60,
	ID61,
	ID62,
	ID63,
	ID64,
	ID65,
	ID66,
	ID67,
	ID68,
  ID69,
	ID70,
	ID71,
	ID72,
	ID73,
	ID74,
	ID75,
	ID76,
	ID77,
	ID78,
  ID79,
	ID80,
	ID81,
	ID82,
	ID83,
	ID84,
	ID85,
	ID86,
	ID87,
	ID88,
  ID89,
	ID90,
	ID91,
	ID92,
	ID93,
	ID94,
	ID95,
	ID96,
	ID97,
	ID98,
  ID99	
}FPS_FingerId;

/* Finger_Enroll function return struct */
typedef struct 
{
	BOOL 	FingerResult;
	BOOL 	TempletResult;
	FPS_FailCode FailCode;
	uint8_t ID;
	uint8_t MatchId;
	uint8_t Num;
}FPS_enroll_return_struct;

/* Finger_Delete function return struct */
typedef struct 
{
	BOOL 			Result;
	BOOL 			TempletDelete;
	uint8_t   FingerModeState;
	FPS_FailCode	FailCode;
}FPS_delete_return_struct;

/* Finger_Match function return struct */
typedef struct
{
	BOOL			Result;
	BOOL			TempletUpdate;
	FPS_FailCode	FailCode;
	uint8_t 		ID;
	uint8_t   MatchScore;
}FPS_match_return_struct;

/* Finger_DeleteFixId function return struct */
typedef struct
{
	BOOL			Result;
	FPS_FailCode 	FailCode;
}FPS_DeleteFixId_return_struct;

/* ID read return struct */
typedef struct
{
	uint8_t		ExistIdNum;
	uint8_t	  ExistId[32];
}FPS_IdRead_return_struct;

/* Finger_ReadImg function return struct */
typedef struct 
{
	BOOL			Result;
	uint8_t   FingerModeState;
}FPS_ReadImg_return_struct;

/*!
    \brief      enroll finger
    \param[in]  FirstEnroll
	\arg      	TRUE: 	enroll finger image for the first time
				FALSE:	!TRUE
	\param[in]  EnrollId: fix enroll id 
	\arg		IDx(x=0..99)	: fix id enroll
				0xff		: auto id enroll
	\param[in] EnrollPara: set the enroll parameter 
	\arg 
		bit0: IsIDCover , allow the same finger ID to be enroll
		0: not allow
		1: allow									
		bit1: IsFingerRepeat: allow the same finger enroll
		0: allow
		1: not allow
	0: allow
	1: not allow
  bit2: IsLastCheck:
	0: check is thte finger haved enrolled when the first timer enroll
	1: check is thte finger haved enrolled when the last timer enroll
  bit3: IsNotSaveTemplet:
	0: save
	1: not save
  bit4: 0
  bit5:IsReadImage
	0:not read image
  1:read image 	
	bit6~bit7:0
 	
    \param[out] 
	  \arg 		
    \retval     FPS_enroll_return_struct 
	\arg	
		FingerResult:					
			TRUE:						finger image enroll success
			FALSE:						finger image enroll fail
		TempletResult:					
			TRUE:						finger templet enroll success
			FALSE:						finger templet enroll fail
		FailCode:						causation 0f fail
			FPS_FailCode
		ID:								enroll	finger id
		Num:							the times of enroll the finger image
*/
extern FPS_enroll_return_struct Finger_Enroll(BOOL FirstEnroll,uint8_t EnrollId,uint8_t EnrollPara);



/*!
    \brief      delete finger templet
    \param[in]  
	\arg      	
    \param[out] 
	  \arg 		
    \retval     FPS_delete_return_struct 
	\arg	
		Result:					
			TRUE:						finger image delete success
			FALSE:						finger image delete fail
		TempletDelete:					
			TRUE:						finger templet delete success
			FALSE:						finger templet delete fail
		FailCode:						causation 0f fail
			FPS_FailCode
*/
extern FPS_delete_return_struct Finger_Delete(void);

/*!
    \brief      finger image match
	\param[in]  MatchId: fix match id 
	\arg		IDx(x=0..99)	: fix id match (just match the fix ID)
					0xffff		: match the whole templete
	\param[in] matchNum: when the matchid is not 0xffff,the matchNum is the num template should be match
	\param[in] MatchPara: 0x00:default, 0xff:special use
	bit0:IsReadImage
	0:not read image
  1:read image	
    \retval     FPS_match_return_struct 
	\arg	
		Result:					
			TRUE:						finger image match success
			FALSE:						finger image match fail
		FailCode:						causation 0f fail
			FPS_FailCode
		ID:								matched finger id
*/
extern FPS_match_return_struct Finger_Match(uint16_t matchid, uint8_t matchNum,uint8_t MatchPara);

/*!
    \brief      delete all finger images
    \param[in]  none
	\arg      	
    \param[out] none
	  \arg 
	\retval     bool 
	  \arg					
			TRUE:						all finger images delete success
			FALSE:						all finger images delete fail
*/
extern BOOL Finger_DeleteAll(void);

/*!
    \brief      delete fix id finger images
    \param[in]  FPS_FingerId DeleteId: IDx(x=0~99) 
	\arg      	
    \param[out] none
	  \arg 
    \retval     FPS_DeleteFixId_return_struct 
	\arg	
		Result:					
			TRUE:						delete fix id finger images success
			FALSE:						delete fix id finger images fail
		FailCode:						causation 0f fail
			FPS_FailCode
*/
extern FPS_DeleteFixId_return_struct Finger_DeleteFixId(FPS_FingerId DeleteId);

/*!
    \brief      read finger exist id
    \param[in]  none
	\arg      	
    \param[out] none
	  \arg 
	\retval     FPS_IdRead_return_struct 
	\arg	
		ExistIdNum:		Number of exist finger id			
		ExistId:		bits=1,id exist;bits=0,id empty
*/
extern FPS_IdRead_return_struct Finger_IdRead(void);




/*!
    \brief      set the Security level and number of enroll
    \param[in]   enrolcnt:3 is valid ; Security_level: 1,2,3 is valid
    \param[out] none
	\retval     bool 
	  \arg
			TRUE:		set the alogrithm parameter success
			FALSE:		!TRUE		
*/
extern BOOL Finger_SetAlgPara(uint8_t enrolcnt,uint8_t Security_level);


/*!
    \brief      Byd Templet Update
    \param[in]  
		TempletUpdate:					
			1:						finger templet need update
			0:					finger templet no need update		
    \param[out] none
	\retval     uint8_t 
	  \arg
			TRUE:		update template success
			FALSE:		update template error		
*/
extern BOOL Finger_TempletUpdate(uint8_t updataflag);


/*!
    \brief      get finger interrupt state
    \param[in]  ucMatchFlag : TRUE or FALSE
      \arg        
    \param[out] none
	  \arg 
    \retval     uint8_t 0--no finger;1--finger on;2--finger off
*/
extern uint8_t Finger_GetIntState(BOOL ucMatchFlag);


/*!
    \brief      read finger image data
    \param[in]  *ImageData
	\arg      	point of save image data
    \param[in]  ReadMode
    \arg      	bit0:enroll :1,match:0
    \arg        bit1~bit2: the enroll time index 1,2,3
    \param[out] *ImageData
	  \arg 		image data: ptImageData
	\retval     bool 
	\arg
    Result:					
			TRUE:		read finger image data success	
		  FALSE:		read finger image data fail	
		FingerModeState:   0x00 enter finger detect mode sucess
		                   0x04 RT_CHIP_RESET_FAIL
		                   0x80 RT_SFR_CHECK_ERROR
*/
extern FPS_ReadImg_return_struct Finger_ReadImage(uint8_t *ImageData, uint8_t ReadMode);


/*!
    \brief  Finger_QueryTemplateState :  query the finger id states  
    \param[in]  fingerid��0xff query the whole template states
		            IDx(x=0~99) query the fix id template state      
    \param[out] 
	\retval  FPS_TemplateSates   
	\arg   	 FPS_TempNomal: the fix id is not exist,you can enroll this id.
	         FPS_TempFull: the whole tempaltes lib is full.
						 FPS_TempEmpty: the whole tempaltes lib is empty.
						 FPS_FinerIDExist: the fix id is exist
*/
extern FPS_TemplateSates Finger_QueryTemplateState(uint8_t fingerid);

/*!
    \brief  Finger_Calculate_Feature :  Calculate the feature of the image 
    \param[in]  none   
    \param[out] 
		\retval  0x00: success, 0x01: fail  
*/
//extern uint8_t Finger_Calculate_Feature(void);



/*!
    \brief  Finger_Save_Template :  Save the template 
    \param[in]  SaveFingerId : IDx(x=0~99)   
    \param[out] 
		\retval  0x00: success, 0x01: fail  
*/
//extern uint8_t Finger_Save_Template(uint16_t SaveFingerId);

/*!
    \brief  Finger_Read_Finger_State :  Judge the finger is still on the sensor 
    \param[in]  void  
    \param[out] 
		\retval  0x00: the finger is on the sensor, 0x01: the finger is not on the sensor 
*/
extern uint8_t Finger_Read_Finger_State(void);




#define BYD_FPS_MAX_ENROLL_NUM          3   // The maximum number of entries, 3~8 times
#define BYD_ALG_SECURITY_LEVEL          1   //Security level, support 1/2/3



/* the pointer point to the image buf, 
BF5835A/BF538A/BF5926/BF5325A/BF5325AS:the size is 160*160*2 bytes;
*/
extern uint8_t *ptImageData;



#endif
