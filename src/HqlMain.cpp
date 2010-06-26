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
#include "utils/logger.hpp"
#include "grammar/structures.hpp"
#include "config.hpp"
#include "utils/HqlTable.hpp"


using namespace pqxx;
using namespace std;


/*********************************/
/* Initialize logging */
#include "utils/logger.cpp"
/*********************************/



HqlMain HqlMain::instance;
connection_base* HqlMain::pg_conn = NULL;
r_Database* HqlMain::rman_db = NULL;

/* Define the output of Rasql globally, because of bug in r_Set<r_Ref_Any> destructor */
//r_Set< r_Ref_Any > globalRasqlResultSet;

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

    /* Init PG database connections */
    TRACE << "Connecting to Postgres database ...";
    pg_conn = new lazyconnection(POSTGRES_CONN_OPTS);
    TRACE << "Successfully connected to Postgres !";
    /* Init RMAN database connections */
    TRACE << "Connecting to Rasdaman host " << RASDAMAN_SERVER
            << " with port " << RASDAMAN_PORT
            << " using user/password " << RASDAMAN_USERNAME << "/"
            << RASDAMAN_PASSWORD
            << " and opening database " << RASDAMAN_DATABASE;
    rman_db = new r_Database();
    rman_db->set_servername(RASDAMAN_SERVER, RASDAMAN_PORT);
    rman_db->set_useridentification(RASDAMAN_USERNAME, RASDAMAN_PASSWORD);
    rman_db->open(RASDAMAN_DATABASE);

    /* Rasdaman tables */
    vector<string> rasTables = getRasdamanCoverages();
    vector<string>::iterator iter;
    DEBUG << "Found " << rasTables.size() << " Rasdaman tables. ";

    /* Postgres tables */
    vector<string> pgTables = getPostgresTables();
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
            FATAL << "\n ERROR ! Table '" << *iter << "' is a coverage name in Rasdaman AND "
                    << " a table in Postgres. Cannot continue. ";
            cerr << "\n ERROR ! Table '" << *iter << "' is a coverage name in Rasdaman AND "
                    << " a table in Postgres. Cannot continue. ";
            exit(1);
        }

    /* Print the Table dictionary */
    map<string, DbEnum>::const_iterator tuple;
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

    if (rman_db != NULL)
    {
        rman_db->close();
        TRACE << "Deleting the Rasdaman Database Connection ..." << flush;
        delete rman_db;
        rman_db = NULL;
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
    list<tableRefStruct*>::const_iterator it;
    for (it = select->what->begin(); it != select->what->end(); it++)
        DEBUG << " - SELECT " << (*it)->tname;
    for (it = select->from->begin(); it != select->from->end(); it++)
    {
        DEBUG << " - FROM " << (*it)->tname;
        if ((*it)->alias != NULL)
            DEBUG << "       AS " << *((*it)->alias);
    }

    /* Find out what data sources are involved in this query: PG or RMAN ? */
    TRACE;
    int rman = 0;
    int pg = 0;
    bool error = false;
    list<tableRefStruct*> *from = select->from;
    for (it = from->begin(); it != from->end(); it++)
    {
        string name = (*it)->tname;
        if (tableMap.count(name) == 1)
        {
            (*it)->source = tableMap[name];
            if (tableMap[name] == RASDAMAN)
                rman++;
            else
                pg++;
        }
        else
        {
            ERROR << "Table '" << name << "' does not exist in Postgres nor in Rasdaman !";
            INFO << "Skipping this query...";
            status = "non-existing table ('" + name + "') ...";
            error = true;
        }
    }
    DEBUG << " Found " << rman << " Rasdaman tables in query.";
    DEBUG << " Found " << pg << " Postgres tables in query.";

    if (error == false)
    {
        /* FIXME: Currently I only execute only RMAN or only PG queries. */
        if (rman == 0 && pg > 0)
        {
            /* The whole query is a SQL query... */
            cout << RESPONSE_PROMPT << "Executing as Postgres query... " << endl;
            try
            {
                DEBUG << "Executing SQL query ...";
                HqlTable table = runSqlQuery(*pg_conn, select->query);
                table.print(cout);
                status = "ok";
            }
            catch (exception &e)
            {
                ERROR << "Query execution error: " << e.what();
                status = string("failed... ") + e.what();
            }
        }
        if (pg == 0 && rman > 0)
        {
            /* The whole query is a RaSQL query ... */
            cout << RESPONSE_PROMPT << "Executing as Rasdaman query... " << endl;
            try
            {
                DEBUG << "Executing RaSQL query ...";
                HqlTable table = runRasqlQuery(rman_db, select->query);
                table.print(cout);
                status = "ok";
            }
            catch (r_Error &e)
            {
                ERROR << "Query execution error: " << e.what();
                status = string("failed... ") + e.what();
            }
        }

        /* Now we come to the more interesting case: RaSQL *and* SQL query. */
        if (rman > 0 && pg > 0)
        {
            cout << RESPONSE_PROMPT << "DELAYED ... Executing mixed queries "
                    "is not yet implemented ..." << endl;
            status = string("failed");
        }
    }

    //    /* Dummy method call. */
    //    status = this->executeHqlQuery(select->query);

//    try
//    {
//        DEBUG << "Executing RaSQL query ...";
//        HqlTable table = runRasqlQuery(rman_db, rman_tr, select->query);
//        table.print(cout);
//        status = "ok";
//    }
//    catch (r_Error &e)
//    {
//        ERROR << "Query execution error: " << e.what();
//        status = string("failed... ") + e.what();
//    }

    cout << RESPONSE_PROMPT << status << endl;

    cout << QUERY_PROMPT;
}

/* Returns the available Rasdaman collections as a set of strings. */
vector<string> HqlMain::getRasdamanCoverages()
{
    const char query[] = "select mddcollname from RAS_MDDCOLLNAMES;";

    /* Connect to the default rasdaman backend dabatase. */
    connection C("dbname=RASBASE");

    HqlTable table = getInstance().runSqlQuery(C, query);

    vector<string> cov = table.getColumn(0);

    TRACE << "Rasdaman collections: ";
    for (int i = 0; i < cov.size(); i++)
        TRACE << " * " << cov[i];

    return cov;
}

/* Returns the available Postgres tables as a set of strings. */
vector<string> HqlMain::getPostgresTables()
{
    const char query[] = "SELECT tablename FROM pg_tables where tablename "
            "not like 'pg_%' and tablename not like 'sql_%';";

    /* Connect to the default SQL database.*/
    connection C;

    HqlTable table = getInstance().runSqlQuery(C, query);

    vector<string> names = table.getColumn(0);

    TRACE << "Postgres tables: ";
    for (int i = 0; i < names.size(); i++)
        TRACE << " * " << names[i];

    return names;
}

/* Execute a query on a predefined Postgres connection, and return the
 * results at column "outIndex" as a set of strings.
 * NOTE: Throws an std::exception in case of error.
 */
HqlTable HqlMain::runSqlQuery(connection_base& C, const char* queryString)
{
    DEBUG << "Connected to SQL database: " << C.dbname();
    TRACE << "SQL Backend version: " << C.server_version();
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
        return HqlTable();
    }

    T.abort();


    HqlTable table = HqlTable();
    table.importFromSql(R);
    return table;
}



void commitRasqlTransaction(r_Transaction *tr)
{
    TRACE << "Committing current Rasdaman transaction ...";
    tr->commit();
    delete tr;
    tr = NULL;
    TRACE << "Rasdaman transaction closed.";
}

void abortRasqlTransaction(r_Transaction *tr)
{
    TRACE << "Aborting current Rasdaman transaction ...";
    tr->abort();
    delete tr;
    tr = NULL;
    TRACE << "Rasdaman transaction closed.";
}

/* Run a Rasql query on a given rasdaman database and transaction.
 * NOTE: Throws an (r_Error) exception if something goes wrong.
 */
HqlTable HqlMain::runRasqlQuery(r_Database *db, const char* queryString)
{
    TRACE << "Opening Rasdaman transaction ...";
    r_Transaction *rman_tr = new r_Transaction();
    rman_tr->begin(r_Transaction::read_write);
    TRACE << "Successfully connected to rasdaman !";

    DEBUG << "Executing RaSQL query: " << queryString;
    r_OQL_Query query(queryString);

    /* The RaSQL result set is now declared globally, following trick in RaSQL app. */
    r_Set< r_Ref_Any > result_set;
    r_Ref< r_GMarray > image;
    r_Iterator< r_Ref_Any > iter;

    /* Execute the actual query. */
    try
    {
        TRACE << "Executing RaSQL ...";
        r_oql_execute(query, result_set);
        TRACE << "RaSQL execution ended. ";
        DEBUG << "Result has " << result_set.cardinality() << " objects... ";
        TRACE << "Finished executing RaSQL.";
    }
    catch (r_Error &e)
    {
        abortRasqlTransaction(rman_tr);
        throw e;
    }

    HqlTable table;
    table.importFromRasql(&result_set);

    /* Close the transaction (after processing of data) */
    commitRasqlTransaction(rman_tr);

    /* Return the data. */
    return table;
}
