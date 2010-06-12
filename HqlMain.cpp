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

HqlMain::HqlMain()
{
    /* Initialization */
    cout << " Initializing ... " << endl << endl;

    /* Rasdaman tables */
    set<string> rasTables = listRasdamanCoverages();
    set<string>::iterator iter;
    cout << rasTables.size() << " Rasdaman tables: " << endl;
    for (iter = rasTables.begin(); iter != rasTables.end(); iter ++ )
        cout << " - " << (*iter) << endl;
    cout << " ------------ " << endl;

    /* Postgres tables */
    set<string> pgTables = listPostgresTables();
    cout << pgTables.size() << " Postgres tables: " << endl;
    for (iter = pgTables.begin(); iter != pgTables.end(); iter ++ )
        cout << " - " << (*iter) << endl;
    cout << " ------------ " << endl;

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

set<string> HqlMain::listRasdamanCoverages()
{
    std::set<std::string> tableSet;

    const char query[] = "select mddcollname from RAS_MDDCOLLNAMES;";
    connection C("dbname=RASBASE");

    cout << "Connected to database: " << C.dbname() << endl
        << "Backend version: " << C.server_version() << endl
        << "Protocol version: " << C.protocol_version() << endl << endl;

    // Begin a transaction acting on our current connection.  Give it a human-
    // readable name so the library can include it in error messages.
    nontransaction T(C, "test1");

    // Perform the query on the database, storing result tuples in R.
    result R( T.exec(query) );

    // We're expecting to find some tables...
    if (R.empty())
    {
        cout << "No tables found.  Cannot test.";
        return tableSet;
    }

    // Process each successive result tuple, store the table names into the set
    for (result::const_iterator c = R.begin(); c != R.end(); ++c)
    {
        // Dump tuple number and column 0 value to cout.  Read the value using
        // as(), which converts the field to the same type as the default value
        // you give it (or returns the default value if the field is null).
//        cout << '\t' << to_string(c.num()) << '\t' << c[0].as(string()) << endl;
        tableSet.insert(c[0].as(string()));
    }

    T.commit();

    return tableSet;
}


set<string> HqlMain::listPostgresTables()
{
    std::set<std::string> tableSet;

    const char query[] = "SELECT tablename FROM pg_tables where tablename "
                         "not like 'pg_%' and tablename not like 'sql_%';";
    connection C;

    cout << "Connected to database: " << C.dbname() << endl
        << "Backend version: " << C.server_version() << endl
        << "Protocol version: " << C.protocol_version() << endl << endl;

    // Begin a transaction acting on our current connection.  Give it a human-
    // readable name so the library can include it in error messages.
    nontransaction T(C, "test1");

    // Perform the query on the database, storing result tuples in R.
    result R( T.exec(query) );

    // We're expecting to find some tables... Return empty set in this case.
    if (R.empty())
    {
        cout << "No tables found.  Cannot test.";
        return tableSet;
    }

    // Process each successive result tuple, store the table names into the set
    for (result::const_iterator c = R.begin(); c != R.end(); ++c)
        tableSet.insert(c[0].as(string()));

    T.commit();

    return tableSet;
}


void HqlMain::runSqlQuery(char* queryString)
{
    connection C;

    cout << "Connected to database: " << C.dbname() << endl
        << "Backend version: " << C.server_version() << endl
        << "Protocol version: " << C.protocol_version() << endl;

    // Begin a transaction acting on our current connection.  Give it a human-
    // readable name so the library can include it in error messages.
    nontransaction T(C, "test1");

    // Perform a query on the database, storing result tuples in R.
    result R( T.exec(queryString) );

    // We're expecting to find some tables...
    if (R.empty())
    {
        cout << "No tables found.  Cannot test.";
        return ;
    }

    // Process each successive result tuple
    for (result::const_iterator c = R.begin(); c != R.end(); ++c)
    {
        // Dump tuple number and column 0 value to cout.  Read the value using
        // as(), which converts the field to the same type as the default value
        // you give it (or returns the default value if the field is null).
        cout << '\t' << to_string(c.num()) << '\t' << c[0].as(string()) << endl;
    }

    T.commit();
}

void HqlMain::runRasqlQuery(char* queryString)
{
    r_OQL_Query query(queryString);

    r_Database database;
    r_Transaction transaction;
    r_Set< r_Ref_Any > result_set;
    r_Set< r_Ref< r_GMarray > > *image_set;
    r_Ref< r_GMarray > image;
    r_Iterator< r_Ref_Any > iter;

    database.set_servername( "localhost" );
    database.set_useridentification( "rasguest", "rasguest" );
    database.open( "RASBASE" );
    transaction.begin( r_Transaction::read_only );

    r_oql_execute(query, result_set);
    cout << "Result has " << result_set.cardinality() << " objects... " << endl;

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

        // work with the image
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

    transaction.commit();
    database.close();

}
