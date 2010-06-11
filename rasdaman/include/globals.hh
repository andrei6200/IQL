/*
* This file is part of rasdaman community.
*
* Rasdaman community is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Rasdaman community is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
*
* Copyright 2003, 2004, 2005, 2006, 2007, 2008, 2009 Peter Baumann /
rasdaman GmbH.
*
* For more information please see <http://www.rasdaman.org>
* or contact Peter Baumann via <baumann@rasdaman.com>.
/
/**
 * SOURCE: globals.hh
 *
 * MODULE: rasdaman
 * CLASS:  Globals
 *
 * PURPOSE:
 * This class provides basic constants used across rasdaman.
 * Any value that is of external importance (default names, timeouts, ...)
 * but are NOT TO BE DELIVERED should be defined here.
 *    
 * COMMENTS:
 * - make this a real class
 * - successively collect all important values here
 *
*/

#ifndef GLOBALS_HH
#define GLOBALS_HH

/// default rasmgr listen port
#define DEFAULT_PORT		7001

/// name of env var pointing to rasdaman home dir (ex: "/opt/rasdaman")
#define RMANHOME_VAR		"RMANHOME"

/// default name of server machine, if gethostname() fails
#define DEFAULT_HOSTNAME	"localhost"

/// default database name
#define DEFAULT_DBNAME		"RASBASE"

/// name of rasmgr configuration file
#define RASMGR_CONF_FILE	"rasmgr.conf"

/// default r/o login name for client tools
#define DEFAULT_USER		"rasguest"
/// default password for this user
#define DEFAULT_PASSWD		"rasguest"

/// server log directory
#define LOGDIR          	"/log"
/// server alternative log directory, if RMANHOME not specified, defined via autoconf/automake
#define ALT_LOGDIR      	ABSOLUTE_LOG_DIR
/// log file suffix
#define LOG_SUFFIX     		"log"

/// max transfer buffer size (import clients)
#define MAX_BUFFER_SIZE 	4194304L

/// client timeout [secs]
#define CLIENT_TIMEOUT		300

#endif // GLOBALS_HH

