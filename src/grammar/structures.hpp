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

class selectStruct
{
public:
    std::list<std::string> *from;
    std::list<std::string> *what;
    char *query;

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

#endif	/* STRUCTURES_HPP */
