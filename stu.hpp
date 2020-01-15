#pragma once
#include<iostream>
#include<string>
#include<map>
#include<iomanip>
#include<fstream>
using std::string;
class Stu{
    public:
        //const double&pointView=point;
        friend int* pointSort(Stu stu[],int people);
        bool basicRead(string KeyName);
        void basicPrint();
        void basicStore(std::ofstream &out);
        void scoreRead();
        void scorePrint();
        void scoreStore(std::ofstream &out);
        void scoreLoad(std::ifstream &in);
        void scoreRearch();
        void pointCal();
        friend std::ostream& operator<<(std::ostream&out,Stu a);
        friend std::istream& operator>>(std::istream&out,Stu a);
        friend int nameSearch(Stu s[],int size,string nameS);
        friend int IDSearch(Stu s[],int size,string id);
        friend double ave(Stu s[],int size,string sub);
        friend int passNumber(Stu s[],int size,string sub);
        Stu operator=(const Stu &a);
        void scoreRevise(const string sub,const int revise);
        bool operator<(const Stu stu)
        {
            if(this->point<stu.point)return 1;
            else if(this->point==stu.point){
                if(this->name<stu.name)return 1;
                else if(this->name==stu.name)
                {
                    if(this->ID<stu.ID)return 1;
                }
                else return 0;
            }else return 0;
        }
        Stu(){}
        Stu(string n,string g,string id,string m,int yOs):name(n),gender(g),ID(id),majorInSchool(m),yearOfSchool(yOs){
            std::cout<<"This student "<<n<<" is constructed!"<<std::endl;
        }
        Stu(string n,string g,string id,string m,int yOs,double p):name(n),gender(g),ID(id),majorInSchool(m),yearOfSchool(yOs),point(p){
            std::cout<<"This student "<<n<<" is constructed!"<<std::endl;
        }
        Stu(const Stu &s):name(s.name),gender(s.gender),ID(s.ID),majorInSchool(s.majorInSchool),yearOfSchool(s.yearOfSchool),point(s.point){
            std::cout<<"This student "<<s.name<<" is constructed!"<<std::endl;}
        ~Stu(){std::cout<<"The student "<<name<<" is destructed"<<std::endl;}
    private:
        string name,gender,ID,majorInSchool;
        std::map<string,double> score;
        int yearOfSchool=2019;
        double point=0;
};
std::ostream& operator<<(std::ostream&out,Stu a);
std::istream& operator>>(std::istream&out,Stu a);
int* pointSort(Stu stu[],int people);
void Stu::basicPrint(){
    std::cout<<"The name of this student is "<<name<<" and the gender is "<<gender<<" ID is "<<ID<<" majors in "<<majorInSchool<<' '
    <<"and joined in "<<yearOfSchool<<" and now with the GPA "<<point<<std::endl;
}
void Stu::basicStore(std::ofstream &out){
    out<<name<<' '<<gender<<' '<<ID<<' '<<majorInSchool<<' '<<yearOfSchool<<' '<<point<<std::endl;
}
bool Stu::basicRead(string KeyName){
    string temp=KeyName;
    name=KeyName;
    if(temp=="EOF")
    return 0;
    std::cout<<"Please enter the gender:";
    std::cin>>gender;
    temp=gender;
    if(temp=="EOF")
    return 0;
    std::cout<<"Please enter the ID:";
    std::cin>>ID;
    temp=ID;
    if(temp=="EOF")
    return 0;
    std::cout<<"Please enter the major:";
    std::cin>>majorInSchool;
    temp=majorInSchool;
    if(temp=="EOF")
    return 0;
    std::cout<<"Please enter the year of School:";
    std::cin>>yearOfSchool;
    while(std::cin.fail())
    {
        getchar();getchar();
        std::cout<<"Input error! Please enter the year of School again:";
        std::cin.clear();
        std::cin>>yearOfSchool;
    }return 1;
}
void Stu::scoreRead(){
    string subject;
    double get_score;
    std::cout<<"Please input the subject and the score.If the subject is wrong make the score negtive."<<std::endl;
    std::cin>>subject;
    if(subject=="EOF")return;
    else {GS: std::cin>>get_score;
    if(get_score<0);
    else if(get_score>100){std::cout<<"wrong score!Please enter a correct one!"<<std::endl;goto GS;}
    else score[subject]=get_score;
    scoreRead();}
    //point+=get_score*4/(100*score.size());
}
void Stu::scoreStore(std::ofstream &out){
    for(const auto &s:score)
    out<<s.first<<' '<<s.second<<std::endl;
    std::cout<<std::endl<<"^D";
}
void Stu::scoreLoad(std::ifstream &in){
    string str1,str2;
    int t=9;
    while(t--){
        in>>str1;
        std::cout<<str1;
        if(str1=="")return;
        in>>str2;
        score[str1]=std::stod(str2);
    }
}
void Stu::scorePrint(){
    for(auto const &s:score)
    std::cout<<s.first<<" is "<<s.second<<" points"<<std::endl;
}
void Stu::scoreRearch(){
    string sub;
    std::cout<<"Which subjuct score do you want to search ?"<<std::endl;
    std::cin>>sub;
    auto it=score.find(sub);
    if(it!=score.end())std::cout<<"The score of "<<sub<<" is "<<it->second<<std::endl;
}
void Stu::pointCal()
{
    auto it=score.begin();
    int count=0;
    while(it!=score.end())
    {
        point+=it->second;
        ++it;
        ++count;
    }
    point/=count;
    point*=0.04;
}
void Stu::scoreRevise(const string sub,const int revise){
    bool exist=1;
    try{score.at(sub);}
    catch(std::out_of_range a){
        std::cout<<sub<<" is not read. Subject is out of range!"<<std::endl;
        exist=0;
    }if(exist)
        score[sub]=revise;
}
std::ostream& operator<<(std::ostream&out,Stu a){
    out<<a.name<<' '<<a.ID<<' '<<a.majorInSchool<<' '<<a.point<<std::endl;
    return out;
}
std::istream& operator>>(std::istream&in,Stu a){
    in>>a.name>>a.gender>>a.ID>>a.majorInSchool>>a.yearOfSchool;
    return in;
}
Stu Stu::operator=(const Stu &a){
    return Stu(a.name,a.gender,a.ID,a.majorInSchool,a.yearOfSchool,a.point);
    return *this;
}
int nameSearch(Stu s[],int size,string nameS)
{
    for(int i=0;i<size;++i)
    {
        if(s[i].name==nameS)
        return i;
    }
    return -1;
}
int IDSearch(Stu s[],int size,string id)
{
    for(int i=0;i<size;++i)
    {
        if(s[i].ID==id)
        return i;
    }
    return -1;
}
double ave(Stu s[],int size,string sub)
{
    double all=0;
    for(int i=0;i<size;++i)
    {
        all+=s[i].score[sub];
    }return all/size;
}
int passNumber(Stu s[],int size,string sub)
{
    int i=0;
    for(int j=0;j<size;++j)
    if(s[j].score[sub]>=60)++i;
    return i;
}
int* pointSort(Stu stu[],int people)//need delete by hand
{
    int *rank=new int[people];
    int *deal=new int[people];
    for(int i=0;i<people;++i)
    {
        deal[i]=0;rank[i]=0;
        for(int j=0;j<people;++j)
        {
            if(stu[i]<stu[j])
            ++deal[i];
        }
    }
    for(int i=0;i<people;++i){
    rank[deal[i]]=i;
    }
    delete deal;
    return rank;
}
