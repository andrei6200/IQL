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
    string out = "";
    if (data.size() == 0)
        return out;

    if (data[0] != NULL)
    {
        string s = (data[0])->toString();;
        out += s;
    }
    else
        out += "(null)";

    for (int i = 1; i < data.size(); i++)
    {
        out += string(", ");
        if (data[i] != NULL)
        {
            string s = data[i]->toString();
            out += s;
        }
        else
            out += "(null)";
    }

    return out;
}

void QtList::executeChildren()
{
    results.clear();
    HqlTable *table = NULL;
    for (int i = 0; i < data.size(); i++)
    {
        table = data[i]->execute();
        if (table == NULL)
            throw string("Internal error: Received NULL intermediate result.");
        if (table->dataAlsoInMemory == false)
        {
            /* This table only carries the tableName. Retrieve the table explicitly. */
            string name = table->getName();
            delete table;
            string q = "SELECT * FROM " + name;
            TRACE << q;
            table = HqlMain::getInstance().getSqlDataSource().query(q);
            table->setName(name);
        }
        /* Store the table for later processing */
//        TRACE << "Table " << i << " of the list: " << endl << table;
        results.push_back(table);
    }
}

HqlTable* QtList::execute()
{
    executeChildren();

    /* Default action: add columns of the results. One can also use the multiplyResults()
     * function for different processing.
     */
    HqlTable *result = addResults();
    return result;
}

HqlTable* QtList::multiplyResults()
{
    TRACE << "Computing the cross product between " << data.size() << " HqlTables...";

    if (results.size() == 0)
    {
        executeChildren();
        if (results.size() == 0)
            return NULL;
    }

    if (results[0] == NULL)
    {
        ERROR << "Cannot compute cross product, intermediate result was NULL.";
        throw string("Intermediate result was NULL. ");
    }
    string tableList = results[0]->getName();
    for (int i = 1; i < results.size(); i++)
        if (results[i] == NULL)
        {
            ERROR << "Cannot compute cross product, intermediate result was NULL.";
            throw string("Intermediate result was NULL. ");
        }
        else
        {
            tableList += ", " + results[i]->getName();
        }

    string q = "SELECT * INTO " + this->id + " FROM " + tableList;
    HqlTable *result = HqlMain::getInstance().runSqlQuery(q);
    HqlMain::getInstance().getSqlDataSource().addTempTable(this->id);
    HqlMain::getInstance().getSqlDataSource().insertHqlIdToTable(this->id);
    result->setName(this->id, false);

    TRACE << "The cross product between " << data.size() << " HqlTables is " << result;

    return result;
}

HqlTable* QtList::addResults()
{
    TRACE << "Computing the column-addition between " << data.size() << " HqlTables...";

    if (results.size() == 0)
    {
        executeChildren();
        if (results.size() == 0)
            return NULL;
    }

    if (results[0] == NULL)
    {
        ERROR << "Cannot add columns of tables, intermediate result was NULL.";
        throw string("Intermediate result was NULL. ");
    }
    string tableList = results[0]->getName();
//    HqlMain::getInstance().getSqlDataSource().insertHqlIdToTable(results[0]->getName());
    for (int i = 1; i < results.size(); i++)
        if (results[i] == NULL)
        {
            ERROR << "Cannot add columns of tables, intermediate result was NULL.";
            throw string("Intermediate result was NULL. ");
        }
        else
        {
            tableList += " JOIN " + results[i]->getName() + " USING (" + HQL_COL + ")";
//            HqlMain::getInstance().getSqlDataSource().insertHqlIdToTable(results[i]->getName());
        }

    string q;
    HqlTable *result = NULL;
    if (results.size() == 1)
    {
        // Only one table, query does not have join.
        q = "SELECT * INTO " + this->id + " FROM " + results[0]->getName();
        result = HqlMain::getInstance().runSqlQuery(q);
    }
    else
    {
        q = "SELECT * INTO " + this->id + " FROM " + tableList;
        result = HqlMain::getInstance().runSqlQuery(q);
    }
    result->setName(this->id);
    HqlMain::getInstance().getSqlDataSource().addTempTable(this->id);
    TRACE << "The column addition between " << data.size() << " HqlTables is " << result;

    return result;
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
