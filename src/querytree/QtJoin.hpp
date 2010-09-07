/* 
 * File:   QtJoin.hpp
 * Author: andrei
 *
 * Created on August 4, 2010, 1:34 PM
 *
 * Represents a JOIN operation between two object collections,
 * possibly from different data sources.
 */

#ifndef QTJOIN_HPP
#define	QTJOIN_HPP

#include <map>
#include "QtNode.hpp"

class QtJoin : public QtNode
{
public:
    /* Join two tables with the given join type */
    QtJoin(QtNode* table1, QtNode* table2, char* jointype);
    /* Join two tables with the given join type, optionally using a natural join*/
    QtJoin(QtNode* table1, QtNode* table2, char* jointype, bool naturaljoin);
    /* Join two tables with the given join type, using the specified columns */
    QtJoin(QtNode* table1, QtNode* table2, char* jointype, std::vector<std::string *> *columns);
    /* Join two tables with the given join type, using a user-specified theta-condition. */
    QtJoin(QtNode* table1, QtNode* table2, char* jointype, QtNode *condition);
    /* Destructor*/
    virtual ~QtJoin();

    HqlTable* execute();
    std::string toString();
    void print(ostream &o, std::string indent);

    DbEnum setupDbSource();
private:

    std::map < std::string, std::string > getColumns(std::string tableName);

    std::string type;
    QtNode *child1, *child2;
    bool natural;
    std::vector < std::string * > * columns;
    QtNode *cond;
};

#endif	/* QTJOIN_HPP */