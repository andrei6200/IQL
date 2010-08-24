/* 
 * File:   QtPostGisFunction.cpp
 * Author: andrei
 * 
 * Created on August 14, 2010, 12:04 PM
 */

#include "QtPostGisFunction.hpp"
#include "HqlMain.hpp"
#include "QueryTree.hpp"

QtPostGisFunction::QtPostGisFunction(char *fname, QtNode *node)
    : function(fname), child(node)
{
}

QtPostGisFunction::~QtPostGisFunction()
{
    if (child)
    {
        delete child;
        child = NULL;
    }
}

HqlTable* QtPostGisFunction::execute()
{
    PostgresDS &pg = HqlMain::getInstance().getSqlDataSource();
    RasdamanDS &rman = HqlMain::getInstance().getRasqlDataSource();
    HqlTable *result = NULL, *tmp = NULL;

    tmp = child->execute();
    string childTableName = tmp->getName();
    delete tmp;
    string q = "SELECT * FROM " + childTableName;
    tmp = pg.query(q);
    string col = tmp->getColumnNames()[0];
    delete tmp;
    q = "SELECT " + function + " ( " + col + " ) AS " + this->id + "_" + function
            + ", " + HQL_COL + " INTO " + this->id + " FROM " + childTableName;
    result = pg.query(q);
    pg.addTempTable(this->id);
    result->setName(this->id);

    return result;
}

string QtPostGisFunction::toString()
{
    string str = function + " ( " + child->toString() + " ) ";
    return str;
}

DbEnum QtPostGisFunction::setupDbSource()
{
//    TRACE << toString();
//
//    /* Check the source tables. */
//    if (column == "*")
//    {
//        QtList *from = QueryTree::getInstance().getRoot()->getSourceTables();
//        db_source = from->setupDbSource();
//        return db_source;
//    }
//
//    string search = table;
//    if (table == "")
//        search = column;
//
//
//    /* Else try to use the global data source dictionary. */
//    map<string, DbEnum> tables = HqlMain::getInstance().tableMap;
//    switch (tables.count(search))
//    {
//    case 0:
//        WARN << "Could not find table '" << search <<
//                "' in global table dictionary";
//        db_source = UNKNOWN_DB;
//        break;
//    case 1:
//        DEBUG << "Found String " << search << " as a table.";
//        DEBUG << "\tDatabase source: " << tables[search];
//        db_source = tables.at(search);
//        break;
//    default:
//        TRACE << "Found " << tables.count(search) << " occurrences of '" << search
//                << "' inside the global table dictionary!";
//        db_source = UNKNOWN_DB;
//        break;
//    }

    return db_source;
}

void QtPostGisFunction::print(ostream &o, std::string indent)
{
    o << indent << "QtPostGisFunction (" << id << "): " << function << endl;
    child->print(o, indent + QTINDENT);
}
