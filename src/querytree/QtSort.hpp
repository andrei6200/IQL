/* 
 * File:   QtSort.hpp
 * Author: andrei
 *
 * Created on August 4, 2010, 1:43 PM
 *
 * SQL Feature
 */

#ifndef QTSORT_HPP
#define	QTSORT_HPP

#include "QtNode.hpp"

class QtSort : public QtNode
{
public:
    QtSort();
    QtSort(const QtSort& orig);
    virtual ~QtSort();
private:

};

#endif	/* QTSORT_HPP */

