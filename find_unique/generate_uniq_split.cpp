#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <string>
#include <atomic>
#include "unique_kmer_new.h"

using namespace std;

atomic_int file_index(-1);
//atomic_int file_index(900);

void thread_fun(vector<string> *file_names, vector<string> *v_kinds, Write_file2 *w_file, int kmer_len, 
                uint64_t n_ref)
{
    const int n_bins = file_names->size(); //number bin files
    // init kmer_collection file
    vector<kc_t> kmer_collections(n_ref);
    while(true)
    {
        int i = ++file_index;
        string s = (*file_names)[i];
        //cout << i << endl;
        cout << "starting" << i << " - " << s << endl;
        if(i >= n_bins)
            break;
        //string s = (*file_names)[i];
        get_unique_kmer_2(s, kmer_len, *v_kinds, *w_file, kmer_collections);
        cout << i << " - " << s << " done" << endl;
    }
}


int main(int argc, char **argv)
{
    vector<string> v(100000);
    for(int i = 0; i < 100000; i++)
        v[i] = to_string(i);
    uint64_t n_ref = v.size(); // number of reference genome files

    string file_name(argv[1]); //binList.txt
    fstream f(file_name, ios::in);
    if(!f)
    {
        cerr << "open file " << file_name << " fail\n";
        exit(0);
    }
    vector<string> file_vectors;
    while(!f.eof())
    {
        char buf[256];
        f.getline(buf, 256);
        string s(buf);
        if(s.size() != 0)
            file_vectors.push_back(s);
    }

    cout << file_vectors.size() << endl;

    Write_file2 w_file("./tmp", file_vectors.size(), v);
    thread t(std::ref(w_file));

    vector<thread> threads;

    int work_th = 40;
    for(int i = 0; i < work_th; i++)
        threads.emplace_back(thread_fun, &file_vectors, &v, &w_file, stoi(argv[2]), n_ref);

    for(int i = 0; i < work_th; i++)
        threads[i].join();

    t.join();
}

