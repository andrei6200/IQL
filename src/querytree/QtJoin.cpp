/* 
 * File:   QtJoin.cpp
 * Author: andrei
 * 
 * Created on August 4, 2010, 1:34 PM
 */

#include <algorithm>
#include <utility>

#include "QtJoin.hpp"
#include "datasources/PostgresDS.hpp"
#include "IqlApp.hpp"
#include "QueryTree.hpp"

QtJoin::QtJoin(QtNode* table1, QtNode* table2, char *jointype)
    : child1(table1), child2(table2), type(jointype), natural(false), cond(NULL), columns(NULL)
{
}

QtJoin::QtJoin(QtNode* table1, QtNode* table2, char* jointype, bool naturaljoin)
    : child1(table1), child2(table2), type(jointype), natural(naturaljoin), cond(NULL), columns(NULL)
{
}

QtJoin::QtJoin(QtNode* table1, QtNode* table2, char* jointype, std::vector<std::string *> *columns)
    : child1(table1), child2(table2), type(jointype), natural(false), cond(NULL), columns(columns)
{
}

QtJoin::QtJoin(QtNode* table1, QtNode* table2, char* jointype, QtNode *condition)
    : child1(table1), child2(table2), type(jointype), natural(false), cond(condition), columns(NULL)
{
}

QtJoin::~QtJoin()
{
    if (child1)
    {
        delete child1;
        child1 = NULL;
    }
    if (child2)
    {
        delete child2;
        child2 = NULL;
    }
    if (cond)
    {
        delete cond;
        cond = NULL;
    }
    if (columns)
    {
        for (int i = 0; i < columns->size(); i++)
            if (columns->at(i) != NULL)
            {
                delete columns->at(i);
                columns->at(i) = NULL;
            }
        delete columns;
        columns = NULL;
    }
}

IqlTable* QtJoin::execute()
{
    PostgresDS &pg = IqlApp::getInstance().getSqlDataSource();
    
    IqlTable *result = NULL, *tmp1 = NULL, *tmp2 = NULL;
    tmp1 = child1->execute();
    tmp2 = child2->execute();

    string query = "SELECT * INTO " + this->id + " FROM " +
            tmp1->getName() + " AS t1 ";
    query += type + " JOIN " + tmp2->getName() + " AS t2 ";

    // Perform column matching and use the respective columns
    map<string, string> cols1, cols2;   // map from unqualified column name to qualified col name
    cols1 = getColumns(tmp1->getName());
    cols2 = getColumns(tmp2->getName());
    map<string, string>::iterator it1;
    TRACE << "Columns of table '" << tmp1->getName() << "': ";
    for (it1 = cols1.begin(); it1 != cols1.end(); it1++)
        TRACE << " * " << it1->second << " ( " << it1->first << " )";
    TRACE << "Columns of table '" << tmp2->getName() << "': ";
    for (it1 = cols2.begin(); it1 != cols2.end(); it1++)
        TRACE << " * " << it1->second << " ( " << it1->first << " )";

    if (natural)
    {
        // Add the common columns to the query string
        query += " ON ";
        map<string, string>::iterator i;
        string col, sep = "";
        bool found = false;
        for (i = cols1.begin(); i != cols1.end(); i++)
        {
            col = (*i).first;
            if (cols2.count(col) > 0)
            {
                query +=  sep + "t1." + cols1[col] + " = t2." + cols2[col];
                sep = " and ";
                found = true;
            }
        }
        if (!found)
            throw string("Natural join requested, but no common columns could be found !");
    }
    else
    if (columns != NULL)
    {
        query += " ON ";
        string sep = "", colName = "";
        for (int i = 0; i < columns->size(); i++)
        {
            colName = *(columns->at(i));
            if (cols1.count(colName) != 1)
                throw string("Could not find exactly one match for column '" + colName
                        + "' in left table while performing JOIN.");
            if (cols2.count(colName) != 1)
                throw string("Could not find exactly one match for column '" + colName
                        + "' in right table while performing JOIN.");
            query += sep + "t1." + cols1[colName] + " = t2." + cols2[colName];
            sep = " and ";
        }
    }
    else
    if (cond != NULL)
    {
        /* First create the cartesian join between the two tables, and set it as the
         context for the execution of the JOIN. */
        string q = "DROP TABLE IF EXISTS tmp; ";
        q += "SELECT * INTO tmp FROM " + tmp1->getName() + " CROSS JOIN " + tmp2->getName();
        IqlTable *tmp3 = pg.query(q);
        delete tmp3;
        /* Insert the HQL internal ID, needed for the execution of the condition */
        pg.insertHqlIdToTable("tmp");
        q = "SELECT * FROM tmp";
        tmp3 = pg.query(q);
        tmp3->setName("tmp");
        QueryTree::getInstance().getRoot()->switchContextForJoin(tmp3);
        TRACE << tmp3;
        /* Now we can execute the theta condition independently. The columns from the
         condition will refer to the columns of the cartesian product, stored as the execution context. */
        tmp3 = cond->execute();
        string condTable = tmp3->getName();
        delete tmp3;
        tmp3 = pg.query("TABLE " + condTable + " LIMIT 1");
        string colCond = tmp3->getColumnNames().at(0);
        delete tmp3;
        query = "SELECT * INTO " + this->id + " FROM tmp JOIN " + condTable +
                " USING (" + HQL_COL + ") WHERE " + colCond;
        query += "; ALTER TABLE " + this->id + " DROP COLUMN " + HQL_COL;
        query += "; DROP TABLE tmp";
    }

    delete tmp1;
    delete tmp2;
    
    result = pg.query(query);
    result->setName(this->id);
    pg.addTempTable(this->id);

    return result;
}

string QtJoin::toString()
{
    string str = child1->toString() + " ";
    if (natural)
        str += " NATURAL ";
    str += type + " JOIN " + child2->toString();
    return str;
}

DbEnum QtJoin::setupDbSource()
{
    DbEnum e1, e2, result;
    e1 = child1->setupDbSource();
    e2 = child2->setupDbSource();
    result = e1;
    if (e1 == UNKNOWN_DB || e2 == UNKNOWN_DB)
        return UNKNOWN_DB;
    if (e1 == MIXED || e2 == MIXED)
        return MIXED;
    if (e1 == e2)
        return e1;
    else
        return MIXED;
}

void QtJoin::print(ostream& o, std::string indent)
{
    string jointype = type + " JOIN ";
    if (type == "")
        jointype = "";
    o << indent << "QtJoin (" << id << "): " << jointype <<  endl;
    o << indent << QTINDENT << "table 1: " << endl;
    child1->print(o, indent + QTINDENT + QTINDENT);
    o << indent << QTINDENT << "table 2: " << endl;
    child2->print(o, indent + QTINDENT + QTINDENT);
    if (columns)
    {
        o << indent << QTINDENT << "Join Columns:" << endl;
        for (int i = 0; i < columns->size(); i++)
            o << indent << QTINDENT << QTINDENT << " - " << columns->at(i) << endl;
    }
    if (cond)
    {
        o << indent << QTINDENT << "Join Condition:" << endl;
        cond->print(o, indent + QTINDENT + QTINDENT);
    }
}

map<string, string> QtJoin::getColumns(std::string tableName)
{
    PostgresDS &pg = IqlApp::getInstance().getSqlDataSource();
    IqlTable *tbl = pg.query("TABLE " + tableName);
    vector<string> qualifiedNames = tbl->getQualifiedColumnNames();

    map<string, string> result;
    string table, column;
    for (int i = 0; i < qualifiedNames.size(); i++)
        if (qualifiedNames[i] != HQL_COL)
        {
            // format: tableName_columnName
            int pos = qualifiedNames[i].find(IQL_TBL_COL_SEP);
            table = qualifiedNames[i].substr(0, pos);
            column = qualifiedNames[i].substr(pos + IQL_TBL_COL_SEP_SIZE);
            result[column] = qualifiedNames[i];
        }

    return result;
}
