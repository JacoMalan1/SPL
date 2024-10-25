#ifndef SPL_PARSER_FILE_HANDLER_H
#define SPL_PARSER_FILE_HANDLER_H

#include <map>
#include <string>
#include <vector>

class ParserFileHandler
{
public:
    ParserFileHandler();
    std::map<std::string, std::map<std::string, std::string>> getParseTable() const;
    std::vector<std::pair<std::string, std::vector<std::string>>> getGrammarRules() const;

private:
    void loadParseTable();
    void loadGrammarRules();
    std::map<std::string, std::map<std::string, std::string>> parseTable;
    std::vector<std::pair<std::string, std::vector<std::string>>> grammarRules;
};

static const std::string GRAMMAR_RULES_INPUT = R"(S -> PROG
PROG -> main GLOBVARS ALGO FUNCTIONS
GLOBVARS -> ''
GLOBVARS -> VTYP VNAME , GLOBVARS
VTYP -> num
VTYP -> text
VNAME -> varname
ALGO -> begin INSTRUC end
INSTRUC -> ''
INSTRUC -> COMMAND ; INSTRUC
COMMAND -> skip
COMMAND -> halt
COMMAND -> print ATOMIC
COMMAND -> ASSIGN
COMMAND -> CALL
COMMAND -> BRANCH
COMMAND -> return ATOMIC
ATOMIC -> VNAME
ATOMIC -> CONST
CONST -> numliteral
CONST -> textliteral
ASSIGN -> VNAME <input
ASSIGN -> VNAME = TERM
CALL -> FNAME ( ATOMIC , ATOMIC , ATOMIC )
BRANCH -> if COND then ALGO else ALGO
TERM -> ATOMIC
TERM -> CALL
TERM -> OP
OP -> UNOP ( ARG )
OP -> BINOP ( ARG , ARG )
ARG -> ATOMIC
ARG -> OP
COND -> SIMPLE
COND -> COMPOSIT
SIMPLE -> BINOP ( ATOMIC , ATOMIC )
COMPOSIT -> BINOP ( SIMPLE , SIMPLE )
COMPOSIT -> UNOP ( SIMPLE )
UNOP -> not
UNOP -> sqrt
BINOP -> or
BINOP -> and
BINOP -> eq
BINOP -> grt
BINOP -> add
BINOP -> sub
BINOP -> mul
BINOP -> div
FNAME -> fname
FUNCTIONS -> ''
FUNCTIONS -> DECL FUNCTIONS
DECL -> HEADER BODY
HEADER -> FTYP FNAME ( VNAME , VNAME , VNAME )
FTYP -> num
FTYP -> void
BODY -> PROLOG LOCVARS ALGO EPILOG SUBFUNCS end
PROLOG -> {
EPILOG -> }
LOCVARS -> VTYP VNAME , VTYP VNAME , VTYP VNAME ,
SUBFUNCS -> FUNCTIONS)";

static const std::string PARSE_TABLE_INPUT = R"(State,ACTION,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,GOTO,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
,main,",",num,text,varname,begin,end,;,skip,halt,print,return,numliteral,textliteral,<input,=,(,),if,then,else,not,sqrt,or,and,eq,grt,add,sub,mul,div,fname,void,{,},$,S,PROG,GLOBVARS,VTYP,VNAME,ALGO,INSTRUC,COMMAND,ATOMIC,CONST,ASSIGN,CALL,BRANCH,TERM,OP,ARG,COND,SIMPLE,COMPOSIT,UNOP,BINOP,FNAME,FUNCTIONS,DECL,HEADER,FTYP,BODY,PROLOG,EPILOG,LOCVARS,SUBFUNCS
0,s2,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,1,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
1,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,acc,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
2,,,s5,s6,,r2,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,3,4,,,,,,,,,,,,,,,,,,,,,,,,,,,
3,,,,,,s8,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,7,,,,,,,,,,,,,,,,,,,,,,,,,
4,,,,,s10,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,9,,,,,,,,,,,,,,,,,,,,,,,,,,
5,,,,,r4,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
6,,,,,r5,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
7,,,s15,,,,r48,,,,,,,,,,,,,,,,,,,,,,,,,,s16,,,r48,,,,,,,,,,,,,,,,,,,,,,,11,12,13,14,,,,,
8,,,,,s10,,r8,,s19,s20,s21,s25,,,,,,,s28,,,,,,,,,,,,,s29,,,,,,,,,26,,17,18,,,22,23,24,,,,,,,,,27,,,,,,,,,
9,,s30,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
10,,r6,,,,,,r6,,,,,,,r6,r6,,r6,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
11,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,r1,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
12,,,s15,,,,r48,,,,,,,,,,,,,,,,,,,,,,,,,,s16,,,r48,,,,,,,,,,,,,,,,,,,,,,,31,12,13,14,,,,,
13,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,s34,,,,,,,,,,,,,,,,,,,,,,,,,,,,,32,33,,,
14,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,s29,,,,,,,,,,,,,,,,,,,,,,,,,,35,,,,,,,,,
15,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,r52,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
16,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,r53,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
17,,,,,,,s36,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
18,,,,,,,r9,s37,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
19,,,,,,,,r10,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
20,,,,,,,,r11,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
21,,,,,s10,,,,,,,,s41,s42,,,,,,,,,,,,,,,,,,,,,,,,,,,39,,,,38,40,,,,,,,,,,,,,,,,,,,,,
22,,,,,,,,r13,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
23,,,,,,,,r14,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
24,,,,,,,r15,r15,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
25,,,,,s10,,,,,,,,s41,s42,,,,,,,,,,,,,,,,,,,,,,,,,,,39,,,,43,40,,,,,,,,,,,,,,,,,,,,,
26,,,,,,,,,,,,,,,s44,s45,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
27,,,,,,,,,,,,,,,,,s46,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
28,,,,,,,,,,,,,,,,,,,,,,s60,s61,s52,s53,s54,s55,s56,s57,s58,s59,,,,,,,,,,,,,,,,,,,,,,47,48,49,51,50,,,,,,,,,,
29,,,,,,,,,,,,,,,,,r47,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
30,,,s5,s6,,r2,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,62,4,,,,,,,,,,,,,,,,,,,,,,,,,,,
31,,,,,,,r49,,,,,,,,,,,,,,,,,,,,,,,,,,,,,r49,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
32,,,r50,,,,r50,,,,,,,,,,,,,,,,,,,,,,,,,,r50,,,r50,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
33,,,s5,s6,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,64,,,,,,,,,,,,,,,,,,,,,,,,,,63,
34,,,r55,r55,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
35,,,,,,,,,,,,,,,,,s65,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
36,,,r7,,,,r7,r7,,,,,,,,,,,,,r7,,,,,,,,,,,,r7,,r7,r7,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
37,,,,,s10,,r8,,s19,s20,s21,s25,,,,,,,s28,,,,,,,,,,,,,s29,,,,,,,,,26,,66,18,,,22,23,24,,,,,,,,,27,,,,,,,,,
38,,,,,,,,r12,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
39,,r17,,,,,,r17,,,,,,,,,,r17,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
40,,r18,,,,,,r18,,,,,,,,,,r18,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
41,,r19,,,,,,r19,,,,,,,,,,r19,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
42,,r20,,,,,,r20,,,,,,,,,,r20,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
43,,,,,,,,r16,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
44,,,,,,,,r21,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
45,,,,,s10,,,,,,,,s41,s42,,,,,,,,s60,s61,s52,s53,s54,s55,s56,s57,s58,s59,s29,,,,,,,,,39,,,,68,40,,69,,67,70,,,,,71,72,27,,,,,,,,,
46,,,,,s10,,,,,,,,s41,s42,,,,,,,,,,,,,,,,,,,,,,,,,,,39,,,,73,40,,,,,,,,,,,,,,,,,,,,,
47,,,,,,,,,,,,,,,,,,,,s74,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
48,,,,,,,,,,,,,,,,,,,,r32,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
49,,,,,,,,,,,,,,,,,,,,r33,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
50,,,,,,,,,,,,,,,,,s75,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
51,,,,,,,,,,,,,,,,,s76,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
52,,,,,,,,,,,,,,,,,r39,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
53,,,,,,,,,,,,,,,,,r40,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
54,,,,,,,,,,,,,,,,,r41,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
55,,,,,,,,,,,,,,,,,r42,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
56,,,,,,,,,,,,,,,,,r43,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
57,,,,,,,,,,,,,,,,,r44,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
58,,,,,,,,,,,,,,,,,r45,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
59,,,,,,,,,,,,,,,,,r46,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
60,,,,,,,,,,,,,,,,,r37,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
61,,,,,,,,,,,,,,,,,r38,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
62,,,,,,r3,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
63,,,,,,s8,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,77,,,,,,,,,,,,,,,,,,,,,,,,,
64,,,,,s10,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,78,,,,,,,,,,,,,,,,,,,,,,,,,,
65,,,,,s10,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,79,,,,,,,,,,,,,,,,,,,,,,,,,,
66,,,,,,,r9,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
67,,,,,,,,r22,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
68,,,,,,,,r25,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
69,,,,,,,,r26,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
70,,,,,,,,r27,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
71,,,,,,,,,,,,,,,,,s80,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
72,,,,,,,,,,,,,,,,,s81,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
73,,s82,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
74,,,,,,s8,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,83,,,,,,,,,,,,,,,,,,,,,,,,,
75,,,,,s10,,,,,,,,s41,s42,,,,,,,,,,s52,s53,s54,s55,s56,s57,s58,s59,,,,,,,,,,39,,,,84,40,,,,,,,,85,,,86,,,,,,,,,,
76,,,,,,,,,,,,,,,,,,,,,,,,s52,s53,s54,s55,s56,s57,s58,s59,,,,,,,,,,,,,,,,,,,,,,,87,,,86,,,,,,,,,,
77,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,s89,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,88,,
78,,s90,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
79,,s91,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
80,,,,,s10,,,,,,,,s41,s42,,,,,,,,s60,s61,s52,s53,s54,s55,s56,s57,s58,s59,,,,,,,,,,39,,,,93,40,,,,,94,92,,,,71,72,,,,,,,,,,
81,,,,,s10,,,,,,,,s41,s42,,,,,,,,s60,s61,s52,s53,s54,s55,s56,s57,s58,s59,,,,,,,,,,39,,,,93,40,,,,,94,95,,,,71,72,,,,,,,,,,
82,,,,,s10,,,,,,,,s41,s42,,,,,,,,,,,,,,,,,,,,,,,,,,,39,,,,96,40,,,,,,,,,,,,,,,,,,,,,
83,,,,,,,,,,,,,,,,,,,,,s97,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
84,,s98,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
85,,s99,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
86,,,,,,,,,,,,,,,,,s100,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
87,,,,,,,,,,,,,,,,,,s101,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
88,,,s15,,,,r48,,,,,,,,,,,,,,,,,,,,,,,,,,s16,,,r48,,,,,,,,,,,,,,,,,,,,,,,103,12,13,14,,,,,102
89,,,r56,,,,r56,,,,,,,,,,,,,,,,,,,,,,,,,,r56,,,r56,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
90,,,s5,s6,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,104,,,,,,,,,,,,,,,,,,,,,,,,,,,
91,,,,,s10,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,105,,,,,,,,,,,,,,,,,,,,,,,,,,
92,,,,,,,,,,,,,,,,,,s106,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
93,,r30,,,,,,,,,,,,,,,,r30,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
94,,r31,,,,,,,,,,,,,,,,r31,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
95,,s107,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
96,,s108,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
97,,,,,,s8,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,109,,,,,,,,,,,,,,,,,,,,,,,,,
98,,,,,s10,,,,,,,,s41,s42,,,,,,,,,,,,,,,,,,,,,,,,,,,39,,,,110,40,,,,,,,,,,,,,,,,,,,,,
99,,,,,,,,,,,,,,,,,,,,,,,,s52,s53,s54,s55,s56,s57,s58,s59,,,,,,,,,,,,,,,,,,,,,,,111,,,86,,,,,,,,,,
100,,,,,s10,,,,,,,,s41,s42,,,,,,,,,,,,,,,,,,,,,,,,,,,39,,,,84,40,,,,,,,,,,,,,,,,,,,,,
101,,,,,,,,,,,,,,,,,,,,r36,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
102,,,,,,,s112,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
103,,,,,,,r58,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
104,,,,,s10,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,113,,,,,,,,,,,,,,,,,,,,,,,,,,
105,,s114,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
106,,r28,,,,,,r28,,,,,,,,,,r28,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
107,,,,,s10,,,,,,,,s41,s42,,,,,,,,s60,s61,s52,s53,s54,s55,s56,s57,s58,s59,,,,,,,,,,39,,,,93,40,,,,,94,115,,,,71,72,,,,,,,,,,
108,,,,,s10,,,,,,,,s41,s42,,,,,,,,,,,,,,,,,,,,,,,,,,,39,,,,116,40,,,,,,,,,,,,,,,,,,,,,
109,,,,,,,r24,r24,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
110,,,,,,,,,,,,,,,,,,s117,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
111,,,,,,,,,,,,,,,,,,s118,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
112,,,r54,,,,r54,,,,,,,,,,,,,,,,,,,,,,,,,,r54,,,r54,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
113,,s119,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
114,,,,,s10,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,120,,,,,,,,,,,,,,,,,,,,,,,,,,
115,,,,,,,,,,,,,,,,,,s121,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
116,,,,,,,,,,,,,,,,,,s122,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
117,,r34,,,,,,,,,,,,,,,,r34,,r34,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
118,,,,,,,,,,,,,,,,,,,,r35,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
119,,,s5,s6,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,123,,,,,,,,,,,,,,,,,,,,,,,,,,,
120,,,,,,,,,,,,,,,,,,s124,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
121,,r29,,,,,,r29,,,,,,,,,,r29,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
122,,,,,,,,r23,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
123,,,,,s10,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,125,,,,,,,,,,,,,,,,,,,,,,,,,,
124,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,r51,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
125,,s126,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
126,,,,,,r57,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
)";

#endif
