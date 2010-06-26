/* 
 * File:   QtList.cpp
 * Author: andrei
 * 
 * Created on June 26, 2010, 2:49 PM
 */

#include <list>

#include "QtList.hpp"

using namespace std;

QtList::QtList()
{
    TRACE << "Initializing QtList ...";
    data = vector<QtNode*>();
}

void QtList::add(QtNode *elem)
{
    TRACE << "Adding to QtList: " << elem->toString();
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
    db_source = UNKNOWN_DB;
    DbEnum source;
    for (int i = 0; i < data.size(); i++)
    {
        /* Check all children nodes */
        source = data[i]->setupDbSource();
        switch (source)
        {
            case UNKNOWN_DB:
                continue;
            case MIXED:
                db_source = source;
                return db_source;
            case POSTGRES:
            case RASDAMAN:
                if (db_source == UNKNOWN_DB)
                {
                    db_source = source;
                    break;
                }
                if (source != db_source)
                {
                    db_source = MIXED;
                    return db_source;
                }
        }
    }
    TRACE << "QtList :: source system: " << db_source;
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