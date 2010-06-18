/* 
 * File:   HqlTable.cpp
 * Author: andrei
 * 
 * Created on June 18, 2010, 4:02 PM
 */

#include <exception>
#include <pqxx/result.hxx>

#include "HqlTable.hpp"
#include "config.hpp"

#include <vector>
#include <string>
#include <iomanip>
#include <iostream>


using namespace std;
using namespace pqxx;

HqlTable::HqlTable() : rows(0), columns(0), pg_result(NULL), rman_result(NULL)
{
}

HqlTable::HqlTable(result sqlResult) : pg_result(NULL), rman_result(NULL)
{
    if (sqlResult.size() == 0)
        return;

    rows = sqlResult.size();
    columns = sqlResult.columns();

    widths = vector<int>(columns, 0);

    names = vector<string> (columns, "");
    for (int i = 0; i < columns; i++)
    {
        names[i] = sqlResult.column_name(i);
        if (names[i].length() > widths[i])
            widths[i] = names[i].length();
    }

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

    pg_result = &sqlResult;
}


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
    out << RESPONSE_PROMPT << "SQL Query results" << endl;
    out << RESPONSE_PROMPT;

    if (rows > 0)
    {
        /* First print the column names */
        out << setw(widths[0]) << names[0];
        for (int i = 1; i < columns; i++)
            out << TABLE_COL_SEPARATOR << setw(widths[i]) << names[i];
        out << endl << RESPONSE_PROMPT;
        out << setw(widths[0]) << setfill(TABLE_HEADER_SEPARATOR) << TABLE_HEADER_SEPARATOR;
        for (int i = 1; i < columns; i++)
            out << TABLE_HEADER_SEPARATOR << "+" << setw(widths[i])
            << setfill(TABLE_HEADER_SEPARATOR) << TABLE_HEADER_SEPARATOR
                << TABLE_HEADER_SEPARATOR;
        out << endl << setfill(' ');
        /* Now print the actual data*/
        vector<string> row;
        for (int r = 1; r < rows; r++)
        {
            row = data[r];
            out << RESPONSE_PROMPT;
            out << setw(widths[0]) << row[0];
            for (int i = 1; i < row.size(); i++)
                out << TABLE_COL_SEPARATOR << setw(widths[i]) << row[i];
            out << endl;
        }
        out << endl;
    }
    out << "( " << rows << " rows )" << endl;
}