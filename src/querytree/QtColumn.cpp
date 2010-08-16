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
        string q = "SELECT * INTO " + id + " FROM " + tmp->getName();
        TRACE << "Going to execute: " << q;
        // FIXME: execute the query
    }
    else
    {
        HqlTable *prod = QueryTree::getInstance().getRoot()->getCartesianProduct();
        /* If we select all columns, then just copy the output of the "FROM" clause */
        if (column == "*")
        {
            HqlMain::getInstance().getSqlDataSource().insertData(prod, this->id);
            result = new HqlTable(this->id);
        }
        else
        if (child == NULL)
        {
            // The column names are prefixed with the table name
            string col = table;
            if (table != "")
                col += "_" + this->column;
            else
            {
                /* search for the source table */
                TRACE << "Searching for column: " << this->column;
                vector<string> names = prod->getColumnNames();
                int colCount = 0;
                // Start from 1 to skip the HQL id column
                for (int i = 1; i < names.size(); i ++)
                    if (
                            // Format for Postgres columns : table_column
                            names[i].rfind(this->column) == names[i].size() - this->column.size()
                            ||
                            // Format for Rasdaman columns: table_oid | table_filename
                            names[i] == this->column + "_filename"
                        )
                    {
                        TRACE << "Found column '" << this->column << "' under the name: " << names[i];
                        colCount ++;
                        col = names[i];
                    }
                switch (colCount)
                {
                    case 0:
                        throw string("Column '" + this->column + "' could not be found.");
                    case 1:
                        // OK
                        break;
                    default:
                        throw string("Too many columns named '" + this->column +
                                "' found in the source tables.");
                }
            }
            // Select just the one column
            PostgresDS &pg = HqlMain::getInstance().getSqlDataSource();
            string q = "SELECT " + col + " INTO " + id + " FROM " + prod->getName();
            result = pg.query(q);
            pg.addTempTable(this->id);
            result->setName(this->id);
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
        WARN << "Could not find table '" << table <<
                "' in global table dictionary";
        db_source = UNKNOWN_DB;
        break;
    case 1:
        DEBUG << "Found String " << table << " as a table.";
        DEBUG << "\tDatabase source: " << tables[table];
        db_source = tables.at(table);
        break;
    default:
        TRACE << "Found " << tables.count(table) << " occurrences of '" << table
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
