/* 
 * File:   QtList.cpp
 * Author: andrei
 * 
 * Created on June 26, 2010, 2:49 PM
 */

#include <list>
#include <typeinfo>

#include "QtList.hpp"

using namespace std;

QtList::QtList() : QtNode()
{
    TRACE << "Initializing QtList ...";
    data = vector<QtNode*>();
}

QtList::QtList(vector<QtNode*> newdata)
{
    TRACE << "Initializing QtList ...";
    data = newdata;
}

void QtList::add(QtNode *elem)
{
    TRACE << "Adding to QtList value: " << elem->toString();
    TRACE << "      and type " << typeid(*elem).name();
    data.push_back(elem);
}

QtList::~QtList()
{
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
    return NULL;
}

DbEnum QtList::setupDbSource()
{
    TRACE << "QtList :: setupDbSource()";
    TRACE << "QtList: " << toString();
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
        TRACE << "QtList: Calling setupDbSource for child node: " << typeid(*node).name();
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
    
    TRACE << "QtList :: found source system: " << db_source;
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

