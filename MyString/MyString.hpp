#pragma once
#include<string.h>
#include<iostream>
void GetRoom(char * _des,const char *_src,int &cap);
int CapCal(int num){
    int level=1;
    while(num>level*15){level*=2;}
    return level*15+1; 
}
class MyString{
    char *str=nullptr;
    int capability=0;
public:
    MyString():str(nullptr),capability(15){}
    MyString(const char* s);
    MyString(MyString const &s):str(s.str),capability(s.capability){}
    virtual ~MyString(){delete str;}
    MyString& operator=(MyString &s);
    char* operator+(const char *st);
    char* operator+(MyString &s);
    MyString& operator+=(MyString &s){
        if(capability<this->size()+s.size()) GetRoom(this->str,s.str,capability);
        else strcat(str,s.str);
        return *this;
    }
    MyString& operator+=(const char*st){
        if(capability<this->size()+strlen(st)) GetRoom(this->str,st,capability);
        else strcat(str,st);
        return *this;
    }
    int size();
    int capable(){return capability;}
    virtual char operator[](unsigned index);
    friend std::ostream&operator<<(std::ostream&out,MyString &a);
    friend std::istream&operator>>(std::istream&in,MyString &a);
};
void GetRoom(char * _des,const char *_src,int &cap){
    int tempNum=CapCal(strlen(_des)+strlen(_src));
    std::cout<<tempNum<<std::endl;
    if(!_des){_des=new char[tempNum];
    strcat(_des,_src);
    }else{
        char *temp=new char[tempNum];
        strcat(temp,_des);
        strcat(temp,_src);
        delete _des;
        _des=temp;
    }cap=tempNum;
}
int MyString::size(){
    if(str==nullptr)return 0;
    else return strlen(str);
}
char MyString::operator[](unsigned index){
    try{if(index>this->size())throw"out of range!";}
    catch(const char* error){std::cout<<"The index: "<<index
    <<" is "<<error<<std::endl;return '\0';}
    return *(str+index);
}
char* MyString::operator+(const char *st){
    return strcat(str,st);
}
char* MyString::operator+(MyString &s){
    return strcat(str,s.str);
}
MyString& MyString::operator=(MyString &s){
        if(this->str==s.str)return *this;
        else{
            delete str;
            str=s.str;
            capability=s.capability;
            return *this;
        }
    }
MyString::MyString(const char* s){
    int GetCap=CapCal(strlen(s));
    str=new char[GetCap];
    strcat(str,s);
    capability=GetCap;
}
std::ostream& operator<<(std::ostream&out,MyString &a){
    out<<a.str;
    return out;
}
std::istream& operator>>(std::istream&in,MyString &a){
    char temp[4096];
    in>>temp;
    a.capability=CapCal(strlen(temp));
    delete a.str;
    a.str=new char[a.capability];
    strcat(a.str,temp);
    return in;
}
