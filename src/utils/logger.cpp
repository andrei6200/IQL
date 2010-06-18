/*
 * File:   logging.cpp
 * Author: andrei
 *
 * Created on June 14, 2010, 1:28 PM
 */

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

#include "utils/logger.hpp"
#include "config.hpp"

/******************* CONFIGURATION START **********************/

/* Output file.
 *
 * You can redirect the log output to a standard stream (for example "cout")
 * like the commented line below.
 */
ofstream _outputFile(LOGFILE_NAME, ofstream::app);
//ofstream _outputFile(cout);


/* Global LOG */
LOGGER <char, char_traits < char > > LOG(_outputFile);

/******************* CONFIGURATION END **********************/
