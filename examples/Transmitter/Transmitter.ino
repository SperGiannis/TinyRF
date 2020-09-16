#include "TinyRF_TX.h"

void setup(){
	// transmitter default pin is pin #2. You can change it by editing Settings.h
	setupTransmitter();
}

void loop(){

	const char* msg = "Hello from far away!";

	//send function accepts an array of bytes as first argument
	//second argument is the length of the array
	send((byte*)msg, strlen(msg));

	//make sure there's at least a MIN_TX_INTERVAL delay between transmissions
	//otherwise the receiver's behavior will be undefined
	delay(MIN_TX_INTERVAL);

	// you can provide a third argument to send a message multiple times
	// this is for reliability in case some messages get lost in the way
	// when sending multiple messages make sure you call getReceivedData() frequently in the receiver 
	// the receiver has a 256 byte FIFO buffer, if you send too many messages and/or if they are
	// too long previous messages will get overwritten in the buffer
	// it is socially more responsible to use fewer repetition to minimize your usage of the bandwidth
	sendMulti((byte*)msg, strlen(msg), 10);

	delay(1000);
	
}