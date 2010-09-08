/*-------------------------------------------------------------------------
 *
 * rqlkeywords.c
 *	  lexical token lookup for key words in RaSQL
 *
 *-------------------------------------------------------------------------
 */

#include "grammar/parser.hpp"
#include "rqlkeywords.h"

#define RASQL_KW(a,b) {a,b},
#define lengthof(array) (sizeof (array) / sizeof ((array)[0]))
#define endof(array)        (&(array)[lengthof(array)])

const RasqlKeyword RasqlKeywords[] = {
#include "rqlkwlist.h"
};

/* End of RasqlKeywords, for use in kwlookup.c and elsewhere */
const RasqlKeyword *LastRasqlKeyword = endof(RasqlKeywords);
