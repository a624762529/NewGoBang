#include "logpferi.h"

logpferI::logpferI(string path_):tb(4)
{

    fd=8888;
    file_name=path_;//文件原始的名字
    path=path_;
    index=0;

    record=0;
    setCanUsed();
    initFile();
}

void logpferI::initFile()
{
    //初始化文件
    string path_=compareFileName();
    path=path_;
    fd=open(path_.c_str(),O_RDWR|O_CREAT,0777);
    if(fd==-1)
    {
        perror("open err:");
        throw bad_alloc();
    }
    sh_info=shared_ptr<InfoTag>(new InfoTag("",fd));
    thread th(&logpferI::startConsumer,this);
    th.detach();
}

logpferI::logpferI():tb(4)
{
    fd=8888;
    file_name="./file";//文件原始的名字
    path="./file";
    index=0;
    record=0;
}

void logpferI::setPath(string path_)
{
    path=path_;
}

int  logpferI::getFileSize()
{
    struct stat st;
    fstat (fd,&st);
    int ret=st.st_size/(1024*1024);
    return ret;
}


void logpferI::gettime(char *cur_time)
{
    char Year[6] = {0};
    char Month[4] = {0};
    char Day[4] = {0};
    char Hour[4] = {0};
    char Min[4] = {0};
    char Sec[4] = {0};

    time_t current_time;
    struct tm* now_time;
    time(&current_time);
    now_time = localtime(&current_time);

    strftime(Year, sizeof(Year), "%Y-", now_time);
    strftime(Month, sizeof(Month), "%m-", now_time);
    strftime(Day, sizeof(Day), "%d ", now_time);
    strftime(Hour, sizeof(Hour), "%H:", now_time);
    strftime(Min, sizeof(Min), "%M:", now_time);
    strftime(Sec, sizeof(Sec), "%S", now_time);

    strncat(cur_time, Year, 5);
    strncat(cur_time, Month, 3);
    strncat(cur_time, Day, 3);
    strncat(cur_time, Hour, 3);
    strncat(cur_time, Min, 3);
    strncat(cur_time, Sec, 3);
}

string logpferI::compareFileName()
{
    char *time=new char[21];
    memset(time,0,sizeof(time));
    gettime(time);
    string ret=file_name+time;
    delete[] time;
    ret+=string(" ")+to_string(index);
    index++;
    return ret;

}

void logpferI::change_file()
{
    string path_=compareFileName();
    path=path_;
    {
        //lock_guard<mutex> lock(file_mu);
        //关闭通讯文件描述符
        int new_fd=open(path_.c_str(),O_RDWR|O_CREAT,0777);
        if(new_fd==-1)
        {
            cout<<"open err"<<__LINE__<<__FUNCTION__<<endl;
            throw bad_alloc();
        }
        int ret=dup2(new_fd,fd);
        if(ret==-1)
        {
            perror("err: dup2");
            throw bad_alloc();
        }
    }
    return;
}

void logpferI::startConsumer()
{
    cout<<"start consumer"<<endl;
    while (magic==MAGIC)
    {
        shared_ptr<TubeImple> sh_info=tb.get();
        lock_guard<mutex> lock(file_mu);
        if(sh_info)
        {
            sh_info->doit();
            int file_size=getFileSize();
            if(file_size>=FILEMAX)
            {
                change_file();
            }
        }
    }

}

void logpferI::setCanUsed()
{
    magic=MAGIC;
}

void logpferI::stop()
{
    magic=0;
    lock_guard<mutex> lock(file_mu);//对文件进行上锁
    //进行清理工作
    shared_ptr<InfoTag> tem=sh_info;
    int info_size=tem->getInfoSize();
    if(info_size!=0)
    {
        tb.put(tem);
        //把tem放进去
    }
    tb.put(nullptr);
}

void logpferI::addinfo(string info)
{
    lock_guard<mutex> lock(mu);
    sh_info->addInfo(info);
    int size=sh_info->getInfoSize();
    if(size>INFOMAX)
    {
        tb.put(sh_info);
        sh_info=shared_ptr<InfoTag>(new InfoTag("",fd));
    }
    record++;
}

logpferI::~logpferI()
{
    lock_guard<mutex> lock(file_mu);
    for(int i=0;i<tb.size();i++)
    {
        auto ret=tb.get();
        ret->doit();
    }
}
