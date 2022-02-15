#!/usr/bin/env python3


#read = "AATTCGATCGCCCGCCGC"
read = "AATTCGATCGCGCGCCGC"
klen = 8
mlen = 4

m = {'A':'T', 'T':'A', 'G':'C', 'C':'G'}
def get_rev(kmer):
  return ''.join([m[kmer[klen - i - 1]] for i in range(klen)])

for i in range(len(read) - klen + 1):
  kmer = read[i:i+klen]
  #print(kmer)
  kmer_rev = get_rev(kmer)
  minimizer = ''.join('Z'*mlen)
  for j in range(klen - mlen + 1):
    c_mizer = kmer[j:j+mlen]
    c_mizer_v = kmer_rev[j:j+mlen]
    minimizer = min(minimizer, c_mizer, c_mizer_v)
  #print(minimizer)
  ss = ''.join(' '*i)+kmer
  print(ss, ''.join('\t'*(3-(len(ss)+1)//8)), kmer_rev, '\t', minimizer)
