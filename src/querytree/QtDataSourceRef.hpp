/* 
 * File:   QtDataSourceRef.hpp
 * Author: andrei
 *
 * Created on June 26, 2010, 1:29 PM
 *
 * Stores a reference to
 * - a Rasdaman table (or Iterator name) or
 * - a Postgres field name
 */

#ifndef QtDataSourceRef_HPP
#define	QtDataSourceRef_HPP

#include "QtNode.hpp"

class QtDataSourceRef: public QtNode
{
public:
    QtDataSourceRef(QtNode* b);
    QtDataSourceRef(QtNode* b, char* al);
    ~QtDataSourceRef();

    std::string toString();
    char* toCString();
    HqlTable* execute();

    DbEnum setupDbSource();
private:
    QtNode* base;
    char* alias;
};

#endif	/* QtDataSourceRef_HPP */

