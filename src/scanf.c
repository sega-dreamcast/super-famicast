#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

/* very limited vsscanf, sscanf support */

static int read_int(const char* from, int* idx)
{
  int res = 0, i = *idx, negative = 0;

/*  fprintf(stderr, "from[i]=%c\n", from[i]);*/

  if (from[i] == '-')
  {
    i++;
    negative = 1;
  }
  
  for (; from[i]>='0' && from[i]<='9'; i++)
  {
/*    fprintf(stderr, "from[i]=%c\n", from[i]);*/
    res = (from[i] - '0') + 10*res;
  }
  
  *idx = i;
  
  return negative ? -res : res;
}

int vsscanf(const char* str, const char* fmt, va_list params)
{
  int sidx = 0, fidx = 0, cnt = 0, fmtlen = strlen(fmt);
  
  while (fidx < fmtlen)
  {
/*    fprintf(stderr, "fidx=%d fmt[fidx]=%c\n", fidx, fmt[fidx]);*/
    switch (fmt[fidx])
    {
      case '%':
      fidx++;
      switch (fmt[fidx])
      {
        case 'd':
        case 'i':
        {
          int* ptrtoval = va_arg(params, int*);
          fidx++;
          *ptrtoval = read_int(str, &sidx);
          cnt++;
        }
        break;
        
        case 'c':
        {
          char* ptrtochar = va_arg(params, char*);
          fidx++;
          *ptrtochar = str[sidx++];
          cnt++;
        }
        break;
        
        case 's':
        {
          char* writeto = va_arg(params, char*);
          int i = 0;
          /* skip whitespace */
          fidx++;
          while (str[sidx]==' ' || str[sidx]=='\t' || str[sidx]=='\n') sidx++;
          while (str[sidx]!=' ' && str[sidx]!='\t' && str[sidx]!='\n')
          {
            writeto[i++] = str[sidx++];
          }
          /* write terminating NUL */
          writeto[i] = '\0';
          cnt++;
        }
        break;
        
        default:
        fprintf(stderr, "Bad format character (%c)\n", fmt[fidx]);
        exit(1);
      }
      break;

      case ' ':
      case '\t':
      case '\n':
      fidx++;
      while (str[sidx]==' ' || str[sidx]=='\t' || str[sidx]=='\n') sidx++;
      break;

      default:
/*      printf("str[sidx]=%d fmt[fidx]=%d\n", str[sidx], fmt[fidx]);*/
      if (str[sidx++] != fmt[fidx++])
      {
        goto badmatch;
      }
    }
  }
  
  badmatch:
  
  return cnt;
}

int sscanf(const char* str, const char* fmt, ...)
{
  va_list params;
  int ret;
  
  va_start(params, fmt);
  ret = vsscanf(str, fmt, params);
  va_end(params);
  
  return ret;
}

#ifdef TESTING
int main(void)
{
  char f[255], c;
  int i;
  
  sscanf("S:80:/pc/home/whatever", "%c:%i:%s", &c, &i, f);
  printf("c=%c\ni=%d\nf=%s\n", c, i, f);
  
  return 0;
}
#endif
