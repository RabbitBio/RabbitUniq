#!/usr/bin/env python3
import os

def get_remap(filename):
  count = 0
  remap = dict()
  with open(filename, 'r') as f:
    count = 0
    last_id = -1
    for line in f:
      if line[0] == '>':
        remap[last_id] = count
        last_id = line[1:].strip()
        remap[last_id] = 0
        count = 0
      else:
        count += 1
    remap[last_id] = count
  return remap

def id_fname_map(filename):
  id = 0
  id_fname = dict()
  with open(filename, 'r') as f:
    for line in f:
      id_fname[id] = line.strip()
      id += 1
  return id_fname

def sort_by_value(x):
  assert isinstance(x, dict)
  return dict(sorted(x.items(), key=lambda item: item[1]))

def get_seq_length(filename):
  seq_length = 0
  with open(filename, 'r') as f:
    for line in f:
      if line[0] != '>':
        seq_length += len(line.strip())
  return seq_length

uk_file = './virual.fa'
flist_file = 'uniq_virual.txt'
id_uk_count = get_remap(uk_file)

id_uk_count = sort_by_value(id_uk_count)
for k, v in id_uk_count.items():
  print(k, v)

#id_fname = id_fname_map(flist_file)
id_seqlength = dict()
for id in id_uk_count.keys():
  if id != -1:
    fname = os.path.join('/home/ssd/viral', id)
    id_seqlength[id] = get_seq_length(fname)
  
for k, v in id_uk_count.items():
  if k == -1:
    continue
  print("{},{},{},{}".format(k, v, id_seqlength[k], v/id_seqlength[k]))
  if v/id_seqlength[k] >= 2.0:
    print('kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk')
  #<filename>,<unique kmer number(uk)>,<sequence length(sl)>,<uk / sl> 
