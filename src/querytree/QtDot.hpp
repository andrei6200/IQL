/* 
 * File:   QtDot.hpp
 * Author: andrei
 *
 * Created on July 1, 2010, 6:33 AM
 */

#ifndef QTDOT_HPP
#define	QTDOT_HPP

#include <string>
#include "QtNode.hpp"

class QtDot: public QtNode
{
public:
    QtDot(QtNode* n, std::string e);
    virtual ~QtDot();

    virtual std::string toString();
    char* toCString();

    DbEnum setupDbSource();

    HqlTable* execute();

    void print(ostream &o, std::string indent);
private:
    QtNode* base;
    std::string exp;
};

#endif	/* QTDOT_HPP */

