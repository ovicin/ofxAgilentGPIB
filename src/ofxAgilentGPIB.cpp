/////////////////////////////////////////////////////////////////////
//
// The following simple demonstration program uses the Standard 
// Instrument Control Library to query an GPIB instrument for
// an identification string and then prints the result.
//
// Edit the DEVICE_ADDRESS line below to specify the address of the 
// device you want to talk to.  For example:
//
//     gpib0,0    - refers to an GPIB device at bus address 0 
//                  connected to an interface named "gpib0" by the 
//                  I/O Config utility.
//
//     gpib0,9,0  - refers to an GPIB device at bus address 9, 
//                  secondary address 0, connected to an interface 
//                  named "gpib0" by the I/O Config utility.
//
// Note that this program is meant to be built either as a WIN16 
// QuickWin or EasyWin program on 16 bit Windows 95, or as a WIN32
// console application on 32 bit Windows 95 or Windows NT.  Also 
// note that WIN16 programs must be compiled with the Large memory 
// model.
//
/////////////////////////////////////////////////////////////////////
#pragma once
#include <stdio.h>              // for printf()
#include <string.h>
#include "sicl.h"		// Standard Instrument Control Library routines
#include "ofxAgilentGPIB.h"


#define DEVICE_ADDRESS "gpib0,5"   // Modify this line to match your setup

ofxAgilentGPIB::ofxAgilentGPIB()
{
	

}

/*------------------------------------*/
int ofxAgilentGPIB::gpibOpen(void){
	/*install error handler*/
	ofxAgilentGPIB::mode = MODE_GPIB;
	::ionerror(I_ERROR_NO_EXIT);
	return ::iopen(DEVICE_ADDRESS);
}

/*------------------------------------*/
int ofxAgilentGPIB::rs232Open(string port, int baud){
	ofxAgilentGPIB::mode = MODE_RS232;

	ofxAgilentGPIB::serial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = ofxAgilentGPIB::serial.getDeviceList();

	bool stat;
	stat = ofxAgilentGPIB::serial.setup(port, baud);// windows example 
	if (stat)	
		RS232CommunicationStatus = true;
	else
		RS232CommunicationStatus = false;

	return stat;
}

int ofxAgilentGPIB::rs232Open(void){
	ofxAgilentGPIB::mode = MODE_RS232;

	ofxAgilentGPIB::serial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = ofxAgilentGPIB::serial.getDeviceList();
	// this should be set to whatever com port your serial device is connected to.
	// (ie, COM4 on a pc, /dev/tty.... on linux, /dev/tty... on a mac)
	// arduino users check in arduino app....
	//serial.setup(0, baud); //open the first device
	int baud=DEFAULT_SERIAL_BAUD;
	bool stat;
	stat = ofxAgilentGPIB::serial.setup(DEFAULT_SERIAL_COM, DEFAULT_SERIAL_BAUD);// windows example 
	if (stat)	
		RS232CommunicationStatus = true;
	else
		RS232CommunicationStatus = false;

	return stat;
}

/*------------------------------------*/
void ofxAgilentGPIB::gpibSetTimeout(int id, int ms){
	if (ofxAgilentGPIB::mode == MODE_RS232){
		/* for the moment do nothing */
	}
	else {/* not tested */
	/* MODE_GPIB */
		::itimeout(id, ms);
	}
}


/*------------------------------------*/
void ofxAgilentGPIB::gpibSendCommand(int id,const char *command, unsigned char len){
	if (ofxAgilentGPIB::mode == MODE_RS232){
		ofxAgilentGPIB::serial.writeBytes((unsigned char*)command,len);
	}
	else { /* not tested */
	/* MODE_GPIB */
	   // Write the *RST string (and send an EOI indicator) to put the instrument
	   // in a known state.
	   ::iprintf(id, (char*)command);

	   // Write the *IDN? string and send an EOI indicator, then read
	   // the response into buf.  
	   // For WIN16 programs, this will only work with the Large memory model 
	   // since ipromptf expects to receive far pointers to the format strings.

	   //::ipromptf(id, "*IDN?\n", "%t", respbuf);
	   //printf("%s\n", respbuf);
	}
}

/*------------------------------------*/

void ofxAgilentGPIB::gpibReadResp(unsigned char *buff, unsigned char len){
	if (ofxAgilentGPIB::mode == MODE_RS232){
		ofxAgilentGPIB::serial.readBytes(buff, len);
	} else{
		/* MODE_GPIB */
	}
}


void ofxAgilentGPIB::setConfRemOn(void){
	gpibSendCommand(0,"CONF:REM ON\n", sizeof("CONF:REM ON\n"));
}

void ofxAgilentGPIB::setRCMode(void){
	gpibSendCommand(0,"MODE 5\n", sizeof("MODE 5\n"));
}

void ofxAgilentGPIB::setLoad(double value){
	char *command;
	command = new char[15];
	sprintf(command,"RES:L1 %.2lf\n",value);
	gpibSendCommand(0,command, strlen(command));
	//gpibSendCommand(0,"RES:L1 35.71\n", sizeof("RES:L1 35.71\n"));
	delete command;
}

/*
	--- TO BE REMOVED ---
	Function to test the gpib connection

*/
void ofxAgilentGPIB::sicl_test(void)
{
   INST id;                 	// device session id
   char buf[256] = { 0 };   	// read buffer for idn string

   #if defined(__BORLANDC__) && !defined(__WIN32__)
     _InitEasyWin();		// required for Borland EasyWin programs.
   #endif

   // Install a default SICL error handler that logs an error message and
   // exits.  On Windows 95 view messages with the SICL Message Viewer,
   // and on Windows NT use the Windows NT Event Viewer.
   ::ionerror(I_ERROR_NO_EXIT);	
  
   // Open a device session using the DEVICE_ADDRESS
   id = ::iopen(DEVICE_ADDRESS);
   if (::igeterrno())
		printf("GPIB Error: %s",::igeterrstr(::igeterrno()));

   // Set the I/O timeout value for this session to 1 second
   ::itimeout(id, 1000);
  
   // Write the *RST string (and send an EOI indicator) to put the instrument
   // in a known state.
   ::iprintf(id, "*RST\n");

   // Write the *IDN? string and send an EOI indicator, then read
   // the response into buf.  
   // For WIN16 programs, this will only work with the Large memory model 
   // since ipromptf expects to receive far pointers to the format strings.

   ::ipromptf(id, "*IDN?\n", "%t", buf);
   printf("%s\n", buf);

   ::iclose(id);

   // For WIN16 programs, call _siclcleanup before exiting to release 
   // resources allocated by SICL for this application.  This call is a
   // no-op for WIN32 programs.
   ::_siclcleanup();
}
