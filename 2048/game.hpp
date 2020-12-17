#ifndef _GAME_2048_H_
#define _GAME_2048_H_ 1
#include<iostream>
#include<vector>
class game{
        int table[4][4];
        std::vector np;
    public:
        void quit(){}
    void show(){
        for(int i=0;i<16;++i)
            std::cout<<table[i/4][i%4]<<((i+1)/4==0?'\n':'\t');
        std::cout<<std::endl;
    }
};
void clear(){
    #if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
        #include<windows.h>
        system("cls");
    #else
        #include<unistd.h>
        system("clear");
    #endif
}
#endif
