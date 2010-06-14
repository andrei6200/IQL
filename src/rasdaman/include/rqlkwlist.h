/* 
 * File:   rqlkwlist.h
 * Author: andrei
 *
 * Created on June 5, 2010, 1:07 PM
 */

/* there is deliberately not an #ifndef RQLKWLIST_H here */

/*
 * List of keyword (name, token-value, category) entries.
 *
 * !!WARNING!!: This list must be sorted by ASCII name, because binary
 *		 search is used to locate entries.
 */


/* RASQL KEYWORDS */
RASQL_KW("abs", ABS)
//RASQL_KW("add", ADD)
RASQL_KW("add_cell", ADDCELLS)
RASQL_KW("add_cells", ADDCELLS)
//RASQL_KW("a_index", A_INDEX)
//RASQL_KW("aligned", ALIGNED)
RASQL_KW("all_cell", RQLALL)
RASQL_KW("all_cells", RQLALL)
//RASQL_KW("alter", ALTER)
//RASQL_KW("and", AND)
RASQL_KW("arccos", ARCCOS)
RASQL_KW("arcsin", ARCSIN)
RASQL_KW("arctan", ARCTAN)
//RASQL_KW("area", AREA)
//RASQL_KW("as", AS)
//RASQL_KW("assign", ASSIGN)
//RASQL_KW("at", AT)
//RASQL_KW("auto", AUTO)
RASQL_KW("avg_cell", AVGCELLS)
RASQL_KW("avg_cells", AVGCELLS)
//RASQL_KW("bit", BIT)
RASQL_KW("bmp", BMP)
RASQL_KW("bool", TBOOL)
//RASQL_KW("border", BORDER)
//RASQL_KW("by", BY)
RASQL_KW("char", TCHAR)
//RASQL_KW("collection", COLLECTION)
RASQL_KW("complex", COMPLEX)
RASQL_KW("condense", CONDENSE)
RASQL_KW("cos", COS)
RASQL_KW("cosh", COSH)
RASQL_KW("count_cell", COUNTCELLS)
RASQL_KW("count_cells", COUNTCELLS)
//RASQL_KW("create", CREATE)
//RASQL_KW("csv", CSV)
//RASQL_KW("decomp", DECOMP)
//RASQL_KW("delete", DELETE)
RASQL_KW("dem", DEM)
//RASQL_KW("dimension", DIMENSION)
//RASQL_KW("d_index", D_INDEX)
//RASQL_KW("directional", DIRECTIONAL)
RASQL_KW("double", TDOUBLE)
//RASQL_KW("drop", DROP)
RASQL_KW("exp", EXP)
RASQL_KW("extend", EXTEND)
//RASQL_KW("fastscale", FASTSCALE)
RASQL_KW("float", TFLOAT)
//RASQL_KW("from", FROM)
RASQL_KW("hdf", HDF)
RASQL_KW("hi", HI)
RASQL_KW("im", IM)
//RASQL_KW("index", INDEX)
//RASQL_KW("in", IN)
//RASQL_KW("insert", INSERT)
//RASQL_KW("interest", INTEREST)
//RASQL_KW("into", INTO)
RASQL_KW("inv_bmp", INV_BMP)
RASQL_KW("inv_csv", INV_CSV)
RASQL_KW("inv_dem", INV_DEM)
RASQL_KW("inv_hdf", INV_HDF)
RASQL_KW("inv_jpeg", INV_JPEG)
RASQL_KW("inv_png", INV_PNG)
RASQL_KW("inv_tiff", INV_TIFF)
RASQL_KW("inv_tor", INV_TOR)
RASQL_KW("inv_vff", INV_VFF)
//RASQL_KW("is", IS)
//RASQL_KW("it_index", IT_INDEX)
RASQL_KW("jpeg", JPEG)
//RASQL_KW("list", LIST)
RASQL_KW("ln", LN)
RASQL_KW("log", RQLLOG)
RASQL_KW("lo", LO)
RASQL_KW("long", TLONG)
RASQL_KW("marray", MARRAY)
RASQL_KW("max_cell", MAXCELLS)
RASQL_KW("max_cells", MAXCELLS)
//RASQL_KW("members", MEMBERS)
RASQL_KW("min_cell", MINCELLS)
RASQL_KW("min_cells", MINCELLS)
//RASQL_KW("not", NOT)
RASQL_KW("octet", TOCTET)
//RASQL_KW("of", OF)
RASQL_KW("oid", OID)
//RASQL_KW("or", OR)
//RASQL_KW("overlay", OVERLAY)
//RASQL_KW("over", OVER)
RASQL_KW("png", PNG)
//RASQL_KW("project", PROJECT)
//RASQL_KW("rc_index", RC_INDEX)
//RASQL_KW("rd_index", RD_INDEX)
//RASQL_KW("regular", REGULAR)
RASQL_KW("re", RE)
//RASQL_KW("restrict", RESTRICT)
//RASQL_KW("rpt_index", RPT_INDEX)
//RASQL_KW("rrpt_index", RRPT_INDEX)
RASQL_KW("scale", SCALE)
RASQL_KW("sdom", SDOM)
//RASQL_KW("select", SELECT)
//RASQL_KW("set", SET)
RASQL_KW("shift", SHIFT)
RASQL_KW("short", TSHORT)
RASQL_KW("sinh", SINH)
RASQL_KW("sin", SIN)
//RASQL_KW("size", SIZE)
//RASQL_KW("some_cell", RQLSOME)
//RASQL_KW("some_cells", RQLSOME)
RASQL_KW("sqrt", SQRT)
//RASQL_KW("statistic", STATISTIC)
RASQL_KW("struct", STRCT)
//RASQL_KW("subtiling", SUBTILING)
RASQL_KW("tanh", TANH)
RASQL_KW("tan", TAN)
//RASQL_KW("tc_index", TC_INDEX)
//RASQL_KW("threshold", THRESHOLD)
RASQL_KW("tiff", TIFF)
//RASQL_KW("tile", TILE)
//RASQL_KW("tiling", TILING)
RASQL_KW("tor", TOR)
//RASQL_KW("to", TO)
RASQL_KW("ulong", TULONG)
RASQL_KW("unsigned", TUNSIG)
//RASQL_KW("update", UPDATE)
RASQL_KW("ushort", TUSHORT)
//RASQL_KW("using", USING)
//RASQL_KW("values", VALUES)
RASQL_KW("vff", VFF)
//RASQL_KW("where", WHERE)
//RASQL_KW("with", WITH)
RASQL_KW("xor", XOR)


        
/* The below operators are going to be handled in the lexer file directly. */
//","                                      { SETTOKEN( COMMA, commandToken, COMMA ) }
//"."                                      { SETTOKEN( DOT, commandToken, DOT ) }
//
//"+"                                      { SETTOKEN( PLUS, commandToken, PLUS ) }
//"-"                                      { SETTOKEN( MINUS, commandToken, MINUS ) }
//"*"                                      { SETTOKEN( MULT, commandToken, MULT ) }
//"/"                                      { SETTOKEN( DIV, commandToken, DIV ) }
//"="                                      { SETTOKEN( EQUAL, commandToken, EQUAL ) }
//"<"                                      { SETTOKEN( LESS, commandToken, LESS ) }
//">"                                      { SETTOKEN( GREATER, commandToken, GREATER ) }
//"<="                                     { SETTOKEN( LESSEQUAL, commandToken, LESSEQUAL ) }
//">="                                     { SETTOKEN( GREATEREQUAL, commandToken, GREATEREQUAL ) }
//"<>"                                     { SETTOKEN( NOTEQUAL, commandToken, NOTEQUAL ) }
//"!="                                     { SETTOKEN( NOTEQUAL, commandToken, NOTEQUAL ) }
//":"                                      { SETTOKEN( COLON, commandToken, COLON ) }
//";"                                      { SETTOKEN( SEMICOLON, commandToken, SEMICOLON ) }
//"["                                      { SETTOKEN( LEPAR, commandToken, LEPAR ) }
//"]"                                      { SETTOKEN( REPAR, commandToken, REPAR ) }
//"("                                      { SETTOKEN( LRPAR, commandToken, LRPAR ) }
//")"                                      { SETTOKEN( RRPAR, commandToken, RRPAR ) }
//"{"                                      { SETTOKEN( LCPAR, commandToken, LCPAR ) }
//"}"                                      { SETTOKEN( RCPAR, commandToken, RCPAR ) }