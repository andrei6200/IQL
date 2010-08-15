/* 
 * File:   QtList.cpp
 * Author: andrei
 * 
 * Created on June 26, 2010, 2:49 PM
 */

#include <list>
#include <typeinfo>

#include "QtList.hpp"
#include "QueryTree.hpp"
#include "HqlMain.hpp"

using namespace std;

QtList::QtList() : QtNode()
{
    data = vector<QtNode*>();
    results = vector<HqlTable*>();
}

QtList::QtList(vector<QtNode*> newdata)
{
    data = newdata;
    results = vector<HqlTable*>();
}

void QtList::add(QtNode *elem)
{
    data.push_back(elem);
}

QtList::~QtList()
{
    for (int i = 0; i < data.size(); i ++)
        if (data[i])
        {
            delete data[i];
            data[i] = NULL;
        }
    data.clear();
}

string QtList::toString()
{
    string out("");
    if (data.size() == 0)
        return out;

    out += (data[0])->toString();

    for (int i = 1; i < data.size(); i++)
        out += ", " + data[i]->toString();

    return out;
}

HqlTable* QtList::execute()
{
    results.clear();
    HqlTable *table = NULL;
    for (int i = 0; i < data.size(); i++)
    {
        table = data[i]->execute();
        if (table->dataAlsoInMemory == false)
        {
            /* This table only carries the tableName. Retrieve the table explicitly. */
            string q = "SELECT * FROM " + table->getName();
            pqxx::result R = HqlMain::getInstance().getSqlDataSource().regularQuery(q);
            table->importFromSql(R);
        }
        /* Store the table for later processing */
        TRACE << "Table " << i << " of the list: " << endl << table;
        results.push_back(table);
    }
    // Return NULL, because no action is specified. These intermediate results
    // can be processed with the multiplyResults() or addResults() functions.
    return NULL;
}

HqlTable* QtList::multiplyResults()
{
    TRACE << "Computing the cross product between " << data.size() << " HqlTables...";

    if (results.size() == 0)
    {
        execute();
        if (results.size() == 0)
            return NULL;
    }

    HqlTable *t = results[0];
    if (t == NULL)
    {
        ERROR << "Cannot compute cross product, intermediate result was NULL.";
        throw string("Intermediate result was NULL. ");
    }
    for (int i = 1; i < results.size(); i++)
        if (results[i] == NULL)
        {
            ERROR << "Cannot compute cross product, intermediate result was NULL.";
            throw string("Intermediate result was NULL. ");
        }
        else
        {
            HqlTable *t2 = t->crossProduct(results[i]);
            t = t2;
        }
    
    TRACE << "The cross product between " << data.size() << " HqlTables is " << t;

    return t;
}

HqlTable* QtList::addResults()
{
    TRACE << "Computing the column-addition between " << data.size() << " HqlTables...";

    if (results.size() == 0)
    {
        execute();
        if (results.size() == 0)
            return NULL;
    }

    HqlTable *t = results[0];
    if (t == NULL)
    {
        ERROR << "Cannot add columns of tables, intermediate result was NULL.";
        throw string("Intermediate result was NULL. ");
    }
    for (int i = 1; i < results.size(); i++)
        if (results[i] == NULL)
        {
            ERROR << "Cannot add columns of tables, intermediate result was NULL.";
            throw string("Intermediate result was NULL. ");
        }
        else
            t->addColumns(results[i]);

    TRACE << "The column addition between " << data.size() << " HqlTables is " << t;

    return t;
}

DbEnum QtList::setupDbSource()
{
    /* Cache the result. */
    if (db_source != DB_NOT_INITIALIZED)
        return db_source;
    
    db_source = DB_NOT_INITIALIZED;
    DbEnum source;

    vector<QtNode*>::iterator i;
    /* First iteration checks for errors and searches for init value */
    for (i = data.begin(); i != data.end(); i++)
    {
        /* Check all children nodes */
        QtNode* node = *i;
        source = node->setupDbSource();
        if (source == UNKNOWN_DB || source == DB_NOT_INITIALIZED || source == MIXED)
        {
            db_source = source;
            return source;
        }
        else
            db_source = source;
    }
    /* now db_source is either POSTGRES or RASDAMAN. Check that the other values correspond. */
    for (i = data.begin(); i != data.end(); i++)
    {
        if ((*i)->getDbSource() != db_source)
        {
            db_source = MIXED;
            return db_source;
        }
    }
    
    TRACE << "Found source system: " << db_source;
    return db_source;
}

int QtList::length()
{
    return data.size();
}

QtNode* QtList::get(int index)
{
    if (index < 0 || index >= data.size())
        return NULL;
    return data[index];
}

/*ghhhfjjvk deci acum eu scriu la teza lui andrei
//{hh este orasul in care vrem sa ne mutam curand}
//[nu stiu daca andrei mai are chef sa se intoarca azi la teza, insa banuiesc ca nu-i dupa el]//
    (int acum se joaca cu petru si ma cearta despre un termin pe care nu mai stie cand era, dar stie ca era )
            petru s-a intors pe burtica si partaie...
 */

void QtList::print(ostream &o, string indent)
{
    o << indent << "QtList (" << id << ") with " << data.size() << " element(s)" << endl;
    for (int i = 0; i < data.size(); i++)
        data[i]->print(o, indent + QTINDENT);
}
