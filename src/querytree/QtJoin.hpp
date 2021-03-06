/* 
 * File:   QtJoin.hpp
 * Author: andrei
 *
 * Created on August 4, 2010, 1:34 PM
 */

#ifndef QTJOIN_HPP
#define	QTJOIN_HPP

#include <map>
#include "QtNode.hpp"

/** Represents a JOIN operation between two object collections */
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

    IqlTable* execute();
    std::string toString();
    void print(ostream &o, std::string indent);

    DbEnum setupDbSource();
private:

    /** Given an SQL table name, read its datatype and return a map,
     * using as keys the unqualified column names, and as values the
     * qualified column names. For example, a column "id" from the table "sql"
     * would be represented by map["id"] = "sql_id"
     */
    std::map < std::string, std::string > getColumns(std::string tableName);

    std::string type;
    QtNode *child1, *child2;
    bool natural;
    std::vector < std::string * > * columns;
    QtNode *cond;
};

#endif	/* QTJOIN_HPP */