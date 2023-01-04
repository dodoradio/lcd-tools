#include "catfish-tools.h"
#include "koi-tools.h"

#include <CLI/CLI.hpp>

using namespace AsteroidOS::LCD_Tools;

int main( int argc, char** argv ) {
	CLI::App app{"lcd-tools: Various controls for watches with a secondary displays"};

	bool machineCatfish = false;
	bool machineKoi = false;
	bool syncTime = false;
	int koiDisplayColor = -1;

	app.add_flag("--catfish",machineCatfish,"Ticwatch Pro mode");
	app.add_flag("--koi",machineKoi,"Casio WSD-F10/20 mode");
	app.add_flag("--sync-time",syncTime,"Sync lcd time with linux time");
	app.add_option("--set-display",koiDisplayColor,"(Koi) set display color");

	try{
		app.parse(argc,argv);
		if (machineCatfish) {
			if (syncTime) {
				CatfishSyncTime();
			}
		} else if (machineKoi) {
			if (syncTime) {
				KoiSyncTime();
			}
			if (koiDisplayColor != -1) {
				KoiSetDisplayColor(koiDisplayColor);
			}
		} else
			throw CLI::CallForAllHelp();
	} catch (const CLI::ParseError &e){
		return app.exit(e);
	}
	return 0;
}
