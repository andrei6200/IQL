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

    // Returns the list of available object names (tables/collections)
    std::vector<std::string> getObjectNames();

    // Breaks the connection
    void disconnect();

    // Execute a query on the current data source
    HqlTable* query(std::string q);

    // FIXME: Insert some data in a temporary table
    void insertData();

private:
    // Connection options
    std::string options;

    // Connection
    pqxx::connection_base *conn;

    // Transaction
    pqxx::transaction_base *tr;
};

#endif	/* POSTGRESDS_HPP */
