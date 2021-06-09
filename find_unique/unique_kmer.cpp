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
#include "unique_kmer.h"

using namespace std;

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
    //Write_file() = default;
    Write_file(const string& file_name, int finished_) : finished(finished_), has_finished(0)
  {
    f.open(file_name, ios::out);
    if(!f)
    {
      cerr << "open file " << file_name << " fail" << endl;
      exit(1);
    }
  }

    ~Write_file()
    {
      if(f.is_open())
        f.close();
    }

    void push(char* buf, int buf_pos)
    {
      lock_guard<mutex> lk(mut);
      dq.push_back({buf, buf_pos});
      data_cond.notify_one();
      //cout << "push\n";
      //buf = new char[buflen];
    }

    void set_finished()
    {
      has_finished++;
      data_cond.notify_one();
      //cout << "thread : " << has_finished << endl;
    }

    void operator()()
    {
      while(true)
      {
        unique_lock<mutex> lk(mut);
        data_cond.wait(lk, [this]{return has_finished == finished || !dq.empty();});
        if(has_finished == finished)
        {
          while(!dq.empty())
          {
            auto top = dq.front();
            dq.pop_front();
            f.write(top.first, top.second * sizeof(char));
          }
          //cout << "\nover!!!!!!!!!!!!!\n";
          break;
        }
        pair<char*, int> p = dq.front();
        dq.pop_front();
        lk.unlock();
        //cout << "write\n";
        f.write(p.first, p.second * sizeof(char));
        delete [] p.first;
      }
    }

};

void Flush(kmer_node* buf, int maxbufsize, int kmer_len, int &buf_pos, Write_file &w_file, vector<string> &ids)
{

  char *tmpbuf = new char[96 * maxbufsize];
  int tmpbufsize = 0;

  for(int x = 0; x < maxbufsize; x++)
  {
    tmpbuf[tmpbufsize++] = '>';
    //cout << "id = " << buf[x].id << endl;
    string &idstring = ids[buf[x].id];
    const char* idstringcp = idstring.c_str();
    std::copy(idstringcp, idstringcp + idstring.size(), tmpbuf + tmpbufsize);
    tmpbufsize += idstring.size();
    tmpbuf[tmpbufsize++] = '\n';

    int kmer_len_bytes = (kmer_len / 4);
    uint8_t *bases = (((uint8_t*)(&(buf[x].kmer))) + kmer_len_bytes);
    //TODO uint8_t
    int kmer_len_mode_8 = kmer_len % 4;

    switch(kmer_len_mode_8)
    {
      case 3:
        tmpbuf[tmpbufsize++] = kmerToBase[*bases][1];
      case 2:
        tmpbuf[tmpbufsize++] = kmerToBase[*bases][2];
      case 1:
        tmpbuf[tmpbufsize++] = kmerToBase[*bases][3];
    }

    bases--;
    //kmer_len_bytes--;

    for(int z = 0; z < kmer_len_bytes; z++)
    {
      std::copy(kmerToBase[*bases], kmerToBase[*bases] + 4, tmpbuf + tmpbufsize);    
      tmpbufsize += 4;
      bases--;
    }

    tmpbuf[tmpbufsize++] = '\n';
  }

  w_file.push(tmpbuf, tmpbufsize);
  buf_pos = 0;
}



void find_unique(vector<kmer_node> &v, int kmer_len, int bitstart, int bitlen, int maxbit_len, kmer_node* buf, int &buf_pos, int maxbufsize, Write_file &w_file, vector<string> &ids)
{

  vector<list<kmer_node>> buckets(1ul << bitlen);

  const uint64_t mask = (1ul << bitlen) - 1;

  int l = v.size();
  for(int i = 0; i < l; i++)
    buckets[(v[i].kmer >> bitstart) & mask].emplace_back(v[i].kmer, v[i].id);

  std::vector<kmer_node>().swap(v);


  l = buckets.size();
  for(int i = 0; i < l; i++)
  {
    vector<kmer_node> ltov(buckets[i].begin(), buckets[i].end());
    int vl = ltov.size();
    if(vl == 0)
      continue;
    if(vl == 1)
    {
      //cout << buf_pos << endl;
      buf[buf_pos++] = ltov[0];
      if(buf_pos == maxbufsize)
      {

        //void Flush(kmer_node* buf, int maxbufsize, int kmer_len, int &buf_pos, Write_file &w_file)
        Flush(buf, maxbufsize, kmer_len, buf_pos, w_file, ids);
        //FIXME tmpbuf may be out of bound
        //char *tmpbuf = new char[96 * maxbufsize];
        //int tmpbufsize = 0;
        //for(int x = 0; x < maxbufsize; x++)
        //{
        //    tmpbuf[tmpbufsize++] = '>';
        //    //cout << "id = " << buf[x].id << endl;
        //    string &idstring = ids[buf[x].id];
        //    const char* idstringcp = idstring.c_str();
        //    std::copy(idstringcp, idstringcp + idstring.size(), tmpbuf + tmpbufsize);
        //    tmpbufsize += idstring.size();
        //    tmpbuf[tmpbufsize++] = '\n';

        //    int kmer_len_bytes = (kmer_len / 4) + 1;
        //    uint8_t *bases = (((uint8_t*)(&(buf[x].kmer))) + kmer_len_bytes);
        //    //TODO uint8_t
        //    int kmer_len_mode_8 = kmer_len % 4;
        //    
        //    switch(kmer_len_mode_8)
        //    {
        //        case 3:
        //            tmpbuf[tmpbufsize++] = kmerToBase[*bases][1];
        //        case 2:
        //            tmpbuf[tmpbufsize++] = kmerToBase[*bases][2];
        //        case 1:
        //            tmpbuf[tmpbufsize++] = kmerToBase[*bases][3];
        //    }

        //    bases--;
        //    kmer_len_bytes--;

        //    for(int z = 0; z < kmer_len_bytes; z++)
        //    {
        //        std::copy(kmerToBase[*bases], kmerToBase[*bases] + 4, tmpbuf + tmpbufsize);    
        //        tmpbufsize += 4;
        //        bases--;
        //    }

        //    tmpbuf[tmpbufsize++] = '\n';
        //}

        //w_file.push(tmpbuf, tmpbufsize);
        //buf_pos = 0;
      }
      continue;
    }
    if(bitstart + bitlen < maxbit_len)
    {
      find_unique(ltov, kmer_len, bitstart + bitlen, bitlen, maxbit_len, buf, buf_pos, maxbufsize, w_file, ids);
      continue;
    }
    if(bitstart + bitlen == maxbit_len)
    {
      bool unique = true;
      uint64_t last_id = ltov[0].id;
      for(int j = 1; j < vl; j++)
      {
        if(last_id != ltov[j].id)
        {
          unique = false;
          break;
        }
      }
      if(unique)
      {
        buf[buf_pos++] = ltov[0];
        if(buf_pos == maxbufsize)
          Flush(buf, maxbufsize, kmer_len, buf_pos, w_file, ids);
      }
      //TODO add one unique kmer, Flush

    }
    //
    //void find_unique(vector<node> &v, int bitstart, int bitlen, int maxbit_len, node* buf, int buf_pos, int maxbufsize, Write_file &w_file, vector<string< &ids)
    //
    std::vector<kmer_node>().swap(ltov);
  }

  //return buf_pos;
}



void get_unique_kmer(const string *file_name, int kmer_len, vector<string> &ids, Write_file &w_file)
{
  ifstream f_stream(*file_name, ios::in | ios::binary);
  if(!f_stream)
  {
    cerr << "Open file " << *file_name << "fail\n";
    exit(1);
  }

  //TODO set buf size
  char buf[10240];
  int buf_left = 0;

  uint64_t count_push = 0;

  list<kmer_node> kmerslist;
  while(!f_stream.eof())
  {
    f_stream.read(buf + buf_left, 10240 - buf_left);
    //cout << f_stream.tellg() << endl;
    char* buf_ = buf;

    int read_size = f_stream.gcount() + buf_left;
    int read_pos = 0;


    while(read_pos < read_size - 9)
    {
      uint64_t file_id = *((uint64_t*)buf_);
      //cout << file_id << endl;
      buf_ += sizeof(uint64_t);
      //char super_kmer_len = *buf_;
      int super_kmer_len = ((uint8_t)(*buf_)) + kmer_len;
      cout << file_id << ":" << super_kmer_len << endl;
      buf_ += sizeof(char);
      read_pos += sizeof(uint64_t) + sizeof(char);

      if(read_pos + (super_kmer_len + 3) / 4 > read_size)
      {
        //cout << "------\n";
        read_pos -= sizeof(uint64_t) + sizeof(char);
        buf_ -= sizeof(uint64_t) + sizeof(char);
        break;
      }


      //TODO out of bound ?
      static const uint64_t mask = (1ull << (kmer_len * 2)) - 1;
      uint64_t kmer = 0;
      for(int zero = 0; zero < 7; zero++)
      {
        kmer += buf_[zero];
        kmer <<= 8;
      }
      kmer += buf_[7];
      kmer >>= (64 - kmer_len * 2 + 2);
      kmer &= mask;

      //uint64_t kmer = (*((uint64_t*)buf_)) >> (64 - kmer_len * 2 + 2);
      int index = 2 * kmer_len - 4; //start from 0
      for(int i = 0; i <= super_kmer_len - kmer_len; i++)
      {
        index += 2;
        //int index = (kmer_len - 1 + i) << 2;
        int index_1 = index / 8;
        int index_2 = index - (index_1 * 8);
        int base = ((buf_[index_1]) >> (6 - index_2)) & 0x03;
        kmer <<= 2;
        kmer += base;
        kmer &= mask;

        kmerslist.emplace_back(kmer, file_id);
        count_push++;
        //if(count_push == 33260)
        //{
        //    cout << count_push << endl;
        //    cout << f_stream.tellg() << endl;
        //}
      }

      read_pos += (super_kmer_len + 3) / 4;
      buf_ += (super_kmer_len + 3) / 4;
    }

    //TODO error when pos(buf_) < read_pos;
    //must be tested
    buf_left = read_size - read_pos;
    char tmp_buf_[buf_left];
    std::copy(buf_, buf_ + buf_left, tmp_buf_);
    std::copy(tmp_buf_, tmp_buf_ + buf_left, buf);
    //std::copy(buf_, buf + read_size, buf);
  }

  cout << "1\n";

  vector<kmer_node> kmersvector(kmerslist.begin(), kmerslist.end());
  //fstream tmp_f("tmp.dara", ios::out);
  //for(int i = 0; i < kmersvector.size(); i++)
  //{
  //    tmp_f << kmersvector[i].id << ",";
  //    vector<char> tmpbases(kmer_len);
  //    for(int j = 0; j < kmer_len; j++)
  //    {
  //        int tmpbase = kmersvector[i].kmer & 0x03;
  //        switch(tmpbase)
  //        {
  //            case 0:
  //                tmpbases[kmer_len - 1 - j] = 'A';
  //                break;
  //            case 1:
  //                tmpbases[kmer_len - 1 - j] = 'C';
  //                break;
  //            case 2:
  //                tmpbases[kmer_len - 1 - j] = 'G';
  //                break;
  //            case 3:
  //                tmpbases[kmer_len - 1 - j] = 'T';
  //                break;
  //        }
  //        kmersvector[i].kmer >>= 2;
  //    }

  //    for(int j = 0; j < kmer_len; j++)
  //        tmp_f << tmpbases[j];
  //    tmp_f << endl;
  //    //tmp_f << kmersvector[i].id << ", " << kmersvector[i].kmer << endl;
  //}
  //tmp_f.close();
  kmer_node nodebuf[10240];
  //void find_unique(vector<kmer_node> &v, int kmer_len, int bitstart, int bitlen, int maxbit_len, kmer_node* buf, int buf_pos, int maxbufsize, Write_file &w_file, vector<string> &ids)
  int buf_pos_ = 0;
  cout << "2\n";
  //cout << kmersvector.size() << endl;
  find_unique(kmersvector, kmer_len, 0, 8, (kmer_len * 2 + 7) / 8 * 8, nodebuf, buf_pos_, 10240, w_file, ids);
  //cout << buf_pos_;
  //cout << "buf_pos : " << buf_pos_ << endl;
  //cout << "buf_pos_ = " << buf_pos_ << endl;
  if(buf_pos_ != 0)
  {
    //void Flush(kmer_node* buf, int maxbufsize, int kmer_len, int &buf_pos, Write_file &w_file)
    Flush(nodebuf, buf_pos_, kmer_len, buf_pos_, w_file, ids);
  }
  w_file.set_finished();
  //cout << "over**************************!!!!!!!!!!\n";
}

//to find unique kmer


int main()
{
  vector<string> v(300);
  //void get_unique_kmer(const string *file_name, int kmer_len, vector<string> &ids, Write_file &w_file)

  Write_file w_file("outfile.txt", 1);
  //string file_name("test.data");
  string file_name = "/home/old_home/haoz/workspace/KMC/tests/tmp/kmc_00511.bin";
  thread t1(get_unique_kmer, &file_name, 25, std::ref(v), std::ref(w_file));
  thread t2(std::ref(w_file));
  t1.join();
  t2.join();
}


//struct node
//{
//    uint64_t kmer;
//    bool only_one;
//    uint64_t id;
//
//    node(uint64_t kmer_, bool only_one_, uint64_t id_) : kmer(kmer_), only_one(only_one_), id(id_){}
//
//    bool operator==(const node & n) const
//    {
//        return node.kmer == n.kmer;
//    }
//};
//
//void get_unique_kmer(const string *file_name, int kmer_len, uint64_t hash_array_size, uint64_t(hash_fun*)(uint64_t))
//{
//    ifstream f_stream(*file_name, ios::in | ios::binary);
//    if(!f_stream)
//    {
//        cerr << "Open file fail\n";
//        exit(1);
//    }
//
//    list<node>** hash_array = new list<node>*[hash_array_size];
//    memset((void*)hash_array, 0, sizeof(list<node>*) * hash_array_size); 
//
//    //TODO set buf size
//    char buf[1024];
//    int buf_left = 0;
//
//    while(!f_stream.eof())
//    {
//        f_stream.read(buf + buf_left, 1024 - buf_left);
//        char* buf_ = buf;
//
//        int read_size = f_stream.gcount();
//        int read_pos = 0;
//
//        while(read_pos < read_size - 5)
//        {
//
//            uint64_t file_id = *((uint64_t*)buf_);
//            buf_ += sizeof(uint64_t);
//            char super_kmer_len = *buf;
//            buf_ += sizeof(char);
//            read_pos += sizeof(uint64_t) + sizeof(char);
//
//            if(read_pos + super_kmer_len > read_size)
//            {
//                read_pos -= sizeof(uint64_t) + sizeof(char);
//                break;
//            }
//
//            
//            //TODO out of bound ?
//            static const uint64_t mask = (1ull << (kmer_len * 2)) - 1;
//            uint64_t kmer = *((uint64_t*)buf_) >> (64 - kmer_len * 2 + 2);
//            int index = 2 * kmer_len - 4;
//            for(int i = 0; i < super_kmer_len - kmer_len; i++)
//            {
//                index += 2;
//                //int index = (kmer_len - 1 + i) << 2;
//                int index_1 = index / 4;
//                int index_2 = index - (index_1 * 4);
//                int base = ((buf_[index_1]) >> (6 - 2 * index_2)) & 0x03;
//                kmer <<= 2;
//                kmer += base;
//                kmer &= mask;
//
//                uint64_t hash_index = hash_fun(kmer);
//                if(hash_array[hash_index])
//                {
//                    auto r = std::find(hash_array[hash_index]->begin(), hash_array[hash_index]->end(), node(kmer, true, file_id));                   
//                    if(r != hash_array[hash_index]->end())
//                        r->only_one = false;
//                    else
//                        hash_array[hash_index]->emplace_back(kmer, true, file_id);
//                }
//                else
//                {
//                    hash_array[hash_index] = new list<node>;
//                    list.emplace_back(kmer, true, file_id);
//                }
//            }
//
//            read_pos += (super_kmer_len + 3) / 4;
//            buf_ += (super_kmer_len + 3) / 4;
//
//            //TODO
//        }
//
//        std::copy(buf_, buf + read_size, buf);
//        buf_left = read_size - read_pos;
//    }
//
//
//
//    //delete new array
//    for(int i = 0; i < hash_array_size; i++)
//    {
//        if(hash_array[i])
//        {
//            //TODO output;
//        }
//    }
//    delete [] hash_array;
//}

