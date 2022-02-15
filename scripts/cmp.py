#!/usr/bin/env python3

#################################################
# This module compare two different reference.  #
# Just for experiment                           #
#################################################

f1 = "../bacteria/GCF_901002655.1_4995_2_6_genomic.fna"
f2 = "../bacteria/GCF_901002665.1_4880_7_9_genomic.fna"
contex1 = ''
contex2 = ''
with open(f1, 'r') as f:
  for line in f:
    if line[0] == '>' :
      continue
      if line.find(">NZ_CAAJNS010000001.1") == 0:
        continue
      else:
        break
    contex1 += line.strip()

with open(f2, 'r') as f:
  for line in f:
    if line[0] == '>':
      continue
      if line.find(">NZ_CAAJNU010000001.1") == 0:
        continue
      else:
        break
    contex2 += line.strip()

contex1 = contex1.replace('N', '')    
contex2 = contex2.replace('N', '')

print(f1, len(contex1),"\n", f2, len(contex2))
exit()

same = 0
diff = 0
for i in range(min(len(contex1), len(contex2))):
  if contex1[i] == contex2[i]:
    same += 1
  else: 
    diff += 1

print(same, diff)
