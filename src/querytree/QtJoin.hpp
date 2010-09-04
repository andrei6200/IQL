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
    QtJoin(QtNode* table1, QtNode* table2);
    QtJoin(QtNode* table1, QtNode* table2, char* jointype);
    QtJoin(QtNode* table1, QtNode* table2, char* jointype, bool naturaljoin);
    virtual ~QtJoin();

    HqlTable* execute();
    std::string toString();
    void print(ostream &o, std::string indent);

    void setJoinColumns(std::vector<std::string> cols);

    DbEnum setupDbSource();
private:

    std::map < std::string, std::string > getColumns(std::string tableName);

    std::string type;
    QtNode *child1, *child2;
    bool natural;
};

#endif	/* QTJOIN_HPP */
