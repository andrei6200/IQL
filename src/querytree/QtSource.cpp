/* 
 * File:   QtSource.cpp
 * Author: andrei
 * 
 * Created on June 26, 2010, 12:56 PM
 */

#include "QtSource.hpp"
#include "HqlMain.hpp"

using namespace std;

QtSource::QtSource(char* name) : tableName(name), alias(NULL)
{
    TRACE << "Initializing QtSource: " << name;
}

QtSource::QtSource(char* name, char* alias) : tableName(name), alias(alias)
{
    TRACE << "Initializing QtSource: " << name << " as " << alias;
}

QtSource::QtSource(QtNode* name, char* alias): tableName(name->toCString()), alias(alias)
{
    TRACE << "Initializing QtSource: " << tableName << " as " << alias;
}

QtSource::QtSource(QtNode* name): tableName(name->toCString()), alias(NULL)
{
    TRACE << "Initializing QtSource: " << tableName;
}

QtSource::~QtSource()
{
    if (tableName)
        delete tableName;
    if (alias)
        delete alias;
}

HqlTable* QtSource::execute()
{
    return NULL;
}

string QtSource::toString()
{
    string out(tableName);
    if (alias)
        out += string(" AS ") + string(alias);
    return out;
}

DbEnum QtSource::setupDbSource()
{
    TRACE << "QtSource :: setupDbSource() : " << tableName << " ...";
    map<string,DbEnum> tableMap = HqlMain::getInstance().tableMap;
    if (tableMap.count(string(tableName)) == 0)
    {
        ERROR << "Table '" << tableName << "' does not exist in Postgres nor in Rasdaman !";
        throw string("Table does not exist: ") + string(tableName);
    }
    db_source = tableMap[string(tableName)];
    DEBUG << "QtSource :: setupDbSource() : for " << tableName << " is " << db_source;
    return db_source;
}