/*
** 2000-05-29
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** Driver template for the LEMON parser generator.
**
** The "lemon" program processes an LALR(1) input grammar file, then uses
** this template to construct a parser.  The "lemon" program inserts text
** at each "%%" line.  Also, any "P-a-r-s-e" identifer prefix (without the
** interstitial "-" characters) contained in this template is changed into
** the value of the %name directive from the grammar.  Otherwise, the content
** of this template is copied straight through into the generate parser
** source file.
**
** The following is the concatenation of all %include directives from the
** input grammar file:
*/
#include <stdio.h>
/************ Begin %include sections from the grammar ************************/
#line 7 "./grammar.y"

	#include <stdlib.h>
	#include <stdio.h>
	#include <assert.h>
	#include "token.h"	
	#include "grammar.h"
	#include "ast.h"
	#include "parse.h"
	#include "../value.h"

	void yyerror(char *s);
#line 40 "./grammar.c"
/**************** End of %include directives **********************************/
/* These constants specify the various numeric values for terminal symbols
** in a format understandable to "makeheaders".  This section is blank unless
** "lemon" is run with the "-m" command-line option.
***************** Begin makeheaders token definitions *************************/
/**************** End makeheaders token definitions ***************************/

/* The next sections is a series of control #defines.
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used to store the integer codes
**                       that represent terminal and non-terminal symbols.
**                       "unsigned char" is used if there are fewer than
**                       256 symbols.  Larger types otherwise.
**    YYNOCODE           is a number of type YYCODETYPE that is not used for
**                       any terminal or nonterminal symbol.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       (also known as: "terminal symbols") have fall-back
**                       values which should be used if the original symbol
**                       would not parse.  This permits keywords to sometimes
**                       be used as identifiers, for example.
**    YYACTIONTYPE       is the data type used for "action codes" - numbers
**                       that indicate what to do in response to the next
**                       token.
**    ParseTOKENTYPE     is the data type used for minor type for terminal
**                       symbols.  Background: A "minor type" is a semantic
**                       value associated with a terminal or non-terminal
**                       symbols.  For example, for an "ID" terminal symbol,
**                       the minor type might be the name of the identifier.
**                       Each non-terminal can have a different minor type.
**                       Terminal symbols all have the same minor type, though.
**                       This macros defines the minor type for terminal 
**                       symbols.
**    YYMINORTYPE        is the data type used for all minor types.
**                       This is typically a union of many types, one of
**                       which is ParseTOKENTYPE.  The entry in the union
**                       for terminal symbols is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    ParseARG_SDECL     A static variable declaration for the %extra_argument
**    ParseARG_PDECL     A parameter declaration for the %extra_argument
**    ParseARG_STORE     Code to store %extra_argument into yypParser
**    ParseARG_FETCH     Code to extract %extra_argument from yypParser
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YY_MAX_SHIFT       Maximum value for shift actions
**    YY_MIN_SHIFTREDUCE Minimum value for shift-reduce actions
**    YY_MAX_SHIFTREDUCE Maximum value for shift-reduce actions
**    YY_MIN_REDUCE      Maximum value for reduce actions
**    YY_ERROR_ACTION    The yy_action[] code for syntax error
**    YY_ACCEPT_ACTION   The yy_action[] code for accept
**    YY_NO_ACTION       The yy_action[] code for no-op
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/************* Begin control #defines *****************************************/
#define YYCODETYPE unsigned char
#define YYNOCODE 66
#define YYACTIONTYPE unsigned short int
#define ParseTOKENTYPE Token
typedef union {
  int yyinit;
  ParseTOKENTYPE yy0;
  AST_DeleteNode * yy5;
  AST_LimitNode* yy7;
  AST_FilterNode* yy36;
  AST_ReturnElementNode* yy54;
  Vector* yy66;
  AST_ReturnNode* yy68;
  AST_NodeEntity* yy69;
  SIValue yy70;
  AST_WhereNode* yy71;
  int yy72;
  AST_Variable* yy73;
  AST_ColumnNode* yy74;
  AST_MatchNode* yy75;
  AST_QueryExpressionNode* yy88;
  AST_CreateNode * yy89;
  AST_OrderNode* yy108;
  AST_LinkEntity* yy125;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define ParseARG_SDECL  parseCtx *ctx ;
#define ParseARG_PDECL , parseCtx *ctx 
#define ParseARG_FETCH  parseCtx *ctx  = yypParser->ctx 
#define ParseARG_STORE yypParser->ctx  = ctx 
#define YYNSTATE             81
#define YYNRULE              68
#define YY_MAX_SHIFT         80
#define YY_MIN_SHIFTREDUCE   127
#define YY_MAX_SHIFTREDUCE   194
#define YY_MIN_REDUCE        195
#define YY_MAX_REDUCE        262
#define YY_ERROR_ACTION      263
#define YY_ACCEPT_ACTION     264
#define YY_NO_ACTION         265
/************* End control #defines *******************************************/

/* Define the yytestcase() macro to be a no-op if is not already defined
** otherwise.
**
** Applications can choose to define yytestcase() in the %include section
** to a macro that can assist in verifying code coverage.  For production
** code the yytestcase() macro should be turned off.  But it is useful
** for testing.
*/
#ifndef yytestcase
# define yytestcase(X)
#endif


/* Next are the tables used to determine what action to take based on the
** current state and lookahead token.  These tables are used to implement
** functions that take a state number and lookahead value and return an
** action integer.  
**
** Suppose the action integer is N.  Then the action is determined as
** follows
**
**   0 <= N <= YY_MAX_SHIFT             Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   N between YY_MIN_SHIFTREDUCE       Shift to an arbitrary state then
**     and YY_MAX_SHIFTREDUCE           reduce by rule N-YY_MIN_SHIFTREDUCE.
**
**   N between YY_MIN_REDUCE            Reduce by rule N-YY_MIN_REDUCE
**     and YY_MAX_REDUCE
**
**   N == YY_ERROR_ACTION               A syntax error has occurred.
**
**   N == YY_ACCEPT_ACTION              The parser accepts its input.
**
**   N == YY_NO_ACTION                  No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as either:
**
**    (A)   N = yy_action[ yy_shift_ofst[S] + X ]
**    (B)   N = yy_default[S]
**
** The (A) formula is preferred.  The B formula is used instead if:
**    (1)  The yy_shift_ofst[S]+X value is out of range, or
**    (2)  yy_lookahead[yy_shift_ofst[S]+X] is not equal to X, or
**    (3)  yy_shift_ofst[S] equal YY_SHIFT_USE_DFLT.
** (Implementation note: YY_SHIFT_USE_DFLT is chosen so that
** YY_SHIFT_USE_DFLT+X will be out of range for all possible lookaheads X.
** Hence only tests (1) and (2) need to be evaluated.)
**
** The formulas above are for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array and YY_REDUCE_USE_DFLT is used in place of
** YY_SHIFT_USE_DFLT.
**
** The following are the tables generated in this section:
**
**  yy_action[]        A single table containing all actions.
**  yy_lookahead[]     A table containing the lookahead for each entry in
**                     yy_action.  Used to detect hash collisions.
**  yy_shift_ofst[]    For each state, the offset into yy_action for
**                     shifting terminals.
**  yy_reduce_ofst[]   For each state, the offset into yy_action for
**                     shifting non-terminals after a reduce.
**  yy_default[]       Default action for each state.
**
*********** Begin parsing tables **********************************************/
#define YY_ACTTAB_COUNT (165)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */   163,  164,  167,  165,  166,   80,  264,   37,   41,  131,
 /*    10 */    45,   42,   19,  135,  170,   75,  177,   74,  180,   67,
 /*    20 */    19,  135,  168,   14,  169,  171,  172,  173,  169,  171,
 /*    30 */   172,  173,   68,  177,   74,  180,  176,   74,  180,  191,
 /*    40 */     9,   39,  190,   20,   21,   52,   60,   15,   17,    3,
 /*    50 */     4,   41,   30,   30,    4,   36,   23,   22,  135,  130,
 /*    60 */   191,  160,   77,  189,   11,  187,  188,   47,   16,   27,
 /*    70 */     2,   24,   54,   66,   15,   17,  159,   33,   62,   30,
 /*    80 */    30,   56,   58,   43,   38,   13,   32,  140,   76,   55,
 /*    90 */   161,   40,   50,   61,   49,   30,   51,   53,   63,   72,
 /*   100 */    79,   57,   59,  155,  136,   65,   64,  128,    1,   78,
 /*   110 */    34,   35,   18,    7,    8,   17,   44,   71,  158,   10,
 /*   120 */    46,  141,   48,  150,  147,  151,   25,   26,   29,   69,
 /*   130 */   149,  148,  145,  146,  143,   28,  144,  142,   12,   31,
 /*   140 */   184,    5,  153,  195,   70,  179,  182,  197,  197,   76,
 /*   150 */   197,  197,  197,  197,  197,  197,  197,  197,   73,  197,
 /*   160 */   197,  194,  197,  197,    6,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     3,    4,    5,    6,    7,   39,   40,   41,   12,   43,
 /*    10 */    12,   48,   49,   50,   12,   59,   60,   61,   62,   48,
 /*    20 */    49,   50,   25,   13,   26,   27,   28,   29,   26,   27,
 /*    30 */    28,   29,   59,   60,   61,   62,   60,   61,   62,   61,
 /*    40 */    10,   63,   64,   12,   12,   14,   14,    1,    2,    8,
 /*    50 */     9,   12,   21,   21,    9,   43,   11,   49,   50,   47,
 /*    60 */    61,   15,   12,   64,   58,   35,   36,   12,   13,   16,
 /*    70 */    31,   18,   14,   14,    1,    2,   56,   13,   56,   21,
 /*    80 */    21,   53,   53,   57,   57,   19,   51,   12,   24,   53,
 /*    90 */    57,   57,   54,   53,   52,   21,   53,   53,   12,   12,
 /*   100 */    37,   54,   53,   55,   50,   53,   55,   46,   30,   33,
 /*   110 */    45,   44,   23,   42,   10,    2,   24,   61,   12,   12,
 /*   120 */    24,   12,   10,   20,   16,   20,   12,   12,   10,   32,
 /*   130 */    20,   20,   15,   17,   15,   12,   15,   15,   14,   12,
 /*   140 */    12,   10,   22,    0,   15,   12,   12,   65,   65,   24,
 /*   150 */    65,   65,   65,   65,   65,   65,   65,   65,   32,   65,
 /*   160 */    65,   26,   65,   65,   34,
};
#define YY_SHIFT_USE_DFLT (165)
#define YY_SHIFT_COUNT    (80)
#define YY_SHIFT_MIN      (-4)
#define YY_SHIFT_MAX      (143)
static const short yy_shift_ofst[] = {
 /*     0 */    41,   39,   -4,   10,   10,   -4,   50,   45,   10,   50,
 /*    10 */    -3,   -2,    2,   31,   32,   55,   55,   55,   55,   53,
 /*    20 */    58,   59,   53,   75,   66,   74,   74,   66,   74,   86,
 /*    30 */    86,   74,   10,   87,   63,   76,   78,   89,   46,   30,
 /*    40 */    73,   64,  104,  113,  106,   92,  107,   96,  109,  112,
 /*    50 */   108,  103,  114,  105,  115,  110,  111,  116,  117,  119,
 /*    60 */   123,  121,  118,  124,  120,  122,  127,  104,  131,  128,
 /*    70 */    97,  129,  125,  133,  126,  131,  134,  125,  130,  135,
 /*    80 */   143,
};
#define YY_REDUCE_USE_DFLT (-45)
#define YY_REDUCE_COUNT (37)
#define YY_REDUCE_MIN   (-44)
#define YY_REDUCE_MAX   (71)
static const signed char yy_reduce_ofst[] = {
 /*     0 */   -34,  -44,  -27,  -37,  -29,  -24,  -22,   12,    8,   -1,
 /*    10 */     6,   20,   22,   28,   29,   26,   27,   33,   34,   35,
 /*    20 */    36,   40,   35,   42,   38,   43,   44,   47,   49,   48,
 /*    30 */    51,   52,   54,   56,   61,   65,   67,   71,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   201,  263,  263,  263,  263,  263,  263,  201,  263,  263,
 /*    10 */   263,  263,  263,  220,  220,  263,  263,  263,  263,  205,
 /*    20 */   220,  220,  206,  263,  263,  220,  220,  263,  220,  263,
 /*    30 */   263,  220,  263,  263,  261,  253,  197,  224,  263,  254,
 /*    40 */   225,  249,  200,  230,  263,  238,  263,  263,  263,  207,
 /*    50 */   263,  263,  263,  263,  263,  263,  263,  263,  263,  263,
 /*    60 */   263,  263,  222,  263,  263,  263,  263,  202,  243,  263,
 /*    70 */   251,  263,  263,  263,  246,  242,  263,  260,  263,  263,
 /*    80 */   263,
};
/********** End of lemon-generated parsing tables *****************************/

/* The next table maps tokens (terminal symbols) into fallback tokens.  
** If a construct like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammar, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
**
** This feature can be used, for example, to cause some keywords in a language
** to revert to identifiers if they keyword does not apply in the context where
** it appears.
*/
#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
};
#endif /* YYFALLBACK */

/* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
**
** After the "shift" half of a SHIFTREDUCE action, the stateno field
** actually contains the reduce action for the second half of the
** SHIFTREDUCE.
*/
struct yyStackEntry {
  YYACTIONTYPE stateno;  /* The state-number, or reduce action in SHIFTREDUCE */
  YYCODETYPE major;      /* The major token value.  This is the code
                         ** number for the token at this stack level */
  YYMINORTYPE minor;     /* The user-supplied minor token value.  This
                         ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  yyStackEntry *yytos;          /* Pointer to top element of the stack */
#ifdef YYTRACKMAXSTACKDEPTH
  int yyhwm;                    /* High-water mark of the stack */
#endif
#ifndef YYNOERRORRECOVERY
  int yyerrcnt;                 /* Shifts left before out of the error */
#endif
  ParseARG_SDECL                /* A place to hold %extra_argument */
#if YYSTACKDEPTH<=0
  int yystksz;                  /* Current side of the stack */
  yyStackEntry *yystack;        /* The parser's stack */
  yyStackEntry yystk0;          /* First stack entry */
#else
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
#endif
};
typedef struct yyParser yyParser;

#ifndef NDEBUG
#include <stdio.h>
static FILE *yyTraceFILE = 0;
static char *yyTracePrompt = 0;
#endif /* NDEBUG */

#ifndef NDEBUG
/* 
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL 
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
void ParseTrace(FILE *TraceFILE, char *zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *const yyTokenName[] = { 
  "$",             "OR",            "AND",           "EQ",          
  "GT",            "GE",            "LT",            "LE",          
  "MATCH",         "CREATE",        "COMMA",         "DELETE",      
  "STRING",        "LEFT_PARENTHESIS",  "COLON",         "RIGHT_PARENTHESIS",
  "DASH",          "RIGHT_ARROW",   "LEFT_ARROW",    "LEFT_BRACKET",
  "RIGHT_BRACKET",  "LEFT_CURLY_BRACKET",  "RIGHT_CURLY_BRACKET",  "WHERE",       
  "DOT",           "NE",            "INTEGER",       "FLOAT",       
  "TRUE",          "FALSE",         "RETURN",        "DISTINCT",    
  "AS",            "ORDER",         "BY",            "ASC",         
  "DESC",          "LIMIT",         "error",         "expr",        
  "query",         "matchClause",   "whereClause",   "createClause",
  "returnClause",  "orderClause",   "limitClause",   "deleteClause",
  "chains",        "chain",         "node",          "link",        
  "deleteExpression",  "properties",    "edge",          "mapLiteral",  
  "value",         "cond",          "op",            "returnElements",
  "returnElement",  "variable",      "aggFunc",       "columnNameList",
  "columnName",  
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "query ::= expr",
 /*   1 */ "expr ::= matchClause whereClause createClause returnClause orderClause limitClause",
 /*   2 */ "expr ::= matchClause whereClause createClause",
 /*   3 */ "expr ::= matchClause whereClause deleteClause",
 /*   4 */ "expr ::= createClause",
 /*   5 */ "matchClause ::= MATCH chains",
 /*   6 */ "createClause ::=",
 /*   7 */ "createClause ::= CREATE chains",
 /*   8 */ "chain ::= node",
 /*   9 */ "chain ::= chain link node",
 /*  10 */ "chains ::= chain",
 /*  11 */ "chains ::= chains COMMA chain",
 /*  12 */ "deleteClause ::= DELETE deleteExpression",
 /*  13 */ "deleteExpression ::= STRING",
 /*  14 */ "deleteExpression ::= deleteExpression COMMA STRING",
 /*  15 */ "node ::= LEFT_PARENTHESIS STRING COLON STRING properties RIGHT_PARENTHESIS",
 /*  16 */ "node ::= LEFT_PARENTHESIS COLON STRING properties RIGHT_PARENTHESIS",
 /*  17 */ "node ::= LEFT_PARENTHESIS STRING properties RIGHT_PARENTHESIS",
 /*  18 */ "node ::= LEFT_PARENTHESIS properties RIGHT_PARENTHESIS",
 /*  19 */ "link ::= DASH edge RIGHT_ARROW",
 /*  20 */ "link ::= LEFT_ARROW edge DASH",
 /*  21 */ "edge ::= LEFT_BRACKET properties RIGHT_BRACKET",
 /*  22 */ "edge ::= LEFT_BRACKET STRING properties RIGHT_BRACKET",
 /*  23 */ "edge ::= LEFT_BRACKET COLON STRING properties RIGHT_BRACKET",
 /*  24 */ "edge ::= LEFT_BRACKET STRING COLON STRING properties RIGHT_BRACKET",
 /*  25 */ "properties ::=",
 /*  26 */ "properties ::= LEFT_CURLY_BRACKET mapLiteral RIGHT_CURLY_BRACKET",
 /*  27 */ "mapLiteral ::= STRING COLON value",
 /*  28 */ "mapLiteral ::= STRING COLON value COMMA mapLiteral",
 /*  29 */ "whereClause ::=",
 /*  30 */ "whereClause ::= WHERE cond",
 /*  31 */ "cond ::= STRING DOT STRING op STRING DOT STRING",
 /*  32 */ "cond ::= STRING DOT STRING op value",
 /*  33 */ "cond ::= LEFT_PARENTHESIS cond RIGHT_PARENTHESIS",
 /*  34 */ "cond ::= cond AND cond",
 /*  35 */ "cond ::= cond OR cond",
 /*  36 */ "op ::= EQ",
 /*  37 */ "op ::= GT",
 /*  38 */ "op ::= LT",
 /*  39 */ "op ::= LE",
 /*  40 */ "op ::= GE",
 /*  41 */ "op ::= NE",
 /*  42 */ "value ::= INTEGER",
 /*  43 */ "value ::= STRING",
 /*  44 */ "value ::= FLOAT",
 /*  45 */ "value ::= TRUE",
 /*  46 */ "value ::= FALSE",
 /*  47 */ "returnClause ::= RETURN returnElements",
 /*  48 */ "returnClause ::= RETURN DISTINCT returnElements",
 /*  49 */ "returnElements ::= returnElements COMMA returnElement",
 /*  50 */ "returnElements ::= returnElement",
 /*  51 */ "returnElement ::= variable",
 /*  52 */ "returnElement ::= variable AS STRING",
 /*  53 */ "returnElement ::= aggFunc",
 /*  54 */ "returnElement ::= STRING",
 /*  55 */ "variable ::= STRING DOT STRING",
 /*  56 */ "aggFunc ::= STRING LEFT_PARENTHESIS variable RIGHT_PARENTHESIS",
 /*  57 */ "aggFunc ::= STRING LEFT_PARENTHESIS variable RIGHT_PARENTHESIS AS STRING",
 /*  58 */ "orderClause ::=",
 /*  59 */ "orderClause ::= ORDER BY columnNameList",
 /*  60 */ "orderClause ::= ORDER BY columnNameList ASC",
 /*  61 */ "orderClause ::= ORDER BY columnNameList DESC",
 /*  62 */ "columnNameList ::= columnNameList COMMA columnName",
 /*  63 */ "columnNameList ::= columnName",
 /*  64 */ "columnName ::= variable",
 /*  65 */ "columnName ::= STRING",
 /*  66 */ "limitClause ::=",
 /*  67 */ "limitClause ::= LIMIT INTEGER",
};
#endif /* NDEBUG */


#if YYSTACKDEPTH<=0
/*
** Try to increase the size of the parser stack.  Return the number
** of errors.  Return 0 on success.
*/
static int yyGrowStack(yyParser *p){
  int newSize;
  int idx;
  yyStackEntry *pNew;

  newSize = p->yystksz*2 + 100;
  idx = p->yytos ? (int)(p->yytos - p->yystack) : 0;
  if( p->yystack==&p->yystk0 ){
    pNew = malloc(newSize*sizeof(pNew[0]));
    if( pNew ) pNew[0] = p->yystk0;
  }else{
    pNew = realloc(p->yystack, newSize*sizeof(pNew[0]));
  }
  if( pNew ){
    p->yystack = pNew;
    p->yytos = &p->yystack[idx];
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sStack grows from %d to %d entries.\n",
              yyTracePrompt, p->yystksz, newSize);
    }
#endif
    p->yystksz = newSize;
  }
  return pNew==0; 
}
#endif

/* Datatype of the argument to the memory allocated passed as the
** second argument to ParseAlloc() below.  This can be changed by
** putting an appropriate #define in the %include section of the input
** grammar.
*/
#ifndef YYMALLOCARGTYPE
# define YYMALLOCARGTYPE size_t
#endif

/* 
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to Parse and ParseFree.
*/
void *ParseAlloc(void *(*mallocProc)(YYMALLOCARGTYPE)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (YYMALLOCARGTYPE)sizeof(yyParser) );
  if( pParser ){
#ifdef YYTRACKMAXSTACKDEPTH
    pParser->yyhwm = 0;
#endif
#if YYSTACKDEPTH<=0
    pParser->yytos = NULL;
    pParser->yystack = NULL;
    pParser->yystksz = 0;
    if( yyGrowStack(pParser) ){
      pParser->yystack = &pParser->yystk0;
      pParser->yystksz = 1;
    }
#endif
#ifndef YYNOERRORRECOVERY
    pParser->yyerrcnt = -1;
#endif
    pParser->yytos = pParser->yystack;
    pParser->yystack[0].stateno = 0;
    pParser->yystack[0].major = 0;
  }
  return pParser;
}

/* The following function deletes the "minor type" or semantic value
** associated with a symbol.  The symbol can be either a terminal
** or nonterminal. "yymajor" is the symbol code, and "yypminor" is
** a pointer to the value to be deleted.  The code used to do the 
** deletions is derived from the %destructor and/or %token_destructor
** directives of the input grammar.
*/
static void yy_destructor(
  yyParser *yypParser,    /* The parser */
  YYCODETYPE yymajor,     /* Type code for object to destroy */
  YYMINORTYPE *yypminor   /* The object to be destroyed */
){
  ParseARG_FETCH;
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are *not* used
    ** inside the C code.
    */
/********* Begin destructor definitions ***************************************/
    case 57: /* cond */
{
#line 222 "./grammar.y"
 Free_AST_FilterNode((yypminor->yy36)); 
#line 595 "./grammar.c"
}
      break;
/********* End destructor definitions *****************************************/
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
*/
static void yy_pop_parser_stack(yyParser *pParser){
  yyStackEntry *yytos;
  assert( pParser->yytos!=0 );
  assert( pParser->yytos > pParser->yystack );
  yytos = pParser->yytos--;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yy_destructor(pParser, yytos->major, &yytos->minor);
}

/* 
** Deallocate and destroy a parser.  Destructors are called for
** all stack elements before shutting the parser down.
**
** If the YYPARSEFREENEVERNULL macro exists (for example because it
** is defined in a %include section of the input grammar) then it is
** assumed that the input pointer is never NULL.
*/
void ParseFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
  yyParser *pParser = (yyParser*)p;
#ifndef YYPARSEFREENEVERNULL
  if( pParser==0 ) return;
#endif
  while( pParser->yytos>pParser->yystack ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  if( pParser->yystack!=&pParser->yystk0 ) free(pParser->yystack);
#endif
  (*freeProc)((void*)pParser);
}

/*
** Return the peak depth of the stack for a parser.
*/
#ifdef YYTRACKMAXSTACKDEPTH
int ParseStackPeak(void *p){
  yyParser *pParser = (yyParser*)p;
  return pParser->yyhwm;
}
#endif

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
*/
static unsigned int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
  int stateno = pParser->yytos->stateno;
 
  if( stateno>=YY_MIN_REDUCE ) return stateno;
  assert( stateno <= YY_SHIFT_COUNT );
  do{
    i = yy_shift_ofst[stateno];
    assert( iLookAhead!=YYNOCODE );
    i += iLookAhead;
    if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
#ifdef YYFALLBACK
      YYCODETYPE iFallback;            /* Fallback token */
      if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
             && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
             yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
        }
#endif
        assert( yyFallback[iFallback]==0 ); /* Fallback loop must terminate */
        iLookAhead = iFallback;
        continue;
      }
#endif
#ifdef YYWILDCARD
      {
        int j = i - iLookAhead + YYWILDCARD;
        if( 
#if YY_SHIFT_MIN+YYWILDCARD<0
          j>=0 &&
#endif
#if YY_SHIFT_MAX+YYWILDCARD>=YY_ACTTAB_COUNT
          j<YY_ACTTAB_COUNT &&
#endif
          yy_lookahead[j]==YYWILDCARD && iLookAhead>0
        ){
#ifndef NDEBUG
          if( yyTraceFILE ){
            fprintf(yyTraceFILE, "%sWILDCARD %s => %s\n",
               yyTracePrompt, yyTokenName[iLookAhead],
               yyTokenName[YYWILDCARD]);
          }
#endif /* NDEBUG */
          return yy_action[j];
        }
      }
#endif /* YYWILDCARD */
      return yy_default[stateno];
    }else{
      return yy_action[i];
    }
  }while(1);
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
*/
static int yy_find_reduce_action(
  int stateno,              /* Current state number */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
#ifdef YYERRORSYMBOL
  if( stateno>YY_REDUCE_COUNT ){
    return yy_default[stateno];
  }
#else
  assert( stateno<=YY_REDUCE_COUNT );
#endif
  i = yy_reduce_ofst[stateno];
  assert( i!=YY_REDUCE_USE_DFLT );
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
#ifdef YYERRORSYMBOL
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }
#else
  assert( i>=0 && i<YY_ACTTAB_COUNT );
  assert( yy_lookahead[i]==iLookAhead );
#endif
  return yy_action[i];
}

/*
** The following routine is called if the stack overflows.
*/
static void yyStackOverflow(yyParser *yypParser){
   ParseARG_FETCH;
   yypParser->yytos--;
#ifndef NDEBUG
   if( yyTraceFILE ){
     fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
   }
#endif
   while( yypParser->yytos>yypParser->yystack ) yy_pop_parser_stack(yypParser);
   /* Here code is inserted which will execute if the parser
   ** stack every overflows */
/******** Begin %stack_overflow code ******************************************/
/******** End %stack_overflow code ********************************************/
   ParseARG_STORE; /* Suppress warning about unused %extra_argument var */
}

/*
** Print tracing information for a SHIFT action
*/
#ifndef NDEBUG
static void yyTraceShift(yyParser *yypParser, int yyNewState){
  if( yyTraceFILE ){
    if( yyNewState<YYNSTATE ){
      fprintf(yyTraceFILE,"%sShift '%s', go to state %d\n",
         yyTracePrompt,yyTokenName[yypParser->yytos->major],
         yyNewState);
    }else{
      fprintf(yyTraceFILE,"%sShift '%s'\n",
         yyTracePrompt,yyTokenName[yypParser->yytos->major]);
    }
  }
}
#else
# define yyTraceShift(X,Y)
#endif

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  ParseTOKENTYPE yyMinor        /* The minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yytos++;
#ifdef YYTRACKMAXSTACKDEPTH
  if( (int)(yypParser->yytos - yypParser->yystack)>yypParser->yyhwm ){
    yypParser->yyhwm++;
    assert( yypParser->yyhwm == (int)(yypParser->yytos - yypParser->yystack) );
  }
#endif
#if YYSTACKDEPTH>0 
  if( yypParser->yytos>=&yypParser->yystack[YYSTACKDEPTH] ){
    yyStackOverflow(yypParser);
    return;
  }
#else
  if( yypParser->yytos>=&yypParser->yystack[yypParser->yystksz] ){
    if( yyGrowStack(yypParser) ){
      yyStackOverflow(yypParser);
      return;
    }
  }
#endif
  if( yyNewState > YY_MAX_SHIFT ){
    yyNewState += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
  }
  yytos = yypParser->yytos;
  yytos->stateno = (YYACTIONTYPE)yyNewState;
  yytos->major = (YYCODETYPE)yyMajor;
  yytos->minor.yy0 = yyMinor;
  yyTraceShift(yypParser, yyNewState);
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;         /* Symbol on the left-hand side of the rule */
  unsigned char nrhs;     /* Number of right-hand side symbols in the rule */
} yyRuleInfo[] = {
  { 40, 1 },
  { 39, 6 },
  { 39, 3 },
  { 39, 3 },
  { 39, 1 },
  { 41, 2 },
  { 43, 0 },
  { 43, 2 },
  { 49, 1 },
  { 49, 3 },
  { 48, 1 },
  { 48, 3 },
  { 47, 2 },
  { 52, 1 },
  { 52, 3 },
  { 50, 6 },
  { 50, 5 },
  { 50, 4 },
  { 50, 3 },
  { 51, 3 },
  { 51, 3 },
  { 54, 3 },
  { 54, 4 },
  { 54, 5 },
  { 54, 6 },
  { 53, 0 },
  { 53, 3 },
  { 55, 3 },
  { 55, 5 },
  { 42, 0 },
  { 42, 2 },
  { 57, 7 },
  { 57, 5 },
  { 57, 3 },
  { 57, 3 },
  { 57, 3 },
  { 58, 1 },
  { 58, 1 },
  { 58, 1 },
  { 58, 1 },
  { 58, 1 },
  { 58, 1 },
  { 56, 1 },
  { 56, 1 },
  { 56, 1 },
  { 56, 1 },
  { 56, 1 },
  { 44, 2 },
  { 44, 3 },
  { 59, 3 },
  { 59, 1 },
  { 60, 1 },
  { 60, 3 },
  { 60, 1 },
  { 60, 1 },
  { 61, 3 },
  { 62, 4 },
  { 62, 6 },
  { 45, 0 },
  { 45, 3 },
  { 45, 4 },
  { 45, 4 },
  { 63, 3 },
  { 63, 1 },
  { 64, 1 },
  { 64, 1 },
  { 46, 0 },
  { 46, 2 },
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  unsigned int yyruleno        /* Number of the rule by which to reduce */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  ParseARG_FETCH;
  yymsp = yypParser->yytos;
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno<(int)(sizeof(yyRuleName)/sizeof(yyRuleName[0])) ){
    yysize = yyRuleInfo[yyruleno].nrhs;
    fprintf(yyTraceFILE, "%sReduce [%s], go to state %d.\n", yyTracePrompt,
      yyRuleName[yyruleno], yymsp[-yysize].stateno);
  }
#endif /* NDEBUG */

  /* Check that the stack is large enough to grow by a single entry
  ** if the RHS of the rule is empty.  This ensures that there is room
  ** enough on the stack to push the LHS value */
  if( yyRuleInfo[yyruleno].nrhs==0 ){
#ifdef YYTRACKMAXSTACKDEPTH
    if( (int)(yypParser->yytos - yypParser->yystack)>yypParser->yyhwm ){
      yypParser->yyhwm++;
      assert( yypParser->yyhwm == (int)(yypParser->yytos - yypParser->yystack));
    }
#endif
#if YYSTACKDEPTH>0 
    if( yypParser->yytos>=&yypParser->yystack[YYSTACKDEPTH-1] ){
      yyStackOverflow(yypParser);
      return;
    }
#else
    if( yypParser->yytos>=&yypParser->yystack[yypParser->yystksz-1] ){
      if( yyGrowStack(yypParser) ){
        yyStackOverflow(yypParser);
        return;
      }
      yymsp = yypParser->yytos;
    }
#endif
  }

  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  #line <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  #line <lineno> <thisfile>
  **     break;
  */
/********** Begin reduce actions **********************************************/
        YYMINORTYPE yylhsminor;
      case 0: /* query ::= expr */
#line 33 "./grammar.y"
{ ctx->root = yymsp[0].minor.yy88; }
#line 971 "./grammar.c"
        break;
      case 1: /* expr ::= matchClause whereClause createClause returnClause orderClause limitClause */
#line 35 "./grammar.y"
{
	yylhsminor.yy88 = New_AST_QueryExpressionNode(yymsp[-5].minor.yy75, yymsp[-4].minor.yy71, yymsp[-3].minor.yy89, NULL, yymsp[-2].minor.yy68, yymsp[-1].minor.yy108, yymsp[0].minor.yy7);
}
#line 978 "./grammar.c"
  yymsp[-5].minor.yy88 = yylhsminor.yy88;
        break;
      case 2: /* expr ::= matchClause whereClause createClause */
#line 39 "./grammar.y"
{
	yylhsminor.yy88 = New_AST_QueryExpressionNode(yymsp[-2].minor.yy75, yymsp[-1].minor.yy71, yymsp[0].minor.yy89, NULL, NULL, NULL, NULL);
}
#line 986 "./grammar.c"
  yymsp[-2].minor.yy88 = yylhsminor.yy88;
        break;
      case 3: /* expr ::= matchClause whereClause deleteClause */
#line 43 "./grammar.y"
{
	yylhsminor.yy88 = New_AST_QueryExpressionNode(yymsp[-2].minor.yy75, yymsp[-1].minor.yy71, NULL, yymsp[0].minor.yy5, NULL, NULL, NULL);
}
#line 994 "./grammar.c"
  yymsp[-2].minor.yy88 = yylhsminor.yy88;
        break;
      case 4: /* expr ::= createClause */
#line 47 "./grammar.y"
{
	yylhsminor.yy88 = New_AST_QueryExpressionNode(NULL, NULL, yymsp[0].minor.yy89, NULL, NULL, NULL, NULL);
}
#line 1002 "./grammar.c"
  yymsp[0].minor.yy88 = yylhsminor.yy88;
        break;
      case 5: /* matchClause ::= MATCH chains */
#line 57 "./grammar.y"
{
	yymsp[-1].minor.yy75 = New_AST_MatchNode(yymsp[0].minor.yy66);
}
#line 1010 "./grammar.c"
        break;
      case 6: /* createClause ::= */
#line 64 "./grammar.y"
{ 
	yymsp[1].minor.yy89 = NULL;
}
#line 1017 "./grammar.c"
        break;
      case 7: /* createClause ::= CREATE chains */
#line 68 "./grammar.y"
{
	yymsp[-1].minor.yy89 = New_AST_CreateNode(yymsp[0].minor.yy66);
}
#line 1024 "./grammar.c"
        break;
      case 8: /* chain ::= node */
#line 74 "./grammar.y"
{
	yylhsminor.yy66 = NewVector(AST_GraphEntity*, 1);
	Vector_Push(yylhsminor.yy66, yymsp[0].minor.yy69);
}
#line 1032 "./grammar.c"
  yymsp[0].minor.yy66 = yylhsminor.yy66;
        break;
      case 9: /* chain ::= chain link node */
#line 79 "./grammar.y"
{
	Vector_Push(yymsp[-2].minor.yy66, yymsp[-1].minor.yy125);
	Vector_Push(yymsp[-2].minor.yy66, yymsp[0].minor.yy69);
	yylhsminor.yy66 = yymsp[-2].minor.yy66;
}
#line 1042 "./grammar.c"
  yymsp[-2].minor.yy66 = yylhsminor.yy66;
        break;
      case 10: /* chains ::= chain */
#line 86 "./grammar.y"
{
	yylhsminor.yy66 = yymsp[0].minor.yy66;
}
#line 1050 "./grammar.c"
  yymsp[0].minor.yy66 = yylhsminor.yy66;
        break;
      case 11: /* chains ::= chains COMMA chain */
#line 90 "./grammar.y"
{
	for(int i = 0; i < Vector_Size(yymsp[0].minor.yy66); i++) {
		AST_GraphEntity *entity; 
		Vector_Get(yymsp[0].minor.yy66, i, &entity);
		Vector_Push(yymsp[-2].minor.yy66, entity);
	}
	Vector_Free(yymsp[0].minor.yy66);
	yylhsminor.yy66 = yymsp[-2].minor.yy66;
}
#line 1064 "./grammar.c"
  yymsp[-2].minor.yy66 = yylhsminor.yy66;
        break;
      case 12: /* deleteClause ::= DELETE deleteExpression */
#line 103 "./grammar.y"
{
	yymsp[-1].minor.yy5 = New_AST_DeleteNode(yymsp[0].minor.yy66);
}
#line 1072 "./grammar.c"
        break;
      case 13: /* deleteExpression ::= STRING */
#line 109 "./grammar.y"
{
	yylhsminor.yy66 = NewVector(char*, 1);
	Vector_Push(yylhsminor.yy66, yymsp[0].minor.yy0.strval);
}
#line 1080 "./grammar.c"
  yymsp[0].minor.yy66 = yylhsminor.yy66;
        break;
      case 14: /* deleteExpression ::= deleteExpression COMMA STRING */
#line 114 "./grammar.y"
{
	Vector_Push(yymsp[-2].minor.yy66, yymsp[0].minor.yy0.strval);
	yylhsminor.yy66 = yymsp[-2].minor.yy66;
}
#line 1089 "./grammar.c"
  yymsp[-2].minor.yy66 = yylhsminor.yy66;
        break;
      case 15: /* node ::= LEFT_PARENTHESIS STRING COLON STRING properties RIGHT_PARENTHESIS */
#line 122 "./grammar.y"
{
	yymsp[-5].minor.yy69 = New_AST_NodeEntity(yymsp[-4].minor.yy0.strval, yymsp[-2].minor.yy0.strval, yymsp[-1].minor.yy66);
}
#line 1097 "./grammar.c"
        break;
      case 16: /* node ::= LEFT_PARENTHESIS COLON STRING properties RIGHT_PARENTHESIS */
#line 127 "./grammar.y"
{
	yymsp[-4].minor.yy69 = New_AST_NodeEntity(NULL, yymsp[-2].minor.yy0.strval, yymsp[-1].minor.yy66);
}
#line 1104 "./grammar.c"
        break;
      case 17: /* node ::= LEFT_PARENTHESIS STRING properties RIGHT_PARENTHESIS */
#line 132 "./grammar.y"
{
	yymsp[-3].minor.yy69 = New_AST_NodeEntity(yymsp[-2].minor.yy0.strval, NULL, yymsp[-1].minor.yy66);
}
#line 1111 "./grammar.c"
        break;
      case 18: /* node ::= LEFT_PARENTHESIS properties RIGHT_PARENTHESIS */
#line 137 "./grammar.y"
{
	yymsp[-2].minor.yy69 = New_AST_NodeEntity(NULL, NULL, yymsp[-1].minor.yy66);
}
#line 1118 "./grammar.c"
        break;
      case 19: /* link ::= DASH edge RIGHT_ARROW */
#line 144 "./grammar.y"
{
	yymsp[-2].minor.yy125 = yymsp[-1].minor.yy125;
	yymsp[-2].minor.yy125->direction = N_LEFT_TO_RIGHT;
}
#line 1126 "./grammar.c"
        break;
      case 20: /* link ::= LEFT_ARROW edge DASH */
#line 150 "./grammar.y"
{
	yymsp[-2].minor.yy125 = yymsp[-1].minor.yy125;
	yymsp[-2].minor.yy125->direction = N_RIGHT_TO_LEFT;
}
#line 1134 "./grammar.c"
        break;
      case 21: /* edge ::= LEFT_BRACKET properties RIGHT_BRACKET */
#line 157 "./grammar.y"
{ 
	yymsp[-2].minor.yy125 = New_AST_LinkEntity(NULL, NULL, yymsp[-1].minor.yy66, N_DIR_UNKNOWN);
}
#line 1141 "./grammar.c"
        break;
      case 22: /* edge ::= LEFT_BRACKET STRING properties RIGHT_BRACKET */
#line 162 "./grammar.y"
{ 
	yymsp[-3].minor.yy125 = New_AST_LinkEntity(yymsp[-2].minor.yy0.strval, NULL, yymsp[-1].minor.yy66, N_DIR_UNKNOWN);
}
#line 1148 "./grammar.c"
        break;
      case 23: /* edge ::= LEFT_BRACKET COLON STRING properties RIGHT_BRACKET */
#line 167 "./grammar.y"
{ 
	yymsp[-4].minor.yy125 = New_AST_LinkEntity(NULL, yymsp[-2].minor.yy0.strval, yymsp[-1].minor.yy66, N_DIR_UNKNOWN);
}
#line 1155 "./grammar.c"
        break;
      case 24: /* edge ::= LEFT_BRACKET STRING COLON STRING properties RIGHT_BRACKET */
#line 172 "./grammar.y"
{ 
	yymsp[-5].minor.yy125 = New_AST_LinkEntity(yymsp[-4].minor.yy0.strval, yymsp[-2].minor.yy0.strval, yymsp[-1].minor.yy66, N_DIR_UNKNOWN);
}
#line 1162 "./grammar.c"
        break;
      case 25: /* properties ::= */
#line 178 "./grammar.y"
{
	yymsp[1].minor.yy66 = NULL;
}
#line 1169 "./grammar.c"
        break;
      case 26: /* properties ::= LEFT_CURLY_BRACKET mapLiteral RIGHT_CURLY_BRACKET */
#line 182 "./grammar.y"
{
	yymsp[-2].minor.yy66 = yymsp[-1].minor.yy66;
}
#line 1176 "./grammar.c"
        break;
      case 27: /* mapLiteral ::= STRING COLON value */
#line 187 "./grammar.y"
{
	yylhsminor.yy66 = NewVector(SIValue*, 2);

	SIValue *key = malloc(sizeof(SIValue));
	*key = SI_StringValC(strdup(yymsp[-2].minor.yy0.strval));
	Vector_Push(yylhsminor.yy66, key);

	SIValue *val = malloc(sizeof(SIValue));
	*val = yymsp[0].minor.yy70;
	Vector_Push(yylhsminor.yy66, val);
}
#line 1191 "./grammar.c"
  yymsp[-2].minor.yy66 = yylhsminor.yy66;
        break;
      case 28: /* mapLiteral ::= STRING COLON value COMMA mapLiteral */
#line 199 "./grammar.y"
{
	SIValue *key = malloc(sizeof(SIValue));
	*key = SI_StringValC(strdup(yymsp[-4].minor.yy0.strval));
	Vector_Push(yymsp[0].minor.yy66, key);

	SIValue *val = malloc(sizeof(SIValue));
	*val = yymsp[-2].minor.yy70;
	Vector_Push(yymsp[0].minor.yy66, val);
	
	yylhsminor.yy66 = yymsp[0].minor.yy66;
}
#line 1207 "./grammar.c"
  yymsp[-4].minor.yy66 = yylhsminor.yy66;
        break;
      case 29: /* whereClause ::= */
#line 213 "./grammar.y"
{ 
	yymsp[1].minor.yy71 = NULL;
}
#line 1215 "./grammar.c"
        break;
      case 30: /* whereClause ::= WHERE cond */
#line 216 "./grammar.y"
{
	yymsp[-1].minor.yy71 = New_AST_WhereNode(yymsp[0].minor.yy36);
}
#line 1222 "./grammar.c"
        break;
      case 31: /* cond ::= STRING DOT STRING op STRING DOT STRING */
#line 224 "./grammar.y"
{ yylhsminor.yy36 = New_AST_VaryingPredicateNode(yymsp[-6].minor.yy0.strval, yymsp[-4].minor.yy0.strval, yymsp[-3].minor.yy72, yymsp[-2].minor.yy0.strval, yymsp[0].minor.yy0.strval); }
#line 1227 "./grammar.c"
  yymsp[-6].minor.yy36 = yylhsminor.yy36;
        break;
      case 32: /* cond ::= STRING DOT STRING op value */
#line 225 "./grammar.y"
{ yylhsminor.yy36 = New_AST_ConstantPredicateNode(yymsp[-4].minor.yy0.strval, yymsp[-2].minor.yy0.strval, yymsp[-1].minor.yy72, yymsp[0].minor.yy70); }
#line 1233 "./grammar.c"
  yymsp[-4].minor.yy36 = yylhsminor.yy36;
        break;
      case 33: /* cond ::= LEFT_PARENTHESIS cond RIGHT_PARENTHESIS */
#line 226 "./grammar.y"
{ yymsp[-2].minor.yy36 = yymsp[-1].minor.yy36; }
#line 1239 "./grammar.c"
        break;
      case 34: /* cond ::= cond AND cond */
#line 227 "./grammar.y"
{ yylhsminor.yy36 = New_AST_ConditionNode(yymsp[-2].minor.yy36, AND, yymsp[0].minor.yy36); }
#line 1244 "./grammar.c"
  yymsp[-2].minor.yy36 = yylhsminor.yy36;
        break;
      case 35: /* cond ::= cond OR cond */
#line 228 "./grammar.y"
{ yylhsminor.yy36 = New_AST_ConditionNode(yymsp[-2].minor.yy36, OR, yymsp[0].minor.yy36); }
#line 1250 "./grammar.c"
  yymsp[-2].minor.yy36 = yylhsminor.yy36;
        break;
      case 36: /* op ::= EQ */
#line 232 "./grammar.y"
{ yymsp[0].minor.yy72 = EQ; }
#line 1256 "./grammar.c"
        break;
      case 37: /* op ::= GT */
#line 233 "./grammar.y"
{ yymsp[0].minor.yy72 = GT; }
#line 1261 "./grammar.c"
        break;
      case 38: /* op ::= LT */
#line 234 "./grammar.y"
{ yymsp[0].minor.yy72 = LT; }
#line 1266 "./grammar.c"
        break;
      case 39: /* op ::= LE */
#line 235 "./grammar.y"
{ yymsp[0].minor.yy72 = LE; }
#line 1271 "./grammar.c"
        break;
      case 40: /* op ::= GE */
#line 236 "./grammar.y"
{ yymsp[0].minor.yy72 = GE; }
#line 1276 "./grammar.c"
        break;
      case 41: /* op ::= NE */
#line 237 "./grammar.y"
{ yymsp[0].minor.yy72 = NE; }
#line 1281 "./grammar.c"
        break;
      case 42: /* value ::= INTEGER */
#line 243 "./grammar.y"
{  yylhsminor.yy70 = SI_DoubleVal(yymsp[0].minor.yy0.intval); }
#line 1286 "./grammar.c"
  yymsp[0].minor.yy70 = yylhsminor.yy70;
        break;
      case 43: /* value ::= STRING */
#line 244 "./grammar.y"
{  yylhsminor.yy70 = SI_StringValC(strdup(yymsp[0].minor.yy0.strval)); }
#line 1292 "./grammar.c"
  yymsp[0].minor.yy70 = yylhsminor.yy70;
        break;
      case 44: /* value ::= FLOAT */
#line 245 "./grammar.y"
{  yylhsminor.yy70 = SI_DoubleVal(yymsp[0].minor.yy0.dval); }
#line 1298 "./grammar.c"
  yymsp[0].minor.yy70 = yylhsminor.yy70;
        break;
      case 45: /* value ::= TRUE */
#line 246 "./grammar.y"
{ yymsp[0].minor.yy70 = SI_BoolVal(1); }
#line 1304 "./grammar.c"
        break;
      case 46: /* value ::= FALSE */
#line 247 "./grammar.y"
{ yymsp[0].minor.yy70 = SI_BoolVal(0); }
#line 1309 "./grammar.c"
        break;
      case 47: /* returnClause ::= RETURN returnElements */
#line 251 "./grammar.y"
{
	yymsp[-1].minor.yy68 = New_AST_ReturnNode(yymsp[0].minor.yy66, 0);
}
#line 1316 "./grammar.c"
        break;
      case 48: /* returnClause ::= RETURN DISTINCT returnElements */
#line 254 "./grammar.y"
{
	yymsp[-2].minor.yy68 = New_AST_ReturnNode(yymsp[0].minor.yy66, 1);
}
#line 1323 "./grammar.c"
        break;
      case 49: /* returnElements ::= returnElements COMMA returnElement */
#line 261 "./grammar.y"
{
	Vector_Push(yymsp[-2].minor.yy66, yymsp[0].minor.yy54);
	yylhsminor.yy66 = yymsp[-2].minor.yy66;
}
#line 1331 "./grammar.c"
  yymsp[-2].minor.yy66 = yylhsminor.yy66;
        break;
      case 50: /* returnElements ::= returnElement */
#line 266 "./grammar.y"
{
	yylhsminor.yy66 = NewVector(AST_ReturnElementNode*, 1);
	Vector_Push(yylhsminor.yy66, yymsp[0].minor.yy54);
}
#line 1340 "./grammar.c"
  yymsp[0].minor.yy66 = yylhsminor.yy66;
        break;
      case 51: /* returnElement ::= variable */
#line 273 "./grammar.y"
{
	yylhsminor.yy54 = New_AST_ReturnElementNode(N_PROP, yymsp[0].minor.yy73, NULL, NULL);
}
#line 1348 "./grammar.c"
  yymsp[0].minor.yy54 = yylhsminor.yy54;
        break;
      case 52: /* returnElement ::= variable AS STRING */
#line 276 "./grammar.y"
{
	yylhsminor.yy54 = New_AST_ReturnElementNode(N_PROP, yymsp[-2].minor.yy73, NULL, yymsp[0].minor.yy0.strval);
}
#line 1356 "./grammar.c"
  yymsp[-2].minor.yy54 = yylhsminor.yy54;
        break;
      case 53: /* returnElement ::= aggFunc */
#line 279 "./grammar.y"
{
	yylhsminor.yy54 = yymsp[0].minor.yy54;
}
#line 1364 "./grammar.c"
  yymsp[0].minor.yy54 = yylhsminor.yy54;
        break;
      case 54: /* returnElement ::= STRING */
#line 282 "./grammar.y"
{
	yylhsminor.yy54 = New_AST_ReturnElementNode(N_NODE, New_AST_Variable(yymsp[0].minor.yy0.strval, NULL), NULL, NULL);
}
#line 1372 "./grammar.c"
  yymsp[0].minor.yy54 = yylhsminor.yy54;
        break;
      case 55: /* variable ::= STRING DOT STRING */
#line 288 "./grammar.y"
{
	yylhsminor.yy73 = New_AST_Variable(yymsp[-2].minor.yy0.strval, yymsp[0].minor.yy0.strval);
}
#line 1380 "./grammar.c"
  yymsp[-2].minor.yy73 = yylhsminor.yy73;
        break;
      case 56: /* aggFunc ::= STRING LEFT_PARENTHESIS variable RIGHT_PARENTHESIS */
#line 294 "./grammar.y"
{
	yylhsminor.yy54 = New_AST_ReturnElementNode(N_AGG_FUNC, yymsp[-1].minor.yy73, yymsp[-3].minor.yy0.strval, NULL);
}
#line 1388 "./grammar.c"
  yymsp[-3].minor.yy54 = yylhsminor.yy54;
        break;
      case 57: /* aggFunc ::= STRING LEFT_PARENTHESIS variable RIGHT_PARENTHESIS AS STRING */
#line 297 "./grammar.y"
{
	yylhsminor.yy54 = New_AST_ReturnElementNode(N_AGG_FUNC, yymsp[-3].minor.yy73, yymsp[-5].minor.yy0.strval, yymsp[0].minor.yy0.strval);
}
#line 1396 "./grammar.c"
  yymsp[-5].minor.yy54 = yylhsminor.yy54;
        break;
      case 58: /* orderClause ::= */
#line 303 "./grammar.y"
{
	yymsp[1].minor.yy108 = NULL;
}
#line 1404 "./grammar.c"
        break;
      case 59: /* orderClause ::= ORDER BY columnNameList */
#line 306 "./grammar.y"
{
	yymsp[-2].minor.yy108 = New_AST_OrderNode(yymsp[0].minor.yy66, ORDER_DIR_ASC);
}
#line 1411 "./grammar.c"
        break;
      case 60: /* orderClause ::= ORDER BY columnNameList ASC */
#line 309 "./grammar.y"
{
	yymsp[-3].minor.yy108 = New_AST_OrderNode(yymsp[-1].minor.yy66, ORDER_DIR_ASC);
}
#line 1418 "./grammar.c"
        break;
      case 61: /* orderClause ::= ORDER BY columnNameList DESC */
#line 312 "./grammar.y"
{
	yymsp[-3].minor.yy108 = New_AST_OrderNode(yymsp[-1].minor.yy66, ORDER_DIR_DESC);
}
#line 1425 "./grammar.c"
        break;
      case 62: /* columnNameList ::= columnNameList COMMA columnName */
#line 317 "./grammar.y"
{
	Vector_Push(yymsp[-2].minor.yy66, yymsp[0].minor.yy74);
	yylhsminor.yy66 = yymsp[-2].minor.yy66;
}
#line 1433 "./grammar.c"
  yymsp[-2].minor.yy66 = yylhsminor.yy66;
        break;
      case 63: /* columnNameList ::= columnName */
#line 321 "./grammar.y"
{
	yylhsminor.yy66 = NewVector(AST_ColumnNode*, 1);
	Vector_Push(yylhsminor.yy66, yymsp[0].minor.yy74);
}
#line 1442 "./grammar.c"
  yymsp[0].minor.yy66 = yylhsminor.yy66;
        break;
      case 64: /* columnName ::= variable */
#line 327 "./grammar.y"
{
	yylhsminor.yy74 = AST_ColumnNodeFromVariable(yymsp[0].minor.yy73);
	Free_AST_Variable(yymsp[0].minor.yy73);
}
#line 1451 "./grammar.c"
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 65: /* columnName ::= STRING */
#line 331 "./grammar.y"
{
	yylhsminor.yy74 = AST_ColumnNodeFromAlias(yymsp[0].minor.yy0.strval);
}
#line 1459 "./grammar.c"
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 66: /* limitClause ::= */
#line 337 "./grammar.y"
{
	yymsp[1].minor.yy7 = NULL;
}
#line 1467 "./grammar.c"
        break;
      case 67: /* limitClause ::= LIMIT INTEGER */
#line 340 "./grammar.y"
{
	yymsp[-1].minor.yy7 = New_AST_LimitNode(yymsp[0].minor.yy0.intval);
}
#line 1474 "./grammar.c"
        break;
      default:
        break;
/********** End reduce actions ************************************************/
  };
  assert( yyruleno<sizeof(yyRuleInfo)/sizeof(yyRuleInfo[0]) );
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yyact = yy_find_reduce_action(yymsp[-yysize].stateno,(YYCODETYPE)yygoto);
  if( yyact <= YY_MAX_SHIFTREDUCE ){
    if( yyact>YY_MAX_SHIFT ){
      yyact += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
    }
    yymsp -= yysize-1;
    yypParser->yytos = yymsp;
    yymsp->stateno = (YYACTIONTYPE)yyact;
    yymsp->major = (YYCODETYPE)yygoto;
    yyTraceShift(yypParser, yyact);
  }else{
    assert( yyact == YY_ACCEPT_ACTION );
    yypParser->yytos -= yysize;
    yy_accept(yypParser);
  }
}

/*
** The following code executes when the parse fails
*/
#ifndef YYNOERRORRECOVERY
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  ParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yytos>yypParser->yystack ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
/************ Begin %parse_failure code ***************************************/
/************ End %parse_failure code *****************************************/
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}
#endif /* YYNOERRORRECOVERY */

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  ParseTOKENTYPE yyminor         /* The minor type of the error token */
){
  ParseARG_FETCH;
#define TOKEN yyminor
/************ Begin %syntax_error code ****************************************/
#line 20 "./grammar.y"

	char buf[256];
	snprintf(buf, 256, "Syntax error at offset %d near '%s'\n", TOKEN.pos, TOKEN.s);

	ctx->ok = 0;
	ctx->errorMsg = strdup(buf);
#line 1540 "./grammar.c"
/************ End %syntax_error code ******************************************/
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  ParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
#ifndef YYNOERRORRECOVERY
  yypParser->yyerrcnt = -1;
#endif
  assert( yypParser->yytos==yypParser->yystack );
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
/*********** Begin %parse_accept code *****************************************/
/*********** End %parse_accept code *******************************************/
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "ParseAlloc" which describes the current state of the parser.
** The second argument is the major token number.  The third is
** the minor token.  The fourth optional argument is whatever the
** user wants (and specified in the grammar) and is available for
** use by the action routines.
**
** Inputs:
** <ul>
** <li> A pointer to the parser (an opaque structure.)
** <li> The major token number.
** <li> The minor token number.
** <li> An option argument of a grammar-specified type.
** </ul>
**
** Outputs:
** None.
*/
void Parse(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  ParseTOKENTYPE yyminor       /* The value for the token */
  ParseARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  unsigned int yyact;   /* The parser action. */
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  int yyendofinput;     /* True if we are at the end of input */
#endif
#ifdef YYERRORSYMBOL
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
#endif
  yyParser *yypParser;  /* The parser */

  yypParser = (yyParser*)yyp;
  assert( yypParser->yytos!=0 );
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  yyendofinput = (yymajor==0);
#endif
  ParseARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput '%s'\n",yyTracePrompt,yyTokenName[yymajor]);
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,(YYCODETYPE)yymajor);
    if( yyact <= YY_MAX_SHIFTREDUCE ){
      yy_shift(yypParser,yyact,yymajor,yyminor);
#ifndef YYNOERRORRECOVERY
      yypParser->yyerrcnt--;
#endif
      yymajor = YYNOCODE;
    }else if( yyact <= YY_MAX_REDUCE ){
      yy_reduce(yypParser,yyact-YY_MIN_REDUCE);
    }else{
      assert( yyact == YY_ERROR_ACTION );
      yyminorunion.yy0 = yyminor;
#ifdef YYERRORSYMBOL
      int yymx;
#endif
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE,"%sSyntax Error!\n",yyTracePrompt);
      }
#endif
#ifdef YYERRORSYMBOL
      /* A syntax error has occurred.
      ** The response to an error depends upon whether or not the
      ** grammar defines an error token "ERROR".  
      **
      ** This is what we do if the grammar does define ERROR:
      **
      **  * Call the %syntax_error function.
      **
      **  * Begin popping the stack until we enter a state where
      **    it is legal to shift the error symbol, then shift
      **    the error symbol.
      **
      **  * Set the error count to three.
      **
      **  * Begin accepting and shifting new tokens.  No new error
      **    processing will occur until three tokens have been
      **    shifted successfully.
      **
      */
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yymajor,yyminor);
      }
      yymx = yypParser->yytos->major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yypParser, (YYCODETYPE)yymajor, &yyminorunion);
        yymajor = YYNOCODE;
      }else{
        while( yypParser->yytos >= &yypParser->yystack
            && yymx != YYERRORSYMBOL
            && (yyact = yy_find_reduce_action(
                        yypParser->yytos->stateno,
                        YYERRORSYMBOL)) >= YY_MIN_REDUCE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yytos < yypParser->yystack || yymajor==0 ){
          yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
#ifndef YYNOERRORRECOVERY
          yypParser->yyerrcnt = -1;
#endif
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          yy_shift(yypParser,yyact,YYERRORSYMBOL,yyminor);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
#elif defined(YYNOERRORRECOVERY)
      /* If the YYNOERRORRECOVERY macro is defined, then do not attempt to
      ** do any kind of error recovery.  Instead, simply invoke the syntax
      ** error routine and continue going as if nothing had happened.
      **
      ** Applications can set this macro (for example inside %include) if
      ** they intend to abandon the parse upon the first syntax error seen.
      */
      yy_syntax_error(yypParser,yymajor, yyminor);
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      yymajor = YYNOCODE;
      
#else  /* YYERRORSYMBOL is not defined */
      /* This is what we do if the grammar does not define ERROR:
      **
      **  * Report an error message, and throw away the input token.
      **
      **  * If the input token is $, then fail the parse.
      **
      ** As before, subsequent error messages are suppressed until
      ** three input tokens have been successfully shifted.
      */
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yymajor, yyminor);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
#ifndef YYNOERRORRECOVERY
        yypParser->yyerrcnt = -1;
#endif
      }
      yymajor = YYNOCODE;
#endif
    }
  }while( yymajor!=YYNOCODE && yypParser->yytos>yypParser->yystack );
#ifndef NDEBUG
  if( yyTraceFILE ){
    yyStackEntry *i;
    char cDiv = '[';
    fprintf(yyTraceFILE,"%sReturn. Stack=",yyTracePrompt);
    for(i=&yypParser->yystack[1]; i<=yypParser->yytos; i++){
      fprintf(yyTraceFILE,"%c%s", cDiv, yyTokenName[i->major]);
      cDiv = ' ';
    }
    fprintf(yyTraceFILE,"]\n");
  }
#endif
  return;
}
#line 344 "./grammar.y"


	/* Definitions of flex stuff */
	typedef struct yy_buffer_state *YY_BUFFER_STATE;
	int             yylex( void );
	YY_BUFFER_STATE yy_scan_string( const char * );
  	YY_BUFFER_STATE yy_scan_bytes( const char *, size_t );
  	extern int yylineno;
  	extern char *yytext;
	extern int yycolumn;

	AST_QueryExpressionNode *Query_Parse(const char *q, size_t len, char **err) {
		yycolumn = 1;	// Reset lexer's token tracking position
		yy_scan_bytes(q, len);
  		void* pParser = ParseAlloc(malloc);
  		int t = 0;

		parseCtx ctx = {.root = NULL, .ok = 1, .errorMsg = NULL};

  		while( (t = yylex()) != 0) {
			Parse(pParser, t, tok, &ctx);
		}
		if (ctx.ok) {
			Parse(pParser, 0, tok, &ctx);
  		}
		ParseFree(pParser, free);
		if (err) {
			*err = ctx.errorMsg;
		}
		return ctx.root;
	}
#line 1775 "./grammar.c"
