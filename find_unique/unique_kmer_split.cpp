#include "unique_kmer_split.h"
#include <unordered_set>
#include <deque>
#include <mutex>


std::mutex cout_mut;
//---------------------------------------
void Flush2(uint64_t fid, kc_t &kc, Write_file2 &w_file, const vector<string> fid2fname)
{
  //lock_guard<mutex> lk(mut);
  //{
  //  lock_guard<mutex> lk(cout_mut);
  //  cout << "kc info [buf / pos]: " << (void *)kc.content << " | " << kc.pos << endl;
  //}
  uint64_t *buf = new uint64_t[kc.pos];
  memcpy(buf, kc.content, kc.pos);
  w_file.push(fid, buf, kc.pos);
  kc.reset();
}

void find_unique_2(unordered_map<uint64_t, uint64_t> &kmerslist, Write_file2 &w_file,
                   const vector<string> &ids, vector<kc_t> &kmer_collections)
{
  const uint64_t notuniq = -1;
  for (auto &k : kmerslist)
  {
    if (k.second != notuniq)
    {
      kc_t &target = kmer_collections[k.second];
      if (!target.add_item(k.first))
      {
        {
          lock_guard<mutex> lk(cout_mut);
          cout << "before flush: " << target.pos << endl;
        }
        Flush2(k.second, target, w_file, ids);
      }
    }
  }
  for (int i = 0; i < kmer_collections.size(); ++i)
  {
    if (kmer_collections[i].pos > 0)
    {
      Flush2(i, kmer_collections[i], w_file, ids);
    }
  }
}

//-------------------------wf2--------------------------------

//void Write_file2::write_to_file(uint64_t fid, kc_t& kmer_col, const vector<string> fid2fname){
void Write_file2::write_to_file(std::tuple<uint64_t, uint64_t *, int> &p)
{
  uint64_t fid = std::get<0>(p);
  uint64_t *buf = std::get<1>(p);
  unsigned buf_size = std::get<2>(p);

  auto opened_file_itr = opened_files.find(fid);
  if (opened_file_itr->second != NULL)
  {
    fwrite(buf, sizeof(uint64_t), buf_size, opened_file_itr->second);
  }
  else
  {
    if (q_opened.size() < 2000)
    {
      std::string &fname = fid2fname.at(fid);
      opened_files[fid] = fopen(fname.c_str(), "w");
    }
    else
    {
      uint64_t tmp = q_opened.front();
      q_opened.pop_front();
      if (opened_files.at(tmp) != NULL)
      {
        fclose(opened_files.at(tmp));
        opened_files[tmp] = NULL;
        std::string &fname = fid2fname.at(fid);
        opened_files[fid] = fopen(fname.c_str(), "w");
      }
    }
    q_opened.push_back(fid);
  }
}

Write_file2::Write_file2(string prefix_dir, int finished_, vector<string> &id2fname) : finished(finished_), has_finished(0)
{
  //--------tmp
  for (int i = 0; i < id2fname.size(); i++)
  {
    fid2fname[i] = prefix_dir + "/" + id2fname[i];
    opened_files[i] = NULL;
  }
  //----------
}

Write_file2::~Write_file2()
{
}

void Write_file2::push(uint64_t fid, uint64_t* buf, int buf_pos)
{
  lock_guard<mutex> lk(mut);
  //std::tuple{fid, buf, buf_pos};
  dq.push_back({fid, buf, buf_pos});
  //delete [] buf;
  data_cond.notify_one();
}

void Write_file2::set_finished()
{
  has_finished++;
  data_cond.notify_one();
  //cout << "thread : " << has_finished << endl;
}

void Write_file2::operator()()
{
  while (true)
  {
    unique_lock<mutex> lk(mut);
    data_cond.wait(lk, [this]
                   { return has_finished == finished || !dq.empty(); });
    if (has_finished == finished)
    {
      while (!dq.empty())
      {
        auto top = dq.front();
        dq.pop_front();
        //f.write(top.first, top.second * sizeof(char));
        //<fid, buf, buf_size>
        write_to_file(top);
        delete[] std::get<1>(top);
      }
      //cout << "\nover!!!!!!!!!!!!!\n";
      break;
    }
    //pair<char*, int> p = dq.front();
    tuple<uint64_t, uint64_t *, int> &p = dq.front();
    write_to_file(p);
    dq.pop_front();
    lk.unlock();
    delete[] std::get<1>(p);
  }
}

inline void kmer_add(unordered_map<uint64_t, uint64_t> &kmerslist, const uint64_t kmer, const uint64_t fid){
	auto re = kmerslist.find(kmer);
	if (re == kmerslist.end()){
		kmerslist.insert({kmer, fid});
		//kmerslist[kmer] = fid;
	}else{
		if(re->second != fid){
			re->second = -1;
		}
	}
}

void get_unique_kmer_2(const string &file_name, int kmer_len, 
                       const vector<string> &ids, Write_file2 &w_file,
                       vector<kc_t>& kmer_collections)
{
    int fd = open(file_name.c_str(), O_RDONLY, 0);
    if(fd == -1)
    {
        cerr << "open file : " << file_name << "fail\n";
        exit(1);
    }

    uint64_t file_size = lseek(fd, 0, SEEK_END);
    char *buf = (char*)(mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0));

    //list<kmer_node> kmerslist;
    //vector<kmer_node> kmerslist;
    unordered_map<uint64_t, uint64_t> kmerslist;

    std::cout << "processing data: " << std::endl;
    for(uint64_t i = 0; i < file_size; )
    {
        uint64_t file_id = *((uint64_t*)(buf + i));
        i += sizeof(uint64_t);
        int super_kmer_len = ((uint8_t)(buf[i])) + kmer_len;
        i += sizeof(char);

        static const uint64_t mask = (1ull << (kmer_len * 2)) - 1;

        uint64_t kmer = 0;
        
        for(int zero = 0; zero < (kmer_len + 3) / 4; zero++)
        {
            kmer <<= 8;
            kmer |= ((uint8_t)buf[i + zero]);
        }
        //kmer |= ((uint8_t)buf[i + (kmer_len + 3) / 4]);
        //if(kmer_len % 4 != 0)
        //{
        //    kmer >>= (10 - 2 * (kmer_len % 4));
        //}
        //kmer >>= (64 - kmer_len * 2 + 2);
        if(kmer_len % 4)
            kmer >>= (10 - 2 * (kmer_len % 4));
        kmer &= mask;

        uint64_t kmer_rvs = 0;
        uint64_t kmer_tmp = kmer;
        for(int r = 0; r < kmer_len - 1; r++)
        {
            uint8_t base = kmer_tmp & 0x3;
            kmer_rvs += base;
            kmer_tmp >>= 2;
            kmer_rvs <<= 2;
        }
        //kmer_rvs += kmer_tmp;
        //kmer_rvs = (~kmer_rvs) & (mask >> 2);
        kmer_rvs = (~kmer_rvs) & mask;

        int index = 2 * kmer_len - 4; //start from 0
        for(int j = 0; j <= super_kmer_len - kmer_len; j++)
        {
            index += 2;
            //int index = (kmer_len - 1 + i) << 2;
            int index_1 = index / 8;
            int index_2 = index - (index_1 * 8);
            //char tmp_char = buf[i + index_1]; //for debug
            uint64_t base = ((buf[i + index_1]) >> (6 - index_2)) & 0x03;
            kmer <<= 2;
            kmer += base;
            kmer &= mask;

            kmer_rvs >>= 2;
            //base = 3 - base;
            base = base ^ 0b10;
            kmer_rvs |= (base << ((kmer_len - 1) * 2));

            //kmerslist.emplace_back(kmer, file_id);
            kmer_add(kmerslist, kmer, file_id);
#ifdef RVS
            //kmerslist.emplace_back(kmer_rvs, file_id);
            kmer_add(kmerslist, kmer_rvs, file_id);
#endif
        }
        i += (super_kmer_len + 3) / 4;
    }

    close(fd);
    munmap(buf, file_size);

    //kmer_node nodebuf[1024 * 4];
    //int buf_pos_ = 0;
    //find_unique(kmerslist, kmer_len, 0, 8, (kmer_len * 2 + 7) / 8 * 8, nodebuf, buf_pos_, 1024 * 4, w_file, ids);
    find_unique_2(kmerslist, w_file, ids, kmer_collections);
    //if(buf_pos_ != 0)
    //{
    //    //void Flush(kmer_node* buf, int maxbufsize, int kmer_len, int &buf_pos, Write_file &w_file)
    //    Flush2(nodebuf, buf_pos_, kmer_len, buf_pos_, w_file, ids);
    //}


    //std::vector<kmer_node>().swap(kmerslist);
    unordered_map<uint64_t, uint64_t>().swap(kmerslist);

    w_file.set_finished();
    //cout << "over**************************!!!!!!!!!!\n";
}
