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
                        table = names[i].substr(0, names[i].size() - this->column.size() - 1);
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

            setupDbSource();

            PostgresDS &pg = HqlMain::getInstance().getSqlDataSource();
            RasdamanDS &rman = HqlMain::getInstance().getRasqlDataSource();
            string q = "SELECT " + col + " INTO " + id + " FROM " + prod->getName();

            switch (db_source)
            {
                case POSTGRES:
                    // Select just the one column
                    result = pg.query(q);
                    pg.addTempTable(this->id);
                    result->setName(this->id);

                    break;

                case RASDAMAN:
                    q = "SELECT " + column + " INTO " + id + " FROM " + column;
                    // Also insert the new coverage into Rasdaman
                    rman.updateQuery(q);
                    rman.addTempTable(this->id);
                    // And retrieve the new OIDs
                    result = rman.getCollection(this->id, false);
                    // And store them in Postgres as well
                    pg.insertData(result, this->id);
                    break;
            }
        }
    }

    return result;
}

string QtColumn::toString()
{
    string str(table);
    if (table.size() > 0)
        str += ".";
    str += column;
    return str;
}

DbEnum QtColumn::setupDbSource()
{
    TRACE << toString();
    
    /* Check the source tables. */
    if (column == "*")
    {
        QtList *from = QueryTree::getInstance().getRoot()->getSourceTables();
        db_source = from->setupDbSource();
        return db_source;
    }

    string search = table;
    if (table == "")
        search = column;


    /* Else try to use the global data source dictionary. */
    map<string, DbEnum> tables = HqlMain::getInstance().tableMap;
    switch (tables.count(search))
    {
    case 0:
        WARN << "Could not find table '" << search <<
                "' in global table dictionary";
        db_source = UNKNOWN_DB;
        break;
    case 1:
        DEBUG << "Found String " << search << " as a table.";
        DEBUG << "\tDatabase source: " << tables[search];
        db_source = tables.at(search);
        break;
    default:
        TRACE << "Found " << tables.count(search) << " occurrences of '" << search
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
