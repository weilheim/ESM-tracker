
#ifdef __sun__
#define BigEndian 1
#else
#define BigEndian 0
#endif


const double Real2Int_double2fixmagic = 68719476736.0*1.5;
const int Real2Int_shiftamt        = 16;

#if BigEndian
        #define iman_                           1
#else
        #define iman_                           0
#endif //BigEndian_


#if 0 && defined(__GNUC__) && defined(__i386__)
// While this works for i386, the next one seems to be slightly faster
// (tested on 'morocco', 1.5Ghz Pentium 4, gcc-2.96, linux 2.4.3-12
static inline
int Real2Int(float /*FLOAT*/ f)
{
  // GCC 386
  int r;
  __asm__ ("fistpl %0" : "=m" (r) : "t" (f) : "st");
  return r;
}

#else

// Not GCC
inline int Real2Int(double val)
{
#if (defined(_WIN32) || defined(__i386__))
  // Any 386
  val= val + Real2Int_double2fixmagic;
  return ((int*)&val)[iman_] >> Real2Int_shiftamt;
#else
  // Any real machine
  return (int)val;
#endif
}
#endif

#if 0
// Don't use this, cos _asm disables the optimizer
inline int Real2Int( double f ) {
  int temp;

  _asm fld f;
  _asm fistp temp;
  return temp;
}
#endif
