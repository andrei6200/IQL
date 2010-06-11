#include <iostream>
#include <cstdio>

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


extern "C"
{
    extern int errorCount;
    void yyparse();
    char* hqlQueries;
}


using namespace std;
using namespace pqxx;


void runSqlQuery(char* query)
{
    connection C;

    cout << "Connected to database." << endl
        << "Backend version: " << C.server_version() << endl
        << "Protocol version: " << C.protocol_version() << endl;

    // Begin a transaction acting on our current connection.  Give it a human-
    // readable name so the library can include it in error messages.
    nontransaction T(C, "test1");

    // Perform a query on the database, storing result tuples in R.
    result R( T.exec(query) );

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

void runRasqlQuery(char* queryString)
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


int main()
{
	yyparse();

        if (hqlQueries == NULL)
        {
            hqlQueries = " (nothing) ";
            cout << "\n\n\n" << "HQL QUERIES\n" << hqlQueries << endl << endl << flush;
            return 0;
        }

//        runSqlQuery("SELECT * FROM pg_database");
//        runSqlQuery(hqlQueries);
        runRasqlQuery(hqlQueries);

        
	return errorCount;
}
