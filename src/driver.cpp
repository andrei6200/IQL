/* Various C++ Standard Library includes */
#include <set>
#include <iostream>
#include <string>
#include <cstring>
#include <signal.h>

#include "HqlMain.hpp"
#include "utils/logger.hpp"
#include "config.hpp"


#define EMPTY_QUERY "(no query)"

extern int errorCount;
extern void yyparse();
extern char* hqlQueries;
extern void yyparse();



using namespace std;

string readQuery()
{
    cout << QUERY_PROMPT;
    yyparse();
    string result;
    if (hqlQueries == NULL || strlen(hqlQueries) == 0)
        result = EMPTY_QUERY;
    else
        result = string(hqlQueries);
    return result;
}

void exitProgram(int code, string queries)
{
    DEBUG << endl;
    if (queries != string(""))
    {
        DEBUG << "Below is the complete string of parsed HQL queries:";
        DEBUG << queries;
    }
    DEBUG << "Found " << errorCount << " error(s) ";

    TRACE << "End of main() ... " << endl;
    exit(code);
}

void signalHandler(int signal)
{
    INFO << "Received signal: " << signal;
    INFO << "Terminating program gracefully ...";
    exitProgram(signal, string(""));
}

/* Main function. Returns 0 on successful execution and non-zero otherwise. */
int main(int argc, char** argv)
{
    TRACE << "Start of main()" << endl;

    /* Install signal handlers */
    (void) signal(SIGINT, signalHandler);
    (void) signal(SIGTSTP, signalHandler);

    /* Analyze command-line arguments. */
    TRACE << "Command: " << argv[0];
    argc --;
    TRACE << "Called with " << argc << " arguments.";
    bool onlyOneQuery = false;
    if (argc == 1)
        if (strcmp(argv[1], "--unique") == 0 || strcmp(argv[1], "-u") == 0)
            onlyOneQuery = true;
    if (onlyOneQuery)
        DEBUG << "Accepting only one query.";
    else
        DEBUG << "Accepting as many queries as possible.";


    /* First create an instance of the HqlMain class.
     * Initialization is performed here.*/
    try
    {
        HqlMain *hql = HqlMain::getInstance();
    }
    catch (...)
    {
        // Probably the connections to the source systems could not be established
        exitProgram(-1, "");
    }

    /* Then run the parser and wait input from the user. Queries will be executed
     * as they come in, one by one. The string "allQueries" will contain all
     * the user-typed queries, after the grammar yields control. 
     */
    string allQueries, query;
    do
    {
        query = readQuery();
        allQueries += query;
    } while (query != EMPTY_QUERY && !onlyOneQuery);

    /* And let the user know that we are exiting the application... */
    exitProgram(errorCount, allQueries);

    return errorCount;
}
