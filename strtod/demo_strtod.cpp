#include <stdint.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_bin_float.hpp>

const int STRTOD_NBITS = 1500;

typedef
boost::multiprecision::number<boost::multiprecision::backends::cpp_bin_float<STRTOD_NBITS+4, boost::multiprecision::backends::digit_base_2> >
strtod_float_t;

typedef
boost::multiprecision::number<boost::multiprecision::backends::cpp_int_backend<STRTOD_NBITS, STRTOD_NBITS, boost::multiprecision::unsigned_magnitude, boost::multiprecision::unchecked, void> >
strtod_uint_t;

// return # of digits
static int readDecimalDigits(const char* src, strtod_uint_t& val, int* pnUsed, int* sticky)
{
  static const strtod_uint_t MAX_VAL = (std::numeric_limits<strtod_uint_t>::max()-9)/10;
  int nd, nUsed = 0;
  unsigned lsb = 0;
  for (nd = 0; ; ++nd) {
    unsigned c = src[nd];
    unsigned dig = c - '0';
    if (dig > 9)
      break;
    if (val <= MAX_VAL) {
      val = val*10+dig;
      ++nUsed;
    } else {
      lsb |= dig;
    }
  }
  *sticky |= (lsb != 0); // for tie breaker
  *pnUsed = nUsed;
  return nd;
}

const char *skipWhiteSpaces(const char *str)
{
  // skip leading white spaces
  while (*str && *str <= ' ') ++str;
  return str;
}

double demo_strtod(const char* str, char** endptr)
{
  const char* p = skipWhiteSpaces(str);
  int neg = 0;
  switch (p[0]) {
    case '+': ++p;          break;
    case '-': ++p; neg = 1; break;
    default:                break;
  }

  strtod_uint_t mant = 0;
  int nUsedInt, sticky=0;
  int ni = readDecimalDigits(p, mant, &nUsedInt, &sticky);

  int nf = 0;
  int nUsedFract = 0;
  if (p[ni]=='.')
    nf = readDecimalDigits(&p[ni+1], mant, &nUsedFract, &sticky);

  if (ni == 0 && nf == 0) {
    // conversion failed
    if (endptr)
      *endptr = (char*)str;
    return 0;
  }

  int exp =  ni - nUsedInt - nUsedFract;
  int nm = ni + nf + (p[ni]=='.');
  const char* endptrval = &p[nm];
  if (endptrval[0] == 'e' || endptrval[0] == 'E') {
    p = endptrval + 1;
    int nege = 0;
    switch (p[0]) {
      case '+': ++p;           break;
      case '-': ++p; nege = 1; break;
      default:                 break;
    }

    strtod_uint_t absExpVal = 0;
    int dummy1, dummy2;
    int ne = readDecimalDigits(p, absExpVal, &dummy1, &dummy2);
    if (ne != 0) {
      endptrval = p + ne;

      int64_t expVal = absExpVal < std::numeric_limits<int64_t>::max()/2 ?
        static_cast<int64_t>(absExpVal) :
        std::numeric_limits<int64_t>::max()/2;
      if (nege)
        expVal = -expVal;
      expVal += exp;

      const int MAX_EXP = 310;
      const int MIN_EXP = -325-STRTOD_NBITS/3;
      if (expVal < MIN_EXP)
        exp = MIN_EXP;
      else if  (expVal > MAX_EXP)
        exp = MAX_EXP;
      else
        exp = expVal;
    }
  }

  double retval = 0;
  if (mant != 0) {
    if (exp != 0) {
      strtod_float_t fVal = static_cast<strtod_float_t>(mant);
      fVal *= pow(strtod_float_t(10.0), exp);
      retval = static_cast<double>(fVal);
      // the following complicated code is here because I don't know
      // how to set LS bit of mantissa of cpp_bin_float
      if (sticky != 0 && retval < DBL_MAX && retval < fVal) {
        double nxt = nextafter(retval, DBL_MAX);
        if (fVal-retval == nxt-fVal)
          retval = nxt;
      }
    } else {
      if (sticky != 0)
        mant |= 1;
      retval = static_cast<double>(mant);
    }
  }

  if (endptr)
    *endptr = (char*)endptrval;
  return neg ? -retval : retval;
}
