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
  uniq_ref_num     = args.uniq_ref_num
  if not os.path.exists(work_space):
    os.mkdir(work_space)
  file_path = os.path.dirname(os.path.realpath(__file__))
  bin_dir = os.path.join(file_path, 'bin')
  #---------step1-----------#
  # use part of kmc to generate intermediate results
  # but we modified the process of generating intermediate results, the super-kmer in the generated 
  # intermediate results carries information about which species it belong to.
  # species it belong to.1_start = time.time()
  try:
    print("step1: ", " ".join([os.path.join(bin_dir, 'kmc'),
              '-k'+str(KMERLEN),
              '-n'+str(bin_num),
              '-fm', '@'+infile_list,
              "tmp", work_space]))
    # execute the modified kmc
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
  
  # collect the names of the intermediate result files for use in the following steps.
  with open(os.path.join(work_space, 'binList.list'), 'w') as f:
    for filename in glob.glob(os.path.join(work_space, "*.bin")):
      f.write(filename + '\n')
  s1_end = time.time()
  print("step 1 time: ", s1_end - s1_start)
  
  #---------step2-----------#
  # generate unique kmer
  try:
    print("step2: ", " ".join([os.path.join(bin_dir, 'generate_uniq'),
     os.path.join(work_space, "binList.list"),
     str(KMERLEN), "outfile.txt", str(gu_thread_number),
     infile_list,
     str(1 if args.exclude_last else 0),
     str(uniq_ref_num)
     ]))
    gu_out = subprocess.check_output([os.path.join(bin_dir, 'generate_uniq'),
                                      os.path.join(work_space, "binList.list"),
                                      str(KMERLEN), "outfile.txt", str(gu_thread_number),
                                      infile_list,
                                      str(1 if args.exclude_last else 0),
                                      str(uniq_ref_num)])
  except subprocess.CalledProcessError as e:
    print("run gene uniq error!")
    print(e.output)
    exit(-1)
  #print("done, out file is in outfile.txt")
  s2_end = time.time()
  print("step 2 time: ", s2_end - s1_end)
  try:
    if args.output_char: #if output character, need kmer length
      print('step3: ', ' '.join([os.path.join(bin_dir, 'change_format'),
                                   "./outfile.txt", out_file, infile_list, str(KMERLEN)]))
      cf_out = subprocess.check_output([os.path.join(bin_dir, 'change_format'),
                                        "./outfile.txt", out_file, infile_list, str(KMERLEN)])
    else: #if not output character, output .bin files, kmer_len specify to 0
      print('step3: ', ' '.join([os.path.join(bin_dir, 'change_format'),
                                   "./outfile.txt", out_file, infile_list, str(0)]))
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
  parser.add_argument('--workspace', '-w', help = "workspace directory the bin files stored [default: workspace]", type = str, required = False, default = "workspace")
  parser.add_argument('--infile_list', '-l', help = "input file list, one line per file", type = str, required = True)
  parser.add_argument('--outfile', '-o', help = "out put file", type = str, required = True)
  parser.add_argument('--gu_worker', '-n', help = "The number of worker thread when generate unique kmer [default: 20]", type = int, required = False, default = 20)
  parser.add_argument('--kmer_len', '-k', help = "Unique k-mer length [default: 25]", type = int, required = False, default = 25)
  parser.add_argument('--bin_num', '-b', help = "Number of bin files to be store, from 64 to 2000[default: 512]", type = int, required = False, default = 512)
  parser.add_argument('--exclude_last', '-e', help = "Exclude the last element in infile_list when output", default=False, action = "store_true")
  parser.add_argument('--uniq_ref_num', '-u', help = "Threshold considered as unique kmer, default is 1", type = int, required = False, default = 1)
  parser.add_argument('--output_char', '-c', help = "Output the unique k-mer collection in character-based file instead of binary file (slower, so not recommended)", action = "store_true")
  args = parser.parse_args()
  main_step(args)
