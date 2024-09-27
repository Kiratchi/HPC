# The hex representation of positive integers  0 <= n < 16
[UInt8(n) for n in 0:15]

# The hex representation of positive integers  0 <= n < 256 = 2^8
reshape([UInt8(n) for n in 0:255], 16, 16)

# The hex representation of integers 2^n for 0 <= n < 16 nad 0 <= n < 64
reshape(vcat([reinterpret(UInt8, Int64[1<<n]) for n in 0:15]...), 8, 16)
reshape(vcat([reinterpret(UInt8, Int64[1<<n]) for n in 0:63]...), 8, 64)
