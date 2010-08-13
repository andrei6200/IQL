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

#include <string>
#include "QtNode.hpp"
#include "datasources/HqlTable.hpp"


class QtDataSource: public QtNode
{
public:
    QtDataSource(char* name);
    QtDataSource(QtNode* name);
    QtDataSource(char* name, char* alias);
    QtDataSource(QtNode* name, char* alias);

    ~QtDataSource();

    virtual HqlTable* execute();
    virtual std::string toString();
    virtual DbEnum setupDbSource();

    std::string getTableName();
private:
    std::string tableName;
    std::string alias;
};

#endif	/* QTSOURCE_HPP */

