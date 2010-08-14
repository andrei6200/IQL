/* 
 * File:   PostgresDS.cpp
 * Author: andrei
 * 
 * Created on August 4, 2010, 2:13 PM
 */

#include "PostgresDS.hpp"
#include <pqxx/pqxx>
#include "HqlTable.hpp"

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
    TRACE << "Connection validity status: " << result;
    if (!tr)
        result = false;
    TRACE << "Transaction validity status: " << result;
    if (result)
    {
        try
        {
            TRACE << "Trying out test SQL statement to check transaction...";
            pqxx::result R(tr->exec("SELECT * FROM pg_tables"));
            if (R.empty())
                result = false;
            TRACE << "SELECT Statement executed ok.";
        }
        catch (...)
        {
            result = false;
        }
    }
    TRACE << "Postgres Connection status: " << result;
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
        disconnect();
        /* Init PG database connections */
        TRACE << "PostgresDS: Connecting with options '" <<
                options << "' ...";
        conn = new connection(options);
        TRACE << "PostgresDS: Successfully connected to Postgres !";
        TRACE << "PostgresDS: Opening a new transaction ...";
        tr = new work(*conn, string("Hql-Client-Conn"));
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
    
    return table->getColumn("tablename");
}


void PostgresDS::disconnect()
{
    if (tr)
    {
        tr->abort();
        delete tr;
        tr = NULL;
        TRACE << "Transaction aborted.";
    }
    if (conn)
    {
        conn->disconnect();
        delete conn;
        conn = NULL;
        TRACE << "Connection closed.";
    }
}


void PostgresDS::insertData(HqlTable* table, string tableName)
{
    connect();
    
    /* (1) First drop old table, if it exists.
    This will probably fail because the table shouldn't exist.  */
    try
    {
        tr->exec("DROP TABLE " + tableName);
        tr->commit();
        TRACE << "Dropped table " << tableName;
    }
    catch (pqxx::pqxx_exception &e)
    {
    }

    delete tr;
    tr = NULL;
    TRACE << "Destroyed old transaction";
    connect();

    /* (2) Create the table, with an appropriate structure. */

    string query = "CREATE TABLE " + tableName + " (";
    // The first column always exists : it stores the HQL ID.
    query += table->names[0] + " INTEGER";
    for (int i = 1; i < table->names.size(); i++)
        query += string(", ") + table->names[i] + " VARCHAR";
    query += ")";
    
    tr->exec(query, "Create Table " + tableName);
    TRACE << "Created new Table: " << tableName;
    
    /* (3) Insert the data into the table. */

    tablewriter W(*tr, tableName);
    for (int row = 0; row < table->data.size(); row++)
        W << table->data.at(row); 

    W.complete();
    TRACE << "Inserted data into the table: " << tableName;

}
