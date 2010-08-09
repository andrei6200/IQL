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




/* Header file of this class */
#include "HqlMain.hpp"
#include "logger.hpp"
#include "querytree/QueryTree.hpp"
#include "config.hpp"
#include "HqlTable.hpp"


using namespace pqxx;
using namespace std;



HqlMain* HqlMain::instance = NULL;

extern void exitProgram(int code, string queries);

/* Return (and initialize if needed) the singleton instance of HqlMain. */
HqlMain& HqlMain::getInstance()
{
    if (!instance)
        instance = new HqlMain();
    return *instance;
}

/* Private constructor.
 * Queries the postgres and rasdaman databases for available tables.
 */
HqlMain::HqlMain()
{
    INFO << "Initialization of HqlMain ... ";
    /* Initialization */

    pg = new PostgresDS();
    rman = new RasdamanDS();

    /* Rasdaman tables */
    vector<string> rasTables = rman->getObjectNames();
    DEBUG << "Found " << rasTables.size() << " Rasdaman tables. ";

    /* Postgres tables */
    vector<string> pgTables = pg->getObjectNames();
    DEBUG << "Found " << pgTables.size() << " Postgres tables. ";

    /* Create the table dictionary */
    vector<string>::iterator iter;
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
    map<string, DbEnum>::iterator tuple;
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

    if (pg)
    {
        pg->disconnect();
        delete pg;
        pg = NULL;
    }

    if (rman)
    {
        rman->disconnect();
        delete rman;
        rman = NULL;
    }

    if (instance)
    {
        delete instance;
        instance = NULL;
    }

    TRACE << "HqlMain fields have been destroyed.";
}

HqlTable* HqlMain::runSqlQuery(string query)
{
    return pg->query(query);
}

HqlTable* HqlMain::runRasqlQuery(string query)
{
    return rman->query(query);
}
