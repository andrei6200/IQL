/* 
 * File:   QtConst.hpp
 * Author: andrei
 *
 * Created on August 14, 2010, 12:04 PM
 */

#ifndef QTCONST_HPP
#define	QTCONST_HPP

#include "QtNode.hpp"

/** Represents a constant value, either in SQL or in RaSQl. */
class QtConst : public QtNode
{
public:
    QtConst(char* str, char* type);
    QtConst(char* str);
    virtual ~QtConst();

    IqlTable* execute();
    std::string toString();
    void print(ostream &o, std::string indent);

    DbEnum setupDbSource();
private:
    std::string value, type;
};

#endif	/* QTCONST_HPP */
