#include "catfish-tools.h"
#include "koi-tools.h"

#include <CLI/CLI.hpp>

#include <QSettings>

const char* CONFIG_FILE = "/etc/asteroid/machine.conf";


int main( int argc, char** argv ) {

	QSettings m_settings(CONFIG_FILE, QSettings::IniFormat);
	const QString machineCodename = m_settings.value("Identity/MACHINE", "unknown").toString();

	CLI::App app{"lcd-tools: Various controls for watches with a secondary displays"};
	if (machineCodename == "koi") {
		using namespace AsteroidOS::LCD_Tools::Koi;
		app.add_flag("--sync-time",SyncTime,"Sync lcd time with linux time");
		app.add_flag("--white-background",[](int){SetDisplayColor(true);},"(Koi) set display background to white");
		app.add_flag("--black-background",[](int){SetDisplayColor(false);},"(Koi) set display background to black");
		app.add_flag("--prepare-timepiece",PrepareTimepiece,"(Koi) prepare watch for power off into timekeeping mode. You will then need to shut it down manually");
	} 
	else if (machineCodename == "catfish") {
		using namespace AsteroidOS::LCD_Tools::Catfish;
		app.add_flag("--sync-time",SyncTime,"Sync lcd time with linux time");
	}

	CLI11_PARSE(app, argc, argv);
	return 0;
}
