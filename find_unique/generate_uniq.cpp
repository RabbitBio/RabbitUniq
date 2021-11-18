#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <string>
#include <atomic>
//#include "unique_kmer.h"
#include "unique_kmer_new.h"

using namespace std;

atomic_int file_index(-1);
//atomic_int file_index(900);
std::mutex cout_mut2;

void thread_fun(vector<string> *file_names, vector<string> *v_kinds, Write_file *w_file, int kmer_len, uint64_t exclude_id)
{
    double start_time = get_time();
    const int n_files = file_names->size();
    while(true)
    {
        int i = ++file_index;
        if(i >= n_files)
            break;
        string s = (*file_names)[i];
        //cout << i << endl;
        cout << "starting" << i << " - " << s << endl;
        //string s = (*file_names)[i];
        get_unique_kmer(s, kmer_len, *v_kinds, *w_file, exclude_id);
        cout << i << " - " << s << " done" << endl;
    } 
    double end_time = get_time();
    double ti = end_time - start_time;
    {
    std::lock_guard<std::mutex> lk(cout_mut2);
    cout << "over**************************!!!!!!!!!! time " << ti << endl;
    }
}


int main(int argc, char **argv)
{
    const string file_name(argv[1]);
		const int kmer_len = stoi(argv[2]);
    const string outfile_txt(argv[3]);
    const int work_th = stoi(argv[4]);
		const string file_list(argv[5]);
		const int exclude_last = stoi(argv[6]);

    //vector<string> v(100000);
    //for(int i = 0; i < 100000; i++)
    //    v[i] = to_string(i);

		//------
		vector<string> v;
		uint64_t exclude_id = -1;
		uint64_t fid = 0;
		ifstream flist(file_list, ios::in);
		while(!flist.eof()){
			char ref_name[256];
			flist.getline(ref_name, 256);
			if(string(ref_name).size() != 0){
				v.push_back(to_string(fid));
				fid++;
			}
		}
		if(exclude_last)
			exclude_id = fid - 1;
		//------

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
        if(s.size() != 0){
					file_vectors.push_back(s);
				}
    }

		cout << "eclude file id: " << exclude_id << endl;
    cout << file_vectors.size() << endl;

    //Write_file w_file("outfile.txt", file_vectors.size());
    Write_file w_file(outfile_txt, file_vectors.size());
    thread t(std::ref(w_file));

    vector<thread> threads;

    for(int i = 0; i < work_th; i++)
			threads.emplace_back(thread_fun, &file_vectors, &v, &w_file, kmer_len, exclude_id);

    for(int i = 0; i < work_th; i++)
        threads[i].join();

    t.join();
}

