#ifndef ASTEROIDOS_MEDAKA_TOOLS_H
#define ASTEROIDOS_MEDAKA_TOOLS_H

namespace AsteroidOS::LCD_Tools::Medaka {
	void SyncTime(int);
	void SyncSettings(int);
	void SetDisplayColor(bool value, bool persist = true);
	void SetDisplayColor(bool value);
    void PrepareTimepiece(int);

    int SetCompassDisplay(float bearingoffset);
    int SetDisplayString(const char* input, int inputlength, int set1,bool set2);
    int SetIconDisplay(int input, int input2, bool input3);
    int SetNumberDisplay(const char* input, int inputlength, char punctuation);
    int SetGraphDisplay();
    int Set2LayerMode(int top, int bottom);
    int TestDisplay();
    int DisplayReset();
}
#endif //ASTEROIDOS_MEDAKA_TOOLS_H
