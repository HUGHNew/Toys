#include<iostream>
#include<array>
#include<cmath>
#include<vector>
#include<thread>
#include"RGap.hpp"




struct coordinate{
    size_t x,y;
    coordinate(size_t xv,size_t yx):x(xv),y(yx){}
    double distance(const coordinate&other)const{
        return std::sqrt(std::pow(other.x-x,2)+std::pow(other.y-y,2));
    }
};

template<typename T,size_t Size>
// unusual version
std::ostream& operator<<(std::ostream&os,std::array<T,Size>const& ay){
    for(auto&&it:ay){
        os<<it%10<<' ';
    }return os;
}
template<size_t Size>
class PlantLifeGame:public RGap_Base{
    private:// around*10+state
        std::array<std::array<int,Size>,Size>matrix;
    protected:
        virtual int rules(coordinate const&cd){
            return rules(cd.x,cd.y);
        }
        /**
         * four rules
         * 1. around=3 death=>alive
         * 2. around<2 alive=>death
         * 3. around=2/3 alive(keep)
         * 4. around>3 alive=>death
        */
        virtual int rules(size_t x,size_t y){
            // int cells=matrix;
            switch(matrix[x][y]/10){
                case 0:
                case 1:
                    return 0;
                case 2:return matrix[x][y]%10;
                case 3:
                    return 1;
                default:
                    return 0;
            }
        }
        int around(size_t x,size_t y)const noexcept{
            if(Size<2)return 0;
            else if(Size==2){
                return (matrix[0][1]+matrix[1][1]+matrix[1][0]+matrix[0][0]-matrix[x][y])%10;
            }
            int deltaX=0,deltaY=0;
            /**
             * type:
             * 0 point
             * 1 TB edge
             * 2 LR edge
            */
            int type=0;
            if(x==0){
                if(y==0){
                    type=0;deltaX=1;deltaY=1;
                }else if(y==Size-1){
                    type=0;deltaX=1;deltaY=-1;
                }else{
                    type=1;deltaX=1;deltaY=1;
                }
            }
            else if(x==Size-1){
                if(y==0){
                    type=0;deltaX=-1;deltaY=1;
                }else if(y==Size-1){
                    type=0;deltaX=-1;deltaY=-1;
                }else{
                    type=1;deltaX=-1;deltaY=-1;
                }
            }else if(y==0){
                // 0<x<Size-1
                type=2;deltaX=1;deltaY=1;
            }else if(y==Size-1){
                // 0<x<Size-1
                type=2;deltaX=1;deltaY=-1;
            }else{
                return (matrix[x][y-1]+matrix[x][y+1]
                +matrix[x-1][y-1]+matrix[x-1][y]+matrix[x-1][y+1]
                +matrix[x+1][y-1]+matrix[x+1][y]+matrix[x+1][y+1])%10;
            }
            if(type==1){
                return (matrix[x][y-deltaY]+matrix[x][y+deltaY]
                    +matrix[x+deltaX][y-deltaY]+matrix[x+deltaX][y]+matrix[x+deltaX][y+deltaY])%10;
            }else if(type==2){
                return (matrix[x-deltaX][y]+matrix[x+deltaX][y]
                    +matrix[x+deltaX][y+deltaY]+matrix[x][y+deltaY]+matrix[x-deltaX][y+deltaY])%10;
            }else{
                // corner
                return (matrix[x][y+deltaY]+matrix[x+deltaX][y+deltaY]+matrix[x+deltaX][y])%10;
            }
        }
        void aroundAll(){
            int temp=0;
            for(size_t i = 0; i < Size; i++){
                for(size_t j=0;j<Size;++j){
                    temp=matrix[i][j];
                    matrix[i][j]=around(i,j)*10+temp%10;
                }
            }
        }
    public:
        PlantLifeGame(size_t r=2048,size_t msGap=200):RGap_Base(r,msGap),matrix(){}
        PlantLifeGame(std::initializer_list<coordinate>const&init,size_t r=2048,size_t msGap=200):RGap_Base(r,msGap),matrix(){
            for(auto&&it:init){
                if(it.x<Size&&it.y<Size){
                    matrix[it.x][it.y]=1;
                }
            }aroundAll();
        }
        ~PlantLifeGame()=default;
        void show(std::ostream&Out=std::cout)const noexcept{
            for(auto&&row:matrix)
                Out<<row<<std::endl;
        }
        void reset(std::vector<coordinate>const&cells){
            reset();
            for(auto&&it:cells){
                matrix[it.x][it.y]=1;
            }aroundAll();
        }
        void reset(){
            for(size_t it=0;it<Size;++it)
                clear(matrix[it]);
        }
        static void clear(std::array<int,Size>&array){
            // #pragma omp parallel
            for (size_t i = 0; i < array.size(); i++){
                array[i]=0;
            }
        }
        void run(){
            run(round,round_gap_ms);
        }
        void run(size_t rc,size_t gap){
            static std::chrono::duration<int, std::milli> mil{gap};
            int Rcount=0;
            while(Rcount<rc){
                std::cout<<"------------"<<std::endl
                    <<"Round : "<<Rcount++<<std::endl
                    <<"------------"<<std::endl;
                this->show();
                int temp=0;
                for(size_t x=0;x<Size;++x){
                    for(size_t y=0;y<Size;++y){
                        // temp=matrix[x][y]/10;
                        matrix[x][y]=rules(x,y);
                    }
                }aroundAll();
                std::this_thread::sleep_for(mil);
            }
        }
};