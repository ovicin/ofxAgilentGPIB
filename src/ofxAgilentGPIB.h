/* 
	Wrapper for Agillent IOLibSuite_16_3_16603
	Tested with AGILENT_82357A_USB_GPIB_INTERFACE and 
	Chroma63200 Electronic Load
*/

#ifndef ofxAgilentGPIB_H
#define ofxAgilentGPIB_H

/* include section				*/
/* ---------------------------- */
#include "ofMain.h"


/* definitions  				*/
/* ---------------------------- */
#define MODE_RS232 0
#define MODE_GPIB  1

#define DEFAULT_SERIAL_COM "\\\\.\\COM14"
//#define DEFAULT_SERIAL_COM 0
#define DEFAULT_SERIAL_BAUD 115200

/* classes		 				*/
/* ---------------------------- */
class ofxAgilentGPIB{
	private:
		char mode; /* 0 -> rs232;1 -> GPIB */
		ofSerial	serial;
		

	public:
		ofxAgilentGPIB();
		int gpibOpen(void);
		int rs232Open(string port, int baud);
		int rs232Open(void);
		void gpibSetTimeout(int id, int ms);
		void gpibSendCommand(int id,const char *command,unsigned char len);
		void gpibReadResp(unsigned char *buff, unsigned char len);
		void setConfRemOn(void);
		void setLoad(double value);
		void setRCMode(void);


		void sicl_test(void);
		bool RS232CommunicationStatus;

};

#endif