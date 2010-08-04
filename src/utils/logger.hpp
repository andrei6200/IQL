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
        os << std::endl << "[ " << timestr << " ]" << std::setw(8) << strLevel << " :  ";
        return *this;
    }

    inline LOGGER& trace()
    {
        return setLevel(TRACE_LVL, "TRACE");
    }

    inline LOGGER& debug()
    {
        return setLevel(DEBUG_LVL, "DEBUG");
    }
    
    inline LOGGER& info()
    {
        return setLevel(INFO_LVL, "INFO");
    }

    inline LOGGER& warn()
    {
        return setLevel(WARN_LVL, "WARN");
    }

    inline LOGGER& error()
    {
        return setLevel(ERROR_LVL, "ERROR");
    }

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

#define TRACE   LOG.trace()
#define DEBUG   LOG.debug()
#define INFO    LOG.info()
#define WARN    LOG.warn()
#define ERROR   LOG.error()
#define FATAL   LOG.fatal()


#endif	/* LOGGER_HPP */
