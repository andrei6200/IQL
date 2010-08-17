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
    PostgresDS &pg = HqlMain::getInstance().getSqlDataSource();
    RasdamanDS &rman = HqlMain::getInstance().getRasqlDataSource();
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
        if (column == "*" && table == "")
        {
            HqlMain::getInstance().getSqlDataSource().insertData(prod, this->id);
            result = new HqlTable(this->id);
        }
        else if (column == "*" && table != "")
        {
            // We have the "table.*" syntax. Only select the columns from the referenced table.
            vector<string> names = prod->getColumnNames();
            vector<string> cols;
            for (int col = 0 ; col < names.size(); col++)
                if (names[col].find(table) == 0)
                    cols.push_back(names[col]);
            if (cols.size() == 0)
                throw string("Internal error: No columns could retrieved for table '"
                        + table + "' from The Cartesian Product");
            
            // Now execute the query
            string q = "SELECT " + cols[0];
            for (int i = 1; i < cols.size(); i++)
                q += ", " + cols[i];
            q += " INTO " + this->id + " FROM " + prod->getName();
            result = pg.query(q);
            result->setName(this->id, false);
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
                string suffix("_filename");
                // Start from 1 to skip the HQL id column
                for (int i = 1; i < names.size(); i ++)
                {
                    // Format for Rasdaman columns: table_oid | table_filename
                    if (names[i] == this->column + suffix)
                    {
                        TRACE << "Found Rasdaman column '" << this->column << "' under the name: " << names[i];
                        colCount ++;
                        col = names[i];
                        table = names[i].substr(0, names[i].size() - suffix.size());
                    }
                    // Format for Postgres columns : table_column
                    if (names[i].rfind(this->column) == names[i].size() - this->column.size())
                    {
                        TRACE << "Found Postgres column '" << this->column << "' under the name: " << names[i];
                        colCount ++;
                        col = names[i];
                        table = names[i].substr(0, names[i].size() - this->column.size() - 1);
                    }
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

            
            string q;

            switch (db_source)
            {
                case POSTGRES:
                    q = "SELECT " + col + " FROM " + prod->getName();
                    // Select just the necessary column
                    result = pg.query(q);
                    pg.insertData(result, this->id);
                    result->setName(this->id);
                    TRACE << "Result table " << result;
                    break;

                case RASDAMAN:
                    /* Setup the SQL table */
                    string col1 = table + "_oid";
                    string col2 = table + "_filename";
                    q = "SELECT " + col1 + ", " + col2 + " FROM " + prod->getName();
                    result = pg.query(q);
                    result->setName(this->id, false);
                    // And store them in Postgres as well
                    pg.insertData(result, this->id);

                    /* But also create the RaSQL collection with the right number of rows */
                    vector<string> oids = result->getColumn(col1);
                    for (int row = 0 ; row < oids.size(); row ++)
                    {
                        string oid = oids[row];
                        q = "SELECT " + table + " INTO " + this->id + " FROM " + table
                                + " WHERE oid(" + table + ") = <\"" + oid + "\">";
                        rman.updateQuery(q);
                    }
                    rman.addTempTable(this->id);
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
