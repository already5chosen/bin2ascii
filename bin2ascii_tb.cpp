#include <cstdio>
#include <cstdint>
#include <cstring>
#include <random>
#include <functional>           // for std::bind
#include <vector>
#include <algorithm>
#include <x86intrin.h>

extern "C" {
char *uint64_to_ascii(uint64_t n, char *buf);
char *uint64_to_ascii_o3(uint64_t n, char *buf);
void uint64_to_ascii_hn_base10(uint64_t val, char* dst);
void uint64_to_ascii_hn_base100(uint64_t val, char* dst);
void uint64_to_ascii_hn_base1000(uint64_t val, char* dst);
void uint64_to_ascii_hn_base1000_init(void);
void uint64_to_ascii_hn_base1000_le(uint64_t val, char* dst);
void uint64_to_ascii_hn_base1000_le_init(void);
size_t __attribute__ ((noinline)) print_buf64(char *buffer, uint64_t value);
}

int main(int argz, char** argv)
{
  const int TST_SZ = 11000;
  const int NITER  = 17;

  std::mt19937_64 rndGen;
  std::uniform_int_distribution<uint64_t> rndDistr(0, uint64_t(-1));

  std::vector<uint64_t> inpv(TST_SZ);
  auto rndFunc = std::bind ( rndDistr, std::ref(rndGen) );
  for (int i = 0; i < TST_SZ; ++i)
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
  uint64_t dtv[NITER];
  unsigned dummy = 0;

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
  }
  std::nth_element(&dtv[0], &dtv[NITER/2], &dtv[NITER]);
  printf("%.2f clocks. Terje's\n", double(dtv[NITER/2])/TST_SZ);

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
  }
  std::nth_element(&dtv[0], &dtv[NITER/2], &dtv[NITER]);
  printf("%.2f clocks. Terje's -O3\n", double(dtv[NITER/2])/TST_SZ);

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
  }
  std::nth_element(&dtv[0], &dtv[NITER/2], &dtv[NITER]);
  printf("%.2f clocks. Half-naive base 10.\n", double(dtv[NITER/2])/TST_SZ);

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
  }
  std::nth_element(&dtv[0], &dtv[NITER/2], &dtv[NITER]);
  printf("%.2f clocks. Half-naive base 100.\n", double(dtv[NITER/2])/TST_SZ);

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
  }
  std::nth_element(&dtv[0], &dtv[NITER/2], &dtv[NITER]);
  printf("%.2f clocks. Half-naive base 1000.\n", double(dtv[NITER/2])/TST_SZ);

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
  }
  std::nth_element(&dtv[0], &dtv[NITER/2], &dtv[NITER]);
  printf("%.2f clocks. Joshua's base 100.\n", double(dtv[NITER/2])/TST_SZ);

  if (dummy==42)
    printf("Blue moon\n");

  return 0;
}


