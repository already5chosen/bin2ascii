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

void uint64_to_ascii_3chains_base100(uint64_t val, char* dst)
{

  const uint64_t POW10_6  = 1000*1000;
  const uint64_t POW10_12 = POW10_6*POW10_6;
  const uint64_t POW10_18 = POW10_12*POW10_6;
  const uint64_t DIV_POW10_18 = ((unsigned __int128)1 << 123)/POW10_18 + 1;
  const uint64_t DIV_POW10_12 = ((unsigned __int128)1 << 103)/POW10_12 + 1;
  const uint64_t DIV_POW10_6  = ((unsigned __int128)1 <<  83)/POW10_6  + 1;

  uint64_t valx_l = ((unsigned __int128)DIV_POW10_6  * val  ) >> 49;
  uint64_t val_e  = val & (-64); // val with 6 LS bits cleared
  uint64_t valx_m = ((unsigned __int128)DIV_POW10_12 * val_e) >> 69;
  uint64_t valx_h = ((unsigned __int128)DIV_POW10_18 * val_e) >> 64;
  memcpy(&dst[0],  &b100_tab[(valx_h >> 59)*2], 2);

  valx_l += 1;       // valx_l >= (val/10^6 )*2^34
  valx_m += 1;       // valx_m >= (val_e/10^12)*2^34
  valx_h += 1;       // valx_h >= (val_e/10^18)*2^59

  uint64_t mask_h = ((uint64_t)1 << 59) - 1;
  uint64_t mask_l = ((uint64_t)1 << 34) - 1;
  #if 0
  for (int i = 0; i < 6; i += 2) {
    valx_h   = (valx_h & mask_h) * 25;
    valx_m   = (valx_m & mask_l) * 25;
    valx_l   = (valx_l & mask_l) * 25;
    mask_h >>= 2;
    mask_l >>= 2;
    memcpy(&dst[i+ 2], &b100_tab[(valx_h >> (57-i))*2], 2);
    memcpy(&dst[i+ 8], &b100_tab[(valx_m >> (32-i))*2], 2);
    memcpy(&dst[i+14], &b100_tab[(valx_l >> (32-i))*2], 2);
  }
  #else
  // gcc -O2 does not unroll an above version

  valx_h   = (valx_h & mask_h) * 25;
  valx_m   = (valx_m & mask_l) * 25;
  valx_l   = (valx_l & mask_l) * 25;
  mask_h >>= 2;
  mask_l >>= 2;
  memcpy(&dst[ 2], &b100_tab[(valx_h >> 57)*2], 2);
  memcpy(&dst[ 8], &b100_tab[(valx_m >> 32)*2], 2);
  memcpy(&dst[14], &b100_tab[(valx_l >> 32)*2], 2);

  valx_h   = (valx_h & mask_h) * 100;
  valx_m   = (valx_m & mask_l) * 100;
  valx_l   = (valx_l & mask_l) * 100;
  memcpy(&dst[ 4], &b100_tab[(valx_h >> 57)*2], 2);
  memcpy(&dst[10], &b100_tab[(valx_m >> 32)*2], 2);
  memcpy(&dst[16], &b100_tab[(valx_l >> 32)*2], 2);

  valx_h   = (valx_h & mask_h) * 100;
  valx_m   = (valx_m & mask_l) * 100;
  valx_l   = (valx_l & mask_l) * 100;
  memcpy(&dst[ 6], &b100_tab[(valx_h >> 57)*2], 2);
  memcpy(&dst[12], &b100_tab[(valx_m >> 32)*2], 2);
  memcpy(&dst[18], &b100_tab[(valx_l >> 32)*2], 2);

  #endif

  dst[20] = 0;
}
