//  File player.cpp

//  Author: Renzo Dani
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.

#include <Arduino.h>
#include <SD.h>
#include "string.h"
#include "player.h"
#include "config.h"
#include "vs10xx.h"
#include "newSDLib.h"
#include "Keypad.h"

#define fileNameSize 50
unsigned char playStop = 1; // play or stop flag,1-play,0-stop
int sameFileCheck = 0;
bool type_wav = true;

int dirN = 0;
int fileN = 0;
File currentFile;
/**
 * Restart play list
 */
void rewind(){
	fileN = 0;
}
void nextdir(){
	dirN++;
	if(dirN > 10) dirN = 0;
}
void nextFile(){
	fileN++;
}

/**
 * Initialize current playing:
 * 1. create file name
 * 2. check file exists
 * 3. if not try the other extension
 * 4. if already tried the other extension restart play list
 */
void initP(){
	playStop = 1;

	Serial.print("open dir:");
	Serial.print(dirN);
	Serial.print(" file:");
	Serial.println(fileN);

	char fileName[fileNameSize];
	String fnS = String(dirN);
	fnS += "/";
	fnS += fileN;
	if(type_wav){
		fnS += ".wav";
	}else{
		fnS += ".mp3";
	}
	fnS.toCharArray(fileName, fileNameSize);

	currentFile = SD.open(fileName);
	if(!currentFile){//file does not exists
		sameFileCheck++;

		playStop = 0;//stop playing and do not skip to next file
		type_wav = !type_wav; //try other extension
		if(sameFileCheck >= 2){
			rewind(); //all extensions checked: restart play list
		}

		return;
	}else{
		sameFileCheck = 0;
	}

}

/**
 * Close current file and point to the next one.
 */
void closeP(){
	if(currentFile != NULL && currentFile) currentFile.close();
	if(playStop == 1) nextFile();
}

/**
 * Copy content of the file to the buffer.
 */
int readFile(byte *buffer, int len) {
	int readLen = 0;
	readLen = currentFile.read((char*)buffer,len);
	return readLen;
}


/*******************************************************************/
void scanKey() {
	String msg;

	// Fills kpd.key[ ] array with up-to 10 active keys.
	// Returns true if there are ANY active keys.
	if (kpd.getKeys()) {
		for (int i = 0; i < LIST_MAX; i++)   // Scan the whole key list.
				{
			if (kpd.key[i].stateChanged) // Only find keys that have changed state.
			{
				switch (kpd.key[i].kstate) { // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
				case PRESSED:
					msg = " PRESSED.";
					pressed(kpd.key[i].kchar);
					break;
				case HOLD:
					msg = " HOLD.";
					break;
				case RELEASED:
					msg = " RELEASED.";
					break;
				case IDLE:
					msg = " IDLE.";
				}
				Serial.print("Key ");
				Serial.print(kpd.key[i].kchar);
				Serial.println(msg);
			}
		}
	}
}

void pressed(char presseChar) {
	if(presseChar == '9') return; //there is currently a problem on hardware, so skip button 9

	int dirPressed;
	if(presseChar == '*'){
		dirPressed = 9;
	}else if(presseChar == '#'){
		dirPressed = 10;
	}else{
		dirPressed = presseChar - '0';
	}


	Serial.print("choose:");
	Serial.println(dirPressed);

	if(dirPressed == dirN){
		nextFile();
	}else{
		dirN = dirPressed;
		rewind();
	}

	playStop = 0;
}



//do some interactive things when vs1053 is busy
void AvailableProcessorTime() {
	scanKey();
}

int playFile() {

	initP();
	AvailableProcessorTime();
	if (0 == playStop){
		closeP();
		return 0;
	}

	Mp3SoftReset();  //it is necessary to play each music file


	int readLen = 0;
	byte readBuf[READ_BUF_LEN];
	byte *tp = readBuf;
	while (1) {
		readLen = readFile(readBuf, READ_BUF_LEN); //read file content length of 512 every time
		tp = readBuf;
		//Serial.println(readLen);

		Mp3SelectData();

		while (tp < readBuf + readLen) {
			if (!MP3_DREQ) {
				while (!MP3_DREQ) {
					Mp3DeselectData();

					AvailableProcessorTime(); //here vs1053 is busy, so you can do some interactive things, like key scanning,led controlling.
					if (0 == playStop){
						closeP();
						return 0;
					}else{
						break;
					}

					Mp3SelectData();
				}
			}
			// Send music content data to VS10xx
			SPIPutChar(*tp++);
		}

		SPIWait();
		Mp3DeselectData();

		if (readLen < READ_BUF_LEN) {
			Mp3WriteRegister(SPI_MODE, 0, SM_OUTOFWAV);
			SendZerosToVS10xx();
			break;
		}
	};
	Serial.println("played over\r\n");

	closeP();
	return 0; //OK Exit
}

