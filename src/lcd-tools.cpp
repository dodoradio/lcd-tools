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
		std::basic_string medakaSetDisplayString = "";
		float medakaStringScrollTime = 0.8;
		int medakaStringSet1 = 0;
		bool medakaStringSet2 = 0;
		int medakaIconNumber;
		int medakaIcon2;
		bool medakaIcon3;
		float medakaCompassValue = 0;
		std::basic_string medakaSetDisplayNumber = "";
		int medakaSetDisplayNumberPunct = 0;
		int medaka2LayerSetTop = 0;
               int medaka2LayerSetBottom = 0;

		app.add_flag("--sync-time",SyncTime,"Sync lcd time with linux time");
		app.add_flag("--white-background",[](int){SetDisplayColor(true);},"(Medaka) set display background to white");
		app.add_flag("--black-background",[](int){SetDisplayColor(false);},"(Medaka) set display background to black");
		app.add_flag("--session-restart",SyncSettings,"Initialise LCD for user session restart");

        app.add_flag("--prepare-timepiece",PrepareTimepiece,"(Medaka) prepare watch for power off into timekeeping mode. You will then need to shut it down manually");

        CLI::Option* medakaStringSet = app.add_option("--display-string",medakaSetDisplayString,"scroll a string on dot matrix");
        app.add_option("--string-set1",medakaStringSet1,"additional string setting 2");
        app.add_option("--string-set2",medakaStringSet2,"additional string setting 1");
        //CLI::Option* medakaIconSet = app.add_option("--display-icon",medakaIconNumber,"1: sunrise 2: sunset 3: hitide 4: lotide 5: battery 6: ERR");
        CLI::Option* medakaCompassSet = app.add_option("--display-compass", medakaCompassValue,"puts display into compass mode. float indicates a compass offset.");
        CLI::Option* medakaNumberSet = app.add_option("--display-number",medakaSetDisplayNumber,"puts a number in the main display"); //up to a 5 digit number. spaces are supported. will fill the 4 large segments first, then spill 5th digit into smaller segment
        CLI::Option* medakaNumberpunctSet = app.add_option("--display-number-punctuation",medakaSetDisplayNumberPunct,"sets additional markers for the numeric display"); //this is encoded bitwise - 1: colon 2: point after 2nd digit 4: point after 4th digit 8: apostrophe after 2nd digit

        CLI::Option* medaka2LayerSet = app.add_option("--2layer-set-top",medaka2LayerSetTop,"sets additional markers for the numeric display"); //this should set the top panel state in normal timekeeping mode
        app.add_option("--2layer-set-bottom",medaka2LayerSetBottom,"sets bottom display mode"); //this should set the bottom panel state in normal timekeeping mode

        CLI::Option* medakaIconSet = app.add_option("--display-icon",medakaIconNumber,"sets additional markers for the numeric display");
        app.add_option("--icon2",medakaIcon2,"sets bottom display mode");
        app.add_option("--icon3",medakaIcon3,"sets bottom display mode");

        app.add_flag_callback("--display-pixeltest",TestDisplay,"runs a pixel over the display");
        app.add_flag_callback("--display-graph",SetGraphDisplay,"draws a squashed 'ASTEROIDOS' to the dot matrix display");
        app.add_flag_callback("--display-reset",DisplayReset,"turn off the top display");

		CLI11_PARSE(app, argc, argv);

        if (* medakaStringSet) {
			SetDisplayString(medakaSetDisplayString.c_str(),medakaSetDisplayString.length(),medakaStringSet1,medakaStringSet2);
        }
        if (* medakaIconSet) {
			SetIconDisplay(medakaIconNumber,medakaIcon2,medakaIcon3);
        }
        if (* medakaCompassSet) {
			SetCompassDisplay(medakaCompassValue);
        }
        if (* medakaNumberSet) {
			SetNumberDisplay(medakaSetDisplayNumber.c_str(),medakaSetDisplayNumber.length(),medakaSetDisplayNumberPunct);
        }
        if (* medaka2LayerSet) {
			Set2LayerMode(medaka2LayerSetTop,medaka2LayerSetBottom);
        }

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
