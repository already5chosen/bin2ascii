/*
Copyright (c) 2017 Terje Mathisen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
*/

#include <stdint.h>

/* This function will always generate 20 digits followed by a \0 byte */
#ifndef optimization_o3
char *uint64_to_ascii(uint64_t n, char *buf)
#else
char *uint64_to_ascii_o3(uint64_t n, char *buf)
#endif
{
  uint64_t top6 = n / 100000000000000LL;
  uint64_t top13 = n / 10000000LL;
  uint64_t middle7 = top13 - top6 * 10000000LL;
  uint64_t bottom7 = n - top13 * 10000000LL;

#define SCALE_4_28(n) ((n*(65536LL*65536*65536/1000000+1)>>20)+1)

  uint32_t nb = (uint32_t) SCALE_4_28(bottom7);
  uint32_t nm = (uint32_t) SCALE_4_28(middle7);
  uint32_t nt = (uint32_t) SCALE_4_28(top6);

  uint32_t mask = (1 << 28) - 1;
  //buf[ 0] = (char)(nt >> (28)) + '0'; // Always zero, so skip!
  buf[ 6] = (char)(nm >> (28)) + '0';
  buf[13] = (char)(nb >> (28)) + '0';
  for (unsigned i = 0; i < 6; i++) {
    nt &= mask; nm &= mask; nb &= mask;
    nt *= 5; nm *= 5; nb *= 5;
    mask >>= 1;
    buf[i + 0] = (char)(nt >> (27 - i)) + '0';
    buf[i + 7] = (char)(nm >> (27 - i)) + '0';
    buf[i + 14] = (char)(nb >> (27 - i)) + '0';
  }
  buf[20] = '\0';
  return buf;
}