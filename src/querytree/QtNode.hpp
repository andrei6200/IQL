/* 
 * File:   QtNode.hpp
 * Author: andrei
 *
 * Created on June 26, 2010, 12:31 PM
 *
 * Base Interface class for all nodes of the HQL Query Parse Tree.
 */

#ifndef QTNODE_HPP
#define	QTNODE_HPP

#include <string>

#include "utils/HqlTable.hpp"

/* Enumeration of available DBMS systems */
enum DbEnum
{
    DB_NOT_INITIALIZED = 0,
    POSTGRES = 1,
    RASDAMAN = 2,
    UNKNOWN_DB = 3,
    MIXED = 4
};


/* Public super-class for all nodes of the Query Tree. */
class QtNode
{
public:
    /* Default constructor */
    QtNode();
    
    /* Default destructor */
    virtual ~QtNode();

    /* String representation */
    virtual std::string toString() = 0;

    virtual char* toCString();

    /* Recursively computes the value of the "db_source" field.
     * - If all the child nodes belong to the same DB, then this node also belongs to it
     * - If some child nodes belong to different DB, then this node will be "mixed"
     */
    virtual DbEnum setupDbSource() = 0;

    /* Get the source system that can handle this node. */
    DbEnum getDbSource();

    /* Execute the operation of this node and return a HqlTable result. */
    virtual HqlTable* execute();
    
    /* Query string. */
    char* query;

protected:
    /* Database Source System */
    DbEnum db_source;
};

#endif	/* QTNODE_HPP */

