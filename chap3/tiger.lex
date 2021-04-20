%{
#include <string.h>
#include "util.h"
#include "y.tab.h"
#include "errormsg.h"

#define STRING_INIT_LENGTH 32

int charPos=1;

int comment_deep = 0;

int yywrap(void)
{
 charPos=1;
 return 1;
}


void adjust(void)
{
 EM_tokPos=charPos;
 charPos+=yyleng;
}

typedef struct string_Buffer_{
    string ptr;
    int capacity;
} *String_Buffer;

String_Buffer sb;

String_Buffer string_buffer(){
    String_Buffer sb = checked_malloc(sizeof(*sb));
    sb->ptr = checked_malloc(STRING_INIT_LENGTH);
    sb->ptr[0] = 0;
    sb->capacity = STRING_INIT_LENGTH;
    return sb;
}

void add_char_to_buffer(String_Buffer sb, char ch){
    int length = strlen(sb->ptr) + 1;
    if(length == sb->capacity){
        String_Buffer temp = checked_malloc(sizeof(*temp));
        temp->ptr = checked_malloc(sb->capacity * 2);
        memcpy(temp->ptr, sb->ptr, length);
        sb = temp;
    }
    sb->ptr[length - 1] = ch;
    sb->ptr[length] = 0;
}

%}

%x COMMENT STRING_STATUS

%%
[ \r\t]	 {adjust(); continue;}
\n	 {adjust(); EM_newline(); continue;}

","	 {adjust(); return COMMA;}
":"   {adjust(); return COLON;}
";"   {adjust(); return SEMICOLON;}
"("   {adjust(); return LPAREN;}
")"   {adjust(); return RPAREN;}
"["   {adjust(); return LBRACK;}
"]"   {adjust(); return RBRACK;}
"{"   {adjust(); return LBRACE;}
"}"   {adjust(); return RBRACE;}
"."   {adjust(); return DOT;}
"+"   {adjust(); return PLUS;}
"-"   {adjust(); return MINUS;}
"*"   {adjust(); return TIMES;}
"/"   {adjust(); return DIVIDE;}
"="   {adjust(); return EQ;}
"<>"  {adjust(); return NEQ;}
"<"   {adjust(); return LT;}
"<="  {adjust(); return LE;}
">"   {adjust(); return GT;}
">="  {adjust(); return GE;}
"&"   {adjust(); return AND;}
"|"   {adjust(); return OR;}
":="  {adjust(); return ASSIGN;}

for  	 {adjust(); return FOR;}
while    {adjust(); return WHILE;}
to        {adjust(); return TO;}
break     {adjust(); return BREAK;}
let       {adjust(); return LET;}
in        {adjust(); return IN;}
end       {adjust(); return END;}
function  {adjust(); return FUNCTION;}
var       {adjust(); return VAR;}
type      {adjust(); return TYPE;}
array     {adjust(); return ARRAY;}
if        {adjust(); return IF;}
then      {adjust(); return THEN;}
else      {adjust(); return ELSE;}
do        {adjust(); return DO;}
of        {adjust(); return OF;}
nil       {adjust(); return NIL;}

[0-9]+	 {adjust(); yylval.ival=atoi(yytext); return INT;}

[a-zA-Z]+[_0-9a-zA-Z]*  {adjust(); yylval.sval = strdup(yytext); return ID;}

"/*"    {adjust(); comment_deep++; BEGIN(COMMENT);}

"*/"    {adjust(); EM_error(EM_tokPos,"Found '*/' while there is no comment!");}

\"  {adjust(); sb = string_buffer(); BEGIN(STRING_STATUS);}

.	 {adjust(); EM_error(EM_tokPos,"illegal token");}


<COMMENT>{
    "*/" {
        adjust();
        comment_deep--;
        if(comment_deep == 0){
            BEGIN(INITIAL);
        }
    }

    \n {
        adjust(); EM_newline(); continue;
    }

    <<EOF>> {
        EM_error(EM_tokPos, "Comment not end at the end of file!");
    }

    . {
        adjust();
    }
}

<STRING_STATUS>{
    \"  {
        adjust();
        BEGIN(INITIAL);
        yylval.sval = strdup(sb->ptr);
        return STRING;
    }

    <<EOF>> {
        EM_error(EM_tokPos, "String not end at the end of file!");
    }

    "\\\"" {
        adjust();
        add_char_to_buffer(sb, '"');
    }

    . {
        adjust();
        string str = yytext;
        add_char_to_buffer(sb, *str);
    }

}