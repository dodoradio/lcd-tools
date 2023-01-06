#include "catfish-tools.h"

#include <iostream>
#include <dlfcn.h>
#include <hybris/common/dlfcn.h>
#include <hybris/properties/properties.h>
#include <MGConfItem>

namespace AsteroidOS::LCD_Tools::Catfish {
void SyncTime(int) {
	auto lib_mcutool = hybris_dlopen("libmcutool.so", RTLD_LAZY);
	if (!lib_mcutool) {
		std::cerr << "Unable to load libmcutool.so" << std::endl;
		return;
	}

	auto use12h = new MGConfItem("/org/asteroidos/settings/use-12h-format");

	if (use12h->value(false).toBool()) {
		property_set("persist.sys.time_12_24", "12");
	} else {
		property_set("persist.sys.time_12_24", "24");
	}

	auto syncTime = (int (*)())
			hybris_dlsym(lib_mcutool, "Java_com_mobvoi_ticwear_mcuservice_CoreService_nativeSyncTime");
	if (!syncTime) {
		std::cerr << "Unable to get symbol" << std::endl;
		hybris_dlclose(lib_mcutool);
		return;
	}
	auto res = syncTime();
	if (hybris_dlclose(lib_mcutool)) {
		std::cerr << "Failed to safely close the library" << std::endl;
		return;
	}
}
}
