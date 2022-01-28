#!/usr/bin/env python3

klen = 25 

m = {'A':'T', 'T':'A', 'G':'C', 'C':'G'}
def get_rev(kmer):
  assert len(kmer) == klen
  return ''.join([m[kmer[klen - i - 1]] for i in range(klen)])

def get_seq(fn):
  contex = ''
  with open(fn, 'r') as f:
    for line in f:
      if line[0] == '>' :
        continue
      contex += line.strip()
  return contex

def process_file(path) -> set:
  seq = get_seq(path)
  kmers = set()
  #kmers = {seq[i:i+klen] for i in range(len(seq) - klen)}
  #kmers.add({ get_rev(seq[i:i+klen]) for i in range(len(seq) - klen)}) #reverse completement
  for i in range(len(seq) - klen):
    kmers.add(seq[i:i+klen])
    kmers.add(get_rev(seq[i:i+klen]))
  #if 'AACTCATAGAATAATATAATTTTTT' in kmers:
  #  print("okkkkkkkkkkkkkkkkkkkkk", '-->' , path)
  return kmers

def pure_refernce(s1, s2):
  tmp_inter = s1.intersection(s2)
  s1 = s1 - tmp_inter
  s2 = s2 - tmp_inter
  return s1, s2
  #tobe_removed = set()
  #for item in s1:
  #  if item in s2:
  #    tobe_removed.add(item)
  #s1.remove(tobe_removed)
  #s2.remove(tobe_removed)

def print_info(d):
  for k, v in d.items():
    print(k, len(v))
  
def validate(flist):
  kmersets = dict() # key: file, value: kmerset 
  with open(flist, 'r') as f:
    for line in f:
      filename = line.strip().split("/")[-1]
      kmersets[filename] = process_file(line.strip())
  it = list(kmersets.keys())
  kmersets_new = dict()
  for i in range(len(it)):
    target_ref = kmersets[it[i]].copy()
    for j in range(len(it)):
      if i == j: continue
      target_ref = target_ref.difference(kmersets[it[j]])
      #print('puring: {} ==== {}'.format(it[i], it[j]))
      #kmersets[it[i]], kmersets[it[j]] = pure_refernce(kmersets[it[i]], kmersets[it[j]])
      #print_info(kmersets)
    kmersets_new[it[i]] = target_ref

  kmersets = kmersets_new 
  for k, v in kmersets.items():
    with open(k+'.kmers', 'w') as f:
      for item in v:
        f.write(item+'\n')
  return kmersets

def get_rabituniq_result(fname):
  rabu_result=dict()
  with open(fname) as f:
    current_ref = ''
    for line in f:
      if line[0] == ">":
        current_ref = line.strip().split("/")[-1]
        rabu_result[current_ref]=set()
      else:
        rabu_result[current_ref].add(line.strip())
  return rabu_result
        
if __name__ == "__main__":
  flist = './uniq_virual.txt'
  rabbituniq_result_file = './tmp.fasta2'
  manual_result = validate(flist)
  rabuniq_result = get_rabituniq_result(rabbituniq_result_file)
  #print(list(manual_result.keys()), list(rabuniq_result.keys()))
  print('file\t manual_result \t rabbituniq_result \t difference')
  for k in manual_result.keys():
    print(k, len(manual_result[k]), len(rabuniq_result[k]), len(manual_result[k]) - len(rabuniq_result[k]))
    # if 'ATTATATTTTCGTATATCATTATAA' in manual_result:
    #   print('okkkkkkkkkkkkkkkkkkkkk')
    # if 'ATTATATTTTCGTATATCATTATAA' in rabuniq_result:
    #   print('okkkkkkkkkkkkkkkkkkkkk')
    # exit()
    #for ii in rabuniq_result[k] - manual_result[k]:
    #for ii in manual_result[k] - rabuniq_result[k]:
    #  print(ii)
    #exit()
