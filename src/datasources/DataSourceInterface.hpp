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
#include "HqlTable.hpp"

class DataSourceInterface
{
public:
    // Public constructor
    DataSourceInterface();

    // Public destructor
    virtual ~DataSourceInterface();

    // Connects to the data source
    virtual void connect() = 0;

    // Returns the connection status
    virtual bool isConnected() = 0;

    // Returns the list of available object names (tables/collections)
    virtual vector<string> getObjectNames() = 0;

    // Breaks the connection
    virtual void disconnect() = 0;

    // Execute a query on the current data source
    virtual HqlTable* query(std::string) = 0;

    // FIXME: Insert some data in a temporary table
    virtual void insertData() = 0;
};

#endif	/* DATASOURCEINTERFACE_HPP */

