/* 
 * File:   config.hpp
 * Author: andrei
 *
 * Created on June 16, 2010, 6:20 AM
 *
 * This header contains global configuration settings.
 */

#ifndef CONFIG_HPP
#define	CONFIG_HPP


/* Default Log Level. Can be one of TRACE, DEBUG, INFO, WARN, ERROR, and FATAL. */
//#define GLOBAL_LOGGING_LEVEL INFO
#define GLOBAL_LOGGING_LEVEL TRACE

/* The name of the logfile (if the log is directed to a file, of course).
 See "logger.cpp" if you want to redirect the log output. */
#define LOGFILE_NAME "program.log"


#endif	/* CONFIG_HPP */

