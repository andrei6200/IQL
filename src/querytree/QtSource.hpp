/* 
 * File:   QtSource.hpp
 * Author: andrei
 *
 * Created on June 26, 2010, 12:56 PM
 *
 * Stores the name of a source table (either postgres of rasdaman table)
 */

#ifndef QTSOURCE_HPP
#define	QTSOURCE_HPP

#include <map>
#include "QtNode.hpp"


class QtSource: public QtNode
{
public:
    QtSource(char* name);
    QtSource(char* name, char* alias);
    virtual ~QtSource();

    HqlTable* execute();
    std::string toString();
    DbEnum setupDbSource();
private:
    char* tableName;
    char* alias;
};

#endif	/* QTSOURCE_HPP */

