/* 
 * File:   QtString.hpp
 * Author: andrei
 *
 * Created on July 2, 2010, 7:16 PM
 *
 * Placeholder class for a simple string
 */

#ifndef QTSTRING_HPP
#define	QTSTRING_HPP

#include <string>
#include "QtNode.hpp"

class QtString: public QtNode
{
public:
    QtString(std::string s);
    QtString(char* s);
    virtual ~QtString();
    HqlTable* execute();
    std::string toString();
    char* toCString();

    DbEnum setupDbSource();
private:
    std::string str;
};

#endif	/* QTSTRING_HPP */

