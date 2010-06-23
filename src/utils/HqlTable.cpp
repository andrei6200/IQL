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


#include "utils/HqlTable.hpp"
#include "config.hpp"
#include "utils/logger.hpp"
#include "HqlTable.hpp"


using namespace std;
using namespace pqxx;

HqlTable::HqlTable() : rows(0), columns(0), pg_result(NULL), rman_result(NULL)
{
}


extern r_Set<r_Ref_Any> globalRasqlResultSet;


/* Import data from the result of a RaSQL query. */
void HqlTable::importFromRasql(/*r_Set<r_Ref_Any> resultSet*/)
{
    INFO << "Building HqlTable from Rasql result ...";

    rows = globalRasqlResultSet.cardinality();
    columns = 1;

    string colname = "rasql_output";
    names = vector<string>();
    names.push_back(colname);
    widths = vector<int>();
    widths.push_back(colname.length());

//    rows = columns = 0;
    rman_result = NULL;
    pg_result = NULL;

    vector<string> row;

    r_Iterator< r_Ref_Any > iter = globalRasqlResultSet.create_iterator();
    // iter.not_done() seems to behave wrongly on empty set, therefore this additional check -- PB 2003-aug-16
    for (int i = 1; i <= globalRasqlResultSet.cardinality() && iter.not_done(); iter++, i++)
    {
        stringstream cell (stringstream::out);
        switch (globalRasqlResultSet.get_element_type_schema()->type_id())
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
                fwrite((void*) r_Ref<r_GMarray > (*iter)->get_array(), 1, r_Ref<r_GMarray > (*iter)->get_array_size(), tfile);
                fclose(tfile);
                TRACE << "ok." << endl;

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
                //            printScalar(*(r_Ref<r_Scalar > (*iter)));
                //            cout << endl;
                // or simply
                //            r_Ref<r_Scalar>(*iter)->print_status( cout );

                cell << *(r_Ref<r_Scalar > (*iter));
        } // switch

        /* Add the value of the current cell to the table */
        TRACE << " Cell value: " << cell.str();
        row.clear();
        row.push_back(cell.str());

        data.push_back(row);

    } // for(...)
}

/* Import data from the result of an SQL query. */
void HqlTable::importFromSql(result sqlResult)
{
    pg_result = &sqlResult;
    rman_result = NULL;

    rows = sqlResult.size();
    columns = sqlResult.columns();

    widths = vector<int>(columns, 0);

    names = vector<string > (columns, "");
    for (int i = 0; i < columns; i++)
    {
        names[i] = sqlResult.column_name(i);
        if (names[i].length() > widths[i])
            widths[i] = names[i].length();
    }

    if (sqlResult.size() == 0)
        return;

    result::const_iterator it;
    vector<string> row;
    int length;
    data.clear();
    for (it = sqlResult.begin(); it != sqlResult.end(); it++)
    {
        row.clear();
        result::tuple tuple = *it;
        for (int col = 0; col < tuple.size(); col++)
        {
            row.push_back(tuple[col].as(string()));
            length = tuple[col].as(string()).length();
            if (length > widths[col])
                widths[col] = length;
        }
        data.push_back(row);
    }
}

/*
 * The default constructor assumes RaSQL data is available !
 * NOTE: This constructor does not use the argument !
 * It fetches data from the global variable "globalRasqlResultSet"
 */
//HqlTable::HqlTable(/* r_Set< r_Ref_Any > rasqlResult */)
//{
//
//}
//
//HqlTable::HqlTable(result sqlResult)
//{
//
//}

vector<string> HqlTable::getColumn(int index)
{
    vector<string> vec;
    for (int i = 0; i < rows; i++)
        vec.push_back(data[i][index]);
    return vec;
}

//HqlTable::HqlTable(vector<vector<string> > data, vector<string> names)
//{
//    rows = data.size();
//    if (rows > 0)
//        columns = data[0].size();
//    /* Error checks */
//    if (columns != names.size())
//        throw std::exception("Cannot build HqlTable, because vector sizes do not match.");
//
//    this->names = names;
//    this->data = data;
//
//    /* Find out the width of each table column: take the maximum length of
//     any value in that column. */
//    this->widths = vector<int>(data[0].size());
//    for (int i = 0; i < widths.size(); i++)
//        widths[i] = 0;
//
//    vector<vector<string> >::iterator it;
//    vector<string> row;
//    vector<string>::iterator val;
//    for (it = data.begin(); it != data.end(); it ++)
//    {
//        row = *it;
//        for (i = 0, val = row.begin(); val != row.end(); val ++, i++)
//            if ((*val).length() > widths[i])
//                widths[i] = (*val).length();
//    }
//
//    TRACE << "Here are the widths of the table columns: ";
//    for (int i = 0; i < widths.size(); i++)
//        TRACE << widths[i];
//    TRACE;
//}

HqlTable::~HqlTable()
{
    //    if (pg_result)
    //    {
    //        delete pg_result;
    //        pg_result = NULL;
    //    }
    //    if (rman_result)
    //    {
    //        delete rman_result;
    //        rman_result = NULL;
    //    }
}

/* Print the table contents to a specified stream. */
void HqlTable::print(ostream &out)
{
    TRACE << "Printing HqlTable instance.";
    DEBUG << "HqlTable instance:";
    DEBUG << " * " << rows << " rows";
    DEBUG << " * " << columns << " columns";
    DEBUG;

    out << INDENT_PROMPT << "SQL Query results" << endl;

    if (columns > 0)
    {
        out << INDENT_PROMPT;
        /* First print the column names */
        out << setw(widths[0]) << names[0];
        for (int i = 1; i < columns; i++)
            out << TABLE_COL_SEPARATOR << setw(widths[i]) << names[i];
        out << endl;
        /* And the separator line ...*/
        out << INDENT_PROMPT << setw(widths[0])
                << setfill(TABLE_HEADER_SEPARATOR) << TABLE_HEADER_SEPARATOR;
        for (int i = 1; i < columns; i++)
            out << TABLE_HEADER_SEPARATOR << "+" << setw(widths[i])
            << setfill(TABLE_HEADER_SEPARATOR) << TABLE_HEADER_SEPARATOR
                << TABLE_HEADER_SEPARATOR;
        out << setfill(' ');
    }

    if (rows > 0)
    {

        /* Now print the actual data*/
        vector<string> row;
        for (int r = 0; r < rows; r++)
        {
            out << endl;
            row = data[r];
            out << INDENT_PROMPT << setw(widths[0]) << row[0];
            for (int i = 1; i < row.size(); i++)
                out << TABLE_COL_SEPARATOR << setw(widths[i]) << row[i];
        }
        
    }

    // If no rows in the result, then just print "0 rows".
    out << endl << INDENT_PROMPT << "( " << rows << " rows )" << endl;

    TRACE << "Done Printing HqlTable instance.";
}