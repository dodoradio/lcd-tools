#include "medaka-tools.h"

#include <array>
#include <iostream>
#include <MGConfItem>

#include <QTime>
#include <QDate>
#include <QFile>

#include<chrono>
#include<thread>

using SpiMsg = std::array<uint8_t, 7>;

static void Write(const SpiMsg& data) {
	QFile multiSensorFile("/dev/MultiSensors_CD_01");
	if(!multiSensorFile.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qCritical("Unable to open file for write. Check permissions");
		return;
	}
	multiSensorFile.write((const char *)data.data(), data.size());
	multiSensorFile.close();
}

static void MedakaSet12H(bool value) {
	Write({0xFE,0x01,0x01,value,0x00,0x00,0x00});
}

namespace AsteroidOS::LCD_Tools::Medaka {

void SyncTime(int) {
	MedakaSet12H(!MGConfItem("/org/asteroidos/settings/use-12h-format").value().toBool());
	QDate dateNow = QDate::currentDate();
	QTime timeNow = QTime::currentTime();
	Write({
	(uint8_t) 0x50,
	(uint8_t) 0,
	(uint8_t) ((((dateNow.year() - 2000) & 0xEF) << 1) | ((dateNow.month() & 0x08) >> 3)),
	(uint8_t) ((dateNow.month() << 5) | (dateNow.day() & 0x1F)),
	(uint8_t) (((dateNow.dayOfWeek() % 7) << 5) | (timeNow.hour() & 0x1F)), //weeks are counted from sunday, sunday encoded as zero.
	(uint8_t) (timeNow.minute() & 0xFF),
	(uint8_t) ((timeNow.second()) & 0xFF)
	});
}

void SyncSettings(int) { // this is meant to be run on a session restart
	SetDisplayColor(MGConfItem("/org/asteroidos/lcd-tools/medaka/display-color").value().toInt(),false);
}

void SetDisplayColor(bool value, bool persist) {
	if (persist) {
		MGConfItem("/org/asteroidos/lcd-tools/medaka/display-color").set(value);
	}
	Write({0xFE,0x01,0x05,(value ? 0x01 : 0x02),0x00,0x00,0x00});
}

void PrepareTimepiece(int) {
       Write({0x02,0x00,0x00,0x00,0x00,0x00,0x00});
       Write({0xFE,0x81,0x01,0x00,0x00,0x00,0x00});
}
int SetCompassDisplay(float bearingoffset) { //works
       int bearingint = bearingoffset + 0.5;
       Write({0xFE,0x00,0x03,0x00,0x00,0x00,0x00});
       Write({0xFE,0x01,0x03,(bearingint ^ 0xFF),((bearingint>>8) ^ 0xFF),((bearingint>>16) ^ 0xFF),((bearingint>>24) ^ 0xFF)});
       Write({0xFE,0x00,0x83,0x00,0x00,0x00,0x00});
       return 1;
}

int SetDisplayString(const char* input, int inputlength, int set1, bool set2) { //broken. only one letter shows.
       Write({0xFE,0x00,0xA5,0x00,0x00,0x00,0x00});
       Write({0xFE,(set2 ? 0x06 : 0x03),set1,input[0],input[1],input[2],input[3]});
       if ((set2 == 1)) {
               Write({0xFE,0x07,input[4],input[5],0x00,0x00,0x00});
       }
       Write({0xFE,0x00,0x85,0x00,0x00,0x00,0x00});
       return 1;
}

int SetIconDisplay(int input, int input2, bool input3) { //works, kinda? it only lets us see 3 or so icons, those being 0%, Kj and Cal
       Write({0xFE,0x00,0xA5,0x00,0x00,0x00,0x00});
       Write({0xFE,0x02,(input & 0xFF),(input > 255 ? (input2 | 0x80) : input2),input3,0x00,0x00});
       Write({0xFE,0x00,0x85,0x00,0x00,0x00,0x00});
       return 1;
}

int SetNumberDisplay(const char* input, int inputlength,char punctuation) { //works, but punctuation doesn't
       inputlength = std::min(inputlength,6);
       int offset = 4 - inputlength;
       offset = std::max(offset,0);
       char outputarr[6] = {0x0F,0x0F,0x0F,0x0F,0x0F,0x0F};
       for(int i = 0; i < inputlength; i++) {
               if(!(input[i] == 0x20)) {
                       int num = input[i] - '0';
                       outputarr[i+offset] = num&0x0F;
               }
       }
       Write({0xFE,0x00,0xA5,0x00,0x00,0x00,0x00});
       Write({0xFE,0x04,((outputarr[0]<<4)|outputarr[1]),((outputarr[2]<<4)|outputarr[3]),(outputarr[4]<<4|outputarr[5]),(punctuation),0xFF});
       Write({0xFE,0x00,0x85,0x00,0x00,0x00,0x00});
       return 1;
}

int SetGraphDisplay() { //draws a squashed 'ASTEROIDOS' to the dot matrix display - works
       Write({0xFE,0x00,0xA5,0x00,0x00,0x00,0x00});
       Write({0xFE,0x02,0x01,0x17,0b11110100,0b10011011,0b0110100});
       Write({0xFE,0x02,0x18,0x17,0b01001110,0b01101011,0b1010101});
       Write({0xFE,0x02,0x2F,0x17,0b01010101,0b01011001,0b1010101});
       Write({0xFE,0x02,0x46,0x17,0b01001101,0b10101011,0b0110100});
       Write({0xFE,0x00,0x85,0x00,0x00,0x00,0x00});
       return 1;
}

int TestDisplay() {
       int i = 0;
       while(i < 255) {
               Write({0xFE,0x00,0xA5,0x00,0x00,0x00,0x00});
               Write({0xFE,0x02,i,0x01,0b00000001,0x00,0x00});
               Write({0xFE,0x00,0x85,0x00,0x00,0x00,0x00});
               i++;
               std::this_thread::sleep_for(std::chrono::milliseconds(int (100)));
       }
       return 1;
}

int Set2LayerMode(int top, int bottom) {
       Write({0xFE,0x01, 0x19, top, bottom,0x00,0x00});
       Write({0xFE,0x00, 16, 0x00, 0x00,0x00,0x00});
       return 1;
}

int DisplayReset() { //works, kinda? display stops showing anything after this one, which doesn't seem super useful.
       Write({0xFE,0x00,0x01,0x00,0x00,0x00,0x00});
       return 1;
}

int SetStepCount(int input) {
	   Write({0xFE,0x00,0x0a,0x00,0x00,0x00,0x00});
	   Write({0xFE,0x01,0x0a,(input&0xFF),((0x00>>8)&0xFF),((0x00>>16)&0xFF),((0x00>>24)&0xFF)});
	   Write({0xFE,0x00,0x8a,0x00,0x00,0x00,0x00});
}
} // end of namespace AsteroidOS::LCD_Tools::Medaka
