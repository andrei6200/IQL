/* 
 * File:   QtWhere.cpp
 * Author: andrei
 * 
 * Created on September 2, 2010, 6:26 PM
 *
 * Corresponds to the WHERE clause
 */

#include "QtWhere.hpp"
#include "QtInducedOperation.hpp"
#include "datasources/PostgresDS.hpp"
#include "HqlMain.hpp"

QtWhere::QtWhere(QtNode *cond) : condition(cond), srcTable(NULL)
{
}

QtWhere::~QtWhere()
{
    if (condition)
    {
        delete condition;
        condition = NULL;
    }
    if (srcTable)
    {
        delete srcTable;
        srcTable = NULL;
    }
}

HqlTable* QtWhere::execute()
{
    PostgresDS &pg = HqlMain::getInstance().getSqlDataSource();
    HqlTable* result = NULL, *tmp = NULL;
    string query, colName, condTableName;

    /* The execution of the condition takes into account the raw cartesian
     * product of the tables in the FROM clause. */
    tmp = condition->execute();
    condTableName = tmp->getName();
    delete tmp;
    query = "SELECT * FROM " + condTableName;
    tmp = pg.query(query);
    
    colName = tmp->getQualifiedColumnNames().at(0);
    delete tmp;
    TRACE << "Found column name for condition: " << colName;

    query = "SELECT * INTO " + this->id +
            " FROM " + condTableName + " WHERE " + condTableName + "." + colName;
    result = pg.query(query);
    result->setName(this->id);
    pg.addTempTable(this->id);

    return result;
}

string QtWhere::toString()
{
    string str = condition->toString();
    return str;
}

DbEnum QtWhere::setupDbSource()
{
    return condition->setupDbSource();
}

void QtWhere::print(ostream &o, std::string indent)
{
    o << indent << "QtWhere (" << id << "): '" << endl;
    condition->print(o, indent + QTINDENT);
}

//void QtWhere::setSourceTable(HqlTable* src)
//{
//    srcTable = src;
//}