#ifndef ASTEROIDOS_MEDAKA_TOOLS_H
#define ASTEROIDOS_MEDAKA_TOOLS_H

namespace AsteroidOS::LCD_Tools::Medaka {
	void SyncTime(int);
	void SyncSettings(int);
	void SetDisplayColor(bool value, bool persist = true);
}
#endif //ASTEROIDOS_MEDAKA_TOOLS_H
