/* 
 * File:   QtSource.cpp
 * Author: andrei
 * 
 * Created on June 26, 2010, 12:56 PM
 */

#include <map>
#include "QtDataSource.hpp"
#include "HqlMain.hpp"

using namespace std;

QtDataSource::QtDataSource(char* name)
    : tableName(name), alias("")
{
    TRACE << "Initializing Data Source with table name from char : " << name << flush;
}

QtDataSource::QtDataSource(char* name, char* alias)
    : tableName(name), alias(alias)
{
    TRACE << "Initializing Data Source with table name and alias from char: " << name 
            << " AS " << alias << flush;
}

QtDataSource::QtDataSource(QtNode* name, char* alias)
    : tableName(name->toString()), alias(alias)
{
    TRACE << "Initializing Data Source with table name from node and alias from char: " << tableName 
            << " AS " << alias << flush;
}

QtDataSource::QtDataSource(QtNode* name)
    : tableName(name->toString()), alias("")
{
    TRACE << "Initializing Data Source with table name from node: " << tableName << flush;
}

QtDataSource::~QtDataSource()
{
	TRACE << "Destructor ... " << flush;
}

HqlTable* QtDataSource::execute()
{
    TRACE << "step 1" << flush;
    HqlTable* table = NULL;
    TRACE << "step 2" << flush;
    string q;
    TRACE << "step 3" << flush;
    
    setupDbSource();
    TRACE << "step 4" << flush;
    
    switch (db_source)
    {
        case POSTGRES:
            q = string("SELECT * FROM ") + tableName;
            table = HqlMain::getInstance().runSqlQuery(q);
            break;
        case RASDAMAN:
            q = string("SELECT ") + tableName + " FROM " + tableName;
            table = HqlMain::getInstance().runRasqlQuery(q);
            break;
        default:
            break;
    }
    
    return table;
}

string QtDataSource::toString()
{
    string out(tableName);
    if (alias != "")
        out += string(" AS ") + string(alias);
    return out;
}

DbEnum QtDataSource::setupDbSource()
{
    TRACE << tableName << " ...";
    map<string,DbEnum> tableMap = HqlMain::getInstance().tableMap;
    if (tableMap.count(string(tableName)) == 0)
    {
        ERROR << "Table '" << tableName << "' does not exist in Postgres nor in Rasdaman !";
		// FIXME: use dedicated exception
        throw string("Table does not exist: ") + string(tableName);
    }
    db_source = tableMap[string(tableName)];
    DEBUG << "DB source for " << tableName << " is " << db_source;

    return db_source;
}

string QtDataSource::getTableName()
{
    return string(tableName);
}