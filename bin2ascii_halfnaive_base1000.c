#include <stdint.h>
#include <string.h>

static char b1000_tab[3000+1];

void uint64_to_ascii_hn_base1000(uint64_t val, char* dst)
{
  const int64_t POW10_10 = ((int64_t)10)*1000*1000*1000;
  int64_t hix = val / POW10_10;
  int64_t lox = val % POW10_10;

  dst[0*10+9] = (hix % 10) + '0'; hix /= 10;
  dst[1*10+9] = (lox % 10) + '0'; lox /= 10;

  int32_t hi = hix;
  int32_t lo = lox;

  memcpy(dst+0*10+6, &b1000_tab[(hi % 1000)*3], 3); hi /= 1000;
  memcpy(dst+1*10+6, &b1000_tab[(lo % 1000)*3], 3); lo /= 1000;

  memcpy(dst+0*10+3, &b1000_tab[(hi % 1000)*3], 3); hi /= 1000;
  memcpy(dst+1*10+3, &b1000_tab[(lo % 1000)*3], 3); lo /= 1000;

  memcpy(dst+0*10+0, &b1000_tab[hi*3], 3);
  memcpy(dst+1*10+0, &b1000_tab[lo*3], 3);

  dst[2*10] = 0;
}

void uint64_to_ascii_hn_base1000_init(void)
{
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 10; ++j) {
      for (int k = 0; k < 10; ++k) {
        b1000_tab[(i*100+j*10+k)*3+0] = i + '0';
        b1000_tab[(i*100+j*10+k)*3+1] = j + '0';
        b1000_tab[(i*100+j*10+k)*3+2] = k + '0';
      }
    }
  }
}
