/*
    Copyright Quentin Geissmann 2012-2013

    This file is part of OpenCFU

    OpenCFU is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenCFU is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with OpenCFU.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "defines.hpp"
#include "ArgumentParser.hpp"
#include "ProcessingOptions.hpp"
#include "Processor.hpp"

 /*Only if compiled with Gtk support*/
#ifdef WITH_GUI
	#include "Gui_MyApplication.hpp"
#endif

int main(int argc, char **argv){

    ProcessingOptions opts;
    Processor processor(opts);
    DEV_INFOS("Starting OpenCFU");
    if(argc > 1){
        DEV_INFOS("At least one argument -> parsing arguments...");
        ArgumentParser par(argc, argv,opts);
        DEV_INFOS("Argument parsed OK. Now processing");
        processor.runAll();
        processor.writeResult();
    }
    else{
    #ifndef WITH_GUI
        std::cerr<<"If you use no argument, OpenCFU will run as a GUI, but the program was not compiled with GTK support. Run 'opencfu -h' for more information"<<std::endl;
        return EXIT_FAILURE;
    #else
        DEV_INFOS("No argument used, running the GUI...");
        Glib::thread_init();
        Glib::set_application_name (PROGRAM_NAME);
        Gui_MyApplication myApp(argc,argv,processor);
    #endif
    }

    return EXIT_SUCCESS;
}
