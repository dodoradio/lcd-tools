#include "catfish-tools.h"
#include "koi-tools.h"

#include <CLI/CLI.hpp>

#include <QSettings>

const char* CONFIG_FILE = "/etc/asteroid/machine.conf";

using namespace AsteroidOS::LCD_Tools;

int main( int argc, char** argv ) {

	QSettings m_settings(CONFIG_FILE, QSettings::IniFormat);
	const QString machineCodename = m_settings.value("Identity/MACHINE", "unknown").toString();

	CLI::App app{"lcd-tools: Various controls for watches with a secondary displays"};

	bool syncTime = false;

	int koiDisplayColor = -1;
	bool koiTimepiece = false;

	app.add_flag("--sync-time",syncTime,"Sync lcd time with linux time");

	if (machineCodename == "koi") {
		app.add_option("--set-display",koiDisplayColor,"(Koi) set display color");
		app.add_flag("--prepare-timepiece",koiTimepiece,"(Koi) prepare watch for power off into timekeeping mode. You will then need to shut it down manually");
	}

	try{
		app.parse(argc,argv);
		if (argc >= 2) {
			if (machineCodename == "catfish") {
				if (syncTime) {
					CatfishSyncTime();
				}
			} else if (machineCodename == "koi") {
				if (syncTime) {
					KoiSyncTime();
				}
				if (koiDisplayColor != -1) {
					KoiSetDisplayColor(koiDisplayColor);
				}
				if (koiTimepiece) {
					KoiPrepareTimepiece();
				}
			}
		} else
			throw CLI::CallForAllHelp();
	} catch (const CLI::ParseError &e){
		return app.exit(e);
	}
	return 0;
}
