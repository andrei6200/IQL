/* Various C++ Standard Library includes */
#include <set>
#include <iostream>
#include <string>
#include <cstring>

#include "HqlMain.hpp"
#include "logger.hpp"
#include "config.hpp"

//extern "C"
//{
    extern int errorCount;
    extern void yyparse();
    extern char* hqlQueries;
//}

extern void yyparse();


using namespace std;

string readAllQueries()
{
    cout << PROMPT;
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
    TRACE << "Start of main()" << endl;

    /* First create an instance of the HqlMain class. */
    HqlMain hql = HqlMain::getInstance();

    /* Then run the parser and wait input from the user. Queries will be executed
     * as they come in, one by one. The string "allQueries" will contain all
     * the user-typed queries, after the grammar yields control. 
     */
    string allQueries = readAllQueries();

    /* And let the user know that we are exiting the application... */
    DEBUG << endl;
    DEBUG << "Below is the complete string of parsed HQL queries:";
    DEBUG << allQueries;
    DEBUG << "Found " << errorCount << " error(s) ";

//   runSqlQuery("SELECT * FROM pg_database");
//   runSqlQuery(hqlQueries);
//    hql.runRasqlQuery(NULL, NULL, "SELECT png(rgb) from rgb");

    TRACE << "End of main() ... " << endl;

    return errorCount;
}
