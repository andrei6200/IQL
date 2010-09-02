%{

#include <stdio.h>
#include <string.h>

#include <list>
#include <string>

#include "str.h"
#include "HqlMain.hpp"
#include "logger.hpp"
#include "querytree/QueryTree.hpp"


#define YYDEBUG 1

extern int yylex();
extern void yyerror(const char* msg);

/* AA: The final HQL queries are stored here*/
char* hqlQueries;

/******************************** START POSTGRESQL *********************************/

#include <ctype.h>
#include <limits.h>

///* Location tracking support --- simpler than bison's default */
//#define YYLLOC_DEFAULT(Current, Rhs, N) \
//	do { \
//		if (N) \
//			(Current) = (Rhs)[1]; \
//		else \
//			(Current) = (Rhs)[0]; \
//	} while (0)
/******************************** END POSTGRESQL *********************************/

%}

%code requires
{
#include "querytree/QueryTree.hpp"
}


/******************************** START POSTGRESQL *********************************/
%union
{
	char*                           ival;
	char*				chr;
	char*				str;
	char*				keyword;
        char*                           rasqlKeyword;
        char*                           postgisKeyword;
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

        QtSelectStatement               *select;
        QtList                          *nodelist;
        QtDataSource                    *source;
        
        QtNode                          *mynode;        // dummy placeholder
}



%type <select>	stmt SelectStmt
%type <select>	select_no_parens select_with_parens select_clause simple_select

%type <node>    opt_asymmetric

%type <ival>	opt_asc_desc opt_nulls_order

%type <str>	 attr_name name
%type <str>  relation_name

%type <list>	func_name qual_Op qual_all_Op subquery_Op

%type <mynode>	qualified_name

%type <str>	all_Op MathOp SpecialRuleRelation

%type <list>	stmtblock stmtmulti

%type <list>	sort_clause opt_sort_clause sortby_list
                name_list  
                 attrs
%type <nodelist>  expr_list
%type <nodelist> target_list from_list from_clause
//%type <mynode>  

%type <list>	indirection opt_indirection
                select_limit
                any_operator
                TableFuncElementList opt_type_modifiers


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
%type <mynode>  a_expr b_expr c_expr func_expr AexprConst indirection_el
                columnref in_expr func_table
%type <mynode>	row type_list
%type <node>	case_expr case_arg when_clause case_default
%type <list>	when_clause_list
%type <ival>	sub_type
%type <alias>	alias_clause
%type <sortby>	sortby
%type <source>	table_ref
%type <source>	joined_table
%type <mynode>	relation_expr
%type <mynode>	target_el

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
%type <str>	ColId ColLabel type_function_name postgis_func_name

%type <keyword> unreserved_keyword type_func_name_keyword
%type <keyword> col_name_keyword reserved_keyword


//%type <target>	xml_attribute_el
//%type <list>	xml_attribute_list xml_attributes
//%type <node>	xml_root_version opt_xml_root_standalone
//%type <ival>	document_or_content
//%type <boolean> xml_whitespace_option

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
%left           COLON
%left           OVERLAY
%nonassoc	SET				/* see relation_expr_opt_alias */
%left		UNION EXCEPT
%left		INTERSECT
%left		OR XOR
%left		AND
%right		NOT
%right          EQUAL NOTEQUAL
%nonassoc       LESS GREATER LESSEQUAL GREATEREQUAL
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
%left           PLUS MINUS
%left           MULT DIV MOD
%left		POWER
/* Unary Operators */
%left		AT ZONE			/* sets precedence for AT TIME ZONE */
%right		UMINUS
%left           LEPAR REPAR
%left           LRPAR RRPAR
%left		TYPECAST
%left           DOT SDOM
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

/* Rasdaman keywords (which are not SQL keywords) */
%token <rasqlKeyword>   ABS ADDCELLS
            RQLALL ARCCOS ARCSIN ARCTAN
            AVGCELLS BMP TBOOL TCHAR 
            COMPLEX CONDENSE COS COSH COUNTCELLS
            DEM TDOUBLE 
            EXP EXTEND TFLOAT HDF HI IM
            INV_BMP INV_CSV INV_DEM INV_HDF INV_JPEG INV_PNG
            INV_TIFF INV_TOR INV_VFF JPEG LN RQLLOG LO
            TLONG MARRAY MAXCELLS MINCELLS
            TOCTET OID PNG
            RE SCALE SDOM
            SHIFT TSHORT SINH SIN SQRT STRCT
            TANH TAN TIFF TOR 
            TULONG TUNSIG TUSHORT VFF XOR


%token <rasqlKeyword>
                PLUS
                MINUS
                MULT
                DIV
                MOD
                POWER
                EQUAL
                LESS
                GREATER
                LESSEQUAL
                GREATEREQUAL
                NOTEQUAL
                COLON
                SEMICOLON
                COMMA
                DOT
                LEPAR
                REPAR
                LRPAR
                RRPAR
                LCPAR
                RCPAR
                MDDPARAM


%token <postgisKeyword> POSTGIS_FUNCTION





/* Rasql statements */
%type <nodelist> spatialOpList2
%type <mynode> generalExp spatialOpList 
            spatialOp condenseOpLit inductionExp castType
            variable reduceIdent intLitExp generalLit
            mddLit scalarLit atomicLit scalarLitList ivList iv

            mddExp trimExp reduceExp functionExp integerExp mintervalExp 
            condenseExp  intervalExp

            complexLit marray_head

%type <str> IntegerLit StringLit FloatLit oidLit dimensionLitList Identifier
            
            

%type <str>    collectionIterator attributeIdent marrayVariable condenseVariable
                structSelection

%type <boolean> BooleanLit

%error-verbose
%locations
%debug

%%


/******************************** START POSTGRESQL *********************************/
/*
 *	Handle comment-only lines, and ;; SELECT * FROM pg_class ;;;
 *	psql already handles such cases, but other interfaces don't.
 *	bjm 1999/10/05
 */
stmtblock:	stmtmulti
                    { hqlQueries = $1; }
		;

/* the thrashing around here is to discard "empty" statements... */
stmtmulti:	stmtmulti SEMICOLON stmt
				{
                                    if ($3 != NULL)
                                    {
                                        $$ = cat3($1, (char*)"\n", $3->query);
                                    }
                                    else
                                    {
					$$ = $1;
                                    }
				}
			| stmt
					{
                                            if ($1 != NULL)
                                                $$ = $1->query;
					}
		;

stmt:	SelectStmt
                {
                    if ($1 != NULL)
                    {
                        DEBUG << "Parser matched query: " << $1->toString() << endl;
                        $$ = $1;
                        QueryTree::getInstance().load($1);
                        QueryTree::getInstance().execute();
                        cout << QUERY_PROMPT;
                    }
                    else
                    {
                        $$ = NULL;
                    }
                }
        |  // Empty
                {
                    $$ = NULL;
                }
            ;

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
			LRPAR select_no_parens RRPAR			
                            {
                                $$ = $2;
                                $2->query = cat3($1, $2->query, $3);
                            }
			| LRPAR select_with_parens RRPAR
                            {
                                $$ = $2;
                                $2->query = cat3($1, $2->query, $3);
                            }
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
                                    $$ = $1;
                                    $$->query = cat2($1->query, $2);
				}
/*
// AA: We do not want to support the locking clause FOR READ ONLY
			| select_clause opt_sort_clause for_locking_clause opt_select_limit
			| select_clause opt_sort_clause select_limit opt_for_locking_clause
*/
			| select_clause opt_sort_clause select_limit
                                {
                                    $$ = $1;
                                    $$->query = cat3($1->query, $2, $3);
				}
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
// AA: HQL does not yet support duplicate elimination through "DISTINCT"
//			SELECT opt_distinct target_list
                        SELECT target_list
// AA: We do not want to support SELECT INTO queries
//			into_clause from_clause where_clause
                        from_clause where_clause
//                        group_clause having_clause
// AA: We do not want to support r SQL windows
//			group_clause having_clause window_clause
                        {
                            QtSelectStatement *select = new QtSelectStatement($2, $3);
                            $$ = select;
                        }
                        
// AA: Disable VALUES clause, we do not want data to be input at run-time
//			| values_clause							{ $$ = $1; }
			| TABLE relation_expr
				{
                            /* same as SELECT * FROM relation_expr */
                            
                            QtList *from = new QtList();
                            QtDataSource src($2);
                            from->add(&src);
                            QtColumn *ref = new QtColumn((char*)"*");
                            QtList *what = new QtList();
                            what->add(ref);
                            QtSelectStatement *select = new QtSelectStatement(from, what);
                            select->query = cat2($1, $2->toCString());

                            $$ = select;
				}
			| select_clause UNION opt_all select_clause
				{
                            /* TODO: also include the second query in the struct */
                            $$ = $1;
                            $$->query = cat4($1->query, $2, $3, $4->query);
				}
			| select_clause INTERSECT opt_all select_clause
				{
                            /* TODO: also include the second query in the struct */
                            $$ = $1;
                            $$->query = cat4($1->query, $2, $3, $4->query);
				}
			| select_clause EXCEPT opt_all select_clause
				{
                            /* TODO: also include the second query in the struct */
                            $$ = $1;
                            $$->query = cat4($1->query, $2, $3, $4->query);
				}
		;

opt_all:	ALL										{ $$ = $1; }
			| DISTINCT								{ $$ = $1; }
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
			| sortby_list COMMA sortby				{ $$ = cat3($1, $2, $3); }
		;

sortby:		a_expr USING qual_all_Op opt_nulls_order
				{
                            $$ = cat4($1->toCString(), $2, $3, $4);
				}
			| a_expr opt_asc_desc opt_nulls_order
				{
                            $$ = cat3($1->toCString(), $2, $3);
				}
		;


opt_asc_desc: ASC                           { $$ = $1; }
			| DESC                          { $$ = $1; }
			| /*EMPTY*/                     { $$ = (char*) "SORTBY_DEFAULT"; }
			;

opt_nulls_order: NULLS_FIRST                { $$ = (char*)"NULLS FIRST"; }
				| NULLS_LAST                    { $$ = (char*)"NULLS LAST"; }
				| /*EMPTY*/                     { $$ = (char*)"SORTBY_NULLS_DEFAULT"; }
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
			| LIMIT select_limit_value COMMA select_offset_value
				{
					/* Disabled because it was too confusing, bjm 2002-02-18 */
                                        $$ = cat4($1, $2, $3, $4);
                                        // TODO: Report error
				}
			/* SQL:2008 syntax variants */
/*			| OFFSET select_offset_value2 row_or_rows
			| FETCH first_or_next opt_select_fetch_first_value row_or_rows ONLY
			| OFFSET select_offset_value2 row_or_rows FETCH first_or_next opt_select_fetch_first_value row_or_rows ONLY
*/
		;

select_limit_value:
			a_expr									{ $$ = $1->toCString(); }
			| ALL
				{
					/* LIMIT ALL is represented as a NULL constant */
                                        $$ = $1;
				}
		;


select_offset_value:
			a_expr									{ $$ = $1->toCString(); }
		;


/*****************************************************************************
 *
 *	clauses common to all Optimizable Stmts:
 *		from_clause		- allow list of both JOIN expressions and table names
 *		where_clause	- qualifications for joins or restrictions
 *
 *****************************************************************************/

from_clause:
			FROM from_list
                            { 
                                $$ = $2;
                            }
			| /*EMPTY*/
                            { $$ = NULL; }
		;

from_list:
			table_ref
                            {
                                $$ = new QtList();
                                $$->add($1);
                            }
			| from_list COMMA table_ref
                            {
                                $$ = $1;
                                $$->add($3);
                            }
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
					$$ = new QtDataSource($1);
				}
			| relation_expr alias_clause
				{
                                        $$ = new QtDataSource($1, $2);
				}
			| func_table
				{
                                        $$ = new QtDataSource($1);
				}
			| func_table alias_clause
				{
                                        $$ = new QtDataSource($1, $2);
				}
			| func_table AS LRPAR TableFuncElementList RRPAR
				{
                                        $$ = new QtDataSource($1);
				}
			| func_table AS ColId LRPAR TableFuncElementList RRPAR
				{
                                        $$ = new QtDataSource($1, $2);
				}
			| func_table ColId LRPAR TableFuncElementList RRPAR
				{
                                        $$ = new QtDataSource($1, $2);
				}
// AA: We do not allow subqueries in the FROM clause
/*			| select_with_parens
			| select_with_parens alias_clause
 */
			| joined_table
				{
                            /* TODO: Create nested structure for joined table */
					$$ = $1;
				}
			| LRPAR joined_table RRPAR alias_clause
				{
                            /* TODO: Create nested structure for joined table */
                                        $$ = $2;
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

/* TODO: Create nested structure for joined table */
joined_table:
			LRPAR joined_table RRPAR
				{
                                    $$ = $2;
				}
			| table_ref CROSS JOIN table_ref
				{
                            /* TODO: Also consider the second table in the join */
                                        $$ = $1;
				}
			| table_ref join_type JOIN table_ref join_qual
				{
                            /* TODO: Also consider the second table in the join */
                                        $$ = $1;
				}
			| table_ref JOIN table_ref join_qual
				{
                            /* TODO: Also consider the second table in the join */
                                        $$ = $1;
				}
			| table_ref NATURAL join_type JOIN table_ref
				{
                            /* TODO: Also consider the second table in the join */
                                        $$ = $1;
				}
			| table_ref NATURAL JOIN table_ref
				{
                            /* TODO: Also consider the second table in the join */
                                        $$ = $1;
				}
		;

alias_clause:
			AS ColId LRPAR name_list RRPAR
				{
                                        $$ = $2;
				}
			| AS ColId
				{
                                        $$ = $2;
				}
			| ColId LRPAR name_list RRPAR
				{
                                        $$ = $1;
				}
			| ColId
				{
                                        $$ = $1;
				}
		;

join_type:	FULL join_outer							{ $$ = $1; }
			| LEFT join_outer						{ $$ = $1; }
			| RIGHT join_outer						{ $$ = $1; }
			| INNER_P								{ $$ = $1; }
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

join_qual:	USING LRPAR name_list RRPAR					{ $$ = cat4($1, $2, $3, $4); }
			| ON a_expr								{ $$ = cat2($1, $2->toCString()); }
		;


relation_expr:
			qualified_name
				{
					/* default inheritance */
                                        $$ = $1;
				}
			| qualified_name MULT
				{
					/* inheritance query */
                                        $$ = $1;
				}
			| ONLY qualified_name
				{
					/* no inheritance */
                                        $$ = $2;
				}
			| ONLY LRPAR qualified_name RRPAR
				{
					/* no inheritance, SQL99-style syntax */
                                        $$ = $3;
				}
		;



func_table: func_expr								{ $$ = $1; }
		;


where_clause:
			WHERE a_expr							{ $$ = cat2($1, $2->toCString()); }
			| /*EMPTY*/								{ $$ = NULL; }
		;

TableFuncElementList:
			TableFuncElement
				{
					$$ = $1;
				}
			| TableFuncElementList COMMA TableFuncElement
				{
					$$ = cat3($1, $2, $3);
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

Typename:
// AA: Do not use SQL arrays, since their use conflicts with RaSQL arrays
//                        SimpleTypename opt_array_bounds
                        SimpleTypename
				{
                                        $$ = $1;
				}
//			| SETOF SimpleTypename opt_array_bounds
//			/* SQL standard syntax, currently only one-dimensional */
//			| SimpleTypename ARRAY LEPAR Iconst REPAR
//			| SETOF SimpleTypename ARRAY LEPAR Iconst REPAR
//			| SimpleTypename ARRAY
//			| SETOF SimpleTypename ARRAY
		;

//opt_array_bounds:
//			opt_array_bounds LEPAR REPAR
//			| opt_array_bounds LEPAR Iconst RRPAR
//			| /*EMPTY*/
//		;

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
			| ConstInterval LRPAR Iconst RRPAR opt_interval
				{
                                        $$ = cat5($1, $2, $3, $4, $5);
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

opt_type_modifiers: LRPAR expr_list RRPAR				{ $$ = cat3($1, $2->toCString(), $3); }
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

opt_float:	LRPAR Iconst RRPAR
				{
                                        $$ = cat3($1, $2, $3);
				}
			| /*EMPTY*/
				{
					$$ = (char*)"empty";
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
			BIT VARYING LRPAR expr_list RRPAR
				{
                                        $$ = cat5($1, $2, $3, $4->toCString(), $5);
				}
                        |
                        BIT LRPAR expr_list RRPAR
                                {
                                        $$ = cat4($1, $2, $3->toCString(), $4);
				}
		;

BitWithoutLength:
			BIT VARYING
				{
                                        $$ = cat2($1, $2);
				}
                        |
                        BIT
				{
                                        $$ = $1;
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

CharacterWithLength:  character LRPAR Iconst RRPAR opt_charset
				{
                                        $$ = cat5($1, $2, $3, $4, $5);
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
										{ $$ = $1; }
			| NATIONAL CHARACTER opt_varying
										{ $$ = cat3($1, $2, $3); }
			| NATIONAL CHAR_P opt_varying
										{ $$ = cat3($1, $2, $3); }
			| NCHAR opt_varying
										{ $$ = cat2($1, $2); }
		;

opt_varying:
// AA: Disable the VARYING keyword. It introduces ambiguities in the grammar. 
			VARYING					{ $$ = $1; }
			|
/*EMPTY*/								{ $$ = NULL; }
		;

opt_charset:
			CHARACTER SET ColId						{ $$ = $3; }
			| /*EMPTY*/								{ $$ = NULL; }
		;

/*
 * SQL92 date/time types
 */
ConstDatetime:
			TIMESTAMP LRPAR Iconst RRPAR opt_timezone
				{
                                        $$ = cat5($1, $2, $3, $4, $5);
				}
			| TIMESTAMP opt_timezone
				{
                                        $$ = cat2($1, $2);
				}
			| TIME LRPAR Iconst RRPAR opt_timezone
				{
                                        $$ = cat5($1, $2, $3, $4, $5);
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
			WITH_TIME ZONE			{ $$ = (char*) "WITH TIMEZONE"; }
			| WITHOUT TIME ZONE		{ $$ = (char*) "WITHOUT TIMEZONE"; }
			| /*EMPTY*/                     { $$ = (char*) "EMPTY (WITHOUT TIMEZONE)"; }
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
			| SECOND_P LRPAR Iconst RRPAR
				{
                            $$ = cat4($1, $2, $3, $4);
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
a_expr:		c_expr
            {
                $$ = $1;
            }
			| a_expr TYPECAST Typename
                            { $$ = new QtString(cat3($1->toCString(), $2, $3)); }
			| a_expr AT TIME ZONE a_expr
                            {
                                $$ = new QtString(cat5($1->toCString(), $2, $3, $4, $5->toCString()));
                            }

                /*
                 * AA: Include RaSQL expressions as well.
                 */

                        | generalExp
                            {
                                $$ = $1;
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
			| PLUS a_expr					%prec UMINUS
                                { $$ = new QtString(cat2($1, $2->toCString())); }
			| MINUS a_expr					%prec UMINUS
				{ $$ = new QtString(cat2($1, $2->toCString())); }
			| a_expr PLUS a_expr
				{ $$ = new QtBinaryOperation($1, strdup($2), $3); }
			| a_expr MINUS a_expr
				{ $$ = new QtBinaryOperation($1, strdup($2), $3); }
			| a_expr MULT a_expr
				{ $$ = new QtBinaryOperation($1, strdup($2), $3); }
			| a_expr DIV a_expr
				{ $$ = new QtBinaryOperation($1, strdup($2), $3); }
			| a_expr MOD a_expr
				{ $$ = new QtBinaryOperation($1, strdup($2), $3); }
			| a_expr POWER a_expr
				{ $$ = new QtBinaryOperation($1, strdup($2), $3); }
			| a_expr LESS a_expr
				{ $$ = new QtString(cat3($1->toCString(), $2, $3->toCString())); }
                        | a_expr LESSEQUAL a_expr
				{ $$ = new QtString(cat3($1->toCString(), $2, $3->toCString())); }
			| a_expr GREATER a_expr
				{ $$ = new QtString(cat3($1->toCString(), $2, $3->toCString())); }
                        | a_expr GREATEREQUAL a_expr
				{ $$ = new QtString(cat3($1->toCString(), $2, $3->toCString())); }
			| a_expr EQUAL a_expr
				{ $$ = new QtString(cat3($1->toCString(), $2, $3->toCString())); }
                        | a_expr NOTEQUAL a_expr
				{ $$ = new QtString(cat3($1->toCString(), $2, $3->toCString())); }

			| a_expr qual_Op a_expr				%prec Op
				{ $$ = new QtString(cat3($1->toCString(), $2, $3->toCString())); }
			| qual_Op a_expr					%prec Op
				{ $$ = new QtString(cat2($1, $2->toCString())); }
			| a_expr qual_Op					%prec POSTFIXOP
				{ $$ = new QtString(cat2($1->toCString(), $2)); }

			| a_expr AND a_expr
				{ $$ = new QtString(cat3($1->toCString(), $2, $3->toCString())); }
			| a_expr OR a_expr
				{ $$ = new QtString(cat3($1->toCString(), $2, $3->toCString())); }
			| NOT a_expr
				{ $$ = new QtString(cat2($1, $2->toCString())); }

			| a_expr LIKE a_expr
				{ $$ = new QtString(cat3($1->toCString(), $2, $3->toCString())); }
//			| a_expr LIKE a_expr ESCAPE a_expr
//				{ $$ = new QtString(cat5($1->toCString(), $2, $3->toCString(), $4, $5->toCString())); }
			| a_expr NOT LIKE a_expr
				{ $$ = new QtString(cat4($1->toCString(), $2, $3, $4->toCString())); }
//			| a_expr NOT LIKE a_expr ESCAPE a_expr
//				{ $$ = new QtString(cat6($1->toCString(), $2, $3, $4->toCString(), $5, $6->toCString())); }
//			| a_expr ILIKE a_expr
//				{ $$ = new QtString(cat3($1->toCString(), $2, $3->toCString())); }
//			| a_expr ILIKE a_expr ESCAPE a_expr
//				{ $$ = new QtString(cat5($1->toCString(), $2, $3->toCString(), $4, $5->toCString())); }
//			| a_expr NOT ILIKE a_expr
//				{ $$ = new QtString(cat4($1->toCString(), $2, $3, $4->toCString())); }
//			| a_expr NOT ILIKE a_expr ESCAPE a_expr
//				{ $$ = new QtString(cat6($1->toCString(), $2, $3, $4->toCString(), $5, $6->toCString())); }
//
//			| a_expr SIMILAR TO a_expr				%prec SIMILAR
//				{ $$ = new QtString(cat4($1->toCString(), $2, $3, $4->toCString())); }
//			| a_expr SIMILAR TO a_expr ESCAPE a_expr
//				{ $$ = new QtString(cat6($1->toCString(), $2, $3, $4->toCString(), $5, $6->toCString())); }
//			| a_expr NOT SIMILAR TO a_expr			%prec SIMILAR
//				{ $$ = new QtString(cat5($1->toCString(), $2, $3, $4, $5->toCString())); }
//			| a_expr NOT SIMILAR TO a_expr ESCAPE a_expr
//				{ $$ = new QtString(cat7($1->toCString(), $2, $3, $4, $5->toCString(), $6, $7->toCString())); }

			/* NullTest clause
			 * Define SQL92-style Null test clause.
			 * Allow two forms described in the standard:
			 *	a IS NULL
			 *	a IS NOT NULL
			 * Allow two SQL extensions
			 *	a ISNULL
			 *	a NOTNULL
			 */
//			| a_expr IS NULL_P
//				{ $$ = new QtString(cat3($1->toCString(), $2, $3)); }
//			| a_expr ISNULL
//				{ $$ = new QtString(cat2($1->toCString(), $2)); }
//			| a_expr IS NOT NULL_P
//				{ $$ = new QtString(cat4($1->toCString(), $2, $3, $4)); }
//			| a_expr NOTNULL
//				{ $$ = new QtString(cat2($1->toCString(), $2)); }
//			| row OVERLAPS row
//				{ $$ = new QtString(cat3($1->toCString(), $2, $3->toCString())); }
//			| a_expr IS TRUE_P
//				{ $$ = new QtString(cat3($1->toCString(), $2, $3)); }
//			| a_expr IS NOT TRUE_P
//				{ $$ = new QtString(cat4($1->toCString(), $2, $3, $4)); }
//			| a_expr IS FALSE_P
//				{ $$ = new QtString(cat3($1->toCString(), $2, $3)); }
//			| a_expr IS NOT FALSE_P
//				{ $$ = new QtString(cat4($1->toCString(), $2, $3, $4)); }
//			| a_expr IS UNKNOWN
//				{ $$ = new QtString(cat3($1->toCString(), $2, $3)); }
//			| a_expr IS NOT UNKNOWN
//				{ $$ = new QtString(cat4($1->toCString(), $2, $3, $4)); }
//			| a_expr IS DISTINCT FROM a_expr			%prec IS
//				{
//                                        $$ = new QtString(cat5($1->toCString(), $2, $3, $4, $5->toCString()));
//				}
//			| a_expr IS NOT DISTINCT FROM a_expr		%prec IS
//				{
//                            $$ = new QtString(cat6($1->toCString(), $2, $3, $4, $5, $6->toCString()));
//				}
//			| a_expr IS OF LRPAR type_list RRPAR			%prec IS
//				{
//                            $$ = new QtString(cat6($1->toCString(), $2, $3, $4, $5->toCString(), $6));
//				}
//			| a_expr IS NOT OF LRPAR type_list RRPAR		%prec IS
//				{
//                            $$ = new QtString(cat7($1->toCString(), $2, $3, $4, $5, $6->toCString(), $7));
//				}
//			/*
//			 *	Ideally we would not use hard-wired operators below but instead use
//			 *	opclasses.  However, mixed data types and other issues make this
//			 *	difficult:  http://archives.postgresql.org/pgsql-hackers/2008-08/msg01142.php
//			 */
//			| a_expr BETWEEN opt_asymmetric b_expr AND b_expr		%prec BETWEEN
//				{
//                            $$ = new QtString(cat6($1->toCString(), $2, $3, $4->toCString(), $5, $6->toCString()));
//				}
//			| a_expr NOT BETWEEN opt_asymmetric b_expr AND b_expr	%prec BETWEEN
//				{
//                            $$ = new QtString(cat7($1->toCString(), $2, $3, $4, $5->toCString(), $6, $7->toCString()));
//				}
//			| a_expr BETWEEN SYMMETRIC b_expr AND b_expr			%prec BETWEEN
//				{
//                            $$ = new QtString(cat6($1->toCString(), $2, $3, $4->toCString(), $5, $6->toCString()));
//				}
//			| a_expr NOT BETWEEN SYMMETRIC b_expr AND b_expr		%prec BETWEEN
//				{
//                            $$ = new QtString(cat7($1->toCString(), $2, $3, $4, $5->toCString(), $6, $7->toCString()));
//				}
//			| a_expr IN_P in_expr
//				{
//                            $$ = new QtString(cat3($1->toCString(), $2, $3->toCString()));
//				}
//			| a_expr NOT IN_P in_expr
//				{
//                            $$ = new QtString(cat4($1->toCString(), $2, $3, $4->toCString()));
//				}
///*
//// AA: We do not allow nested sub-queries to occur
//			| a_expr subquery_Op sub_type select_with_parens	%prec Op
//*/
//			| a_expr subquery_Op sub_type LRPAR a_expr RRPAR		%prec Op
//				{
//                            $$ = new QtString(cat6($1->toCString(), $2, $3, $4, $5->toCString(), $6));
//				}
//// AA: We do not allow nested sub-queries to occur
////			| UNIQUE select_with_parens
//			| a_expr IS DOCUMENT_P					%prec IS
//				{
//                            $$ = new QtString(cat3($1->toCString(), $2, $3));
//				}
//			| a_expr IS NOT DOCUMENT_P				%prec IS
//				{
//                            $$ = new QtString(cat4($1->toCString(), $2, $3, $4));
//				}
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
				{ $$ = new QtString($1->toString()); }
			| b_expr TYPECAST Typename
				{ $$ = new QtString(cat3($1->toCString(), $2, $3)); }
			| PLUS b_expr					%prec UMINUS
				{ $$ = new QtString(cat2($1, $2->toCString())); }
			| MINUS b_expr					%prec UMINUS
				{ $$ = new QtString(cat2($1, $2->toCString())); }
			| b_expr PLUS b_expr
				{ $$ = new QtString(cat3($1->toCString(), $2, $3->toCString())); }
			| b_expr MINUS b_expr
				{ $$ = new QtString(cat3($1->toCString(), $2, $3->toCString())); }
			| b_expr MULT b_expr
				{ $$ = new QtString(cat3($1->toCString(), $2, $3->toCString())); }
			| b_expr DIV b_expr
				{ $$ = new QtString(cat3($1->toCString(), $2, $3->toCString())); }
			| b_expr MOD b_expr
				{ $$ = new QtString(cat3($1->toCString(), $2, $3->toCString())); }
			| b_expr POWER b_expr
				{ $$ = new QtString(cat3($1->toCString(), $2, $3->toCString())); }
			| b_expr LESS b_expr
				{ $$ = new QtString(cat3($1->toCString(), $2, $3->toCString())); }
                        | b_expr LESSEQUAL b_expr
				{ $$ = new QtString(cat3($1->toCString(), $2, $3->toCString())); }
			| b_expr GREATER b_expr
				{ $$ = new QtString(cat3($1->toCString(), $2, $3->toCString())); }
                        | b_expr GREATEREQUAL b_expr
				{ $$ = new QtString(cat3($1->toCString(), $2, $3->toCString())); }
			| b_expr EQUAL b_expr
				{ $$ = new QtString(cat3($1->toCString(), $2, $3->toCString())); }
                        | b_expr NOTEQUAL b_expr
				{ $$ = new QtString(cat3($1->toCString(), $2, $3->toCString())); }
			| b_expr qual_Op b_expr				%prec Op
				{ $$ = new QtString(cat3($1->toCString(), $2, $3->toCString())); }
			| qual_Op b_expr					%prec Op
				{ $$ = new QtString(cat2($1, $2->toCString())); }
			| b_expr qual_Op					%prec POSTFIXOP
				{ $$ = new QtString(cat2($1->toCString(), $2)); }
//			| b_expr IS DISTINCT FROM b_expr		%prec IS
//				{ $$ = new QtString(cat5($1->toCString(), $2, $3, $4, $5->toCString())); }
//			| b_expr IS NOT DISTINCT FROM b_expr	%prec IS
//				{ $$ = new QtString(cat6($1->toCString(), $2, $3, $4, $5, $6->toCString())); }
//			| b_expr IS OF LRPAR type_list RRPAR		%prec IS
//				{
//                                        $$ = new QtString(cat6($1->toCString(), $2, $3, $4, $5->toCString(), $6));
//				}
//			| b_expr IS NOT OF LRPAR type_list RRPAR	%prec IS
//				{
//                            $$ = new QtString(cat7($1->toCString(), $2, $3, $4, $5, $6->toCString(), $7));
//				}
//			| b_expr IS DOCUMENT_P					%prec IS
//				{
//                            $$ = new QtString(cat3($1->toCString(), $2, $3));
//				}
//			| b_expr IS NOT DOCUMENT_P				%prec IS
//				{
//                            $$ = new QtString(cat4($1->toCString(), $2, $3, $4));
//				}
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
                            $$ = new QtString(cat2($1, $2));
				}
			| LRPAR a_expr RRPAR opt_indirection
				{
                            $$ = new QtString(cat4($1, $2->toCString(), $3, $4));
				}
			| case_expr
				{ $$ = new QtString($1); }
			| func_expr
				{ $$ = $1; }
/*
// AA: We do not allow nested sub-queries to occur
			| select_with_parens			%prec UMINUS
			| EXISTS select_with_parens
			| ARRAY select_with_parens
// AA: Disallow SQL array constructs. They conflict with RaSQL arrays.
			| ARRAY array_expr
*/
			| row
				{
                                        $$ = new QtString($1->toString());
				}
		;

postgis_func_name:
                        POSTGIS_FUNCTION
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
func_expr:
                        postgis_func_name LRPAR a_expr RRPAR
                                {
                                    $$ = new QtPostGisFunction($1, $3);
                                }
                        | func_name LRPAR RRPAR over_clause
				{
                                        $$ = new QtSqlFunction($1, NULL);
				}
			| func_name LRPAR expr_list RRPAR over_clause
				{
                                        $$ = new QtSqlFunction($1, $3);
				}
//			| func_name LRPAR VARIADIC a_expr RRPAR over_clause
//				{
//                                        $$ = new QtString(cat6($1, $2, $3, $4->toCString(), $5, $6));
//				}
//			| func_name LRPAR expr_list COMMA VARIADIC a_expr RRPAR over_clause
//				{
//                                        $$ = new QtString(cat8($1, $2, $3->toCString(), $4, $5, $6->toCString(), $7, $8));
//				}
//			| func_name LRPAR ALL expr_list RRPAR over_clause
//				{
//                                        $$ = new QtString(cat6($1, $2, $3, $4->toCString(), $5, $6));
//				}
//			| func_name LRPAR DISTINCT expr_list RRPAR over_clause
//				{
//                                        $$ = new QtString(cat6($1, $2, $3, $4->toCString(), $5, $6));
//				}
//			| func_name LRPAR MULT RRPAR over_clause
//				{
//                                        $$ = new QtString(cat5($1, $2, $3, $4, $5));
//				}
			| CURRENT_DATE
				{
                                        $$ = new QtString($1);
				}
			| CURRENT_TIME
				{
					$$ = new QtString($1);
				}
			| CURRENT_TIME LRPAR Iconst RRPAR
				{
					$$ = new QtString(cat4($1, $2, $3, $4));
				}
			| CURRENT_TIMESTAMP
				{
					$$ = new QtString($1);
				}
			| CURRENT_TIMESTAMP LRPAR Iconst RRPAR
				{
                                       $$ = new QtString(cat4($1, $2, $3, $4));
				}
			| LOCALTIME
				{
					$$ = new QtString($1);
				}
			| LOCALTIME LRPAR Iconst RRPAR
				{
                                        $$ = new QtString(cat4($1, $2, $3, $4));
				}
			| LOCALTIMESTAMP
				{
                                        $$ = new QtString($1);
				}
			| LOCALTIMESTAMP LRPAR Iconst RRPAR
				{
                                        $$ = new QtString(cat4($1, $2, $3, $4));
				}
			| CURRENT_ROLE
				{
					$$ = new QtString($1);
				}
			| CURRENT_USER
				{
					$$ = new QtString($1);
				}
			| SESSION_USER
				{
					$$ = new QtString($1);
				}
			| USER
				{
					$$ = new QtString($1);
				}
			| CURRENT_CATALOG
				{
					$$ = new QtString($1);
				}
			| CURRENT_SCHEMA
				{
					$$ = new QtString($1);
				}
			| CAST LRPAR a_expr AS Typename RRPAR
				{ 
                                        $$ = new QtString(cat6($1, $2, $3->toCString(), $4, $5, $6));
                                }
			| EXTRACT LRPAR extract_list RRPAR
				{
                                        $$ = new QtString(cat4($1, $2, $3, $4));
				}
			| OVERLAY LRPAR overlay_list RRPAR
				{
                                        $$ = new QtString(cat4($1, $2, $3, $4));
				}
			| POSITION LRPAR position_list RRPAR
				{
                                        $$ = new QtString(cat4($1, $2, $3, $4));
				}
			| SUBSTRING LRPAR substr_list RRPAR
				{
                                        $$ = new QtString(cat4($1, $2, $3, $4));
				}
			| TREAT LRPAR a_expr AS Typename RRPAR
				{
                                        $$ = new QtString(cat6($1, $2, $3->toCString(), $4, $5, $6));
				}
			| TRIM LRPAR BOTH trim_list RRPAR
				{
                                        $$ = new QtString(cat5($1, $2, $3, $4, $5));
				}
			| TRIM LRPAR LEADING trim_list RRPAR
				{
                                        $$ = new QtString(cat5($1, $2, $3, $4, $5));
				}
			| TRIM LRPAR TRAILING trim_list RRPAR
				{
                                        $$ = new QtString(cat5($1, $2, $3, $4, $5));
				}
			| TRIM LRPAR trim_list RRPAR
				{
                                        $$ = new QtString(cat4($1, $2, $3, $4));
				}
			| NULLIF LRPAR a_expr COMMA a_expr RRPAR
				{
                                        $$ = new QtString(cat6($1, $2, $3->toCString(), $4, $5->toCString(), $6));
				}
			| COALESCE LRPAR expr_list RRPAR
				{
                                        $$ = new QtString(cat4($1, $2, $3->toCString(), $4));
				}
			| GREATEST LRPAR expr_list RRPAR
				{
                                        $$ = new QtString(cat4($1, $2, $3->toCString(), $4));
				}
			| LEAST LRPAR expr_list RRPAR
				{
                                        $$ = new QtString(cat4($1, $2, $3->toCString(), $4));
				}
//			| XMLCONCAT LRPAR expr_list RRPAR
//				{
//                                        $$ = new QtString(cat4($1, $2, $3, $4));
//				}
//			| XMLELEMENT LRPAR NAME_P ColLabel RRPAR
//				{
//                                        $$ = new QtString(cat5($1, $2, $3, $4, $5));
//				}
//			| XMLELEMENT LRPAR NAME_P ColLabel COMMA xml_attributes RRPAR
//				{
//                                        $$ = new QtString(cat7($1, $2, $3, $4, $5, $6, $7));
//				}
//			| XMLELEMENT LRPAR NAME_P ColLabel COMMA expr_list RRPAR
//				{
//                                        $$ = new QtString(cat7($1, $2, $3, $4, $5, $6, $7));
//				}
//			| XMLELEMENT LRPAR NAME_P ColLabel COMMA xml_attributes COMMA expr_list RRPAR
//				{
//                                        $$ = new QtString(cat9($1, $2, $3, $4, $5, $6, $7, $8, $9));
//				}
//			| XMLFOREST LRPAR xml_attribute_list RRPAR
//				{
//                                        $$ = new QtString(cat4($1, $2, $3, $4));
//				}
//			| XMLPARSE LRPAR document_or_content a_expr xml_whitespace_option RRPAR
//				{
//                                        $$ = new QtString(cat6($1, $2, $3, $4->toCString(), $5, $6));
//				}
//			| XMLPI LRPAR NAME_P ColLabel RRPAR
//				{
//                                        $$ = new QtString(cat5($1, $2, $3, $4, $5));
//				}
//			| XMLPI LRPAR NAME_P ColLabel COMMA a_expr RRPAR
//				{
//                                        $$ = new QtString(cat7($1, $2, $3, $4, $5, $6->toCString(), $7));
//				}
//			| XMLROOT LRPAR a_expr COMMA xml_root_version opt_xml_root_standalone RRPAR
//				{
//                                        $$ = new QtString(cat7($1, $2, $3->toCString(), $4, $5, $6, $7));
//				}
//			| XMLSERIALIZE LRPAR document_or_content a_expr AS SimpleTypename RRPAR
//				{
//                                        $$ = new QtString(cat7($1, $2, $3, $4->toCString(), $5, $6, $7));
//				}
		;
//
///*
// * SQL/XML support
// */
//xml_root_version: VERSION_P a_expr
//				{ $$ = cat2($1, $2->toCString()); }
//			| VERSION_P NO VALUE_P
//				{ $$ = cat2($1, $2); }
//		;
//
//opt_xml_root_standalone: COMMA STANDALONE_P YES_P
//				{ $$ = cat3($1, $2, $3); }
//			| COMMA STANDALONE_P NO
//				{ $$ = cat3($1, $2, $3); }
//			| COMMA STANDALONE_P NO VALUE_P
//				{ $$ = cat3($1, $2, $3); }
//			| /*EMPTY*/
//				{ $$ = NULL; }
//		;
//
//xml_attributes: XMLATTRIBUTES LRPAR xml_attribute_list RRPAR	{ $$ = cat4($1, $2, $3, $4); }
//		;
//
//xml_attribute_list:	xml_attribute_el					{ $$ = $1; }
//			| xml_attribute_list COMMA xml_attribute_el	{ $$ = cat3($1, $2, $3); }
//		;
//
//xml_attribute_el: a_expr AS ColLabel
//				{
//                                        $$ = cat3($1->toCString(), $2, $3);
//				}
//			| a_expr
//				{
//					$$ = $1->toCString();
//				}
//		;
//
//document_or_content: DOCUMENT_P						{ $$ = $1; }
//			| CONTENT_P					{ $$ = $1; }
//		;
//
//xml_whitespace_option: PRESERVE WHITESPACE_P		{ $$ = (char*) "TRUE"; }
//			| STRIP_P WHITESPACE_P		{ $$ = (char*) "FALSE"; }
//			| /*EMPTY*/			{ $$ = (char*) "FALSE"; }
//		;


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
row:		ROW LRPAR expr_list RRPAR					{ $$ = new QtString(cat4($1, $2, $3->toCString(), $4)); }
			| ROW LRPAR RRPAR					{ $$ = NULL; }
			| LRPAR expr_list COMMA a_expr RRPAR			{ $$ = new QtString(cat5($1, $2->toCString(), $3, $4->toCString(), $5)); }
		;

sub_type:	ANY										{ $$ = $1; }
			| SOME									{ $$ = $1; }
			| ALL									{ $$ = $1; }
		;

all_Op:		Op										{ $$ = $1; }
			| MathOp								{ $$ = $1; }
		;

MathOp:                 PLUS									{ $$ = $1; }
			| MINUS									{ $$ = $1; }
			| MULT									{ $$ = $1; }
			| DIV									{ $$ = $1; }
			| MOD									{ $$ = $1; }
			| POWER									{ $$ = $1; }
			| LESS									{ $$ = $1; }
                        | LESSEQUAL								{ $$ = $1; }
			| GREATER								{ $$ = $1; }
                        | GREATEREQUAL								{ $$ = $1; }
			| EQUAL									{ $$ = $1; }
                        | NOTEQUAL								{ $$ = $1; }
		;

qual_Op:	Op
					{ $$ = $1; }
			| OPERATOR LRPAR any_operator RRPAR
					{ $$ = cat4($1, $2, $3, $4); }
		;

qual_all_Op:
			all_Op
					{ $$ = $1; }
			| OPERATOR LRPAR any_operator RRPAR
					{ $$ = cat4($1, $2, $3, $4); }
		;

subquery_Op:
			all_Op
					{ $$ = $1; }
			| OPERATOR LRPAR any_operator RRPAR
					{ $$ = cat4($1, $2, $3, $4); }
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
                                        $$ = new QtList();
                                        $$->add($1);
				}
			| expr_list COMMA a_expr
				{
                                        $$->add($3);
				}
		;

type_list:	Typename								{ $$ = new QtString($1); }
			| type_list COMMA Typename				{ $$ = new QtString(cat3($1->toCString(), $2, $3)); }
		;

extract_list:
			extract_arg FROM a_expr
				{
                                    $$ = cat3($1, $2, $3->toCString());
				}
			| /*EMPTY*/								{ $$ = NULL; }
		;

extract_arg:
			IDENT									{ $$ = $1; }
			| YEAR_P								{ $$ = (char*) "year"; }
			| MONTH_P								{ $$ = (char*) "month"; }
			| DAY_P									{ $$ = (char*) "day"; }
			| HOUR_P								{ $$ = (char*) "hour"; }
			| MINUTE_P								{ $$ = (char*) "minute"; }
			| SECOND_P								{ $$ = (char*) "second"; }
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
                                        $$ = cat4($1->toCString(), $2, $3, $4);
				}
			| a_expr overlay_placing substr_from
				{
                                        $$ = cat3($1->toCString(), $2, $3);
				}
		;

overlay_placing:
			PLACING a_expr
				{ $$ = cat2($1, $2->toCString()); }
		;

/* position_list uses b_expr not a_expr to avoid conflict with general IN */

position_list:
			b_expr IN_P b_expr						{ $$ = cat3($1->toCString(), $2, $3->toCString()); }
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
                            $$ = cat3($1->toCString(), $2, $3);
				}
			| a_expr substr_for substr_from
				{
					/* not legal per SQL99, but might as well allow it */
                            $$ = cat3($1->toCString(), $2, $3);
				}
			| a_expr substr_from
				{
                            $$ = cat2($1->toCString(), $2);
				}
			| a_expr substr_for
				{
					$$ = cat2($1->toCString(), $2);
				}
			| expr_list
				{
					$$ = $1->toCString();
				}
			| /*EMPTY*/
				{ $$ = NULL; }
		;

substr_from:
			FROM a_expr								{ $$ = cat2($1, $2->toCString()); }
		;

substr_for: FOR a_expr								{ $$ = cat2($1, $2->toCString()); }
		;

trim_list:	a_expr FROM expr_list					{ $$ = cat3($1->toCString(), $2, $3->toCString()); }
			| FROM expr_list						{ $$ = cat2($1, $2->toCString()); }
			| expr_list								{ $$ = $1->toCString(); }
		;

in_expr:	
// AA: Disable correlated subqueries: SELECT ... FROM ... WHERE expr IN (in_expr: SELECT ...)
/*                select_with_parens
			|
*/
                LRPAR expr_list RRPAR						{ $$ = new QtString(cat3($1, $2->toCString(), $3)); }
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
                                    $$ = cat4($1, $2->toCString(), $3, $4->toCString());
				}
		;

case_default:
			ELSE a_expr								{ $$ = cat2($1, $2->toCString()); }
			| /*EMPTY*/								{ $$ = NULL; }
		;

case_arg:	a_expr									{ $$ = $1->toCString(); }
			| /*EMPTY*/								{ $$ = NULL; }
		;

/*
 * columnref starts with relation_name not ColId, so that OLD and NEW
 * references can be accepted.	Note that when there are more than two
 * dotted names, the first name is not actually a relation name...
 */
columnref:	relation_name
				{
					$$ = new QtColumn($1);
				}
			| relation_name indirection
				{
                                        $$ = new QtColumn($1, $2);
				}
		;

indirection_el:
			DOT attr_name
				{
                                        $$ = new QtString($2);
				}
			| DOT MULT
				{
                                        $$ = new QtString($2);
				}
/*
AA: Disallow syntax that would interfere with the RaSQL array
			| LEPAR a_expr REPAR
			| LEPAR a_expr COLON a_expr REPAR
*/
		;

indirection:
			indirection_el							{ $$ = $1->toCString(); }
			| indirection indirection_el			{ $$ = cat2($1, $2->toCString()); }
		;

opt_indirection:
			/*EMPTY*/								{ $$ = NULL; }
			| opt_indirection indirection_el		{ $$ = cat2($1, $2->toCString()); }
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
			target_el								
                            { $$ = new QtList(); $$->add($1); }
			| target_list COMMA target_el
                            { $$ = $1; $$->add($3); }
		;

target_el:	
                        a_expr AS ColLabel
				{
                                        // FIXME: allow the column label
                                        $$ = new QtColumn($1->toCString());
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
                                        $$ = $1;
				}
			| a_expr
				{
                                        $$ = $1;
				}
			| MULT
				{
                                        $$ = new QtColumn($1);
				}
/* AA: RaSQL expressions allowed in the SELECT clause */
                        | generalExp
                                {
                                        $$ = $1;
                                }
		;


/*****************************************************************************
 *
 *	Names and constants
 *
 *****************************************************************************/

relation_name:
			SpecialRuleRelation		{ $$ = $1; }
			| ColId				{ $$ = $1; }
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
                                        $$ = new QtColumn($1);
				}
			| relation_name indirection
				{
                                        $$ = new QtColumn($1, $2);
				}
		;

name_list:	name
					{ $$ = $1; }
                | name_list COMMA name
					{ $$ = cat3($1, $2, $3); }
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
AexprConst:             Iconst
				{
					$$ = new QtConst($1, "integer");
				}
			| FCONST
				{
					$$ = new QtConst($1, "float");
				}
			| Sconst
				{
					$$ = new QtConst($1, "string");
				}
			| BCONST
				{
					$$ = new QtConst($1, "bool");
                                }
			| XCONST
				{
					$$ = new QtConst($1, "hex");
                                }
			| func_name Sconst
				{
                                        $$ = new QtConst(cat2($1, $2), "func");
                                }
//			| func_name LRPAR expr_list RRPAR Sconst
//				{
//                                        $$ = new QtString(cat5($1, $2, $3, $4, $5));
//                                }
//			| ConstTypename Sconst
//				{
//                                        $$ = new QtConst(cat2($1, $2), "cast string");
//                                }
//			| ConstInterval Sconst opt_interval
//				{
//                                        $$ = new QtString(cat3($1, $2, $3));
//                                }
//			| ConstInterval LRPAR Iconst RRPAR Sconst opt_interval
//				{
//                                        $$ = new QtString(cat6($1, $2, $3, $4, $5, $6));
//                                }
			| TRUE_P
				{
					$$ = new QtConst($1, "bool");
                                }
			| FALSE_P
				{
					$$ = new QtConst($1, "bool");
                                }
			| NULL_P
				{
					$$ = new QtConst($1, "null");
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
type_function_name:	IDENT							{ $$ = strdup($1); }
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
// AA: Include some RaSQL keywords here
                        | OID                                                   { $$ = $1; }
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
//			| CSV
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

attrs:      DOT attr_name
				{ $$ = cat2($1, $2); }
			| attrs DOT attr_name
				{ $$ = cat3($1, $2, $3); }
			;


any_operator:
            all_Op
				{ $$ = $1; }
			| ColId DOT any_operator
				{ $$ = cat3($1, $2, $3); }
			;

/******************************** END POSTGRESQL *********************************/




/******************************** START RASQL *********************************/

generalExp: mddExp                          { $$ = $1; }
	| trimExp                           { $$ = $1; }
	| reduceExp                         { $$ = $1; }
	| inductionExp                      { $$ = $1; }
	| functionExp                       { $$ = $1; }
	| integerExp                        { $$ = $1; }
	| condenseExp                       { $$ = $1; }
	| variable                          { $$ = $1; }
	| mintervalExp                      { $$ = $1; }
	| intervalExp                       { $$ = $1; }
	| generalLit                        { $$ = $1; }
        ;

integerExp: generalExp DOT LO
	{
            $$ = $1;
	}
	| generalExp DOT HI
	{
            $$ = $1;
	};

mintervalExp: LEPAR spatialOpList REPAR
	{
            $$ = $2;
	}
	| SDOM LRPAR collectionIterator RRPAR
	{
            $$ = new QtString(cat4($1, $2, $3, $4));
	};

spatialOpList:  // empty
	{
            $$ = new QtString("emptySpatialOpList");
	}
	| spatialOpList2
	{
            $$ = $1;
	};

spatialOpList2: spatialOpList2 COMMA spatialOp
	{
            $$->add($3);
	}
	| spatialOp
	{
            $$ = new QtList();
            $$->add($1);
	};

spatialOp: generalExp        { $$ = $1; };

intervalExp: generalExp COLON generalExp
	{
            $$ = new QtRasqlInterval($1, $3);
	}
	| MULT COLON generalExp
	{
            $$ = new QtRasqlInterval("*", $3);
	}
	| generalExp COLON MULT
	{
            $$ = new QtRasqlInterval($1, "*");
	}
	| MULT COLON MULT
	{
            $$ = new QtRasqlInterval("*", "*");
	};

condenseExp: CONDENSE condenseOpLit OVER condenseVariable IN_P generalExp WHERE generalExp USING generalExp
	{
            $$ = new QtString(cat10($1, $2->toCString(), $3, $4, $5, $6->toCString(),
                    $7, $8->toCString(), $9, $10->toCString()));
	}
	| CONDENSE condenseOpLit OVER condenseVariable IN_P generalExp USING generalExp
	{
            $$ = new QtString(cat8($1, $2->toCString(), $3, $4, $5, $6->toCString(),
                    $7, $8->toCString()));
	};

condenseOpLit: PLUS
	{
            $$ = new QtString($1);
	}
	| MINUS
	{
            $$ = new QtString($1);
	}
	| MULT
	{
            $$ = new QtString($1);
	}
	| DIV
	{
            $$ = new QtString($1);
	}
	| AND
	{
            $$ = new QtString($1);
	}
	| OR
	{
            $$ = new QtString($1);
	};

functionExp: OID LRPAR collectionIterator RRPAR
	{
            $$ = new QtString($1);
	}
	| SHIFT LRPAR generalExp COMMA generalExp RRPAR
	{
            $$ = new QtRasqlFunction($1, $3, $5);
	}
	| EXTEND LRPAR generalExp COMMA generalExp RRPAR
	{
            $$ = new QtRasqlFunction($1, $3, $5);
	}
	| SCALE LRPAR generalExp COMMA generalExp RRPAR
	{
            $$ = new QtRasqlFunction($1, $3, $5);
	}
	| BIT LRPAR generalExp COMMA generalExp RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	}
	| TIFF LRPAR generalExp COMMA StringLit RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	}
	| TIFF LRPAR generalExp  RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	}
	| BMP LRPAR generalExp COMMA StringLit RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	}
	| BMP LRPAR generalExp  RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	}
	| HDF LRPAR generalExp COMMA StringLit RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	}
	| HDF LRPAR generalExp  RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	}
	| CSV LRPAR generalExp COMMA StringLit RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	}
	| CSV LRPAR generalExp  RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	}
	| JPEG LRPAR generalExp COMMA StringLit RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	}
	| JPEG LRPAR generalExp  RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	}
	| PNG LRPAR generalExp COMMA StringLit RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	}
	| PNG LRPAR generalExp  RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	}
	| VFF LRPAR generalExp COMMA StringLit RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	}
	| VFF LRPAR generalExp  RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	}
	| TOR LRPAR generalExp COMMA StringLit RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	}
	| TOR LRPAR generalExp  RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	}
	| DEM LRPAR generalExp COMMA StringLit RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	}
	| DEM LRPAR generalExp  RRPAR
	{
           $$ = new QtEncodeArray($3, $1);
	}
	| INV_TIFF LRPAR generalExp COMMA StringLit RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	}
	| INV_TIFF LRPAR generalExp  RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	}
	| INV_BMP LRPAR generalExp COMMA StringLit RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	}
	| INV_BMP LRPAR generalExp  RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	}
	| INV_HDF LRPAR generalExp COMMA StringLit RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	}
	| INV_HDF LRPAR generalExp  RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	}
	| INV_CSV LRPAR generalExp COMMA StringLit RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	}
	| INV_CSV LRPAR generalExp  RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	}
	| INV_JPEG LRPAR generalExp COMMA StringLit RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	}
	| INV_JPEG LRPAR generalExp  RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	}
	| INV_PNG LRPAR generalExp COMMA StringLit RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	}
	| INV_PNG LRPAR generalExp  RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	}
	| INV_VFF LRPAR generalExp COMMA StringLit RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	}
	| INV_VFF LRPAR generalExp  RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	}
	| INV_TOR LRPAR generalExp COMMA StringLit RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	}
	| INV_TOR LRPAR generalExp  RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	}
	| INV_DEM LRPAR generalExp COMMA StringLit RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	}
	| INV_DEM LRPAR generalExp  RRPAR
	{
            $$ = new QtEncodeArray($3, $1);
	};


structSelection: DOT attributeIdent
	{
            $$ = cat2($1, $2);
	}
	| DOT intLitExp
	{
            $$ = cat2($1, $2->toCString());
	};

inductionExp: SQRT LRPAR generalExp RRPAR
	{
//            $$ = cat4($1, $2, $3->toCString(), $4);
            $$ = $3;
 	}
	| ABS LRPAR generalExp RRPAR
	{
//            $$ = cat4($1, $2, $3->toCString(), $4);
            $$ = $3;
	}
	| EXP LRPAR generalExp RRPAR
	{
//            $$ = cat4($1, $2, $3->toCString(), $4);
            $$ = $3;
	}
	| RQLLOG LRPAR generalExp RRPAR
	{
//            $$ = cat4($1, $2, $3->toCString(), $4);
            $$ = $3;
	}
	| LN LRPAR generalExp RRPAR
	{
//            $$ = cat4($1, $2, $3->toCString(), $4);
            $$ = $3;
	}
	| SIN LRPAR generalExp RRPAR
	{
//            $$ = cat4($1, $2, $3->toCString(), $4);
            $$ = $3;
	}
	| COS LRPAR generalExp RRPAR
	{
//            $$ = cat4($1, $2, $3->toCString(), $4);
            $$ = $3;
	}
	| TAN LRPAR generalExp RRPAR
	{
//            $$ = cat4($1, $2, $3->toCString(), $4);
            $$ = $3;
	}
	| SINH LRPAR generalExp RRPAR
	{
//            $$ = cat4($1, $2, $3->toCString(), $4);
            $$ = $3;
	}
	| COSH LRPAR generalExp RRPAR
	{
//            $$ = cat4($1, $2, $3->toCString(), $4);
            $$ = $3;
	}
	| TANH LRPAR generalExp RRPAR
	{
//            $$ = cat4($1, $2, $3->toCString(), $4);
            $$ = $3;
	}
	| ARCSIN LRPAR generalExp RRPAR
	{
//            $$ = cat4($1, $2, $3->toCString(), $4);
            $$ = $3;
	}
	| ARCCOS LRPAR generalExp RRPAR
	{
//            $$ = cat4($1, $2, $3->toCString(), $4);
            $$ = $3;
	}
	| ARCTAN LRPAR generalExp RRPAR
	{
//            $$ = cat4($1, $2, $3->toCString(), $4);
            $$ = $3;
	}
        | generalExp DOT RE
        {
//            $$ = cat3($1->toCString(), $2, $3);
            $$ = $1;
        }
        | generalExp DOT IM
        {
//            $$ = cat3($1->toCString(), $2, $3);
            $$ = $1;
        }
	| NOT generalExp
	{
//            $$ = cat2($1, $2->toCString());
            $$ = $2;
	}
	| generalExp OVERLAY generalExp
	{
//            $$ = cat3($1->toCString(), $2, $3->toCString());
            $$ = $3;
	}
	| generalExp IS generalExp
	{
//            $$ = cat3($1->toCString(), $2, $3->toCString());
            $$ = $3;
	}
	| generalExp AND generalExp
	{
//            $$ = cat3($1->toCString(), $2, $3->toCString());
            $$ = $3;
	}
	| generalExp OR generalExp
	{
//            $$ = cat3($1->toCString(), $2, $3->toCString());
            $$ = $3;
	}
	| generalExp XOR generalExp
	{
//            $$ = cat3($1->toCString(), $2, $3->toCString());
            $$ = $3;
	}
	| generalExp PLUS generalExp
	{
//            $$ = cat3($1->toCString(), $2, $3->toCString());
            $$ = $3;
	}
	| generalExp MINUS generalExp
	{
//            $$ = cat3($1->toCString(), $2, $3->toCString());
            $$ = $3;
	}
	| generalExp MULT generalExp
	{
//            $$ = cat3($1->toCString(), $2, $3->toCString());
            $$ = $3;
	}
	| generalExp DIV generalExp
	{
//            $$ = cat3($1->toCString(), $2, $3->toCString());
            $$ = $3;
	}
	| generalExp EQUAL generalExp
	{
//            $$ = cat3($1->toCString(), $2, $3->toCString());
            $$ = $3;
	}
	| generalExp LESS generalExp
	{
//            $$ = cat3($1->toCString(), $2, $3->toCString());
            $$ = $3;
	}
	| generalExp GREATER generalExp
	{
//            $$ = cat3($1->toCString(), $2, $3->toCString());
            $$ = $3;
 	}
	| generalExp LESSEQUAL generalExp
	{
//            $$ = cat3($1->toCString(), $2, $3->toCString());
            $$ = $3;
	}
	| generalExp GREATEREQUAL generalExp
	{
//            $$ = cat3($1->toCString(), $2, $3->toCString());
            $$ = $3;
	}
	| generalExp NOTEQUAL generalExp
	{
//            $$ = cat3($1->toCString(), $2, $3->toCString());
            $$ = $3;
 	}
	| PLUS  generalExp %prec UNARYOP
	{
            $$ = $2;
	}
	| MINUS generalExp %prec UNARYOP
	{
            $$ = $2;
	}
	| LRPAR castType RRPAR generalExp %prec UNARYOP
	{
            $$ = $4;
	}
	| LRPAR generalExp RRPAR
	{
            $$ = $2;
	}
	| generalExp structSelection
	{
            $$ = $1;
	};

castType: TBOOL			{ $$ = new QtString($1); }
	| TCHAR			{ $$ = new QtString($1); }
	| TOCTET		{ $$ = new QtString($1); }
	| TSHORT		{ $$ = new QtString($1); }
	| TUSHORT		{ $$ = new QtString($1); }
	| TLONG			{ $$ = new QtString($1); }
	| TULONG		{ $$ = new QtString($1); }
	| TFLOAT		{ $$ = new QtString($1); }
	| TDOUBLE		{ $$ = new QtString($1); }
	| TUNSIG TSHORT	        { $$ = new QtString($1); }
	| TUNSIG TLONG	        { $$ = new QtString($1); }

//collectionList: collectionList COMMA iteratedCollection
//	{
//    $$ = cat3($1, $2, $3);
//	}
//	| iteratedCollection
//	{
//            $$ = $1;
//	};
//
//iteratedCollection: namedCollection AS collectionIterator
//	{
//            $$ = cat3($1, $2, $3);
//	}
//	| namedCollection collectionIterator
//	{
//            $$ = cat2($1, $2);
//	}
//	| namedCollection
//	{
//            $$ = $1;
//	};

variable: Identifier
	{
            $$ = new QtColumn($1);
	};

//namedCollection: Identifier;

collectionIterator: Identifier;

attributeIdent: Identifier;

//typeName: Identifier;

marrayVariable: Identifier;

condenseVariable: Identifier;

reduceExp: reduceIdent LRPAR generalExp RRPAR
	{
            $$ = new QtString(cat4($1->toCString(), $2, $3->toCString(), $4));
	};

reduceIdent: ALL
	{
            $$ = new QtString($1);
	}
	| SOME
	{
            $$ = new QtString($1);
	}
	| COUNTCELLS
	{
            $$ = new QtString($1);
	}
	| ADDCELLS
	{
            $$ = new QtString($1);
	}
	| AVGCELLS
	{
            $$ = new QtString($1);
	}
	| MINCELLS
	{
            $$ = new QtString($1);
	}
	| MAXCELLS
	{
            $$ = new QtString($1);
	};

intLitExp: IntegerLit                          { $$ = new QtString($1); };

generalLit: scalarLit                          { $$ = $1; }
	| mddLit                               { $$ = $1; }
	| StringLit
	{
            $$ = new QtString($1);
	}
	| oidLit                              { $$ = new QtString($1); };

oidLit: LESS StringLit GREATER
	{
            $$ = cat3($1, $2, $3);
	};

mddLit: LESS mintervalExp dimensionLitList GREATER
	{
            $$ = new QtString(cat4($1, $2->toCString(), $3, $4));
	}
	| MDDPARAM
	{
            $$ = new QtString($1);
	};

dimensionLitList: dimensionLitList SEMICOLON scalarLitList
	{
	}
	| scalarLitList
	{
	  $$ = $1->toCString();
	};

scalarLit: complexLit
	{
	  $$ = $1;
	}
	| atomicLit
	{
	  $$ = $1;
	};

atomicLit: BooleanLit
	{
            $$ = new QtConst($1, "bool");
	}
	| IntegerLit
	{
            $$ = new QtConst($1, "integer");
	}
	| FloatLit
	{
            $$ = new QtConst($1, "float");
	}
        | COMPLEX LRPAR FloatLit COMMA FloatLit RRPAR
        {
            $$ = new QtConst(cat3($3, " + i*", $5), "complex");
	};

complexLit: LCPAR scalarLitList RCPAR
	{
            $$ = new QtString(cat3($1, $2->toCString(), $3));
	}
	| STRCT LCPAR scalarLitList RCPAR
	{
            $$ = new QtString(cat4($1, $2, $3->toCString(), $4));
	};

scalarLitList: scalarLitList COMMA scalarLit
	{
            $$ = $3;
	}
	| scalarLit
	{
            $$ = $1;
	};


trimExp: generalExp mintervalExp
	{
//            $$ = new QtString(cat2($1->toCString(), $2->toCString()));
            $$ = new QtTrimOperation($1, $2);
	};

marray_head:
        MARRAY iv
	{
            $$ = $2;
	};

mddExp: marray_head VALUES generalExp
	{
            $$ = $3;
	}
	| marray_head COMMA ivList VALUES generalExp
	{
            $$ = $5;
	};

ivList: ivList COMMA iv
	{
            $$ = $3;
	}
	| iv
	{
            $$ = $1;
	};

iv: marrayVariable IN_P generalExp
	{
            $$ = new QtString(cat3($1, $2, $3->toCString()));
	};



/******************************** END RASQL *********************************/


/* The following rules link the RaSQL terminal rules to the PostgreSQL terminal lexmes */
Identifier: IDENT   { $$ = $1; }

IntegerLit: ICONST  { $$ = $1; }

FloatLit: FCONST    { $$ = $1; }

StringLit: SCONST   { $$ = $1; }

BooleanLit: BCONST  { $$ = $1;  }


%%




#include "str.c"
