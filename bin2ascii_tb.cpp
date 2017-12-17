#include <cstdio>
#include <cstdint>
#include <cstring>
#include <random>
#include <functional>           // for std::bind
#include <vector>
#include <algorithm>
#include <x86intrin.h>

extern "C" {
uint64_t zero(void);
char *uint64_to_ascii(uint64_t n, char *buf);
char *uint64_to_ascii_o3(uint64_t n, char *buf);
void uint64_to_ascii_hn_base10(uint64_t val, char* dst);
void uint64_to_ascii_hn_base100(uint64_t val, char* dst);
void uint64_to_ascii_tn_base100(uint64_t val, char* dst);
void uint64_to_ascii_hnts_base10(uint64_t val, char* dst);
void uint64_to_ascii_hnts_base100(uint64_t val, char* dst);
void uint64_to_ascii_tnts_base100(uint64_t val, char* dst);
void uint64_to_ascii_hn_base1000(uint64_t val, char* dst);
void uint64_to_ascii_hn_base1000_init(void);
size_t __attribute__ ((noinline)) print_buf64(char *buffer, uint64_t value);
void uint64_to_ascii_ta_base100(uint64_t val, char* dst);
void uint64_to_ascii_ta_base100_icc(uint64_t val, char* dst);
void uint64_to_ascii_ta7_32_base100(uint64_t val, char* dst);
void uint64_to_ascii_ta3_base100(uint64_t val, char* dst);
void uint64_to_ascii_1mul_base10(uint64_t val, char* dst);
}

static int print_error(const char* res, const char* ref, int tstSz);
int main(int argz, char** argv)
{
  const int TST_SZ = 11000;
  const int NITER  = 17;

  std::mt19937_64 rndGen;
  std::uniform_int_distribution<uint64_t> rndDistr(0, uint64_t(-1));

  std::vector<uint64_t> inpv(TST_SZ);
  inpv[0] = 0;
  inpv[1] = uint64_t(-1);
  inpv[2] = 17999999999999999999ull;
  inpv[3] = 9999999999ull;
  inpv[4] = 19999999999ull;
  inpv[5] = 1;
  auto rndFunc = std::bind ( rndDistr, std::ref(rndGen) );
  for (int i = 6; i < TST_SZ; ++i)
    inpv[i] = rndFunc();

  std::vector<char> refv(TST_SZ*21);
  char* pref = &refv.at(0);
  for (int i = 0; i < TST_SZ; ++i) {
    char ref[32];
    sprintf(ref, "%020llu", (unsigned long long)inpv[i]);
    memcpy(&pref[i*21], ref, 21);
  }

  std::vector<char> outv(TST_SZ*21);
  char* pout = &outv.at(0);
  uint64_t dtv[NITER], dtvl[NITER];
  unsigned dummy = 0;

  uint64_t aZero = zero(); // we know it's zero, but compiler and CPU don't know

  for (int it = 0; it < NITER; ++it) {
    memset(pout, '*', TST_SZ*21);
    uint64_t t0 = __rdtsc();
    for (int i = 0; i < TST_SZ; ++i) {
      uint64_to_ascii(inpv[i], &outv[i*21]);
    }
    uint64_t t1 = __rdtsc();
    dtv[it] = t1 - t0;
    for (int i = 0; i < TST_SZ*21; ++i)
      dummy += outv[i];
    if (it==0) {
      if (memcmp(pout, pref, TST_SZ*21) != 0) {
        printf("uint64_to_ascii fail.\n");
        return 1;
      }
    }
    char latbuf[32]={0};
    uint64_t xx = 0;
    uint64_t t2 = __rdtsc();
    for (int i = 0; i < TST_SZ; ++i) {
      uint64_to_ascii(inpv[i] ^ xx, latbuf);
      uint64_t x0, x1;
      memcpy(&x0, &latbuf[0],  sizeof(uint64_t));
      memcpy(&x1, &latbuf[16], sizeof(uint64_t));
      xx = x0 & x1 & aZero;
    }
    uint64_t t3 = __rdtsc();
    dtvl[it] = t3 - t2;
    for (int i = 0; i < 21; ++i)
      dummy += latbuf[i];
  }
  std::nth_element(&dtv[0], &dtv[NITER/2], &dtv[NITER]);
  std::nth_element(&dtvl[0], &dtvl[NITER/2], &dtvl[NITER]);
  printf("%.2f/%.2f clocks. Terje's\n", double(dtv[NITER/2])/TST_SZ, double(dtvl[NITER/2])/TST_SZ);

  for (int it = 0; it < NITER; ++it) {
    memset(pout, '*', TST_SZ*21);
    uint64_t t0 = __rdtsc();
    for (int i = 0; i < TST_SZ; ++i) {
      uint64_to_ascii_o3(inpv[i], &outv[i*21]);
    }
    uint64_t t1 = __rdtsc();
    dtv[it] = t1 - t0;
    for (int i = 0; i < TST_SZ*21; ++i)
      dummy += outv[i];
    if (it==0) {
      if (memcmp(pout, pref, TST_SZ*21) != 0) {
        printf("uint64_to_ascii_o3 fail.\n");
        return 1;
      }
    }
    char latbuf[32]={0};
    uint64_t xx = 0;
    uint64_t t2 = __rdtsc();
    for (int i = 0; i < TST_SZ; ++i) {
      uint64_to_ascii_o3(inpv[i] ^ xx, latbuf);
      uint64_t x0, x1;
      memcpy(&x0, &latbuf[0],  sizeof(uint64_t));
      memcpy(&x1, &latbuf[16], sizeof(uint64_t));
      xx = x0 & x1 & aZero;
    }
    uint64_t t3 = __rdtsc();
    dtvl[it] = t3 - t2;
    for (int i = 0; i < 21; ++i)
      dummy += latbuf[i];
  }
  std::nth_element(&dtv[0], &dtv[NITER/2], &dtv[NITER]);
  std::nth_element(&dtvl[0], &dtvl[NITER/2], &dtvl[NITER]);
  printf("%.2f/%.2f clocks. Terje's -O3\n", double(dtv[NITER/2])/TST_SZ, double(dtvl[NITER/2])/TST_SZ);

  for (int it = 0; it < NITER; ++it) {
    memset(pout, '*', TST_SZ*21);
    uint64_t t0 = __rdtsc();
    for (int i = 0; i < TST_SZ; ++i) {
      uint64_to_ascii_hn_base10(inpv[i], &outv[i*21]);
    }
    uint64_t t1 = __rdtsc();
    dtv[it] = t1 - t0;
    for (int i = 0; i < TST_SZ*21; ++i)
      dummy += outv[i];
    if (it==0) {
      if (memcmp(pout, pref, TST_SZ*21) != 0) {
        printf("uint64_to_ascii_hn_base10 fail.\n");
        return 1;
      }
    }
    char latbuf[32]={0};
    uint64_t xx = 0;
    uint64_t t2 = __rdtsc();
    for (int i = 0; i < TST_SZ; ++i) {
      uint64_to_ascii_hn_base10(inpv[i] ^ xx, latbuf);
      uint64_t x0, x1;
      memcpy(&x0, &latbuf[0],  sizeof(uint64_t));
      memcpy(&x1, &latbuf[16], sizeof(uint64_t));
      xx = x0 & x1 & aZero;
    }
    uint64_t t3 = __rdtsc();
    dtvl[it] = t3 - t2;
    for (int i = 0; i < 21; ++i)
      dummy += latbuf[i];
  }
  std::nth_element(&dtv[0], &dtv[NITER/2], &dtv[NITER]);
  std::nth_element(&dtvl[0], &dtvl[NITER/2], &dtvl[NITER]);
  printf("%.2f/%.2f clocks. Half-naive base 10.\n", double(dtv[NITER/2])/TST_SZ, double(dtvl[NITER/2])/TST_SZ);

  for (int it = 0; it < NITER; ++it) {
    memset(pout, '*', TST_SZ*21);
    uint64_t t0 = __rdtsc();
    for (int i = 0; i < TST_SZ; ++i) {
      uint64_to_ascii_hnts_base10(inpv[i], &outv[i*21]);
    }
    uint64_t t1 = __rdtsc();
    dtv[it] = t1 - t0;
    for (int i = 0; i < TST_SZ*21; ++i)
      dummy += outv[i];
    if (it==0) {
      if (memcmp(pout, pref, TST_SZ*21) != 0) {
        printf("uint64_to_ascii_hnts_base10 fail.\n");
        return 1;
      }
    }
    char latbuf[32]={0};
    uint64_t xx = 0;
    uint64_t t2 = __rdtsc();
    for (int i = 0; i < TST_SZ; ++i) {
      uint64_to_ascii_hnts_base10(inpv[i] ^ xx, latbuf);
      uint64_t x0, x1;
      memcpy(&x0, &latbuf[0],  sizeof(uint64_t));
      memcpy(&x1, &latbuf[16], sizeof(uint64_t));
      xx = x0 & x1 & aZero;
    }
    uint64_t t3 = __rdtsc();
    dtvl[it] = t3 - t2;
    for (int i = 0; i < 21; ++i)
      dummy += latbuf[i];
  }
  std::nth_element(&dtv[0], &dtv[NITER/2], &dtv[NITER]);
  std::nth_element(&dtvl[0], &dtvl[NITER/2], &dtvl[NITER]);
  printf("%.2f/%.2f clocks. Half-naive base 10. Compiler fed by tea spoon.\n", double(dtv[NITER/2])/TST_SZ, double(dtvl[NITER/2])/TST_SZ);

  for (int it = 0; it < NITER; ++it) {
    memset(pout, '*', TST_SZ*21);
    uint64_t t0 = __rdtsc();
    for (int i = 0; i < TST_SZ; ++i) {
      uint64_to_ascii_hn_base100(inpv[i], &outv[i*21]);
    }
    uint64_t t1 = __rdtsc();
    dtv[it] = t1 - t0;
    for (int i = 0; i < TST_SZ*21; ++i)
      dummy += outv[i];
    if (it==0) {
      if (memcmp(pout, pref, TST_SZ*21) != 0) {
        printf("uint64_to_ascii_hn_base100 fail.\n");
        return 1;
      }
    }
    char latbuf[32]={0};
    uint64_t xx = 0;
    uint64_t t2 = __rdtsc();
    for (int i = 0; i < TST_SZ; ++i) {
      uint64_to_ascii_hn_base100(inpv[i] ^ xx, latbuf);
      uint64_t x0, x1;
      memcpy(&x0, &latbuf[0],  sizeof(uint64_t));
      memcpy(&x1, &latbuf[16], sizeof(uint64_t));
      xx = x0 & x1 & aZero;
    }
    uint64_t t3 = __rdtsc();
    dtvl[it] = t3 - t2;
    for (int i = 0; i < 21; ++i)
      dummy += latbuf[i];
  }
  std::nth_element(&dtv[0], &dtv[NITER/2], &dtv[NITER]);
  std::nth_element(&dtvl[0], &dtvl[NITER/2], &dtvl[NITER]);
  printf("%.2f/%.2f clocks. Half-naive base 100.\n", double(dtv[NITER/2])/TST_SZ, double(dtvl[NITER/2])/TST_SZ);

  for (int it = 0; it < NITER; ++it) {
    memset(pout, '*', TST_SZ*21);
    uint64_t t0 = __rdtsc();
    for (int i = 0; i < TST_SZ; ++i) {
      uint64_to_ascii_hnts_base100(inpv[i], &outv[i*21]);
    }
    uint64_t t1 = __rdtsc();
    dtv[it] = t1 - t0;
    for (int i = 0; i < TST_SZ*21; ++i)
      dummy += outv[i];
    if (it==0) {
      if (memcmp(pout, pref, TST_SZ*21) != 0) {
        printf("uint64_to_ascii_hnts_base100 fail.\n");
        return 1;
      }
    }
    char latbuf[32]={0};
    uint64_t xx = 0;
    uint64_t t2 = __rdtsc();
    for (int i = 0; i < TST_SZ; ++i) {
      uint64_to_ascii_hnts_base100(inpv[i] ^ xx, latbuf);
      uint64_t x0, x1;
      memcpy(&x0, &latbuf[0],  sizeof(uint64_t));
      memcpy(&x1, &latbuf[16], sizeof(uint64_t));
      xx = x0 & x1 & aZero;
    }
    uint64_t t3 = __rdtsc();
    dtvl[it] = t3 - t2;
    for (int i = 0; i < 21; ++i)
      dummy += latbuf[i];
  }
  std::nth_element(&dtv[0], &dtv[NITER/2], &dtv[NITER]);
  std::nth_element(&dtvl[0], &dtvl[NITER/2], &dtvl[NITER]);
  printf("%.2f/%.2f clocks. Half-naive base 100. Compiler fed by tea spoon.\n", double(dtv[NITER/2])/TST_SZ, double(dtvl[NITER/2])/TST_SZ);

  uint64_to_ascii_hn_base1000_init();
  for (int it = 0; it < NITER; ++it) {
    memset(pout, '*', TST_SZ*21);
    uint64_t t0 = __rdtsc();
    for (int i = 0; i < TST_SZ; ++i) {
      uint64_to_ascii_hn_base1000(inpv[i], &outv[i*21]);
    }
    uint64_t t1 = __rdtsc();
    dtv[it] = t1 - t0;
    for (int i = 0; i < TST_SZ*21; ++i)
      dummy += outv[i];
    if (it==0) {
      if (memcmp(pout, pref, TST_SZ*21) != 0) {
        printf("uint64_to_ascii_hn_base1000 fail.\n");
        return 1;
      }
    }
    char latbuf[32]={0};
    uint64_t xx = 0;
    uint64_t t2 = __rdtsc();
    for (int i = 0; i < TST_SZ; ++i) {
      uint64_to_ascii_hn_base1000(inpv[i] ^ xx, latbuf);
      uint64_t x0, x1;
      memcpy(&x0, &latbuf[0],  sizeof(uint64_t));
      memcpy(&x1, &latbuf[16], sizeof(uint64_t));
      xx = x0 & x1 & aZero;
    }
    uint64_t t3 = __rdtsc();
    dtvl[it] = t3 - t2;
    for (int i = 0; i < 21; ++i)
      dummy += latbuf[i];
  }
  std::nth_element(&dtv[0], &dtv[NITER/2], &dtv[NITER]);
  std::nth_element(&dtvl[0], &dtvl[NITER/2], &dtvl[NITER]);
  printf("%.2f/%.2f clocks. Half-naive base 1000.\n", double(dtv[NITER/2])/TST_SZ, double(dtvl[NITER/2])/TST_SZ);

  for (int it = 0; it < NITER; ++it) {
    memset(pout, '*', TST_SZ*21);
    uint64_t t0 = __rdtsc();
    for (int i = 0; i < TST_SZ; ++i) {
      uint64_to_ascii_tn_base100(inpv[i], &outv[i*21]);
    }
    uint64_t t1 = __rdtsc();
    dtv[it] = t1 - t0;
    for (int i = 0; i < TST_SZ*21; ++i)
      dummy += outv[i];
    if (it==0) {
      if (memcmp(pout, pref, TST_SZ*21) != 0) {
        printf("uint64_to_ascii_tn_base100 fail.\n");
        return 1;
      }
    }
    char latbuf[32]={0};
    uint64_t xx = 0;
    uint64_t t2 = __rdtsc();
    for (int i = 0; i < TST_SZ; ++i) {
      uint64_to_ascii_tn_base100(inpv[i] ^ xx, latbuf);
      uint64_t x0, x1;
      memcpy(&x0, &latbuf[0],  sizeof(uint64_t));
      memcpy(&x1, &latbuf[16], sizeof(uint64_t));
      xx = x0 & x1 & aZero;
    }
    uint64_t t3 = __rdtsc();
    dtvl[it] = t3 - t2;
    for (int i = 0; i < 21; ++i)
      dummy += latbuf[i];
  }
  std::nth_element(&dtv[0], &dtv[NITER/2], &dtv[NITER]);
  std::nth_element(&dtvl[0], &dtvl[NITER/2], &dtvl[NITER]);
  printf("%.2f/%.2f clocks. Third-naive base 100.\n", double(dtv[NITER/2])/TST_SZ, double(dtvl[NITER/2])/TST_SZ);

  for (int it = 0; it < NITER; ++it) {
    memset(pout, '*', TST_SZ*21);
    uint64_t t0 = __rdtsc();
    for (int i = 0; i < TST_SZ; ++i) {
      uint64_to_ascii_tnts_base100(inpv[i], &outv[i*21]);
    }
    uint64_t t1 = __rdtsc();
    dtv[it] = t1 - t0;
    for (int i = 0; i < TST_SZ*21; ++i)
      dummy += outv[i];
    if (it==0) {
      if (memcmp(pout, pref, TST_SZ*21) != 0) {
        printf("uint64_to_ascii_tnts_base100 fail.\n");
        return 1;
      }
    }
    char latbuf[32]={0};
    uint64_t xx = 0;
    uint64_t t2 = __rdtsc();
    for (int i = 0; i < TST_SZ; ++i) {
      uint64_to_ascii_tnts_base100(inpv[i] ^ xx, latbuf);
      uint64_t x0, x1;
      memcpy(&x0, &latbuf[0],  sizeof(uint64_t));
      memcpy(&x1, &latbuf[16], sizeof(uint64_t));
      xx = x0 & x1 & aZero;
    }
    uint64_t t3 = __rdtsc();
    dtvl[it] = t3 - t2;
    for (int i = 0; i < 21; ++i)
      dummy += latbuf[i];
  }
  std::nth_element(&dtv[0], &dtv[NITER/2], &dtv[NITER]);
  std::nth_element(&dtvl[0], &dtvl[NITER/2], &dtvl[NITER]);
  printf("%.2f/%.2f clocks. Third-naive base 100. Compiler fed by tea spoon.\n", double(dtv[NITER/2])/TST_SZ, double(dtvl[NITER/2])/TST_SZ);

  for (int it = 0; it < NITER; ++it) {
    memset(pout, '*', TST_SZ*21);
    uint64_t t0 = __rdtsc();
    for (int i = 0; i < TST_SZ; ++i) {
      print_buf64(&outv[i*21], inpv[i]);
      outv[i*21+20] = 0;
    }
    uint64_t t1 = __rdtsc();
    dtv[it] = t1 - t0;
    for (int i = 0; i < TST_SZ*21; ++i)
      dummy += outv[i];
    if (it==0) {
      if (memcmp(pout, pref, TST_SZ*21) != 0) {
        printf("print_buf64 fail.\n");
        return 1;
      }
    }
    char latbuf[32]={0};
    uint64_t xx = 0;
    uint64_t t2 = __rdtsc();
    for (int i = 0; i < TST_SZ; ++i) {
      print_buf64(latbuf, inpv[i] ^ xx);
      latbuf[20] = 0;
      uint64_t x0, x1;
      memcpy(&x0, &latbuf[0],  sizeof(uint64_t));
      memcpy(&x1, &latbuf[16], sizeof(uint64_t));
      xx = x0 & x1 & aZero;
    }
    uint64_t t3 = __rdtsc();
    dtvl[it] = t3 - t2;
    for (int i = 0; i < 21; ++i)
      dummy += latbuf[i];
  }
  std::nth_element(&dtv[0], &dtv[NITER/2], &dtv[NITER]);
  std::nth_element(&dtvl[0], &dtvl[NITER/2], &dtvl[NITER]);
  printf("%.2f/%.2f clocks. Joshua's base 100.\n", double(dtv[NITER/2])/TST_SZ, double(dtvl[NITER/2])/TST_SZ);

  for (int it = 0; it < NITER; ++it) {
    memset(pout, '*', TST_SZ*21);
    uint64_t t0 = __rdtsc();
    for (int i = 0; i < TST_SZ; ++i) {
      uint64_to_ascii_ta_base100(inpv[i], &outv[i*21]);
    }
    uint64_t t1 = __rdtsc();
    dtv[it] = t1 - t0;
    for (int i = 0; i < TST_SZ*21; ++i)
      dummy += outv[i];
    if (it==0) {
      if (memcmp(pout, pref, TST_SZ*21) != 0) {
        printf("uint64_to_ascii_ta_base100 fail.\n");
        return print_error(pout, pref, TST_SZ);
      }
    }
    char latbuf[32]={0};
    uint64_t xx = 0;
    uint64_t t2 = __rdtsc();
    for (int i = 0; i < TST_SZ; ++i) {
      uint64_to_ascii_ta_base100(inpv[i] ^ xx, latbuf);
      uint64_t x0, x1;
      memcpy(&x0, &latbuf[0],  sizeof(uint64_t));
      memcpy(&x1, &latbuf[16], sizeof(uint64_t));
      xx = x0 & x1 & aZero;
    }
    uint64_t t3 = __rdtsc();
    dtvl[it] = t3 - t2;
    for (int i = 0; i < 21; ++i)
      dummy += latbuf[i];
  }
  std::nth_element(&dtv[0], &dtv[NITER/2], &dtv[NITER]);
  std::nth_element(&dtvl[0], &dtvl[NITER/2], &dtvl[NITER]);
  printf("%.2f/%.2f clocks. Terje-alike (2 chains) base 100.\n", double(dtv[NITER/2])/TST_SZ, double(dtvl[NITER/2])/TST_SZ);

  for (int it = 0; it < NITER; ++it) {
    memset(pout, '*', TST_SZ*21);
    uint64_t t0 = __rdtsc();
    for (int i = 0; i < TST_SZ; ++i) {
      uint64_to_ascii_ta_base100_icc(inpv[i], &outv[i*21]);
    }
    uint64_t t1 = __rdtsc();
    dtv[it] = t1 - t0;
    for (int i = 0; i < TST_SZ*21; ++i)
      dummy += outv[i];
    if (it==0) {
      if (memcmp(pout, pref, TST_SZ*21) != 0) {
        printf("uint64_to_ascii_ta_base100_icc fail.\n");
        return print_error(pout, pref, TST_SZ);
      }
    }
    char latbuf[32]={0};
    uint64_t xx = 0;
    uint64_t t2 = __rdtsc();
    for (int i = 0; i < TST_SZ; ++i) {
      uint64_to_ascii_ta_base100_icc(inpv[i] ^ xx, latbuf);
      uint64_t x0, x1;
      memcpy(&x0, &latbuf[0],  sizeof(uint64_t));
      memcpy(&x1, &latbuf[16], sizeof(uint64_t));
      xx = x0 & x1 & aZero;
    }
    uint64_t t3 = __rdtsc();
    dtvl[it] = t3 - t2;
    for (int i = 0; i < 21; ++i)
      dummy += latbuf[i];
  }
  std::nth_element(&dtv[0], &dtv[NITER/2], &dtv[NITER]);
  std::nth_element(&dtvl[0], &dtvl[NITER/2], &dtvl[NITER]);
  printf("%.2f/%.2f clocks. Terje-alike (2 chains) base 100. Inline asm imitates ICC.\n", double(dtv[NITER/2])/TST_SZ, double(dtvl[NITER/2])/TST_SZ);

  for (int it = 0; it < NITER; ++it) {
    memset(pout, '*', TST_SZ*21);
    uint64_t t0 = __rdtsc();
    for (int i = 0; i < TST_SZ; ++i) {
      uint64_to_ascii_ta7_32_base100(inpv[i], &outv[i*21]);
    }
    uint64_t t1 = __rdtsc();
    dtv[it] = t1 - t0;
    for (int i = 0; i < TST_SZ*21; ++i)
      dummy += outv[i];
    if (it==0) {
      if (memcmp(pout, pref, TST_SZ*21) != 0) {
        printf("uint64_to_ascii_ta7_32_base100 fail.\n");
        return print_error(pout, pref, TST_SZ);
      }
    }
    char latbuf[32]={0};
    uint64_t xx = 0;
    uint64_t t2 = __rdtsc();
    for (int i = 0; i < TST_SZ; ++i) {
      uint64_to_ascii_ta7_32_base100(inpv[i] ^ xx, latbuf);
      uint64_t x0, x1;
      memcpy(&x0, &latbuf[0],  sizeof(uint64_t));
      memcpy(&x1, &latbuf[16], sizeof(uint64_t));
      xx = x0 & x1 & aZero;
    }
    uint64_t t3 = __rdtsc();
    dtvl[it] = t3 - t2;
    for (int i = 0; i < 21; ++i)
      dummy += latbuf[i];
  }
  std::nth_element(&dtv[0], &dtv[NITER/2], &dtv[NITER]);
  std::nth_element(&dtvl[0], &dtvl[NITER/2], &dtvl[NITER]);
  printf("%.2f/%.2f clocks. Terje-alike (2 chains) base 100. Fix point format 7.32 for a benefit of stupid compilers.\n", double(dtv[NITER/2])/TST_SZ, double(dtvl[NITER/2])/TST_SZ);

  for (int it = 0; it < NITER; ++it) {
    memset(pout, '*', TST_SZ*21);
    uint64_t t0 = __rdtsc();
    for (int i = 0; i < TST_SZ; ++i) {
      uint64_to_ascii_ta3_base100(inpv[i], &outv[i*21]);
    }
    uint64_t t1 = __rdtsc();
    dtv[it] = t1 - t0;
    for (int i = 0; i < TST_SZ*21; ++i)
      dummy += outv[i];
    if (it==0) {
      if (memcmp(pout, pref, TST_SZ*21) != 0) {
        printf("uint64_to_ascii_ta3_base100 fail.\n");
        return print_error(pout, pref, TST_SZ);
      }
    }
    char latbuf[32]={0};
    uint64_t xx = 0;
    uint64_t t2 = __rdtsc();
    for (int i = 0; i < TST_SZ; ++i) {
      uint64_to_ascii_ta3_base100(inpv[i] ^ xx, latbuf);
      uint64_t x0, x1;
      memcpy(&x0, &latbuf[0],  sizeof(uint64_t));
      memcpy(&x1, &latbuf[16], sizeof(uint64_t));
      xx = x0 & x1 & aZero;
    }
    uint64_t t3 = __rdtsc();
    dtvl[it] = t3 - t2;
    for (int i = 0; i < 21; ++i)
      dummy += latbuf[i];
  }
  std::nth_element(&dtv[0], &dtv[NITER/2], &dtv[NITER]);
  std::nth_element(&dtvl[0], &dtvl[NITER/2], &dtvl[NITER]);
  printf("%.2f/%.2f clocks. Terje-alike (3 chains) base 100.\n", double(dtv[NITER/2])/TST_SZ, double(dtvl[NITER/2])/TST_SZ);

  for (int it = 0; it < NITER; ++it) {
    memset(pout, '*', TST_SZ*21);
    uint64_t t0 = __rdtsc();
    for (int i = 0; i < TST_SZ; ++i) {
      uint64_to_ascii_1mul_base10(inpv[i], &outv[i*21]);
    }
    uint64_t t1 = __rdtsc();
    dtv[it] = t1 - t0;
    for (int i = 0; i < TST_SZ*21; ++i)
      dummy += outv[i];
    if (it==0) {
      if (memcmp(pout, pref, TST_SZ*21) != 0) {
        printf("uint64_to_ascii_1mul_base10 fail.\n");
        return print_error(pout, pref, TST_SZ);
      }
    }
    char latbuf[32]={0};
    uint64_t xx = 0;
    uint64_t t2 = __rdtsc();
    for (int i = 0; i < TST_SZ; ++i) {
      uint64_to_ascii_1mul_base10(inpv[i] ^ xx, latbuf);
      uint64_t x0, x1;
      memcpy(&x0, &latbuf[0],  sizeof(uint64_t));
      memcpy(&x1, &latbuf[16], sizeof(uint64_t));
      xx = x0 & x1 & aZero;
    }
    uint64_t t3 = __rdtsc();
    dtvl[it] = t3 - t2;
    for (int i = 0; i < 21; ++i)
      dummy += latbuf[i];
  }
  std::nth_element(&dtv[0], &dtv[NITER/2], &dtv[NITER]);
  std::nth_element(&dtvl[0], &dtvl[NITER/2], &dtvl[NITER]);
  printf("%.2f/%.2f clocks. Terje-alike 1 multiplication base 10.\n", double(dtv[NITER/2])/TST_SZ, double(dtvl[NITER/2])/TST_SZ);

  if (dummy==42)
    printf("Blue moon\n");

  return 0;
}

static int print_error(const char* res, const char* ref, int tstSz)
{
  for (int i = 0; i < tstSz; ++i) {
    if (memcmp(&ref[i*21], &res[i*21], 21) != 0) {
      char buf[22];
      memcpy(buf, &res[i*21], 21);
      buf[21] = 0;
      printf("%s => %s\n", &ref[i*21], buf);
      return i + 1;
    }
  }
  return -1;
}

