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
    tempTables = map<string, bool>();
}

PostgresDS::PostgresDS(string opts) : conn(NULL), tr(NULL),
        options(opts)
{
    tempTables = map<string, bool>();
}

PostgresDS::~PostgresDS()
{
    TRACE << "Destroying Postgres Data Source ...";
    /* Just in case there are temp tables still lying around ...*/
    removeTempTables();
    /* Remove pqxx connections to base DB. */
    disconnect();
    TRACE << "Postgres DataSource was now destroyed.";
}

void PostgresDS::removeTempTables()
{
    TRACE << "Removing " << tempTables.size() << " temporary tables";
    /* Drop created tables. */
    connect();
    map<string,bool>::iterator it;
    for (it = tempTables.begin(); it != tempTables.end(); it++)
        try
        {
            regularQuery("DROP TABLE " + it->first);
        }
        catch (sql_error &e)
        {
            // do not react on this error
        }
    commitTa();
    tempTables.clear();
}

void PostgresDS::abortTa()
{
    if (tr != NULL)
    {
        try
        {
            tr->abort();
        }
        catch (pqxx_exception &e)
        {
            WARN << e.base().what();
        }
        delete tr;
        tr = NULL;
        TRACE << "Aborted current transaction. ";
    }
    else
        WARN << "Could not abort transaction, no transaction active. ";
}

void PostgresDS::commitTa()
{
    if (tr != NULL)
    {
        try
        {
            tr->commit();
        }
        catch (pqxx::pqxx_exception &e)
        {
            WARN << e.base().what();
        }
        delete tr;
        tr = NULL;
        TRACE << "Commited current transaction. ";
    }
    else
        WARN << "Could not commit transaction, no transaction active. ";
}

void PostgresDS::openTa()
{
    commitTa();
    if (tr == NULL)
    {
        TRACE << "Opening a new transaction ...";
        tr = new work(*conn, string("Hql-Client-Conn"));
        TRACE << "Transaction open.";
    }
}

void PostgresDS::openConn()
{
    if ( conn == NULL || conn->is_open() == false )
    {
        /* Init PG database connections */
        TRACE << "Connecting with options '" <<
                options << "' ...";
        conn = new connection(options);
        TRACE << "Successfully connected to Postgres !";
    }
    else
        WARN << "Connection already functional.";
}

void PostgresDS::closeConn()
{
    if (conn)
    {
//        conn->disconnect();
        delete conn;
        conn = NULL;
        TRACE << "Connection closed.";
    }
}

bool PostgresDS::isConnected()
{
    map<int,string> o;
    o[0] = "broken";
    o[1] = "ok";

    bool result = false;
    
    if (conn)
        result = conn->is_open();
    TRACE << "Connection validity status: " << o[result];
    if (tr == NULL)
        result = false;
    TRACE << "Transaction validity status: " << o[result];
//    if (result)
//    {
//        try
//        {
//            pqxx::result R(tr->exec("SELECT * FROM pg_tables"));
//            // The pg_tables table should never be empty
//            if (R.empty())
//                result = false;
//            TRACE << "Test SELECT Statement executed ok.";
//        }
//        catch (...)
//        {
//            WARN << "Error while executing test SELECT statement. ";
//            result = false;
//        }
//    }
    TRACE << "Postgres Connection status: " << o[result];
    return result;
}

void PostgresDS::connect()
{
//    if (isConnected())
//    {
//        WARN << "Already connected. ";
//        return;
//    }

    try
    {
//        disconnect();
        openConn();
        openTa();

        DEBUG << "Connected to SQL database: " << conn->dbname();
        TRACE << "SQL Backend version: " << conn->server_version();
        TRACE << "Protocol version: " << conn->protocol_version();
    }
    catch (broken_connection &e)
    {
        FATAL << "PostgresDS: Could not connect to Postgresql : " << e.what();
        disconnect();
        throw;
    }
}

HqlTable* PostgresDS::query(string queryString)
{
    connect();
    result R = regularQuery(queryString);
    commitTa();
    HqlTable *result = new HqlTable();

    if (R.empty())
        WARN << "No rows in query result. ";
    /* Only import data in memory if table does not contain an "INTO" */
    if (queryString.find("INTO") == string::npos)
        result->importFromSql(R);
    
    return result;
}

result PostgresDS::regularQuery(string queryString)
{
    DEBUG << "SQL Query: " << queryString;

    // Perform a query on the database, storing result tuples in R.
    result R;
    try
    {
        TRACE << "Executing query ...";
        R = tr->exec(queryString);
        TRACE << "Query execution ended.";
    }
    catch (sql_error &e)
    {
        ERROR << e.query() << " : " << e.what();
        abortTa();
        throw;
    }
    catch (pqxx_exception &e)
    {
        ERROR << "General libpqxx exception: " << e.base().what();
        abortTa();
        throw;
    }

    return R;
}

vector<string> PostgresDS::getObjectNames()
{
    string queryStr = "SELECT tablename FROM pg_tables where tablename "
            "not like 'pg_%' and tablename not like 'sql_%';";

    HqlTable *table = this->query(queryStr);
    TRACE << table << endl;
    
    return table->getColumn("tablename");
}

void PostgresDS::disconnect()
{
    commitTa();
    closeConn();
}

void PostgresDS::insertData(HqlTable* table, string tableName)
{
    TRACE << "Insert into SQL table '" << tableName << "': ";
    TRACE << table << endl;

    /* (1) First drop old table, if it exists. */
    connect();
    try
    {
        tr->exec("DROP TABLE " + tableName);
        TRACE << "Dropped table" << tableName;
    }
    catch (pqxx_exception &e)
    {
        // Query may fail because table may not exist.
        // If so, libpqxx automatically aborts the transaction. We also need to reset it.
        abortTa();
    }

    /* (2) Create the table, with an appropriate structure. */

    string query = "\nCREATE TABLE " + tableName + " (\n";
    // The first column always exists : it stores the HQL ID.
    query += "\t" + table->names[0] + " VARCHAR \n";
    for (int i = 1; i < table->names.size(); i++)
        query += string(", \n\t") + table->names[i] + " VARCHAR \n";
    query += ")";

    connect();
    try
    {
        tr->exec(query);
        TRACE << "Created table " << tableName;
    }
    catch (pqxx_exception &e)
    {
        ERROR << e.base().what();
        abortTa();
    }
    
    /* (3) Insert the data into the table. */
    connect();
    try
    {
        tablewriter W(*tr, tableName);
        for (int row = 0; row < table->data.size(); row++)
            W << table->data.at(row);

        W.complete();
        TRACE << "Inserted data into the table: " << tableName;
    }
    catch (pqxx_exception &e)
    {
        ERROR << e.base().what();
        abortTa();
        throw;
    }

    /* (4) Commit data to db. */
    commitTa();

    // store the table for subequent deletion
    tempTables[tableName] = true;
}

void PostgresDS::addTempTable(string name)
{
    tempTables[name] = true;
    TRACE << "Temporary tables: " << tempTables.size();
    map<string,bool>::iterator it;
    for (it = tempTables.begin(); it != tempTables.end(); it ++)
        TRACE << " Collection " << it->first;
    TRACE;
}

void PostgresDS::commit()
{
    commitTa();
}

void PostgresDS::insertHqlIdToTable(string table)
{
    string q = "CREATE SEQUENCE hqlid; \n";
    q += "ALTER TABLE " + table + " ADD COLUMN " + HQL_COL + " integer; \n";
    q += "UPDATE " + table + " SET " + HQL_COL + " = nextval('hqlid'); \n";
    q += "DROP SEQUENCE hqlid; ";
    HqlTable *result = this->query(q);
    delete result;
}