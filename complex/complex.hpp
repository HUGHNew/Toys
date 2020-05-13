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
// Complex operator-(const Complex&Cpx,const Complex&Cx){
//     return Complex(Cpx.Real-Cx.Real,Cpx.Real-Cx.Vir);
// }
Complex operator-(const Complex&Cx){
    return Complex(-Cx.Real,-Cx.Vir);
}
std::ostream& operator<<(std::ostream&os,const Complex&Cx)noexcept{
    bool seq=(Cx.Real!=0);
    if(seq)os<<Cx.Real;
    if(Cx.Vir!=0){
        os<<(Cx.Vir>0?"+":"")<<Cx.Vir<<"i";
        seq=false;
    }else seq=true;
    if(seq){
        os<<0;
    }
    return os;
}
std::istream& operator>>(std::istream&is,Complex&Cx){
    std::string cpx;
    is>>cpx;
    bool Finish[4]{false};
    switch(cpx[0]){
        case '+':Cx.Real=1;break;
        case '-':Cx.Real=-1;break;
        case 'i':Cx.Real=0;Cx.Vir=1;break;
        default:
            if(isdigit(cpx[0])){
                Cx.Real=cpx[0]-'0';
            }else{
                throw Except("Unexpected charactor");
            }
    }
    const std::string&cpx_sub=cpx.substr(1);
    if(cpx_sub[cpx_sub.size()-1]!='i'){
        Cx.Real=Cx.Real>0?std::stold(cpx_sub):-std::stold(cpx_sub);
        Cx.Vir=0;
        return is;
    }
    if(cpx_sub.find('+')!=std::string::npos){
        size_t pos=cpx_sub.find('+');
        const std::string&RP=cpx_sub.substr(0,pos);
        Cx.Real=Cx.Real>0?std::stold(RP):-std::stold(RP);
        const std::string&VP=cpx_sub.substr(pos,cpx_sub.size()-pos-1);
        Cx.Vir=std::stold(VP);
    }
    else if(cpx_sub.find('-')!=std::string::npos){
        size_t pos=cpx_sub.find('-');
        const std::string&RP=cpx_sub.substr(0,pos);
        Cx.Real=Cx.Real>0?std::stold(RP):-std::stold(RP);
        const std::string&VP=cpx_sub.substr(pos,cpx_sub.size()-pos-1);
        Cx.Vir=-std::stold(VP);
    }else{
        if(cpx_sub.find('i')!=std::string::npos){
            Cx.Vir=Cx.Real;
            Cx.Real=0;
        }
    }
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
