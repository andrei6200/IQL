/*
 * File:   logging.cpp
 * Author: andrei
 *
 * Created on June 14, 2010, 1:28 PM
 */

#include "datasources/HqlTable.hpp"


#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

#include "logger.hpp"
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


string formatClassName(string prettyFunction)
{
    size_t index = prettyFunction.find( "(" );
    if ( index == string::npos )
        return prettyFunction;  /* Degenerate case */

    prettyFunction.erase( index );

    index = prettyFunction.rfind( " " );
    if ( index == string::npos )
        return prettyFunction;  /* Degenerate case */

    prettyFunction.erase( 0, index + 1 );

    /* The scoped class + function name. */
    return prettyFunction;
}

LOGGER<char, char_traits < char > >& operator <<
    (LOGGER<char, char_traits < char > > &o, HqlTable *a)
{
    a->print(o.os);
    return o;
}