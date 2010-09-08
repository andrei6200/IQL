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
#include <map>

/** Datasource adapter for PostgreSQL. */
class PostgresDS : public DataSourceInterface
{
public:
    PostgresDS();

    PostgresDS(std::string opts);

    ~PostgresDS();

    void connect();

    bool isConnected();

    void commit();

    std::vector<std::string> getObjectNames();

    void disconnect();

    HqlTable* query(std::string q);

    pqxx::result regularQuery(std::string queryString);

    void insertData(HqlTable* table, std::string tableName);

    void addTempTable(std::string name);

    void removeTempTables();

    /** Insert an HQL id column to the named table */
    void insertHqlIdToTable(std::string table);

private:

    /** Open transaction */
    void openTa();

    /** Abort transaction */
    void abortTa();

    /** Commit transaction */
    void commitTa();

    /** Open connection to base DB */
    void openConn();

    /** Close connection to base DB */
    void closeConn();

    
    /** Connection options */
    std::string options;

    /** Connection */
    pqxx::connection_base *conn;

    /** Transaction */
    pqxx::work *tr;

    /** List of temporary table names */
    std::map<std::string, bool> tempTables;
};

#endif	/* POSTGRESDS_HPP */
