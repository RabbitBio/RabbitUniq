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
#include "./robin_hood.h"

using namespace std;

template<typename Key, typename Value>
using unordered_map = robin_hood::unordered_map<Key, Value>;

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

char m[4] = {'A', 'C', 'T', 'G'};
string kmer_bit2char(uint64_t kmer, int kmer_len){
  char tmp[kmer_len + 2];
  tmp[kmer_len] = '\n';
  tmp[kmer_len+1] = '\0';
  for(int i = 0; i < kmer_len; ++i){
    tmp[kmer_len - i - 1] = m[kmer & 0x03];
    kmer >>= 2;
  }
  return string(tmp);
}

int write_binary(string& input_file, string& result_file, 
                map<uint64_t, string>& fid2fname, uint64_t exclude_id)
{
  unordered_map<uint64_t, vector<uint64_t>> r;

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
    //kmer = redecode_the_kmer(kmer);
    if(id != exclude_id){
      r[id].push_back(kmer);
    }
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

  //for(auto b = r.begin(); b != r.end(); b++)
  for(auto &b: r)
  {
      vector<uint64_t> &kmers = b.second;
      out << fid2fname[b.first] << '\n';
      //for(int i = 0; i < kmers.size(); i++)
      //out << kmers[i];
      uint64_t ksize = kmers.size();
      out.write(reinterpret_cast<char*>(&ksize), 8);
      out.write(reinterpret_cast<char*>(kmers.data()), kmers.size() * sizeof(uint64_t) / sizeof(char)); 
      cout << fid2fname[b.first] << " out put " << ksize << " kmers!" << endl;
      //out << "\n";
  }

  out.close();
  cout << "write over" << endl;
  return 0;
}

int write_character(string& input_file, string& result_file, 
                    map<uint64_t, string>& fid2fname, const int kmer_len)
{
  map<uint64_t, vector<string>> r;

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
    string kc = kmer_bit2char(kmer, kmer_len);
    if( kc.size() != kmer_len + 1){
      cerr << "err length: " << kc << " - " << kmer_len << endl;
    }
    r[id].push_back(kc);
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
      vector<string> &kmers = b->second;
      out << '>' << fid2fname[b->first] << '\n';
      //for(int i = 0; i < kmers.size(); i++)
      //out << kmers[i];
      uint64_t ksize = kmers.size();
      //out.write(reinterpret_cast<char*>(&ksize), 8);
      //out.write(reinterpret_cast<char*>(kmers.data()), kmers.size() * sizeof(uint64_t) / sizeof(char)); 
      for(string& s: kmers){
        out.write(s.data(), s.size());
      }
      cout << fid2fname[b->first] << " out put " << ksize << " kmers!" << endl;
      //out << "\n";
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
    cerr << "change_format [infile] [outfile] [flist.list] [(optional)exclude_file]" << endl;
    exit(-1);
  }
  string input_file(argv[1]);
  string result_file(argv[2]);
  string file_list_path(argv[3]);
	const int kmer_len = stoi(argv[4]);
	bool out_character = kmer_len == 0 ? true : false;

  map<uint64_t, std::string> id2fname;
  ifstream flist(file_list_path, ios::in);
  char ref_name[256];
  uint64_t fid = 0;
  uint64_t exclude_id = -1;
  while(!flist.eof()){
    flist.getline(ref_name, 256);
    //cout << "reading " << string(ref_name) << endl;
    //if(exclude_file != "" && string(ref_name) == exclude_file){
    //  exclude_id = fid;
    //  std::cout << "excluding file: " << exclude_file << " id: " << exclude_id << endl;
    //}
    id2fname[fid] = string(ref_name);
    fid++;
  }

	if(out_character){
		write_character(input_file, result_file, id2fname, kmer_len);
	}else{
		write_binary(input_file, result_file, id2fname, exclude_id);
	}
  //write_split(argc, argv);
  return 0;
}
