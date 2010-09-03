/* 
 * File:   QtWhere.hpp
 * Author: andrei
 *
 * Created on September 2, 2010, 6:26 PM
 */

#ifndef QTWHERE_HPP
#define	QTWHERE_HPP

#include "QtNode.hpp"


class QtWhere  : public QtNode
{
public:
    QtWhere(QtNode* cond);
    virtual ~QtWhere();

    HqlTable* execute();
    std::string toString();
    void print(ostream &o, std::string indent);

    DbEnum setupDbSource();

//    void setSourceTable(HqlTable* src);
private:
    QtNode *condition;
    HqlTable *srcTable;
};

#endif	/* QTWHERE_HPP */
