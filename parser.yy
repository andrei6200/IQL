%{

#include <stdio.h>
#include <string.h>

#include "str.h"

extern int yylex();

/******************************** START POSTGRESQL *********************************/

#include <ctype.h>
#include <limits.h>

/* Location tracking support --- simpler than bison's default */
#define YYLLOC_DEFAULT(Current, Rhs, N) \
	do { \
		if (N) \
			(Current) = (Rhs)[1]; \
		else \
			(Current) = (Rhs)[0]; \
	} while (0)
/******************************** END POSTGRESQL *********************************/

char* parsetree;

%}

/******************************** START POSTGRESQL *********************************/
%union
{
	char*                           ival;
	char*				chr;
	char*				str;
	char*				keyword;
	char*				boolean;
	char*				node;
	char*				list;
	char*				value;
	char*				typnam;
	char*				range;
	char*				jtype;
        char*                           alias;
        char*                           sortby;
        char*                           target;
        char*                           windef;
        char*                           jexpr;
        char*                           into;
}



%type <node>	stmt SelectStmt
%type <node>	select_no_parens select_with_parens select_clause simple_select
                opt_asymmetric

%type <ival>	opt_asc_desc opt_nulls_order

%type <str>	relation_name attr_name name opt_table

%type <list>	func_name qual_Op qual_all_Op subquery_Op

%type <range>	qualified_name

%type <str>	all_Op MathOp SpecialRuleRelation

%type <list>	stmtblock stmtmulti opt_distinct

                sort_clause opt_sort_clause sortby_list
                name_list from_clause from_list opt_array_bounds
                expr_list attrs
                target_list

                indirection opt_indirection
                group_clause select_limit
                any_operator
                TableFuncElementList opt_type_modifiers


%type <range>	OptTempTableName
%type <into>	into_clause

%type <boolean>	opt_all

%type <node>	join_outer join_qual
%type <jtype>	join_type

%type <list>	extract_list overlay_list position_list
%type <list>	substr_list trim_list
%type <list>	opt_interval interval_second
%type <node>	overlay_placing substr_from substr_for

%type <node>	select_limit_value select_offset_value

%type <node>	TableFuncElement
%type <node>	where_clause
                a_expr b_expr c_expr func_expr AexprConst indirection_el
                columnref in_expr having_clause func_table array_expr
%type <list>	row type_list array_expr_list
%type <node>	case_expr case_arg when_clause case_default
%type <list>	when_clause_list
%type <ival>	sub_type
%type <alias>	alias_clause
%type <sortby>	sortby
%type <node>	table_ref
%type <jexpr>	joined_table
%type <range>	relation_expr
%type <target>	target_el

%type <typnam>	Typename SimpleTypename ConstTypename
                GenericType Numeric opt_float
                Character ConstCharacter
                CharacterWithLength CharacterWithoutLength
                ConstDatetime ConstInterval
                Bit ConstBit BitWithLength BitWithoutLength
%type <str>	character
%type <str>	extract_arg
%type <str>	opt_charset
%type <boolean> opt_varying opt_timezone

%type <ival>	Iconst
%type <str>	Sconst
%type <str>	ColId ColLabel type_function_name

%type <keyword> unreserved_keyword type_func_name_keyword
%type <keyword> col_name_keyword reserved_keyword


%type <target>	xml_attribute_el
%type <list>	xml_attribute_list xml_attributes
%type <node>	xml_root_version opt_xml_root_standalone
%type <ival>	document_or_content
%type <boolean> xml_whitespace_option

%type <windef>	over_clause


/*
 * If you make any token changes, update the keyword table in
 * src/include/parser/kwlist.h and add new keywords to the appropriate one of
 * the reserved-or-not-so-reserved keyword lists, below; search
 * this file for "Name classification hierarchy".
 */

/* ordinary key words in alphabetical order */
%token <keyword> ABORT_P ABSOLUTE_P ACCESS ACTION ADD_P ADMIN AFTER
	AGGREGATE ALL ALSO ALTER ALWAYS ANALYSE ANALYZE AND ANY ARRAY AS ASC
	ASSERTION ASSIGNMENT ASYMMETRIC AT AUTHORIZATION

	BACKWARD BEFORE BEGIN_P BETWEEN BIGINT BINARY BIT
	BOOLEAN_P BOTH BY

	CACHE CALLED CASCADE CASCADED CASE CAST CATALOG_P CHAIN CHAR_P
	CHARACTER CHARACTERISTICS CHECK CHECKPOINT CLASS CLOSE
	CLUSTER COALESCE COLLATE COLUMN COMMENT COMMIT
	COMMITTED CONCURRENTLY CONFIGURATION CONNECTION CONSTRAINT CONSTRAINTS
	CONTENT_P CONTINUE_P CONVERSION_P COPY COST CREATE CREATEDB
	CREATEROLE CREATEUSER CROSS CSV CURRENT_P
	CURRENT_CATALOG CURRENT_DATE CURRENT_ROLE CURRENT_SCHEMA
	CURRENT_TIME CURRENT_TIMESTAMP CURRENT_USER CURSOR CYCLE

	DATA_P DATABASE DAY_P DEALLOCATE DEC DECIMAL_P DECLARE DEFAULT DEFAULTS
	DEFERRABLE DEFERRED DEFINER DELETE_P DELIMITER DELIMITERS DESC
	DICTIONARY DISABLE_P DISCARD DISTINCT DO DOCUMENT_P DOMAIN_P DOUBLE_P DROP

	EACH ELSE ENABLE_P ENCODING ENCRYPTED END_P ENUM_P ESCAPE EXCEPT
	EXCLUDING EXCLUSIVE EXECUTE EXISTS EXPLAIN EXTERNAL EXTRACT

	FALSE_P FAMILY FETCH FIRST_P FLOAT_P FOLLOWING FOR FORCE FOREIGN FORWARD
	FREEZE FROM FULL FUNCTION

	GLOBAL GRANT GRANTED GREATEST GROUP_P

	HANDLER HAVING HEADER_P HOLD HOUR_P

	IDENTITY_P IF_P ILIKE IMMEDIATE IMMUTABLE IMPLICIT_P IN_P
	INCLUDING INCREMENT INDEX INDEXES INHERIT INHERITS INITIALLY
	INNER_P INOUT INPUT_P INSENSITIVE INSERT INSTEAD INT_P INTEGER
	INTERSECT INTERVAL INTO INVOKER IS ISNULL ISOLATION

	JOIN

	KEY

	LANCOMPILER LANGUAGE LARGE_P LAST_P LC_COLLATE_P LC_CTYPE_P LEADING
	LEAST LEFT LEVEL LIKE LIMIT LISTEN LOAD LOCAL LOCALTIME LOCALTIMESTAMP
	LOCATION LOCK_P LOGIN_P

	MAPPING MATCH MAXVALUE MINUTE_P MINVALUE MODE MONTH_P MOVE

	NAME_P NAMES NATIONAL NATURAL NCHAR NEW NEXT NO NOCREATEDB
	NOCREATEROLE NOCREATEUSER NOINHERIT NOLOGIN_P NONE NOSUPERUSER
	NOT NOTHING NOTIFY NOTNULL NOWAIT NULL_P NULLIF NULLS_P NUMERIC

	OBJECT_P OF OFF OFFSET OIDS OLD ON ONLY OPERATOR OPTION OPTIONS OR
	ORDER OUT_P OUTER_P OVER OVERLAPS OVERLAY OWNED OWNER

	PARSER PARTIAL PARTITION PASSWORD PLACING PLANS POSITION
	PRECEDING PRECISION PRESERVE PREPARE PREPARED PRIMARY
	PRIOR PRIVILEGES PROCEDURAL PROCEDURE

	QUOTE

	RANGE READ REAL REASSIGN RECHECK RECURSIVE REFERENCES REINDEX
	RELATIVE_P RELEASE RENAME REPEATABLE REPLACE REPLICA RESET RESTART
	RESTRICT RETURNING RETURNS REVOKE RIGHT ROLE ROLLBACK ROW ROWS RULE

	SAVEPOINT SCHEMA SCROLL SEARCH SECOND_P SECURITY SELECT SEQUENCE
	SERIALIZABLE SERVER SESSION SESSION_USER SET SETOF SHARE
	SHOW SIMILAR SIMPLE SMALLINT SOME STABLE STANDALONE_P START STATEMENT
	STATISTICS STDIN STDOUT STORAGE STRICT_P STRIP_P SUBSTRING SUPERUSER_P
	SYMMETRIC SYSID SYSTEM_P

	TABLE TABLESPACE TEMP TEMPLATE TEMPORARY TEXT_P THEN TIME TIMESTAMP
	TO TRAILING TRANSACTION TREAT TRIGGER TRIM TRUE_P
	TRUNCATE TRUSTED TYPE_P

	UNBOUNDED UNCOMMITTED UNENCRYPTED UNION UNIQUE UNKNOWN UNLISTEN UNTIL
	UPDATE USER USING

	VACUUM VALID VALIDATOR VALUE_P VALUES VARCHAR VARIADIC VARYING
	VERBOSE VERSION_P VIEW VOLATILE

	WHEN WHERE WHITESPACE_P WINDOW WITH WITHOUT WORK WRAPPER WRITE

	XML_P XMLATTRIBUTES XMLCONCAT XMLELEMENT XMLFOREST XMLPARSE
	XMLPI XMLROOT XMLSERIALIZE

	YEAR_P YES_P

	ZONE

/* The grammar thinks these are keywords, but they are not in the kwlist.h
 * list and so can never be entered directly.  The filter in parser.c
 * creates these tokens when required.
 */
%token			NULLS_FIRST NULLS_LAST WITH_TIME

/* Special token types, not actually keywords - see the "lex" file */
%token <str>	IDENT FCONST SCONST BCONST XCONST Op
%token <ival>	ICONST PARAM

/* precedence: lowest to highest */
%nonassoc	SET				/* see relation_expr_opt_alias */
%left		UNION EXCEPT
%left		INTERSECT
%left		OR
%left		AND
%right		NOT
%right		'='
%nonassoc	'<' '>'
%nonassoc	LIKE ILIKE SIMILAR
%nonassoc	ESCAPE
%nonassoc	OVERLAPS
%nonassoc	BETWEEN
%nonassoc	IN_P
%left		POSTFIXOP		/* dummy for postfix Op rules */
/*
 * To support target_el without AS, we must give IDENT an explicit priority
 * between POSTFIXOP and Op.  We can safely assign the same priority to
 * various unreserved keywords as needed to resolve ambiguities (this can't
 * have any bad effects since obviously the keywords will still behave the
 * same as if they weren't keywords).  We need to do this for PARTITION,
 * RANGE, ROWS to support opt_existing_window_name; and for RANGE, ROWS
 * so that they can follow a_expr without creating
 * postfix-operator problems.
 */
%nonassoc	IDENT PARTITION RANGE ROWS
%left		Op OPERATOR		/* multi-character ops and user-defined operators */
%nonassoc	NOTNULL
%nonassoc	ISNULL
%nonassoc	IS NULL_P TRUE_P FALSE_P UNKNOWN /* sets precedence for IS NULL, etc */
%left		'+' '-'
%left		'*' '/' '%'
%left		'^'
/* Unary Operators */
%left		AT ZONE			/* sets precedence for AT TIME ZONE */
%right		UMINUS
%left		'[' ']'
%left		'(' ')'
%left		TYPECAST
%left		'.'
/*
 * These might seem to be low-precedence, but actually they are not part
 * of the arithmetic hierarchy at all in their use as JOIN operators.
 * We make them high-precedence to support their use as function names.
 * They wouldn't be given a precedence at all, were it not that we need
 * left-associativity among the JOIN rules themselves.
 */
%left		JOIN CROSS LEFT FULL RIGHT INNER_P NATURAL
/* kluge to keep xml_whitespace_option from causing shift/reduce conflicts */
%right		PRESERVE STRIP_P


/******************************** END POSTGRESQL *********************************/

%token <str> TYPECAST

%error-verbose
%locations
%expect 0



%%


/******************************** START POSTGRESQL *********************************/
/*
 *	Handle comment-only lines, and ;; SELECT * FROM pg_class ;;;
 *	psql already handles such cases, but other interfaces don't.
 *	bjm 1999/10/05
 */
stmtblock:	stmtmulti
                    { parsetree = $1; }
		;

/* the thrashing around here is to discard "empty" statements... */
stmtmulti:	stmtmulti ';' stmt
				{ if ($3 != NULL)
					{
					$$ = cat3($1, "\n", $3);
					printf("\t*** Single statement (in a series): %s\n", $3);
					}
				  else
				  {
					$$ = $1;
					printf("\t*** Empty statement (in a series). \n");
					}
				}
			| stmt
					{ if ($1 != NULL)
					{
						$$ = $1;
						printf("\t*** Single unique statement: %s\n", $1);
						}
					  else
					  {
						$$ = NULL;
						printf("\t*** Single empty statement.\n");
					  }
					}
		;

stmt:	SelectStmt  { $$ = $1; }
        | // Empty
                { $$ = NULL; }

/*****************************************************************************
 *
 *		QUERY:
 *				SELECT STATEMENTS
 *
 *****************************************************************************/

/* A complete SELECT statement looks like this.
 *
 * The rule returns either a single SelectStmt node or a tree of them,
 * representing a set-operation tree.
 *
 * There is an ambiguity when a sub-SELECT is within an a_expr and there
 * are excess parentheses: do the parentheses belong to the sub-SELECT or
 * to the surrounding a_expr?  We don't really care, but yacc wants to know.
 * To resolve the ambiguity, we are careful to define the grammar so that
 * the decision is staved off as long as possible: as long as we can keep
 * absorbing parentheses into the sub-SELECT, we will do so, and only when
 * it's no longer possible to do that will we decide that parens belong to
 * the expression.	For example, in "SELECT (((SELECT 2)) + 3)" the extra
 * parentheses are treated as part of the sub-select.  The necessity of doing
 * it that way is shown by "SELECT (((SELECT 2)) UNION SELECT 2)".	Had we
 * parsed "((SELECT 2))" as an a_expr, it'd be too late to go back to the
 * SELECT viewpoint when we see the UNION.
 *
 * This approach is implemented by defining a nonterminal select_with_parens,
 * which represents a SELECT with at least one outer layer of parentheses,
 * and being careful to use select_with_parens, never '(' SelectStmt ')',
 * in the expression grammar.  We will then have shift-reduce conflicts
 * which we can resolve in favor of always treating '(' <select> ')' as
 * a select_with_parens.  To resolve the conflicts, the productions that
 * conflict with the select_with_parens productions are manually given
 * precedences lower than the precedence of ')', thereby ensuring that we
 * shift ')' (and then reduce to select_with_parens) rather than trying to
 * reduce the inner <select> nonterminal to something else.  We use UMINUS
 * precedence for this, which is a fairly arbitrary choice.
 *
 * To be able to define select_with_parens itself without ambiguity, we need
 * a nonterminal select_no_parens that represents a SELECT structure with no
 * outermost parentheses.  This is a little bit tedious, but it works.
 *
 * In non-expression contexts, we use SelectStmt which can represent a SELECT
 * with or without outer parentheses.
 */

SelectStmt:     select_no_parens			%prec UMINUS
			| select_with_parens		%prec UMINUS
		;

select_with_parens:
			'(' select_no_parens ')'			{ $$ = cat3("(", $2, ")"); }
			| '(' select_with_parens ')'			{ $$ = cat3("(", $2, ")"); }
		;

/*
 * This rule parses the equivalent of the standard's <query expression>.
 * The duplicative productions are annoying, but hard to get rid of without
 * creating shift/reduce conflicts.
 *
 *	FOR UPDATE/SHARE may be before or after LIMIT/OFFSET.
 *	In <=7.2.X, LIMIT/OFFSET had to be after FOR UPDATE
 *	We now support both orderings, but prefer LIMIT/OFFSET before FOR UPDATE/SHARE
 *	2002-08-28 bjm
 */
select_no_parens:
			simple_select						{ $$ = $1; }
			| select_clause sort_clause
				{
                            $$ = cat2($1, $2);
				}
/*
// AA: We do not want to support the locking clause FOR READ ONLY
			| select_clause opt_sort_clause for_locking_clause opt_select_limit
			| select_clause opt_sort_clause select_limit opt_for_locking_clause
*/
			| select_clause opt_sort_clause select_limit
// AA: We do not want to support sql nested queries
/*
			| with_clause select_clause
			| with_clause select_clause sort_clause
			| with_clause select_clause opt_sort_clause for_locking_clause opt_select_limit
			| with_clause select_clause opt_sort_clause select_limit opt_for_locking_clause
*/
		;

select_clause:
			simple_select						{ $$ = $1; }
			| select_with_parens					{ $$ = $1; }
		;

/*
 * This rule parses SELECT statements that can appear within set operations,
 * including UNION, INTERSECT and EXCEPT.  '(' and ')' can be used to specify
 * the ordering of the set operations.	Without '(' and ')' we want the
 * operations to be ordered per the precedence specs at the head of this file.
 *
 * As with select_no_parens, simple_select cannot have outer parentheses,
 * but can have parenthesized subclauses.
 *
 * Note that sort clauses cannot be included at this level --- SQL92 requires
 *		SELECT foo UNION SELECT bar ORDER BY baz
 * to be parsed as
 *		(SELECT foo UNION SELECT bar) ORDER BY baz
 * not
 *		SELECT foo UNION (SELECT bar ORDER BY baz)
 * Likewise for WITH, FOR UPDATE and LIMIT.  Therefore, those clauses are
 * described as part of the select_no_parens production, not simple_select.
 * This does not limit functionality, because you can reintroduce these
 * clauses inside parentheses.
 *
 * NOTE: only the leftmost component SelectStmt should have INTO.
 * However, this is not checked by the grammar; parse analysis must check it.
 */
simple_select:
			SELECT opt_distinct target_list
			into_clause from_clause where_clause
// AA: We do not want to support SQL windows
//			group_clause having_clause window_clause		
			group_clause having_clause
				{
    $$ = cat8($1, $2, $3, $4, $5, $6, $7, $8);
				}
// AA: Disable VALUES clause, we do not want data to be input at run-time
//			| values_clause							{ $$ = $1; }
			| TABLE relation_expr
				{
                            /* same as SELECT * FROM relation_expr */
                            $$ = cat2($1, $2);
				}
			| select_clause UNION opt_all select_clause
				{
                            $$ = cat4($1, $2, $3, $4);
				}
			| select_clause INTERSECT opt_all select_clause
				{
                            $$ = cat4($1, $2, $3, $4);
				}
			| select_clause EXCEPT opt_all select_clause
				{
                            $$ = cat4($1, $2, $3, $4);
				}
		;

into_clause:
			INTO OptTempTableName
				{
    $$ = cat2($1, $2);
				}
			| /*EMPTY*/
				{ $$ = NULL; }
		;

/*
 * Redundancy here is needed to avoid shift/reduce conflicts,
 * since TEMP is not a reserved word.  See also OptTemp.
 */
OptTempTableName:
			TEMPORARY opt_table qualified_name
				{
    $$ = cat3($1, $2, $3);
				}
			| TEMP opt_table qualified_name
				{
                            $$ = cat3($1, $2, $3);
				}
			| LOCAL TEMPORARY opt_table qualified_name
				{
                            $$ = cat4($1, $2, $3, $4);
				}
			| LOCAL TEMP opt_table qualified_name
				{
                            $$ = cat4($1, $2, $3, $4);
				}
			| GLOBAL TEMPORARY opt_table qualified_name
				{
                            $$ = cat4($1, $2, $3, $4);
				}
			| GLOBAL TEMP opt_table qualified_name
				{
                            $$ = cat4($1, $2, $3, $4);
				}
			| TABLE qualified_name
				{
                            $$ = cat2($1, $2);
				}
			| qualified_name
				{
                            $$ = $1;
				}
		;

opt_table:	TABLE									{ $$ = "TABLE"; }
			| /*EMPTY*/								{ $$ = NULL; }
		;

opt_all:	ALL										{ $$ = "ALL"; }
			| DISTINCT								{ $$ = "DISTINCT"; }
			| /*EMPTY*/								{ $$ = NULL; }
		;

/* We use (NULL) as a placeholder to indicate that all target expressions
 * should be placed in the DISTINCT list during parsetree analysis.
 */
opt_distinct:
			DISTINCT								{ $$ = "DISTINCT"; }
			| DISTINCT ON '(' expr_list ')'			{ $$ = cat5($1, $2, "(", $4, ")"); }
			| ALL									{ $$ = "ALL"; }
			| /*EMPTY*/								{ $$ = NULL; }
		;

opt_sort_clause:
			sort_clause								{ $$ = $1;}
			| /*EMPTY*/								{ $$ = NULL; }
		;

sort_clause:
			ORDER BY sortby_list					{ $$ = cat3($1, $2, $3); }
		;

sortby_list:
			sortby									{ $$ = $1; }
			| sortby_list ',' sortby				{ $$ = cat3($1, ",", $3); }
		;

sortby:		a_expr USING qual_all_Op opt_nulls_order
				{
    $$ = cat4($1, $2, $3, $4);
				}
			| a_expr opt_asc_desc opt_nulls_order
				{
                            $$ = cat3($1, $2, $3);
				}
		;


opt_asc_desc: ASC                           { $$ = "SORTBY_ASC"; }
			| DESC                          { $$ = "SORTBY_DESC"; }
			| /*EMPTY*/                     { $$ = "SORTBY_DEFAULT"; }
			;

opt_nulls_order: NULLS_FIRST                { $$ = "SORTBY_NULLS_FIRST"; }
				| NULLS_LAST                    { $$ = "SORTBY_NULLS_LAST"; }
				| /*EMPTY*/                     { $$ = "SORTBY_NULLS_DEFAULT"; }
				;

select_limit:
			LIMIT select_limit_value OFFSET select_offset_value
				{ $$ = cat4($1, $2, $3, $4); }
			| OFFSET select_offset_value LIMIT select_limit_value
				{ $$ = cat4($1, $2, $3, $4); }
			| LIMIT select_limit_value
				{ $$ = cat2($1, $2); }
			| OFFSET select_offset_value
				{ $$ = cat2($1, $2); }
			| LIMIT select_limit_value ',' select_offset_value
				{
					/* Disabled because it was too confusing, bjm 2002-02-18 */
                                        $$ = cat4($1, $2, ",", $4);
                                        // TODO: Report error
				}
			/* SQL:2008 syntax variants */
/*			| OFFSET select_offset_value2 row_or_rows
			| FETCH first_or_next opt_select_fetch_first_value row_or_rows ONLY
			| OFFSET select_offset_value2 row_or_rows FETCH first_or_next opt_select_fetch_first_value row_or_rows ONLY
*/
		;

select_limit_value:
			a_expr									{ $$ = $1; }
			| ALL
				{
					/* LIMIT ALL is represented as a NULL constant */
                                        $$ = $1;
				}
		;


select_offset_value:
			a_expr									{ $$ = $1; }
		;


group_clause:
			GROUP_P BY expr_list					{ $$ = cat3($1, $2, $3); }
			| /*EMPTY*/								{ $$ = NULL; }
		;

having_clause:
			HAVING a_expr							{ $$ = cat2($1, $2); }
			| /*EMPTY*/								{ $$ = NULL; }
		;


/*****************************************************************************
 *
 *	clauses common to all Optimizable Stmts:
 *		from_clause		- allow list of both JOIN expressions and table names
 *		where_clause	- qualifications for joins or restrictions
 *
 *****************************************************************************/

from_clause:
			FROM from_list							{ $$ = cat2($1, $2); }
			| /*EMPTY*/								{ $$ = NULL; }
		;

from_list:
			table_ref								{ $$ = $1; }
			| from_list ',' table_ref				{ $$ = cat3($1, ",", $3); }
		;

/*
 * table_ref is where an alias clause can be attached.	Note we cannot make
 * alias_clause have an empty production because that causes parse conflicts
 * between table_ref := '(' joined_table ')' alias_clause
 * and joined_table := '(' joined_table ')'.  So, we must have the
 * redundant-looking productions here instead.
 */
table_ref:	relation_expr
				{
					$$ = $1;
				}
			| relation_expr alias_clause
				{
                                        $$ = cat2($1, $2);
				}
			| func_table
				{
                                        $$ = $1;
				}
			| func_table alias_clause
				{
                                        $$ = cat2($1, $2);
				}
			| func_table AS '(' TableFuncElementList ')'
				{
                                        $$ = cat5($1, $2, "(", $4, ")");
				}
			| func_table AS ColId '(' TableFuncElementList ')'
				{
                                        $$ = cat6($1, $2, $3, "(", $5, ")");
				}
			| func_table ColId '(' TableFuncElementList ')'
				{
                                        $$ = cat5($1, $2, "(", $4, ")");
				}
// AA: We do not allow subqueries in the FROM clause
/*			| select_with_parens
			| select_with_parens alias_clause
 */
			| joined_table
				{
					$$ = $1;
				}
			| '(' joined_table ')' alias_clause
				{
                                        $$ = cat4("(", $2, ")", $4);
				}
		;


/*
 * It may seem silly to separate joined_table from table_ref, but there is
 * method in SQL92's madness: if you don't do it this way you get reduce-
 * reduce conflicts, because it's not clear to the parser generator whether
 * to expect alias_clause after ')' or not.  For the same reason we must
 * treat 'JOIN' and 'join_type JOIN' separately, rather than allowing
 * join_type to expand to empty; if we try it, the parser generator can't
 * figure out when to reduce an empty join_type right after table_ref.
 *
 * Note that a CROSS JOIN is the same as an unqualified
 * INNER JOIN, and an INNER JOIN/ON has the same shape
 * but a qualification expression to limit membership.
 * A NATURAL JOIN implicitly matches column names between
 * tables and the shape is determined by which columns are
 * in common. We'll collect columns during the later transformations.
 */

joined_table:
			'(' joined_table ')'
				{
                                    $$ = cat3("(", $2, ")");
				}
			| table_ref CROSS JOIN table_ref
				{
					/* CROSS JOIN is same as unqualified inner join */
                                        $$ = cat4($1, $2, $3, $4);
				}
			| table_ref join_type JOIN table_ref join_qual
				{
                                        $$ = cat5($1, $2, $3, $4, $5);
				}
			| table_ref JOIN table_ref join_qual
				{
					/* letting join_type reduce to empty doesn't work */
                                        $$ = cat4($1, $2, $3, $4);
				}
			| table_ref NATURAL join_type JOIN table_ref
				{
                                        $$ = cat5($1, $2, $3, $4, $5);
				}
			| table_ref NATURAL JOIN table_ref
				{
					/* letting join_type reduce to empty doesn't work */
                                        $$ = cat4($1, $2, $3, $4);
				}
		;

alias_clause:
			AS ColId '(' name_list ')'
				{
                                        $$ = cat5($1, $2, "(", $4, ")");
				}
			| AS ColId
				{
                                        $$ = cat2($1, $2);
				}
			| ColId '(' name_list ')'
				{
                                        $$ = cat4($1, "(", $3, ")");
				}
			| ColId
				{
                                        $$ = $1;
				}
		;

join_type:	FULL join_outer							{ $$ = "JOIN_FULL"; }
			| LEFT join_outer						{ $$ = "JOIN_LEFT"; }
			| RIGHT join_outer						{ $$ = "JOIN_RIGHT"; }
			| INNER_P								{ $$ = "JOIN_INNER"; }
		;

/* OUTER is just noise... */
join_outer: OUTER_P									{ $$ = NULL; }
			| /*EMPTY*/								{ $$ = NULL; }
		;

/* JOIN qualification clauses
 * Possibilities are:
 *	USING ( column list ) allows only unqualified column names,
 *						  which must match between tables.
 *	ON expr allows more general qualifications.
 *
 * We return USING as a List node, while an ON-expr will not be a List.
 */

join_qual:	USING '(' name_list ')'					{ $$ = cat4($1, "(", $3, ")"); }
			| ON a_expr								{ $$ = cat2($1, $2); }
		;


relation_expr:
			qualified_name
				{
					/* default inheritance */
                                        $$ = $1;
				}
			| qualified_name '*'
				{
					/* inheritance query */
                                        $$ = cat2($1, "*");
				}
			| ONLY qualified_name
				{
					/* no inheritance */
                                        $$ = cat2($1, $2);
				}
			| ONLY '(' qualified_name ')'
				{
					/* no inheritance, SQL99-style syntax */
                                        $$ = cat4($1, "(", $3, ")");
				}
		;



func_table: func_expr								{ $$ = $1; }
		;


where_clause:
			WHERE a_expr							{ $$ = cat2($1, $2); }
			| /*EMPTY*/								{ $$ = NULL; }
		;

/* variant for UPDATE and DELETE */

TableFuncElementList:
			TableFuncElement
				{
					$$ = $1;
				}
			| TableFuncElementList ',' TableFuncElement
				{
					$$ = cat3($1, ",", $3);
				}
		;

TableFuncElement:	ColId Typename
				{
                                        $$ = cat2($1, $2);
				}
		;

/*****************************************************************************
 *
 *	Type syntax
 *		SQL92 introduces a large amount of type-specific syntax.
 *		Define individual clauses to handle these cases, and use
 *		 the generic case to handle regular type-extensible Postgres syntax.
 *		- thomas 1997-10-10
 *
 *****************************************************************************/

Typename:	SimpleTypename opt_array_bounds
				{
                                        $$ = cat2($1, $2);
				}
			| SETOF SimpleTypename opt_array_bounds
				{
					$$ = cat3($1, $2, $3);
				}
			/* SQL standard syntax, currently only one-dimensional */
			| SimpleTypename ARRAY '[' Iconst ']'
				{
                                        $$ = cat5($1, $2, "[", $4, "]");
				}
			| SETOF SimpleTypename ARRAY '[' Iconst ']'
				{
					$$ = cat6($1, $2, $3, "[", $5, "]");
				}
			| SimpleTypename ARRAY
				{
                                        $$ = cat2($1, $2);
				}
			| SETOF SimpleTypename ARRAY
				{
					$$ = cat3($1, $2, $3);
				}
		;

opt_array_bounds:
			opt_array_bounds '[' ']'
					{  $$ = cat3($1, "[", "]"); }
			| opt_array_bounds '[' Iconst ']'
					{  $$ = cat4($1, "[", $3, "]"); }
			| /*EMPTY*/
					{  $$ = NULL; }
		;

SimpleTypename:
			GenericType								{ $$ = $1; }
			| Numeric								{ $$ = $1; }
			| Bit									{ $$ = $1; }
			| Character								{ $$ = $1; }
			| ConstDatetime							{ $$ = $1; }
			| ConstInterval opt_interval
				{
                                        $$ = cat2($1, $2);
				}
			| ConstInterval '(' Iconst ')' opt_interval
				{
                                        $$ = cat5($1, "(", $3, ")", $5);
				}
		;

/* We have a separate ConstTypename to allow defaulting fixed-length
 * types such as CHAR() and BIT() to an unspecified length.
 * SQL9x requires that these default to a length of one, but this
 * makes no sense for constructs like CHAR 'hi' and BIT '0101',
 * where there is an obvious better choice to make.
 * Note that ConstInterval is not included here since it must
 * be pushed up higher in the rules to accomodate the postfix
 * options (e.g. INTERVAL '1' YEAR). Likewise, we have to handle
 * the generic-type-name case in AExprConst to avoid premature
 * reduce/reduce conflicts against function names.
 */
ConstTypename:
			Numeric								{ $$ = $1; }
			| ConstBit							{ $$ = $1; }
			| ConstCharacter						{ $$ = $1; }
			| ConstDatetime							{ $$ = $1; }
		;

/*
 * GenericType covers all type names that don't have special syntax mandated
 * by the standard, including qualified names.  We also allow type modifiers.
 * To avoid parsing conflicts against function invocations, the modifiers
 * have to be shown as expr_list here, but parse analysis will only accept
 * constants for them.
 */
GenericType:
			type_function_name opt_type_modifiers
				{
                                        $$ = cat2($1, $2);
				}
			| type_function_name attrs opt_type_modifiers
				{
                                        $$ = cat3($1, $2, $3);
				}
		;

opt_type_modifiers: '(' expr_list ')'				{ $$ = cat3("(", $2, ")"); }
					| /* EMPTY */					{ $$ = NULL; }
		;

/*
 * SQL92 numeric data types
 */
Numeric:	INT_P
				{
                                        $$ = $1;
				}
			| INTEGER
				{
                                        $$ = $1;
				}
			| SMALLINT
				{
                                        $$ = $1;
				}
			| BIGINT
				{
                                        $$ = $1;
				}
			| REAL
				{
                                        $$ = $1;
				}
			| FLOAT_P opt_float
				{
					$$ = cat2($1, $2);
				}
			| DOUBLE_P PRECISION
				{
					$$ = cat2($1, $2);
				}
			| DECIMAL_P opt_type_modifiers
				{
					$$ = cat2($1, $2);
				}
			| DEC opt_type_modifiers
				{
					$$ = cat2($1, $2);
				}
			| NUMERIC opt_type_modifiers
				{
					$$ = cat2($1, $2);
				}
			| BOOLEAN_P
				{
                                        $$ = $1;
				}
		;

opt_float:	'(' Iconst ')'
				{
					/*
					 * Check FLOAT() precision limits assuming IEEE floating
					 * types - thomas 1997-09-18
					 */
                                        $$ = cat3("(", $2, ")");
				}
			| /*EMPTY*/
				{
					$$ = "empty";
				}
		;

/*
 * SQL92 bit-field data types
 * The following implements BIT() and BIT VARYING().
 */
Bit:		BitWithLength
				{
					$$ = $1;
				}
			| BitWithoutLength
				{
					$$ = $1;
				}
		;

/* ConstBit is like Bit except "BIT" defaults to unspecified length */
/* See notes for ConstCharacter, which addresses same issue for "CHAR" */
ConstBit:	BitWithLength
				{
					$$ = $1;
				}
			| BitWithoutLength
				{
					$$ = $1;
				}
		;

BitWithLength:
			BIT opt_varying '(' expr_list ')'
				{
                                        $$ = cat5($1, $2, "(", $4, ")");
				}
		;

BitWithoutLength:
			BIT opt_varying
				{
					/* bit defaults to bit(1), varbit to no limit */
                                        $$ = cat2($1, $2);
				}
		;


/*
 * SQL92 character data types
 * The following implements CHAR() and VARCHAR().
 */
Character:  CharacterWithLength
				{
					$$ = $1;
				}
			| CharacterWithoutLength
				{
					$$ = $1;
				}
		;

ConstCharacter:  CharacterWithLength
				{
					$$ = $1;
				}
			| CharacterWithoutLength
				{
					/* Length was not specified so allow to be unrestricted.
					 * This handles problems with fixed-length (bpchar) strings
					 * which in column definitions must default to a length
					 * of one, but should not be constrained if the length
					 * was not specified.
					 */
					$$ = $1;
				}
		;

CharacterWithLength:  character '(' Iconst ')' opt_charset
				{
                                        $$ = cat5($1, "(", $3, ")", $5);
				}
		;

CharacterWithoutLength:	 character opt_charset
				{
                                        $$ = cat2($1, $2);
				}
		;

character:	CHARACTER opt_varying
										{ $$ = cat2($1, $2); }
			| CHAR_P opt_varying
										{ $$ = cat2($1, $2); }
			| VARCHAR
										{ $$ = "varchar"; }
			| NATIONAL CHARACTER opt_varying
										{ $$ = cat3($1, $2, $3); }
			| NATIONAL CHAR_P opt_varying
										{ $$ = cat3($1, $2, $3); }
			| NCHAR opt_varying
										{ $$ = cat2($1, $2); }
		;

opt_varying:
			VARYING									{ $$ = "TRUE"; }
			| /*EMPTY*/								{ $$ = "FALSE"; }
		;

opt_charset:
			CHARACTER SET ColId						{ $$ = $3; }
			| /*EMPTY*/								{ $$ = NULL; }
		;

/*
 * SQL92 date/time types
 */
ConstDatetime:
			TIMESTAMP '(' Iconst ')' opt_timezone
				{
                                        $$ = cat5($1, "(", $3, ")", $5);
				}
			| TIMESTAMP opt_timezone
				{
                                        $$ = cat2($1, $2);
				}
			| TIME '(' Iconst ')' opt_timezone
				{
                                        $$ = cat5($1, "(", $3, ")", $5);
				}
			| TIME opt_timezone
				{
                                        $$ = cat2($1, $2);
				}
		;

ConstInterval:
			INTERVAL
				{
                                        $$ = $1;
				}
		;

opt_timezone:
			WITH_TIME ZONE							{ $$ = "WITH TIMEZONE"; }
			| WITHOUT TIME ZONE						{ $$ = "WITHOUT TIMEZONE"; }
			| /*EMPTY*/							{ $$ = "EMPTY (WITHOUT TIMEZONE)"; }
		;

opt_interval:
			YEAR_P
				{ $$ = $1; }
			| MONTH_P
				{ $$ = $1; }
			| DAY_P
				{ $$ = $1; }
			| HOUR_P
				{ $$ = $1; }
			| MINUTE_P
				{ $$ = $1; }
			| interval_second
				{ $$ = $1; }
			| YEAR_P TO MONTH_P
				{
                                    $$ = cat3($1, $2, $3);
				}
			| DAY_P TO HOUR_P
				{
                                    $$ = cat3($1, $2, $3);
				}
			| DAY_P TO MINUTE_P
				{
                                    $$ = cat3($1, $2, $3);
				}
			| DAY_P TO interval_second
				{
                                    $$ = cat3($1, $2, $3);
				}
			| HOUR_P TO MINUTE_P
				{
                                    $$ = cat3($1, $2, $3);
				}
			| HOUR_P TO interval_second
				{
                            $$ = cat3($1, $2, $3);
				}
			| MINUTE_P TO interval_second
				{
                            $$ = cat3($1, $2, $3);
				}
			| /*EMPTY*/
				{ $$ = NULL; }
		;

interval_second:
			SECOND_P
				{
                                    $$ = $1;
				}
			| SECOND_P '(' Iconst ')'
				{
                            $$ = cat4($1, "(", $3, ")");
				}
		;


/*****************************************************************************
 *
 *	expression grammar
 *
 *****************************************************************************/

/*
 * General expressions
 * This is the heart of the expression syntax.
 *
 * We have two expression types: a_expr is the unrestricted kind, and
 * b_expr is a subset that must be used in some places to avoid shift/reduce
 * conflicts.  For example, we can't do BETWEEN as "BETWEEN a_expr AND a_expr"
 * because that use of AND conflicts with AND as a boolean operator.  So,
 * b_expr is used in BETWEEN and we remove boolean keywords from b_expr.
 *
 * Note that '(' a_expr ')' is a b_expr, so an unrestricted expression can
 * always be used by surrounding it with parens.
 *
 * c_expr is all the productions that are common to a_expr and b_expr;
 * it's factored out just to eliminate redundant coding.
 */
a_expr:		c_expr									{ $$ = $1; }
			| a_expr TYPECAST Typename
					{ $$ = cat3($1, $2, $3); }
			| a_expr AT TIME ZONE a_expr
				{
                                        $$ = cat5($1, $2, $3, $4, $5);
				}
		/*
		 * These operators must be called out explicitly in order to make use
		 * of yacc/bison's automatic operator-precedence handling.  All other
		 * operator names are handled by the generic productions using "Op",
		 * below; and all those operators will have the same precedence.
		 *
		 * If you add more explicitly-known operators, be sure to add them
		 * also to b_expr and to the MathOp list above.
		 */
			| '+' a_expr					%prec UMINUS
                                { $$ = cat2("+", $2); }
			| '-' a_expr					%prec UMINUS
				{ $$ = cat2("-", $2); }
			| a_expr '+' a_expr
				{ $$ = cat3($1, "+", $3); }
			| a_expr '-' a_expr
				{ $$ = cat3($1, "-", $3); }
			| a_expr '*' a_expr
				{ $$ = cat3($1, "*", $3); }
			| a_expr '/' a_expr
				{ $$ = cat3($1, "/", $3); }
			| a_expr '%' a_expr
				{ $$ = cat3($1, "%", $3); }
			| a_expr '^' a_expr
				{ $$ = cat3($1, "^", $3); }
			| a_expr '<' a_expr
				{ $$ = cat3($1, "<", $3); }
			| a_expr '>' a_expr
				{ $$ = cat3($1, ">", $3); }
			| a_expr '=' a_expr
				{ $$ = cat3($1, "=", $3); }

			| a_expr qual_Op a_expr				%prec Op
				{ $$ = cat3($1, $2, $3); }
			| qual_Op a_expr					%prec Op
				{ $$ = cat2($1, $2); }
			| a_expr qual_Op					%prec POSTFIXOP
				{ $$ = cat2($1, $2); }

			| a_expr AND a_expr
				{ $$ = cat3($1, $2, $3); }
			| a_expr OR a_expr
				{ $$ = cat3($1, $2, $3); }
			| NOT a_expr
				{ $$ = cat2($1, $2); }

			| a_expr LIKE a_expr
				{ $$ = cat3($1, $2, $3); }
			| a_expr LIKE a_expr ESCAPE a_expr
				{ $$ = cat5($1, $2, $3, $4, $5); }
			| a_expr NOT LIKE a_expr
				{ $$ = cat4($1, $2, $3, $4); }
			| a_expr NOT LIKE a_expr ESCAPE a_expr
				{ $$ = cat6($1, $2, $3, $4, $5, $6); }
			| a_expr ILIKE a_expr
				{ $$ = cat3($1, $2, $3); }
			| a_expr ILIKE a_expr ESCAPE a_expr
				{ $$ = cat5($1, $2, $3, $4, $5); }
			| a_expr NOT ILIKE a_expr
				{ $$ = cat4($1, $2, $3, $4); }
			| a_expr NOT ILIKE a_expr ESCAPE a_expr
				{ $$ = cat6($1, $2, $3, $4, $5, $6); }

			| a_expr SIMILAR TO a_expr				%prec SIMILAR
				{ $$ = cat4($1, $2, $3, $4); }
			| a_expr SIMILAR TO a_expr ESCAPE a_expr
				{ $$ = cat6($1, $2, $3, $4, $5, $6); }
			| a_expr NOT SIMILAR TO a_expr			%prec SIMILAR
				{ $$ = cat5($1, $2, $3, $4, $5); }
			| a_expr NOT SIMILAR TO a_expr ESCAPE a_expr
				{ $$ = cat7($1, $2, $3, $4, $5, $6, $7); }

			/* NullTest clause
			 * Define SQL92-style Null test clause.
			 * Allow two forms described in the standard:
			 *	a IS NULL
			 *	a IS NOT NULL
			 * Allow two SQL extensions
			 *	a ISNULL
			 *	a NOTNULL
			 */
			| a_expr IS NULL_P
				{ $$ = cat3($1, $2, $3); }
			| a_expr ISNULL
				{ $$ = cat2($1, $2); }
			| a_expr IS NOT NULL_P
				{ $$ = cat4($1, $2, $3, $4); }
			| a_expr NOTNULL
				{ $$ = cat2($1, $2); }
			| row OVERLAPS row
				{ $$ = cat3($1, $2, $3); }
			| a_expr IS TRUE_P
				{ $$ = cat3($1, $2, $3); }
			| a_expr IS NOT TRUE_P
				{ $$ = cat4($1, $2, $3, $4); }
			| a_expr IS FALSE_P
				{ $$ = cat3($1, $2, $3); }
			| a_expr IS NOT FALSE_P
				{ $$ = cat4($1, $2, $3, $4); }
			| a_expr IS UNKNOWN
				{ $$ = cat3($1, $2, $3); }
			| a_expr IS NOT UNKNOWN
				{ $$ = cat4($1, $2, $3, $4); }
			| a_expr IS DISTINCT FROM a_expr			%prec IS
				{
                                        $$ = cat5($1, $2, $3, $4, $5);
				}
			| a_expr IS NOT DISTINCT FROM a_expr		%prec IS
				{
                            $$ = cat6($1, $2, $3, $4, $5, $6);
				}
			| a_expr IS OF '(' type_list ')'			%prec IS
				{
                            $$ = cat6($1, $2, $3, "(", $5, ")");
				}
			| a_expr IS NOT OF '(' type_list ')'		%prec IS
				{
                            $$ = cat7($1, $2, $3, $4, "(", $6, ")");
				}
			/*
			 *	Ideally we would not use hard-wired operators below but instead use
			 *	opclasses.  However, mixed data types and other issues make this
			 *	difficult:  http://archives.postgresql.org/pgsql-hackers/2008-08/msg01142.php
			 */
			| a_expr BETWEEN opt_asymmetric b_expr AND b_expr		%prec BETWEEN
				{
                            $$ = cat6($1, $2, $3, $4, $5, $6);
				}
			| a_expr NOT BETWEEN opt_asymmetric b_expr AND b_expr	%prec BETWEEN
				{
                            $$ = cat7($1, $2, $3, $4, $5, $6, $7);
				}
			| a_expr BETWEEN SYMMETRIC b_expr AND b_expr			%prec BETWEEN
				{
                            $$ = cat6($1, $2, $3, $4, $5, $6);
				}
			| a_expr NOT BETWEEN SYMMETRIC b_expr AND b_expr		%prec BETWEEN
				{
                            $$ = cat7($1, $2, $3, $4, $5, $6, $7);
				}
			| a_expr IN_P in_expr
				{
                            $$ = cat3($1, $2, $3);
				}
			| a_expr NOT IN_P in_expr
				{
                            $$ = cat4($1, $2, $3, $4);
				}
/*
// AA: We do not allow nested sub-queries to occur
			| a_expr subquery_Op sub_type select_with_parens	%prec Op
*/
			| a_expr subquery_Op sub_type '(' a_expr ')'		%prec Op
				{
                            $$ = cat6($1, $2, $3, "(", $5, ")");
				}
// AA: We do not allow nested sub-queries to occur
//			| UNIQUE select_with_parens
			| a_expr IS DOCUMENT_P					%prec IS
				{
                            $$ = cat3($1, $2, $3);
				}
			| a_expr IS NOT DOCUMENT_P				%prec IS
				{
                            $$ = cat4($1, $2, $3, $4);
				}
		;

/*
 * Restricted expressions
 *
 * b_expr is a subset of the complete expression syntax defined by a_expr.
 *
 * Presently, AND, NOT, IS, and IN are the a_expr keywords that would
 * cause trouble in the places where b_expr is used.  For simplicity, we
 * just eliminate all the boolean-keyword-operator productions from b_expr.
 */
b_expr:		c_expr
				{ $$ = $1; }
			| b_expr TYPECAST Typename
				{ $$ = cat3($1, $2, $3); }
			| '+' b_expr					%prec UMINUS
				{ $$ = cat2("+", $2); }
			| '-' b_expr					%prec UMINUS
				{ $$ = cat2("-", $2); }
			| b_expr '+' b_expr
				{ $$ = cat3($1, "+", $3); }
			| b_expr '-' b_expr
				{ $$ = cat3($1, "-", $3); }
			| b_expr '*' b_expr
				{ $$ = cat3($1, "*", $3); }
			| b_expr '/' b_expr
				{ $$ = cat3($1, "/", $3); }
			| b_expr '%' b_expr
				{ $$ = cat3($1, "%", $3); }
			| b_expr '^' b_expr
				{ $$ = cat3($1, "^", $3); }
			| b_expr '<' b_expr
				{ $$ = cat3($1, "<", $3); }
			| b_expr '>' b_expr
				{ $$ = cat3($1, ">", $3); }
			| b_expr '=' b_expr
				{ $$ = cat3($1, "=", $3); }
			| b_expr qual_Op b_expr				%prec Op
				{ $$ = cat3($1, $2, $3); }
			| qual_Op b_expr					%prec Op
				{ $$ = cat2($1, $2); }
			| b_expr qual_Op					%prec POSTFIXOP
				{ $$ = cat2($1, $2); }
			| b_expr IS DISTINCT FROM b_expr		%prec IS
				{ $$ = cat5($1, $2, $3, $4, $5); }
			| b_expr IS NOT DISTINCT FROM b_expr	%prec IS
				{ $$ = cat6($1, $2, $3, $4, $5, $6); }
			| b_expr IS OF '(' type_list ')'		%prec IS
				{
                                        $$ = cat6($1, $2, $3, "(", $5, ")");
				}
			| b_expr IS NOT OF '(' type_list ')'	%prec IS
				{
                            $$ = cat7($1, $2, $3, $4, "(", $6, "(");
				}
			| b_expr IS DOCUMENT_P					%prec IS
				{
                            $$ = cat3($1, $2, $3);
				}
			| b_expr IS NOT DOCUMENT_P				%prec IS
				{
                            $$ = cat4($1, $2, $3, $4);
				}
		;

/*
 * Productions that can be used in both a_expr and b_expr.
 *
 * Note: productions that refer recursively to a_expr or b_expr mostly
 * cannot appear here.	However, it's OK to refer to a_exprs that occur
 * inside parentheses, such as function arguments; that cannot introduce
 * ambiguity to the b_expr syntax.
 */
c_expr:		columnref								{ $$ = $1; }
			| AexprConst							{ $$ = $1; }
			| PARAM opt_indirection
				{
                            $$ = cat2($1, $2);
				}
			| '(' a_expr ')' opt_indirection
				{
                            $$ = cat4("(", $2, ")", $4);
				}
			| case_expr
				{ $$ = $1; }
			| func_expr
				{ $$ = $1; }
/*
// AA: We do not allow nested sub-queries to occur
			| select_with_parens			%prec UMINUS
			| EXISTS select_with_parens
			| ARRAY select_with_parens
*/
			| ARRAY array_expr
				{
                            $$ = cat2($1, $2);
				}
			| row
				{
                                        $$ = $1;
				}
		;

/*
 * func_expr is split out from c_expr just so that we have a classification
 * for "everything that is a function call or looks like one".  This isn't
 * very important, but it saves us having to document which variants are
 * legal in the backwards-compatible functional-index syntax for CREATE INDEX.
 * (Note that many of the special SQL functions wouldn't actually make any
 * sense as functional index entries, but we ignore that consideration here.)
 */
func_expr:	func_name '(' ')' over_clause
				{
                                        $$ = cat4($1, "(", ")", $4);
				}
			| func_name '(' expr_list ')' over_clause
				{
                                        $$ = cat5($1, "(", $3, ")", $5);
				}
			| func_name '(' VARIADIC a_expr ')' over_clause
				{
                                        $$ = cat6($1, "(", $3, $4, ")", $6);
				}
			| func_name '(' expr_list ',' VARIADIC a_expr ')' over_clause
				{
                                        $$ = cat8($1, "(", $3, ",", $5, $6, ")", $8);
				}
			| func_name '(' ALL expr_list ')' over_clause
				{
                                        $$ = cat6($1, "(", $3, $4, ")", $6);
				}
			| func_name '(' DISTINCT expr_list ')' over_clause
				{
                                        $$ = cat6($1, "(", $3, $4, ")", $6);
				}
			| func_name '(' '*' ')' over_clause
				{
					/*
					 * We consider AGGREGATE(*) to invoke a parameterless
					 * aggregate.  This does the right thing for COUNT(*),
					 * and there are no other aggregates in SQL92 that accept
					 * '*' as parameter.
					 *
					 * The FuncCall node is also marked agg_star = true,
					 * so that later processing can detect what the argument
					 * really was.
					 */
                                        $$ = cat5($1, "(", "*", ")", $5);
				}
			| CURRENT_DATE
				{
					/*
					 * Translate as "'now'::text::date".
					 *
					 * We cannot use "'now'::date" because coerce_type() will
					 * immediately reduce that to a constant representing
					 * today's date.  We need to delay the conversion until
					 * runtime, else the wrong things will happen when
					 * CURRENT_DATE is used in a column default value or rule.
					 *
					 * This could be simplified if we had a way to generate
					 * an expression tree representing runtime application
					 * of type-input conversion functions.  (As of PG 7.3
					 * that is actually possible, but not clear that we want
					 * to rely on it.)
					 */
                                        $$ = $1;
				}
			| CURRENT_TIME
				{
					/*
					 * Translate as "'now'::text::timetz".
					 * See comments for CURRENT_DATE.
					 */
					$$ = $1;
				}
			| CURRENT_TIME '(' Iconst ')'
				{
					/*
					 * Translate as "'now'::text::timetz(n)".
					 * See comments for CURRENT_DATE.
					 */
					$$ = cat4($1, "(", $3, ")");
				}
			| CURRENT_TIMESTAMP
				{
					/*
					 * Translate as "now()", since we have a function that
					 * does exactly what is needed.
					 */
					$$ = $1;
				}
			| CURRENT_TIMESTAMP '(' Iconst ')'
				{
					/*
					 * Translate as "'now'::text::timestamptz(n)".
					 * See comments for CURRENT_DATE.
					 */
                                       $$ = cat4($1, "(", $3, ")");
				}
			| LOCALTIME
				{
					/*
					 * Translate as "'now'::text::time".
					 * See comments for CURRENT_DATE.
					 */
					$$ = $1;
				}
			| LOCALTIME '(' Iconst ')'
				{
					/*
					 * Translate as "'now'::text::time(n)".
					 * See comments for CURRENT_DATE.
					 */
                                        $$ = cat4($1, "(", $3, ")");
				}
			| LOCALTIMESTAMP
				{
					/*
					 * Translate as "'now'::text::timestamp".
					 * See comments for CURRENT_DATE.
					 */
                                        $$ = $1;
				}
			| LOCALTIMESTAMP '(' Iconst ')'
				{
					/*
					 * Translate as "'now'::text::timestamp(n)".
					 * See comments for CURRENT_DATE.
					 */
                                        $$ = cat4($1, "(", $3, ")");
				}
			| CURRENT_ROLE
				{
					$$ = $1;
				}
			| CURRENT_USER
				{
					$$ = $1;
				}
			| SESSION_USER
				{
					$$ = $1;
				}
			| USER
				{
					$$ = $1;
				}
			| CURRENT_CATALOG
				{
					$$ = $1;
				}
			| CURRENT_SCHEMA
				{
					$$ = $1;
				}
			| CAST '(' a_expr AS Typename ')'
				{ $$ = cat6($1, "(", $3, $4, $5, ")"); }
			| EXTRACT '(' extract_list ')'
				{
                                        $$ = cat4($1, "(", $3, ")");
				}
			| OVERLAY '(' overlay_list ')'
				{
					/* overlay(A PLACING B FROM C FOR D) is converted to
					 * substring(A, 1, C-1) || B || substring(A, C+1, C+D)
					 * overlay(A PLACING B FROM C) is converted to
					 * substring(A, 1, C-1) || B || substring(A, C+1, C+char_length(B))
					 */
                                        $$ = cat4($1, "(", $3, ")");
				}
			| POSITION '(' position_list ')'
				{
					/* position(A in B) is converted to position(B, A) */
                                        $$ = cat4($1, "(", $3, ")");
				}
			| SUBSTRING '(' substr_list ')'
				{
					/* substring(A from B for C) is converted to
					 * substring(A, B, C) - thomas 2000-11-28
					 */
                                        $$ = cat4($1, "(", $3, ")");
				}
			| TREAT '(' a_expr AS Typename ')'
				{
					/* TREAT(expr AS target) converts expr of a particular type to target,
					 * which is defined to be a subtype of the original expression.
					 * In SQL99, this is intended for use with structured UDTs,
					 * but let's make this a generally useful form allowing stronger
					 * coercions than are handled by implicit casting.
					 */
                                        $$ = cat6($1, "(", $3, $4, $5, ")");
				}
			| TRIM '(' BOTH trim_list ')'
				{
					/* various trim expressions are defined in SQL92
					 * - thomas 1997-07-19
					 */
                                        $$ = cat5($1, "(", $3, $4, ")");
				}
			| TRIM '(' LEADING trim_list ')'
				{
                                        $$ = cat5($1, "(", $3, $4, ")");
				}
			| TRIM '(' TRAILING trim_list ')'
				{
                                        $$ = cat5($1, "(", $3, $4, ")");
				}
			| TRIM '(' trim_list ')'
				{
                                        $$ = cat4($1, "(", $3, ")");
				}
			| NULLIF '(' a_expr ',' a_expr ')'
				{
                                        $$ = cat6($1, "(", $3, ",", $5, ")");
				}
			| COALESCE '(' expr_list ')'
				{
                                        $$ = cat4($1, "(", $3, ")");
				}
			| GREATEST '(' expr_list ')'
				{
                                        $$ = cat4($1, "(", $3, ")");
				}
			| LEAST '(' expr_list ')'
				{
                                        $$ = cat4($1, "(", $3, ")");
				}
			| XMLCONCAT '(' expr_list ')'
				{
                                        $$ = cat4($1, "(", $3, ")");
				}
			| XMLELEMENT '(' NAME_P ColLabel ')'
				{
                                        $$ = cat5($1, "(", $3, $4, ")");
				}
			| XMLELEMENT '(' NAME_P ColLabel ',' xml_attributes ')'
				{
                                        $$ = cat7($1, "(", $3, $4, ",", $6, ")");
				}
			| XMLELEMENT '(' NAME_P ColLabel ',' expr_list ')'
				{
                                        $$ = cat7($1, "(", $3, $4, ",", $6, ")");
				}
			| XMLELEMENT '(' NAME_P ColLabel ',' xml_attributes ',' expr_list ')'
				{
                                        $$ = cat9($1, "(", $3, $4, ",", $6, ",", $8, ")");
				}
			| XMLFOREST '(' xml_attribute_list ')'
				{
                                        $$ = cat4($1, "(", $3, ")");
				}
			| XMLPARSE '(' document_or_content a_expr xml_whitespace_option ')'
				{
                                        $$ = cat6($1, "(", $3, $4, $5, ")");
				}
			| XMLPI '(' NAME_P ColLabel ')'
				{
                                        $$ = cat5($1, "(", $3, $4, ")");
				}
			| XMLPI '(' NAME_P ColLabel ',' a_expr ')'
				{
                                        $$ = cat7($1, "(", $3, $4, ",", $6, ")");
				}
			| XMLROOT '(' a_expr ',' xml_root_version opt_xml_root_standalone ')'
				{
                                        $$ = cat7($1, "(", $3, ",", $5, $6, ")");
				}
			| XMLSERIALIZE '(' document_or_content a_expr AS SimpleTypename ')'
				{
                                        $$ = cat7($1, "(", $3, $4, $5, $6, ")");
				}
		;

/*
 * SQL/XML support
 */
xml_root_version: VERSION_P a_expr
				{ $$ = cat2($1, $2); }
			| VERSION_P NO VALUE_P
				{ $$ = cat2($1, $2); }
		;

opt_xml_root_standalone: ',' STANDALONE_P YES_P
				{ $$ = cat3(",", $2, $3); }
			| ',' STANDALONE_P NO
				{ $$ = cat3(",", $2, $3); }
			| ',' STANDALONE_P NO VALUE_P
				{ $$ = cat3(",", $2, $3); }
			| /*EMPTY*/
				{ $$ = NULL; }
		;

xml_attributes: XMLATTRIBUTES '(' xml_attribute_list ')'	{ $$ = cat4($1, "(", $3, ")"); }
		;

xml_attribute_list:	xml_attribute_el					{ $$ = $1; }
			| xml_attribute_list ',' xml_attribute_el	{ $$ = cat3($1, ",", $3); }
		;

xml_attribute_el: a_expr AS ColLabel
				{
                                        $$ = cat3($1, $2, $3);
				}
			| a_expr
				{
					$$ = $1;
				}
		;

document_or_content: DOCUMENT_P						{ $$ = "XMLOPTION_DOCUMENT"; }
			| CONTENT_P					{ $$ = "XMLOPTION_CONTENT"; }
		;

xml_whitespace_option: PRESERVE WHITESPACE_P		{ $$ = "TRUE"; }
			| STRIP_P WHITESPACE_P		{ $$ = "FALSE"; }
			| /*EMPTY*/			{ $$ = "FALSE"; }
		;


over_clause:
/*
			OVER window_specification
			| OVER ColId
			|
*/
			/*EMPTY*/
				{ $$ = NULL; }
		;



/*
 * Supporting nonterminals for expressions.
 */

/* Explicit row production.
 *
 * SQL99 allows an optional ROW keyword, so we can now do single-element rows
 * without conflicting with the parenthesized a_expr production.  Without the
 * ROW keyword, there must be more than one a_expr inside the parens.
 */
row:		ROW '(' expr_list ')'					{ $$ = cat4($1, "(", $3, ")"); }
			| ROW '(' ')'					{ $$ = NULL; }
			| '(' expr_list ',' a_expr ')'			{ $$ = cat5("(", $2, ",", $4, ")"); }
		;

sub_type:	ANY										{ $$ = "ANY_SUBLINK"; }
			| SOME									{ $$ = "ANY_SUBLINK"; }
			| ALL									{ $$ = "ALL_SUBLINK"; }
		;

all_Op:		Op										{ $$ = $1; }
			| MathOp								{ $$ = $1; }
		;

MathOp:		 '+'									{ $$ = "+"; }
			| '-'									{ $$ = "-"; }
			| '*'									{ $$ = "*"; }
			| '/'									{ $$ = "/"; }
			| '%'									{ $$ = "%"; }
			| '^'									{ $$ = "^"; }
			| '<'									{ $$ = "<"; }
			| '>'									{ $$ = ">"; }
			| '='									{ $$ = "="; }
		;

qual_Op:	Op
					{ $$ = $1; }
			| OPERATOR '(' any_operator ')'
					{ $$ = cat4($1, "(", $3, ")"); }
		;

qual_all_Op:
			all_Op
					{ $$ = $1; }
			| OPERATOR '(' any_operator ')'
					{ $$ = cat4($1, "(", $3, ")"); }
		;

subquery_Op:
			all_Op
					{ $$ = $1; }
			| OPERATOR '(' any_operator ')'
					{ $$ = cat4($1, "(", $3, ")"); }
			| LIKE
                                        { $$ = $1; }
			| NOT LIKE
					{ $$ = $1; }
			| ILIKE
					{ $$ = $1; }
			| NOT ILIKE
					{ $$ = $1; }
/* cannot put SIMILAR TO here, because SIMILAR TO is a hack.
 * the regular expression is preprocessed by a function (similar_escape),
 * and the ~ operator for posix regular expressions is used.
 *        x SIMILAR TO y     ->    x ~ similar_escape(y)
 * this transformation is made on the fly by the parser upwards.
 * however the SubLink structure which handles any/some/all stuff
 * is not ready for such a thing.
 */
			;

expr_list:	a_expr
				{
					$$ = $1;
				}
			| expr_list ',' a_expr
				{
					$$ = cat3($1, ",", $3);
				}
		;

type_list:	Typename								{ $$ = $1; }
			| type_list ',' Typename				{ $$ = cat3($1, ",", $3); }
		;

array_expr: '[' expr_list ']'
				{
                            $$ = cat3("[", $2, "]");
				}
			| '[' array_expr_list ']'
				{
                            $$ = cat3("[", $2, "]");
				}
			| '[' ']'
				{
					$$ = cat2("[", "]");
				}
		;

array_expr_list: array_expr							{ $$ = $1; }
			| array_expr_list ',' array_expr		{ $$ = cat3($1, ",", $3); }
		;


extract_list:
			extract_arg FROM a_expr
				{
                                    $$ = cat3($1, $2, $3);
				}
			| /*EMPTY*/								{ $$ = NULL; }
		;

/* Allow delimited string Sconst in extract_arg as an SQL extension.
 * - thomas 2001-04-12
 */
extract_arg:
			IDENT									{ $$ = $1; }
			| YEAR_P								{ $$ = "year"; }
			| MONTH_P								{ $$ = "month"; }
			| DAY_P									{ $$ = "day"; }
			| HOUR_P								{ $$ = "hour"; }
			| MINUTE_P								{ $$ = "minute"; }
			| SECOND_P								{ $$ = "second"; }
			| Sconst								{ $$ = $1; }
		;

/* OVERLAY() arguments
 * SQL99 defines the OVERLAY() function:
 * o overlay(text placing text from int for int)
 * o overlay(text placing text from int)
 */
overlay_list:
			a_expr overlay_placing substr_from substr_for
				{
                                        $$ = cat4($1, $2, $3, $4);
				}
			| a_expr overlay_placing substr_from
				{
                                        $$ = cat3($1, $2, $3);
				}
		;

overlay_placing:
			PLACING a_expr
				{ $$ = cat2($1, $2); }
		;

/* position_list uses b_expr not a_expr to avoid conflict with general IN */

position_list:
			b_expr IN_P b_expr						{ $$ = cat3($1, $2, $3); }
			| /*EMPTY*/								{ $$ = NULL; }
		;

/* SUBSTRING() arguments
 * SQL9x defines a specific syntax for arguments to SUBSTRING():
 * o substring(text from int for int)
 * o substring(text from int) get entire string from starting point "int"
 * o substring(text for int) get first "int" characters of string
 * o substring(text from pattern) get entire string matching pattern
 * o substring(text from pattern for escape) same with specified escape char
 * We also want to support generic substring functions which accept
 * the usual generic list of arguments. So we will accept both styles
 * here, and convert the SQL9x style to the generic list for further
 * processing. - thomas 2000-11-28
 */
substr_list:
			a_expr substr_from substr_for
				{
                            $$ = cat3($1, $2, $3);
				}
			| a_expr substr_for substr_from
				{
					/* not legal per SQL99, but might as well allow it */
                            $$ = cat3($1, $2, $3);
				}
			| a_expr substr_from
				{
                            $$ = cat2($1, $2);
				}
			| a_expr substr_for
				{
					/*
					 * Since there are no cases where this syntax allows
					 * a textual FOR value, we forcibly cast the argument
					 * to int4.  The possible matches in pg_proc are
					 * substring(text,int4) and substring(text,text),
					 * and we don't want the parser to choose the latter,
					 * which it is likely to do if the second argument
					 * is unknown or doesn't have an implicit cast to int4.
					 */
					$$ = cat2($1, $2);
				}
			| expr_list
				{
					$$ = $1;
				}
			| /*EMPTY*/
				{ $$ = NULL; }
		;

substr_from:
			FROM a_expr								{ $$ = cat2($1, $2); }
		;

substr_for: FOR a_expr								{ $$ = cat2($1, $2); }
		;

trim_list:	a_expr FROM expr_list					{ $$ = cat3($1, $2, $3); }
			| FROM expr_list						{ $$ = cat2($1, $2); }
			| expr_list								{ $$ = $1; }
		;

in_expr:	
// AA: Disable correlated subqueries: SELECT ... FROM ... WHERE expr IN (in_expr: SELECT ...)
/*                select_with_parens
			|
*/
                '(' expr_list ')'						{ $$ = cat3("(", $2, ")"); }
		;

/*
 * Define SQL92-style case clause.
 * - Full specification
 *	CASE WHEN a = b THEN c ... ELSE d END
 * - Implicit argument
 *	CASE a WHEN b THEN c ... ELSE d END
 */
case_expr:	CASE case_arg when_clause_list case_default END_P
				{
                    $$ = cat5($1, $2, $3, $4, $5);
				}
		;

when_clause_list:
			/* There must be at least one */
			when_clause								{ $$ = $1; }
			| when_clause_list when_clause			{ $$ = cat2($1, $2); }
		;

when_clause:
			WHEN a_expr THEN a_expr
				{
                                    $$ = cat4($1, $2, $3, $4);
				}
		;

case_default:
			ELSE a_expr								{ $$ = cat2($1, $2); }
			| /*EMPTY*/								{ $$ = NULL; }
		;

case_arg:	a_expr									{ $$ = $1; }
			| /*EMPTY*/								{ $$ = NULL; }
		;

/*
 * columnref starts with relation_name not ColId, so that OLD and NEW
 * references can be accepted.	Note that when there are more than two
 * dotted names, the first name is not actually a relation name...
 */
columnref:	relation_name
				{
					$$ = $1;
				}
			| relation_name indirection
				{
                                        $$ = cat2($1, $2);
				}
		;

indirection_el:
			'.' attr_name
				{
                                        $$ = cat2(".", $2);
				}
			| '.' '*'
				{
                                        $$ = cat2(".", "*");
				}
			| '[' a_expr ']'
				{
                                        $$ = cat3("[", $2, "]");
				}
			| '[' a_expr ':' a_expr ']'
				{
                                        $$ = cat5("[", $2, ":", $4, "]");
				}
		;

indirection:
			indirection_el							{ $$ = $1; }
			| indirection indirection_el			{ $$ = cat2($1, $2); }
		;

opt_indirection:
			/*EMPTY*/								{ $$ = NULL; }
			| opt_indirection indirection_el		{ $$ = cat2($1, $2); }
		;

opt_asymmetric: ASYMMETRIC                                              { $$ = $1; }
			| /*EMPTY*/                                     { $$ = NULL; }
		;

/*
 * The SQL spec defines "contextually typed value expressions" and
 * "contextually typed row value constructors", which for our purposes
 * are the same as "a_expr" and "row" except that DEFAULT can appear at
 * the top level.
 */


/*****************************************************************************
 *
 *	target list for SELECT
 *
 *****************************************************************************/

target_list:
			target_el								{ $$ = $1; }
			| target_list ',' target_el				{ $$ = cat3($1, ",", $3); }
		;

target_el:	a_expr AS ColLabel
				{
                                        $$ = cat3($1, $2, $3);
				}
			/*
			 * We support omitting AS only for column labels that aren't
			 * any known keyword.  There is an ambiguity against postfix
			 * operators: is "a ! b" an infix expression, or a postfix
			 * expression and a column label?  We prefer to resolve this
			 * as an infix expression, which we accomplish by assigning
			 * IDENT a precedence higher than POSTFIXOP.
			 */
			| a_expr IDENT
				{
                                        $$ = cat2($1, $2);
				}
			| a_expr
				{
                                        $$ = $1;
				}
			| '*'
				{
                                        $$ = "*";
				}
		;


/*****************************************************************************
 *
 *	Names and constants
 *
 *****************************************************************************/

relation_name:
			SpecialRuleRelation						{ $$ = $1; }
			| ColId									{ $$ = $1; }
		;

/*
 * The production for a qualified relation name has to exactly match the
 * production for a qualified func_name, because in a FROM clause we cannot
 * tell which we are parsing until we see what comes after it ('(' for a
 * func_name, something else for a relation). Therefore we allow 'indirection'
 * which may contain subscripts, and reject that case in the C code.
 */
qualified_name:
			relation_name
				{
                                        $$ = $1;
				}
			| relation_name indirection
				{
                                        $$ = cat2($1, $2);
				}
		;

name_list:	name
					{ $$ = $1; }
			| name_list ',' name
					{ $$ = cat2($1, ","); }
		;


name:		ColId									{ $$ = $1; };

attr_name:	ColLabel								{ $$ = $1; };

/*
 * The production for a qualified func_name has to exactly match the
 * production for a qualified columnref, because we cannot tell which we
 * are parsing until we see what comes after it ('(' or Sconst for a func_name,
 * anything else for a columnref).  Therefore we allow 'indirection' which
 * may contain subscripts, and reject that case in the C code.  (If we
 * ever implement SQL99-like methods, such syntax may actually become legal!)
 */
func_name:	type_function_name
					{ $$ = $1; }
			| relation_name indirection
					{ $$ = cat2($1, $2); }
		;


/*
 * Constants
 */
AexprConst: Iconst
				{
					$$ = $1;
				}
			| FCONST
				{
					$$ = $1;
				}
			| Sconst
				{
					$$ = $1;
				}
			| BCONST
				{
					$$ = $1;
                                }
			| XCONST
				{
					$$ = $1;
                                }
			| func_name Sconst
				{
                                        $$ = cat2($1, $2);
                                }
			| func_name '(' expr_list ')' Sconst
				{
                                        $$ = cat5($1, "(", $3, ")", $5);
                                }
			| ConstTypename Sconst
				{
                                        $$ = cat2($1, $2);
                                }
			| ConstInterval Sconst opt_interval
				{
                                        $$ = cat3($1, $2, $3);
                                }
			| ConstInterval '(' Iconst ')' Sconst opt_interval
				{
                                        $$ = cat6($1, "(", $3, ")", $5, $6);
                                }
			| TRUE_P
				{
					$$ = $1;
                                }
			| FALSE_P
				{
					$$ = $1;
                                }
			| NULL_P
				{
					$$ = $1;
                                }
		;

Iconst:		ICONST									{ $$ = $1; };
Sconst:		SCONST									{ $$ = $1; };


/*
 * Name classification hierarchy.
 *
 * IDENT is the lexeme returned by the lexer for identifiers that match
 * no known keyword.  In most cases, we can accept certain keywords as
 * names, not only IDENTs.	We prefer to accept as many such keywords
 * as possible to minimize the impact of "reserved words" on programmers.
 * So, we divide names into several possible classes.  The classification
 * is chosen in part to make keywords acceptable as names wherever possible.
 */

/* Column identifier --- names that can be column, table, etc names.
 */
ColId:		IDENT									{ $$ = $1; }
			| unreserved_keyword					{ $$ = strdup($1); }
			| col_name_keyword						{ $$ = strdup($1); }
		;

/* Type/function identifier --- names that can be type or function names.
 */
type_function_name:	IDENT							{ $$ = $1; }
			| unreserved_keyword					{ $$ = strdup($1); }
			| type_func_name_keyword				{ $$ = strdup($1); }
		;

/* Column label --- allowed labels in "AS" clauses.
 * This presently includes *all* Postgres keywords.
 */
ColLabel:	IDENT									{ $$ = $1; }
			| unreserved_keyword					{ $$ = strdup($1); }
			| col_name_keyword						{ $$ = strdup($1); }
			| type_func_name_keyword				{ $$ = strdup($1); }
			| reserved_keyword						{ $$ = strdup($1); }
		;


/*
 * Keyword category lists.  Generally, every keyword present in
 * the Postgres grammar should appear in exactly one of these lists.
 *
 * Put a new keyword into the first list that it can go into without causing
 * shift or reduce conflicts.  The earlier lists define "less reserved"
 * categories of keywords.
 *
 * Make sure that each keyword's category in keywords.c matches where
 * it is listed here.  (Someday we may be able to generate these lists and
 * keywords.c's table from a common master list.)
 */

/* "Unreserved" keywords --- available for use as any kind of name.
 */
unreserved_keyword:
			  ABORT_P
			| ABSOLUTE_P
			| ACCESS
			| ACTION
			| ADD_P
			| ADMIN
			| AFTER
			| AGGREGATE
			| ALSO
			| ALTER
			| ALWAYS
			| ASSERTION
			| ASSIGNMENT
			| AT
			| BACKWARD
			| BEFORE
			| BEGIN_P
			| BY
			| CACHE
			| CALLED
			| CASCADE
			| CASCADED
			| CATALOG_P
			| CHAIN
			| CHARACTERISTICS
			| CHECKPOINT
			| CLASS
			| CLOSE
			| CLUSTER
			| COMMENT
			| COMMIT
			| COMMITTED
			| CONCURRENTLY
			| CONFIGURATION
			| CONNECTION
			| CONSTRAINTS
			| CONTENT_P
			| CONTINUE_P
			| CONVERSION_P
			| COPY
			| COST
			| CREATEDB
			| CREATEROLE
			| CREATEUSER
			| CSV
			| CURRENT_P
			| CURSOR
			| CYCLE
			| DATA_P
			| DATABASE
			| DAY_P
			| DEALLOCATE
			| DECLARE
			| DEFAULTS
			| DEFERRED
			| DEFINER
			| DELETE_P
			| DELIMITER
			| DELIMITERS
			| DICTIONARY
			| DISABLE_P
			| DISCARD
			| DOCUMENT_P
			| DOMAIN_P
			| DOUBLE_P
			| DROP
			| EACH
			| ENABLE_P
			| ENCODING
			| ENCRYPTED
			| ENUM_P
			| ESCAPE
			| EXCLUDING
			| EXCLUSIVE
			| EXECUTE
			| EXPLAIN
			| EXTERNAL
			| FAMILY
			| FIRST_P
			| FOLLOWING
			| FORCE
			| FORWARD
			| FUNCTION
			| GLOBAL
			| GRANTED
			| HANDLER
			| HEADER_P
			| HOLD
			| HOUR_P
			| IDENTITY_P
			| IF_P
			| IMMEDIATE
			| IMMUTABLE
			| IMPLICIT_P
			| INCLUDING
			| INCREMENT
			| INDEX
			| INDEXES
			| INHERIT
			| INHERITS
			| INPUT_P
			| INSENSITIVE
			| INSERT
			| INSTEAD
			| INVOKER
			| ISOLATION
			| KEY
			| LANCOMPILER
			| LANGUAGE
			| LARGE_P
			| LAST_P
			| LC_COLLATE_P
			| LC_CTYPE_P
			| LEVEL
			| LISTEN
			| LOAD
			| LOCAL
			| LOCATION
			| LOCK_P
			| LOGIN_P
			| MAPPING
			| MATCH
			| MAXVALUE
			| MINUTE_P
			| MINVALUE
			| MODE
			| MONTH_P
			| MOVE
			| NAME_P
			| NAMES
			| NEXT
			| NO
			| NOCREATEDB
			| NOCREATEROLE
			| NOCREATEUSER
			| NOINHERIT
			| NOLOGIN_P
			| NOSUPERUSER
			| NOTHING
			| NOTIFY
			| NOWAIT
			| NULLS_P
			| OBJECT_P
			| OF
			| OIDS
			| OPERATOR
			| OPTION
			| OPTIONS
			| OWNED
			| OWNER
			| PARSER
			| PARTIAL
			| PARTITION
			| PASSWORD
			| PLANS
			| PRECEDING
			| PREPARE
			| PREPARED
			| PRESERVE
			| PRIOR
			| PROCEDURAL
			| PROCEDURE
			| QUOTE
			| RANGE
			| READ
			| REASSIGN
			| RECHECK
			| RECURSIVE
			| REINDEX
			| RELATIVE_P
			| RELEASE
			| RENAME
			| REPEATABLE
			| REPLACE
			| REPLICA
			| RESET
			| RESTART
			| RESTRICT
			| RETURNS
			| REVOKE
			| ROLE
			| ROLLBACK
			| ROWS
			| RULE
			| SAVEPOINT
			| SCHEMA
			| SCROLL
			| SEARCH
			| SECOND_P
			| SECURITY
			| SEQUENCE
			| SERIALIZABLE
			| SERVER
			| SESSION
			| SET
			| SHARE
			| SHOW
			| SIMPLE
			| STABLE
			| STANDALONE_P
			| START
			| STATEMENT
			| STATISTICS
			| STDIN
			| STDOUT
			| STORAGE
			| STRICT_P
			| STRIP_P
			| SUPERUSER_P
			| SYSID
			| SYSTEM_P
			| TABLESPACE
			| TEMP
			| TEMPLATE
			| TEMPORARY
			| TEXT_P
			| TRANSACTION
			| TRIGGER
			| TRUNCATE
			| TRUSTED
			| TYPE_P
			| UNBOUNDED
			| UNCOMMITTED
			| UNENCRYPTED
			| UNKNOWN
			| UNLISTEN
			| UNTIL
			| UPDATE
			| VACUUM
			| VALID
			| VALIDATOR
			| VALUE_P
			| VARYING
			| VERSION_P
			| VIEW
			| VOLATILE
			| WHITESPACE_P
			| WITHOUT
			| WORK
			| WRAPPER
			| WRITE
			| XML_P
			| YEAR_P
			| YES_P
			| ZONE
		;

/* Column identifier --- keywords that can be column, table, etc names.
 *
 * Many of these keywords will in fact be recognized as type or function
 * names too; but they have special productions for the purpose, and so
 * can't be treated as "generic" type or function names.
 *
 * The type names appearing here are not usable as function names
 * because they can be followed by '(' in typename productions, which
 * looks too much like a function call for an LR(1) parser.
 */
col_name_keyword:
			  BIGINT
			| BIT
			| BOOLEAN_P
			| CHAR_P
			| CHARACTER
			| COALESCE
			| DEC
			| DECIMAL_P
			| EXISTS
			| EXTRACT
			| FLOAT_P
			| GREATEST
			| INOUT
			| INT_P
			| INTEGER
			| INTERVAL
			| LEAST
			| NATIONAL
			| NCHAR
			| NONE
			| NULLIF
			| NUMERIC
			| OUT_P
			| OVERLAY
			| POSITION
			| PRECISION
			| REAL
			| ROW
			| SETOF
			| SMALLINT
			| SUBSTRING
			| TIME
			| TIMESTAMP
			| TREAT
			| TRIM
			| VALUES
			| VARCHAR
			| XMLATTRIBUTES
			| XMLCONCAT
			| XMLELEMENT
			| XMLFOREST
			| XMLPARSE
			| XMLPI
			| XMLROOT
			| XMLSERIALIZE
		;

/* Type/function identifier --- keywords that can be type or function names.
 *
 * Most of these are keywords that are used as operators in expressions;
 * in general such keywords can't be column names because they would be
 * ambiguous with variables, but they are unambiguous as function identifiers.
 *
 * Do not include POSITION, SUBSTRING, etc here since they have explicit
 * productions in a_expr to support the goofy SQL9x argument syntax.
 * - thomas 2000-11-28
 */
type_func_name_keyword:
			  AUTHORIZATION
			| BETWEEN
			| BINARY
			| CROSS
			| CURRENT_SCHEMA
			| FREEZE
			| FULL
			| ILIKE
			| INNER_P
			| IS
			| ISNULL
			| JOIN
			| LEFT
			| LIKE
			| NATURAL
			| NOTNULL
			| OUTER_P
			| OVER
			| OVERLAPS
			| RIGHT
			| SIMILAR
			| VERBOSE
		;

/* Reserved keyword --- these keywords are usable only as a ColLabel.
 *
 * Keywords appear here if they could not be distinguished from variable,
 * type, or function names in some contexts.  Don't put things here unless
 * forced to.
 */
reserved_keyword:
			  ALL
			| ANALYSE
			| ANALYZE
			| AND
			| ANY
			| ARRAY
			| AS
			| ASC
			| ASYMMETRIC
			| BOTH
			| CASE
			| CAST
			| CHECK
			| COLLATE
			| COLUMN
			| CONSTRAINT
			| CREATE
			| CURRENT_CATALOG
			| CURRENT_DATE
			| CURRENT_ROLE
			| CURRENT_TIME
			| CURRENT_TIMESTAMP
			| CURRENT_USER
			| DEFAULT
			| DEFERRABLE
			| DESC
			| DISTINCT
			| DO
			| ELSE
			| END_P
			| EXCEPT
			| FALSE_P
			| FETCH
			| FOR
			| FOREIGN
			| FROM
			| GRANT
			| GROUP_P
			| HAVING
			| IN_P
			| INITIALLY
			| INTERSECT
			| INTO
			| LEADING
			| LIMIT
			| LOCALTIME
			| LOCALTIMESTAMP
			| NEW
			| NOT
			| NULL_P
			| OFF
			| OFFSET
			| OLD
			| ON
			| ONLY
			| OR
			| ORDER
			| PLACING
			| PRIMARY
			| REFERENCES
			| RETURNING
			| SELECT
			| SESSION_USER
			| SOME
			| SYMMETRIC
			| TABLE
			| THEN
			| TO
			| TRAILING
			| TRUE_P
			| UNION
			| UNIQUE
			| USER
			| USING
			| VARIADIC
			| WHEN
			| WHERE
			| WINDOW
			| WITH
		;


SpecialRuleRelation:
			OLD
				{
					$$ = $1;
				}
			| NEW
				{
					$$ = $1;
				}
		;

attrs:      '.' attr_name
				{ $$ = "."; }
			| attrs '.' attr_name
				{ $$ = cat3($1, ".", $3); }
			;


any_operator:
            all_Op
				{ $$ = $1; }
			| ColId '.' any_operator
				{ $$ = cat3($1, ".", $3); }
			;

/******************************** END POSTGRESQL *********************************/


%%

#include "str.c"

extern int errorCount;

int main()
{
	yyparse();
        printf("\n\n\nPARSE TREE: \n%s\n", parsetree);
	return errorCount;
} 
