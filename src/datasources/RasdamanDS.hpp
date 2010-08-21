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

#include <map>

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

    // Execute a SELECT query on the current data source
    HqlTable* query(std::string query);
    HqlTable* query(std::string query, bool storeOnDisk);
    r_Ref<r_GMarray> queryByOid(std::string);

    // Execute an update/insert query on the current datasource
    void updateQuery(std::string query);

    // Save an array to disk
    std::string saveRasdamanMddToFile(r_Ref<r_GMarray> mdd, bool storeOnDisk, int index);

    // Retrieve a collection by its name
    HqlTable* getCollection(std::string name, bool storeOnDisk = false, bool updateCols = false);

    // Insert some data in a temporary table
    void insertData(HqlTable* table, std::string tableName);

    // Mark a table for deletion on program exit
    void addTempTable(std::string name);

    // Drop tables that have been created during a query execution
    void removeTempTables();
    
private:
    r_Database *db;
    r_Transaction *tr;

    std::map<std::string, bool> tempTables;
};

#endif	/* RASDAMANDS_HPP */

