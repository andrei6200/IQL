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
    void add(QtNode *elem);
    ~QtList();
    HqlTable* execute();
    std::string toString();

    DbEnum setupDbSource();

    int length();
    QtNode* get(int index);
private:
    std::vector<QtNode*> data;
};

#endif	/* QTLIST_HPP */
