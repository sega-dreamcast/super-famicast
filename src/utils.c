#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "utils.h"

int stricmp(const char *a, const char *b) {
  int ca, cb;
  do {
     ca = (unsigned char) *a++;
     cb = (unsigned char) *b++;
     ca = tolower(toupper(ca));
     cb = tolower(toupper(cb));
   } while (ca == cb && ca != '\0');
   return ca - cb;
}

int strnicmp(const char *s1, const char *s2, unsigned int len)
{
    int diff = 0;
    while (len-- && *s1 && *s2) {
        if (*s1 != *s2)
            if ((diff) = ((int)tolower(*s1) - (int)tolower(*s2)))
                break;
        s1++;
        s2++;
    }
    return diff;
}
