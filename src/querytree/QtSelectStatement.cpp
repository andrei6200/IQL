/*
 * File:   QtSelect.cpp
 * Author: andrei
 *
 * Created on June 26, 2010, 1:36 PM
 */

#include "QtSelectStatement.hpp"
#include <vector>
#include <typeinfo>
#include "HqlMain.hpp"
#include "logger.hpp"

using namespace std;

QtSelectStatement::QtSelectStatement()
{
    what = QtList();
    from = QtList();
    TRACE << "Initialized QtSelect.";
}

QtSelectStatement::QtSelectStatement(QtList refs, QtList sources)
    : what(refs), from(sources)
{
    TRACE << "Initializing QtSelect with 2 QtList objects";
    TRACE << "Selects: " << what.toString();
    TRACE << "From: " << from.toString();
}

QtSelectStatement::~QtSelectStatement()
{
}

DbEnum QtSelectStatement::setupDbSource()
{
    string s = toString();
    TRACE << "QtSelect :: setupDbSource() for query'" << s << "' ...";
    TRACE << "QtSelect: Setting up sources in FROM clause...";
    db_source = from.setupDbSource();
    TRACE << "QtSelect: Setting up sources in SELECT clause...";
    what.setupDbSource();

    TRACE << "QtSelect: Found source tables: " << db_source << endl;
    TRACE << "QtSelect: Found selected tables: " << what.getDbSource() << endl;
    return db_source;
}

HqlTable* QtSelectStatement::execute()
{
    TRACE << "Executing QtSelect ...";

    HqlTable *table = NULL;

    setupDbSource();
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
                        << "' (" << typeid(*node).name() << ") is not a concrete system: "
                        << node->getDbSource();
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
                        << "' (" << typeid(*node).name() << ") is not a concrete system: "
                        << node->getDbSource();
            }

            QtList fromSql = QtList(sqlFrom);
            QtList whatSql = QtList(sqlWhat);
            QtSelectStatement sqlSelect = QtSelectStatement(whatSql, fromSql);
            DEBUG << "subquery SQL: " << sqlSelect.toString();
            HqlTable *t2 = sqlSelect.execute();
//            if (t2)
//                t2->print(cout);

            QtList fromRasql = QtList(rasqlFrom);
            QtList whatRasql = QtList(rasqlWhat);
            QtSelectStatement rasqlSelect = QtSelectStatement(whatRasql, fromRasql);
            DEBUG << "subquery RaSQL: " << rasqlSelect.toString();
            HqlTable *t1 = rasqlSelect.execute();
//            if (t1)
//                t1->print(cout);

            if (t1 && t2)
            {
                table = t2->crossProduct(t1);
            }
            else
                /* FIXME: error handling */
                ;

            if (t1)
            {
                delete t1;
            }
            if (t2)
            {
                delete t2;
            }

        }
        else
        {
            INFO << "Mixed queries are too complicated (at least for now).";
            table = NULL;
        }
        break;
    default:
        cout << RESPONSE_PROMPT << "Cannot execute query, unknown data source. " << endl;
        table = NULL;
    }

    return table;
}


std::string QtSelectStatement::toString()
{
    string out = string("SELECT ") + what.toString() +
                 string(" FROM ") + from.toString();
    return out;
}

/* Returns true if the current Hql query contains a simple mixture of RaSQl and SQL.
 In case this is not a mixed query, this function returns false. */
bool QtSelectStatement::mixedQueryIsSimple()
{
    if (db_source != MIXED)
        return false;
    for (int i = 0; i < from.length(); i++)
    {
        DbEnum db = from.get(i)->setupDbSource();
        if (db == MIXED || db == UNKNOWN_DB || db == DB_NOT_INITIALIZED)
            return false;
    }
    return true;
}
