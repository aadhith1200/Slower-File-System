#ifndef OSFS_hpp
#define OSFS_hpp
#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;



const int DISK_SIZE = 100;
const int FILE_SIZE = 512;
static int openBlock = -1;
static int openMode = -1;
static int cursor = -1;




class Block
{
private:
    int number;
    Block* frwd;
    Block* back;
    string name;
public:
    Block();
    Block(int number, string name);
    ~Block();
    int getNumber();
    void setFrwd(Block* frwd);
    void setBack(Block* back);
    void reset();
    Block* getFrwd();
    Block* getBack();
    string getName();
};



class File: public Block
{
private:
    char data[FILE_SIZE];
public:
    File(int number, string name);
    ~File();
    void writeFile(int& count, string& input, int& current);
    void readFile(int& count, int& current);
    int getEnd();
};



class Disk: public Block
{
private:
    Block* sector[DISK_SIZE];
    bool freeSpaceList[DISK_SIZE];
public:
    Disk();
    ~Disk();
    int index();
    bool isOpen();
    void display();
    void count();
    void create(string name);
    void open(string name);
    void close();
    void write(int count, string input);
    void helpWrite(int&count,string&input,int&current);
    void read(int count);
    void seek(int base, int offset);
    Block* findBlock(string name);
    Block* helpCreate(string name);
};






#endif
