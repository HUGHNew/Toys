#include<iostream>
#include<string>
#include<ctime>
#include<stdio.h>
#include<stdlib.h>
#if defined(_WIN32)||defined(_WINDOWS_)
#include<windows.h>
#define PLAT_NAME "win"
void fun2(){
    char User[256]={0};
    DWORD size=256;
    GetUserName(User,&size);
    std::cout<<User<<std::endl;
}
#endif
/*copyright @SiriusChaos
 *
 *
 */
using namespace std;
void numberGuess();
bool sizeJudge(int m,int t);
bool guess(int target,int num);
void out(const string &me){cout<<me<<endl;}
void timeDelay(){auto b=time(0);while(time(0)-b<=2);}
void fun1(){
    //struct tm *local=localTime(0);
    time_t t = time(0);
    char tmp[64];
    strftime( tmp, sizeof(tmp), "%Y/%m/%d %X %A 本年第%j天 %z",localtime(&t) );
    puts( tmp );
}
void CC(int num){
    int t;
    switch(num){
        case 0:
            out("您不会真以为还有隐藏选项吧？");
            break;
        case 1:
            out("位数都分不清了？");
            break;
        case 2:
            out("错误选项又不是负号，不会负负得正的！");
            break;
        case 3:
            out("玩够了没有？");
            break;
        case 4:
            t=3;
            while(t--){
                out("?");
                timeDelay();
            }
            break;
        default:
            out("大侠饶过我吧，能不能别这么玩？");
            break;
    }
}
void code(){cout<<"这么强？！要不代码您来写？https://github.com/SiriusChaos/Toys/blob/master/boring%20game/boring.cpp"<<endl;terminate();}
bool deal(string str){
	static int times=0;
	if(str.size()==1){
		if(str[0]=='3'){times=0;return true;}
		else if(str[0]=='4'){out("All right,goodbye");terminate();return true;}
		else if(str[0]=='5'){times=0;numberGuess();return true;}
		else if(str[0]=='1'){times=0;fun1();return true;}
		else if(str[0]=='2'){times=0;
    #ifdef PLAT_NAME
        fun2();
    #endif
        return true;}
		else if(str[0]=='0')code();
		else{if(times==0)out("有这个选项吗？我怎么不记得了");
		if(times==1)CC(0);else CC(times);
		++times;
		}
	}else if(str.size()==0)out("咋啦？回车敲上瘾都不想选了？");
	else{if(times==0)CC(1);
        else CC(times);
            ++times;}
	return false;
//	for(
}
int input(const string &me){
	int n;
	out(me);
	bool s=scanf("%d",&n);
	if(!s)return 0;
	return n;}
void lEc(){out("敲个回车这么难？还是看完了在练字儿呢？");}
void Enter(int time=0){
	switch(time){
		case 0:
		        out("不认识回车吗？Enter键都不知道？");
		        break;
		case 1:
		        out("敲回车啊！怎么不听呢？！");
		        break;
	    case 2:
		        out("???");
		        break;
		case 3:
		        out("啥意思啊？这都学不会？诚心不想好好玩了？！");
		        break;
		 case 4:
		        out("还来？！");
		        break;
		 case 5:
		        out("行行行，真是惹不起");
		        break;
		 default:
		        cout<<"这是您第"<<time<<"次按错键了"<<endl;
	}
}
int main(){
	cout<<"哟，竟然点开了？！那想做点啥？下面给你几个选项"<<endl;
	while(1){
#ifdef PLAT_NAME
SetConsoleTitle("boring program");
	out("1:看个时间\n2:看看当前用户\n3:NULL\n4:小爷不玩了\n5:猜数游戏\n选一个:");
#else
    out("1:just have a look about the time\n2:to know the user\n3:NULL\n4:exit\n5:guess number game\nChoose one:");
#endif
    auto begin=time(0);
#ifdef PLAT_NAME
	out("先按回车后输入选项，然后再按回车确定");
#else
    out("Enter your choice after press \"Enter\" and then press \"Enter\" to confirm your choice");
#endif
	char enter=0;
	int enTi=-1;
	while((enter=getchar())!='\n'){++enTi;
	cout<<endl;
	//begin=clock();
	Enter(enTi);}
#ifdef PLAT_NAME
	out("您终于知道按回车了");
#else
    out("You finally press \"Enter\"");
#endif
	string c;
	if(time(0)-begin>15)lEc();
	getline(cin,c);
	while(!deal(c)){
#ifdef PLAT_NAME
	out("重新好好输一次？");
#else
    out("input your choice again,and press \"Enter\" to continue");
#endif
        getline(cin,c);
	}
	}return 0;
}
void numberGuess(){
    //out("请输入猜数上限:");
    int maxNum=-1,timeOfGuess=-1,Wt=0;
    while(maxNum){
    maxNum=input("请输入猜数上限:(PS数字都是非负整数，请正确输入)");
    if(!maxNum)out("要玩就好好玩，别调戏程序啊");}
    while(timeOfGuess){
        timeOfGuess=input("请输入猜的次数:");
        if(!timeOfGuess){out("正经点！！");if(Wt){out("还能不能好好玩了？");terminate();}++Wt;}
    }
    if(!sizeJudge(maxNum,timeOfGuess))out("这么怕猜不中？");
    srand(time(0));
    int number=rand(),g=input("来吧，开猜");
    bool get=0;
    while(timeOfGuess--){
        if(guess(number,g)){break;get=1;}
        else cout<<"go on:";
    }
    if(!get)out("看来运气不好？");
}
bool guess(int target,int num){
    if(num-target>10)out("大得离谱");
    else if(num-target>=5&&num-target<=10)out("大得不多了");
    else if(num-target<5&&num-target>-5&&num!=target)out("马上就猜到了");
    else if(num==target){out("Congratulations!!");return true;}
    else if(num-target>=-10&&num-target<=-5)out("小得不多了");
    else if(num-target<-10)out("小得离谱");
    return false;
}
bool sizeJudge(int m,int t){
    int num=1;
    while(t--)num*=2;
    return m>num;
}
