/* 
 * File:   QtColumn.hpp
 * Author: andrei
 *
 * Created on August 14, 2010, 12:04 PM
 */

#ifndef QTCOLUMN_HPP
#define	QTCOLUMN_HPP

#include "QtNode.hpp"

/** Represents a reference to a relational table column or to a Rasdaman collection.*/
class QtColumn : public QtNode
{
public:
    QtColumn(char *col);
    QtColumn(char *tbl, char* col);
    QtColumn(QtNode* node, char* col);
    virtual ~QtColumn();

    IqlTable* execute();
    std::string toString();
    void print(ostream &o, std::string indent);

    DbEnum setupDbSource();
private:
    std::string table, column;
    QtNode* child;
};

#endif	/* QTCOLUMN_HPP */
