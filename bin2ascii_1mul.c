#include <stdint.h>
#include <string.h>

void uint64_to_ascii_1mul_base10(uint64_t val, char* dst)
{
  const uint64_t POW10_9  = 1000*1000*1000;
  const uint64_t POW10_18 = POW10_9 * POW10_9;
  const uint64_t POW10_19 = POW10_18 * 10;
  const uint64_t DIV_POW10_18 = ((unsigned __int128)1 << 123)/POW10_18 + 1;
#if 1
  const uint64_t MASK_60 = ((uint64_t)1 << 60) - 1;

  int msb = (val >= POW10_19);
  uint64_t msbAdj = msb ? POW10_19 : 0;
  val -= msbAdj;
  dst[0] = msb + '0';

  uint64_t val_e = val & (-2); // even part of val
  uint64_t valx = ((unsigned __int128)DIV_POW10_18 * val_e) >> 63;
  valx += 1;  // valx >= (val_e/10^19)*2^60
  for (int i = 1; i < 19; ++i) {
    dst[i] = (valx >> 60) + '0';
    valx   = (valx & MASK_60) * 10;
  }
  uint32_t dig19 = (valx >> 60);
  dig19 = (dig19 & (-2)) | (val & 1);
  dst[19] = dig19 + '0';

  dst[20] = 0;
#elif 1
  uint64_t val_a = 0 - POW10_19;
  uint64_t res   = 0;
  asm  (
    "add   %1, %0\n"
    "cmovc %0, %1\n"
    "adc   %2, %2\n"
    "add   $48,%2\n"
    "movb  %b2,%3\n"
     : "+r"(val_a), "+r"(val), "+r"(res), "=m"(dst[0]) :
  );

  uint64_t valx = DIV_POW10_18, valx_l;
  asm  (
    "mov  %2,    %0 \n"
    "and  $(-2), %0 \n"
    "mulq %1        \n"
    "shld $1, %0, %1\n"
    "inc  %1        \n"
    : "=a"(valx_l), "+d"(valx) : "r"(val)
  );

  uint64_t mask61 = ((uint64_t)1 << 61) - 1;
  uint64_t valx2;

  asm (
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "shr  $60, %0      \n"
    "add  $48, %0      \n"
    "movb %b0, %2      \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "=r"(valx2), "=m"(dst[1]) : "r"(mask61) : "cc"
  );
  asm (
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "shr  $60, %0      \n"
    "add  $48, %0      \n"
    "movb %b0, %2   \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "+r"(valx2), "=m"(dst[2]) : "r"(mask61) : "cc"
  );
  asm (
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "shr  $60, %0      \n"
    "add  $48, %0      \n"
    "movb %b0, %2   \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "+r"(valx2), "=m"(dst[3]) : "r"(mask61) : "cc"
  );
  asm (
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "shr  $60, %0      \n"
    "add  $48, %0      \n"
    "movb %b0, %2   \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "+r"(valx2), "=m"(dst[4]) : "r"(mask61) : "cc"
  );
  asm (
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "shr  $60, %0      \n"
    "add  $48, %0      \n"
    "movb %b0, %2   \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "+r"(valx2), "=m"(dst[5]) : "r"(mask61) : "cc"
  );
  asm (
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "shr  $60, %0      \n"
    "add  $48, %0      \n"
    "movb %b0, %2   \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "+r"(valx2), "=m"(dst[6]) : "r"(mask61) : "cc"
  );
  asm (
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "shr  $60, %0      \n"
    "add  $48, %0      \n"
    "movb %b0, %2   \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "+r"(valx2), "=m"(dst[7]) : "r"(mask61) : "cc"
  );
  asm (
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "shr  $60, %0      \n"
    "add  $48, %0      \n"
    "movb %b0, %2   \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "+r"(valx2), "=m"(dst[8]) : "r"(mask61) : "cc"
  );
  asm (
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "shr  $60, %0      \n"
    "add  $48, %0      \n"
    "movb %b0, %2   \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "+r"(valx2), "=m"(dst[9]) : "r"(mask61) : "cc"
  );
  asm (
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "shr  $60, %0      \n"
    "add  $48, %0      \n"
    "movb %b0, %2   \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "+r"(valx2), "=m"(dst[10]) : "r"(mask61) : "cc"
  );
  asm (
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "shr  $60, %0      \n"
    "add  $48, %0      \n"
    "movb %b0, %2   \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "+r"(valx2), "=m"(dst[11]) : "r"(mask61) : "cc"
  );
  asm (
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "shr  $60, %0      \n"
    "add  $48, %0      \n"
    "movb %b0, %2   \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "+r"(valx2), "=m"(dst[12]) : "r"(mask61) : "cc"
  );
  asm (
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "shr  $60, %0      \n"
    "add  $48, %0      \n"
    "movb %b0, %2   \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "+r"(valx2), "=m"(dst[13]) : "r"(mask61) : "cc"
  );
  asm (
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "shr  $60, %0      \n"
    "add  $48, %0      \n"
    "movb %b0, %2   \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "+r"(valx2), "=m"(dst[14]) : "r"(mask61) : "cc"
  );
  asm (
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "shr  $60, %0      \n"
    "add  $48, %0      \n"
    "movb %b0, %2   \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "+r"(valx2), "=m"(dst[15]) : "r"(mask61) : "cc"
  );
  asm (
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "shr  $60, %0      \n"
    "add  $48, %0      \n"
    "movb %b0, %2   \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "+r"(valx2), "=m"(dst[16]) : "r"(mask61) : "cc"
  );
  asm (
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "shr  $60, %0      \n"
    "add  $48, %0      \n"
    "movb %b0, %2   \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "+r"(valx2), "=m"(dst[17]) : "r"(mask61) : "cc"
  );
  asm (
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "shr  $60, %0      \n"
    "add  $48, %0      \n"
    "movb %b0, %2      \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "+r"(valx2), "=m"(dst[18]) : "r"(mask61) : "cc"
  );
  asm (
    "shr  $60, %0      \n"
    "and  $-2, %0      \n"
    "and  $1,  %1      \n"
    "lea  48(%0,%1),%0\n"
    "movb %b0, %2      \n"
    "movb $0,  %3      \n"
    : "+r"(valx), "+r"(val), "=m"(dst[19]), "=m"(dst[20]) : : "cc"
  );
#else
  uint64_t val_a = 0 - POW10_19;
  uint64_t res   = 0;
  asm  (
    "add   %1, %0\n"
    "cmovc %0, %1\n"
    "sbb   %2, %2\n"
     : "+r"(val_a), "+r"(val), "+r"(res) : : "cc"
  );

  uint64_t valx = DIV_POW10_18, valx_l;
  asm  (
    "mov  %2,    %0 \n"
    "and  $(-2), %0 \n"
    "mulq %1        \n"
    "shld $1, %0, %1\n"
    "inc  %1        \n"
    : "=a"(valx_l), "+d"(valx) : "r"(val)
  );

  uint64_t mask61 = ((uint64_t)1 << 61) - 1;
  uint64_t mask_h =  (uint64_t)15 << 60;
  uint64_t valx2;

  asm ( // +digit[1]
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "and  %4,  %0      \n"
    "shr  $11, %2      \n"
    "add  %0,  %2      \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "=r"(valx2), "+r"(res) : "r"(mask61), "r"(mask_h) : "cc"
  );
  asm ( // +digit[2]
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "and  %4,  %0      \n"
    "shr  $8,  %2      \n"
    "add  %0,  %2      \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "+r"(valx2), "+r"(res) : "r"(mask61), "r"(mask_h) : "cc"
  );
  asm ( // +digit[3]
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "and  %4,  %0      \n"
    "shr  $8,  %2      \n"
    "add  %0,  %2      \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "+r"(valx2), "+r"(res) : "r"(mask61), "r"(mask_h) : "cc"
  );
  asm ( // +digit[4]
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "and  %4,  %0      \n"
    "shr  $8,  %2      \n"
    "add  %0,  %2      \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "+r"(valx2), "+r"(res) : "r"(mask61), "r"(mask_h) : "cc"
  );
  asm ( // +digit[5]
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "and  %4,  %0      \n"
    "shr  $8,  %2      \n"
    "add  %0,  %2      \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "+r"(valx2), "+r"(res) : "r"(mask61), "r"(mask_h) : "cc"
  );
  asm ( // +digit[6]
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "and  %4,  %0      \n"
    "shr  $8,  %2      \n"
    "add  %0,  %2      \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "+r"(valx2), "+r"(res) : "r"(mask61), "r"(mask_h) : "cc"
  );
  asm ( // +digit[7]
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "and  %4,  %0      \n"
    "shr  $8,  %2      \n"
    "add  %0,  %2      \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "+r"(valx2), "+r"(res) : "r"(mask61), "r"(mask_h) : "cc"
  );

  uint64_t ascii0x8 = 0x3030303030303030ull;
  asm ( // store digit[0..7]
    "shr  $4,  %0\n"
    "or   %2,  %0\n"
    "mov  %0,  %1\n"
    : "+r"(res), "=m"(dst[0]): "r"(ascii0x8) : "cc"
  );

  asm ( // +digit[8]
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "and  %4,  %0      \n"
    "mov  %0,  %2      \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "+r"(valx2), "=r"(res) : "r"(mask61), "r"(mask_h) : "cc"
  );
  asm ( // +digit[9]
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "and  %4,  %0      \n"
    "shr  $8,  %2      \n"
    "add  %0,  %2      \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "+r"(valx2), "+r"(res) : "r"(mask61), "r"(mask_h) : "cc"
  );
  asm ( // +digit[10]
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "and  %4,  %0      \n"
    "shr  $8,  %2      \n"
    "add  %0,  %2      \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "+r"(valx2), "+r"(res) : "r"(mask61), "r"(mask_h) : "cc"
  );
  asm ( // +digit[11]
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "and  %4,  %0      \n"
    "shr  $8,  %2      \n"
    "add  %0,  %2      \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "+r"(valx2), "+r"(res) : "r"(mask61), "r"(mask_h) : "cc"
  );
  asm ( // +digit[12]
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "and  %4,  %0      \n"
    "shr  $8,  %2      \n"
    "add  %0,  %2      \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "+r"(valx2), "+r"(res) : "r"(mask61), "r"(mask_h) : "cc"
  );
  asm ( // +digit[13]
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "and  %4,  %0      \n"
    "shr  $8,  %2      \n"
    "add  %0,  %2      \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "+r"(valx2), "+r"(res) : "r"(mask61), "r"(mask_h) : "cc"
  );
  asm ( // +digit[14]
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "and  %4,  %0      \n"
    "shr  $8,  %2      \n"
    "add  %0,  %2      \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "+r"(valx2), "+r"(res) : "r"(mask61), "r"(mask_h) : "cc"
  );
  asm ( // +digit[15]
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "and  %4,  %0      \n"
    "shr  $8,  %2      \n"
    "add  %0,  %2      \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "+r"(valx2), "+r"(res) : "r"(mask61), "r"(mask_h) : "cc"
  );

  asm ( // store digit[8..15]
    "shr  $4,  %0\n"
    "or   %2,  %0\n"
    "mov  %0,  %1\n"
    : "+r"(res), "=m"(dst[8]): "r"(ascii0x8) : "cc"
  );

  asm ( // +digit[16]
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "and  %4,  %0      \n"
    "mov  %0,  %2      \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "+r"(valx2), "=r"(res) : "r"(mask61), "r"(mask_h) : "cc"
  );
  asm ( // +digit[17]
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "and  %4,  %0      \n"
    "shr  $8,  %2      \n"
    "add  %0,  %2      \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "+r"(valx2), "+r"(res) : "r"(mask61), "r"(mask_h) : "cc"
  );
  asm ( // +digit[18]
    "lea  (%0,%0), %1  \n"
    "and  %3,  %1      \n"
    "and  %4,  %0      \n"
    "shr  $8,  %2      \n"
    "add  %0,  %2      \n"
    "lea  (%1,%1,4), %0\n"
    : "+r"(valx), "+r"(valx2), "+r"(res) : "r"(mask61), "r"(mask_h) : "cc"
  );
  asm ( // +digit[19]
    "and  %2,  %0      \n"
    "shr  $8,  %1      \n"
    "add  %0,  %1      \n"
    : "+r"(valx), "+r"(res) : "r"(mask_h) : "cc"
  );

  asm ( // store digit[16..20]
    "shr  $36, %0         \n"
    "and  $0xfeffffff, %k0\n"
    "or   %k4, %k0        \n"
    "and  $1,  %k1        \n"
    "shl  $24, %k1        \n"
    "add  %k1, %k0        \n"
    "mov  %k0, %2         \n"
    "movb $0,  %3         \n"
    : "+r"(res), "+r"(val), "=m"(dst[16]), "=m"(dst[20]): "r"(ascii0x8) : "cc"
  );


#endif
}
