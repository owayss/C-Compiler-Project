%{
    #include<iostream>
    #include<string>

    using namespace std;
    class Node;
    #include "parser.tab.h"
%}

%%

[ \n\t\r]+                  ;

"+" |
"-" |
"*" |
"/" |
"=" |
"&" |
"(" |
")" |
"{" |
"}" |
","                   {return yytext[0];}

[0-9]+                  {yylval.num = atoi(yytext); return NUMBER;}

"char"          {return (CHAR);}
"int"           {return (INT);}
"float"         {return (FLOAT);}

[a-zA-Z_]+      {yylval.name = new string(yytext); return (ID);}


";"                     {return EOL;}
.                           cout << "Error lexico, caracter (" << yytext << ") desconocido\n";
