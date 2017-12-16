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

//
// Imitate icc18
// It would be much simpler to use "basic" inline ASM, but then the function would be tied to particylar ABI.
// "Advanced" as is harder (in this particular case), but it works both with Microsoft and with System V ABIs.
//
void uint64_to_ascii_ta_base100_icc(uint64_t val, char* dst)
{
  uint64_t rcx, rax, r8, rdx, r10, r9, rsi = (uint64_t)dst;
  uint32_t r11d, r9d, r8d, ecx;
  uint16_t r11w, r9w, r8w, cx;
  asm("movq      $0xdbe6fecebdedd5bf, %0 \n" : "=r"(rcx) );               // movq      $0xdbe6fecebdedd5bf, %rcx
  asm("movq      %1, %0                  \n" : "=a"(rax) : "r"(val));     // movq      %rdi, %rax
  asm("movq      $0x2540be400, %0        \n" : "=r"(r8) );                // movq      $0x2540be400, %r8
  asm("mulq      %2                      \n" : "=d"(rdx), "+a"(rax) : "r"(rcx) );    // mulq      %rcx
  asm("shrq      $33, %0                 \n" : "+d"(rdx) );               // shrq      $33, %rdx
  asm("imulq     $1441151881, %1, %0     \n" : "=r"(rcx) : "d"(rdx));     // imulq     $1441151881, %rdx, %rcx
  asm("imulq     %1, %0                  \n" : "+r"(r8)  : "d"(rdx));     // imulq     %rdx, %r8
  asm("movq      %1, %0                  \n" : "=r"(r10) : "r"(rcx));     // movq      %rcx, %r10
  asm("subq      %1, %0                  \n" : "+r"(val) : "r"(r8));      // subq      %r8, %rdi
  asm("shrq      $57, %0                 \n" : "+r"(r10) );               // shrq      $57, %r10
  asm("movq      %1, %0                  \n" : "=r"(r9)  : "r"(val));     // movq      %rdi, %r9
  asm("andq      $-2, %0                 \n" : "+r"(r9) );                // andq      $-2, %r9
  asm("imulq     $1441151881, %1, %0     \n" : "=d"(rdx) : "r"(r9));      // imulq     $1441151881, %r9, %rdx
  asm("movzwl    b100_tab(,%1,2), %0     \n" : "=r"(r11d): "r"(r10));     // movzwl    b100_tab(,%r10,2), %r11d
  r11w = r11d;
  asm("movw      %1, (%0)                \n" : : "r"(rsi), "r"(r11w));    // movw      %r11w, (%rsi)
  asm("movq      %1, %0                  \n" : "=r"(r8)  : "r"(rdx));     // movq      %rdx, %r8
  asm("movq      $0x1ffffffffffffff, %0  \n" : "=a"(rax) );               // movq      $0x1ffffffffffffff, %rax
  asm("shrq      $57, %0                 \n" : "+r"(r8)  );               // shrq      $57, %r8
  asm("andq      %1, %0                  \n" : "+r"(rcx) : "a"(rax));     // andq      %rax, %rcx
  asm("imulq     $100, %0                \n" : "+r"(rcx) );               // imulq     $100, %rcx, %rcx
  asm("movzwl    b100_tab(,%1,2), %0     \n" : "=r"(r9d) : "r"(r8));      // movzwl    b100_tab(,%r8,2), %r9d
  r9w = r9d;
  asm("movw      %1, 10(%0)              \n" : : "r"(rsi), "r"(r9w));     // movw      %r9w, 10(%rsi)
  asm("movq      %1, %0                  \n" : "=r"(r8)  : "r"(rcx));     // movq      %rcx, %r8
  asm("andq      %1, %0                  \n" : "+r"(rdx) : "r"(rax));     // andq      %rax, %rdx
  asm("shrq      $57,%0                  \n" : "+r"(r8) );                // shrq      $57, %r8
  asm("imulq     $100, %0, %0            \n" : "+r"(rdx));                // imulq     $100, %rdx, %rdx
  asm("movzwl    b100_tab(,%1,2), %0     \n" : "=r"(r9d) : "r"(r8) );     // movzwl    b100_tab(,%r8,2), %r9d
  r9w = r9d;
  asm("movw      %1, 2(%0)               \n" : : "r"(rsi), "r"(r9w));     // movw      %r9w, 2(%rsi)
  asm("movq      %1, %0                  \n" : "=r"(r8)  : "r"(rdx));     // movq      %rdx, %r8
  asm("andq      %1, %0                  \n" : "+r"(rcx) : "r"(rax));     // andq      %rax, %rcx
  asm("shrq      $57, %0                 \n" : "+r"(r8) );                // shrq      $57, %r8
  asm("imulq     $100, %0, %0            \n" : "+r"(rcx));                // imulq     $100, %rcx, %rcx
  asm("movzwl    b100_tab(,%1,2), %0     \n" : "=r"(r9d) : "r"(r8) );     // movzwl    b100_tab(,%r8,2), %r9d
  r9w = r9d;
  asm("movw      %1, 12(%0)              \n" : : "r"(rsi), "r"(r9w));     // movw      %r9w, 12(%rsi)
  asm("movq      %1, %0                  \n" : "=r"(r8)  : "r"(rcx));     // movq      %rcx, %r8
  asm("andq      %1, %0                  \n" : "+r"(rdx) : "r"(rax));     // andq      %rax, %rdx
  asm("shrq      $57, %0                 \n" : "+r"(r8) );                // shrq      $57, %r8
  asm("imulq     $100, %0, %0            \n" : "+r"(rdx));                // imulq     $100, %rdx, %rdx
  asm("movzwl    b100_tab(,%1,2), %0     \n" : "=r"(r9d) : "r"(r8));      // movzwl    b100_tab(,%r8,2), %r9d
  r9w = r9d;
  asm("movw      %1, 4(%0)               \n" : : "r"(rsi), "r"(r9w));     // movw      %r9w, 4(%rsi)
  asm("movq      %1, %0                  \n" : "=r"(r8)  : "r"(rdx));     // movq      %rdx, %r8
  asm("andq      %1, %0                  \n" : "+r"(rcx) : "r"(rax));     // andq      %rax, %rcx
  asm("shrq      $57, %0                 \n" : "+r"(r8) );                // shrq      $57, %r8
  asm("imulq     $100, %0, %0            \n" : "+r"(rcx));                // imulq     $100, %rcx, %rcx
  asm("movzwl    b100_tab(,%1,2), %0     \n" : "=r"(r9d) : "r"(r8) );     // movzwl    b100_tab(,%r8,2), %r9d
  r9w = r9d;
  asm("movw      %1, 14(%0)              \n" : : "r"(rsi), "r"(r9w));     // movw      %r9w, 14(%rsi)
  asm("movq      %1, %0                  \n" : "=r"(r8)  : "r"(rcx));     // movq      %rcx, %r8
  asm("andq      %1, %0                  \n" : "+r"(rdx) : "r"(rax));     // andq      %rax, %rdx
  asm("shrq      $57, %0                 \n" : "+r"(r8)) ;                // shrq      $57, %r8
  asm("imulq     $100, %0, %0            \n" : "+r"(rdx));                // imulq     $100, %rdx, %rdx
  asm("movzwl    b100_tab(,%1,2), %0     \n" : "=r"(r9d) : "r"(r8) );     // movzwl    b100_tab(,%r8,2), %r9d
  r9w = r9d;
  asm("movw      %1, 6(%0)               \n" : : "r"(rsi), "r"(r9w));     // movw      %r9w, 6(%rsi)
  asm("movq      %1, %0                  \n" : "=r"(r8)  : "r"(rdx));     // movq      %rdx, %r8
  asm("shrq      $57, %0                 \n" : "+r"(r8) );                // shrq      $57, %r8
  asm("movzwl    b100_tab(,%1,2), %0     \n" : "=r"(r9d) : "r"(r8) );     // movzwl    b100_tab(,%r8,2), %r9d
  r9w = r9d;
  asm("movw      %1, 16(%0)              \n" : : "r"(rsi), "r"(r9w) );    // movw      %r9w, 16(%rsi)
  asm("andq      %1, %0                  \n" : "+r"(rcx) : "r"(rax));               // andq      %rax, %rcx
  asm("imulq     $100, %0, %0            \n" : "+r"(rcx));                // imulq     $100, %rcx, %rcx
  asm("shrq      $57, %0                 \n" : "+r"(rcx));                // shrq      $57, %rcx
  asm("movzwl    b100_tab(,%1,2), %0     \n" : "=r"(r8d): "r"(rcx));      // movzwl    b100_tab(,%rcx,2), %r8d
  r8w = r8d;
  asm("movw      %1, 8(%0)               \n" : : "r"(rsi), "r"(r8w) );    // movw      %r8w, 8(%rsi)
  asm("andq      %1, %0                  \n" : "+r"(rax) : "r"(rdx) );    // andq      %rdx, %rax
  asm("andq      $1, %0                  \n" : "+r"(val));                // andq      $1, %rdi
  asm("imulq     $100, %1, %0            \n" : "=r"(rdx): "rx"(rax));               // imulq     $100, %rax, %rdx
  asm("shrq      $57, %0                 \n" : "+r"(rdx));                // shrq      $57, %rdx
  asm("andq      $-2, %0                 \n" : "+r"(rdx) );               // andq      $-2, %rdx
  asm("orq       %1, %0                  \n" : "+r"(rdx): "r"(val));      // orq       %rdi, %rdx
  asm("movzwl    b100_tab(,%1,2), %0     \n" : "=r"(ecx): "r"(rdx));      // movzwl    b100_tab(,%rdx,2), %ecx
  cx = ecx;
  asm("movw      %1, 18(%0)              \n" : : "r"(rsi), "r"(cx) );     // movw      %cx, 18(%rsi)
  asm("movb      $0, 20(%0)              \n" : : "r"(rsi));               // movb      $0, 20(%rsi)
}

const char* dummy(void) { return b100_tab; }

