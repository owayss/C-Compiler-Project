%{
    #include<iostream>
    #include<string>

    using namespace std;
    class Node;
    #include "parser.tab.h"
%}

%%

[ \n\t\r]+                  ;

"||"			{ yylval.name = new string(yytext); return(OR_OP); }
"&&"			{ yylval.name = new string(yytext); return(AND_OP);}
"=="			{ yylval.name = new string(yytext); return(EQ_OP); }
"!="			{ yylval.name = new string(yytext); return(NE_OP); }
"<="			{ yylval.name = new string(yytext); return(LE_OP); }
">="			{ yylval.name = new string(yytext); return(GE_OP); }

"+="			{ yylval.name = new string(yytext); return(ADD_ASSIGN); }
"-="			{ yylval.name = new string(yytext); return(SUB_ASSIGN); }
"*="			{ yylval.name = new string(yytext); return(MUL_ASSIGN); }
"/="			{ yylval.name = new string(yytext); return(DIV_ASSIGN); }
"="			  { yylval.name = new string(yytext); return(ASSIGN); }


"+" |
"-" |
"*" |
"/" |
"&" |
"(" |
")" |
"{" |
"}" |
"<" |
">" |
","                   {return yytext[0];}

[0-9]+                  {yylval.num = atoi(yytext); return NUMBER;}

"char"          {return (CHAR);}
"int"           {return (INT);}
"float"         {return (FLOAT);}

[a-zA-Z_]+      {yylval.name = new string(yytext); return (ID);}


";"                     {return EOL;}
.                           cout << "Error lexico, caracter (" << yytext << ") desconocido\n";
