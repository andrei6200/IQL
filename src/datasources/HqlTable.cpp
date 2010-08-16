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
//#define PRINT_HIDDEN_COLUMNS

// Printing helpers
#define TABLE_COL_SEPARATOR     " | "
#define TABLE_HEADER_SEPARATOR  '-'

// Filename pattern, for Rasdaman coverages stored on disk
#define DEFAULT_OUTFILE_MASK "hql_%d"



#include "HqlTable.hpp"
#include "config.hpp"
#include "logger.hpp"
#include "RasdamanDS.hpp"
#include "PostgresDS.hpp"
#include "HqlMain.hpp"


using namespace std;
using namespace pqxx;


HqlTable::HqlTable(storageType type)
    : rows(0), columns(1), hiddenCount(1), lastId(0), storage(type), 
        tableName(""), dataAlsoInMemory(false)
{
    TRACE << "Created HqlTable '" << tableName << "' with " << rows << " rows and "
            << columns - hiddenCount << " columns ( +" << hiddenCount << " hidden)";
    string id("_hql_id");
    names = vector<string>();
    names.push_back(id);
    widths = vector<int>();
    widths.push_back(id.length());
    hidden = vector<bool>();
    hidden.push_back(true);
    data = vector<vector<string> >();
}


HqlTable::HqlTable(string name)
    : rows(0), columns(1), hiddenCount(1), lastId(0), storage(POSTGRES),
        tableName(name), dataAlsoInMemory(false)
{
    TRACE << "Created HqlTable '" << tableName << "' with " << rows << " rows and "
            << columns - hiddenCount << " columns ( +" << hiddenCount << " hidden)";
    string id("_hql_id");
    names = vector<string>();
    names.push_back(id);
    widths = vector<int>();
    widths.push_back(id.length());
    hidden = vector<bool>();
    hidden.push_back(true);
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
                char defFileName[FILENAME_MAX];
                (void) snprintf(defFileName, sizeof (defFileName) - 1, DEFAULT_OUTFILE_MASK, i);
//                TRACE << "filename for #" << i << " is: " << defFileName;

                // special treatment only for DEFs
                r_Data_Format mafmt = r_Ref<r_GMarray > (*iter)->get_current_format();
                switch (mafmt)
                {
                    case r_TIFF:
                        strcat(defFileName, ".tif");
                        break;
                    case r_JPEG:
                        strcat(defFileName, ".jpg");
                        break;
                    case r_HDF:
                        strcat(defFileName, ".hdf");
                        break;
                    case r_PNG:
                        strcat(defFileName, ".png");
                        break;
                    case r_BMP:
                        strcat(defFileName, ".bmp");
                        break;
                    case r_VFF:
                        strcat(defFileName, ".vff");
                        break;
                    default:
                        strcat(defFileName, ".unknown");
                        break;
                }

                if (storeOnDisk)
                {
                    DEBUG << "  MDD Result object " << i << ": going into file " << defFileName << "..." << flush;
                    FILE *tfile = fopen(defFileName, "wb");
                    char* output = r_Ref<r_GMarray > (*iter)->get_array();
                    size_t size = r_Ref<r_GMarray > (*iter)->get_array_size();
                    if (size == fwrite((void*) output, sizeof(char), size, tfile))
                        TRACE << "ok." << endl;
                    else
                        TRACE << "could not write data into file !" << flush;
                    fclose(tfile);

                    cell << defFileName;
                }
                else
                    // Do not enter filenames into table if file does not exist
                    cell << "";
                
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
        // First generate ID of current row
        row.push_back(generateId());

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
    TRACE << "Importing from Postgres result ... ";
    
    rows = sqlResult.size();
    int newcols = sqlResult.columns();
    int startCol = 0;
    columns += newcols;

    vector<bool> newhidden = vector<bool>(newcols, false);
    newhidden[0] = true;

    vector<string> newnames = vector<string > (newcols, "");
    for (int i = 0; i < newcols; i++)
    {
        newnames[i] = sqlResult.column_name(i);
    }

    // If this is an HQL table, then the first column is "_hql_id" and should be hidden
    if (string(sqlResult.column_name(0)) == string("_hql_id"))
    {
        columns --;
        startCol = 1;
        newnames.erase(newnames.begin());
    }

    if (sqlResult.size() == 0)
        return;

    result::const_iterator it;
    vector<string> row;
    // FIXME: clear the table before the import ? 
//    data.clear();
    for (it = sqlResult.begin(); it != sqlResult.end(); it++)
    {
        row.clear();
        // Insert row ID
        row.push_back(generateId());
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
    hidden.insert(hidden.end(), newcols, false);
    names.insert(names.end(), newnames.begin(), newnames.end());

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


    out << endl << INDENT_PROMPT << " --- Table '" << tableName << "' ---" << endl;

    if (dataAlsoInMemory == false)
    {
        out << "Table is stored only in Postgresql SQL data source. ";
        return;
    }

    updateWidths();

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
                if ((widths[i]+l) % 2 == 1 xor (widths[i]-l) % 2 == 1)
                    out << " ";
                out << sep << setw((widths[i]+l)/2) << names[i]
                        << setw((widths[i]-l)/2) << "";
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


HqlTable* HqlTable::crossProduct(HqlTable* other)
{
    TRACE << "Cross product between two HqlTables." << endl
            << this << endl << endl << other << endl;

    HqlTable *output = new HqlTable();

    output->hidden = this->hidden;
    vector<bool> h2 = other->hidden;
    h2.erase(h2.begin());
    output->hidden.insert(output->hidden.end(), h2.begin(), h2.end());
    // "-1" so that we do not count the HQL id column twice
    output->hiddenCount = this->hiddenCount + other->hiddenCount - 1;
    output->columns = this->columns + other->columns - 1;
    output->rows = this->rows * other->rows;

    int r1, r2;
    /* Copy column names */
    output->names = this->names;
    vector<string> names2 = other->names;
    names2.erase(names2.begin());   // erase the "HQL ID" column
    output->names.insert(output->names.end(), names2.begin(), names2.end());

    /* Compute the actual cross product */
    resetId();
    vector<string> row, row2;
    for (r1 = 0; r1 < this->rows; r1++)
        for (r2 = 0; r2 < other->rows; r2++)
        {
            row = this->data[r1];
            row2 = other->data[r2];
            /* Update the IDs */
            row.erase(row.begin());
            row2.erase(row2.begin());
            row.insert(row.begin(), generateId());
            row.insert(row.end(), row2.begin(), row2.end());
            output->data.push_back(row);
//            TRACE << "Just inserted row: ";
//            vector<string>::iterator i;
//            for (i = row.begin(); i != row.end(); i++)
//                TRACE << "- " << *i;
        }

    output->dataAlsoInMemory = true;

    TRACE << "Cross product has been evaluated.";

    return output;
}


HqlTable* HqlTable::addColumns(HqlTable* other)
{
    TRACE << "Column addition between two HqlTables: " << endl <<
            this << endl << endl << other << endl;

    if (this->rows != other->rows)
    {
        ERROR << "The two tables have different number of rows ! Cannot combine the columns !";
        throw string("Two intermediate tables have different number of rows ! Cannot combine the columns !");
    }

    vector<bool> h2 = other->hidden;
    h2.erase(h2.begin());
    this->hidden.insert(this->hidden.end(), h2.begin(), h2.end());
    // "-1" so that we do not count the HQL id column twice
    this->hiddenCount += other->hiddenCount - 1;
    this->columns += other->columns - 1;
    TRACE << "Result table has " << columns - hiddenCount << " columns ( + "
            << hiddenCount << " hidden ) and " << hidden.size() << " elements in the 'hidden' vector";
    
    /* Copy column names */
    vector<string> names2 = other->names;
    names2.erase(names2.begin());   // erase the "HQL ID" column
    this->names.insert(this->names.end(), names2.begin(), names2.end());

    /* Copy the data */
    for (int row = 0; row < this->rows; row++)
    {
        vector<string> otherrow = other->data[row];
        otherrow.erase(otherrow.begin());
        this->data[row].insert(this->data[row].end(), otherrow.begin(), otherrow.end());
//        TRACE << "Just inserted row: ";
//        vector<string>::iterator i;
//        for (i = otherrow.begin(); i != otherrow.end(); i++)
//            TRACE << "- " << *i;
//        TRACE << "Combined row: ";
//        for (i = data[row].begin(); i != data[row].end(); i++)
//            TRACE << "- " << *i;
    }

    TRACE << "Column addition has been evaluated.";

    return this;
}


string HqlTable::generateId()
{
    stringstream stream;
    stream << ++lastId;
    return stream.str();
}

void HqlTable::resetId()
{
    lastId = 0;
}

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
        for (int i = 1; i < names.size(); i ++)
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