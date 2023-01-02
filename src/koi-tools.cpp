#include "koi-tools.h"

#include <iostream>
#include <dlfcn.h>
#include <MGConfItem>

#include <QTime>
#include <QDate>
#include <QFile>

int AsteroidOS::LCD_Tools::KoiSyncTime() {
	char outputBuff[7] = {0xFE,0x01,0x01,0x00,0x00,0x00,0x00};
	outputBuff[3] = (uint8_t) !MGConfItem("/org/asteroidos/settings/use-12h-format").value().toBool();
	Write(outputBuff);

	QDate dateNow = QDate::currentDate();
	QTime timeNow = QTime::currentTime();
	outputBuff[0] = (uint8_t) 0x50;
	outputBuff[1] = (uint8_t) 0;
	outputBuff[2] = (uint8_t)((((dateNow.year() - 2000) & 0xEF) << 1) | ((dateNow.month() & 0x08) >> 3));
	outputBuff[3] = (uint8_t) ((dateNow.month() & 0x07) << 5) | ((dateNow.day() & 0x1F));
	outputBuff[4] = (uint8_t) (((dateNow.dayOfWeek() & 0x07) << 5) |  (timeNow.hour() & 0x1F));
	outputBuff[5] = (uint8_t) (timeNow.minute() & 0xFF);
	outputBuff[6] = (uint8_t) ((timeNow.second() + 1) & 0xFF); //the original casio code seems to add one second here to compensate for an update delay in the subcpu
	return Write(outputBuff);
}

int AsteroidOS::LCD_Tools::Write(char* data) {
	QFile multiSensorFile("/dev/MultiSensors_CD_01");
	if(!multiSensorFile.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qCritical("Unable to open file for write. Check permissions");
		return(-1);
    }
	multiSensorFile.write(data,7);
	multiSensorFile.close();
	return 1;
}
