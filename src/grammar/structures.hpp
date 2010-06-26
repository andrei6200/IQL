/* 
 * File:   structures.hpp
 * Author: andrei
 *
 * Created on June 16, 2010, 6:44 AM
 *
 * Contains definitions of Parser structures.
 * These use STL as much as possible. 
 */

#ifndef STRUCTURES_HPP
#define	STRUCTURES_HPP

#include <list>
#include <string>

/* Forward declarations. */
struct tableRefStruct;

/* Enumeration of available DBMS systems */
enum DbEnum
{
    UNKNOWN_DB = 0,
    POSTGRES = 1,
    RASDAMAN = 2
};


/* SELECT statement. */
struct selectStruct
{
    std::list<tableRefStruct*> *from;   // FROM which tables?
    std::list<tableRefStruct*> *what;   // Which columns to select?

    char *query;                    // query string

    selectStruct(): from(NULL), what(NULL), query(NULL)
    {
    }

    ~selectStruct()
    {
        if (from != NULL)
            delete from;
        if (what != NULL)
            delete what;
        if (query != NULL)
            delete query;
    }
} ;

/* Reference to a SQL table / RaSQL collection */
struct tableRefStruct
{
public:
    std::string tname;        // table name
    std::string *alias;        // table AS alias
    DbEnum source;
    std::string total;

    tableRefStruct(char* name): alias(NULL), source(UNKNOWN_DB)
    {
        tname = std::string(name);
        total = tname;
    }

    tableRefStruct(char* name, char* a): source(UNKNOWN_DB)
    {
        tname = std::string(name);
        alias = new std::string(a);
        total = tname + std::string(" AS ") + *alias;
    }

    ~tableRefStruct()
    {
        if (alias)
            delete alias;
    }
};

#endif	/* STRUCTURES_HPP */
