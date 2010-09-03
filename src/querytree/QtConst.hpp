/* 
 * File:   QtConst.hpp
 * Author: andrei
 *
 * Created on August 14, 2010, 12:04 PM
 */

#ifndef QTCONST_HPP
#define	QTCONST_HPP

#include "QtNode.hpp"


class QtConst : public QtNode
{
public:
    QtConst(char* str, char* type);
    QtConst(char* str);
    virtual ~QtConst();

    HqlTable* execute();
    std::string toString();
    void print(ostream &o, std::string indent);

    DbEnum setupDbSource();
private:
    std::string value, type;
};

#endif	/* QTCONST_HPP */