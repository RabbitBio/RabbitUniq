#!/usr/bin/env python3

import subprocess
import os
import glob
import sys
import time
import argparse



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



def main_step(args):
  work_space       = args.workspace
  infile_list      = args.infile_list
  out_file         = args.outfile
  gu_thread_number = args.gu_worker
  KMERLEN          = args.kmer_len
  bin_num          = args.bin_num
  if not os.path.exists(work_space):
    os.mkdir(work_space)
  file_path = os.path.dirname(os.path.realpath(__file__))
  bin_dir = os.path.join(file_path, 'bin')
  #---------step1-----------#
  s1_start = time.time()
  try:
    kmc_out = subprocess.check_output([os.path.join(bin_dir, 'kmc'),
                                       '-k'+str(KMERLEN),
                                       '-n'+str(bin_num),
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
    print("running: ", " ".join([os.path.join(bin_dir, 'generate_uniq'),
     os.path.join(work_space, "binList.list"),
     str(KMERLEN), "outfile.txt", str(gu_thread_number),
     infile_list,
     str(1 if args.exclude_last else 0)
     ]))
    gu_out = subprocess.check_output([os.path.join(bin_dir, 'generate_uniq'),
                                      os.path.join(work_space, "binList.list"),
                                      str(KMERLEN), "outfile.txt", str(gu_thread_number),
                                      infile_list,
                                      str(1 if args.exclude_last else 0)])
  except subprocess.CalledProcessError as e:
    print("run gene uniq error!")
    print(e.output)
    exit(-1)
  #print("done, out file is in outfile.txt")
  s2_end = time.time()
  print("step 2: generate uniqu step time: ", s2_end - s1_end)
  try:
    if args.output_char: #if output character, need kmer length
      cf_out = subprocess.check_output([os.path.join(bin_dir, 'change_format'),
                                        "./outfile.txt", out_file, infile_list, str(KMERLEN)])
    else: #if not output character, output .bin files, kmer_len specify to 0
      cf_out = subprocess.check_output([os.path.join(bin_dir, 'change_format'),
                                        "./outfile.txt", out_file, infile_list, str(0)])
      
  except subprocess.CalledProcessError as e:
    print("run change format error")
    print(e.output)
    exit(-1)
  s3_end = time.time()
  print("step 3: merge step time: ", s3_end - s2_end)

if __name__ == "__main__":
  #if len(sys.argv) < 4:
  #  print("./run_this.py [tmp workspace] [fasta file list] [result file] [gen uniq thread number]")
  #  exit(-1)
  #work_space = sys.argv[1] #"./workspace"
  #infile_list = sys.argv[2]
  #out_file = sys.argv[3]
  #gu_thread_number = sys.argv[4]

  parser = argparse.ArgumentParser(description = "RabbitUniq")
  parser.add_argument('--workspace', '-w', help = "", type = str, required = False, default = "workspace")
  parser.add_argument('--infile_list', '-l', help = "", type = str, required = True)
  parser.add_argument('--outfile', '-o', help = "", type = str, required = True)
  parser.add_argument('--gu_worker', '-n', help = "", type = int, required = True)
  parser.add_argument('--kmer_len', '-k', help = "", type = int, required = False, default = 25)
  parser.add_argument('--bin_num', '-b', help = "", type = int, required = False, default = 2000)
  parser.add_argument('--exclude_last', '-e', help = "", action = "store_true")
  parser.add_argument('--output_char', '-c', help = "", action = "store_true")
  args = parser.parse_args()
  main_step(args)
