## Compile
``` bash
git clone https://github.com/RabbitBio/RabbitUniq
cd RabbitUniq
make
```

## Parameters
``` bash
usage: RabbitUniq.py [-h] [--workspace WORKSPACE] --infile_list INFILE_LIST --outfile OUTFILE [--gu_worker GU_WORKER] [--kmer_len KMER_LEN] [--bin_num BIN_NUM]
                     [--exclude_last] [--output_char]

RabbitUniq

optional arguments:
  -h, --help            show this help message and exit
  --workspace WORKSPACE, -w WORKSPACE
                        workspace directory the bin files stored [default: workspace]
  --infile_list INFILE_LIST, -l INFILE_LIST
                        input file list, one line per file
  --outfile OUTFILE, -o OUTFILE
                        out put file
  --gu_worker GU_WORKER, -n GU_WORKER
                        The number of worker thread when generate unique kmer [default: 20]
  --kmer_len KMER_LEN, -k KMER_LEN
                        Unique k-mer length [default: 25]
  --bin_num BIN_NUM, -b BIN_NUM
                        Number of bin files to be store, from 64 to 2000[default: 512]
  --exclude_last, -e    Exclude the last element in infile_list when output
  --output_char, -c     Output the unique k-mer collection in character-based file instead of binary file (slower, so not recommended)
```

## Run RabbitUniq
### Generate unique kmer
``` bash
cat $REF_FILE_PATH >> ${infile_list}
time${RabbitUniq_PATH }/RabbitUniq.py \
  --infile_list  ${infile_list} \
  --outfile  ${outname}.fa \
  -n 20 -k 25 -b 2000 \
```

### exclude the reference file
Add the reference file to the end of input list file list, 
and run RabbitUniq with parameter `--exclude_last`.
``` bash
cat $REF_FILE_PATH >> ${infile_list}
time${RabbitUniq_PATH }/RabbitUniq.py \
  --infile_list  ${infile_list} -19. list \
  --outfile  ${outname}.fa \
  -n 20 -k 25 -b 512 \
  --exclude_last\
```
