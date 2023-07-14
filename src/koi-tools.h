#ifndef ASTEROIDOS_KOI_TOOLS_H
#define ASTEROIDOS_KOI_TOOLS_H

namespace AsteroidOS::LCD_Tools::Koi {
	void SyncTime(int);
	void SyncSettings(int);
	void SetDisplayColor(bool value, bool persist = true);
	void PrepareTimepiece(int);
}
#endif //ASTEROIDOS_KOI_TOOLS_H
