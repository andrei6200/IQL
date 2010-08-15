/* 
 * File:   QtColumn.cpp
 * Author: andrei
 * 
 * Created on August 14, 2010, 12:04 PM
 */

#include "QtColumn.hpp"
#include "HqlMain.hpp"
#include "QueryTree.hpp"

QtColumn::QtColumn(char *col)
    : column(col), table(""), child(NULL)
{
}

QtColumn::QtColumn(char* tbl, char* col)
    : column(col), table(tbl), child(NULL)
{
}

QtColumn::QtColumn(QtNode* node, char* col)
    : column(col), table(""), child(node)
{
}

QtColumn::~QtColumn()
{
}

HqlTable* QtColumn::execute()
{
    HqlTable *result = NULL, *tmp = NULL;

    QtList *from = QueryTree::getInstance().getRoot()->getSourceTables();
    
    
    
    if (child != NULL)
    {
        tmp = child->execute();
        // Result table should have one column ( + one HQL id hidden column )
//        if (tmp->columns() != 2)
//            throw string("Intermediate query result should contain only one column !");
        string q = "SELECT * INTO " + id + " FROM " + tmp->getName();
        TRACE << "Going to execute: " << q;
    }
    else
    {
        /* If we select all columns, then just copy the output of the "FROM" clause */
        if (column == "*")
        {
            result = from->multiplyResults();
            HqlMain::getInstance().getSqlDataSource().insertData(result, this->id);
            result->setName(this->id);
        }
        else
        if (child == NULL)
        {
            result = from->multiplyResults();
            // Select just one column
            string q = "SELECT " + column + " INTO " + id + " FROM " + result->getName();
            tmp = HqlMain::getInstance().runSqlQuery(q);
            delete tmp;
            result = new HqlTable(id);
        }
    }
        
    return result;
}

string QtColumn::toString()
{
    string str(table);
    if (table != "")
        str += ".";
    str += column;
    return str;
}

DbEnum QtColumn::setupDbSource()
{
    /* Check the source tables. */
    if (column == "*")
    {
        QtList *from = QueryTree::getInstance().getRoot()->getSourceTables();
        db_source = from->setupDbSource();
        return db_source;
    }
    
    /* Else try to use the global data source dictionary. */
    map<string, DbEnum> tables = HqlMain::getInstance().tableMap;
    switch (tables.count(table))
    {
    case 0:
        WARN << "QtString::setupDbSource(): Could not find table '" << table <<
                "' in global table dictionary";
        db_source = UNKNOWN_DB;
        break;
    case 1:
        DEBUG << "QtString::setupDbSource(). Found String " << table << " as a table.";
        DEBUG << "\tDatabase source: " << tables[table];
        db_source = tables.at(table);
        break;
    default:
        TRACE << "QtString: Found " << tables.count(table) << " occurrences of '" << table
                << "' inside the global table dictionary!";
        db_source = UNKNOWN_DB;
        break;
    }

    return db_source;
}

void QtColumn::print(ostream &o, std::string indent)
{
    o << indent << "QtColumn (" << id << "): ";
    if (table != "")
        o << table << ".";
    o << column << endl;
}
