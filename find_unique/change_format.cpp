#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <cstdint>

#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>

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

int write_binary(string& input_file, string& result_file, map<uint64_t, string>& fid2fname)
{
  map<uint64_t, vector<uint64_t>> r;

  /*
  fstream in(input_file, ios::in);
  if(!in)
  {
      cerr << "open " << input_file << " fail" << endl;
      exit(0);
  }

  uint64_t count = 0;

  while(!in.eof())
  {
    const int nodes_per_read = 4096; 
    uint64_t nodes[2 * nodes_per_read];
    //in.read((char*)node, 2 * sizeof(uint64_t));
    in.read((char*)nodes, nodes_per_read * 2 * sizeof(uint64_t));
    for (unsigned i = 0; i < nodes_per_read; ++i)
    {
      uint64_t id   = nodes[2*i + 0];
      uint64_t kmer = nodes[2*i + 1];
      r[id].push_back(kmer);
      count++;
      if (count % 100000 == 0)
        cout << ".";
    }
  }
  cout << endl;
  in.close();
  cout << "read over" << endl;
  */

  int fd = open(input_file.c_str(), O_RDONLY, 0);
  if(fd == -1)
  {
    cerr << "open file : " << input_file << "fail\n";
    exit(1);
  }
  uint64_t file_size = lseek(fd, 0, SEEK_END);
  const uint64_t node_size = file_size >> 3; // 8 = sizeof(uint64) / sizeof(char)
  uint64_t *buf = (uint64_t*)(mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0));
  for(uint64_t i = 0; i < node_size; i+=2){
    uint64_t id   = buf[i];
    uint64_t kmer = buf[i+1];
    r[id].push_back(kmer);
    //if (i % 1000000 == 0)
    //  cout << ".";
    if (i % 1000000 == 0){
      printf("\r complete [%.2f%%]", i * 100.0f / node_size);
    }
  }
  close(fd);
  munmap(buf, file_size);


  fstream out(result_file, ios::out|ios::binary);
  if(!out)
  {
      cerr << "open " << result_file << " fail" << endl;
      exit(0);
  }

  for(auto b = r.begin(); b != r.end(); b++)
  {
      vector<uint64_t> &kmers = b->second;
      out << fid2fname[b->first] << '\n';
      //for(int i = 0; i < kmers.size(); i++)
      //out << kmers[i];
      out.write(reinterpret_cast<char*>(kmers.data()), kmers.size() * sizeof(uint64_t) / sizeof(char)); 
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
  if (argc < 3){
    cerr << "change_format [infile] [outfile] [flist.list]" << endl;
    exit(-1);
  }
  string input_file(argv[1]);
  string result_file(argv[2]);
  string file_list_path(argv[3]);
  map<uint64_t, std::string> id2fname;
  ifstream flist(file_list_path, ios::in);
  char ref_name[256];
  uint64_t fid = 0;
  while(!flist.eof()){
    flist.getline(ref_name, 256);
    id2fname[fid] = string(ref_name);
    fid++;
  }

  write_binary(input_file, result_file, id2fname);

  //write_split(argc, argv);
  return 0;
}