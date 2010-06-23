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


/* SELECT statement. */
class selectStruct
{
public:
    std::list<std::string> *from;   // FROM which tables?
    std::list<std::string> *what;   // Which columns to select? 

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
class tableRefStruct
{
    std::string *tname; // table name
    std::string *alias; // table AS alias 
    bool all;           // SELECT *
};

#endif	/* STRUCTURES_HPP */
