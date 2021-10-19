#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <string>
#include <atomic>
#include "unique_kmer.h"

using namespace std;

atomic_int file_index(-1);
//atomic_int file_index(900);

void thread_fun(vector<string> *file_names, vector<string> *v_kinds, Write_file *w_file, int kmer_len)
{
	  const int n_files = file_names->size();
    while(true)
    {
        int i = ++file_index;
        string s = (*file_names)[i];
        //cout << i << endl;
				cout << "starting" << i << " - " << s << endl;
        if(i >= n_files)
            break;
        //string s = (*file_names)[i];
        get_unique_kmer(s, kmer_len, *v_kinds, *w_file);
				cout << i << " - " << s << " done" << endl;
    }
}

int main(int argc, char **argv)
{
    vector<string> v(100000);
    for(int i = 0; i < 100000; i++)
        v[i] = to_string(i);
    //void get_unique_kmer(const string *file_name, int kmer_len, vector<string> &ids, Write_file &w_file)

    //string file_name("test.data");
    //string file_name = "/home/old_home/haoz/workspace/KMC/tests/tmp/kmc_00511.bin";
    string file_name(argv[1]);
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

    Write_file w_file("outfile.txt", file_vectors.size());
    thread t(std::ref(w_file));

    vector<thread> threads;

    int work_th = 40;
    for(int i = 0; i < work_th; i++)
        threads.emplace_back(thread_fun, &file_vectors, &v, &w_file, stoi(argv[2]));

    for(int i = 0; i < work_th; i++)
        threads[i].join();

    t.join();
}

