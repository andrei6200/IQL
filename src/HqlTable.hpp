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

#include "logger.hpp"

class HqlTable
{
public:
    /* Default Constructor. */
    HqlTable();

//    /* Constructor with data and column names */
//    HqlTable(std::vector<std::vector<std::string> > data, std::vector<std::string> names);

//    /* Constructor from SQL result type */
//    HqlTable(pqxx::result sqlResult);

    /* Import data from Rasdaman to the current table */
    void importFromRasql(r_Set<r_Ref_Any> *resultSet);

    /* Import data from Postgres to the current table. */
    void importFromSql(pqxx::result sqlResult);

    /* Execute a cross Product between this table and another HqlTable. */
    HqlTable* crossProduct(HqlTable *other);

//    /* Constructor from RaSQL result type */
//    HqlTable(r_Set<r_Ref_Any> rasqlResult);

//    /* Copy constructor */
//    HqlTable(const HqlTable& orig);

    /* Destructor */
    virtual ~HqlTable();

    /* Return a certain column as a vector */
    std::vector<std::string> getColumn(int index);

    /* Return a certain column as a vector */
    std::vector<std::string> getColumn(std::string name);

    /* Display this table to stdout */
    void print(std::ostream& out);

private:
    /* The column names.  */
    std::vector<std::string> names;
    /* The column widths */
    std::vector<int> widths;
    /* Hidden info */
    std::vector<bool> hidden;
    int hiddenCount;
    /* The actual data, as strings */
    std::vector<std::vector<std::string> > data;

    /* Reset the ID counter. */
    void resetId();

    /* Generates a new ID, unique for this table. */
    string generateId();
    
    /* Last used id */
    long lastId;

    /* Update the width information for printing. */
    void updateWidths();

    /* Counters */
    long columns, rows;
};

#define TABLE_COL_SEPARATOR     " | "
#define TABLE_HEADER_SEPARATOR  '-'

#define DEFAULT_OUTFILE_MASK "hql_%d"

#endif	/* HQLTABLE_HPP */