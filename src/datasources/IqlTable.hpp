/* 
 * File:   IqlTable.hpp
 * Author: andrei
 *
 * Created on June 18, 2010, 4:02 PM
 */

#ifndef IqlTable_HPP
#define	IqlTable_HPP

#include <pqxx/pqxx>
#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#ifdef __GNUG__
#include "raslib/template_inst.hh"
#endif
#endif
#include "rasdaman.hh"

#include "logger.hpp"


// Internal HQL id
#define HQL_COL "iqlinternalid"
// Separator between Iql Table name and Column name, in qualified names
#define IQL_TBL_COL_SEP         string("__")
#define IQL_TBL_COL_SEP_SIZE    2

/** Represents a table in main memory.
 * 
 * This class can be initialized with the results of an SQL-query, with the
 * results of a RaSQL-query, or simply with a name. In the latter case, this
 * class only stores the name of the table, while in the rest of the cases we
 * also store the result data and auxiliary information (columns, widths etc).
 *
 * The datatype is assumed to be *almost* always string. The only exception is
 * made when importing data from a RaSQL result, when the result type is scalar.
 * Then we use the float datatype. \todo implement
 */
class IqlTable
{
public:
    /** If false, the only useful information may be the table name. */
    bool dataAlsoInMemory;

    /** Allow full access to the private members of the IqlTable, for efficiency.*/
    friend class PostgresDS;
    friend class RasdamanDS;
    friend class QtRasqlFunction;

    /** A table can be stored in memory, in Postgres or in Rasdaman. */
    enum storageType {MEMORY, POSTGRES, RASDAMAN};

    /** Constructor with Postgres table name. The data will not be replicated in memory automatically. */
    IqlTable(std::string name);

    /** Constructor from Rasdaman query result*/
    IqlTable(r_Set<r_Ref_Any> *resultSet, bool storeOnDisk = false);

    /** Import data from Postgres to the current table. Data will be stored in memory */
    IqlTable(pqxx::result sqlResult);

    /** Execute a cross Product between this table and another IqlTable. */
    IqlTable* crossProduct(IqlTable *other);

    /** Add the columns of another table. The number of tuples must be the same. */
    IqlTable* addColumns(IqlTable *other);

    /** Destructor */
    virtual ~IqlTable();

    /** Return the qualified column names as vector */
    std::vector<std::string> getQualifiedColumnNames();

    /** Return the raw column names as a vector */
    std::vector<std::string> getColumnNames();

    /** Return a certain column as a vector */
    std::vector<std::string> getColumn(int index);

    /** Return a certain column as a vector */
    std::vector<std::string> getColumn(std::string name);

    /** Modify the filenames of Rasdaman objects. */
    void setFilenames(std::vector<std::string> values, int colIndex);

    /** Display this table to stdout */
    void print(std::ostream& out);

    /** Set the name of this table. */
    void setName(std::string name, bool updateColumnNames = false);

    /** Get the name of this table, as it is stored in the DataSources. */
    std::string getName();

    /** Return the data stored inside this table*/
    std::vector<std::vector<std::string > > getData();

    /** Returns the number of rows of the current table. */
    int rowCount();

    /** Set a new PostgreSQL datatype for this table.
     NOTE: The datatype applies for all columns !
     */
    void setDataType(std::string newtype);

    /** Returns the PostgreSQL datatype of all the columns. */
    std::string getDataType();

private:

    /** Delete a column. */
    void deleteColumn(int index);

    /** Set a column name */
    void setColumnName(int index, std::string name);

    /** The qualified column names (tableName_columnName).  */
    std::vector<std::string> qnames;

    /** The un-qualified column names (just the column names) */
    std::vector<std::string> names;

    /** The column widths */
    std::vector<int> widths;

    /** Hidden column info */
    std::vector<bool> hidden;
    int hiddenCount;

    /** The actual data, as strings */
    std::vector<std::vector<std::string> > data;

    /** Update the width information for printing. */
    void updateWidths();

    /** Counters */
    long columns, rows;

    /** Storage information */
    storageType storage;
    string tableName;
    string dataType;
};


std::ostream& operator << (std::ostream &o, IqlTable *a);

#endif	/* IqlTable_HPP */
