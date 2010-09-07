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
#include "QtJoin.hpp"

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

private:
    QtList* what;
    QtList* from;
    QtWhere *where;

    HqlTable *product;

    /* In order to evaluate joins with theta-conditions (JOIN .. ON condition),
     * we need to provide the available tables to the QtJoin node. We do this
     * by using a mechanism which we call context switching. When needed, we modify
     * the "cartesian product" of the entire query to contain the source table for
     * a JOIN. 
     */
    void switchContextForJoin(HqlTable* newContext);
    /* Allow class QtJoin to access the private methods. */
    friend class QtJoin;
};

#endif	/* QTSELECT_HPP */
