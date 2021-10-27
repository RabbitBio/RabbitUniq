#ifndef __UNIQUE_KEMR_SPLIT_H__
#define __UNIQUE_KEMR_SPLIT_H__

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

#include "robin_hood.h"
#include <sys/time.h>

template<typename Key, typename Value>
using unordered_map = robin_hood::unordered_map<Key, Value>;

inline double get_time(){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec + (double)tv.tv_usec / 1000000;
}

using namespace std;

extern const char kmerToBase[256][4];

struct kmer_node
{
    uint64_t kmer;
    uint64_t id;

    kmer_node(uint64_t kmer_, uint64_t id_) : kmer(kmer_), id(id_){}
    kmer_node() = default;
};

#define MAX_ITEM_NUM 4096
struct kc_t{
    uint64_t content[MAX_ITEM_NUM];
    unsigned pos = 0;

    bool add_item(uint64_t item){
        content[pos++] = item;
        return pos < MAX_ITEM_NUM; // at most MAX_ITEM_NUM
    }
    void reset(){
        pos = 0;
    }
};



class Write_file2
{
    private:
        //deque<pair<char*, int>> dq;
        deque<tuple<uint64_t, uint64_t*, int>> dq;
        mutex mut;
        condition_variable data_cond;
        int finished;
        fstream f;
        atomic_int has_finished;
        unordered_map<uint64_t, std::string> fid2fname;
        unordered_map<uint64_t, FILE*> opened_files;
        deque<uint64_t> q_opened;
    public:
        void write_to_file(std::tuple<uint64_t, uint64_t*, int>& p);
        Write_file2(string file_name, int finished_, vector<string> &id2fname);
        ~Write_file2();
        //void push(char* buf, int buf_pos);
        void push(uint64_t fid, uint64_t* buf, int buf_pos);
        void set_finished();
        void operator()();
};

void Flush2(uint64_t fid, kc_t &kc, Write_file2 &w_file, const vector<string> fid2fname);

void find_unique_2(unordered_map<uint64_t, uint64_t> &kmerslist, Write_file2 &w_file,
                   const vector<string> &ids, vector<kc_t> &kmer_collections);

void get_unique_kmer_2(const string &file_name, int kmer_len, 
                       const vector<string> &ids, Write_file2 &w_file,
                       vector<kc_t>& kmer_collections);

#endif
