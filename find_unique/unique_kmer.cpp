#include "unique_kmer.h"

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
    f.open(file_name, ios::out);
    if(!f)
    {
        cerr << "open file " << file_name << " fail\n";
        exit(1);
    }
}

Write_file::~Write_file()
{
    if(f.is_open())
        f.close();
}

void Write_file::push(char* buf, int buf_pos)
{
    lock_guard<mutex> lk(mut);
    dq.push_back({buf, buf_pos});
    data_cond.notify_one();
    //cout << "push, and size of dq is: " << dq.size() << endl;
    //buf = new char[buflen];
}

void Write_file::set_finished()
{
    has_finished++;
    data_cond.notify_one();
    //cout << "thread : " << has_finished << endl;
}

void Write_file::operator()()
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
                delete [] top.first;
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

void Flush(kmer_node* buf, int maxbufsize, int kmer_len, int &buf_pos, Write_file &w_file, const vector<string> &ids)
{
    
    char *tmpbuf = new char[96 * maxbufsize];
    int tmpbufsize = 0;

    for(int x = 0; x < maxbufsize; x++)
    {
        tmpbuf[tmpbufsize++] = '>';
        //cout << "id = " << buf[x].id << endl;
        const string &idstring = ids[buf[x].id];
        //cout << buf[x].id << endl;
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



void find_unique(vector<kmer_node> &v, int kmer_len, int bitstart, int bitlen, int maxbit_len, kmer_node* buf, int &buf_pos, int maxbufsize, Write_file &w_file, const vector<string> &ids)
{

    //vector<list<kmer_node>> buckets(1ull << bitlen);
    vector<vector<kmer_node>> buckets(1ull << bitlen);

    const uint64_t mask = (1ull << bitlen) - 1;

    int l = v.size();
    for(int i = 0; i < l; i++)
        buckets[(v[i].kmer >> bitstart) & mask].emplace_back(v[i].kmer, v[i].id);

    std::vector<kmer_node>().swap(v);

    l = buckets.size();
    for(int i = 0; i < l; i++)
    {
        //vector<kmer_node> ltov(buckets[i].begin(), buckets[i].end());
        vector<kmer_node> &ltov = buckets[i];
        //std::list<kmer_node>().swap(buckets[i]);
        int vl = ltov.size();
        if(vl == 0)
            continue;
        if(vl == 1)
        {
            //cout << buf_pos << endl;
            buf[buf_pos++] = ltov[0];
            //cout << buf_pos << endl;
            //cout << "!!\n";
            if(buf_pos == maxbufsize)
            {

                //void Flush(kmer_node* buf, int maxbufsize, int kmer_len, int &buf_pos, Write_file &w_file)
                Flush(buf, maxbufsize, kmer_len, buf_pos, w_file, ids);
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
        //std::vector<kmer_node>().swap(ltov);
    }

    //return buf_pos;
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
    vector<kmer_node> kmerslist;

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
            base = 3 - base;
            kmer_rvs |= (base << ((kmer_len - 1) * 2));

            kmerslist.emplace_back(kmer, file_id);
            //if (file_id == 2768)
            //    print(kmer, 25);
#ifdef RVS
            kmerslist.emplace_back(kmer_rvs, file_id);
#endif

            //cout << kmer << "|" << kmer_rvs << endl;

            //count_push++; //for debug
        }

        i += (super_kmer_len + 3) / 4;
    }

    close(fd);
    munmap(buf, file_size);

    //vector<kmer_node> kmersvector(kmerslist.begin(), kmerslist.end());
    //std::list<kmer_node>().swap(kmerslist);
    kmer_node nodebuf[1024 * 4];
    int buf_pos_ = 0;
    find_unique(kmerslist, kmer_len, 0, 8, (kmer_len * 2 + 7) / 8 * 8, nodebuf, buf_pos_, 1024 * 4, w_file, ids);
    //cout << "buf_pos : " << buf_pos_ << endl;
    //cout << "buf_pos_ = " << buf_pos_ << endl;
    if(buf_pos_ != 0)
    {
        //void Flush(kmer_node* buf, int maxbufsize, int kmer_len, int &buf_pos, Write_file &w_file)
        Flush(nodebuf, buf_pos_, kmer_len, buf_pos_, w_file, ids);
    }


    std::vector<kmer_node>().swap(kmerslist);

    w_file.set_finished();
    //cout << "over**************************!!!!!!!!!!\n";
}
