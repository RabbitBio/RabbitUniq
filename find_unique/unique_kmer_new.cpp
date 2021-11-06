#include "unique_kmer_new.h"
#include <unordered_set>
#include <deque>
#include <mutex>


std::mutex cout_mut;
//test rsync

const char kmerToBase[256][4] = 
{
     {'A', 'A', 'A', 'A'},
     {'A', 'A', 'A', 'C'},
     {'A', 'A', 'A', 'G'},
     {'A', 'A', 'A', 'T'},
     {'A', 'A', 'C', 'A'},
     {'A', 'A', 'C', 'C'},
     {'A', 'A', 'C', 'G'},
     {'A', 'A', 'C', 'T'},
     {'A', 'A', 'G', 'A'},
     {'A', 'A', 'G', 'C'},
     {'A', 'A', 'G', 'G'},
     {'A', 'A', 'G', 'T'},
     {'A', 'A', 'T', 'A'},
     {'A', 'A', 'T', 'C'},
     {'A', 'A', 'T', 'G'},
     {'A', 'A', 'T', 'T'},
     {'A', 'C', 'A', 'A'},
     {'A', 'C', 'A', 'C'},
     {'A', 'C', 'A', 'G'},
     {'A', 'C', 'A', 'T'},
     {'A', 'C', 'C', 'A'},
     {'A', 'C', 'C', 'C'},
     {'A', 'C', 'C', 'G'},
     {'A', 'C', 'C', 'T'},
     {'A', 'C', 'G', 'A'},
     {'A', 'C', 'G', 'C'},
     {'A', 'C', 'G', 'G'},
     {'A', 'C', 'G', 'T'},
     {'A', 'C', 'T', 'A'},
     {'A', 'C', 'T', 'C'},
     {'A', 'C', 'T', 'G'},
     {'A', 'C', 'T', 'T'},
     {'A', 'G', 'A', 'A'},
     {'A', 'G', 'A', 'C'},
     {'A', 'G', 'A', 'G'},
     {'A', 'G', 'A', 'T'},
     {'A', 'G', 'C', 'A'},
     {'A', 'G', 'C', 'C'},
     {'A', 'G', 'C', 'G'},
     {'A', 'G', 'C', 'T'},
     {'A', 'G', 'G', 'A'},
     {'A', 'G', 'G', 'C'},
     {'A', 'G', 'G', 'G'},
     {'A', 'G', 'G', 'T'},
     {'A', 'G', 'T', 'A'},
     {'A', 'G', 'T', 'C'},
     {'A', 'G', 'T', 'G'},
     {'A', 'G', 'T', 'T'},
     {'A', 'T', 'A', 'A'},
     {'A', 'T', 'A', 'C'},
     {'A', 'T', 'A', 'G'},
     {'A', 'T', 'A', 'T'},
     {'A', 'T', 'C', 'A'},
     {'A', 'T', 'C', 'C'},
     {'A', 'T', 'C', 'G'},
     {'A', 'T', 'C', 'T'},
     {'A', 'T', 'G', 'A'},
     {'A', 'T', 'G', 'C'},
     {'A', 'T', 'G', 'G'},
     {'A', 'T', 'G', 'T'},
     {'A', 'T', 'T', 'A'},
     {'A', 'T', 'T', 'C'},
     {'A', 'T', 'T', 'G'},
     {'A', 'T', 'T', 'T'},
     {'C', 'A', 'A', 'A'},
     {'C', 'A', 'A', 'C'},
     {'C', 'A', 'A', 'G'},
     {'C', 'A', 'A', 'T'},
     {'C', 'A', 'C', 'A'},
     {'C', 'A', 'C', 'C'},
     {'C', 'A', 'C', 'G'},
     {'C', 'A', 'C', 'T'},
     {'C', 'A', 'G', 'A'},
     {'C', 'A', 'G', 'C'},
     {'C', 'A', 'G', 'G'},
     {'C', 'A', 'G', 'T'},
     {'C', 'A', 'T', 'A'},
     {'C', 'A', 'T', 'C'},
     {'C', 'A', 'T', 'G'},
     {'C', 'A', 'T', 'T'},
     {'C', 'C', 'A', 'A'},
     {'C', 'C', 'A', 'C'},
     {'C', 'C', 'A', 'G'},
     {'C', 'C', 'A', 'T'},
     {'C', 'C', 'C', 'A'},
     {'C', 'C', 'C', 'C'},
     {'C', 'C', 'C', 'G'},
     {'C', 'C', 'C', 'T'},
     {'C', 'C', 'G', 'A'},
     {'C', 'C', 'G', 'C'},
     {'C', 'C', 'G', 'G'},
     {'C', 'C', 'G', 'T'},
     {'C', 'C', 'T', 'A'},
     {'C', 'C', 'T', 'C'},
     {'C', 'C', 'T', 'G'},
     {'C', 'C', 'T', 'T'},
     {'C', 'G', 'A', 'A'},
     {'C', 'G', 'A', 'C'},
     {'C', 'G', 'A', 'G'},
     {'C', 'G', 'A', 'T'},
     {'C', 'G', 'C', 'A'},
     {'C', 'G', 'C', 'C'},
     {'C', 'G', 'C', 'G'},
     {'C', 'G', 'C', 'T'},
     {'C', 'G', 'G', 'A'},
     {'C', 'G', 'G', 'C'},
     {'C', 'G', 'G', 'G'},
     {'C', 'G', 'G', 'T'},
     {'C', 'G', 'T', 'A'},
     {'C', 'G', 'T', 'C'},
     {'C', 'G', 'T', 'G'},
     {'C', 'G', 'T', 'T'},
     {'C', 'T', 'A', 'A'},
     {'C', 'T', 'A', 'C'},
     {'C', 'T', 'A', 'G'},
     {'C', 'T', 'A', 'T'},
     {'C', 'T', 'C', 'A'},
     {'C', 'T', 'C', 'C'},
     {'C', 'T', 'C', 'G'},
     {'C', 'T', 'C', 'T'},
     {'C', 'T', 'G', 'A'},
     {'C', 'T', 'G', 'C'},
     {'C', 'T', 'G', 'G'},
     {'C', 'T', 'G', 'T'},
     {'C', 'T', 'T', 'A'},
     {'C', 'T', 'T', 'C'},
     {'C', 'T', 'T', 'G'},
     {'C', 'T', 'T', 'T'},
     {'G', 'A', 'A', 'A'},
     {'G', 'A', 'A', 'C'},
     {'G', 'A', 'A', 'G'},
     {'G', 'A', 'A', 'T'},
     {'G', 'A', 'C', 'A'},
     {'G', 'A', 'C', 'C'},
     {'G', 'A', 'C', 'G'},
     {'G', 'A', 'C', 'T'},
     {'G', 'A', 'G', 'A'},
     {'G', 'A', 'G', 'C'},
     {'G', 'A', 'G', 'G'},
     {'G', 'A', 'G', 'T'},
     {'G', 'A', 'T', 'A'},
     {'G', 'A', 'T', 'C'},
     {'G', 'A', 'T', 'G'},
     {'G', 'A', 'T', 'T'},
     {'G', 'C', 'A', 'A'},
     {'G', 'C', 'A', 'C'},
     {'G', 'C', 'A', 'G'},
     {'G', 'C', 'A', 'T'},
     {'G', 'C', 'C', 'A'},
     {'G', 'C', 'C', 'C'},
     {'G', 'C', 'C', 'G'},
     {'G', 'C', 'C', 'T'},
     {'G', 'C', 'G', 'A'},
     {'G', 'C', 'G', 'C'},
     {'G', 'C', 'G', 'G'},
     {'G', 'C', 'G', 'T'},
     {'G', 'C', 'T', 'A'},
     {'G', 'C', 'T', 'C'},
     {'G', 'C', 'T', 'G'},
     {'G', 'C', 'T', 'T'},
     {'G', 'G', 'A', 'A'},
     {'G', 'G', 'A', 'C'},
     {'G', 'G', 'A', 'G'},
     {'G', 'G', 'A', 'T'},
     {'G', 'G', 'C', 'A'},
     {'G', 'G', 'C', 'C'},
     {'G', 'G', 'C', 'G'},
     {'G', 'G', 'C', 'T'},
     {'G', 'G', 'G', 'A'},
     {'G', 'G', 'G', 'C'},
     {'G', 'G', 'G', 'G'},
     {'G', 'G', 'G', 'T'},
     {'G', 'G', 'T', 'A'},
     {'G', 'G', 'T', 'C'},
     {'G', 'G', 'T', 'G'},
     {'G', 'G', 'T', 'T'},
     {'G', 'T', 'A', 'A'},
     {'G', 'T', 'A', 'C'},
     {'G', 'T', 'A', 'G'},
     {'G', 'T', 'A', 'T'},
     {'G', 'T', 'C', 'A'},
     {'G', 'T', 'C', 'C'},
     {'G', 'T', 'C', 'G'},
     {'G', 'T', 'C', 'T'},
     {'G', 'T', 'G', 'A'},
     {'G', 'T', 'G', 'C'},
     {'G', 'T', 'G', 'G'},
     {'G', 'T', 'G', 'T'},
     {'G', 'T', 'T', 'A'},
     {'G', 'T', 'T', 'C'},
     {'G', 'T', 'T', 'G'},
     {'G', 'T', 'T', 'T'},
     {'T', 'A', 'A', 'A'},
     {'T', 'A', 'A', 'C'},
     {'T', 'A', 'A', 'G'},
     {'T', 'A', 'A', 'T'},
     {'T', 'A', 'C', 'A'},
     {'T', 'A', 'C', 'C'},
     {'T', 'A', 'C', 'G'},
     {'T', 'A', 'C', 'T'},
     {'T', 'A', 'G', 'A'},
     {'T', 'A', 'G', 'C'},
     {'T', 'A', 'G', 'G'},
     {'T', 'A', 'G', 'T'},
     {'T', 'A', 'T', 'A'},
     {'T', 'A', 'T', 'C'},
     {'T', 'A', 'T', 'G'},
     {'T', 'A', 'T', 'T'},
     {'T', 'C', 'A', 'A'},
     {'T', 'C', 'A', 'C'},
     {'T', 'C', 'A', 'G'},
     {'T', 'C', 'A', 'T'},
     {'T', 'C', 'C', 'A'},
     {'T', 'C', 'C', 'C'},
     {'T', 'C', 'C', 'G'},
     {'T', 'C', 'C', 'T'},
     {'T', 'C', 'G', 'A'},
     {'T', 'C', 'G', 'C'},
     {'T', 'C', 'G', 'G'},
     {'T', 'C', 'G', 'T'},
     {'T', 'C', 'T', 'A'},
     {'T', 'C', 'T', 'C'},
     {'T', 'C', 'T', 'G'},
     {'T', 'C', 'T', 'T'},
     {'T', 'G', 'A', 'A'},
     {'T', 'G', 'A', 'C'},
     {'T', 'G', 'A', 'G'},
     {'T', 'G', 'A', 'T'},
     {'T', 'G', 'C', 'A'},
     {'T', 'G', 'C', 'C'},
     {'T', 'G', 'C', 'G'},
     {'T', 'G', 'C', 'T'},
     {'T', 'G', 'G', 'A'},
     {'T', 'G', 'G', 'C'},
     {'T', 'G', 'G', 'G'},
     {'T', 'G', 'G', 'T'},
     {'T', 'G', 'T', 'A'},
     {'T', 'G', 'T', 'C'},
     {'T', 'G', 'T', 'G'},
     {'T', 'G', 'T', 'T'},
     {'T', 'T', 'A', 'A'},
     {'T', 'T', 'A', 'C'},
     {'T', 'T', 'A', 'G'},
     {'T', 'T', 'A', 'T'},
     {'T', 'T', 'C', 'A'},
     {'T', 'T', 'C', 'C'},
     {'T', 'T', 'C', 'G'},
     {'T', 'T', 'C', 'T'},
     {'T', 'T', 'G', 'A'},
     {'T', 'T', 'G', 'C'},
     {'T', 'T', 'G', 'G'},
     {'T', 'T', 'G', 'T'},
     {'T', 'T', 'T', 'A'},
     {'T', 'T', 'T', 'C'},
     {'T', 'T', 'T', 'G'},
     {'T', 'T', 'T', 'T'}
};

Write_file::Write_file(const string& file_name, int finished_) : finished(finished_), has_finished(0)
{
    //f.open(file_name, ios::out);
    f = fopen(file_name.c_str(), "wb");
    if(!f)
    {
        cerr << "open file " << file_name << " fail\n";
        exit(1);
    }
    arr = new std::pair<uint64_t*, int>[1<<20];
    memset(arr, 0, sizeof(std::pair<uint64_t*, int>) * (1 << 20)); //-------????
    write_pos = 0;
    read_pos = 0;
}

Write_file::~Write_file()
{
    //if(f.is_open())
    //    f.close();
    if(f){
      fclose(f);
      f = NULL;
    }
}

void Write_file::push(uint64_t* buf, int buf_pos)
{
    //lock_guard<mutex> lk(mut);
    //dq.push_back({buf, buf_pos});
    //data_cond.notify_one();
    //cout << "push, and size of dq is: " << dq.size() << endl;
    arr[write_pos++] = {buf, buf_pos};
}

void Write_file::set_finished()
{
    has_finished++;
    data_cond.notify_one();
    //cout << "thread : " << has_finished << endl;
}

void Write_file::operator()()
{
  double s = get_time();
  char *barry = new char[222ul * (1ul << 30)];
  uint64_t p_barry = 0;
  while(true){
    while( has_finished != finished && read_pos >= write_pos ){
      usleep(50);
    }
    if(read_pos >= write_pos){
      break;
    }
    while(arr[read_pos].first == nullptr)
      usleep(10);

    //f.write((char*)(arr[read_pos].first), arr[read_pos].second * 8);
    fwrite((char*)(arr[read_pos].first), arr[read_pos].second * 8, 1, f);
    //memcpy(barry + p_barry, arr[read_pos].first, arr[read_pos].second*8);
    //p_barry += arr[read_pos].second*8;

    delete [] arr[read_pos].first;
    read_pos++;
  }
  double e = get_time();
  double ti = e - s;
  {
    std::lock_guard<std::mutex> lk(cout_mut);
    cout << "write over**************************!!!!!!!!!! time " << ti << endl;
  }
  //double s2 = get_time();
  //fwrite(barry, p_barry, 1, f);
  //delete [] barry;
  //double e2 = get_time();
  //double ti2 = e2 - s2;
  //{
  //  std::lock_guard<std::mutex> lk(cout_mut);
  //  cout << "write big array time " << ti2 << endl;
  //}

    /*
    double s = get_time();
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
                //f.write(top.first, top.second * sizeof(char));
                f.write((char*)(top.first), 
                        top.second * sizeof(uint64_t) / sizeof(char));
                delete [] top.first;
            }
            //cout << "\nover!!!!!!!!!!!!!\n";
            break;
        }
        //pair<char*, int> p = dq.front();
        pair<uint64_t*, int> p = dq.front();
        dq.pop_front();
        lk.unlock();
        //f.write(p.first, p.second * sizeof(char));
        //f.write(p.first, p.second * sizeof(uint64_t));
        f.write((char*)(p.first), 
            p.second * sizeof(uint64_t) / sizeof(char));
        delete [] p.first;
    }
    double e = get_time();
    double ti = e - s;
    {
      std::lock_guard<std::mutex> lk(cout_mut);
      cout << "write over**************************!!!!!!!!!! time " << ti << endl;
    }
    */
}


void Flush(kmer_node *buf, int maxbufsize, int kmer_len, int &buf_pos, Write_file &w_file, const vector<string> &ids)
{
  //uint64_t *tmpbuf = new uint64_t[2 * maxbufsize];
  uint64_t *tmpbuf = new uint64_t[2 * buf_pos];
  int tmpbufsize = 0;
  for (int x = 0; x < buf_pos; x++)
  {
    tmpbuf[tmpbufsize++] = buf[x].id;
    tmpbuf[tmpbufsize++] = buf[x].kmer;
  }

  w_file.push(tmpbuf, tmpbufsize);
  buf_pos = 0;
}

void find_unique(unordered_map<uint64_t, uint64_t> &kmerslist, int kmer_len, kmer_node* buf, 
                 int &buf_pos, int maxbufsize, Write_file &w_file, const vector<string> &ids)
{
	const uint64_t notuniq = -1;
	for(auto& k : kmerslist){
		if(k.second != notuniq){
			buf[buf_pos++] = kmer_node{k.first, k.second};
			if(buf_pos == maxbufsize)
			{
				Flush(buf, maxbufsize, kmer_len, buf_pos, w_file, ids);
			}
		}
	}
  if(buf_pos != 0){
      Flush(buf, maxbufsize, kmer_len, buf_pos, w_file, ids);
  }
}


inline void print(uint64_t kmer, int kl)
{
    char kmerstr[kl];
    static char maparray[4] = {'A', 'C', 'G', 'T'};
    for(int i = 0; i < kl; i++)
    {
        uint8_t base = kmer & 0x3;
        kmerstr[kl - 1 - i] = maparray[base];
        kmer >>= 2;
    }
    cout << string(kmerstr) << endl; 
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

void get_unique_kmer(const string &file_name, int kmer_len, const vector<string> &ids, Write_file &w_file)
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
            //kmer_rvs += base;
            kmer_rvs += (base ^ 0b10);
            kmer_tmp >>= 2;
            kmer_rvs <<= 2;
        }
        //kmer_rvs = (~kmer_rvs) & mask;
        kmer_rvs = kmer_rvs & mask;

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

    const int MAXBUFSIZE = 1 << 19;
    kmer_node *nodebuf = new kmer_node[MAXBUFSIZE];
    int buf_pos_ = 0;
    //find_unique(kmerslist, kmer_len, 0, 8, (kmer_len * 2 + 7) / 8 * 8, nodebuf, buf_pos_, 1024 * 4, w_file, ids);
    find_unique(kmerslist, kmer_len, nodebuf, buf_pos_, MAXBUFSIZE, w_file, ids);
    if(buf_pos_ != 0)
    {
        //void Flush(kmer_node* buf, int maxbufsize, int kmer_len, int &buf_pos, Write_file &w_file)
        Flush(nodebuf, buf_pos_, kmer_len, buf_pos_, w_file, ids);
    }


    //std::vector<kmer_node>().swap(kmerslist);
    unordered_map<uint64_t, uint64_t>().swap(kmerslist);

    w_file.set_finished();
    delete nodebuf;
   
}