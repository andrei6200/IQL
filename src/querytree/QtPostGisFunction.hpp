/* 
 * File:   QtPostGisFunction.hpp
 * Author: andrei
 *
 * Created on August 14, 2010, 12:04 PM
 */

#ifndef QTPOSTGISFUNCTION_HPP
#define	QTPOSTGISFUNCTION_HPP

#include "QtNode.hpp"


class QtPostGisFunction : public QtNode
{
public:
    QtPostGisFunction(char *fname, QtNode *node);
    virtual ~QtPostGisFunction();

    HqlTable* execute();
    std::string toString();
    void print(ostream &o, std::string indent);

    DbEnum setupDbSource();
private:
    std::string function;
    QtNode *child;
};

#endif	/* QTPOSTGISFUNCTION_HPP */
