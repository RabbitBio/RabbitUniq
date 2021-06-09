from tqdm import tqdm

s1 = 'outfile.txt'
result = []
index = 0
with open(s1, 'r') as f:
    for s in tqdm(f.readlines()):
        if index % 2 == 1:
            s = s[ : -1]
            result.append(s)
        index += 1

s = 'test_origin_kmer.data'
kmers = []
with open(s, 'r') as f:
    for s in tqdm(f.readlines()):
        s = s[2 : -1]
        if s not in kmers:
            kmers.append(s)


for i in tqdm(result):
    if i not in kmers:
        print(i)

print('!!!!!!!!!!!!!!!!!!!!');

for i in tqdm(kmers):
    if i not in result:
        print(i)




