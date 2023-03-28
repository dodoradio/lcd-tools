#ifndef ASTEROIDOS_CATFISH_TOOLS_H
#define ASTEROIDOS_CATFISH_TOOLS_H

#include <cstdint>

class CatfishLcdTools {
public:
	CatfishLcdTools();
	virtual ~CatfishLcdTools();
	virtual int SyncTime();
	virtual int PrepareTimepiece();
	virtual int DisableStepCounter();
	virtual int EnableStepCounter();
	virtual int DisableHeartRate();
	virtual int EnableHeartRate();
	virtual int DisableMotion();
	virtual int EnableMotion();
private:
	void* OpenLibrary();
	void LoadSymbols();
	void* LoadSymbol(const char *symbol_string);
	int CloseLibrary(void* lib_mcutool);
	void* mcutool_handle;

	typedef struct nativeFunctions_t {
		int (*autoLowPowerScreen)(int32_t, int32_t, int32_t enable);
		int (*bandMode)();
		int (*cutOffScreen)();
		int (*enableHeartRate)(int32_t, int32_t, int32_t enable);
		int (*enableLowPowerScreen)(int32_t, int32_t, int32_t enable);
		int (*enableMotion)(int32_t, int32_t, int32_t enable);
		int (*enableStepCounter)(int32_t, int32_t, int32_t enable);
		int (*getBandModeData)();
		int (*getDataVersion)();
		int (*syncSteps)(int32_t, int32_t, int32_t steps);
		int (*syncTime)();
		int (*updateFitnessState)(int32_t, int32_t, int32_t type, int32_t state, int32_t heartRate, int64_t duration, int32_t calorie, float distance, int32_t speed, int32_t gpsStatus, uint8_t isKilometer);
		int (*wipeBandModeData)();
	} nativeFunctions_t;

	nativeFunctions_t nativeFunctions;
};

#endif //ASTEROIDOS_CATFISH_TOOLS_H
