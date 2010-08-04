/* 
 * File:   QtDot.hpp
 * Author: andrei
 *
 * Created on July 1, 2010, 6:33 AM
 */

#ifndef QTDOT_HPP
#define	QTDOT_HPP

//#include <string>
#include "QtNode.hpp"
#include "utils/HqlTable.hpp"

class QtDot: public QtNode
{
public:
    QtDot(QtNode* n, std::string e);
    virtual ~QtDot();

    std::string toString();
    char* toCString();

    DbEnum setupDbSource();

    HqlTable* execute();
private:
    QtNode* base;
    std::string exp;
};

#endif	/* QTDOT_HPP */

