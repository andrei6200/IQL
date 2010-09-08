/* 
 * File:   DataSourceInterface.hpp
 * Author: andrei
 *
 * Created on August 4, 2010, 2:00 PM
 *
 * Interface for data sources.
 */

#ifndef DATASOURCEINTERFACE_HPP
#define	DATASOURCEINTERFACE_HPP

#include <string>
#include "IqlTable.hpp"

/** Interface for the Datasources. */
class DataSourceInterface
{
public:
    /** Public constructor */
    DataSourceInterface();

    /** Public destructor */
    virtual ~DataSourceInterface();

    /** Connects to the data source */
    virtual void connect() = 0;

    /** Returns the connection status */
    virtual bool isConnected() = 0;

    /** Returns the list of available object names (tables/collections) */
    virtual vector<string> getObjectNames() = 0;

    /** Breaks the connection */
    virtual void disconnect() = 0;

    /** Execute a query on the current data source */
    virtual IqlTable* query(std::string) = 0;

    /** Insert some data in a new table*/
    virtual void insertData(IqlTable* table, std::string tableName) = 0;

    /** Mark a table as temporary - to delete after query execution */
    void addTempTable(std::string name);

    /** Drop tables that have been created during a query execution */
    virtual void removeTempTables() = 0;
};

#endif	/* DATASOURCEINTERFACE_HPP */

