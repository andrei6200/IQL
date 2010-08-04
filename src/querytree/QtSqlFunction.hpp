/* 
 * File:   QtSqlFunction.hpp
 * Author: andrei
 *
 * Created on August 4, 2010, 1:45 PM
 */

#ifndef QTSQLFUNCTION_HPP
#define	QTSQLFUNCTION_HPP

#include "QtNode.hpp"

class QtSqlFunction : public QtNode
{
public:
    QtSqlFunction();
    QtSqlFunction(const QtSqlFunction& orig);
    virtual ~QtSqlFunction();
private:

};

#endif	/* QTSQLFUNCTION_HPP */

