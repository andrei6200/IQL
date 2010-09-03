/* 
 * File:   QtPostGisFunction.hpp
 * Author: andrei
 *
 * Created on August 14, 2010, 12:04 PM
 */

#ifndef QTPOSTGISFUNCTION_HPP
#define	QTPOSTGISFUNCTION_HPP

#include "QtNode.hpp"
#include "QtList.hpp"

class QtPostGisFunction : public QtNode
{
public:
    QtPostGisFunction(char *fname, QtNode *node);
    QtPostGisFunction(char *fname, QtList *node);
    virtual ~QtPostGisFunction();

    HqlTable* execute();
    std::string toString();
    void print(ostream &o, std::string indent);

    DbEnum setupDbSource();
private:
    std::string function;
    QtNode *child;
    QtList *children;
};

#endif	/* QTPOSTGISFUNCTION_HPP */
