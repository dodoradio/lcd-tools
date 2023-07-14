#include "catfish-tools.h"
#include "koi-tools.h"
#include "medaka-tools.h"

#include <CLI/CLI.hpp>

#include <QSettings>

const char* CONFIG_FILE = "/etc/asteroid/machine.conf";

int main( int argc, char** argv ) {

	QSettings m_settings(CONFIG_FILE, QSettings::IniFormat);
	const QString machineCodename = m_settings.value("Identity/MACHINE", "unknown").toString();

	CLI::App app{"lcd-tools: Various controls for watches with a secondary display"};
	if (machineCodename == "koi") {
		using namespace AsteroidOS::LCD_Tools::Koi;
		app.add_flag("--sync-time",SyncTime,"Sync lcd time with linux time");
		app.add_flag("--white-background",[](int){SetDisplayColor(true);},"(Koi) set display background to white");
		app.add_flag("--black-background",[](int){SetDisplayColor(false);},"(Koi) set display background to black");
		app.add_flag("--prepare-timepiece",PrepareTimepiece,"(Koi) prepare watch for power off into timekeeping mode. You will then need to shut it down manually");
		app.add_flag("--session-restart",SyncSettings,"Initialise LCD for user session restart");
		CLI11_PARSE(app, argc, argv);
	}
	if (machineCodename == "medaka") {
		using namespace AsteroidOS::LCD_Tools::Medaka;
		app.add_flag("--sync-time",SyncTime,"Sync lcd time with linux time");
		app.add_flag("--white-background",[](int){SetDisplayColor(true);},"(Medaka) set display background to white");
		app.add_flag("--black-background",[](int){SetDisplayColor(false);},"(Medaka) set display background to black");
		app.add_flag("--session-restart",SyncSettings,"Initialise LCD for user session restart");
		CLI11_PARSE(app, argc, argv);
	}
	else if (machineCodename == "catfish") {
		CatfishLcdTools catfish;
		app.add_flag("--sync-time",[&catfish](int){ catfish.SyncTime(); },"Sync lcd time with linux time");
		app.add_flag("--prepare-timepiece",[&catfish](int){ catfish.PrepareTimepiece(); },"(Catfish) Start timepiece only mode");
		app.add_flag("--enable-stepcounter",[&catfish](int){ catfish.EnableStepCounter(); },"(Catfish) Enable the step counter");
		app.add_flag("--disable-stepcounter",[&catfish](int){ catfish.DisableStepCounter(); },"(Catfish) Disable the step counter");
		app.add_flag("--enable-heartrate",[&catfish](int){ catfish.EnableHeartRate(); },"(Catfish) Enable the heart rate sensor");
		app.add_flag("--disable-heartrate",[&catfish](int){ catfish.DisableHeartRate(); },"(Catfish) Disable the heart rate sensor");
		app.add_flag("--enable-motion",[&catfish](int){ catfish.EnableMotion(); },"(Catfish) Enable motion");
		app.add_flag("--disable-motion",[&catfish](int){ catfish.DisableMotion(); },"(Catfish) Disable motion");
		app.add_flag("--session-restart","Initialise LCD for user session restart"); // this isn't used on catfish, but the service to error out if this option isn't present.
		CLI11_PARSE(app, argc, argv);
	}

	return 0;
}
