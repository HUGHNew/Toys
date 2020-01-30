#pragma once
#include<iostream>
#include<cstdlib>
#include<ctime>
template<class T>
void swap(T &a,T &b){
    T temp=a;
    a=b;
    b=temp;
}
template<class T>
T *ranArr(T *arr,int size)
{
    srand((unsigned)time(NULL));
    int times=size;
    while(times--){
        swap<T>(arr[size-times-1],arr[rand()%size]);
    }
    return arr;
}
