/* 
 * File:   PostgresDS.hpp
 * Author: andrei
 *
 * Created on August 4, 2010, 2:13 PM
 */

#ifndef POSTGRESDS_HPP
#define	POSTGRESDS_HPP

#include "DataSourceInterface.hpp"
#include <pqxx/pqxx>


class PostgresDS : public DataSourceInterface
{
public:
    // default constructor
    PostgresDS();

    // constructor with connection options
    PostgresDS(std::string opts);

    // destructor
    ~PostgresDS();

    // Connects to the data source
    void connect();

    // Returns the connection status
    bool isConnected();

    // Public interface for commit current transaction
    void commit();

    // Returns the list of available object names (tables/collections)
    std::vector<std::string> getObjectNames();

    // Breaks the connection
    void disconnect();

    // Execute a query on the current data source
    HqlTable* query(std::string q);

    // Execute a query and return a pqxx result object
    pqxx::result regularQuery(std::string queryString);

    // Insert some data in a temporary table
    void insertData(HqlTable* table, std::string tableName);

    // Remember the name of a temporary table
    void addTempTable(std::string name);

    // Drop tables that have been created during a query execution
    void removeTempTables();

private:

    // Open transaction
    void openTa();

    // Abort transaction
    void abortTa();

    // Commit transaction
    void commitTa();

    // Open connection to base DB
    void openConn();

    // Close connection to base DB
    void closeConn();

    
    // Connection options
    std::string options;

    // Connection
    pqxx::connection_base *conn;

    // Transaction
    pqxx::work *tr;

    // List of temporary table names
    std::vector<std::string> tempTables;
};

#endif	/* POSTGRESDS_HPP */
