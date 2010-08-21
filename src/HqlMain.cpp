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


#include <algorithm>
#include <cctype>

/* Header file of this class */
#include "HqlMain.hpp"
#include "logger.hpp"
#include "querytree/QueryTree.hpp"
#include "config.hpp"


using namespace pqxx;
using namespace std;



HqlMain* HqlMain::instance = NULL;

extern void exitProgram(int code, string queries);

/* Return (and initialize if needed) the singleton instance of HqlMain. */
HqlMain& HqlMain::getInstance() {
    if (!instance)
        instance = new HqlMain();
    return *instance;
}

/* Private constructor.
 * Queries the postgres and rasdaman databases for available tables.
 */
HqlMain::HqlMain() {
    id = random() % 10000;

    /* Initialization */
    INFO << "Initialization of HqlMain (ID " << id << ")... ";
    pg = new PostgresDS();
    rman = new RasdamanDS();
    pg->connect();
    /* Connect to Rasdaman only when needed. */
//    rman->connect();

    /* Rasdaman tables */
    vector<string> rasTables = rman->getObjectNames();
    DEBUG << "Found " << rasTables.size() << " Rasdaman tables. ";

    /* Postgres tables */
    vector<string> pgTables = pg->getObjectNames();
    DEBUG << "Found " << pgTables.size() << " Postgres tables. ";

    /* Create the table dictionary */
    vector<string>::iterator iter;
    for (iter = pgTables.begin(); iter != pgTables.end(); iter++)
        tableMap[*iter] = POSTGRES;
    for (iter = rasTables.begin(); iter != rasTables.end(); iter++)
        if (tableMap.count(*iter) == 0)
            tableMap[*iter] = RASDAMAN;
        else {
            FATAL << "\n ERROR ! Table '" << *iter << "' is a coverage name in Rasdaman AND "
                    << " a table in Postgres. ";
            cerr << "\n ERROR ! Table '" << *iter << "' is a coverage name in Rasdaman AND "
                    << " a table in Postgres. " << endl;
            //            exit(1);
        }

    /* Print the Table dictionary */
    map<string, DbEnum>::iterator tuple;
    TRACE;
    TRACE << "Name dictionary : ";
    for (tuple = tableMap.begin(); tuple != tableMap.end(); tuple++)
        TRACE << " - " << (*tuple).first << " -> " << (*tuple).second;
    TRACE;

    /* Initialize the PostGIS keywords */
    initPostgisFunctionList();

    INFO << " Initialization complete. ";
}

/* Public destructor */
HqlMain::~HqlMain() {
    TRACE << "Destroying Singleton instance of HqlMain.";

    if (pg) {
        pg->disconnect();
        delete pg;
        pg = NULL;
    }

    if (rman) {
        rman->disconnect();
        delete rman;
        rman = NULL;
    }

    if (instance) {
        delete instance;
        instance = NULL;
    }

    TRACE << "HqlMain fields have been destroyed.";
}

HqlTable* HqlMain::runSqlQuery(string query) {
    return pg->query(query);
}

HqlTable* HqlMain::runRasqlQuery(string query) {
    return rman->query(query);
}

long HqlMain::getId() {
    return id;
}

PostgresDS& HqlMain::getSqlDataSource() {
    return *pg;
}

RasdamanDS& HqlMain::getRasqlDataSource() {
    return *rman;
}

void HqlMain::initPostgisFunctionList() {
    TRACE << "Initializing PostGIS function list ...";
    /* This function will be called in the constructor, after we will have a
     Postgres connection available */
    string q = string("SELECT DISTINCT routine_name ") +
            "FROM information_schema.routines " +
            "WHERE specific_schema NOT IN " +
            "('pg_catalog', 'information_schema') " +
            "AND type_udt_name != 'trigger' " +
            "ORDER BY routine_name ASC;";
    pg->connect();
    pqxx::result sqlResult(pg->regularQuery(q));

    postgisKw.clear();
    result::const_iterator it;
    vector<string> row;
    string kw("");
    stringstream kwList;
    for (it = sqlResult.begin(); it != sqlResult.end(); it++) {
        row.clear();
        // Insert actual data
        result::tuple tuple = *it;
        kw = (*it)[0].as(string());
        postgisKw.push_back(kw);
        kwList << kw << ", ";
    }
    sort(postgisKw.begin(), postgisKw.end());
    TRACE << "Finished initialization of PostGIS function list. " << endl << kwList.str() << endl;
}

bool HqlMain::isPostGisFunction(char* str)
{
    TRACE << "Keyword query : " << str;
    char buf[50];
    int i = 0;
    for (i = 0; i < strlen(str) && i < 49; i ++)
        buf[i] = tolower(str[i]);
    buf[i] = 0;
    string key(buf);
    bool result = binary_search(postgisKw.begin(), postgisKw.end(), key);
    TRACE << "Keyword '" << key << "' is " << (result ? "" : " *not*") << " a PostGIS keyword";
    return result;
}
