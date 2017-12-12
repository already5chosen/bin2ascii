/*
 Written by Joshua L.
 Seems like he does not care about copyright.
*/

#include <stdint.h>
#include <string.h>

const char DIGIT_PAIRS[] =
    "00" "01" "02" "03" "04" "05" "06" "07" "08" "09"
    "10" "11" "12" "13" "14" "15" "16" "17" "18" "19"
    "20" "21" "22" "23" "24" "25" "26" "27" "28" "29"
    "30" "31" "32" "33" "34" "35" "36" "37" "38" "39"
    "40" "41" "42" "43" "44" "45" "46" "47" "48" "49"
    "50" "51" "52" "53" "54" "55" "56" "57" "58" "59"
    "60" "61" "62" "63" "64" "65" "66" "67" "68" "69"
    "70" "71" "72" "73" "74" "75" "76" "77" "78" "79"
    "80" "81" "82" "83" "84" "85" "86" "87" "88" "89"
    "90" "91" "92" "93" "94" "95" "96" "97" "98" "99";

size_t __attribute__ ((noinline)) print_buf64(char *buffer, uint64_t value) {
    char digits[20];

    uint64_t top12 = value / UINT64_C(100000000);
    uint32_t uvalue_0 = value - top12 * 100000000;
    uint32_t uvalue_1 = (uvalue_0 * UINT64_C(1374389535)) >> 37;
    uint32_t digit_pair_0 = uvalue_0 * 2 - uvalue_1 * 200;
    memcpy(digits + 18, &DIGIT_PAIRS[digit_pair_0], 2);

    uint32_t uvalue_2 = (uvalue_0 * UINT64_C(3518437209)) >> 45;
    uint32_t digit_pair_1 = uvalue_1 * 2 - uvalue_2 * 200;
    memcpy(digits + 16, &DIGIT_PAIRS[digit_pair_1], 2);

    uint32_t uvalue_3 = (uvalue_0 * UINT64_C(1125899907)) >> 50;
    uint32_t digit_pair_2 = uvalue_2 * 2 - uvalue_3 * 200;
    memcpy(digits + 14, &DIGIT_PAIRS[digit_pair_2], 2);

    uint32_t digit_pair_3 = uvalue_3 * 2;
    memcpy(digits + 12, &DIGIT_PAIRS[digit_pair_3], 2);

    uint64_t top4 = value / UINT64_C(10000000000000000);
    uint32_t uvalue_4 = top12 - top4 * 100000000;
    uint32_t uvalue_5 = (uvalue_4 * UINT64_C(1374389535)) >> 37;
    uint32_t digit_pair_4 = uvalue_4 * 2 - uvalue_5 * 200;
    memcpy(digits + 10, &DIGIT_PAIRS[digit_pair_4], 2);

    uint32_t uvalue_6 = (uvalue_4 * UINT64_C(3518437209)) >> 45;
    uint32_t digit_pair_5 = uvalue_5 * 2 - uvalue_6 * 200;
    memcpy(digits +  8, &DIGIT_PAIRS[digit_pair_5], 2);

    uint32_t uvalue_7 = (uvalue_4 * UINT64_C(1125899907)) >> 50;
    uint32_t digit_pair_6 = uvalue_6 * 2 - uvalue_7 * 200;
    memcpy(digits +  6, &DIGIT_PAIRS[digit_pair_6], 2);

    uint32_t digit_pair_7 = uvalue_7 * 2;
    memcpy(digits +  4, &DIGIT_PAIRS[digit_pair_7], 2);

    uint32_t uvalue_8 = top4;
    uint32_t uvalue_9 = (uvalue_8 * UINT64_C(1374389535)) >> 37;
    uint32_t digit_pair_8 = uvalue_8 * 2 - uvalue_9 * 200;
    memcpy(digits +  2, &DIGIT_PAIRS[digit_pair_8], 2);

    uint32_t digit_pair_9 = uvalue_9 * 2;
    memcpy(digits +  0, &DIGIT_PAIRS[digit_pair_9], 2);

    memcpy(buffer, digits, 20);
    return 20;
}
