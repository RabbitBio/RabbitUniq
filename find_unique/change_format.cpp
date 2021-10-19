#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <cstdint>

using namespace std;

int write_one(int argc, char **argv)
{
    string argv_1(argv[1]);
    string argv_2(argv[2]);

    map<string, vector<string>> r;

    fstream in(argv_1, ios::in);
    if(!in)
    {
        cerr << "open " << argv_1 << " fail" << endl;
        exit(0);
    }

    uint64_t count = 0;

    while(!in.eof())
    {
        char buf[256];
        in.getline(buf, 256);
        char buf_[256];
        in.getline(buf_, 256);
        string id(buf);
        string kmer(buf_);
        //if(count <= 10)
        //    cout << id << "," << kmer << endl;
        if(id.size() != 0 && kmer.size() != 0)
        {
            r[id].push_back(kmer);
            //if(id == ">2768")
            //   cout << kmer << endl;
        }
        count++;
        if(count % 10000000 == 0)
            cout << ".";
    }

    cout << endl;

    in.close();

    cout << "read over" << endl;

    fstream out(argv_2, ios::out);
    if(!out)
    {
        cerr << "open " << argv_2 << " fail" << endl;
        exit(0);
    }

    for(auto b = r.begin(); b != r.end(); b++)
    {
        vector<string> &b_second = b->second;
        out << b->first << '\n';
        for(int i = 0; i < b_second.size(); i++)
            out << b_second[i] << '\n';
    }

    out.close();

    cout << "write over" << endl;

    return 0;
}

int write_split(int argc, char **argv){
    string infile(argv[1]);
    string outd(argv[2]);
    fstream in(infile, ios::in);   
    if(!in){cerr << "open " << infile << "fail" << endl; exit(0);}
    map<string, fstream> writers;
    uint64_t count = 0;
    while(!in.eof())
    {
        char buf[256];
        in.getline(buf, 256);
        char buf_[256];
        in.getline(buf_, 256);
        string id(buf);
        string kmer(buf_);
        if(id.size() != 0 && kmer.size() != 0)
        {
            //r[id].push_back(kmer);
            if(writers.count(id)){
                writers[id] << kmer << "\n";
            }
            else{
                string file_n = outd + "/"+id+".fa";
                writers[id] = fstream(file_n, ios::out);
                if(!writers[id]) {
                    cerr << "open " << file_n << "fail" << endl; 
                    exit(0);
                }
                writers[id] << kmer << "\n";
            }
        }
        count++;
        if(count % 10000000 == 0)
            cout << ".";
    }

    for(auto& w : writers){
        cout << "closing " << w.first << endl;
        w.second.close();
    }
    cout << "write over" << endl;
}

int main(int argc, char **argv){
    write_one(argc, argv);
    write_split(argc, argv);
    return 0;
}
