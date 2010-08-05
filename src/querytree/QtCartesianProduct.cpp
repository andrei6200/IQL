/* 
 * File:   QtCartesianProduct.cpp
 * Author: andrei
 * 
 * Created on August 4, 2010, 1:25 PM
 */

#include "QtCartesianProduct.hpp"

QtCartesianProduct::QtCartesianProduct(QtNode* node1, QtNode* node2)
    : child1(node1), child2(node2)
{
    if (!child1 || !child2)
    {
        string msg = "Cannot build Cartesian Product node because"
                " one of its children are NULL.";
        ERROR << msg;
        // FIXME: Use a custom exception
        throw msg;
    }
}

QtCartesianProduct::~QtCartesianProduct()
{
    if (child1)
    {
        delete child1;
        child1 = NULL;
    }
    if (child2)
    {
        delete child2;
        child2 = NULL;
    }
}

DbEnum QtCartesianProduct::setupDbSource()
{
    DbEnum db1 = child1->setupDbSource();
    DbEnum db2 = child2->setupDbSource();
    if (db1 == db2)
        db_source = db1;
    else
        db_source = MIXED;

    return db_source;
}

string QtCartesianProduct::toString()
{
    string out = child1->toString() + ", " + child2->toString();
    return out;
}

HqlTable* QtCartesianProduct::execute()
{
    // FIXME: implement
    return NULL;
}