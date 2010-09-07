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
            string q = "SELECT * INTO " + this->id + " FROM " + prod->getName();
            result = HqlMain::getInstance().runSqlQuery(q);
            result->setName(this->id);
            pg.addTempTable(this->id);
        }
        else if (column == "*" && table != "")
        {
            // We have the "table.*" syntax. Only select the columns from the referenced table.
            vector<string> names = prod->getQualifiedColumnNames();
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
            q += string(", ") + HQL_COL + " INTO " + this->id + " FROM " + prod->getName();
            result = pg.query(q);
            result->setName(this->id, false);
            pg.addTempTable(this->id);
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
                vector<string> names = prod->getQualifiedColumnNames();
                int colCount = 0;
                string suffix("_filename");
                for (int i = 0; i < names.size(); i ++)
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
                    if (names[i].rfind("_" + this->column) == names[i].size() - this->column.size() - 1)
                    {
                        TRACE << "Found Postgres column '" << this->column << "' under the name: " << names[i];
                        colCount ++;
                        col = names[i];
                        table = names[i].substr(0, names[i].size() - this->column.size() - 1);
                    }
                }
//                if (colCount == 0)
//                {
//                    // Try to search the alias dictionary
//                    try
//                    {
//                        table = QueryTree::getInstance().getAliasFromTable(this->column);
//                        TRACE << "Found table '" << table << "' for the alias: " << this->column;
//                        col = table;
//                        colCount ++;
//                    }
//                    catch (std::out_of_range)
//                    {
//                        TRACE << "No alias '" << this->column << "' exists.";
//                    }
//                }
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
                    q = "SELECT " + col + ", " + HQL_COL + " INTO " + this->id + " FROM " + prod->getName();
                    // Select just the necessary column
                    result = pg.query(q);
                    result->setName(this->id);
                    pg.addTempTable(this->id);
                    TRACE << "Result table " << result;
                    break;

                case RASDAMAN:
//                    /* Setup the SQL table */
//                    string col1 = table + "_oid", alias1 = " AS " + this->id + "_oid";
//                    string col2 = table + "_filename", alias2 = " AS " + this->id + "_filename";
//                    q = "SELECT " + col1 + alias1 + " , " + col2 + alias2 + ", " + HQL_COL + " INTO "
//                            + this->id +  " FROM " + prod->getName();
//                    result = pg.query(q);
//                    pg.addTempTable(this->id);
//                    /* Fetch a copy in memory */
//                    delete result;
                    q = "SELECT " + table + "_oid FROM " + prod->getName();
                    result = pg.query(q);
                    result->setName(this->id);

                    /* Create the RaSQL collection with the right number of rows */
                    vector<string> oids = result->getColumn(table + "_oid");
                    for (int row = 0 ; row < oids.size(); row ++)
                    {
                        string oid = oids[row];
                        q = "SELECT " + table + " INTO " + this->id + " FROM " + table
                                + " WHERE oid(" + table + ") = <\"" + oid + "\">";
                        rman.updateQuery(q);
                    }
                    rman.addTempTable(this->id);

                    /* Retrieve the OIDs into memory */
                    result = rman.getCollection(this->id, false, true);
                    // here i could rename the local ID into the original collection name
//                    result->setName(table, true);
                    /* And setup the corresponding SQL table with the proper OIDs */
                    pg.insertData(result, this->id);
                    pg.insertHqlIdToTable(this->id);
                    
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

    string search = table, newsearch;
    if (table == "")
        search = column;

//    /* First check if this is an alias */
//    try
//    {
//        newsearch = QueryTree::getInstance().getTableFromAlias(search);
//        TRACE << "Using alias '" << newsearch << "' for table '" << search << "'";
//        search = newsearch;
//    }
//    catch (std::out_of_range)
//    {
//
//    }

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
