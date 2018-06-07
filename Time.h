#ifndef __TIME_H__
#define __TIME_H__
#include "Define.h"
#include<time.h>
class ClockTime final{
        public:
            ClockTime(){
                clock_gettime(CLOCK_REALTIME,&_clockTime);    
            }   

            /** 
             * @brief 刷新时间
             */
            void RefreshTimer(){
                clock_gettime(CLOCK_REALTIME,&_clockTime);
            }   

            /** 
             * @brief 获得秒数
             *
             * @return 
             */
            DWORD GetSec()const{
                return _clockTime.tv_sec;
            }   

            /** 
             * @brief 获得毫秒数
             *
             * @return 
             */
            QWORD GetMsec()const{
                return _clockTime.tv_sec *1000LL + _clockTime.tv_nsec/1000000LL;
            }   

        private:
            timespec        _clockTime; 
};
#endif
