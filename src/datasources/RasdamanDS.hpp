/* 
 * File:   RasdamanDS.hpp
 * Author: andrei
 *
 * Created on August 4, 2010, 2:15 PM
 */

#ifndef RASDAMANDS_HPP
#define	RASDAMANDS_HPP


/* Rasdaman includes and defines */
#define DEBUG_MAIN

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#ifdef __GNUG__
#include "raslib/template_inst.hh"
#endif
#endif
#include "rasdaman.hh"

/* Other includes */
#include "DataSourceInterface.hpp"

class RasdamanDS : public DataSourceInterface
{
public:
    // constructor
    RasdamanDS();

    // destructor
    ~RasdamanDS();

    // Returns the data source ID
    std::string getId();

    // Connects to the data source
    void connect();

    // Returns the connection status
    virtual bool isConnected();

    // Returns the list of available object names (tables/collections)
    std::vector<std::string> getObjectNames();

    // Breaks the connection, aborting the active transaction.
    void disconnect();

    void commitTa();
    void abortTa();
    void openTa();

    // Execute a query on the current data source
    HqlTable* query(std::string query);

    // FIXME: Insert some data in a temporary table
    void insertData();
    
private:
    r_Database *db;
    r_Transaction *tr;
};

#endif	/* RASDAMANDS_HPP */

