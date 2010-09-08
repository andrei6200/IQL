/* 
 * File:   QtEncodeArray.cpp
 * Author: andrei
 * 
 * Created on August 4, 2010, 1:47 PM
 */

#include "QtEncodeArray.hpp"
#include "IqlApp.hpp"
#include "QueryTree.hpp"

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

IqlTable* QtEncodeArray::execute()
{
    /* Delegate the format encoding to the Query Tree post-processing.
     We have to do this because of a bug in rasdaman, which crashes the server
     when more than one encoded mdd is inserted into a collection, using a SELECT INTO
     query. */
//    RasdamanDS &rman = IqlApp::getInstance().getRasqlDataSource();
//    PostgresDS &pg = IqlApp::getInstance().getSqlDataSource();
    IqlTable *tmp = child->execute();
    QueryTree::getInstance().setEncodingFormat(format);
//    string query = "SELECT " + format + "( " + tmp->getName() + " ) INTO " + this->id +
//            " FROM " + tmp->getName();
//    delete tmp;
//    rman.updateQuery(query);
//    tmp = rman.getCollection(this->id, false);
//    tmp->setName(this->id, true);
//    rman.addTempTable(this->id);
//
//    // Store the OID table in Postgres as well
//    pg.insertData(tmp, this->id);
//    pg.addTempTable(this->id);

    return tmp;
}

void QtEncodeArray::print(ostream &o, std::string indent)
{
    o << indent << "QtEncodeArray (" << id << "): format = " << format << endl;
    child->print(o, indent + QTINDENT);
}
