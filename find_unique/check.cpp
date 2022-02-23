//verify that the result is correct
//argv[1] : true answer path
//argv[2] : check answer path
//check if two files contain exactly the same entries.
#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <cstdint>
#include <string>

using namespace std;

int main(int argc, char** argv)
{
    map<string, string> true_answer_;

    fstream f(argv[1], ios::in);

    //fstream f_("argv1_file.txt", ios::out);

    if(!f)
    {
        cerr << "open " << argv[1] << " fail\n";
        exit(0);
    }
    while(!f.eof())
    {
        char buf[256] = {0};
        f.getline(buf, 256);
        char buf_[256] = {0};
        f.getline(buf_, 256);
        string id(buf);
        string kmer(buf_);
        //f_ << id << endl;
        //f_ << kmer << endl;
        //exit(0);
        if(kmer.size() != 0)
        {
            if(true_answer_.find(kmer) != true_answer_.end())
            {
                if(true_answer_[kmer] != id)
                    true_answer_[kmer] = "F";
            }
            else
            {
                true_answer_[kmer] = id;
            }
        }
    }

    map<string, string> true_answer;
    for(auto i = true_answer_.begin(); i != true_answer_.end(); i++)
    {
        if(i->second != "F")
            true_answer[i->first] = i->second;
    }

    f.close();
    //f_.close();

    cerr << "read complete\n";

    map<string, string> check_answer;

    fstream f1(argv[2], ios::in);
    //fstream f1_("argv2_file.txt", ios::out);
    if(!f1)
    {
        cerr << "open " << argv[2] << " fail\n";
        exit(0);
    }

    //int count_ = 0;

    while(!f1.eof())
    {
        char buf[256] = {0};
        f1.getline(buf, 256);
        char buf_[256] = {0};
        f1.getline(buf_, 256);
        string id(buf);
        string kmer(buf_);

        //f1_ << id << endl;
        //f1_ << kmer << endl;
        //count_++;
        //if(count_ == 20)
        //    exit(0);

        if(kmer.size() != 0)
        {
            check_answer[kmer] = id;
        }
    }
    f1.close();
    //f1_.close();

    cerr << "read complete\n";

    //check true_answer in check_answer
    cout << "true_answer in check_answer\n";
    for(auto i = true_answer.begin(); i != true_answer.end(); i++)
    {
        if(check_answer.find(i->first) == check_answer.end())
            cout << i->first << ":" << i->second << endl;
    }

    cerr << "compare complete\n";

    cout << "check_answer in true_answer\n";
    for(auto i = check_answer.begin(); i != check_answer.end(); i++)
    {
        if(true_answer.find(i->first) == true_answer.end())
            cout << i->first << ":" << i->second << endl;
    }
    
    cerr << "compare complete\n";

    return 0;
}

