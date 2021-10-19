#/usr/bin/env python3

###################################################
# This module remove the redundancy reference     #
# according rabbitTCluster                        #
###################################################

with open('./viral.out', 'r') as f:
  r_bases, r_fname = 0, ''
  for line in f:
    if line.find("the cluster") == 0:
      if r_fname != '': print(r_fname)
      new_cluster = True
      r_bases, r_fname = 0, ''
    else:
      array = [x.strip() for x in line.split('\t')]
      if len(array) != 7:
        #print('invalid line: ', line)
        continue
      bases, fname = array[3:5]
      bases = int(bases[:-2])
      if bases > r_bases:
        #if r_bases != 0:
        #  print("remove ==> ", r_fname)
        r_bases = bases
        r_fname = fname
     
  print(r_fname)
