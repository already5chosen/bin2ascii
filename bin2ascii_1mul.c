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
  uint64_t mask = MASK_60;
  for (int i = 1; i < 19; ++i) {
    dst[i] = (valx >> (61-i)) + '0';
    valx   = (valx & mask) * 5;
    mask >>= 1;
  }
  uint32_t dig19 = (valx >> (61-19));
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

  uint64_t mask = ((uint64_t)1 << 60) - 1;
  #define uint64_to_ascii_1mul_base10_step(rshift, dst_i, msk_shift_off) \
  asm ( \
    "mov  %0,  %1        \n" \
    "and  %3,  %0        \n" \
    "lea  (%0,%0,4), %0  \n" \
    msk_shift_off            \
    "shr  $1,  %3        \n" \
    "shr  $" rshift ", %1\n" \
    "add  $48, %1        \n" \
    "movb %b1, %2        \n" \
    : "+r"(valx), "=r"(res), "=m"(dst[dst_i]), "+r"(mask) : : "cc" \
  )

  uint64_to_ascii_1mul_base10_step("60", 1, "");
  uint64_to_ascii_1mul_base10_step("59", 2, "");
  uint64_to_ascii_1mul_base10_step("58", 3, "");
  uint64_to_ascii_1mul_base10_step("57", 4, "");
  uint64_to_ascii_1mul_base10_step("56", 5, "");
  uint64_to_ascii_1mul_base10_step("55", 6, "");
  uint64_to_ascii_1mul_base10_step("54", 7, "");
  uint64_to_ascii_1mul_base10_step("53", 8, "");
  uint64_to_ascii_1mul_base10_step("52", 9, "");
  uint64_to_ascii_1mul_base10_step("51",10, "");
  uint64_to_ascii_1mul_base10_step("50",11, "");
  uint64_to_ascii_1mul_base10_step("49",12, "");
  uint64_to_ascii_1mul_base10_step("48",13, "");
  uint64_to_ascii_1mul_base10_step("47",14, "");
  uint64_to_ascii_1mul_base10_step("46",15, "");
  uint64_to_ascii_1mul_base10_step("45",16, "");
  uint64_to_ascii_1mul_base10_step("44",17, "");
  uint64_to_ascii_1mul_base10_step("43",18, "# ");

  asm (
    "shr  $42, %0      \n"
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

  #define uint64_to_ascii_1mul_base10_step(rshift) \
  asm ( \
    "lea  (%0,%0), %1     \n" \
    "and  %3,  %1         \n" \
    "and  %4,  %0         \n" \
    "shr  $" rshift ", %2 \n" \
    "add  %0,  %2         \n" \
    "lea  (%1,%1,4), %0   \n" \
    : "+&r"(valx), "=&r"(valx2), "+&r"(res) : "r"(mask61), "r"(mask_h) : "cc" \
  )

  uint64_to_ascii_1mul_base10_step("11"); // +digit[1]
  uint64_to_ascii_1mul_base10_step("8");  // +digit[2]
  uint64_to_ascii_1mul_base10_step("8");  // +digit[3]
  uint64_to_ascii_1mul_base10_step("8");  // +digit[4]
  uint64_to_ascii_1mul_base10_step("8");  // +digit[5]
  uint64_to_ascii_1mul_base10_step("8");  // +digit[6]
  uint64_to_ascii_1mul_base10_step("8");  // +digit[7]

  uint64_t ascii0x8 = 0x3030303030303030ull;
  asm ( // store digit[0..7]
    "shr  $4,  %0\n"
    "or   %2,  %0\n"
    "mov  %0,  %1\n"
    : "+&r"(res), "=m"(dst[0]): "r"(ascii0x8) : "cc"
  );


  #define uint64_to_ascii_1mul_base10_ls_step() \
  asm ( \
    "mov  %0,  %1      \n" \
    "add  %0,  %0      \n" \
    "and  %2,  %0      \n" \
    "lea  (%0,%0,4), %0\n" \
    "and  %3,  %1      \n" \
    : "+&r"(valx), "=&r"(res) : "r"(mask61), "r"(mask_h) : "cc" \
  )

  uint64_to_ascii_1mul_base10_ls_step();  // +digit[8]
  uint64_to_ascii_1mul_base10_step("8");  // +digit[9]
  uint64_to_ascii_1mul_base10_step("8");  // +digit[10]
  uint64_to_ascii_1mul_base10_step("8");  // +digit[11]
  uint64_to_ascii_1mul_base10_step("8");  // +digit[12]
  uint64_to_ascii_1mul_base10_step("8");  // +digit[13]
  uint64_to_ascii_1mul_base10_step("8");  // +digit[14]
  uint64_to_ascii_1mul_base10_step("8");  // +digit[15]

  asm ( // store digit[8..15]
    "shr  $4,  %0\n"
    "or   %2,  %0\n"
    "mov  %0,  %1\n"
    : "+&r"(res), "=m"(dst[8]): "r"(ascii0x8) : "cc"
  );

  uint64_to_ascii_1mul_base10_ls_step();  // +digit[16]
  uint64_to_ascii_1mul_base10_step("8");  // +digit[17]
  uint64_to_ascii_1mul_base10_step("8");  // +digit[18]
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
