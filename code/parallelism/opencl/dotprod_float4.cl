__kernel
void
dot_prod_mul(
    __global const float4 *a,
    __global const float4 *b,
    __global float *c
    )
{
  int ix = get_global_id(0);
  float4 prod = a[ix] * b[ix];
  c[ix] = prod.x + prod.y + prod.z + prod.w;
}
