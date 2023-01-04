#ifndef ASTEROIDOS_KOI_TOOLS_H
#define ASTEROIDOS_KOI_TOOLS_H

namespace AsteroidOS::LCD_Tools {
	int KoiSyncTime();
	int KoiSetDisplayColor(bool value);
	int Write(char* data);
}
#endif //ASTEROIDOS_KOI_TOOLS_H
