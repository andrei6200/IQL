/* 
 * File:   QtRasqlInterval.hpp
 * Author: andrei
 *
 * Created on July 2, 2010, 7:16 PM
 *
 * Stores information about a Rasdaman interval. 
 */

#ifndef QTRASQLINTERVAL_HPP
#define	QTRASQLINTERVAL_HPP

#include <string>
#include <ostream>
#include "QtNode.hpp"

class QtRasqlInterval: public QtNode
{
public:
    QtRasqlInterval(char *low, char *high);
    QtRasqlInterval(QtNode *low, char *high);
    QtRasqlInterval(char *low, QtNode *high);
    QtRasqlInterval(QtNode *low, QtNode *high);
    virtual ~QtRasqlInterval();
    HqlTable* execute();
    std::string toString();
    void print(ostream &o, std::string indent);

    DbEnum setupDbSource();
private:
    std::string slow, shigh;
    QtNode *clow, *chigh;
    int type;
};

#endif	/* QTRASQLINTERVAL_HPP */
