#pragma once

class RGap_Base{
    protected:
        unsigned long long round;
        int round_gap_ms;
    public:
        RGap_Base(unsigned long long r=2048,unsigned long long msGap=200):round(r),round_gap_ms(msGap){}
        virtual ~RGap_Base()=default;
};