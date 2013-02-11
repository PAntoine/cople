#ifndef lint
/*static char yysccsid[] = "from: @(#)yaccpar	1.9 (Berkeley) 02/21/93";*/
static char yyrcsid[] = "$Id: cople.tab.c,v 1.2 1999/11/07 10:36:53 devel Exp $";
#endif
#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define yyclearin (yychar=(-1))
#define yyerrok (yyerrflag=0)
#define YYRECOVERING (yyerrflag!=0)
#define YYPREFIX "yy"
#line 20 "cople.y"
	#include	"cople.h"
	#include	"codetree.h"
	#include	<stdio.h>

	#define	YYDEBUG  	1

	extern	CONSTANT	*tmp_const;
	extern	SYMBOL	*tmp_symbol;
	extern	PARAMETER	tp;
	extern	CLAUSE	*system_pnt;
	extern	CLAUSE	*user_pnt;
	extern	CLAUSE	*node_pnt;

	extern	CLAUSE	*tree_root;
	extern	CODE_TYPE	program_type;
	extern	char	*prog_name;

	int	yydebug = 0;
#line 40 "cople.y"
typedef union{
	char	char_string[255];
	char	character;
	int	int_number;
	float	float_number;
	void*	complex_type;
	SYMBOL*	symbol_type;
	CONSTANT*	const_type;
	INST_TYPE	instr_type;
	CLAUSE*	clause;
	LIST*	list_type;
	PARAMETER	parameter;
	PARAMETER_LIST*	parameter_list;
	} YYSTYPE;
#line 47 "cople.tab.c"
#define STRING_CONST 257
#define VALID_NAME 258
#define CHARACTER 259
#define INTEGER 260
#define FLOAT 261
#define COPLE 262
#define COPLE_APP 263
#define COPLE_OBJ 264
#define COPLE_SYS 265
#define LT_OP 266
#define LE_OP 267
#define GT_OP 268
#define GE_OP 269
#define EQ_OP 270
#define NE_OP 271
#define ASSIGN_OP 272
#define AND_OP 273
#define OR_OP 274
#define NOT_OP 275
#define XOR_OP 276
#define MOD_OP 277
#define IF 278
#define THEN 279
#define ELSE 280
#define ELSEIF 281
#define FI 282
#define WHILE 283
#define FORALL 284
#define FOR 285
#define BEGIN 286
#define END 287
#define WHENEVER 288
#define REF 289
#define INT 290
#define CHAR 291
#define BOOLEAN 292
#define REAL 293
#define INSTANCE 294
#define STRING 295
#define VOID 296
#define MANDATORY 297
#define STRUCT 298
#define NEW 299
#define DELETE 300
#define OF 301
#define WITH 302
#define TRUE 303
#define FALSE 304
#define YYERROR_VERBOSE 305
#define YYERRCODE 256
short yylhs[] = {                                        -1,
    0,    0,    0,   49,   15,   15,   50,   53,   14,   54,
   51,   16,   16,    2,    2,   10,   10,    9,    9,    3,
    3,    4,    4,   52,   52,   55,   55,    6,    6,   11,
   11,   26,    5,    5,    7,    7,    7,    8,    8,   28,
   27,   17,   17,   17,   17,   17,   56,   58,   58,   59,
   59,   57,   60,   60,   60,   25,   25,   18,   18,   18,
   18,   18,   18,   18,   18,   18,   61,   30,   62,   30,
   40,   31,   31,   31,   31,   32,   32,   32,   32,   32,
   33,   33,   34,   35,   35,   35,   39,   39,   39,   45,
   45,   45,   45,   43,   43,   44,   42,   41,   41,   41,
   46,   46,   46,   47,   47,   47,   47,   47,   47,   48,
   48,   48,   48,   48,   29,   29,   19,   19,   19,   19,
   19,   20,   20,   21,   22,   23,   23,    1,    1,    1,
    1,    1,   24,   24,   24,   24,   36,   36,   38,   38,
   38,   38,   38,   38,   37,   63,   12,   12,   13,   13,
   13,   13,   13,   13,
};
short yylen[] = {                                         2,
    1,    1,    1,    7,    1,    1,    7,    0,    4,    3,
    7,    1,    1,    3,    1,    3,    1,    3,    3,    3,
    1,    2,    1,    2,    1,    2,    1,    3,    1,    3,
    1,    3,    3,    1,    3,    3,    1,    3,    1,    3,
    1,    1,    1,    1,    1,    1,    5,    6,    5,    4,
    2,    4,    3,    5,    6,    2,    2,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    0,    5,    0,    4,
    4,    9,    8,    7,    8,    3,    4,    5,    3,    3,
    3,    5,    5,    6,    7,    5,    4,    6,    5,    1,
    1,    1,    1,    3,    2,    3,    3,    3,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    4,    1,    1,    1,    1,
    1,    2,    1,    3,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    3,    3,    5,    6,    6,    6,    6,
    7,    7,    4,    4,    2,    4,    1,    1,    4,    5,
    6,    2,    3,    4,
};
short yydefred[] = {                                      0,
    0,    0,    1,    2,    3,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    8,    0,    6,    0,    4,    5,
    7,    0,   13,   11,   12,    0,    0,    0,  131,  128,
  129,  130,    0,  132,    0,    0,   25,    0,   42,   43,
   44,   45,   46,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   23,    0,    0,    0,   58,   59,
   60,   61,   62,   63,   64,   65,   66,    0,    0,    0,
    0,    0,   24,    0,    0,    0,    0,    0,    0,    0,
  121,    0,  125,    0,    0,  126,  127,    0,    0,  100,
  117,  119,  118,  120,   99,    0,   91,   90,   92,    0,
    0,    0,    0,    0,   56,   57,    0,   70,   22,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   27,
    0,    0,   68,    0,    0,   34,    0,    0,    0,   39,
   37,   41,  134,    0,    0,   95,    0,  122,  105,  104,
  107,  106,  109,  108,  101,  102,  103,  110,  111,  112,
  113,  114,    0,    0,    0,    0,    0,    0,   81,    0,
    0,    0,    0,    0,    0,    0,   80,   79,   76,    0,
    0,    0,   52,    0,   50,   15,    0,    0,   29,    0,
    0,   26,  145,    0,    0,  116,    0,    0,  143,    0,
  144,    0,  124,   98,   94,   97,   96,    0,    0,    0,
    0,   31,    0,    0,    0,   71,    0,   77,    0,    0,
   54,    0,   47,    0,    0,   49,    0,    0,  147,    0,
    0,    0,  136,   33,   40,    0,   35,   38,   36,    0,
    0,    0,    0,   86,    0,   83,    0,    0,   82,    0,
    0,   78,    0,    0,    0,   55,    0,   48,   14,   28,
    0,    0,  146,    0,  137,  138,    0,    0,   84,   32,
   30,    0,    0,    0,    0,    0,  139,  140,    0,   17,
    0,   21,  153,    0,    0,    0,   85,    0,   74,    0,
    0,    0,  141,  142,    0,   18,    0,   19,  154,    0,
    0,    0,   73,   75,    0,   20,   16,  150,    0,    0,
   89,   72,  151,   88,
};
short yydgoto[] = {                                       2,
   36,  178,  269,   54,  125,  180,  128,  129,  248,  271,
  201,  221,  222,   21,   19,   24,   37,   55,   90,   91,
   92,   93,   94,   56,   57,  202,  130,  131,   95,   59,
   60,   61,   62,   63,   64,   65,  120,   66,  235,   67,
   96,   97,   98,   99,  132,  153,  154,  155,    3,    4,
    5,   38,   22,   74,  121,   39,   40,   41,   42,   43,
   26,   27,  122,
};
short yysindex[] = {                                    -17,
 -207,    0,    0,    0,    0,    3,  -88,   61,   88,  101,
 -123, -119, -114, -221,    0, -216,    0,    0,    0,    0,
    0, -126,    0,    0,    0, -126,  401, -144,    0,    0,
    0,    0, -104,    0,  -99,  -97,    0, -180,    0,    0,
    0,    0,    0,  261,    6,  128,  -15,  148,  -69,  151,
  -66, -184,  -65,  349,    0,  149, -102,  -78,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  -62,  -96,   76,
  -10,  143,    0,  -56,  461,  -55,  -15,   -2,  -52,  -51,
    0,  118,    0,  164,    5,    0,    0,  -15,  -48,    0,
    0,    0,    0,    0,    0,  520,    0,    0,    0,  -64,
  -15, -244,  -30,  -61,    0,    0,  -39,    0,    0,  -45,
  -15,  -35,   -9,  -44, -126,   97,  -82,  -41,  179,    0,
  -56,  -59,    0,  185,  -18,    0,   -8,   18,   22,    0,
    0,    0,    0,  181,  -19,    0,  209,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    5,    5,    5,  401,  210,   19,    0,  -15,
   20,  212,  401,   -5,   21,  240,    0,    0,    0,  246,
   25,    2,    0,  161,    0,    0,  229,   12,    0,  248,
  282,    0,    0,   33,  -15,    0,  -15,   37,    0,   -2,
    0,   -2,    0,    0,    0,    0,    0, -100,  -59,  235,
  -40,    0,  239,   28,  -15,    0,   45,    0,  264,   -2,
    0,   13,    0,  182,   47,    0,   52, -144,    0,   53,
  271,  276,    0,    0,    0,  262,    0,    0,    0,   27,
   75,  401,  -15,    0,   43,    0,    5,   19,    0,  -28,
  296,    0,   -2,   79,   92,    0,   93,    0,    0,    0,
  100,  298,    0, -164,    0,    0,   78,   82,    0,    0,
    0,  -59,  321,  -21,   99,  112,    0,    0,  -29,    0,
  -22,    0,    0,   97, -144,  105,    0,  401,    0,  -59,
  -59,  324,    0,    0,   97,    0,  182,    0,    0,  132,
  325, -182,    0,    0,  -59,    0,    0,    0,   97,  401,
    0,    0,    0,    0,
};
short yyrindex[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  311,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  -37,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  162,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  430,    0,   48,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  390,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  223,    0,    0,    0,    0,    0,    0,    0,
  388,    0,    0,   86,    0,    0,  487,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  124,    0,    0,    0,    0,    0,    0,    0,  133,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  351,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  114,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  130,  111,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,
};
short yygindex[] = {                                      0,
   16,  224,    0,  350,    0,    0, -176, -156, -208,    0,
    0,    0,    0,    0,    0,    0,  -11,   -6,  -95,    0,
    0,    0,    0,  -91,  -98,  160,  213, -110,  -26,  -14,
    0,  320,    0,    0,    0,    0,  278,    0,  137,    0,
  -53,    0,    0,    0,  691,    0,    0,    0,    0,    0,
    0,  398,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,
};
#define YYTABLESIZE 955
short yytable[] = {                                      20,
   58,   25,   52,  238,   88,  161,  165,   52,  133,   89,
  177,   88,  262,  168,  285,  230,   89,   58,   88,  281,
  175,  287,  173,   89,   88,  185,   73,   58,  160,   89,
  207,  136,   73,  244,   17,  231,   79,   88,  270,   23,
   79,   18,   89,   68,   88,   78,    1,  109,   58,   89,
  116,  170,  187,  245,    6,  215,  215,  158,  189,   76,
    7,  188,  191,   76,   18,  190,  265,  255,  109,   18,
  188,  211,  208,  105,  186,  106,   58,  227,  297,  229,
  117,  171,   77,  123,  123,  123,  266,  159,  123,  123,
  123,  123,  123,   11,  123,  286,   77,  300,  233,  195,
  196,  197,  288,  174,  216,  246,  123,  183,   28,   29,
   30,   31,   32,   33,   34,  256,   72,   35,  190,  267,
   12,  135,  188,  135,  275,   29,   30,   31,   32,   58,
   34,  135,  268,   13,   14,  190,   58,   89,   15,  283,
  123,   89,  188,   16,  135,   29,   30,   31,   32,  198,
   34,  272,  284,   69,  152,  190,  206,  152,   70,  133,
   71,  133,   28,   29,   30,   31,   32,   33,   34,  111,
  149,   35,  123,  149,    8,    9,   10,  176,  289,  232,
  233,  234,  133,  260,  236,   80,  239,  101,  102,  296,
  103,  104,  107,  112,  110,  113,  220,   51,  115,   51,
  118,  119,  124,  303,  114,   58,  134,  133,   77,  135,
  135,  138,  166,  172,  156,  247,  179,  163,  181,  184,
  192,   81,   82,   83,   84,  257,   18,   82,   81,   82,
   83,   84,  164,  251,  115,   81,   82,   83,   84,   85,
  193,   81,   82,   83,   84,   18,   85,  279,  133,  194,
  199,   58,  172,   85,   81,  127,   83,   84,   53,   85,
   53,   81,   82,   83,   84,  293,  294,   86,   87,  276,
  205,  292,   85,   58,   86,   87,  200,  204,  209,  210,
  302,   86,   87,   52,  176,  213,   51,   86,   87,  214,
  290,  217,  223,  304,  226,  237,   53,  240,   52,  164,
   86,   87,  242,  243,  247,  123,  249,   86,   87,  250,
  252,  253,  123,  123,  123,  123,  123,  123,  123,  254,
  123,  123,  187,  123,  259,  123,  123,  123,  123,  123,
  123,  123,  123,  123,  123,  123,  123,  123,  123,  123,
  123,  123,  123,  135,  123,  123,   69,   53,   69,   81,
  135,   83,   84,   81,  264,   83,   84,  273,  274,  277,
  278,  280,  291,  135,  295,  135,  135,  135,  135,  135,
  135,  135,  135,  135,  135,  135,  135,  135,  135,  135,
  135,  133,  135,  135,   53,  299,   52,    9,  133,  298,
   10,  148,   87,   75,  212,   86,   87,  261,  182,   86,
   87,  133,  228,  133,  133,  133,  133,  133,  133,  133,
  133,  133,  133,  133,  133,  133,  133,  133,  133,   51,
  133,  133,  162,   44,    0,   93,   51,   93,  301,    0,
   93,    0,    0,   93,    0,    0,   53,    0,   52,   51,
    0,    0,    0,    0,   51,   51,   51,   51,   93,   51,
   51,   51,   51,   51,   51,   51,   51,    0,   51,   51,
    0,    0,    0,    0,    0,  115,  115,  115,    0,    0,
  115,  115,  115,  115,  115,    0,  115,    0,    0,    0,
   53,    0,   93,    0,    0,    0,    0,   53,  115,    0,
    0,    0,    0,    0,    0,    0,   53,    0,   52,    0,
   53,    0,    0,    0,    0,   53,   53,   53,   53,    0,
   53,   53,   53,   53,   53,   53,   53,   53,   45,   53,
   53,    0,  115,  115,    0,   46,    0,  115,  115,  115,
  115,  115,    0,  115,    0,    0,    0,    0,   47,    0,
    0,    0,    0,   48,   49,   50,   18,    0,   51,   28,
   29,   30,   31,   32,   33,   34,  149,    0,   35,    0,
    0,  148,  151,    0,  152,    0,  150,    0,   69,    0,
  218,   29,   30,   31,   32,   69,   34,  219,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   69,    0,
    0,    0,    0,   69,   69,   69,   69,    0,   69,   67,
   67,   67,   67,   67,   67,   67,   45,    0,   67,    0,
    0,    0,    0,   46,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   47,    0,    0,    0,
    0,   48,   49,   50,   18,  108,   51,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   93,    0,    0,
    0,    0,    0,    0,   93,    0,    0,    0,   45,    0,
    0,    0,    0,    0,    0,   46,    0,   93,   93,   93,
   93,   93,   93,   93,   93,   93,   93,   93,   47,    0,
    0,    0,    0,   48,   49,   50,   18,  115,   51,    0,
    0,    0,    0,    0,  115,  115,  115,  115,  115,  115,
  115,  115,  115,  115,    0,  115,    0,  115,  115,  115,
  115,  115,  115,  115,  115,  115,  115,  115,   45,    0,
    0,    0,    0,    0,    0,   46,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  100,   47,    0,
    0,    0,    0,   48,   49,   50,   18,  123,   51,    0,
    0,    0,  115,  115,  115,  115,  115,  115,    0,  115,
  115,    0,  115,    0,    0,    0,    0,  126,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  137,    0,
    0,    0,    0,    0,    0,  139,  140,  141,  142,  143,
  144,  157,  145,  146,    0,  147,    0,    0,    0,    0,
    0,  167,  169,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  203,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  224,    0,  225,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  241,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  258,    0,    0,    0,    0,    0,    0,
  263,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  282,
};
short yycheck[] = {                                      14,
   27,   16,   38,   44,   40,   36,   46,   38,   46,   45,
   93,   40,   41,  112,   44,  192,   45,   44,   40,   41,
  116,   44,  114,   45,   40,   44,   38,   54,   59,   45,
   36,   85,   44,  210,  256,  192,   35,   40,  247,  256,
   35,  286,   45,   28,   40,   40,   64,   54,   75,   45,
   61,   61,   61,  210,  262,   44,   44,  302,   41,   58,
   58,   44,   41,   58,  286,   44,  243,   41,   75,  286,
   44,  170,  164,  258,   93,  260,  103,  188,  287,  190,
   91,   91,   91,   36,   37,   38,  243,  102,   41,   42,
   43,   44,   45,   33,   47,  125,   91,  280,  281,  153,
  154,  155,  125,  115,   93,   93,   59,  122,  289,  290,
  291,  292,  293,  294,  295,   41,  297,  298,   44,   41,
   33,   36,   44,   38,  289,  290,  291,  292,  293,  156,
  295,   46,   41,   33,  258,   44,  163,   45,  258,   41,
   93,   45,   44,  258,   59,  290,  291,  292,  293,  156,
  295,  247,   41,  258,   41,   44,  163,   44,  258,   36,
  258,   38,  289,  290,  291,  292,  293,  294,  295,  272,
   41,  298,  125,   44,  263,  264,  265,  260,  274,  280,
  281,  282,   59,  237,  199,   58,  201,   40,  258,  285,
   40,  258,  258,  272,   46,  258,  181,   36,  123,   38,
   58,  258,  258,  299,  301,  232,  258,  260,   91,   46,
  125,  260,  258,  258,  279,  123,  258,  279,   40,   35,
   40,  257,  258,  259,  260,  232,  286,  258,  257,  258,
  259,  260,  272,  218,  272,  257,  258,  259,  260,  275,
  260,  257,  258,  259,  260,  286,  275,  262,  125,   41,
   41,  278,  258,  275,  257,  258,  259,  260,   36,  275,
   38,  257,  258,  259,  260,  280,  281,  303,  304,  254,
   59,  278,  275,  300,  303,  304,  258,  258,  258,   40,
  295,  303,  304,   38,  260,  125,  125,  303,  304,   61,
  275,   44,  260,  300,  258,   61,   36,   59,   38,  272,
  303,  304,  258,   40,  123,  258,  260,  303,  304,  258,
  258,   41,  265,  266,  267,  268,  269,  270,  271,   44,
  273,  274,   61,  276,  282,  278,  279,  280,  281,  282,
  283,  284,  285,  286,  287,  288,  289,  290,  291,  292,
  293,  294,  295,  258,  297,  298,   36,  125,   38,  257,
  265,  259,  260,  257,   59,  259,  260,  258,   61,  282,
  279,   41,  258,  278,   41,  280,  281,  282,  283,  284,
  285,  286,  287,  288,  289,  290,  291,  292,  293,  294,
  295,  258,  297,  298,   36,   61,   38,    0,  265,  258,
  258,   41,  282,   44,  171,  303,  304,  238,  121,  303,
  304,  278,  190,  280,  281,  282,  283,  284,  285,  286,
  287,  288,  289,  290,  291,  292,  293,  294,  295,  258,
  297,  298,  103,   26,   -1,   36,  265,   38,  292,   -1,
   41,   -1,   -1,   44,   -1,   -1,   36,   -1,   38,  278,
   -1,   -1,   -1,   -1,  283,  284,  285,  286,   59,  288,
  289,  290,  291,  292,  293,  294,  295,   -1,  297,  298,
   -1,   -1,   -1,   -1,   -1,   36,   37,   38,   -1,   -1,
   41,   42,   43,   44,   45,   -1,   47,   -1,   -1,   -1,
  258,   -1,   93,   -1,   -1,   -1,   -1,  265,   59,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   36,   -1,   38,   -1,
  278,   -1,   -1,   -1,   -1,  283,  284,  285,  286,   -1,
  288,  289,  290,  291,  292,  293,  294,  295,  258,  297,
  298,   -1,   93,   37,   -1,  265,   -1,   41,   42,   43,
   44,   45,   -1,   47,   -1,   -1,   -1,   -1,  278,   -1,
   -1,   -1,   -1,  283,  284,  285,  286,   -1,  288,  289,
  290,  291,  292,  293,  294,  295,   37,   -1,  298,   -1,
   -1,   42,   43,   -1,   45,   -1,   47,   -1,  258,   -1,
  289,  290,  291,  292,  293,  265,  295,  296,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  278,   -1,
   -1,   -1,   -1,  283,  284,  285,  286,   -1,  288,  289,
  290,  291,  292,  293,  294,  295,  258,   -1,  298,   -1,
   -1,   -1,   -1,  265,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  278,   -1,   -1,   -1,
   -1,  283,  284,  285,  286,  287,  288,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  258,   -1,   -1,
   -1,   -1,   -1,   -1,  265,   -1,   -1,   -1,  258,   -1,
   -1,   -1,   -1,   -1,   -1,  265,   -1,  278,  279,  280,
  281,  282,  283,  284,  285,  286,  287,  288,  278,   -1,
   -1,   -1,   -1,  283,  284,  285,  286,  258,  288,   -1,
   -1,   -1,   -1,   -1,  265,  266,  267,  268,  269,  270,
  271,  272,  273,  274,   -1,  276,   -1,  278,  279,  280,
  281,  282,  283,  284,  285,  286,  287,  288,  258,   -1,
   -1,   -1,   -1,   -1,   -1,  265,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   47,  278,   -1,
   -1,   -1,   -1,  283,  284,  285,  286,  287,  288,   -1,
   -1,   -1,  266,  267,  268,  269,  270,  271,   -1,  273,
  274,   -1,  276,   -1,   -1,   -1,   -1,   77,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   88,   -1,
   -1,   -1,   -1,   -1,   -1,  266,  267,  268,  269,  270,
  271,  101,  273,  274,   -1,  276,   -1,   -1,   -1,   -1,
   -1,  111,  112,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  160,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  185,   -1,  187,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  205,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  233,   -1,   -1,   -1,   -1,   -1,   -1,
  240,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  264,
};
#define YYFINAL 2
#ifndef YYDEBUG
#define YYDEBUG 1
#endif
#define YYMAXTOKEN 305
#if YYDEBUG
char *yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
"'!'",0,"'#'","'$'","'%'","'&'",0,"'('","')'","'*'","'+'","','","'-'","'.'",
"'/'",0,0,0,0,0,0,0,0,0,0,"':'","';'",0,"'='",0,0,"'@'",0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,"'['",0,"']'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,"'{'",0,"'}'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"STRING_CONST",
"VALID_NAME","CHARACTER","INTEGER","FLOAT","COPLE","COPLE_APP","COPLE_OBJ",
"COPLE_SYS","LT_OP","LE_OP","GT_OP","GE_OP","EQ_OP","NE_OP","ASSIGN_OP",
"AND_OP","OR_OP","NOT_OP","XOR_OP","MOD_OP","IF","THEN","ELSE","ELSEIF","FI",
"WHILE","FORALL","FOR","BEGIN","END","WHENEVER","REF","INT","CHAR","BOOLEAN",
"REAL","INSTANCE","STRING","VOID","MANDATORY","STRUCT","NEW","DELETE","OF",
"WITH","TRUE","FALSE","YYERROR_VERBOSE",
};
char *yyrule[] = {
"$accept : langauge",
"langauge : application",
"langauge : object",
"langauge : system",
"application : '@' COPLE ':' COPLE_APP '!' VALID_NAME app_body",
"app_body : compound",
"app_body : error",
"object : '@' COPLE ':' COPLE_OBJ '!' VALID_NAME obj_body",
"$$1 :",
"obj_body : $$1 declaration_list madatory_def service_list",
"madatory_def : MANDATORY ':' VALID_NAME_list",
"system : '@' COPLE ':' COPLE_SYS '!' VALID_NAME sys_body",
"sys_body : compound",
"sys_body : error",
"int_list : int_list ',' INTEGER",
"int_list : INTEGER",
"md_list : md_list ',' sd_list",
"md_list : sd_list",
"sd_list : '{' const_list '}'",
"sd_list : '{' md_list '}'",
"const_list : const_list ',' constant",
"const_list : constant",
"statement_list : statement_list statement",
"statement_list : statement",
"declaration_list : declaration_list declaration",
"declaration_list : declaration",
"service_list : service_list service_def",
"service_list : service_def",
"VALID_NAME_list : VALID_NAME_list ',' VALID_NAME",
"VALID_NAME_list : VALID_NAME",
"lookup_list : lookup_list ',' lookup_entry",
"lookup_list : lookup_entry",
"lookup_entry : VALID_NAME '=' term",
"expression_list : expression_list ',' expression",
"expression_list : expression",
"mixed_arg_list : mixed_arg_list ',' name_argument",
"mixed_arg_list : pos_arg_list ',' name_argument",
"mixed_arg_list : name_argument",
"pos_arg_list : pos_arg_list ',' pos_argument",
"pos_arg_list : pos_argument",
"name_argument : VALID_NAME '=' expression",
"pos_argument : expression",
"declaration : struct_def",
"declaration : instance_def",
"declaration : array_def",
"declaration : standard_def",
"declaration : memory_def",
"struct_def : STRUCT VALID_NAME '{' declaration '}'",
"array_def : type_def VALID_NAME '[' ']' '=' sd_list",
"array_def : type_def VALID_NAME '[' int_list ']'",
"standard_def : type_def VALID_NAME '=' constant",
"standard_def : type_def VALID_NAME",
"instance_def : INSTANCE VALID_NAME OF object_name",
"memory_def : REF type_def VALID_NAME",
"memory_def : REF type_def VALID_NAME '=' memory_ref",
"memory_def : REF type_def VALID_NAME '[' int_list ']'",
"memory_ref : '&' VALID_NAME",
"memory_ref : '&' INTEGER",
"statement : compound",
"statement : for_loop",
"statement : assignment",
"statement : forall_loop",
"statement : while_loop",
"statement : if_statement",
"statement : system_call",
"statement : service_call",
"statement : whenever_stat",
"$$2 :",
"compound : BEGIN $$2 declaration_list statement_list END",
"$$3 :",
"compound : BEGIN $$3 statement_list END",
"whenever_stat : WHENEVER VALID_NAME THEN statement",
"for_loop : FOR '(' assignment ';' expression ';' expression ')' compound",
"for_loop : FOR '(' ';' expression ';' expression ')' compound",
"for_loop : FOR '(' ';' expression ';' ')' compound",
"for_loop : FOR '(' assignment ';' expression ';' ')' compound",
"assignment : item ASSIGN_OP expression",
"assignment : '$' VALID_NAME ASSIGN_OP object_name",
"assignment : '$' VALID_NAME ASSIGN_OP '$' VALID_NAME",
"assignment : item ASSIGN_OP memory_ref",
"assignment : memory_ref ASSIGN_OP expression",
"forall_loop : FORALL VALID_NAME compound",
"forall_loop : FORALL VALID_NAME WITH lookup_list compound",
"while_loop : WHILE '(' expression ')' compound",
"if_statement : IF expression THEN statement esif_statement FI",
"if_statement : IF expression THEN statement ELSE statement FI",
"if_statement : IF expression THEN statement FI",
"esif_statement : ELSEIF expression THEN statement",
"esif_statement : ELSEIF expression THEN statement ELSE statement",
"esif_statement : ELSEIF expression THEN statement esif_statement",
"expression : boolean_exp",
"expression : logical_exp",
"expression : arith_exp",
"expression : term",
"boolean_exp : term boolean_op term",
"boolean_exp : NOT_OP term",
"arith_exp : term arith_op term",
"logical_exp : term logical_op term",
"term : '(' expression ')'",
"term : item",
"term : constant",
"boolean_op : AND_OP",
"boolean_op : OR_OP",
"boolean_op : XOR_OP",
"logical_op : LE_OP",
"logical_op : LT_OP",
"logical_op : GE_OP",
"logical_op : GT_OP",
"logical_op : NE_OP",
"logical_op : EQ_OP",
"arith_op : '*'",
"arith_op : '%'",
"arith_op : '/'",
"arith_op : '+'",
"arith_op : '-'",
"item : VALID_NAME",
"item : VALID_NAME '[' expression_list ']'",
"constant : int_const",
"constant : char_const",
"constant : float_const",
"constant : boolean_const",
"constant : STRING_CONST",
"int_const : '-' INTEGER",
"int_const : INTEGER",
"float_const : INTEGER '.' INTEGER",
"char_const : CHARACTER",
"boolean_const : TRUE",
"boolean_const : FALSE",
"type_def : CHAR",
"type_def : BOOLEAN",
"type_def : REAL",
"type_def : INT",
"type_def : STRING",
"object_name : VALID_NAME",
"object_name : VALID_NAME '#' INTEGER",
"object_name : VALID_NAME ':' VALID_NAME",
"object_name : VALID_NAME ':' VALID_NAME '#' INTEGER",
"system_call : COPLE_SYS ':' VALID_NAME '(' mixed_arg_list ')'",
"system_call : COPLE_SYS ':' VALID_NAME '(' pos_arg_list ')'",
"service_call : object_name '.' VALID_NAME '(' mixed_arg_list ')'",
"service_call : object_name '.' VALID_NAME '(' pos_arg_list ')'",
"service_call : '$' VALID_NAME '.' VALID_NAME '(' mixed_arg_list ')'",
"service_call : '$' VALID_NAME '.' VALID_NAME '(' pos_arg_list ')'",
"service_call : VALID_NAME '(' mixed_arg_list ')'",
"service_call : VALID_NAME '(' pos_arg_list ')'",
"service_def : service_hdr compound",
"service_hdr : VALID_NAME '(' param_list ')'",
"param_list : VOID",
"param_list : param_def_list",
"param_def_list : param_def_list ',' type_def VALID_NAME",
"param_def_list : param_def_list ',' REF type_def VALID_NAME",
"param_def_list : param_def_list ',' type_def VALID_NAME '=' constant",
"param_def_list : type_def VALID_NAME",
"param_def_list : REF type_def VALID_NAME",
"param_def_list : type_def VALID_NAME '=' constant",
};
#endif
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 500
#define YYMAXDEPTH 500
#endif
#endif
int yydebug;
int yynerrs;
int yyerrflag;
int yychar;
short *yyssp;
YYSTYPE *yyvsp;
YYSTYPE yyval;
YYSTYPE yylval;
short yyss[YYSTACKSIZE];
YYSTYPE yyvs[YYSTACKSIZE];
#define yystacksize YYSTACKSIZE
#define YYABORT goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab
int
#if defined(__STDC__)
yyparse(void)
#else
yyparse()
#endif
{
    register int yym, yyn, yystate;
#if YYDEBUG
    register char *yys;
    extern char *getenv();

    if (yys = getenv("YYDEBUG"))
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = (-1);

    yyssp = yyss;
    yyvsp = yyvs;
    *yyssp = yystate = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yyssp >= yyss + yystacksize - 1)
        {
            goto yyoverflow;
        }
        *++yyssp = yystate = yytable[yyn];
        *++yyvsp = yylval;
        yychar = (-1);
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;
    yyerror("syntax error");
#ifdef lint
    goto yyerrlab;
#endif
yyerrlab:
    ++yynerrs;
yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yyssp, yytable[yyn]);
#endif
                if (yyssp >= yyss + yystacksize - 1)
                {
                    goto yyoverflow;
                }
                *++yyssp = yystate = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yyssp);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = (-1);
        goto yyloop;
    }
yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    yyval = yyvsp[1-yym];
    switch (yyn)
    {
case 4:
#line 139 "cople.y"
{ tree_root = yyvsp[0].clause;
					program_type = p_application;
					prog_name = (char*) malloc(sizeof(yyvsp[-1].char_string)+1);
					strcpy(prog_name,yyvsp[-1].char_string);}
break;
case 5:
#line 145 "cople.y"
{}
break;
case 6:
#line 146 "cople.y"
{}
break;
case 7:
#line 149 "cople.y"
{ tree_root = yyvsp[0].clause;
					program_type = p_object;
					prog_name = (char*) malloc(sizeof(yyvsp[-1].char_string)+1);
					strcpy(prog_name,yyvsp[-1].char_string);}
break;
case 8:
#line 155 "cople.y"
{new_level();}
break;
case 9:
#line 156 "cople.y"
{ yyval.clause = end_level();}
break;
case 11:
#line 162 "cople.y"
{ tree_root = yyvsp[0].clause;
					program_type = p_system;
					prog_name = (char*) malloc(sizeof(yyvsp[-1].char_string)+1);
					strcpy(prog_name,yyvsp[-1].char_string);}
break;
case 12:
#line 168 "cople.y"
{}
break;
case 13:
#line 169 "cople.y"
{}
break;
case 14:
#line 182 "cople.y"
{ yyval.list_type = yyvsp[-2].list_type; add_to_list(yyvsp[-2].list_type, create_int_const(yyvsp[0].int_number));}
break;
case 15:
#line 183 "cople.y"
{ yyval.list_type = create_list(l_int); add_to_list(yyval.list_type,create_int_const(yyvsp[0].int_number));}
break;
case 16:
#line 186 "cople.y"
{ yyval.list_type = yyvsp[-2].list_type; add_to_list(yyvsp[-2].list_type,create_list_const(yyvsp[0].list_type));}
break;
case 17:
#line 187 "cople.y"
{ yyval.list_type = create_list(l_list); add_to_list(yyval.list_type,create_list_const(yyvsp[0].list_type));}
break;
case 18:
#line 190 "cople.y"
{ yyval.list_type = yyvsp[-1].list_type;}
break;
case 19:
#line 191 "cople.y"
{ yyval.list_type = yyvsp[-1].list_type;}
break;
case 20:
#line 194 "cople.y"
{ yyval.list_type = yyvsp[-2].list_type; add_to_list(yyvsp[-2].list_type, yyvsp[0].const_type);}
break;
case 21:
#line 195 "cople.y"
{ yyval.list_type = create_list(l_const); add_to_list(yyval.list_type,yyvsp[0].const_type);}
break;
case 22:
#line 198 "cople.y"
{ add_clause(yyvsp[0].clause);}
break;
case 23:
#line 199 "cople.y"
{ add_clause(yyvsp[0].clause);}
break;
case 24:
#line 202 "cople.y"
{ }
break;
case 25:
#line 203 "cople.y"
{ }
break;
case 26:
#line 206 "cople.y"
{ add_clause(yyvsp[0].clause);}
break;
case 27:
#line 207 "cople.y"
{ add_clause(yyvsp[0].clause);}
break;
case 28:
#line 210 "cople.y"
{ yyval.list_type = yyvsp[-2].list_type; add_to_list(yyvsp[-2].list_type, create_sym_ref(yyvsp[0].char_string));}
break;
case 29:
#line 211 "cople.y"
{ yyval.list_type = create_list(l_name); add_to_list(yyval.list_type,create_sym_ref(yyvsp[0].char_string));}
break;
case 30:
#line 214 "cople.y"
{ yyval.list_type = yyvsp[-2].list_type; add_to_list(yyvsp[-2].list_type,yyvsp[0].const_type);}
break;
case 31:
#line 215 "cople.y"
{ yyval.list_type = create_list(l_lookup); add_to_list(yyval.list_type,yyvsp[0].const_type);}
break;
case 32:
#line 218 "cople.y"
{ yyval.const_type = create_lookup_const(create_str_const(yyvsp[-2].char_string),yyvsp[0].clause);}
break;
case 33:
#line 221 "cople.y"
{ yyval.list_type = yyvsp[-2].list_type; add_to_list(yyval.list_type, create_expr_const(yyvsp[0].clause));}
break;
case 34:
#line 222 "cople.y"
{ yyval.list_type = create_list(l_expr); add_to_list(yyval.list_type,create_expr_const(yyvsp[0].clause));}
break;
case 35:
#line 236 "cople.y"
{ yyval.list_type = yyvsp[-2].list_type; add_to_list(yyvsp[-2].list_type, yyvsp[0].const_type);}
break;
case 36:
#line 237 "cople.y"
{ yyval.list_type = yyvsp[-2].list_type; add_to_list(yyvsp[-2].list_type, yyvsp[0].const_type);}
break;
case 37:
#line 238 "cople.y"
{ yyval.list_type = create_list(l_args); add_to_list(yyval.list_type,yyvsp[0].const_type);}
break;
case 38:
#line 241 "cople.y"
{ yyval.list_type = yyvsp[-2].list_type; add_to_list(yyvsp[-2].list_type, yyvsp[0].const_type);}
break;
case 39:
#line 242 "cople.y"
{ yyval.list_type = create_list(l_args); add_to_list(yyval.list_type,yyvsp[0].const_type);}
break;
case 40:
#line 245 "cople.y"
{ yyval.const_type = create_arg_const(get_type(yyvsp[0].clause),create_str_const(yyvsp[-2].char_string),yyvsp[0].clause);}
break;
case 41:
#line 248 "cople.y"
{ yyval.const_type = create_arg_const(get_type(yyvsp[0].clause),0,yyvsp[0].clause);}
break;
case 42:
#line 260 "cople.y"
{}
break;
case 43:
#line 261 "cople.y"
{}
break;
case 44:
#line 262 "cople.y"
{}
break;
case 45:
#line 263 "cople.y"
{}
break;
case 46:
#line 264 "cople.y"
{}
break;
case 47:
#line 267 "cople.y"
{}
break;
case 48:
#line 270 "cople.y"
{ conv_const_list_to_array(yyvsp[0].list_type,yyvsp[-4].char_string,yyvsp[-5].int_number,a_var);}
break;
case 49:
#line 271 "cople.y"
{ conv_list_to_array(yyvsp[-1].list_type,yyvsp[-3].char_string,yyvsp[-4].int_number,a_var);}
break;
case 50:
#line 274 "cople.y"
{ add_variable(yyvsp[-2].char_string,yyvsp[-3].int_number,yyvsp[0].const_type,a_var);}
break;
case 51:
#line 275 "cople.y"
{ add_variable(yyvsp[0].char_string,yyvsp[-1].int_number,NULL,a_var);}
break;
case 52:
#line 278 "cople.y"
{ add_variable(yyvsp[-2].char_string,d_instance,yyvsp[0].const_type,a_inst);}
break;
case 53:
#line 281 "cople.y"
{ add_variable(yyvsp[0].char_string,yyvsp[-1].int_number,NULL,a_ptr);}
break;
case 54:
#line 282 "cople.y"
{ add_variable(yyvsp[-2].char_string,yyvsp[-3].int_number,yyvsp[0].const_type,a_ptr);}
break;
case 55:
#line 283 "cople.y"
{ conv_list_to_array(yyvsp[-1].list_type,yyvsp[-3].char_string,yyvsp[-4].int_number,a_ptr);}
break;
case 56:
#line 286 "cople.y"
{ yyval.const_type = create_sym_ref(yyvsp[0].char_string);}
break;
case 57:
#line 287 "cople.y"
{ yyval.const_type = create_ref_const(yyvsp[0].int_number);}
break;
case 67:
#line 309 "cople.y"
{ new_level(); }
break;
case 68:
#line 312 "cople.y"
{ yyval.clause = end_level();}
break;
case 69:
#line 313 "cople.y"
{ new_level(); }
break;
case 70:
#line 315 "cople.y"
{ yyval.clause = end_level();}
break;
case 71:
#line 322 "cople.y"
{ yyval.clause = new_when_clause(yyvsp[-2].char_string,yyvsp[0].clause);}
break;
case 72:
#line 325 "cople.y"
{ yyval.clause = new_for_clause(yyvsp[-6].clause,yyvsp[-4].clause,yyvsp[-2].clause,yyvsp[0].clause);}
break;
case 73:
#line 326 "cople.y"
{ yyval.clause = new_for_clause(NULL,yyvsp[-4].clause,yyvsp[-2].clause,yyvsp[0].clause);}
break;
case 74:
#line 327 "cople.y"
{ yyval.clause = new_for_clause(NULL,yyvsp[-3].clause,NULL,yyvsp[0].clause);}
break;
case 75:
#line 328 "cople.y"
{ yyval.clause = new_for_clause(yyvsp[-5].clause,yyvsp[-3].clause,NULL,yyvsp[0].clause);}
break;
case 76:
#line 331 "cople.y"
{yyval.clause = new_binary_clause(store,yyvsp[-2].clause,yyvsp[0].clause);}
break;
case 77:
#line 332 "cople.y"
{yyval.clause = new_binary_clause(store,find_operand(yyvsp[-2].char_string,d_instance),new_const_clause(yyvsp[0].const_type));}
break;
case 78:
#line 333 "cople.y"
{yyval.clause = new_binary_clause(store,find_operand(yyvsp[-3].char_string,d_instance),find_operand(yyvsp[0].char_string,d_instance));}
break;
case 79:
#line 334 "cople.y"
{yyval.clause = new_binary_clause(store,yyvsp[-2].clause,new_const_clause(yyvsp[0].const_type));}
break;
case 80:
#line 335 "cople.y"
{yyval.clause = new_binary_clause(store,new_const_clause(yyvsp[-2].const_type),yyvsp[0].clause);}
break;
case 81:
#line 338 "cople.y"
{ yyval.clause = new_forall_clause(NULL,find_operand(yyvsp[-1].char_string,d_instance),yyvsp[0].clause);}
break;
case 82:
#line 339 "cople.y"
{ yyval.clause = new_forall_clause(yyvsp[-1].list_type,find_operand(yyvsp[-3].char_string,d_instance),yyvsp[0].clause);}
break;
case 83:
#line 342 "cople.y"
{ yyval.clause = new_while_clause(yyvsp[-2].clause,yyvsp[0].clause);}
break;
case 84:
#line 345 "cople.y"
{ yyval.clause = new_if_clause(yyvsp[-4].clause,yyvsp[-2].clause,yyvsp[-1].clause);}
break;
case 85:
#line 346 "cople.y"
{ yyval.clause = new_if_clause(yyvsp[-5].clause,yyvsp[-3].clause,yyvsp[-1].clause);}
break;
case 86:
#line 347 "cople.y"
{ yyval.clause = new_test_clause(yyvsp[-3].clause,yyvsp[-1].clause);}
break;
case 87:
#line 350 "cople.y"
{ yyval.clause = new_test_clause(yyvsp[-2].clause,yyvsp[0].clause); }
break;
case 88:
#line 351 "cople.y"
{ yyval.clause = new_if_clause(yyvsp[-4].clause,yyvsp[-2].clause,yyvsp[0].clause); }
break;
case 89:
#line 352 "cople.y"
{ yyval.clause = new_if_clause(yyvsp[-3].clause,yyvsp[-1].clause,yyvsp[0].clause); }
break;
case 94:
#line 365 "cople.y"
{ yyval.clause = new_binary_clause(yyvsp[-1].instr_type,yyvsp[-2].clause,yyvsp[0].clause);}
break;
case 95:
#line 366 "cople.y"
{ yyval.clause = new_unary_clause(not,yyvsp[0].clause);}
break;
case 96:
#line 369 "cople.y"
{ yyval.clause = new_binary_clause(yyvsp[-1].instr_type,yyvsp[-2].clause,yyvsp[0].clause);}
break;
case 97:
#line 372 "cople.y"
{ yyval.clause = new_binary_clause(yyvsp[-1].instr_type,yyvsp[-2].clause,yyvsp[0].clause);}
break;
case 98:
#line 375 "cople.y"
{ yyval.clause = yyvsp[-1].clause;}
break;
case 99:
#line 376 "cople.y"
{ yyval.clause = yyvsp[0].clause;}
break;
case 100:
#line 377 "cople.y"
{ yyval.clause = new_const_clause(yyvsp[0].const_type); }
break;
case 101:
#line 384 "cople.y"
{ yyval.instr_type = and; }
break;
case 102:
#line 385 "cople.y"
{ yyval.instr_type = or;  }
break;
case 103:
#line 386 "cople.y"
{ yyval.instr_type = xor; }
break;
case 104:
#line 389 "cople.y"
{ yyval.instr_type = less_equal; }
break;
case 105:
#line 390 "cople.y"
{ yyval.instr_type = less_than; }
break;
case 106:
#line 391 "cople.y"
{ yyval.instr_type = greater_equal; }
break;
case 107:
#line 392 "cople.y"
{ yyval.instr_type = greater_than; }
break;
case 108:
#line 393 "cople.y"
{ yyval.instr_type = not_eq; }
break;
case 109:
#line 394 "cople.y"
{ yyval.instr_type = equal; }
break;
case 110:
#line 397 "cople.y"
{ yyval.instr_type = multi; }
break;
case 111:
#line 398 "cople.y"
{ yyval.instr_type = modulo; }
break;
case 112:
#line 399 "cople.y"
{ yyval.instr_type = divide; }
break;
case 113:
#line 400 "cople.y"
{ yyval.instr_type = add; }
break;
case 114:
#line 401 "cople.y"
{ yyval.instr_type = sub; }
break;
case 115:
#line 404 "cople.y"
{ yyval.clause = find_operand(yyvsp[0].char_string,d_null);}
break;
case 116:
#line 405 "cople.y"
{ yyval.clause = new_array_clause(find_operand(yyvsp[-3].char_string,d_array),yyvsp[-1].list_type);}
break;
case 117:
#line 408 "cople.y"
{ yyval.const_type = create_int_const(yyvsp[0].int_number); }
break;
case 118:
#line 409 "cople.y"
{ yyval.const_type = create_char_const(yyvsp[0].character); }
break;
case 119:
#line 410 "cople.y"
{ yyval.const_type = create_float_const(yyvsp[0].float_number); }
break;
case 120:
#line 411 "cople.y"
{ yyval.const_type = create_bool_const(yyvsp[0].int_number); }
break;
case 121:
#line 412 "cople.y"
{ yyval.const_type = create_str_const(yyvsp[0].char_string); }
break;
case 122:
#line 415 "cople.y"
{ yyval.int_number = 0 - yyvsp[0].int_number; }
break;
case 123:
#line 416 "cople.y"
{ yyval.int_number = yyvsp[0].int_number; }
break;
case 124:
#line 419 "cople.y"
{ yyval.float_number = yyvsp[-2].int_number; yyval.float_number += (1 / yyvsp[0].int_number);}
break;
case 125:
#line 421 "cople.y"
{ yyval.character = yyvsp[0].character; }
break;
case 126:
#line 423 "cople.y"
{ yyval.int_number = 1; }
break;
case 127:
#line 424 "cople.y"
{ yyval.int_number = 0; }
break;
case 128:
#line 427 "cople.y"
{ yyval.int_number = d_char; }
break;
case 129:
#line 428 "cople.y"
{ yyval.int_number = d_boolean; }
break;
case 130:
#line 429 "cople.y"
{ yyval.int_number = d_real; }
break;
case 131:
#line 430 "cople.y"
{ yyval.int_number = d_int; }
break;
case 132:
#line 431 "cople.y"
{ yyval.int_number = d_string; }
break;
case 133:
#line 434 "cople.y"
{ yyval.const_type = create_inst_const(NULL,yyvsp[0].char_string,0);}
break;
case 134:
#line 435 "cople.y"
{ yyval.const_type = create_inst_const(NULL,yyvsp[-2].char_string,yyvsp[0].int_number);}
break;
case 135:
#line 436 "cople.y"
{ yyval.const_type = create_inst_const(yyvsp[-2].char_string,yyvsp[0].char_string,0);}
break;
case 136:
#line 437 "cople.y"
{ yyval.const_type = create_inst_const(yyvsp[-4].char_string,yyvsp[-2].char_string,yyvsp[0].int_number);}
break;
case 137:
#line 444 "cople.y"
{ yyval.clause = new_call_clause(system_pnt,create_str_const(yyvsp[-3].char_string),yyvsp[-1].list_type);}
break;
case 138:
#line 445 "cople.y"
{ yyval.clause = new_call_clause(system_pnt,create_str_const(yyvsp[-3].char_string),yyvsp[-1].list_type);}
break;
case 139:
#line 448 "cople.y"
{ yyval.clause = new_call_clause(new_const_clause(yyvsp[-5].const_type),create_str_const(yyvsp[-3].char_string),yyvsp[-1].list_type);}
break;
case 140:
#line 449 "cople.y"
{ yyval.clause = new_call_clause(new_const_clause(yyvsp[-5].const_type),create_str_const(yyvsp[-3].char_string),yyvsp[-1].list_type);}
break;
case 141:
#line 450 "cople.y"
{ yyval.clause = new_call_clause(find_operand(yyvsp[-5].char_string,d_instance),create_str_const(yyvsp[-3].char_string),yyvsp[-1].list_type);}
break;
case 142:
#line 451 "cople.y"
{ yyval.clause = new_call_clause(find_operand(yyvsp[-5].char_string,d_instance),create_str_const(yyvsp[-3].char_string),yyvsp[-1].list_type);}
break;
case 143:
#line 452 "cople.y"
{ yyval.clause = new_call_clause(NULL,create_str_const(yyvsp[-3].char_string),yyvsp[-1].list_type);}
break;
case 144:
#line 453 "cople.y"
{ yyval.clause = new_call_clause(NULL,create_str_const(yyvsp[-3].char_string),yyvsp[-1].list_type);}
break;
case 145:
#line 456 "cople.y"
{ yyval.clause = end_service_level(yyvsp[0].clause);}
break;
case 146:
#line 459 "cople.y"
{ new_service_entry(add_variable(yyvsp[-3].char_string,d_service,NULL,a_service),yyvsp[-1].parameter_list);}
break;
case 147:
#line 462 "cople.y"
{ yyval.parameter_list = NULL;}
break;
case 148:
#line 463 "cople.y"
{ yyval.parameter_list = yyvsp[0].parameter_list;}
break;
case 149:
#line 466 "cople.y"
{ yyval.parameter_list = yyvsp[-3].parameter_list; add_parameter(yyvsp[-3].parameter_list,yyvsp[-1].int_number,yyvsp[0].char_string,NULL,0);}
break;
case 150:
#line 467 "cople.y"
{ yyval.parameter_list = yyvsp[-4].parameter_list; add_parameter(yyvsp[-4].parameter_list,yyvsp[-1].int_number,yyvsp[0].char_string,NULL,1);}
break;
case 151:
#line 468 "cople.y"
{ yyval.parameter_list = yyvsp[-5].parameter_list; add_parameter(yyvsp[-5].parameter_list,yyvsp[-3].int_number,yyvsp[-2].char_string,yyvsp[0].const_type,0);}
break;
case 152:
#line 469 "cople.y"
{ yyval.parameter_list = new_param_list();add_parameter(yyval.parameter_list,yyvsp[-1].int_number,yyvsp[0].char_string,NULL,0);}
break;
case 153:
#line 470 "cople.y"
{ yyval.parameter_list = new_param_list();add_parameter(yyval.parameter_list,yyvsp[-1].int_number,yyvsp[0].char_string,NULL,1);}
break;
case 154:
#line 471 "cople.y"
{ yyval.parameter_list = new_param_list();add_parameter(yyval.parameter_list,yyvsp[-3].int_number,yyvsp[-2].char_string,yyvsp[0].const_type,0);}
break;
#line 1347 "cople.tab.c"
    }
    yyssp -= yym;
    yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (yychar < 0)
        {
            if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yyssp, yystate);
#endif
    if (yyssp >= yyss + yystacksize - 1)
    {
        goto yyoverflow;
    }
    *++yyssp = yystate;
    *++yyvsp = yyval;
    goto yyloop;
yyoverflow:
    yyerror("yacc stack overflow");
yyabort:
    return (1);
yyaccept:
    return (0);
}
