/* 
 * File:   QtSource.cpp
 * Author: andrei
 * 
 * Created on June 26, 2010, 12:56 PM
 */

#include <map>
#include "QtDataSource.hpp"
#include "IqlApp.hpp"
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

IqlTable* QtDataSource::execute()
{
    PostgresDS &pg = IqlApp::getInstance().getSqlDataSource();
    RasdamanDS &rman = IqlApp::getInstance().getRasqlDataSource();
    IqlTable* table = NULL, *tmp = NULL;
    vector<string> cols;
    string q;
    
    setupDbSource();
    
    switch (db_source)
    {
        case POSTGRES:
            q = "SELECT * FROM " + tableName;
            TRACE << q;
            tmp = pg.query(q);
            TRACE << tmp;
            cols = tmp->getQualifiedColumnNames();

            TRACE << q;
            q = "SELECT " + cols[0] + " AS " + tableName + IQL_TBL_COL_SEP + cols[0];
            for (int i = 1; i < cols.size(); i++)
                q += ", " + cols[i] + " AS " + tableName + IQL_TBL_COL_SEP + cols[i];
            q += " INTO " + this->id + " FROM " + tableName;

            table = pg.query(q);
            pg.addTempTable(this->id);
            delete table;
            table = tmp;
            table->setName(tableName, true);
            table->setName(this->id, false);
            break;
            
        case RASDAMAN:
            /* Fetch the OIDs */
            table = rman.getCollection(tableName, false, true);
            table->setName(this->id, false);

            /* Store the IqlTable in Postgres for subsequent calculations */
            pg.insertData(table, this->id);

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
    map<string,DbEnum> tableMap = IqlApp::getInstance().tableMap;
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