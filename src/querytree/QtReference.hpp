/* 
 * File:   QtReference.hpp
 * Author: andrei
 *
 * Created on June 26, 2010, 1:29 PM
 *
 * Stores a reference to
 * - a Rasdaman table (or Iterator name) or
 * - a Postgres field name
 */

#ifndef QTREFERENCE_HPP
#define	QTREFERENCE_HPP

#include "QtNode.hpp"

class QtReference: public QtNode
{
public:
    QtReference(QtNode* b);
    QtReference(QtNode* b, char* al);
    ~QtReference();

    std::string toString();
    char* toCString();
    HqlTable* execute();

    DbEnum setupDbSource();
private:
    QtNode* base;
    char* alias;
};

#endif	/* QTREFERENCE_HPP */

