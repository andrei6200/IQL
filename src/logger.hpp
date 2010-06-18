/* 
 * File:   logger.hpp
 * Author: andrei
 *
 * Created on June 14, 2010, 1:28 PM
 */

#ifndef LOGGER_HPP
#define	LOGGER_HPP

#include <fstream>
#include <iomanip>
#include <cstring>

#undef DEBUG

enum verbosityLevelsEnum
{
    TRACE = 1,
    DEBUG = 100,
    INFO = 200,
    WARN = 300,
    ERROR = 400,
    FATAL = 500
};

#include "config.hpp"

#define STRLENGTH 20

static time_t rawtime;
static char timestr[STRLENGTH];

template<typename Char, typename Traits = char_traits<Char> >
        class LOGGER
{

public:
    typedef std::basic_ostream<Char, Traits> ostream_type;
    typedef ostream_type & (*manip_type)(ostream_type&);

    LOGGER(ostream_type & os) :
        os(os), level(GLOBAL_LOGGING_LEVEL)
    {
    }

    LOGGER & operator<<(manip_type pfn)
    {
        /* Take verbosity level into account */
        if (level < GLOBAL_LOGGING_LEVEL)
            return *this;
        
        os << pfn;
        return *this;
    }

    template<typename T >
            LOGGER & operator<<(T const& t)
    {
        /* Take verbosity level into account */
        if (level < GLOBAL_LOGGING_LEVEL)
            return *this;
        
        os << t;
        return *this;
    }

    inline LOGGER& setLevel(verbosityLevelsEnum newlevel, const char* strLevel)
    {
        level = newlevel;
        /* Take verbosity level into account */
        if (level < GLOBAL_LOGGING_LEVEL)
            return *this;
        /* Write time as a string. */
        time(&rawtime);
        
        strftime(timestr, STRLENGTH, "%x %X", localtime(&rawtime));
        os << std::endl << "[ " << timestr << " ]" << std::setw(8) << strLevel << " - ";
        return *this;
    }

    inline LOGGER& trace()
    {
        return setLevel(TRACE, "TRACE");
    }

    inline LOGGER& debug()
    {
        return setLevel(DEBUG, "DEBUG");
    }
    
    inline LOGGER& info()
    {
        return setLevel(INFO, "INFO");
    }

    inline LOGGER& warn()
    {
        return setLevel(WARN, "WARN");
    }

    inline LOGGER& error()
    {
        return setLevel(ERROR, "ERROR");
    }

    inline LOGGER& fatal()
    {
        return setLevel(FATAL, "FATAL");
    }

private:
    verbosityLevelsEnum level;
    
    ostream_type & os;
};



extern std::ofstream _outputFile;
extern LOGGER <char, char_traits < char > > LOG;

#define TRACE   LOG.trace()
#define DEBUG   LOG.debug()
#define INFO    LOG.info()
#define WARN    LOG.warn()
#define ERROR   LOG.error()
#define FATAL   LOG.fatal() << std::endl << std::endl


#endif	/* LOGGER_HPP */
