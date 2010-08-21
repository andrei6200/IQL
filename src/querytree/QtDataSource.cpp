/* 
 * File:   QtSource.cpp
 * Author: andrei
 * 
 * Created on June 26, 2010, 12:56 PM
 */

#include <map>
#include "QtDataSource.hpp"
#include "HqlMain.hpp"
#include "QueryTree.hpp"

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
	TRACE << "Destructor ... ";
}

HqlTable* QtDataSource::execute()
{
    HqlTable* table = NULL, *tmp = NULL;
    vector<string> cols;
    string q;
    
    setupDbSource();
    
    switch (db_source)
    {
        case POSTGRES:
            q = "SELECT * FROM " + tableName;
            TRACE << q;
            tmp = HqlMain::getInstance().runSqlQuery(q);
            TRACE << tmp;
            cols = tmp->getColumnNames();

            TRACE << q;
            q = "SELECT " + cols[0] + " AS " + tableName + "_" + cols[0];
            for (int i = 1; i < cols.size(); i++)
                q += ", " + cols[i] + " AS " + tableName + "_" + cols[i];
            q += " INTO " + this->id + " FROM " + tableName;

            table = HqlMain::getInstance().runSqlQuery(q);
            HqlMain::getInstance().getSqlDataSource().addTempTable(this->id);
            delete table;
            table = tmp;
            table->setName(tableName, true);
            table->setName(this->id, false);
            break;
            
        case RASDAMAN:
            /* Create a new rasdaman collection */
//            q = string("SELECT ") + tableName + " INTO " + this->id + " FROM " + tableName;
//            TRACE << q;
//            HqlMain::getInstance().getRasqlDataSource().updateQuery(q);
//            HqlMain::getInstance().getRasqlDataSource().addTempTable(this->id);

            /* And fetch the OIDs */
            table = HqlMain::getInstance().getRasqlDataSource().getCollection(tableName, false, true);
            table->setName(this->id, false);
//            table->setName(tableName, true);
//            table->setName(tableName, true);
//            table->setName(this->id, true);

            /* Store the HqlTable in Postgres for subsequent calculations */
            HqlMain::getInstance().getSqlDataSource().insertData(table, this->id);

            /* And remember the alias */
//            QueryTree::getInstance().addAlias(tableName, this->id);

            break;
            
        default:
            q = "Cannot retrieve table '" + tableName +
                    "' because of unknown DataSource system.";
            ERROR << q;
            throw q;
            break;
    }

    
    TRACE << table;
    
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

void QtDataSource::print(ostream &o, std::string indent)
{
    o << indent << "QtDataSource (" << id << "): tableName '" << tableName << "'";
    if (alias != "")
        o << alias << "'" << alias << "'";
    o << endl;
}