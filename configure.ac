#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT(hql, 0.1, a.aiordachioaie@jacobs-university.de)

AM_INIT_AUTOMAKE([foreign])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_MAKE_SET
AM_PROG_LEX
AC_PROG_YACC
AC_PROG_RANLIB

# Checks for libraries.

# Checks for header files.
#AC_FUNC_ALLOCA
#AC_CHECK_HEADERS([arpa/inet.h fcntl.h float.h inttypes.h libintl.h limits.h locale.h malloc.h netdb.h netinet/in.h stddef.h stdlib.h string.h strings.h sys/ioctl.h sys/socket.h sys/time.h unistd.h wchar.h wctype.h])

# Checks for typedefs, structures, and compiler characteristics.
#AC_HEADER_STDBOOL
#AC_TYPE_UID_T
#AC_C_INLINE
#AC_TYPE_INT16_T
#AC_TYPE_INT32_T
#AC_TYPE_INT8_T
#AC_TYPE_MODE_T
#AC_TYPE_OFF_T
#AC_TYPE_PID_T
#AC_TYPE_SIZE_T
#AC_TYPE_SSIZE_T
#AC_STRUCT_TIMEZONE
#AC_TYPE_UINT16_T
#AC_TYPE_UINT32_T
#AC_TYPE_UINT8_T

# Checks for library functions.
#AC_FUNC_ERROR_AT_LINE
#AC_FUNC_FSEEKO
#AC_FUNC_MALLOC
#AC_FUNC_REALLOC
#AC_CHECK_FUNCS([fdatasync floor ftruncate gethostname gettimeofday memset putenv rint select socket strchr strdup strstr strtol strtoul])

# --------------- General Configuration ----------------------------
####################################################################



# The Rasdaman installation path could be overwritten by a commandline option to ./configure
RASDAMAN=/var/lib/postgresql/installed/rasdaman.community
CXXFLAGS=-g 
CFLAGS=-g

AC_SUBST(RASDAMAN)
AC_SUBST(CXX)

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 src/datasources/Makefile
                 src/grammar/Makefile
                 src/postgres/Makefile
                 src/querytree/Makefile
                 src/rasdaman/Makefile])
AC_OUTPUT
