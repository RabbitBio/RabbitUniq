#ifndef __UNIQUE_KEMR_H__
#define __UNIQUE_KEMR_H__

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdint>
#include <list>
#include <vector>
#include <algorithm>
#include <condition_variable>
#include <cstdlib>
#include <deque>
#include <atomic>
#include <thread>

//only for linux
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

extern const char kmerToBase[256][4];

struct kmer_node
{
    uint64_t kmer;
    uint64_t id;

    kmer_node(uint64_t kmer_, uint64_t id_) : kmer(kmer_), id(id_){}
    kmer_node() = default;
};

class Write_file
{
    private:
        deque<pair<char*, int>> dq;
        mutex mut;
        condition_variable data_cond;
        int finished;
        fstream f;
        atomic_int has_finished;
    public:
        Write_file(const string& file_name, int finished_);
        ~Write_file();
        void push(char* buf, int buf_pos);
        void set_finished();
        void operator()();
};

void Flush(kmer_node* buf, int maxbufsize, int kmer_len, int &buf_pos, Write_file &w_file, const vector<string> &ids);

void find_unique(vector<kmer_node> &v, int kmer_len, int bitstart, int bitlen, int maxbit_len, kmer_node* buf, int &buf_pos, int maxbufsize, Write_file &w_file, const vector<string> &ids);

void get_unique_kmer(const string& file_name, int kmer_len, const vector<string> &ids, Write_file &w_file);

#endif
