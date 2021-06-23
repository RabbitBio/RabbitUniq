#!/usr/bin/env python3

import subprocess
import os
import glob
import sys

KMERLEN=25


def merge(infile, outfile):
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
      f.write(">" + k + "\n")
      for line in v:
        f.write(line)



def main_step():
  if not os.path.exists(work_space):
    os.mkdir(work_space)
  #---------step1-----------#
  try:
    kmc_out = subprocess.check_output(["./bin/kmc",
                                       '-k'+str(KMERLEN),
                                       '-fm', '@'+infile_list,
                                       "tmp", work_space])
    id2filenames = set(kmc_out.decode().split('\n'))
    for s in id2filenames:
      print(s)
  except e:
    print("run kmc error!")
  
  with open(os.path.join(work_space, 'binList.list'), 'w') as f:
    for filename in glob.glob(os.path.join(work_space, "*.bin")):
      f.write(filename + '\n')
  
  #---------step2-----------#
  try:
    gu_out = subprocess.check_output(['./bin/generate_uniq',
                                      os.path.join(work_space, "binList.list"),
                                      str(KMERLEN)])
  except subprocess.CalledProcessError as e:
    print("run gene uniq error!")
    print(e.output)
    exit(-1)
  #print("done, out file is in outfile.txt")
  merge('./outfile.txt', out_file)
  os.remove('./outfile.txt')

if __name__ == "__main__":
  if len(sys.argv) < 4:
    print("./run_this.py [tmp workspace] [fasta file list] [result file]")
    exit(-1)
  work_space = sys.argv[1] #"./workspace"
  infile_list = sys.argv[2]
  out_file = sys.argv[3]
  main_step()


