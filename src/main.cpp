#include<vector>
#include<string>
#include<iostream>
#include<algorithm>
#include<cmath>
#include<fstream>
#include<map>

using namespace std;



int main(int argc,char *argv[])
{
    fstream read;
    read.open(argv[1],ios::in);

    read.seekg(0,read.end);
    int file_length=read.tellg();
    read.seekg(0,read.beg);

    char* buffer=new char[file_length];
    read.read(buffer,file_length);

    read.close();

    cout<<buffer<<endl;

    delete[] buffer;  
    

    return 0;
}