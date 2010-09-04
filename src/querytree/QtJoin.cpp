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
#include "HqlMain.hpp"

QtJoin::QtJoin(QtNode* table1, QtNode* table2)
    : child1(table1), child2(table2), type(""), natural(false)
{
}

QtJoin::QtJoin(QtNode* table1, QtNode* table2, char *jointype)
    : child1(table1), child2(table2), type(jointype), natural(false)
{
}

QtJoin::QtJoin(QtNode* table1, QtNode* table2, char* jointype, bool naturaljoin)
    : child1(table1), child2(table2), type(jointype), natural(naturaljoin)
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
}

HqlTable* QtJoin::execute()
{
    PostgresDS &pg = HqlMain::getInstance().getSqlDataSource();
    
    HqlTable *result = NULL, *tmp1 = NULL, *tmp2 = NULL;
    tmp1 = child1->execute();
    tmp2 = child2->execute();

    string query = "SELECT * INTO " + this->id + " FROM " +
            tmp1->getName() + " AS t1 ";
    query += type + " JOIN " + tmp2->getName() + " AS t2 ";
    if (natural)
    {
        // Perform column matching and use the respective columns
        map<string, string> cols1, cols2;   // map from column name to table name
        cols1 = getColumns(tmp1->getName());
        cols2 = getColumns(tmp2->getName());
        map<string, string>::iterator it1;
        TRACE << "Columns of table '" << tmp1->getName() << "': ";
        for (it1 = cols1.begin(); it1 != cols1.end(); it1++)
            TRACE << " * " << it1->first << " ( " << it1->second << " )";
        TRACE << "Columns of table '" << tmp2->getName() << "': ";
        for (it1 = cols2.begin(); it1 != cols2.end(); it1++)
            TRACE << " * " << it1->first << " ( " << it1->second << " )";

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
                query +=  "t1." + cols1[col] + "_" + col + " = " +
                        "t2." + cols2[col] + "_" + col + sep;
                sep = ", ";
                found = true;
            }
        }
        if (!found)
            throw string("Natural join requested, but no common columns could be found !");
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
}

map<string, string> QtJoin::getColumns(std::string tableName)
{
    PostgresDS &pg = HqlMain::getInstance().getSqlDataSource();
    HqlTable *tbl = pg.query("TABLE " + tableName);
    vector<string> qualifiedNames = tbl->getQualifiedColumnNames();

    map<string, string> result;
    string table, column;
    for (int i = 0; i < qualifiedNames.size(); i++)
        if (qualifiedNames[i] != HQL_COL)
        {
            // format: tableName_columnName
            int pos = qualifiedNames[i].find("_");
            table = qualifiedNames[i].substr(0, pos);
            column = qualifiedNames[i].substr(pos+1);
            result[column] = table;
        }

    return result;
}
