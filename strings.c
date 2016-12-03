// some string funcs we needed

#include <kos.h>

char *strrev (char *s) {                                               
  char *ret = s;
  char *t = s;
  char c;

  while (*t != '\0') t++;
  t--;

  while (t > s) {
    c = *s;
    *s = *t;
    *t = c;
    s++;
    t--;
  }

  return ret;
}


char *itoa (int val) {
  char *s = (char *) malloc (sizeof (char) * 16); 
  char *t;
  int mod;

  if (val < 0) {
    *s++ = '-';
    val = -val;
  }
  t = s;

  while (val) {
    mod = val % 10;
    *t++ = (char) mod + '0';
    val /= 10;
  }

  if (s == t)
    *t++ = '0';

  *t = '\0';

  return strrev(s);
}
