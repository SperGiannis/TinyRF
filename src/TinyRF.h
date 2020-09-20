#ifndef TRF_H
#define TRF_H

#include "Arduino.h"
#include "Settings.h"

//#define DEBUG

//enable serial output if board is not ATtiny13 (assuming using MicroCore)
//used for developement debugging
#ifndef __AVR_ATtiny13__
	#define TRF_SERIAL_ENABLE
#endif


#if defined(TRF_SERIAL_ENABLE) && defined(DEBUG)
	#define TRF_SERIAL_BEGIN(x) (Serial.begin(x))
	#define TRF_PRINTLN(x) (Serial.println(x))
	#define TRF_PRINT(x) (Serial.print(x))
	#define TRF_PRINT2(x,y) (Serial.print(x,y))
#else
	#define TRF_SERIAL_BEGIN(x)
	#define TRF_PRINTLN(x)
	#define TRF_PRINT(x)
	#define TRF_PRINT2(x,y)
#endif


#if defined(TRF_TX_UNCALIBRATED)
	#define TRF_CALIB_ERROR 10
#elif defined(TRF_TX_CALIBRATED)
	#define TRF_CALIB_ERROR 2
#elif defined(TRF_TX_CRYSTAL)
	#define TRF_CALIB_ERROR 0
#endif


#if defined(TRF_ERROR_CHECKING_CRC)
	#define TRF_ERR_CHK_FUNC crc8
#elif defined(TRF_ERROR_CHECKING_CHECKSUM)
	#define TRF_ERR_CHK_FUNC checksum8
#endif


/**
 * This isn't used anywhere in the library. It is defined here for reference and for being used
 * as buffer size in programs that use this library.
 * Even tho sending messages of this length is theoretically possible, it is not recommended to 
 * send anything larger than 32 bytes. The longer a message is the more susceptible to noise it become
 * On top of that, the CRC function will become weaker and weaker as the message length increases
 * Also it will take more time to calculate the CRC, increasing transmission time and bugging down
 * the receiver CPU
**/
#define TRF_MAX_MSG_LEN TRF_RX_BUFFER_SIZE-5	//-5 to account for len, CRC, seq# and just to be safe


//data rate presets
#ifdef TRF_BITRATE_240
	const uint16_t START_PULSE_PERIOD = 6000;
	const uint16_t ONE_PULSE_PERIOD = 4000;
	const uint16_t ZERO_PULSE_PERIOD = 3000;
	const uint16_t PERIOD_HIGH_DURATION = 2000;
	const uint16_t TRIGER_ERROR = 50;
	const uint16_t NUM_PREAMBLE_BYTES = 3;
#endif

#ifdef TRF_BITRATE_500
	const uint16_t START_PULSE_PERIOD = 3000;
	const uint16_t ONE_PULSE_PERIOD = 2000;
	const uint16_t ZERO_PULSE_PERIOD = 1500;
	const uint16_t PERIOD_HIGH_DURATION = 1000;
	const uint16_t TRIGER_ERROR = 50;
	const uint16_t NUM_PREAMBLE_BYTES = 6;
#endif

#ifdef TRF_BITRATE_1100
	const uint16_t START_PULSE_PERIOD = 1998;	//for some reason these weird values save 4 bytes
	const uint16_t ONE_PULSE_PERIOD = 1002;
	const uint16_t ZERO_PULSE_PERIOD = 752;
	const uint16_t PERIOD_HIGH_DURATION = 498;
	const uint16_t TRIGER_ERROR = 30;
	const uint16_t NUM_PREAMBLE_BYTES = 15;
#endif

#ifdef TRF_BITRATE_2500
	#ifdef TRF_TX_UNCALIBRATED
		#warning "This data rate is too fast for an uncalibrated ATtiny."
	#endif
	const uint16_t START_PULSE_PERIOD = 2000;
	const uint16_t ONE_PULSE_PERIOD = 400;
	const uint16_t ZERO_PULSE_PERIOD = 300;
	const uint16_t PERIOD_HIGH_DURATION = 200;
	const uint16_t TRIGER_ERROR = 30;
	const uint16_t NUM_PREAMBLE_BYTES = 30;
#endif

const uint16_t ONE_PULSE_TRIGG_ERROR = (TRIGER_ERROR + ONE_PULSE_PERIOD * TRF_CALIB_ERROR / 100);
const uint16_t ZERO_PULSE_TRIGG_ERROR = (TRIGER_ERROR + ZERO_PULSE_PERIOD * TRF_CALIB_ERROR / 100);
const uint16_t START_PULSE_TRIGG_ERROR = (TRIGER_ERROR + START_PULSE_PERIOD * TRF_CALIB_ERROR / 100);
//for some reason longer delays are more inaccurate, so our start pulse maximum needs more leeway for error
const uint16_t START_PULSE_MAX_ERROR = 2*START_PULSE_TRIGG_ERROR;

const uint16_t MIN_TX_INTERVAL_REAL = START_PULSE_PERIOD + START_PULSE_MAX_ERROR;
//this is for end-user usage 
const uint16_t MIN_TX_INTERVAL = (MIN_TX_INTERVAL_REAL * 2)/1000;


/**
 * Function declarations
**/
byte checksum8(byte data[], uint8_t len, uint8_t seq = 0);
byte crc8(byte data[], uint8_t len, uint8_t seq = 0);


#endif  /* TRF_H */ 
