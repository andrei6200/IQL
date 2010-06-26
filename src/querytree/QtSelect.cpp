/* 
 * File:   QtSelect.cpp
 * Author: andrei
 * 
 * Created on June 26, 2010, 1:36 PM
 */

#include "QtSelect.hpp"
#include <vector>
#include "HqlMain.hpp"

//#include "utils/logger.hpp"

using namespace std;

QtSelect::QtSelect()
{
    what = QtList();
    from = QtList();
    TRACE << "Initialized QtSelect.";
}

QtSelect::QtSelect(QtList refs, QtList sources)
    : what(refs), from(sources)
{
    TRACE << "Initializing QtSelect with 2 QtList objects";
    TRACE << "Selects: " << what.toString();
    TRACE << "From: " << from.toString();
}

QtSelect::~QtSelect()
{
}

DbEnum QtSelect::setupDbSource()
{
    TRACE << "QtSelect :: setupDbSource() for query'" << toString() << "' ...";
    db_source = from.setupDbSource();
    TRACE << "QtSelect: Found source system: " << db_source << endl;
    return db_source;
}

HqlTable* QtSelect::execute()
{
    TRACE << "Executing QtSelect ...";
    static HqlTable table;
    
    switch (db_source)
    {
    case POSTGRES:
        cout << RESPONSE_PROMPT << "Executing postgres query ..." << endl;
        TRACE << "Executing postgres query: " << toString() ;
        table = HqlMain::getInstance().runSqlQuery(toString());
        break;
    case RASDAMAN:
        cout << RESPONSE_PROMPT << "Executing rasdaman query ..." << endl;
        TRACE << "Executing rasdaman query: " << toString();
        table = HqlMain::getInstance().runRasqlQuery(toString());
        break;
    case MIXED:
        cout << RESPONSE_PROMPT << "Executing mixed query ..." << endl;
        TRACE << "Executing mixed query: " << toString();
        if (mixedQueryIsSimple())
        {
            TRACE << "Mixed query is simple";
        }
        else
            TRACE << "Mixed query is complicated.";
        break;
    default:
        cout << RESPONSE_PROMPT << "Cannot execute query, unknown data source. " << endl;
    }

    return &table;
}


std::string QtSelect::toString()
{
    string out = string("SELECT ") + what.toString() +
                 string(" FROM ") + from.toString();
    return out;
}

/* Returns true if the current Hql query contains a simple mixture of RaSQl and SQL.
 In case this is not a mixed query, this function returns false. */
bool QtSelect::mixedQueryIsSimple()
{
    if (db_source != MIXED)
        return false;
    for (int i = 0; i < from.length(); i++)
    {
        DbEnum db = from.get(i)->setupDbSource();
        if (db == MIXED || db == UNKNOWN_DB)
            return false;
    }
    return true;
}