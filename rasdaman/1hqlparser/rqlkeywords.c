/*-------------------------------------------------------------------------
 *
 * keywords.c
 *	  lexical token lookup for key words in RaSQL
 *
 *
 * Portions Copyright (c) 1996-2009, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $PostgreSQL: pgsql/src/backend/parser/keywords.c,v 1.212 2009/03/08 16:53:30 alvherre Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "rqlkeywords.h"
#include "parser.h"

#define RASQL_KW(a,b) {a,b},
#define lengthof(array) (sizeof (array) / sizeof ((array)[0]))
#define endof(array)        (&(array)[lengthof(array)])

const RasqlKeyword RasqlKeywords[] = {
#include "rqlkwlist.h"
};

/* End of RasqlKeywords, for use in kwlookup.c and elsewhere */
const RasqlKeyword *LastRasqlKeyword = endof(RasqlKeywords);
