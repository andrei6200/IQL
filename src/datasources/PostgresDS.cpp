/* 
 * File:   PostgresDS.cpp
 * Author: andrei
 * 
 * Created on August 4, 2010, 2:13 PM
 */

#include "PostgresDS.hpp"
#include <pqxx/pqxx>
#include "utils/HqlTable.hpp"

using namespace std;
using namespace pqxx;

PostgresDS::PostgresDS() : conn(NULL), tr(NULL),
        options(POSTGRES_CONN_OPTS)
{
}

PostgresDS::PostgresDS(string opts) : conn(NULL), tr(NULL),
        options(opts)
{
}

PostgresDS::~PostgresDS()
{
    disconnect();
    TRACE << "Postgres DataSource was now destroyed.";
}

bool PostgresDS::isConnected()
{
    bool result = false;
    if (conn)
        result = conn->is_open();
    return result;
}

void PostgresDS::connect()
{
    if (isConnected())
    {
        WARN << "PostgresDS: Already connected. ";
        return;
    }

    try
    {
        /* Init PG database connections */
        TRACE << "PostgresDS: Connecting with options '" <<
                options << "' ...";
        conn = new connection(options);
        TRACE << "PostgresDS: Successfully connected to Postgres !";
        TRACE << "PostgresDS: Opening a new transaction ...";
        tr = new nontransaction(*conn, string("Hql-Client-Conn"));
        TRACE << "PostgresDS: Transaction open.";

        DEBUG << "Connected to SQL database: " << conn->dbname();
        TRACE << "SQL Backend version: " << conn->server_version();
        TRACE << "Protocol version: " << conn->protocol_version();
    }
    catch (broken_connection e)
    {
        FATAL << "PostgresDS: Could not connect to Postgresql : " << e.what();
        throw;
    }
}

HqlTable* PostgresDS::query(string queryString)
{
    connect();

    TRACE;
    TRACE << "Executing SQL query: " << queryString;

    // Perform a query on the database, storing result tuples in R.
    result R(tr->exec(queryString));

    // We're expecting to find some tuples...
    if (R.empty())
    {
        WARN << "No rows in query result. ";
        return new HqlTable();
    }

    HqlTable *table = new HqlTable();
    table->importFromSql(R);
    return table;
}

vector<string> PostgresDS::getObjectNames()
{
    string queryStr = "SELECT tablename FROM pg_tables where tablename "
            "not like 'pg_%' and tablename not like 'sql_%';";

    HqlTable *table = this->query(queryStr);
    
    return table->getColumn(0);
}


void PostgresDS::disconnect()
{
    if (tr)
    {
        tr->abort();
        delete tr;
        tr = NULL;
    }
    if (conn)
    {
        conn->disconnect();
        delete conn;
        conn = NULL;
    }
}

// FIXME: implement
void PostgresDS::insertData()
{

}
