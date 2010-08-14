
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
PostgresDS *pg = NULL;
HqlTable *mr = NULL, *mr2 = NULL, *rgb = NULL;


/* Add the columns of two tables. */
void testAddColumns()
{
    if (rgb == NULL || mr2 == NULL)
        throw string("Error, test input is NULL.");
    
    rgb->addColumns(mr2);
    if (rgb)
        cout << rgb << endl;
}

/* Compute the cartesian product of two tables. */
void testCartesianProd()
{
    if (mr == NULL || rgb == NULL)
        throw string("Error, test input is NULL.");

    HqlTable* prod = mr->crossProduct(rgb);
    if (prod)
    {
        cout << prod << endl;
        delete prod;
    }
    else
        throw string("Error, query result is NULL.");
}

void testInsertIntoPostgres()
{
    pg->insertData(mr, "testMr");

    HqlTable *pgtbl = pg->query("TABLE testMr");
    if (pgtbl)
    {
        cout << "Table testMr, fetched from Postgres: " << endl;
        cout << pgtbl << endl;
        delete pgtbl;
    }
    else
        throw string("Error, query result is NULL.");
}

void endProgram()
{
    if (mr)
        delete mr;
    if (mr2)
        delete mr2;
    if (rgb)
        delete rgb;

    if (rman)
    {
        delete rman;
        rman = NULL;
    }
    if (pg)
    {
        delete pg;
        pg = NULL;
    }
}

int main()
{
    cout << "Test program : Hello, everyone. " << endl;

    try
    {
        rman = new RasdamanDS();
        rman->connect();
        TRACE << "Connected to rasdaman !!!" << endl;

        pg = new PostgresDS();
        pg->connect();

        mr = rman->getCollection(string("mr"));
        rgb = rman->getCollection(string("rgb"));
        mr2 = rman->getCollection(string("mr2"));

        testAddColumns();

        testCartesianProd();

        testInsertIntoPostgres();

        testInsertIntoPostgres();

        rman->disconnect();

        cout << "Exiting gracefully..." << endl;

    }
    catch (pqxx::pqxx_exception &e)
    {
        FATAL << "Caught PQXX error: " << e.base().what();
        cout << "Caught PQXX error: " << e.base().what() << endl;
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
