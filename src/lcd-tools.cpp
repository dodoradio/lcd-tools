#include "lcd-tools.h"

#include <CLI/CLI.hpp>

using namespace AsteroidOS::LCD_Tools;

int main( int argc, char** argv ) {
	CLI::App app{"lcd-tools: Various controls for the TicWatch LCD"};

	app.add_flag_callback("--sync-time",SyncTime,"Sync lcd time with linux time");

	try{
		app.parse(argc,argv);
		if (argc <= 1)
			throw CLI::CallForAllHelp();
	} catch (const CLI::ParseError &e){
		return app.exit(e);
	}
	return 0;
}
