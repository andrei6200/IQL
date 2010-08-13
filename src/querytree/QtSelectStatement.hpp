/* 
 * File:   QtSelect.hpp
 * Author: andrei
 *
 * Created on June 26, 2010, 1:36 PM
 *
 * Represents a complete HQL SELECT query.
 */

#ifndef QTSELECT_HPP
#define	QTSELECT_HPP

#include "QtDataSourceRef.hpp"
#include "QtDataSource.hpp"
#include "QtNode.hpp"
#include "QtList.hpp"

#include "datasources/HqlTable.hpp"

#include <vector>

class QtSelectStatement : public QtNode
{
public:
    QtSelectStatement();
    QtSelectStatement(QtList refs, QtList sources);
    ~QtSelectStatement();

    HqlTable* execute();

    DbEnum setupDbSource();

    std::string toString();

    /* Returns true if this structure can be evaluated with exactly two queries,
     one RaSQL and one SQL query. */
    bool mixedQueryIsSimple();
    
private:
    QtList what;
    QtList from;
};

#endif	/* QTSELECT_HPP */
