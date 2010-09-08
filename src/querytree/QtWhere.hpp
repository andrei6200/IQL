/* 
 * File:   QtWhere.hpp
 * Author: andrei
 *
 * Created on September 2, 2010, 6:26 PM
 */

#ifndef QTWHERE_HPP
#define	QTWHERE_HPP

#include "QtNode.hpp"

/** Represents the WHERE clause of an IQL query. */
class QtWhere  : public QtNode
{
public:
    /** Public constructor */
    QtWhere(QtNode* cond);
    /** Destructor */
    virtual ~QtWhere();

    IqlTable* execute();
    std::string toString();
    void print(ostream &o, std::string indent);

    DbEnum setupDbSource();
private:
    /** Condition that is to be evaluated. */
    QtNode *condition;
};

#endif	/* QTWHERE_HPP */
