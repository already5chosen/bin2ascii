#include <stdint.h>

static int32_t uint64_to_ascii_1st_step(char* dst, int32_t val)
{
  const int32_t POW2_34_DIV10 = ((int64_t)(1) << 34)/10 + 1;
  int32_t res = ((int64_t)val*POW2_34_DIV10)>>34;
  *dst = val - res*10 + '0';
  return res;
}

static int32_t uint64_to_ascii_step(char* dst, int32_t val)
{
  const int32_t POW2_18_DIV10 = ((int32_t)(1) << 18)/10 + 1;
  int32_t res = (val*POW2_18_DIV10)>>18;
  *dst = val - res*10 + '0';
  return res;
}

void uint64_to_ascii_hnts_base10(uint64_t val, char* dst)
{
  const int64_t POW10_10 = ((int64_t)10)*1000*1000*1000;
  int64_t hix = val / POW10_10;
  int64_t lox = val % POW10_10;
  int32_t v0 = hix / 100000;
  int32_t v1 = hix % 100000;
  int32_t v2 = lox / 100000;
  int32_t v3 = lox % 100000;

  v0 = uint64_to_ascii_1st_step(&dst[4+0*5], v0);
  v1 = uint64_to_ascii_1st_step(&dst[4+1*5], v1);
  v2 = uint64_to_ascii_1st_step(&dst[4+2*5], v2);
  v3 = uint64_to_ascii_1st_step(&dst[4+3*5], v3);

  v0 = uint64_to_ascii_step(&dst[3+0*5], v0);
  v1 = uint64_to_ascii_step(&dst[3+1*5], v1);
  v2 = uint64_to_ascii_step(&dst[3+2*5], v2);
  v3 = uint64_to_ascii_step(&dst[3+3*5], v3);

  v0 = uint64_to_ascii_step(&dst[2+0*5], v0);
  v1 = uint64_to_ascii_step(&dst[2+1*5], v1);
  v2 = uint64_to_ascii_step(&dst[2+2*5], v2);
  v3 = uint64_to_ascii_step(&dst[2+3*5], v3);

  v0 = uint64_to_ascii_step(&dst[1+0*5], v0);
  v1 = uint64_to_ascii_step(&dst[1+1*5], v1);
  v2 = uint64_to_ascii_step(&dst[1+2*5], v2);
  v3 = uint64_to_ascii_step(&dst[1+3*5], v3);

  dst[0*5] = v0 + '0';
  dst[1*5] = v1 + '0';
  dst[2*5] = v2 + '0';
  dst[3*5] = v3 + '0';
  dst[4*5] = 0;
}