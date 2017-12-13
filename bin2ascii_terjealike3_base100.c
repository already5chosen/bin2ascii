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

void uint64_to_ascii_ta3_base100(uint64_t val, char* dst)
{
  const int64_t POW10_6 = 1000*1000;
  int64_t hx = val / POW10_6; // digits [0..13]
  int32_t lo = val % POW10_6; // digits [14..19]
  int32_t hi = hx  / POW10_6; // digits [0..7]
  int32_t mi = hx  % POW10_6; // digits [8..13]

  const int64_t POW2_50_DIV_POW100_3 = ((int64_t)(1) << 50)/100/100/100 + 1;
  const int64_t MASK50 = ((int64_t)(1) << 50) - 1;
  const int64_t POW2_44_DIV_POW100_2 = ((int64_t)(1) << 44)/100/100 + 1;
  const int64_t MASK44 = ((int64_t)(1) << 44) - 1;

  int64_t fxhi = hi*POW2_50_DIV_POW100_3; // fix-point 7.50 format. 7 MS bits unused
  int64_t fxmi = mi*POW2_44_DIV_POW100_2; // fix-point 7.44 format. 13 MS bits unused
  int64_t fxlo = lo*POW2_44_DIV_POW100_2; // fix-point 7.44 format. 13 MS bits unused

  memcpy(dst+ 0, &b100_tab[(fxhi >> 50)*2], 2); fxhi = (fxhi & MASK50) * 100;
  memcpy(dst+ 8, &b100_tab[(fxmi >> 44)*2], 2); fxmi = (fxmi & MASK44) * 100;
  memcpy(dst+14, &b100_tab[(fxlo >> 44)*2], 2); fxlo = (fxlo & MASK44) * 100;

  memcpy(dst+ 2, &b100_tab[(fxhi >> 50)*2], 2); fxhi = (fxhi & MASK50) * 100;
  memcpy(dst+10, &b100_tab[(fxmi >> 44)*2], 2); fxmi = (fxmi & MASK44) * 100;
  memcpy(dst+16, &b100_tab[(fxlo >> 44)*2], 2); fxlo = (fxlo & MASK44) * 100;

  memcpy(dst+ 4, &b100_tab[(fxhi >> 50)*2], 2); fxhi = (fxhi & MASK50) * 100;
  memcpy(dst+12, &b100_tab[(fxmi >> 44)*2], 2);
  memcpy(dst+18, &b100_tab[(fxlo >> 44)*2], 2);

  memcpy(dst+ 6, &b100_tab[(fxhi >> 50)*2], 2);

  dst[20] = 0;
}