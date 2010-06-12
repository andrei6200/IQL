/* 
 * File:   HqlMain.hpp
 * Author: andrei
 *
 * Created on June 12, 2010, 10:31 PM
 *
 * Implements the singleton design pattern.
 */

#ifndef TEST_HH
#define	TEST_HH

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
    
    std::set<std::string> listRasdamanCoverages();
    std::set<std::string> listPostgresTables();

    void runSqlQuery(char* queryString);
    void runRasqlQuery(char* queryString);

};



#endif	/* TEST_HH */

