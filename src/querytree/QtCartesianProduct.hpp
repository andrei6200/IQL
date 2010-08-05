/* 
 * File:   QtCartesianProduct.hpp
 * Author: andrei
 *
 * Created on August 4, 2010, 1:25 PM
 *
 * A cartesian product is just a special case of a Join operation.
 */

#ifndef QTCARTESIANPRODUCT_HPP
#define	QTCARTESIANPRODUCT_HPP

#include "QtNode.hpp"

class QtCartesianProduct : public QtNode
{
public:

    QtCartesianProduct(QtNode* node1, QtNode* node2);
    
    virtual ~QtCartesianProduct();

    /* C++ String representation */
    virtual std::string toString() = 0;

    /* Recursively computes the value of the "db_source" field.
     * - If all the child nodes belong to the same DB, then this node also belongs to it
     * - If some child nodes belong to different DB, then this node will be "mixed"
     */
    virtual DbEnum setupDbSource() = 0;

    /* Get the source system that can handle this node. */
    DbEnum getDbSource();

    /* Execute the operation of this node and return a HqlTable result. */
    virtual HqlTable* execute();
    
private:

    QtNode *child1, *child2;
};

#endif	/* QTCARTESIANPRODUCT_HPP */
