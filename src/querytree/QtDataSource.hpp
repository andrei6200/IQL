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
#include <ostream>
#include "QtNode.hpp"

/** Represents a source table in PostgreSQL or an array collection in Rasdaman.*/
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

    void print(ostream &o, std::string indent = "");
private:
    std::string tableName;
    std::string alias;
};

#endif	/* QTSOURCE_HPP */

