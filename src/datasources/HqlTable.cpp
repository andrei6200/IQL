/*
 * File:   HqlTable.cpp
 * Author: andrei
 *
 * Created on June 18, 2010, 4:02 PM
 */

#include <vector>
#include <string>
#include <iomanip>
#include <iostream>
#include <exception>
#include <sstream>


// Uncomment to print hidden columns of tables
#define PRINT_HIDDEN_COLUMNS

// Printing helpers
#define TABLE_COL_SEPARATOR     " | "
#define TABLE_HEADER_SEPARATOR  '-'




#include "HqlTable.hpp"
#include "config.hpp"
#include "logger.hpp"
#include "RasdamanDS.hpp"
#include "PostgresDS.hpp"
#include "HqlMain.hpp"


using namespace std;
using namespace pqxx;


HqlTable::HqlTable(storageType type)
    : rows(0), columns(0), hiddenCount(0), storage(type),
        tableName(""), dataAlsoInMemory(false)
{
    TRACE << "Created HqlTable '" << tableName << "' with " << rows << " rows and "
            << columns - hiddenCount << " columns ( +" << hiddenCount << " hidden)";
//    string id("_hql_id");
    names = vector<string>();
//    names.push_back(id);
    widths = vector<int>();
//    widths.push_back(id.length());
    hidden = vector<bool>();
//    hidden.push_back(true);
    data = vector<vector<string> >();
}


HqlTable::HqlTable(string name)
    : rows(0), columns(0), hiddenCount(0), storage(POSTGRES),
        tableName(name), dataAlsoInMemory(false)
{
    TRACE << "Created HqlTable '" << tableName << "' with " << rows << " rows and "
            << columns - hiddenCount << " columns ( +" << hiddenCount << " hidden)";
//    string id("_hql_id");
    names = vector<string>();
//    names.push_back(id);
    widths = vector<int>();
//    widths.push_back(id.length());
    hidden = vector<bool>();
//    hidden.push_back(true);
    data = vector<vector<string> >();
}

/* Import data from the result of a RaSQL query. */
void HqlTable::importFromRasql(r_Set<r_Ref_Any> *resultSet, bool storeOnDisk)
{
    INFO << "Import from Rasdaman result ...";

    rows = resultSet->cardinality();
    columns += 2;

    string colname("filename");
    string oid("oid");
    names.push_back(oid);
    names.push_back(colname);
    widths.push_back(oid.length());
    widths.push_back(colname.length());
    hidden.push_back(true);
    hidden.push_back(false);
    hiddenCount ++;

    vector<string> row;

    if (storeOnDisk == false)
        DEBUG << "No file will be created on disk, this is an intermediate result table.";

    r_Iterator< r_Ref_Any > iter = resultSet->create_iterator();
    // iter.not_done() seems to behave wrongly on empty set, therefore this additional check -- PB 2003-aug-16
    for (int i = 1; i <= resultSet->cardinality() && iter.not_done(); iter++, i++)
    {
        stringstream cell (stringstream::out);
        switch (resultSet->get_element_type_schema()->type_id())
        {
            case r_Type::MARRAYTYPE:
            {
                cell <<
                        HqlMain::getInstance().getRasqlDataSource().
                        saveRasdamanMddToFile(r_Ref<r_GMarray> (*iter), storeOnDisk, i);
                break;
            }

            case r_Type::POINTTYPE:
                TRACE << "  Point Result element " << i << ": ";

                cell << *(r_Ref<r_Point > (*iter));
                break;

            case r_Type::SINTERVALTYPE:
                TRACE << "  SInterval Result element " << i << ": ";

                cell << *(r_Ref<r_Sinterval > (*iter));
                break;

            case r_Type::MINTERVALTYPE:
                TRACE << "  MInterval Result element " << i << ": ";

                cell << *(r_Ref<r_Minterval > (*iter));
                break;

            case r_Type::OIDTYPE:
                TRACE << "  OID Result element " << i << ": ";

                cell << *(r_Ref<r_OId > (*iter));
                break;

            default:
                TRACE << "  Scalar Result element " << i << ": " << flush;
                
                cell << *(r_Ref<r_Scalar > (*iter));
                break;
        } // switch


        row.clear();

        /* Add the ObjectID (OID) of the current result to the table */
        r_Ref_Any ref = *iter;
        stringstream stroid(stringstream::out);
        stroid << ref.get_oid();
        row.push_back(stroid.str());

        /* Add the value of the current cell to the table */
        TRACE << " Cell value: " << cell.str();
        row.push_back(cell.str());

        data.push_back(row);
    } // for(...)

    dataAlsoInMemory = true;

    INFO << "Import successful from Rasdaman result.";
    TRACE << "Updated HqlTable '" << tableName << "' with " << rows << " rows and "
            << columns - hiddenCount << " columns ( +" << hiddenCount << " hidden)";
}


/* Import data from the result of an SQL query. */
void HqlTable::importFromSql(result sqlResult)
{
    TRACE << "Importing from Postgres result with " << sqlResult.size() 
            << " rows and " << sqlResult.columns() << " columns ... ";
    
    rows = sqlResult.size();
    int newcols = sqlResult.columns();
    int startCol = 0;
    columns += newcols;

    if (sqlResult.size() > 0)
    {

        vector<bool> newhidden = vector<bool>(newcols, false);
        newhidden[0] = true;

        vector<string> newnames = vector<string > (newcols, "");
        for (int i = 0; i < newcols; i++)
        {
            newnames[i] = sqlResult.column_name(i);
            // Hide OIDs
            if (newnames[i].rfind("_oid") == newnames[i].length() - 4)
            {
                newhidden[i] = true;
                hiddenCount ++;
            }
        }

        result::const_iterator it;
        vector<string> row;
        for (it = sqlResult.begin(); it != sqlResult.end(); it++)
        {
            row.clear();
            // Insert actual data
            result::tuple tuple = *it;
            string val;
            for (int col = startCol; col < tuple.size(); col++)
            {
                val = tuple[col].as(string());
                row.push_back(val);
            }
            data.push_back(row);
        }

        // Append the new data
        hidden.insert(hidden.end(), newhidden.begin(), newhidden.end());
        names.insert(names.end(), newnames.begin(), newnames.end());

    }

    dataAlsoInMemory = true;

    TRACE << "Import from Postgres successful. ";
    TRACE << "Updated HqlTable '" << tableName << "' with " << rows << " rows and "
            << columns - hiddenCount << " columns ( +" << hiddenCount << " hidden)";
}

vector<string> HqlTable::getColumn(int index)
{
    vector<string> vec;
    for (int i = 0; i < rows; i++)
        vec.push_back(data[i][index]);
    return vec;
}

vector<string> HqlTable::getColumn(string name)
{
    int index = -1, count = 0;
    for (int i = 0; i < columns; i ++)
        if (names[i] == name)
        {
            index = i;
            count ++;
        }

    switch (count)
    {
        case 0:
            throw string("Column with name '" + name + "' could not found.");
        case 1:
            return getColumn(index);
        default:
            throw string("Too many columns with name: '" + name + "'");
    }

    return vector<string>();
}


HqlTable::~HqlTable()
{
	TRACE << "Destroyed table. ";
}

/* Print the table contents to a specified stream. */
void HqlTable::print(ostream &out)
{
//    TRACE << "HqlTable instance '" << tableName << "':";
//    TRACE << " * " << rows << " rows";
//    TRACE << " * " << columns - hiddenCount << " columns ( +" << hiddenCount << " hidden )";
//    TRACE;

    if (dataAlsoInMemory == false)
    {
        out << INDENT_PROMPT << "Internal error: Table " << tableName << "is not in memory. " << endl;
        return;
    }

    updateWidths();

    out << endl << INDENT_PROMPT << " --- Table '" << tableName << "' ---" << endl;

    if (columns > 0)
    {
        string sep = INDENT_PROMPT;
        /* First print the column names */
        int l = 0;
        for (int i = 0; i < columns; i++)
#ifndef PRINT_HIDDEN_COLUMNS
            if (hidden[i] == false)
#endif
            {
                l = names[i].size();
                out << sep << setw((widths[i]+l)/2) << names[i]
                        << setw(widths[i] - (widths[i]+l)/2) << "";
                sep = TABLE_COL_SEPARATOR;
            }
        out << endl;
        /* And the separator line ...*/
        sep = INDENT_PROMPT;
        char newsep[5];
        sprintf(newsep, "%c%s%c", TABLE_HEADER_SEPARATOR, "+", TABLE_HEADER_SEPARATOR);
        for (int i = 0; i < columns; i++)
#ifndef PRINT_HIDDEN_COLUMNS
            if (hidden[i] == false)
#endif
            {
                out << sep << setw(widths[i]) << setfill(TABLE_HEADER_SEPARATOR) << "";
                sep = newsep;
            }
        out << setfill(' ');
    }

    if (rows > 0)
    {
        out << left;
        /* Now print the actual data*/
        vector<string> row;
        int l = 0;
        for (int r = 0; r < rows; r++)
        {
            string sep = INDENT_PROMPT;
            out << endl;
            row = data[r];
            for (int i = 0; i < row.size(); i++)
#ifndef PRINT_HIDDEN_COLUMNS
                if (hidden[i] == false)
#endif
                {
                    l = row[i].size();
                    out << sep << setw(widths[i]) << row[i];
                    sep = TABLE_COL_SEPARATOR;
                }
        }
        out << right;

    }

    // If no rows in the result, then just print "0 rows".
    out << endl << INDENT_PROMPT << "( " 
            << rows << " row(s) , "
            << columns - hiddenCount << " column(s) "
#ifdef PRINT_HIDDEN_COLUMNS
            << ", " << hiddenCount << " hidden column(s) "
#endif
            << ")" << endl;
}


//HqlTable* HqlTable::crossProduct(HqlTable* other)
//{
////    TRACE << "Cross product between two HqlTables." << endl
////            << this << endl << endl << other << endl;
//
//    HqlTable *output = new HqlTable();
//
//    output->hidden = this->hidden;
//    vector<bool> h2 = other->hidden;
//    output->hidden.insert(output->hidden.end(), h2.begin(), h2.end());
//    output->hiddenCount = this->hiddenCount + other->hiddenCount;
//    output->columns = this->columns + other->columns;
//    output->rows = this->rows * other->rows;
//
//    int r1, r2;
//    /* Copy column names */
//    output->names = this->names;
//    vector<string> names2 = other->names;
//    output->names.insert(output->names.end(), names2.begin(), names2.end());
//
//    /* Compute the actual cross product */
//    resetId();
//    vector<string> row, row2;
//    for (r1 = 0; r1 < this->rows; r1++)
//        for (r2 = 0; r2 < other->rows; r2++)
//        {
//            row = this->data[r1];
//            row2 = other->data[r2];
//            /* Update the IDs */
//            row.insert(row.end(), row2.begin(), row2.end());
//            output->data.push_back(row);
////            TRACE << "Just inserted row: ";
////            vector<string>::iterator i;
////            for (i = row.begin(); i != row.end(); i++)
////                TRACE << "- " << *i;
//        }
//
//    output->dataAlsoInMemory = true;
//
//    TRACE << "Cross product has been evaluated.";
//
//    return output;
//}
//
//
//HqlTable* HqlTable::addColumns(HqlTable* other)
//{
////    TRACE << "Column addition between two HqlTables: " << endl <<
////            this << endl << endl << other << endl;
//
//    if (this->rows != other->rows)
//    {
//        ERROR << "The two tables have different number of rows ! Cannot combine the columns !";
//        throw string("Two intermediate tables have different number of rows ! Cannot combine the columns !");
//    }
//
//    vector<bool> h2 = other->hidden;
//    this->hidden.insert(this->hidden.end(), h2.begin(), h2.end());
//    this->hiddenCount += other->hiddenCount;
//    this->columns += other->columns;
//    TRACE << "Result table has " << columns - hiddenCount << " columns ( + "
//            << hiddenCount << " hidden ) and " << hidden.size() << " elements in the 'hidden' vector";
//
//    /* Copy column names */
//    vector<string> names2 = other->names;
//    this->names.insert(this->names.end(), names2.begin(), names2.end());
//
//    /* Copy the data */
//    for (int row = 0; row < this->rows; row++)
//    {
//        vector<string> otherrow = other->data[row];
//        this->data[row].insert(this->data[row].end(), otherrow.begin(), otherrow.end());
////        TRACE << "Just inserted row: ";
////        vector<string>::iterator i;
////        for (i = otherrow.begin(); i != otherrow.end(); i++)
////            TRACE << "- " << *i;
////        TRACE << "Combined row: ";
////        for (i = data[row].begin(); i != data[row].end(); i++)
////            TRACE << "- " << *i;
//    }
//
//    TRACE << "Column addition has been evaluated.";
//
//    return this;
//}
//
//
//string HqlTable::generateId()
//{
//    stringstream stream;
//    stream << ++lastId;
//    return stream.str();
//}
//
//void HqlTable::resetId()
//{
//    lastId = 0;
//}

void HqlTable::updateWidths()
{
    if (columns == 0)
        return;

    widths.clear();
    widths.assign(columns, 0);

    vector<vector<string> >::iterator row;
    for (row = data.begin(); row != data.end(); row++)
    {
        for (int col = 0; col < row->size(); col++)
            if (row->at(col).length() > widths[col])
                widths[col] = row->at(col).length();
    }

    for (int col = 0; col < names.size(); col++)
        if (names[col].length() > widths[col])
            widths[col] = names[col].length();

    TRACE << "Table widths:";
    for (int col = 0; col < names.size(); col++)
        TRACE << " Column '" << names[col] << "' : " << widths[col];
    TRACE;
}

std::ostream& operator << (std::ostream &o, HqlTable *a)
{
    a->print(o);
    return o;
}

void HqlTable::setName(string name, bool updateColumnNames)
{
    tableName = name;
    // Prefix the field names with the table name, if possible
    if (updateColumnNames && this->dataAlsoInMemory)
    {
        for (int i = 0; i < names.size(); i ++)
            if (names[i].find(name) != 0)
                names[i] = tableName + "_" + names[i];
    }
}

string HqlTable::getName()
{
    return tableName;
}

vector<string> HqlTable::getColumnNames()
{
    return names;
}

void HqlTable::setFilenames(vector<string> values, int colIndex)
{
    if (colIndex < 0 || colIndex >= columns)
        throw string("Table column index out of range. ");
    
    string s("_filename");
    if (names.at(colIndex).rfind(s) != names.at(colIndex).size() - s.size())
        throw string("Internal Error: Setting non-filename columns is not "
                "allowed for HqlTable. ");
    for (int row = 0; row < rows; row++)
        data[row][colIndex] = values[row];
}

vector<vector<string> > HqlTable::getData()
{
    if (dataAlsoInMemory == false)
        WARN << "Trying to retrieve data for HqlTable, but table was not retrieved in memory.";

    return data;
}