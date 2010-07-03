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
    string s = toString();
    TRACE << "QtSelect :: setupDbSource() for query'" << s << "' ...";
    db_source = from.setupDbSource();
    TRACE << "QtSelect: Found source tables: " << db_source << endl;
    TRACE << "QtSelect: Found selected tables: " << what.setupDbSource() << endl;
    return db_source;
}

HqlTable* QtSelect::execute()
{
    TRACE << "Executing QtSelect ...";
    HqlTable *table = NULL;
    HqlTable tableCopy;

    from.setupDbSource();
    what.setupDbSource();
    
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
            vector<QtNode*> rasqlWhat, sqlWhat;
            int i;
            QtNode* node;
            for (i = 0; i < what.length(); i++)
            {
                node = what.get(i);
                if (node->getDbSource() == RASDAMAN)
                    rasqlWhat.push_back(node);
                else if (node->getDbSource() == POSTGRES)
                    sqlWhat.push_back(node);
                else
                    WARN << "Data source for node '" << node->toString()
                        << "' cannot be fully determined !";
            }
            vector<QtNode*> rasqlFrom, sqlFrom;
            for (i = 0; i < from.length(); i++)
            {
                node = from.get(i);
                if (node->getDbSource() == RASDAMAN)
                    rasqlFrom.push_back(node);
                else if (node->getDbSource() == POSTGRES)
                    sqlFrom.push_back(node);
                else
                    WARN << "Data source for node '" << node->toString()
                        << "' cannot be fully determined !";
            }
            
            QtList fromRasql = QtList(rasqlFrom);
            QtList whatRasql = QtList(rasqlWhat);
            QtSelect rasqlSelect = QtSelect(whatRasql, fromRasql);
            HqlTable *t1 = rasqlSelect.execute();
            if (t1)
            {
                t1->print(cout);
                delete t1;
            }

            QtList fromSql = QtList(sqlFrom);
            QtList whatSql = QtList(sqlWhat);
            QtSelect sqlSelect = QtSelect(whatSql, fromSql);
            HqlTable *t2 = sqlSelect.execute();
            if (t2)
            {
                t2->print(cout);
                delete t2;
            }

        }
        else
        {
            INFO << "Mixed query is too complicated (at least for now).";
            table = NULL;
        }
        break;
    default:
        cout << RESPONSE_PROMPT << "Cannot execute query, unknown data source. " << endl;
        table = NULL;
    }

    return table;
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