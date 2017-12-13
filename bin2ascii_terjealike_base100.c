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

void uint64_to_ascii_ta_base100(uint64_t val, char* dst)
{
  const int64_t POW10_10 = ((int64_t)10)*1000*1000*1000;
  const int64_t POW2_57_DIV_POW100_4 = ((int64_t)(1) << 57)/100/100/100/100 + 1;
  const int64_t MASK55 = ((int64_t)(1) << 55) - 1;
  int64_t hix = val / POW10_10; // digits[0..9]
  int64_t lox = val % POW10_10; // digits[10..19]

  // Strip two LS bits of each half. They will be added back at last step.
  // Removal of LS bits bring values under 2**32, which allow to calculate
  // a sufficiently precise 7.55 representation without using 64x64=>128 multiplication.
  // Not that there is something wrong with 64x64=>128. It works great both
  // on both targets architectures that matter, but it's impossible to express in standard 'C'.
  // And separation of LS bits likely not much slower...
  int64_t hi = (hix >> 2)*POW2_57_DIV_POW100_4; // fox-point 7.55 format. 2 MS bits unused
  int64_t lo = (lox >> 2)*POW2_57_DIV_POW100_4;

  memcpy(dst+0*10+0, &b100_tab[(hi >> 55)*2], 2); hi = (hi & MASK55) * 100;
  memcpy(dst+1*10+0, &b100_tab[(lo >> 55)*2], 2); lo = (lo & MASK55) * 100;

  memcpy(dst+0*10+2, &b100_tab[(hi >> 55)*2], 2); hi = (hi & MASK55) * 100;
  memcpy(dst+1*10+2, &b100_tab[(lo >> 55)*2], 2); lo = (lo & MASK55) * 100;

  memcpy(dst+0*10+4, &b100_tab[(hi >> 55)*2], 2); hi = (hi & MASK55) * 100;
  memcpy(dst+1*10+4, &b100_tab[(lo >> 55)*2], 2); lo = (lo & MASK55) * 100;

  memcpy(dst+0*10+6, &b100_tab[(hi >> 55)*2], 2); hi = (hi & MASK55) * 100;
  memcpy(dst+1*10+6, &b100_tab[(lo >> 55)*2], 2); lo = (lo & MASK55) * 100;

  // restore LS bits
  hi = ((hi >> 55) & (-4)) | (hix & 3);
  lo = ((lo >> 55) & (-4)) | (lox & 3);

  memcpy(dst+0*10+8, &b100_tab[hi*2], 2);
  memcpy(dst+1*10+8, &b100_tab[lo*2], 2);

  dst[2*10] = 0;
}