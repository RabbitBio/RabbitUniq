with open("test_origin.data", "r") as f:
    with open("test_origin_kmer.data", "w") as out_f:
        for s in f.readlines():
            s = s[: -1]
            id = int(s[0 ])
            super_kmer = s.split(':')[1]
            for i in range(len(super_kmer) - 25 + 1):
                kmer = str(id) + "," + super_kmer[i : i + 25] + "\n";
                out_f.write(kmer)



