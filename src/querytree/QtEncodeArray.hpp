/* 
 * File:   QtEncodeArray.hpp
 * Author: andrei
 *
 * Created on August 4, 2010, 1:47 PM
 */

#ifndef QTENCODEARRAY_HPP
#define	QTENCODEARRAY_HPP

#include <string>
#include "QtNode.hpp"

/** Represents a Rasdaman Array Format Encode operation. */
class QtEncodeArray : public QtNode
{
public:
    QtEncodeArray(QtNode *node, std::string fmt);

    QtEncodeArray(QtNode *node, std::string fmt, std::string opt);

    virtual ~QtEncodeArray();

    std::string toString();

    DbEnum setupDbSource();

    DbEnum getDbSource();

    IqlTable* execute();

    virtual void print(ostream &o, std::string indent);

private:
    std::string format;
    std::string options;
    QtNode *child;
};

#endif	/* QTENCODEARRAY_HPP */
