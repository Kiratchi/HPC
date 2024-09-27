#include <x86intrin.h>
#include <stdio.h>

int
main()
{
  long int a[2] = {17, 28};
  long int b[2] = {4, 19};
  long int c[2];

  __m128i ar = _mm_loadu_si128((__m128i *)(a));
  __m128i br = _mm_loadu_si128((__m128i *)(b));

  __m128i cr = _mm_add_epi64(ar, br);

  _mm_storeu_si128((__m128i *)(c), cr);

  printf("%li, %li\n", c[0], c[1]);

  return 0;
}
