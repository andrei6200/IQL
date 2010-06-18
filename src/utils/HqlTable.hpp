/* 
 * File:   HqlTable.hpp
 * Author: andrei
 *
 * Created on June 18, 2010, 4:02 PM
 */

#ifndef HQLTABLE_HPP
#define	HQLTABLE_HPP

#include <pqxx/pqxx>
#include "rasdaman.hh"

class HqlTable
{
public:
    /* Emtpy constructor */
    HqlTable();
//    /* Constructor with data and column names */
//    HqlTable(std::vector<std::vector<std::string> > data, std::vector<std::string> names);
    /* Constructor from SQL result type */
    HqlTable(pqxx::result sqlResult);
//    /* Copy constructor */
//    HqlTable(const HqlTable& orig);
    /* Destructor */
    virtual ~HqlTable();

    /* Return a certain column as a vector */
    std::vector<std::string> getColumn(int index);

    /* Display this table to stdout */
    void print(std::ostream& out);
    
private:
    /* The column names */
    std::vector<std::string> names;
    /* The column widths */
    std::vector<int> widths;
    /* The actual data, as strings */
    std::vector<std::vector<std::string> > data;

    /* Counters */
    int columns, rows;

    /* The pqxx::result object, if available */
    pqxx::result *pg_result;
    r_Set<r_Ref_Any> *rman_result;
};

#define TABLE_COL_SEPARATOR     " | "
#define TABLE_HEADER_SEPARATOR  '-'

#endif	/* HQLTABLE_HPP */
