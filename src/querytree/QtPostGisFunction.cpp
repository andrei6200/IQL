/* 
 * File:   QtPostGisFunction.cpp
 * Author: andrei
 * 
 * Created on August 14, 2010, 12:04 PM
 */

#include "QtPostGisFunction.hpp"
#include "IqlApp.hpp"
#include "QueryTree.hpp"

QtPostGisFunction::QtPostGisFunction(char *fname, QtNode *node)
    : function(fname), child(node), children(NULL)
{
}

QtPostGisFunction::QtPostGisFunction(char* fname, QtList* node)
    : function(fname), child(NULL), children(node)
{
}

QtPostGisFunction::~QtPostGisFunction()
{
    if (child)
    {
        delete child;
        child = NULL;
    }
    if (children)
    {
        delete children;
        children = NULL;
    }
}

IqlTable* QtPostGisFunction::execute()
{
    PostgresDS &pg = IqlApp::getInstance().getSqlDataSource();
    IqlTable *result = NULL, *tmp = NULL;
    string childTableName, arguments;

    if (child)
        tmp = child->execute();
    if (children)
        tmp = children->execute();
    childTableName = tmp->getName();
    delete tmp;
    string q = "SELECT * FROM " + childTableName;
    tmp = pg.query(q);
    if (child)
        arguments = tmp->getQualifiedColumnNames()[0];
    if (children)
    {
        vector<string> names = tmp->getQualifiedColumnNames();
        // Names[0] is the internal HQL ID column
        arguments = names[1];
        for (int i = 2; i < names.size(); i++)
            arguments += ", " + names[i];
    }
    delete tmp;
    
    q = "SELECT " + function + " ( " + arguments + " ) AS " + this->id + IQL_TBL_COL_SEP + function
            + ", " + HQL_COL + " INTO " + this->id + " FROM " + childTableName;
    result = pg.query(q);
    pg.addTempTable(this->id);
    result->setName(this->id);

    return result;
}

string QtPostGisFunction::toString()
{
    string str = function + " ( ";
    if (child)
        str += child->toString();
    if (children)
        str += children->toString();
    str += " ) ";
    return str;
}

DbEnum QtPostGisFunction::setupDbSource()
{
    db_source = POSTGRES;
    DbEnum other;
    if (child)
        other = child->setupDbSource();
    if (children)
        other = children->setupDbSource();
    switch (other)
    {
        case POSTGRES:
            break;
        case RASDAMAN:
        case MIXED:
            db_source = MIXED;
        case UNKNOWN_DB:
        default:
            db_source = UNKNOWN_DB;
    }
    
    return db_source;
}

void QtPostGisFunction::print(ostream &o, std::string indent)
{
    o << indent << "QtPostGisFunction (" << id << "): " << function << endl;
    if (child)
        child->print(o, indent + QTINDENT);
    if (children)
        children->print(o, indent + QTINDENT);
}
