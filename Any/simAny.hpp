#ifndef _SIMANY_HPP
#define _SIMANY_HPP 1
#include<iostream>
#include<memory>
#include<typeindex>
#include<type_traits>
namespace Tool{
    class SimAny{
        protected:
            struct Base;
            using UBPtr=std::unique_ptr<Base>;
            struct Base{
                virtual ~Base(){}
                virtual UBPtr clone() const noexcept = 0;
                // virtual bool operator==(Base const&)const noexcept=0;
            };
            template<typename D>
                struct Derived:Base{
                    D val;
                    template<typename U>
                        Derived(U &&v):val(std::forward<U>(v)){}
                    UBPtr clone()const noexcept{
                        return UBPtr(new Derived(this->val));
                    }
                    bool operator==(Derived const&der)const noexcept{return val==der.val;}
                    template<class U>
                        bool operator==(Derived<U> const&){return false;}

                };
            std::type_index info;
            UBPtr val;

            UBPtr clone()const{
                if(val)
                    return val->clone();
                return nullptr;
            }
        public:
            SimAny():info(std::type_index(typeid(void))){}
        template<typename T>
            SimAny(T &&v):val(new Derived<typename std::decay<T>::type>(std::forward<T>(v))),
                        info(std::type_index(typeid(typename std::decay<T>::type))){}
        template<typename T>
            SimAny(SimAny const&any):val(*any.val),info(any.info){}
        template<typename T>
            bool is_same()const{
                return std::type_index(typeid(T))==this->info;
            }
        template<typename T>
            T& any_cast()const{
                if(is_same<T>())
                    return dynamic_cast<Derived<T>*>(val.get())->val;
                throw std::bad_cast();
            }
        // template<typename T>
        //     SimAny& operator=(T const&value){
        //         this->val.reset(new Derived<typename std::decay<T>::type>(value));
        //         this->info=typeid(typename std::decay<T>::type(value));
        //         return *this;
        //     }

        // template<typename T>
            SimAny& operator=(SimAny const&any){
                if(this->val!=any.val){
                    this->val=any.clone();
                    this->info=any.info;
                }
                return *this;
            }
            // bool operator==(SimAny const&any){
            //     return this->info==any.info && *val.get() == *any.val.get();
            // }
    };
}
#endif//SIMANY_HPP
