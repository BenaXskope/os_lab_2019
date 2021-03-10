#include <string.h>
#include "revert_string.h"

void RevertString(char *str)
{
    for (unsigned long i=0; i < (strlen(str)/2); i++)
    {
        *(str+i) = *(str+i) - *(str+strlen(str)-1-i); 
        *(str+strlen(str)-1-i) = *(str+i) + *(str+strlen(str)-1-i); 
        *(str+i) = *(str+strlen(str)-1-i) - *(str+i);
    }
}

