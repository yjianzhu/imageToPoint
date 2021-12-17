#include<vector>
#include<string>
#include<iostream>
#include<algorithm>
#include<cmath>
#include<fstream>
#include<map>
#include"image.h"

using namespace std;



int main(int argc,char *argv[])
{
    //image read(argv[1]);

    //test
    string filename="../image/yipinhong.jpg";
    fstream read;
    read.open(filename,ios::in);
    
    read.close();
    image I_my(filename.c_str());

    cout<<I_my.height<<endl;






    // fstream read;
    // read.open(argv[1],ios::in);

    // read.seekg(0,read.end);
    // int file_length=read.tellg();
    // read.seekg(0,read.beg);

    // char* buffer=new char[file_length];
    // read.read(buffer,file_length);

    // read.close();

    // cout<<buffer<<endl;

    // delete[] buffer;  
    

    // return 0;
}