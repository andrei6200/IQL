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

using namespace pqxx;
using namespace std;

HqlMain HqlMain::instance;

HqlMain& HqlMain::getInstance()
{
    return instance;
}


void HqlMain::initLogging()
{
}

HqlMain::HqlMain()
{
    initLogging();
    /* Initialization */

    /* Rasdaman tables */
    set<string> rasTables = getRasdamanCoverages();
    set<string>::iterator iter;
    cout << rasTables.size() << " Rasdaman tables: " << endl;

    /* Postgres tables */
    set<string> pgTables = getPostgresTables();
    cout << pgTables.size() << " Postgres tables: " << endl;

    /* Create the table dictionary */
    map<string, string> tableMap;
    string rql = "Rasdaman";
    string sql = "Postgres";
    for (iter = rasTables.begin(); iter != rasTables.end(); iter ++ )
        tableMap.insert(pair<string, string>(*iter, rql));
    for (iter = pgTables.begin(); iter != pgTables.end(); iter ++)
        if (tableMap.count(*iter) == 0)
            tableMap.insert(pair<string, string>(*iter, sql));
        else
        {
            cout << "\n ERROR ! Table " << *iter << " is a coverage name in Rasdaman AND "
                    << " a table in Postgres. Cannot continue. " << endl;
            exit(1);
        }

    /* Print the Table dictionary */
    map<string, string>::iterator tuple;
    cout << "Name dictionary : " << endl;
    for (tuple = tableMap.begin(); tuple != tableMap.end(); tuple ++)
        cout << " - " << (*tuple).first << " -> " << (*tuple).second << endl;
    cout << endl;

    cout << endl << " Initialization complete. " << endl;
}

HqlMain::~HqlMain()
{
}

void HqlMain::executeHqlQuery(const char* msg)
{
    cout << "Found HQL query: " << msg << endl;
}

set<string> HqlMain::getRasdamanCoverages()
{
    const char query[] = "select mddcollname from RAS_MDDCOLLNAMES;";

    /* Connect to the default rasdaman backend dabatase. */
    connection C("dbname=RASBASE");

    set<string> tableSet = getInstance().runSqlQuery(C, query, 0);
    
    return tableSet;
}


set<string> HqlMain::getPostgresTables()
{
    const char query[] = "SELECT tablename FROM pg_tables where tablename "
                         "not like 'pg_%' and tablename not like 'sql_%';";
    
    /* Connect to the default SQL database.*/
    connection C;

    set<string> tableSet = getInstance().runSqlQuery(C, query, 0);

    return tableSet;
}


/* Run a SQL SELECT query on a given SQL database connection.
 Returns the set of values at column "outIndex" from the result table. */
set<string> HqlMain::runSqlQuery(connection_base& C, const char* queryString, int outIndex)
{
    set<string> resultSet;

    cout << "Connected to database: " << C.dbname() << endl
        << "Backend version: " << C.server_version() << endl
        << "Protocol version: " << C.protocol_version() << endl;

    // Begin a transaction acting on our current connection.  Give it a human-
    // readable name so the library can include it in error messages.
    nontransaction T(C, "sqlQuery");

    // Perform a query on the database, storing result tuples in R.
    result R( T.exec(queryString) );

    // We're expecting to find some tables...
    if (R.empty())
    {
        cout << "No tables found.  Cannot test.";
        return resultSet;
    }

    // Process each successive result tuple
    for (result::const_iterator c = R.begin(); c != R.end(); ++c)
        resultSet.insert(c[0].as(string()));

    T.abort();

    return resultSet;
}

/* Run a Rasql query on a given rasdaman database and transaction. 
 * If the database and/or transaction pointers are NULL, they are created and 
 * destroyed at the end of the function execution. */
void HqlMain::runRasqlQuery(r_Database *db, r_Transaction *tr, const char* queryString)
{
    r_OQL_Query query(queryString);

    /* Initialize the database connection and transaction */
    r_Database *database;
    r_Transaction *transaction;

    database = db;
    if (db == NULL)
    {
        database = new r_Database();
        database->set_servername( "localhost" );
        database->set_useridentification( "rasguest", "rasguest" );
        database->open( "RASBASE" );
    }
    transaction = tr;
    if (tr == NULL)
    {
        transaction = new r_Transaction();
        transaction->begin( r_Transaction::read_only );
    }


    r_Set< r_Ref_Any > result_set;
    r_Set< r_Ref< r_GMarray > > *image_set;
    r_Ref< r_GMarray > image;
    r_Iterator< r_Ref_Any > iter;

    /* Execute the actual query. */
    try
    {
        r_oql_execute(query, result_set);   
        cout << "Result has " << result_set.cardinality() << " objects... " << endl;
    }
    catch (r_Error &e)
    {
        cout << "ERROR: " << e.what() << endl;
    }

    iter = result_set.create_iterator();

    for(iter.reset();iter.not_done(); iter++ )
    {
        image = r_Ref<r_GMarray>(*iter);

        /* Print metadata ... taken from RaSQL */
        {
                cout << "  Oid...................: " << result_set.get_oid() << endl;
                cout << "  Type Structure........: "
                     << ( result_set.get_type_structure() ? result_set.get_type_structure() : "<nn>" ) << endl;
                cout << "  Type Schema...........: " << flush;
                if( result_set.get_type_schema() )
                        result_set.get_type_schema()->print_status( cout );
                else
                        cout << "(no name)" << flush;
                cout << endl;
                cout << "  Number of entries.....: " << result_set.cardinality() << endl;
                cout << "  Element Type Schema...: " << flush;
                if( result_set.get_element_type_schema() )
                        result_set.get_element_type_schema()->print_status( cout );
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
                cout << "\n\n\nThe output result is *NOT* a MDD. So I cannot print any spatial domain :-)" << endl;
                cout << "Output type is: " << result_set.get_element_type_schema()->type_id() << endl;
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

}
