/* 
 * File:   HqlMain.cpp
 * Author: andrei
 * 
 * Created on June 12, 2010, 5:51 PM
 *
 * Main class for the Hybrid Query Language Framework.
 * This is the entry point for actions of the HQL grammar.
 * Implements the singleton pattern.
 */

/* PostgreSQL includes and defines */
#include <pqxx/pqxx>

/* Rasdaman includes and defines */
#define DEBUG_MAIN
#include "debug/debug.hh"

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#ifdef __GNUG__
#include "raslib/template_inst.hh"
#endif
#endif
#include "rasdaman.hh"


/* Header file of this class */
#include "HqlMain.hpp"
#include "logger.hpp"
#include "grammar/structures.hpp"
#include "config.hpp"


using namespace pqxx;
using namespace std;


HqlMain HqlMain::instance;
connection_base* HqlMain::pg_conn = NULL;

/* Return (and initialize if needed) the singleton instance of HqlMain. */
HqlMain& HqlMain::getInstance()
{
    return instance;
}

/* Private constructor.
 * Queries the postgres and rasdaman databases for available tables.
 */
HqlMain::HqlMain()
{
    INFO << "Initialization of HqlMain ... ";
    /* Initialization */

    /* Init database connections */
    pg_conn = new lazyconnection();

    /* Rasdaman tables */
    set<string> rasTables = getRasdamanCoverages();
    set<string>::iterator iter;
    DEBUG << "Found " << rasTables.size() << " Rasdaman tables. ";

    /* Postgres tables */
    set<string> pgTables = getPostgresTables();
    DEBUG << "Found " << pgTables.size() << " Postgres tables. ";

    /* Create the table dictionary */
    DbEnum rql = RASDAMAN;
    DbEnum sql = POSTGRES;
    for (iter = rasTables.begin(); iter != rasTables.end(); iter++)
        tableMap[*iter] = RASDAMAN;
    for (iter = pgTables.begin(); iter != pgTables.end(); iter++)
        if (tableMap.count(*iter) == 0)
            tableMap[*iter] = POSTGRES;
        else
        {
            FATAL << "\n ERROR ! Table " << *iter << " is a coverage name in Rasdaman AND "
                    << " a table in Postgres. Cannot continue. ";
            cerr << "\n ERROR ! Table " << *iter << " is a coverage name in Rasdaman AND "
                    << " a table in Postgres. Cannot continue. ";
            exit(1);
        }

    /* Print the Table dictionary */
    map<string, int>::const_iterator tuple;
    TRACE;
    TRACE << "Name dictionary : ";
    for (tuple = tableMap.begin(); tuple != tableMap.end(); tuple++)
        TRACE << " - " << (*tuple).first << " -> " << (*tuple).second;
    TRACE;

    INFO << " Initialization complete. ";
}

/* Public destructor */
HqlMain::~HqlMain()
{
    TRACE << "Destroying Singleton instance of HqlMain.";

    if (pg_conn != NULL)
    {
        TRACE << "Deleting the Postgres connection ..." << flush;
        delete pg_conn;
        pg_conn = NULL;
    }

    TRACE << "HqlMain fields have been destroyed.";
}

/*
 * This function receives the a query string from the parser. It is responsable
 * for the execution of the query, and for the delivery of the results to the
 * user.
 * Returns the status as a string.
 */
string HqlMain::executeHqlQuery(const char* msg)
{
    INFO << "Received HQL query (string): " << msg;

    INFO << "Finished execution of HQL query string: " << msg << endl;

    return string("   ok ...");
}

/*
 * This function receives a SelectStruct structure from the parser, and is
 * responsable for the query execution and result delivery.
 */
void HqlMain::executeHqlQuery(selectStruct *select)
{
    INFO << "Received SELECT structure. ";
    string status;

    /* Debugging output */
    list<string>::const_iterator it;
    for (it = select->what->begin(); it != select->what->end(); it++)
        DEBUG << " - SELECT " << *it;
    for (it = select->from->begin(); it != select->from->end(); it++)
        DEBUG << " - FROM " << *it;

    /* Find out what data sources are involved in this query: PG or RMAN ? */
    TRACE;
    int rman = 0;
    int pg = 0;
    bool error = false;
    for (it = select->from->begin(); it != select->from->end(); it++)
        if (tableMap.count(*it) == 1)
        {
            if (tableMap[*it] == RASDAMAN)
                rman++;
            else
                pg++;
        }
        else
        {
            ERROR << "Table '" << *it << "' does not exist in Postgres nor in Rasdaman !";
            INFO << "Skipping this query...";
            status = "non-existing table ('" + *it + "') ...";
            error = true;
        }
    DEBUG << " Found " << rman << " Rasdaman tables in query.";
    DEBUG << " Found " << pg << " Postgres tables in query.";

    if (error == false)
    {
        /* FIXME: Currently I only execute only RMAN or only PG queries. */
        if (rman == 0)
        {
            cout << RESPONSE_PROMPT << "Executing as Postgres query... " << endl;
            try
            {
                DEBUG << "Executing SQL query ...";
                set<string> sqlTable = runSqlQuery(*pg_conn, select->query, 0);
                printSqlTable(sqlTable);
                status = "ok";
            }
            catch (exception &e)
            {
                ERROR << "Query execution error: " << e.what();
                status = string("failed... ") + e.what();
            }
        }
        if (pg == 0)
        {
            cout << RESPONSE_PROMPT << "Executing as Rasdaman query... " << endl;
            try
            {
                DEBUG << "Executing RaSQL query ...";
                runRasqlQuery(NULL, NULL, select->query);
                status = "ok";
            }
            catch (r_Error &e)
            {
                ERROR << "Query execution error: " << e.what();
                status = string("failed... ") + e.what();
            }
        }
    }

    //    /* Dummy method call. */
    //    status = this->executeHqlQuery(select->query);

    cout << RESPONSE_PROMPT << status << endl;

    cout << QUERY_PROMPT;
}


/* Prints an SQL table in a human-readable form on stdout. */
void HqlMain::printSqlTable(set<string> sqlTable)
{
    set<string>::iterator it;
    cout << "SQL Query results (" << sqlTable.size() << " rows):" << endl;
    for (it = sqlTable.begin(); it != sqlTable.end(); it ++)
        cout << " * " << *it << endl;
    cout << endl;
}


/* Returns the available Rasdaman collections as a set of strings. */
set<string> HqlMain::getRasdamanCoverages()
{
    const char query[] = "select mddcollname from RAS_MDDCOLLNAMES;";

    /* Connect to the default rasdaman backend dabatase. */
    connection C("dbname=RASBASE");

    set<string> tableSet = getInstance().runSqlQuery(C, query, 0);

    return tableSet;
}

/* Returns the available Postgres tables as a set of strings. */
set<string> HqlMain::getPostgresTables()
{
    const char query[] = "SELECT tablename FROM pg_tables where tablename "
            "not like 'pg_%' and tablename not like 'sql_%';";

    /* Connect to the default SQL database.*/
    connection C;

    set<string> tableSet = getInstance().runSqlQuery(C, query, 0);

    return tableSet;
}

/* Execute a query on a predefined Postgres connection, and return the
 * results at column "outIndex" as a set of strings.
 * NOTE: Throws an std::exception in case of error.
 */
set<string> HqlMain::runSqlQuery(connection_base& C, const char* queryString, int outIndex)
{
    set<string> resultSet;

    DEBUG << "Connected to database: " << C.dbname();
    TRACE << "Backend version: " << C.server_version();
    TRACE << "Protocol version: " << C.protocol_version();

    TRACE;
    TRACE << "Executing SQL query: " << queryString;

    // Begin a transaction acting on our current connection.  Give it a human-
    // readable name so the library can include it in error messages.
    nontransaction T(C, "sqlQuery");

    // Perform a query on the database, storing result tuples in R.
    result R(T.exec(queryString));

    // We're expecting to find some tables...
    if (R.empty())
    {
        WARN << "No rows in query result. ";
        return resultSet;
    }

    // Process each successive result tuple
    for (result::const_iterator c = R.begin(); c != R.end(); ++c)
        resultSet.insert(c[outIndex].as(string()));

    T.abort();


    return resultSet;
}

/* Run a Rasql query on a given rasdaman database and transaction. 
 * If the database and/or transaction pointers are NULL, they are created and 
 * destroyed at the end of the function execution. 
 * NOTE: Throws an (r_Error) exception if something goes wrong.
 */
void HqlMain::runRasqlQuery(r_Database *db, r_Transaction *tr, const char* queryString)
{
    DEBUG << "Executing RaSQL query: " << queryString;
    r_OQL_Query query(queryString);

    /* Initialize the database connection and transaction */
    r_Database *database;
    r_Transaction *transaction;

    database = db;
    if (db == NULL)
    {
        TRACE << "Connecting to database ...";
        database = new r_Database();
        database->set_servername("localhost");
        database->set_useridentification("rasguest", "rasguest");
        database->open("RASBASE");
    }
    transaction = tr;
    if (tr == NULL)
    {
        TRACE << "Opening transaction ...";
        transaction = new r_Transaction();
        transaction->begin(r_Transaction::read_only);
    }


    r_Set< r_Ref_Any > result_set;
    r_Set< r_Ref< r_GMarray > > *image_set;
    r_Ref< r_GMarray > image;
    r_Iterator< r_Ref_Any > iter;

    /* Execute the actual query. */
    TRACE << "Executing RaSQL ...";
    r_oql_execute(query, result_set);
    TRACE << "RaSQL execution ended. ";
    DEBUG << "Result has " << result_set.cardinality() << " objects... ";

    iter = result_set.create_iterator();

    for (iter.reset(); iter.not_done(); iter++)
    {
        image = r_Ref<r_GMarray > (*iter);

        /* Print metadata ... taken from RaSQL */
        {
            cout << "  Oid...................: " << result_set.get_oid() << endl;
            cout << "  Type Structure........: "
                    << (result_set.get_type_structure() ? result_set.get_type_structure() : "<nn>") << endl;
            cout << "  Type Schema...........: " << flush;
            if (result_set.get_type_schema())
                result_set.get_type_schema()->print_status(cout);
            else
                cout << "(no name)" << flush;
            cout << endl;
            cout << "  Number of entries.....: " << result_set.cardinality() << endl;
            cout << "  Element Type Schema...: " << flush;
            if (result_set.get_element_type_schema())
                result_set.get_element_type_schema()->print_status(cout);
            else
                cout << "(no name)" << flush;
            cout << endl;
        }

        // work with the result image
        // for example print its spatial domain
        switch (result_set.get_element_type_schema()->type_id())
        {
        case r_Type::MARRAYTYPE:
            cout << image->spatial_domain() << endl;
            break;
        default:
            WARN << "\n\n\nThe output result is *NOT* a MDD. So I cannot print any spatial domain :-)";
            DEBUG << "Output type is: " << result_set.get_element_type_schema()->type_id() << endl;
            cout << "\n\n\nThe output result is *NOT* a MDD. So I cannot print any spatial domain :-)" << endl;

            break;
        }

    }

    /* Delete created objects, if necessary. */
    if (tr == NULL)
    {
        transaction->abort();
        delete transaction;
    }
    if (db == NULL)
    {
        database->close();
        delete database;
    }

    TRACE << "Finished executing RaSQL.";

}
