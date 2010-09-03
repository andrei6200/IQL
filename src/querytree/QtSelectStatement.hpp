/* 
 * File:   QtSelect.hpp
 * Author: andrei
 *
 * Created on June 26, 2010, 1:36 PM
 *
 * Represents a complete HQL SELECT query.
 */

#ifndef QTSELECT_HPP
#define	QTSELECT_HPP

#include "QtDataSource.hpp"
#include "QtNode.hpp"
#include "QtList.hpp"
#include "QtWhere.hpp"

#include <vector>

class QtSelectStatement : public QtNode
{
public:
    QtSelectStatement();
    QtSelectStatement(QtList* refs, QtList* sources, QtWhere* cond);
    ~QtSelectStatement();

    HqlTable* execute();

    DbEnum setupDbSource();

    std::string toString();

    void print(ostream &o, std::string indent = "");

    /* Return a QtList (containing all the QtDataSource objects) */
    QtList* getSourceTables();
    /* Return a table that holds the cartesian product, if it has been evaluated. */
    HqlTable* getCartesianProduct();

    /* Returns true if this structure can be evaluated with exactly two queries,
     one RaSQL and one SQL query. */
//    bool mixedQueryIsSimple();
    
private:
    QtList* what;
    QtList* from;
    QtWhere *where;

    HqlTable *product;
};

#endif	/* QTSELECT_HPP */
