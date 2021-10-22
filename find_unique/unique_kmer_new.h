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

#include "robin_hood.h"

template<typename Key, typename Value>
using unordered_map = robin_hood::unordered_map<Key, Value>;

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

class Write_file
{
    private:
        //deque<pair<char*, int>> dq;
        deque<pair<uint64_t*, int>> dq;
        mutex mut;
        condition_variable data_cond;
        int finished;
        fstream f;
        atomic_int has_finished;
    public:
        Write_file(const string& file_name, int finished_);
        ~Write_file();
        //void push(char* buf, int buf_pos);
        void push(uint64_t* buf, int buf_pos);
        void set_finished();
        void operator()();
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

void Flush(kmer_node* buf, int maxbufsize, int kmer_len, int &buf_pos, Write_file &w_file, const vector<string> &ids);

//void find_unique(vector<kmer_node> &v, int kmer_len, int bitstart, int bitlen, int maxbit_len, kmer_node* buf, int &buf_pos, int maxbufsize, Write_file &w_file, const vector<string> &ids);
void find_unique(unordered_map<uint64_t, uint64_t> &kmerslist, int kmer_len, kmer_node* buf, int &buf_pos, int maxbufsize, Write_file &w_file, const vector<string> &ids);

void get_unique_kmer(const string& file_name, int kmer_len, const vector<string> &ids, Write_file &w_file);
void get_unique_kmer_2(const string &file_name, int kmer_len, 
                       const vector<string> &ids, Write_file2 &w_file,
                       vector<kc_t>& kmer_collections);

#endif
