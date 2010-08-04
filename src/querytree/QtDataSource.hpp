/* 
 * File:   QtSource.hpp
 * Author: andrei
 *
 * Created on June 26, 2010, 12:56 PM
 *
 * Stores the name of a data source collection.
 */

#ifndef QTSOURCE_HPP
#define	QTSOURCE_HPP

#include <map>
#include "QtNode.hpp"


class QtDataSource: public QtNode
{
public:
    QtDataSource(char* name);
    QtDataSource(char* name, char* alias);
    QtDataSource(QtNode* name, char* alias);
    QtDataSource(QtNode* name);
    virtual ~QtDataSource();

    HqlTable* execute();
    std::string toString();
    DbEnum setupDbSource();
private:
    char* tableName;
    char* alias;
};

#endif	/* QTSOURCE_HPP */

