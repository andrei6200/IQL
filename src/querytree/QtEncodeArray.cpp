/* 
 * File:   QtEncodeArray.cpp
 * Author: andrei
 * 
 * Created on August 4, 2010, 1:47 PM
 */

#include "QtEncodeArray.hpp"
#include "QtInducedOperation.hpp"
#include "HqlMain.hpp"

using namespace std;

QtEncodeArray::QtEncodeArray(QtNode *node, string fmt)
    : child(node), format(fmt), options("")
{
}

QtEncodeArray::QtEncodeArray(QtNode *node, string fmt, string opt)
    : child(node), format(fmt), options(opt)
{
}

QtEncodeArray::~QtEncodeArray()
{
    if (child)
    {
        delete child;
        child = NULL;
    }
}

string QtEncodeArray::toString()
{
    string repr= format;
    repr += "(";
    repr += child->toString();
    if (options != "")
    {
        repr += ",";
        repr += options;
    }
    repr += ")";

    return repr;
}

DbEnum QtEncodeArray::setupDbSource()
{
    db_source = RASDAMAN;
    return db_source;
}

HqlTable* QtEncodeArray::execute()
{
    RasdamanDS &rman = HqlMain::getInstance().getRasqlDataSource();
    PostgresDS &pg = HqlMain::getInstance().getSqlDataSource();
    HqlTable *tmp = child->execute();
    string query = "SELECT " + format + "( " + tmp->getName() + " ) INTO " + this->id +
            " FROM " + tmp->getName();
    delete tmp;
    rman.updateQuery(query);
    tmp = rman.getCollection(this->id);
    rman.addTempTable(this->id);

    // Store the OID table in Postgres as well
    pg.insertData(tmp, this->id);
    pg.addTempTable(this->id);

    return tmp;
}

void QtEncodeArray::print(ostream &o, std::string indent)
{
    o << indent << "QtEncodeArray (" << id << "): format = " << format << endl;
    child->print(o, indent + QTINDENT);
}
