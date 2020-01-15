#include "stu.hpp"
int main(int argc,char**argv)
{
    std::cout<<std::setprecision(2);
    std::map<string,Stu>stu_map;
    std::cout<<"What do you want to do?"<<std::endl;
    while(1){
        char op;
        //bool state=1;
        std::cout<<" 1 reading data\n 2 loging data\n 3 print current infomation \n 4 saving data";
        std::cout<<"5 append data\n 6 add a new student\n 7 delete a student\n 8 exit\n"<<std::endl;
        std::cin>>op;getchar();
        /* try{std::cin>>op;
        if(op>4||op<1){
            throw"out of choice!";state=0;}
        }
        catch(const string fault){
            std::cout<<"the choice:"<<(int)op<<" is "<<fault<<std::endl;
        } */
        
        bool keep=false,exit=false,show=false;
        try{
        switch(op)
            {
                case '1':
                {
                    std::cout<<"When finishing reading , input \"EOF\" to end input"<<std::endl;
                    int flag=1;
                    while(flag)
                    {
                        string tempName;
                        std::cout<<"Please insert the name:"<<std::endl;
                        std::cin>>tempName;
                        flag=stu_map[tempName].basicRead(tempName);
                        if(flag)stu_map[tempName].scoreRead();
                        if(!flag)stu_map.erase(tempName);
                    }std::cout<<"Read finish!"<<std::endl;
                    show=true;
                }
                    break;
                case '2':
                {
                    std::ifstream out;
                    out.open(argv[1],std::ios::in);
                    string sp;
                    bool first=false;
                    for(getline(out,sp);sp!="^D";getline(out,sp))
                    {
                        if(sp=="")continue;
                        int pos[5],len[5],seq=0;
                        for(int i=0;i<sp.size();++i)
                        {
                            if(sp[i]==' '){pos[seq]=i,len[seq]=seq>0?pos[seq]-pos[seq-1]-1:pos[seq];
                            ++seq;}
                            first=true;
                        }
                        stu_map[sp.substr(0,len[0])]=Stu(sp.substr(0,len[0]),sp.substr(pos[0]+1,len[1]),sp.substr(pos[1]+1,len[2]),sp.substr(pos[2]+1,len[3]),std::stoi(sp.substr(pos[3]+1,len[4])),std::stod(sp.substr(pos[4]+1)));
                        stu_map[sp.substr(0,len[0])].scoreLoad(out);

                    }out.close();
                    if(!first&&sp=="^D")std::cout<<"No data to log"<<std::endl;
                }
                    break;
                case '3':
                {
                    if(show){for(const auto &stu:stu_map)
                        {stu_map[stu.first].basicPrint();
                        stu_map[stu.first].scorePrint();}
                    }else {
                        std::cout<<"These is nothing to print"<<std::endl<<std::endl;
                    }
                }
                    break;
                case '4':
                {
                KEEP:
                    std::ofstream out(argv[1],std::ios::trunc);
                    for(const auto &stu:stu_map)
                    {stu_map[stu.first].basicStore(out);
                    stu_map[stu.first].scoreStore(out);}
                    keep=true;
                    out<<"^D";out.close();
                    if(exit)return 0;
                }
                    break;
                case '5':
                {
                    std::ofstream out(argv[1],std::ios::app);
                    for(const auto &stu:stu_map)
                    {stu_map[stu.first].basicStore(out);
                    stu_map[stu.first].scoreStore(out);}
                    out.close();
                }
                    break;
                case '6':
                {
                    string tempName;
                    bool flag=0;
                    std::cout<<"Please insert the name:"<<std::endl;
                    std::cin>>tempName;
                    try{stu_map.at(tempName);}
                    catch(std::out_of_range){
                        flag=stu_map[tempName].basicRead(tempName);
                        if(flag)stu_map[tempName].scoreRead();
                        if(!flag){stu_map.erase(tempName);}
                        keep=false;
                        show=true;
                    }
                    std::cout<<"The Student "<<tempName<<" exists! The addition failed."<<std::endl;                  
                }
                    break;
                case '7':
                {
                    string tempName;
                    bool flag=0;
                    std::cout<<"Please insert the name:"<<std::endl;
                    std::cin>>tempName;
                    try{stu_map.at(tempName);}
                    catch(std::out_of_range){
                        std::cout<<"The Student "<<tempName<<" doesn't exist! The delete failed."<<std::endl;
                        goto end7;
                    }
                    stu_map.erase(tempName);
                    keep=false;end7:;
                }
                    break;
                case '8':
                {
                    if(!keep) {
                        std::cout<<"Detect that you have not saved the update data."
                                 <<"Do you want to save it before exit? "
                                 <<"( press Y or y to save it. Any other key to ignore)"<<std::endl;
                    exit=true;
                    char choice=getchar();
                    if(choice=='y'||choice=='Y')
                    goto KEEP;
                        }
                    return 0;
                }
                default:
                    throw"out of choice!";
            }
        }
        catch(const char* fault){
            std::cout<<"the choice:"<<(int)(op-'0')<<" is "<<fault<<std::endl;
        }
        /* std::cout<<"Is that finished?Press n or N ,and the press 'Enter' to exit.Press any other key to continue."<<std::endl;
        op=getchar();
        if(op=='n'||op=='N')break; */
    }
    return 0;
}
