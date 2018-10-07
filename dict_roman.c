/* 
 * example of dictionary 
 * Teodor Sigaev <teodor@sigaev.ru>
 */
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "postgres.h"
#include "fmgr.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

#include "commands/defrem.h"
#include "tsearch/dicts/spell.h"
#include "tsearch/ts_locale.h"
#include "tsearch/ts_public.h"
#include "tsearch/ts_utils.h"
#include "utils/builtins.h"
#include "utils/memutils.h"


typedef struct {

} DictRoman;

PG_FUNCTION_INFO_V1(dinit_roman);
Datum dinit_roman(PG_FUNCTION_ARGS);

Datum 
dinit_roman(PG_FUNCTION_ARGS) {
 	DictRoman *d = (DictRoman*)malloc(sizeof(DictRoman));
        
 	if(!d)
 		elog(ERROR, "No memory");

        /* Your INIT code */

 	PG_RETURN_POINTER(d);
}

static unsigned char *tokens[] = {"M",  "CM", "D", "CD", "CCC", "CC", "C", "XC", "L", "XL", "XXX", "XX", "X", "IX", "V", "IV", "III", "II", "I"};
static int values[] = {1000, 900, 500, 400, 300, 200, 100, 90, 50, 40, 30, 20, 10, 9, 5, 4, 3, 2, 1}; 
static int ntokens = 19;

static unsigned int roman_to_arabic(unsigned char *string)
{
        unsigned char *pos = string;
        unsigned int result = 0;
        unsigned int prev_value = 0;
    
        while(*pos){
                int d = 0;
                int is_matched = 0;
        
                for(d = 0; d < ntokens; d++)
                        if(strncmp(pos, tokens[d], strlen(tokens[d])) == 0){
                                unsigned int value = values[d];
                
                                if(prev_value && value > prev_value){
                                        /* printf("Wrong order of tokens!\n"); */
                                        return 0;
                                }
                
                                result += values[d];
                                pos += strlen(tokens[d]);
                                is_matched = 1;
                                prev_value = value;
                
                                break;
                        }
        
                if(!is_matched){
                        /* printf("Can't match token: %s,value is %d\n", pos, result); */
                        return 0;
                }
        }

        return result;
}

PG_FUNCTION_INFO_V1(dlexize_roman);
Datum dlexize_roman(PG_FUNCTION_ARGS);
Datum
dlexize_roman(PG_FUNCTION_ARGS) {
 	DictRoman *d = (DictRoman*)PG_GETARG_POINTER(0);
	char *in = (char*)PG_GETARG_POINTER(1);
        char *in_string = pnstrdup(in, PG_GETARG_INT32(2));
        int value = roman_to_arabic(in_string);

        pfree(in_string);
        
        if(value){
                /* Don't know how to optimize it in Postgres memory framework */
                char *txt = palloc(1024);
                TSLexeme *res = palloc(sizeof(TSLexeme)*2);

                sprintf(txt, "%d", value);
                txt = repalloc(txt, strlen(txt) + 1);
                         
                res[1].lexeme = NULL;
                res[0].lexeme = txt;

                PG_RETURN_POINTER(res);
        } else
                PG_RETURN_POINTER(NULL);
}
