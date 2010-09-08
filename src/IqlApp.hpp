/* 
 * File:   HqlMain.hpp
 * Author: andrei
 *
 * Created on June 12, 2010, 10:31 PM
 *
 * Implements the singleton design pattern.
 */

#ifndef IQLAPP_HH
#define	IQLAPP_HH


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
#include "datasources/IqlTable.hpp"
#include "datasources/PostgresDS.hpp"
#include "datasources/RasdamanDS.hpp"



/** Singleton application class. */
class IqlApp
{
public:
    /** Return (and initialize if needed) the singleton instance of HqlMain. */
    static IqlApp& getInstance();

    /** Public destructor. */
    ~IqlApp();

    /** Name Dictionary for available tables */
    std::map<std::string, DbEnum> tableMap;

    PostgresDS& getSqlDataSource();

    RasdamanDS& getRasqlDataSource();

    bool isPostGisFunction(char *);
    
private:
    /*
     * Private constructor.
     * Queries the postgres and rasdaman databases for available tables.
     */
    IqlApp();

    /*
     * Read the functions available for use with PostGIS, and make them available
     * to the lexer.
     */
    void initPostgisFunctionList();

    /* The singleton instance. */
    static IqlApp *instance;

    /* Data Sources */
    PostgresDS *pg;
    RasdamanDS *rman;

    std::vector<std::string> postgisKw;
};


#endif	/* IQLAPP_HH */
