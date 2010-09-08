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

/** Rasdaman datasource adapter. */
class RasdamanDS : public DataSourceInterface
{
public:
    RasdamanDS();

    ~RasdamanDS();

    std::string getId();

    void connect();

    virtual bool isConnected();

    std::vector<std::string> getObjectNames();

    void disconnect();

    void commitTa();
    void abortTa();
    void openTa();

    
    IqlTable* query(std::string query);
    IqlTable* query(std::string query, bool storeOnDisk);
    r_Ref<r_GMarray> queryByOid(std::string);

    /** Execute an update/insert query on the current datasource */
    void updateQuery(std::string query);

    /** Save an array to disk */
    std::string saveRasdamanMddToFile(r_Ref<r_GMarray> mdd, bool storeOnDisk, int index);

    /** Retrieve a collection by its name */
    IqlTable* getCollection(std::string name, bool storeOnDisk = false, bool updateCols = false);

    void insertData(IqlTable* table, std::string tableName);

    void addTempTable(std::string name);

    void removeTempTables();
    
private:
    r_Database *db;
    r_Transaction *tr;

    std::map<std::string, bool> tempTables;
};

#endif	/* RASDAMANDS_HPP */

