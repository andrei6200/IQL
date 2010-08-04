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

#include "QtNode.hpp"

class QtJoin : public QtNode
{
public:
    QtJoin();
    QtJoin(const QtJoin& orig);
    virtual ~QtJoin();
private:

};

#endif	/* QTJOIN_HPP */

