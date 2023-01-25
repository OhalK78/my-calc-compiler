/*
    Very Simple Compiler (Tokenizer part)
        $Id$
 */

#include "s-compile.h"

void error(char *);
int  token();

char *ptr,*last_ptr,*before;
int  value,lvalue;
int  last_token;

int
token()
{
    int c,d;

    last_ptr = ptr;  /* for error position */
    c= *ptr;
    if(!c) {
        last_token = EOF;
        return last_token;
    }
    ptr++;
    if (c<=' ') {       /* comment */ 
        while(*ptr++);
        ptr--;
        last_token = EOF;
        last_ptr = ptr;
        return last_token;
    }
    if('0'<=c && c<='9') {     /* Decimal or Octal or Hexadecimal*/
        if (c=='0' && *ptr){            /* Octal or Hexadecimal */
            c = *ptr;
            ptr++;
            if('0'<=c && c<='7'){                 /* Octal */
                d = 00 + c-'0';
                while((c= *ptr++)){
                    if('0'<=c && c<='7'){
                        d = d*8 + (c-'0');
                    }else{
                        break;
                    }
                }
            }else if(c=='x'){           /* Hexadecimal */
                d = 0x0;
                while((c= *ptr++)){
                    if('0'<=c && c<='9'){
                        d = d*16 + (c-'0');
                    }else if('a'<=c && c<='f'){
                        d = d*16 + ((c-'a') + 10);
                    }else{
                        break;
                    }
                }
            }
        }else{                  /* Decimal */
            d = c-'0';
            while((c= *ptr++)) {
                if('0'<=c && c<='9') {
                    d = d*10 + (c - '0');
                } else {
                    break;
                }
            }
        }
        if (c!=0) ptr--;
        value = d;
        last_token = '0';
        return last_token;
    } else if ('a'<=c && c<='z') {    /* variable */
        value = c-'a';                /* return variable reference */
        last_token = 'v';
        return last_token;
    } else if (c=='<'){    /* << or < */
        last_token = c;
        if(c== *ptr){       // < の後に<が来る→ <<が入力されている
            last_token = 'l';
            ptr++;
        }
        return last_token;
    } else if (c=='>'){    /* >> or > */
        last_token = c;
        if(c== *ptr){       // > の後に>が来る→ >>が入力されている
            last_token = 'r';
            ptr++;
        }
        return last_token;
    } else if (c=='=') {
        last_token = c;
        if(c == *ptr){       // = の後に=が来る→ ==が入力されている
            last_token = 'e';
            ptr++; 
        }
        return last_token;
    } else if (c=='!') {
        last_token = c;
        if(*ptr == '='){       // ! の後に=が来る→ !=が入力されている
            last_token = 'n';
            ptr++; 
        }
        return last_token;
    }else {
        last_token = c;
        return last_token;
        return c;
    }
}

void
error(char *s) {
    fprintf(stderr,"%s on %s\n",s,last_ptr);
}



/* end */
