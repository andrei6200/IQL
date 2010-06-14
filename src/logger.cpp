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

#include "logger.hpp"

/******************* CONFIGURATION START **********************/

/* Output file */
ofstream _outputFile("program.log", ofstream::app);
//ofstream _outputFile(cout);

/* Global LOG */
LOGGER <char, char_traits < char > > LOG(_outputFile);

/******************* CONFIGURATION END **********************/
