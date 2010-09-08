/* *
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
#define GLOBAL_LOGGING_LEVEL TRACE_LVL

/* The name of the logfile (if the log is directed to a file, of course).
 See "logger.cpp" if you want to redirect the log output. */
#define LOGFILE_NAME "program.log"


/* Output Query prompter */
#define QUERY_PROMPT        "> "
#define RESPONSE_PROMPT     "---> "
#define INDENT_PROMPT       "\t"

/* Rasdaman connection */
/************* LOCALHOST *************/
#define RASDAMAN_SERVER    "localhost"
#define RASDAMAN_PORT       7001
#define RASDAMAN_DATABASE   "RASBASE"
#define RASDAMAN_USERNAME   "rasguest"
#define RASDAMAN_PASSWORD   "rasguest"

/* Postgresql connection defaults */
#define POSTGRES_CONN_OPTS  "dbname=gis"

#endif	/* CONFIG_HPP */
