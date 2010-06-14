/* 
 * File:   rqlkeywords.h
 * Author: andrei
 *
 * Created on June 5, 2010, 12:56 PM
 */

#ifndef _RQLKEYWORDS_H
#define	_RQLKEYWORDS_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct RasqlKeyword
{
	const char *name;			/* in lower case */
	int		value;			/* grammar's token code */
} RasqlKeyword;

extern const RasqlKeyword RasqlKeywords[];
extern const RasqlKeyword *LastRasqlKeyword;

extern const RasqlKeyword *RasqlKeywordLookup(const char *text);


#ifdef	__cplusplus
}
#endif

#endif	/* _RQLKEYWORDS_H */

