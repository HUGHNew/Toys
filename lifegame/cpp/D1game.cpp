#include<iostream>
#include<array>
#include<bitset>
#include<thread>
#include"RGap.hpp"


template<size_t T>

class OneDimension : public RGap_Base{
    private:
        std::array<int,T> array;
        void ruleCheck(size_t pos,int (*fn)(int left,int mid,int right)){
            switch(pos){
                case 0:
                    break;
                case T:
                    break;
                default:
                    array[pos]=fn(array[pos-1],array[pos],array[pos+1]);
            }
        }
    public:
        OneDimension(std::initializer_list<int>const& init,size_t r=2048,size_t msGap=200):RGap_Base(r,msGap),array(){
            for(int i:init){
                if(i<T)array[i]=1;
            }
        }
        ~OneDimension()=default;
        void run(){
            constexpr std::chrono::duration<long double, std::milli> mil{round_gap_ms};
            int Rcount=0;
            while(Rcount<this->round){
                std::cout<<"------------"<<std::endl
                    <<"Round : "<<Rcount++<<std::endl
                    <<"------------"<<std::endl;
                for(int i:array){
                    std::cout<<i<<' ';
                }std::cout<<std::endl;
                for(int i=0;i<array.size();++i)
                    ruleCheck(i,[](int left,int mid,int right)->int{
                        // int duo=left&right;
                        if(left==0&&right==0)return 0;
                        return left&right?(!mid):mid;
                    });
                std::this_thread::sleep_for(mil);
                // _sleep(this->round_gap_ms);
            }
        }
};
