
#include <iostream>
#include "config.hpp"
#include "RasdamanDS.hpp"
#include "PostgresDS.hpp"
#include "HqlTable.hpp"
#include "HqlMain.hpp"
#include "logger.hpp"

#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;


RasdamanDS *rman = NULL;




void endProgram()
{
    if (rman)
    {
        delete rman;
        rman = NULL;
    }
}

void testGetOid()
{
    string oid = "PostgreSQL|RASBASE|513";
    string queryString = string("SELECT var FROM mr AS var ") +
                "WHERE oid(var) = <\"" + oid + "\">";
    r_Set<r_Ref< r_GMarray > > mddSet;
    r_OQL_Query query(queryString.c_str());
    r_oql_execute(query, mddSet);

    if (mddSet.cardinality() == 1)
        cout << "Successfully retrieved Rasdaman object: " << oid << endl;
    else
        cerr << "Failed to retrieve Rasdaman object: " << oid << endl;
}

int main()
{
    cout << "Test program : Hello, everyone. " << endl;

    try
    {
        rman = new RasdamanDS();
        rman->connect();
        TRACE << "Connected to rasdaman !!!" << endl;

        testGetOid();

        rman->disconnect();

        cout << "Exiting gracefully..." << endl;

    }
    catch (string &s)
    {
        FATAL << "Caught runtime exception: " << s;
        cout << "Caught runtime exception: " << s << endl;
    }
    catch (exception &e)
    {
        FATAL << "Caught general exception : " << e.what();
        cout << "Caught general exception: " << e.what() << endl;
    }
    catch (...)
    {
        endProgram();
        FATAL << "Caught fatal error.";
        cout << "Caught fatal error." << endl;
        throw;
    }

    endProgram();

    return 0;
}
