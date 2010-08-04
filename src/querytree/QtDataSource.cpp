/* 
 * File:   QtSource.cpp
 * Author: andrei
 * 
 * Created on June 26, 2010, 12:56 PM
 */

#include "QtDataSource.hpp"
#include "HqlMain.hpp"

using namespace std;

QtDataSource::QtDataSource(char* name) : tableName(name), alias(NULL)
{
    TRACE << __FUNCTION__ << ": Initializing QtSource: " << name;
}

QtDataSource::QtDataSource(char* name, char* alias) : tableName(name), alias(alias)
{
    TRACE << "Initializing QtSource: " << name << " as " << alias;
}

QtDataSource::QtDataSource(QtNode* name, char* alias): tableName(name->toCString()), alias(alias)
{
    TRACE << "Initializing QtSource: " << tableName << " as " << alias;
}

QtDataSource::QtDataSource(QtNode* name): tableName(name->toCString()), alias(NULL)
{
    TRACE << "Initializing QtSource: " << tableName;
}

QtDataSource::~QtDataSource()
{
    if (tableName)
        delete tableName;
    if (alias)
        delete alias;
}

HqlTable* QtDataSource::execute()
{
    return NULL;
}

string QtDataSource::toString()
{
    string out(tableName);
    if (alias)
        out += string(" AS ") + string(alias);
    return out;
}

DbEnum QtDataSource::setupDbSource()
{
    TRACE << "QtSource :: setupDbSource() : " << tableName << " ...";
    map<string,DbEnum> tableMap = HqlMain::getInstance()->tableMap;
    if (tableMap.count(string(tableName)) == 0)
    {
        ERROR << "Table '" << tableName << "' does not exist in Postgres nor in Rasdaman !";
        throw string("Table does not exist: ") + string(tableName);
    }
    db_source = tableMap[string(tableName)];
    DEBUG << "QtSource :: setupDbSource() : for " << tableName << " is " << db_source;

    return db_source;
}