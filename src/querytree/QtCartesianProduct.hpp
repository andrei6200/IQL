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
    QtCartesianProduct();
    QtCartesianProduct(const QtCartesianProduct& orig);
    virtual ~QtCartesianProduct();
private:

};

#endif	/* QTCARTESIANPRODUCT_HPP */

