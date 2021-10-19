#!/usr/bin/env python3

import subprocess
import os
import glob
import sys
import time

KMERLEN=25


def merge(infile, outfile, id_name_map):
  merge_dict = {}
  with open(infile, 'r') as f:
    content = ""
    name = ""
    for line in f:
      if line[0] == ">":
        name = line[1:].strip()
        if name not in merge_dict:
          merge_dict[name] = []
      else:
        content = line
        merge_dict[name].append(content)

  with open(outfile, 'w') as f:
    for k, v in merge_dict.items():
      ref_name = id_name_map[k]
      f.write(">" + ref_name + "\n")
      for line in v:
        f.write(line)



def main_step():
  if not os.path.exists(work_space):
    os.mkdir(work_space)
  file_path = os.path.dirname(os.path.realpath(__file__))
  bin_dir = os.path.join(file_path, 'bin')
  #---------step1-----------#
  s1_start = time.time()
  try:
    kmc_out = subprocess.check_output([os.path.join(bin_dir, 'kmc'),
                                       '-k'+str(KMERLEN),
                                       '-n'+str(2000),
                                       '-fm', '@'+infile_list,
                                       "tmp", work_space])
    id2filenames = set(kmc_out.decode().split('\n'))
    print("id2filenames: ", id2filenames)
    for s in id2filenames:
      print(s)
  except e:
    print("run kmc error!")
  
  with open(os.path.join(work_space, 'binList.list'), 'w') as f:
    for filename in glob.glob(os.path.join(work_space, "*.bin")):
      f.write(filename + '\n')
  s1_end = time.time()
  print("step 1: kmc step time: ", s1_end - s1_start)
  
  #---------step2-----------#
  try:
    gu_out = subprocess.check_output([os.path.join(bin_dir, 'generate_uniq'),
                                      os.path.join(work_space, "binList.list"),
                                      str(KMERLEN)])
  except subprocess.CalledProcessError as e:
    print("run gene uniq error!")
    print(e.output)
    exit(-1)
  #print("done, out file is in outfile.txt")
  s2_end = time.time()
  print("step 2: generate uniqu step time: ", s2_end - s1_end)
  #---------step3 merge------#
  print('do not merge.... exit now')
  exit(0)
  id_name_map = dict()
  with open(infile_list, 'r') as f:
    index = 0
    for line in f:
      id_name_map[str(index)] = line.split('/')[-1].strip()
      index += 1
  #print(id_name_map)
  merge('./outfile.txt', out_file, id_name_map)
  os.remove('./outfile.txt')
  s3_end = time.time()
  print("step 3: merge step(python) time: ", s3_end - s2_end)

if __name__ == "__main__":
  if len(sys.argv) < 4:
    print("./run_this.py [tmp workspace] [fasta file list] [result file]")
    exit(-1)
  work_space = sys.argv[1] #"./workspace"
  infile_list = sys.argv[2]
  out_file = sys.argv[3]
  main_step()
