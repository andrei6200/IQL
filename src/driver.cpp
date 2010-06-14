/* Various C++ Standard Library includes */
#include <set>
#include <iostream>
#include <string>
#include <cstring>

#include "HqlMain.hpp"

extern "C"
{
    extern int errorCount;
//    extern void yyparse();
    extern char* hqlQueries;
}

extern void yyparse();


using namespace std;

string readAllQueries()
{
    yyparse();
    string result;
    if (hqlQueries == NULL || strlen(hqlQueries) == 0)
        result = "(null)";
    else
        result = string(hqlQueries);
    return result;
}

int main()
{
    cout << "Beginning ... " << endl;

    /* First create an instance of the HqlMain class. */
    HqlMain hql = HqlMain::getInstance();

    /* Then run the parser and wait input from the user. Queries will be executed
     * as they come in, one by one. The string "allQueries" will contain all
     * the user-typed queries, after the grammar yields control. 
     */
    string allQueries = readAllQueries();

    /* And let the user know that we are exiting the application... */
    cout << endl << "Below is the complete string of parsed HQL queries:" << endl
            << allQueries << endl << "Found " << errorCount << " error(s) " << endl
            << "Will exit now..." << endl;

//   runSqlQuery("SELECT * FROM pg_database");
//   runSqlQuery(hqlQueries);
//    hql.runRasqlQuery(NULL, NULL, "SELECT png(rgb) from rgb");


    return errorCount;
}
