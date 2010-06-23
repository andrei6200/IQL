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

#include "utils/logger.hpp"

class HqlTable
{
public:
    /* Constructor from RaSQL result type (uses global variable for info) */
    HqlTable(/* r_Set< r_Ref_Any > rasqlResult */);

//    /* Constructor with data and column names */
//    HqlTable(std::vector<std::vector<std::string> > data, std::vector<std::string> names);

//    /* Constructor from SQL result type */
//    HqlTable(pqxx::result sqlResult);

    /* Import data from Rasdaman to the current table */
    void importFromRasql();

    /* Import data from Postgres to the current table. */
    void importFromSql(pqxx::result sqlResult);

//    /* Constructor from RaSQL result type */
//    HqlTable(r_Set<r_Ref_Any> rasqlResult);

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

#define DEFAULT_OUTFILE_MASK "hql_%d"

#endif	/* HQLTABLE_HPP */
