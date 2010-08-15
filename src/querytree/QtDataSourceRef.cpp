/* 
 * File:   QtDataSourceRef.cpp
 * Author: andrei
 * 
 * Created on June 26, 2010, 1:29 PM
 */

#include <typeinfo>

#include "QtDataSourceRef.hpp"
#include "QtDot.hpp"
#include "HqlMain.hpp"

QtDataSourceRef::QtDataSourceRef(QtNode* b): base(b), alias(NULL), all(false)
{
    if (b->toString() == "*")
        all = true;
}

QtDataSourceRef::QtDataSourceRef(QtNode* b, char* al) : base(b), alias(al), all(false)
{
}

QtDataSourceRef::~QtDataSourceRef()
{
    if (base)
        delete base;
    if (alias)
        delete alias;
}

HqlTable* QtDataSourceRef::execute()
{
    /* A DataSource Table reference is not supposed to create a new datasource object,
     * contrary to expectations. In Postgres tables exist already, in Rasdaman
     * collections exist already. The only exception is when we refer to a
     * Rasdaman collection; then we want to create a SQl table to talk about it.
     */
    setupDbSource();

    // TODO: what about "*" ?

    // Table name can have alias

    RasdamanDS &rman = HqlMain::getInstance().getRasqlDataSource();
    HqlTable *result = NULL, *tmp = NULL;
    string tableName = base->toString();
    string query;

    switch (db_source)
    {
        case POSTGRES:
            tmp = base->execute();
            TRACE << "Child table: " << endl << tmp;
            query = "SELECT * INTO " + this->id
                         + " FROM " + tmp->getName();
            delete tmp;
            result = HqlMain::getInstance().runSqlQuery(query);
            result->setName(this->id);
            TRACE << "Current node table" << endl << result;
            break;
            
        case RASDAMAN:
            result = rman.getCollection(tableName, false);
            result->setName(this->id);
            TRACE << "Current node table: " << endl << result;

            // now store this table in Postgres
            HqlMain::getInstance().getSqlDataSource().insertData(result, this->id);
            break;

        default:
            // FIXME: throw exception
            throw string("Unknown data source for DataSourceRef node.");
            break;
    }

    return result;
}

string QtDataSourceRef::toString()
{
    
    string out(base->toString());
    if (alias)
        out += string(" AS ") + string(alias);
    return out;
}

char* QtDataSourceRef::toCString()
{
    return (char*) this->toString().c_str();
}

DbEnum QtDataSourceRef::setupDbSource()
{
    db_source = base->setupDbSource();
    return db_source;
}

bool QtDataSourceRef::selectsAll()
{
    return all;
}

void QtDataSourceRef::print(ostream &o, string indent)
{
    o << indent << "DataSourceRef (" << id << "): " << endl;
    base->print(o, indent + QTINDENT);
}