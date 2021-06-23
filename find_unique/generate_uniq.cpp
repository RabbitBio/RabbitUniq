#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <string>
#include <atomic>
#include "unique_kmer.h"

using namespace std;

atomic_int file_index(-1);

void thread_fun(vector<string> *file_names, vector<string> *v_kinds, Write_file *w_file, int kmer_len)
{
    while(true)
    {
        int i = ++file_index;
        cout << i << endl;
        if(i >= 512)
            break;
        string s = (*file_names)[i];
        get_unique_kmer(&s, kmer_len, *v_kinds, *w_file);
    }
}

int main(int argc, char **argv)
{
    vector<string> v(10000);
    for(int i = 0; i < 10000; i++)
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

    int work_th = 20;
    for(int i = 0; i < work_th; i++)
        threads.emplace_back(thread_fun, &file_vectors, &v, &w_file, stoi(argv[2]));

    for(int i = 0; i < work_th; i++)
        threads[i].join();

    t.join();
      //void thread_fun(vector<string> *file_names, vector<string> *v_kinds, Write_file *w_file)
      //
    //!!!!!!!!!!!!!!!!!!!!!!!111

    //for(int i = 0; i < file_vectors.size(); i++)
    //{
    //    threads.emplace_back(get_unique_kmer, &file_vectors[i], 25, std::ref(v), std::ref(w_file));
    //}

    //for(int i = 0; i < file_vectors.size(); i++)
    //    threads[i].join();
    //t.join();


    //string file_name(argv[1]);
    //thread t1(get_unique_kmer, &file_name, 25, std::ref(v), std::ref(w_file));
    //thread t2(std::ref(w_file));
    //t1.join();
    //t2.join();
}

