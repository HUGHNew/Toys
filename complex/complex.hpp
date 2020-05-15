#pragma once
#ifndef _HUGH_COMPLEX_HPP_
#define _HUGH_COMPLEX_HPP_ 1
#include<cmath>
#include<iostream>
#include<string>
#include<exception>
class Except:public std::exception{
    std::string ex;
public:
    Except()noexcept{ex="In the file:";ex+=__FILE__+__LINE__;}
    Except(const Except&Ex)noexcept:ex(Ex.ex){}
    Except(const std::string&Sex)noexcept:ex(Sex){}
    virtual ~Except()noexcept{std::cout<<this->what()<<std::endl;}
    const char* what()noexcept{return ex.c_str();}
};
class Complex{
    long double Real,Vir;
public:
    Complex()noexcept=default;
    Complex(long double R,long double V)noexcept:Real(R),Vir(V){}
    Complex(const Complex&Cx)noexcept:Real(Cx.Real),Vir(Cx.Vir){}
    friend std::ostream& operator<<(std::ostream&,const Complex&)noexcept;
    friend std::istream& operator>>(std::istream&,Complex&);
    friend Complex operator+(long double,const Complex&);
    friend Complex operator-(long double,const Complex&);
    // friend Complex operator-(const Complex&,const Complex&);
    friend Complex operator-(const Complex&);
    template<typename T>
    Complex operator+(T v)noexcept;
    template<typename T>
    Complex operator-(T v)noexcept;
    long double module();
};
Complex operator""i(long double Vir)noexcept{
    return Complex(0,Vir);
}
Complex operator""i(unsigned long long Vir)noexcept{
    return Complex(0,Vir);
}
Complex operator+(long double ld,const Complex&Cx){
    return Complex(Cx.Real+ld,Cx.Vir);
}
Complex operator-(const Complex&Cx){
    return Complex(-Cx.Real,-Cx.Vir);
}
std::ostream& operator<<(std::ostream&os,const Complex&Cx)noexcept{
    bool seq=(Cx.Real!=0);
    if(seq)os<<Cx.Real;
    if(Cx.Vir!=0){
        os<<(Cx.Vir>0?(Cx.Real==0?"":"+"):"")
            <<(Cx.Vir==1?"":(Cx.Vir==-1?"-":std::to_string(Cx.Vir)))<<"i";
        seq=false;
    }else seq=(Cx.Real==0);
    if(seq){
        os<<0;
    }
    return os;
}
std::istream& operator>>(std::istream&is,Complex&Cx){
    std::string cpx;
    is>>cpx;
    if("0"==cpx){Cx.Real=0;Cx.Vir=0;return is;}
    if(cpx[cpx.size()-1]!='i'){
        if(std::string::npos!=cpx.find('.'))
        Cx.Real=std::stold(cpx);
        else Cx.Real=std::stoll(cpx);
        Cx.Vir=0;
    }else{
        int sign=1;
        switch(cpx[0])
        {
        case 'i':
            Cx.Real=0;
            Cx.Vir=1;
            break;
        case '-':// -Vi  -R op Vi
        sign=-1;
        default://s*R op Vi   s*Vi
            bool inc=(sign!=1),in=false;
            const std::string&cpx_sub=cpx.substr(inc,cpx.size()-1-inc);
            if(cpx_sub.size()==0){Cx.Real=0;Cx.Vir=sign;return is;}
            std::size_t pos=cpx_sub.find('-');
            if(std::string::npos==pos)
                pos=cpx_sub.find('+');
            else{//s*R - Vi 
                in=true;
                if(std::string::npos!=cpx_sub.substr(0,pos).find('.'))
                    Cx.Real=sign*std::stold(cpx_sub.substr(0,pos));
                else Cx.Real=sign*std::stoll(cpx_sub.substr(0,pos));
                if(pos==cpx_sub.size()-1)Cx.Vir=-1;
                else if(std::string::npos!=cpx_sub.substr(pos+1).find('.'))
                    Cx.Vir=-std::stold(cpx_sub.substr(pos+1));
                else Cx.Vir=-std::stoll(cpx_sub.substr(pos+1));
            }
            if(std::string::npos==pos){//s*Vi
                Cx.Real=0;
                if(std::string::npos!=cpx_sub.find('.'))
                    Cx.Vir=sign*std::stold(cpx_sub);
                else Cx.Vir=sign*std::stoll(cpx_sub);
            }
            else if(!in){//s*R + Vi
                if(std::string::npos!=cpx_sub.substr(0,pos).find('.'))
                    Cx.Real=sign*std::stold(cpx_sub.substr(0,pos));
                else Cx.Real=sign*std::stoll(cpx_sub.substr(0,pos));
                if(pos==cpx_sub.size()-1)Cx.Vir=1;
                else if(std::string::npos!=cpx_sub.substr(pos+1).find('.'))
                    Cx.Vir=std::stold(cpx_sub.substr(pos+1));
                else Cx.Vir=std::stoll(cpx_sub.substr(pos+1));
            }
            break;
        }
    }
    return is;
}
template<typename T>Complex Complex::operator+(T v)noexcept{
    return Complex(Real+v,Vir);
}
template<>Complex Complex::operator+(const Complex& v)noexcept{
    return Complex(Real+v.Real,Vir+v.Vir);
}
template<>Complex Complex::operator+(Complex v)noexcept{
    return Complex(Real+v.Real,Vir+v.Vir);
}
template<typename T>Complex Complex::operator-(T v)noexcept{
    return Complex(Real-v,Vir);
}
template<>Complex Complex::operator-(const Complex& v)noexcept{
    return Complex(Real-v.Real,Vir-v.Vir);
}
template<>Complex Complex::operator-(Complex v)noexcept{
    return Complex(Real-v.Real,Vir-v.Vir);
}
long double Complex::module(){
    return sqrtl(Real*Real+Vir*Vir);
}
#endif//_HUGH_COMPLEX_HPP_
