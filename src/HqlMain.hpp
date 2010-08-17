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


/* PostgreSQL includes and defines */
#include <pqxx/pqxx>

/* Rasdaman includes and defines */
#define DEBUG_MAIN

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#ifdef __GNUG__
#include "raslib/template_inst.hh"
#endif
#endif
#include "rasdaman.hh"


#include <set>
#include <vector>
#include <string>

#include "querytree/QtNode.hpp"
#include "datasources/HqlTable.hpp"
#include "datasources/PostgresDS.hpp"
#include "datasources/RasdamanDS.hpp"



/* Singleton class responsible for DB access. */
class HqlMain
{
public:
    /* Return (and initialize if needed) the singleton instance of HqlMain. */
    static HqlMain& getInstance();

    /* Public destructor. */
    ~HqlMain();

    /* Name Dictionary for available tables */
    std::map<std::string, DbEnum> tableMap;

    HqlTable* runSqlQuery(std::string query);

    HqlTable* runRasqlQuery(std::string query);

    PostgresDS& getSqlDataSource();

    RasdamanDS& getRasqlDataSource();

    long getId();

    bool isPostGisFunction(char *);
    
private:
    /*
     * Private constructor.
     * Queries the postgres and rasdaman databases for available tables.
     */
    HqlMain();

    /*
     * Read the functions available for use with PostGIS, and make them available
     * to the lexer.
     */
    void initPostgisFunctionList();

    /* The singleton instance. */
    static HqlMain *instance;

    /* Data Sources */
    PostgresDS *pg;
    RasdamanDS *rman;

    std::vector<std::string> postgisKw;

    /* Integer ID */
    long id;
};


#endif	/* HQLMAIN_HH */
