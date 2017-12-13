#include <stdint.h>
#include <string.h>

static const char b100_tab[200] = {
  '0', '0',
  '0', '1',
  '0', '2',
  '0', '3',
  '0', '4',
  '0', '5',
  '0', '6',
  '0', '7',
  '0', '8',
  '0', '9',

  '1', '0',
  '1', '1',
  '1', '2',
  '1', '3',
  '1', '4',
  '1', '5',
  '1', '6',
  '1', '7',
  '1', '8',
  '1', '9',

  '2', '0',
  '2', '1',
  '2', '2',
  '2', '3',
  '2', '4',
  '2', '5',
  '2', '6',
  '2', '7',
  '2', '8',
  '2', '9',

  '3', '0',
  '3', '1',
  '3', '2',
  '3', '3',
  '3', '4',
  '3', '5',
  '3', '6',
  '3', '7',
  '3', '8',
  '3', '9',

  '4', '0',
  '4', '1',
  '4', '2',
  '4', '3',
  '4', '4',
  '4', '5',
  '4', '6',
  '4', '7',
  '4', '8',
  '4', '9',

  '5', '0',
  '5', '1',
  '5', '2',
  '5', '3',
  '5', '4',
  '5', '5',
  '5', '6',
  '5', '7',
  '5', '8',
  '5', '9',

  '6', '0',
  '6', '1',
  '6', '2',
  '6', '3',
  '6', '4',
  '6', '5',
  '6', '6',
  '6', '7',
  '6', '8',
  '6', '9',

  '7', '0',
  '7', '1',
  '7', '2',
  '7', '3',
  '7', '4',
  '7', '5',
  '7', '6',
  '7', '7',
  '7', '8',
  '7', '9',

  '8', '0',
  '8', '1',
  '8', '2',
  '8', '3',
  '8', '4',
  '8', '5',
  '8', '6',
  '8', '7',
  '8', '8',
  '8', '9',

  '9', '0',
  '9', '1',
  '9', '2',
  '9', '3',
  '9', '4',
  '9', '5',
  '9', '6',
  '9', '7',
  '9', '8',
  '9', '9',
};

static const int32_t POW2_37_DIV100 = ((int64_t)(1) << 37)/100 + 1;
static int32_t uint64_to_ascii_1st_step(char* dst, int64_t val)
{
  // first step uses a little more complex sequence,
  // because a simple sequence could overflow
  int64_t res = ((val >> 2)*POW2_37_DIV100)>>35;
  memcpy(dst, &b100_tab[(val - res*100)*2], 2);
  return res;
}

static int32_t uint64_to_ascii_step(char* dst, int32_t val)
{
  int32_t res = ((int64_t)val*POW2_37_DIV100)>>37;
  memcpy(dst, &b100_tab[(val - res*100)*2], 2);
  return res;
}

void uint64_to_ascii_hnts_base100(uint64_t val, char* dst)
{
  const int64_t POW10_10 = ((int64_t)10)*1000*1000*1000;
  int64_t hix = val / POW10_10;
  int64_t lox = val % POW10_10;

  int32_t hi = uint64_to_ascii_1st_step(dst+0*10+8, hix);
  int32_t lo = uint64_to_ascii_1st_step(dst+1*10+8, lox);

  hi = uint64_to_ascii_step(dst+0*10+6, hi);
  lo = uint64_to_ascii_step(dst+1*10+6, lo);

  hi = uint64_to_ascii_step(dst+0*10+4, hi);
  lo = uint64_to_ascii_step(dst+1*10+4, lo);

  hi = uint64_to_ascii_step(dst+0*10+2, hi);
  lo = uint64_to_ascii_step(dst+1*10+2, lo);

  memcpy(dst+0*10+0, &b100_tab[hi*2], 2);
  memcpy(dst+1*10+0, &b100_tab[lo*2], 2);

  dst[2*10] = 0;
}