/* 
 * File:   QtList.hpp
 * Author: andrei
 *
 * Created on June 26, 2010, 2:49 PM
 *
 * Stores a list of nodes.
 */

#ifndef QTLIST_HPP
#define	QTLIST_HPP

#include "QtNode.hpp"
#include <vector>

class QtList: public QtNode
{
public:
    QtList();
    QtList(std::vector<QtNode*> newdata);
    void add(QtNode *elem);
    ~QtList();
    HqlTable* execute();
    std::string toString();
    void print(ostream &o, std::string indent);

    DbEnum setupDbSource();

    /* Return the cartesian product of the HqlTables of its children. No restrictions. */
    HqlTable* multiplyResults();
    /* Return a table with all the columns of the HqlTables of its children. All tables
     must have the same number of elements, otherwise an exception is thrown. */
    HqlTable* addResults();

    /* Vector-like functions */
    int length();
    QtNode* get(int index);
private:
    std::vector<QtNode*> data;
    std::vector<HqlTable*> results;
};

#endif	/* QTLIST_HPP */
