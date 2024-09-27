sz = 10
addrs = fill(0,sz,sz,sz);
for ix in 1:sz, jx in 1:sz
  addrs[ix,jx,:] += collect(0:sz-1)
end
for ix in 1:sz, jx in 1:sz
  addrs[ix,:,jx] += sz .* collect(0:sz-1)
end
for ix in 1:sz, jx in 1:sz
  addrs[:,ix,jx] += sz*sz .* collect(0:sz-1)
end
# Julia uses column major order so we have to revert the order of indices.
addrs = permutedims(addrs,[3,2,1]);

# order ijk
addrs_flatt = reshape(addrs,sz^3);
sum(abs.(addrs_flatt[2:end] - addrs_flatt[1:end-1]))/(sz^3-1)

# order jik
addrs_flatt = reshape(permutedims(addrs,[2,1,3]),sz^3);
sum(abs.(addrs_flatt[2:end] - addrs_flatt[1:end-1]))/(sz^3-1)

# order ikj
addrs_flatt = reshape(permutedims(addrs,[1,3,2]),sz^3);
sum(abs.(addrs_flatt[2:end] - addrs_flatt[1:end-1]))/(sz^3-1)

# order jki
addrs_flatt = reshape(permutedims(addrs,[2,3,1]),sz^3);
sum(abs.(addrs_flatt[2:end] - addrs_flatt[1:end-1]))/(sz^3-1)

# order kij
addrs_flatt = reshape(permutedims(addrs,[3,1,2]),sz^3);
sum(abs.(addrs_flatt[2:end] - addrs_flatt[1:end-1]))/(sz^3-1)

# order kji
addrs_flatt = reshape(permutedims(addrs,[3,2,1]),sz^3);
sum(abs.(addrs_flatt[2:end] - addrs_flatt[1:end-1]))/(sz^3-1)
