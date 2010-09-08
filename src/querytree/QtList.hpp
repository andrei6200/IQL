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

/** Represents a list of Query Tree Nodes. */
class QtList: public QtNode
{
public:
    QtList();
    QtList(std::vector<QtNode*> newdata);
    /** Add a new node to the end of the list. */
    void add(QtNode *elem);
    ~QtList();
    /** Executes the child nodes and adds the results column-wise. */
    HqlTable* execute();
    std::string toString();
    void print(ostream &o, std::string indent);

    DbEnum setupDbSource();

    /** Return the cartesian product of the HqlTables of its children. No restrictions. */
    HqlTable* multiplyResults();
    /** Return a table with all the columns of the HqlTables of its children. All tables
     must have the same number of elements, otherwise an exception is thrown. */
    HqlTable* addResults();

    /** Get the number of nodes of the list. */
    int length();
    /** Get the i-th node from the list. */
    QtNode* get(int index);
private:

    /** Compute the results of the children nodes. */
    void executeChildren();

    std::vector<QtNode*> data;
    std::vector<HqlTable*> results;
};

#endif	/* QTLIST_HPP */
