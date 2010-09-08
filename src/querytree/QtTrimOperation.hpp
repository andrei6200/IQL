/* 
 * File:   QtTrimOperation.hpp
 * Author: andrei
 *
 * Created on August 31, 2010, 12:16 PM
 */

#ifndef QTTRIMOPERATION_HPP
#define	QTTRIMOPERATION_HPP

#include "QtNode.hpp"

/** Represents a Rasdaman Trim operation. */
class QtTrimOperation : public QtNode
{
public:
    QtTrimOperation(QtNode *baseExpr, QtNode* limits);
    virtual ~QtTrimOperation();

    HqlTable* execute();
    std::string toString();
    void print(std::ostream &o, std::string indent);

    DbEnum setupDbSource();
private:
    QtNode *mdd, *intervals;
};

#endif	/* QTTRIMOPERATION_HPP */
