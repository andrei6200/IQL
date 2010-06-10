#include <iostream>

#include <pqxx/pqxx>

extern "C"
{
    extern int errorCount;
    void yyparse();
    char* hqlQueries;
}


using namespace std;
using namespace pqxx;


void runQuery(char* query)
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


int main()
{
	yyparse();
        cout << "\n\n\n" << "HQL QUERIES\n" << hqlQueries << endl << endl << flush;

        runQuery(hqlQueries);
        
	return errorCount;
}
