/*-------------------------------------------------------------------------
 *
 * kwlookup.c
 *	  lexical token lookup for key words in RaSQL
 *
 * This file was taken from PostgreSQL and adapted for RaSQL. 
 *
 * Note that this file expects that the RasqlKeywords array is defined
 * and that LastRasqlKeyword points to its element one past the last.
 *
 * Portions Copyright (c) 1996-2009, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $PostgreSQL: pgsql/src/backend/parser/kwlookup.c,v 2.2 2009/03/08 16:53:30 alvherre Exp $
 *
 *-------------------------------------------------------------------------
 */

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "rqlkeywords.h"
#define NAMEDATALEN 64

/*
 * RasqlKeywordLookup - see if a given word is a keyword
 *
 * Returns a pointer to the RasqlKeyword table entry, or NULL if no match.
 *
 * The match is done case-insensitively.  Note that we deliberately use a
 * dumbed-down case conversion that will only translate 'A'-'Z' into 'a'-'z',
 * even if we are in a locale where tolower() would produce more or different
 * translations.  This is to conform to the SQL99 spec, which says that
 * keywords are to be matched in this way even though non-keyword identifiers
 * receive a different case-normalization mapping.
 */
const RasqlKeyword *
RasqlKeywordLookup(const char *text)
{
	int			len,
				i;
	char		word[NAMEDATALEN];
	const RasqlKeyword *low;
	const RasqlKeyword *high;

	len = strlen(text);
	/* We assume all keywords are shorter than NAMEDATALEN. */
	if (len >= NAMEDATALEN)
		return NULL;

	/*
	 * Apply an ASCII-only downcasing.	We must not use tolower() since it may
	 * produce the wrong translation in some locales (eg, Turkish).
	 */
	for (i = 0; i < len; i++)
	{
		char		ch = text[i];

		if (ch >= 'A' && ch <= 'Z')
			ch += 'a' - 'A';
		word[i] = ch;
	}
	word[len] = '\0';

	/*
	 * Now do a binary search using plain strcmp() comparison.
	 */
	low = &RasqlKeywords[0];
	high = LastRasqlKeyword - 1;
	while (low <= high)
	{
            const RasqlKeyword *middle;
            int			difference;    

		middle = low + (high - low) / 2;
            	difference = strcmp(middle->name, word);
		if (difference == 0)
			return middle;
		else if (difference < 0)
			low = middle + 1;
		else
			high = middle - 1;
	}

	return NULL;
}

