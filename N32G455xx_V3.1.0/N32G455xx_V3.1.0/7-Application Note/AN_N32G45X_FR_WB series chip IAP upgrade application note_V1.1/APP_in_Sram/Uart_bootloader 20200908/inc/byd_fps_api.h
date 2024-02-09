/*!
    \file  byd_fps_api.h
    \brief api define
*/

/*
    Copyright (C) 2019 BYD

    2019-12-27, V1.0.0, api for byd
*/

#ifndef BYD_FPS_API_H
#define	BYD_FPS_API_H

#include "n32g4fr.h"

#define RT_OK                    0x00 //Success
#define RT_FAIL                  0x01 //Failure
#define RT_NO_FG                 0x02 //No finger
#define RT_CHIP_RESET_OK         0x03 //chip reset successfully
#define RT_CHIP_RESET_FAIL       0x04 //chip reset failed
#define RT_CHIP_FAIL             0x05 //The chip cannot be used
#define RT_SH_FAIL               0x06 //Failed to program the offset value to flash
#define RT_NOMT_FAIL             0x07 //The combination scheme has not been tested for modules
#define RT_MT_FAIL               0x08 //The result of the combined solution module test failed
#define RT_KEY_BASE_FAIL         0x09 //Key baseline software setting failed
#define RT_FG_BASE_FAIL          0x0a //Failed to set finger baseline software
#define RT_FG_BASE_ERROR         0x0b //Finger baseline error
#define RT_SFR_BAK_FAIL          0x0c //Error writing backup register
#define RT_EXCEPTION_FAIL        0x0d //Exception occurred
#define RT_FG_CFG_FAIL           0x0e //Error modifying finger configuration word
#define RT_SFR_CHECK_ERROR       0x80 //sfr check error
#define RT_COVER	             0xc5 //with cover

#define FINGER_DOWN    		     0xaa
#define FINGER_UP                0x55

#define VAL_CHIP_MODE_FINGER            0x33//Finger scan, fast timing
#define VAL_CHIP_MODE_FINGER_LOWPOWER   0x55//Finger scan, slow timing, low power finger scan

#define CHIP_NORMALMODE                 0x11//Normal working mode, get image data
#define CHIP_TESTMODE                   0x22//module test working mode

/* variable declarations */
extern uint8_t LastFingerState;
extern uint32_t BYD_SPI;
extern uint32_t *byd_spi_buf;
extern uint16_t g_usSpiRxCount;
extern volatile uint8_t g_ucSpiIntFlag;

extern uint8_t m_WriteOffsetValFlag;

extern uint8_t *ImagePointer;

/* function declarations */
/*!
    \brief      spi config
    \param[in]  spi_periph: SPI0/SPI1
    \param[out] none
    \retval     none
*/
extern void byd_fps_spi_config(uint32_t spi_periph);

/*!
    \brief      spi dma config
    \param[in]  none
    \param[out] none
    \retval     none
*/
extern void byd_fps_spidma_config(void);

/*!
    \brief      read chip id and check
    \param[in]  buf:chip id is 2 bytes
			\arg      bf5835\bf5325 id :0x5e81
			\arg      bf5836 id :0x568e/0x568c/0x568a/0x5688/0x5686/0x5684
			\arg      bf5838 id :0x877f
			\arg      bf5926 id :0x8f7f/0x5e81
    \param[out] none
    \retval     0:the chip id is correct;1:the chip id is wrong
*/
extern uint32_t byd_fps_check_chip_identify(uint8_t *buf);

/*!
    \brief      read module id
    \param[in]  buf:
			\arg      bf5835 :0x5e81 + 8 random bytes 
			\arg      bf5836 :0x568e/0x568c/0x568a/0x5688/0x5686/0x5684 + 8 random bytes
			\arg      bf5838 :0x877f + 4 random bytes
			\arg      bf5926 :0x8f7f/0x5e81 + 4 random bytes
    \param[out] none
    \retval     none
*/
extern void byd_fps_read_moduleID(uint8_t *IdBuf);

/*!
    \brief      fps initalization
    \param[in]  none
    \param[out] none
    \retval     0x00:sucess;
								0x01:chip id is wrong
								other:fail								 
								      RT_CHIP_RESET_FAIL 
								      RT_CHIP_FAIL       
								      RT_SH_FAIL         
								      RT_NOMT_FAIL       
								      RT_MT_FAIL         
								      RT_KEY_BASE_FAIL								 
								      RT_SFR_CHECK_ERROR
								      RT_FG_BASE_FAIL(bf5838/bf5926)  
								      RT_SFR_BAK_FAIL(bf5838)
											RT_FG_CFG_FAIL(bf5926)
*/
extern uint32_t byd_sensor_init(void);

/*!
    \brief      reset fps
    \param[in]  none
    \param[out] none
    \retval     0x03:RT_CHIP_RESET_OK 
		            0x04:RT_CHIP_RESET_FAIL
		            0x0a:RT_FG_BASE_FAIL(bf5838/bf5926)
		            0x0c:RT_SFR_BAK_FAIL(bf5838)
*/
extern uint32_t byd_reset_sensor(void);

/*!
    \brief      detect finger and get 8bit fingerprint image
    \param[in]  _pAlgBuf: imagebuffer pointer
		\param[in]  fingermode: 
			\arg      0x33: fast finger detect
			\arg      0x55: low power finger detect
    \param[out] none
    \retval     0x00:sucess;
								0x01:fail;
								0x02:RT_NO_FG
								0x04:RT_CHIP_RESET_FAIL
                0x0c:RT_SFR_BAK_FAIL(bf5838)
								0x0d:RT_EXCEPTION_FAIL(bf5838/bf5926)
*/
extern uint32_t byd_fps_workprocess(uint8_t *_pAlgBuf,uint32_t fingermode);

/*!
    \brief      read finger state
    \param[in]  none
    \param[out] none
    \retval     0xaa:finger touch;0x55:finger up;0x02:no finger
*/
extern uint32_t byd_fps_finger_detect_state(void);

/*!
    \brief      detect finger
    \param[in]  fingermode: 
			\arg      0x33: fast finger detect
			\arg      0x55: low power finger detect
    \param[out] none
    \retval     0x00:sucess;
								0x04:RT_CHIP_RESET_FAIL								
								0x80:RT_SFR_CHECK_ERROR		
*/
extern uint32_t byd_fps_finger_detect_mode(uint32_t fingermode);

/*!
    \brief      get 8bit fingerprint image
    \param[in]  _pAlgBuf: imagebuffer pointer
    \param[out] none
    \retval     0x00:sucess;
								0x01:fail;								
								0x04:RT_CHIP_RESET_FAIL(bf5838)
                0x0c:RT_SFR_BAK_FAIL(bf5838)
								0x0d:RT_EXCEPTION_FAIL(bf5838/bf5926)
*/
extern uint32_t byd_fps_get_image8bit(uint8_t *_pAlgBuf);

/*!
    \brief      get fpskey rawdata
    \param[in]  fps_keyrawdata
    \param[out] none
    \retval     0:sucess;1:fail
*/
extern uint32_t byd_fps_read_keyrawdata(uint16_t *fps_keyrawdata);

/*!
    \brief      get fpskey baseline
    \param[in]  fps_keybaseline
    \param[out] none
    \retval     0:sucess;1:fail
*/
extern uint32_t byd_fps_read_keybaseline(uint16_t *fps_keybaseline);

/*!
    \brief      get finger rawdata(bf5838/bf5926)
    \param[in]  fg_rawdata
    \param[out] none
    \retval     0:sucess;1:fail
*/
extern uint32_t byd_fg_read_rawdata(uint16_t *fg_rawdata);

/*!
    \brief      get finger baseline(bf5838/bf5926)
    \param[in]  fg_baseline
    \param[out] none
    \retval     0:sucess;1:fail
*/
extern uint32_t byd_fg_read_baseline(uint16_t *fg_baseline);

/*!
    \brief      get error state(bf5838/bf5926)
    \param[in]  errorflag: 0 read from register; 1 read from global variable
    \param[out] none
    \retval:    bit[0]:key touch,senor no touch error
                bit[1]:key time out error
                bit[2]:finger baseline error
					      bit[3]:overtime error(except sleep mode)
					      bit[4]:lvdt<(2.6~2.9v)
                bit[5]:lvdt<2.1v
                bit[6]:sfr error(only bf5838)
*/
extern uint32_t byd_fps_read_errorstate(uint32_t errorflag);

/*!
    \brief      enter fps sleep mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
extern void byd_fps_sleep(void);

/*!
    \brief      enter fps idle mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
extern void byd_fps_idle(void);

/*!
    \brief      read fps reset state
    \param[in]  none
    \param[out] none
    \retval     reset state
								bit[0]:power on
								bit[1]:soft reset
								bit[2]:adj reset
								bit[3]:switch mode
								bit[4]:lvdt reset(bf5838/bf5926)
*/
extern uint8_t byd_fps_rst_state(void);

/*!
    \brief      clear fps reset state
    \param[in]  none
    \param[out] none
    \retval     none
*/
extern void byd_fps_rst_clear(void);

/*!
    \brief      get fps osdata and write to flash
    \param[in]  none
    \param[out] none
    \retval     0:sucess;1:get data fail;2:the data don't meet the standard;3:flash read and write data are not equal
		\notice     make sure there is no finger or other cover on the fps_sensor!!!!!!!!!!!!!!!!!!
*/
extern uint32_t byd_fps_offsetval_process(uint8_t *_pAlgBuf);



extern void byd_fps_init(void);

#endif
