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
#include <vector>

#include "grammar/structures.hpp"
#include "utils/HqlTable.hpp"




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

    /* Execute a query on a predefined Postgres connection, and return the results.
     */
    HqlTable runSqlQuery(pqxx::connection_base& C, const char* queryString);

    /* Execute a Rasql query on a given Rasdaman database connection and transaction.
     * Prints some info about the query result. Returns the results as a HqlTable.
     *
     * "db" and "tr" may be NULL, in which case a DB connection is automatically
     * aquired and released after execution. 
     */
    HqlTable runRasqlQuery(r_Database *db, const char* queryString);

private:
    /*
     * Private constructor.
     * Queries the postgres and rasdaman databases for available tables.
     */
    HqlMain();

    /* Returns the available Rasdaman collections as a set of strings. */
    std::vector<std::string> getRasdamanCoverages();
    /* Returns the available Postgres tables as a set of strings. */
    std::vector<std::string> getPostgresTables();


    /* The singleton instance. */
    static HqlMain instance;

    /* Name Dictionary for available tables */
    std::map<std::string, int> tableMap;

    /* Connection to Postgres */
    static pqxx::connection_base *pg_conn;

    /* Connection and transaction to Rasdaman */
    static r_Database *rman_db;
};


#endif	/* HQLMAIN_HH */
