/* 
 * File:   QtBinaryOperation.hpp
 * Author: andrei
 *
 * Created on August 14, 2010, 12:04 PM
 */

#ifndef QTBINARYOPERATION_HPP
#define	QTBINARYOPERATION_HPP

#include "QtNode.hpp"

/** Represents a binary operation between two source objects.
 Either object may be a relational table or an array collection. */
class QtBinaryOperation : public QtNode
{
public:
    QtBinaryOperation(QtNode *node1, char* op, QtNode *node2);
    virtual ~QtBinaryOperation();

    IqlTable* execute();
    std::string toString();
    void print(ostream &o, std::string indent);

    DbEnum setupDbSource();
private:
    QtNode *child1, *child2;
    std::string op, opname;
};

#endif	/* QTBINARYOPERATION_HPP */
