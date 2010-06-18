/* 
 * File:   HqlMain.hpp
 * Author: andrei
 *
 * Created on June 12, 2010, 10:31 PM
 *
 * Implements the singleton design pattern.
 */

#ifndef HQLMAIN_HH
#define	HQLMAIN_HH

#include "rasdaman.hh"
#include <pqxx/pqxx>

#include <set>
#include <string>

#include "grammar/structures.hpp"




/* Enumeration of available DBMS systems */
enum DbEnum
{
    POSTGRES = 0,
    RASDAMAN = 1
};


/* Singleton class responsible for DB access. */
class HqlMain
{
public:
    /* Return (and initialize if needed) the singleton instance of HqlMain. */
    static HqlMain& getInstance();

    /*
     * This function receives the a query string from the parser. It is responsable
     * for the execution of the query, and for the delivery of the results to the
     * user.
     * Returns the status as a string.
     */
    std::string executeHqlQuery(const char* msg);

    /*
     * This function receives a SelectStruct structure from the parser, and is
     * responsable for the query execution and result delivery. 
     */
    void executeHqlQuery(selectStruct *select);

    /* Public destructor. */
    ~HqlMain();

    /* Execute a query on a predefined Postgres connection, and return the
     * results at column "outIndex" as a set of strings.
     */
    std::set<std::string> runSqlQuery(pqxx::connection_base& C, const char* queryString, int outIndex);

    /* Execute a Rasql query on a given Rasdaman database connection and transaction.
     * Prints some info about the query result.
     *
     * "db" and "tr" may be NULL, in which case a DB connection is automatically
     * aquired and released after execution. 
     */
    void runRasqlQuery(r_Database *db, r_Transaction *tr, const char* queryString);

private:
    /*
     * Private constructor.
     * Queries the postgres and rasdaman databases for available tables.
     */
    HqlMain();

    /* Prints an SQL table in a human-readable form on stdout. */
    void printSqlTable(std::set<std::string> sqlTable);

    /* Returns the available Rasdaman collections as a set of strings. */
    std::set<std::string> getRasdamanCoverages();
    /* Returns the available Postgres tables as a set of strings. */
    std::set<std::string> getPostgresTables();


    /* The singleton instance. */
    static HqlMain instance;

    /* Name Dictionary for available tables */
    std::map<std::string, int> tableMap;

    /* Connection to Postgres */
    static pqxx::connection_base *pg_conn;
};


#endif	/* HQLMAIN_HH */
