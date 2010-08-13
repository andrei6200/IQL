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

#include <pqxx/pqxx>

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#ifdef __GNUG__
#include "raslib/template_inst.hh"
#endif
#endif
#include "rasdaman.hh"

// Uncomment to print hidden columns of tables
#define PRINT_HIDDEN_COLUMNS

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


using namespace std;
using namespace pqxx;


HqlTable::HqlTable(storageType type)
    : rows(0), columns(1), hiddenCount(1), lastId(0), storage(type)
{
    TRACE << "Created new Table with " << rows << " rows and "
            << columns << " columns (" << hiddenCount << " hidden)";
    string id("_hql_id");
    names = vector<string>();
    names.push_back(id);
    widths = vector<int>();
    widths.push_back(id.length());
    hidden = vector<bool>();
    hidden.push_back(true);
}


/* Import data from the result of a RaSQL query. */
void HqlTable::importFromRasql(r_Set<r_Ref_Any> *resultSet)
{
    INFO << "Building HqlTable from Rasql result ...";

    rows = resultSet->cardinality();
    columns += 2;

    string colname("rasql_output");
    string oid("_rasql_oid");
    names.push_back(colname);
    names.push_back(oid);
    widths.push_back(colname.length());
    widths.push_back(oid.length());
    hidden.push_back(false);
    hidden.push_back(true);
    hiddenCount ++;

    vector<string> row;

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
                TRACE << "filename for #" << i << " is: " << defFileName;

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

        /* Add the value of the current cell to the table */
        TRACE << " Cell value: " << cell.str();
        row.push_back(cell.str());

        r_Ref_Any ref = *iter;
        stringstream stroid(stringstream::out);
        stroid << ref.get_oid();
        row.push_back(stroid.str());

        data.push_back(row);
    } // for(...)
}


/* Import data from the result of an SQL query. */
void HqlTable::importFromSql(result sqlResult)
{
    rows = sqlResult.size();
    int newcols = sqlResult.columns();
    columns += newcols;

    vector<bool> newhidden = vector<bool>(newcols, false);
    newhidden[0] = true;

    vector<string> newnames = vector<string > (newcols, "");
    for (int i = 0; i < newcols; i++)
    {
        newnames[i] = sqlResult.column_name(i);
    }

    if (sqlResult.size() == 0)
        return;

    result::const_iterator it;
    vector<string> row;
    data.clear();
    for (it = sqlResult.begin(); it != sqlResult.end(); it++)
    {
        row.clear();
        // Insert row ID
        row.push_back(generateId());
        // Insert actual data
        result::tuple tuple = *it;
        string val;
        for (int col = 0; col < tuple.size(); col++)
        {
            val = tuple[col].as(string());
            row.push_back(val);
        }
        data.push_back(row);
    }

    // Append the new data
    hidden.insert(hidden.end(), newcols, false);
    names.insert(names.end(), newnames.begin(), newnames.end());
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
    updateWidths();

    TRACE << "Printing HqlTable instance.";
    DEBUG << "HqlTable instance:";
    DEBUG << " * " << rows << " rows";
    DEBUG << " * " << columns << " columns ( " << hiddenCount << " hidden )";
    DEBUG;

    out << INDENT_PROMPT << "SQL Query results" << endl;

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
                        << setw((widths[i]-l)/2) << "";
                if ((widths[i]+l) % 2 == 1 || (widths[i]-l) % 2 == 1)
                    out << " ";
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
                    out << sep << setw((widths[i]+l)/2) << row[i]
                               << setw((widths[i]-l)/2) << "";
                    if ((widths[i]+l) % 2 == 1 || (widths[i]-l) % 2 == 1)
                        out << " ";
                    sep = TABLE_COL_SEPARATOR;
                }
        }

    }

    // If no rows in the result, then just print "0 rows".
    out << endl << INDENT_PROMPT << "( " << rows
            << " row" << (rows == 1 ? "" : "s") << " )" << endl;

    TRACE << "Done Printing HqlTable instance.";
}



HqlTable* HqlTable::crossProduct(HqlTable* other)
{
    TRACE << "Cross product between two HqlTables.";

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
            TRACE << "Just inserted row: ";
            vector<string>::iterator i;
            for (i = row.begin(); i != row.end(); i++)
                TRACE << "- " << *i;
        }

    TRACE << "Cross product has been evaluated.";

    return output;
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
    widths = vector<int>(columns, 0);

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
