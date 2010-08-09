/* Rasdaman includes and defines */
#define DEBUG_MAIN

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#ifdef __GNUG__
#include "raslib/template_inst.hh"
#endif
#endif
#include "rasdaman.hh"

#include <iostream>
#include "logger.hpp"
#include "config.hpp"
#include "RasdamanDS.hpp"

using namespace std;

int main()
{
	cout << "Test program : Hello, everyone. " << endl;
	RasdamanDS *rman = NULL;
	try
	{	
		rman = new RasdamanDS();
		rman->connect();
		TRACE << "Connected to rasdaman !!!" << endl;
	}
	catch (...)
	{
		FATAL << "Caught fatal error." << endl;
	}
	delete rman;
	rman = NULL;

	return 0;
}
