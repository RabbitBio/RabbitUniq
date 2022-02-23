#!/usr/bin/env python3
# format output file to fasta format

def split(filename):
  filename = ""
  fout = None
  with open(filename, 'r') as f:
    for line in f:
      if line.startwith(">"):
        if fout:
            fout.close()
        fout = open(os.path.join("./splited/", line[1:]), 'w')
      else:
        fout.write(line)


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

if __name__ == "__main__":
  merge("./outfile.txt", "sars_vs_cov19.fa")
