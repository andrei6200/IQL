/* 
 * File:   logger.hpp
 * Author: andrei
 *
 * Created on June 14, 2010, 1:28 PM
 */

#ifndef LOGGER_HPP
#define	LOGGER_HPP

#include <fstream>
#include <ostream>
#include <iomanip>
#include <cstring>

class IqlTable;

#undef DEBUG
#undef TRACE
#undef INFO
#undef WARN
#undef ERROR
#undef FATAL
#undef LOG

enum verbosityLevelsEnum
{
    TRACE_LVL = 1,
    DEBUG_LVL = 100,
    INFO_LVL = 200,
    WARN_LVL = 300,
    ERROR_LVL = 400,
    FATAL_LVL = 500
};

#include "config.hpp"

#define STRLENGTH 20

static time_t rawtime;
static char timestr[STRLENGTH];

/** Extensible Template Logging class */
template<typename Char, typename Traits = char_traits<Char> >
        class LOGGER
{

public:
    typedef std::basic_ostream<Char, Traits> ostream_type;
    typedef ostream_type & (*manip_type)(ostream_type&);

    friend LOGGER<char, char_traits < char > >& operator <<
        (LOGGER<char, char_traits < char > > &o, IqlTable *a);

    /** Constructor with external outputstream. */
    LOGGER(ostream_type & os) :
        os(os), level(GLOBAL_LOGGING_LEVEL)
    {
    }

    /** Print operator for IOS manipulators. */
    LOGGER & operator<<(manip_type pfn)
    {
        /* Take verbosity level into account */
        if (level < GLOBAL_LOGGING_LEVEL)
            return *this;
        
        os << pfn;
        return *this;
    }

    /** Print operator for regular datatypes. */
    template<typename T >
            LOGGER & operator<<(T const& t)
    {
        /* Take verbosity level into account */
        if (level < GLOBAL_LOGGING_LEVEL)
            return *this;
        
        os << t << std::flush;
        return *this;
    }

    /** Set the verbosity level of the current Logger. */
    inline LOGGER& setLevel(verbosityLevelsEnum newlevel, const char* strLevel)
    {
        level = newlevel;
        /* Take verbosity level into account */
        if (level < GLOBAL_LOGGING_LEVEL)
            return *this;
        /* Write time as a string. */
        time(&rawtime);
        
        strftime(timestr, STRLENGTH, "%X", localtime(&rawtime));
        os << std::endl << "[" << timestr << "]" << std::setw(8) << strLevel << " :  ";
        return *this;
    }

    /** Convenience method for setting a TRACE logger. */
    inline LOGGER& trace()
    {
        return setLevel(TRACE_LVL, "TRACE");
    }

    /** Convenience method for setting a DEBUG logger. */
    inline LOGGER& debug()
    {
        return setLevel(DEBUG_LVL, "DEBUG");
    }

    /** Convenience method for setting a INFO logger. */
    inline LOGGER& info()
    {
        return setLevel(INFO_LVL, "INFO");
    }

    /** Convenience method for setting a WARN logger. */
    inline LOGGER& warn()
    {
        return setLevel(WARN_LVL, "WARN");
    }

    /** Convenience method for setting a ERROR logger. */
    inline LOGGER& error()
    {
        return setLevel(ERROR_LVL, "ERROR");
    }

    /** Convenience method for setting a FATAL error logger. */
    inline LOGGER& fatal()
    {
        return setLevel(FATAL_LVL, "FATAL");
    }

private:
    verbosityLevelsEnum level;
    
    ostream_type & os;
};



extern std::ofstream _outputFile;
extern LOGGER <char, char_traits < char > > LOG;
extern string formatClassName(string prettyFunction);

/** Custom printing of IqlTable objects in log. */
LOGGER<char, char_traits < char > >& operator <<
    (LOGGER<char, char_traits < char > > &o, IqlTable *a);

// Always display log information about the calling function, by processing the __PRETTY_FUNCTION__ macro
#define TRACE   LOG.trace() << "[" << formatClassName(__PRETTY_FUNCTION__) << " - " << __LINE__ << "] "
#define DEBUG   LOG.debug() << "[" << formatClassName(__PRETTY_FUNCTION__) << " - " << __LINE__ << "] "
#define INFO    LOG.info() << "[" << formatClassName(__PRETTY_FUNCTION__) << "] "
#define WARN    LOG.warn() << "[" << formatClassName(__PRETTY_FUNCTION__) << "] "
#define ERROR   LOG.error() << "[" << formatClassName(__PRETTY_FUNCTION__) << " - " << __LINE__ << "] "
#define FATAL   LOG.fatal() << "[" << formatClassName(__PRETTY_FUNCTION__) << " - " << __LINE__ << "] "


#endif	/* LOGGER_HPP */
