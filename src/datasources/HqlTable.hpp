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

    /* Allow full access to the private members of the HqlTable, for efficiency.*/
    friend class PostgresDS;
    friend class RasdamanDS;

    /* A table can be stored in memory, in Postgres or in Rasdaman. */
    enum storageType {MEMORY, POSTGRES, RASDAMAN};

    /* Default Constructor: an in-memory table. */
    HqlTable(storageType type = MEMORY);

    /* Constructor with Postgres table name. The data will not be replicated in memory. */
    HqlTable(std::string name);

    /* Import data from Rasdaman to the current table */
    void importFromRasql(r_Set<r_Ref_Any> *resultSet, bool storeOnDisk = false);

    /* Import data from Postgres to the current table. Data will be stored in memory */
    void importFromSql(pqxx::result sqlResult);

    /* Execute a cross Product between this table and another HqlTable. */
    HqlTable* crossProduct(HqlTable *other);

    /* Add the columns of another table. The number of tuples must be the same. */
    HqlTable* addColumns(HqlTable *other);

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

    /* Hidden column info */
    std::vector<bool> hidden;
    int hiddenCount;

    /* The actual data, as strings */
    std::vector<std::vector<std::string> > data;


    /* Update the width information for printing. */
    void updateWidths();

    /* Counters */
    long columns, rows;

    /* Storage information */
    storageType storage;
    string tableName;


    /*** Functions for tuple-id generation ***/

    /* Reset the ID counter. */
    void resetId();

    /* Generates a new ID, unique for this table. */
    string generateId();

    /* Last used id */
    long lastId;
};


std::ostream& operator << (std::ostream &o, HqlTable *a);

#endif	/* HQLTABLE_HPP */
