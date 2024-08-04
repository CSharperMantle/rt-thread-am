#include <klib.h>
#include <rtthread.h>
#include <limits.h>

char *strchr(const char *s, int c) {
  while (*s != (char)c) {
    if (*(s++) == '\0') {
      return NULL;
    }
  }
  return (char *)s;
}

char *strrchr(const char *s, int c) {
  char *last = NULL;
  do {
    if (*s == (char)c) {
      last = (char *)s;
    }
  } while (*s++);
  return last;
}

char *strstr(const char *haystack, const char *needle) {
  return rt_strstr(haystack, needle);
}

long strtol(const char *nptr, char **endptr, int base) {
  if (base < 0 || base == 1 || base > 36) {
    return 0;
  }

  while (isspace((unsigned char)*nptr))
    nptr++;
  int neg = *nptr == '-';
  if (*nptr == '+' || *nptr == '-')
    nptr++;

  if (base == 0) {
    if (*nptr == '0') {
      base = 8;
      if ((nptr[1] == 'x' || nptr[1] == 'X') && isxdigit(nptr[2])) {
        nptr += 2;
        base = 16;
      }
    } else
      base = 10;
  } else if (base == 16 && *nptr == '0' && (nptr[1] == 'x' || nptr[1] == 'X')) {
    nptr += 2;
  }

  long long ret = 0;
  while (1) {
    int digit;
    if (isdigit((unsigned char)*nptr))
      digit = *nptr - '0';
    else if (isalpha((unsigned char)*nptr))
      digit = toupper((unsigned char)*nptr) - 'A' + 10;
    else
      break;

    if (digit >= base)
      break;
    if (ret > (LLONG_MAX - digit) / base) {
      ret = LLONG_MAX;
    } else {
      ret = ret * base + digit;
      nptr++;
    }
  }

  if (endptr)
    *endptr = (char *)nptr;
  return (neg ? -ret : ret);
}

char *strncat(char *restrict dst, const char *restrict src, size_t sz) {
  assert(0);
}
