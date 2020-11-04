#ifndef LOGPFERI_H
#define LOGPFERI_H

#include"Tube.h"
#include<iostream>
#include<memory>
using namespace std;
#define MAGIC 11
#define FILEMAX 50  //每个文件最大大小是50M
#define INFOMAX 50  //每条信息最大大小的4096


class InfoTag :public TubeImple
{
public:
    InfoTag(string info,int fd)
    {
        //this->info=info;
        this->fd=fd;
    }
    void doit() override
    {
        //将info的内容写入到fd对应的文件中
        int ret=write(fd,info.c_str(),info.size());
        if(ret==-1)
        {
            perror("write err:");
            throw bad_alloc();
        }
    }
    int getInfoSize()
    {
        return info.size();
    }
    void addInfo(string info_)
    {
        info+=info_;
    }
private:
    string info;//长度监控可以使用info去监控
    int fd;     //将info写入的通讯文件描述符
};



class logpferI
{
public:
    logpferI(string path);
    ~logpferI();
    void    addinfo(string info);

public:
    logpferI();
    void    setPath(string path);
    void    start();
public:
    void    setCanUsed();
    void    stop();
private:
    void    startConsumer();
    int     getFileSize();
    void    gettime(char *cur_time);
    string  compareFileName();
    void    change_file();
private:
    void    initFile();
private:
    int    magic;
    mutex  file_mu;
    Tube   tb;       //线程之间通讯的手段
    int    fd;       //通讯文件描述符
    string file_name;//文件原始的名字
    string path;
    int    index;
    shared_ptr<InfoTag> sh_info;
    atomic<int> record;
    mutex mu;
};

#endif // LOGPFERI_H
