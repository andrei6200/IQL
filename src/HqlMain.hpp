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

class HqlMain
{
public:
    static HqlMain& getInstance();

    void executeHqlQuery(const char* msg);

    ~HqlMain();

private:
    static HqlMain instance;
    
    HqlMain();
    
    std::set<std::string> getRasdamanCoverages();
    std::set<std::string> getPostgresTables();

public:
    std::set<std::string> runSqlQuery(pqxx::connection_base& C, const char* queryString, int outIndex);
    void runRasqlQuery(r_Database *db, r_Transaction *tr, const char* queryString);

};


#endif	/* HQLMAIN_HH */
