#ifndef _CLICK_TIME__H__
#define _CLICK_TIME__H__
#include "Time.h"
#define TIMER_FAST_BIT 8
#define TIMER_FAST  (1<<TIMER_FAST_BIT)
#define TIMER_FAST_MASK (TIMER_FAST-1)
#define TIMER_SLOW_BIT 6
#define TIMER_SLOW (1<<TIMER_SLOW_BIT)
#define TIMER_SLOW_MASK (TIMER_SLOW -1 )
#ifndef UINT32
#define UINT32 unsigned int 
#endif
#ifndef UINT64
#define UINT64 unsigned INT64
#endif
#define SLOW_SIZE 4
typedef  void(*TIMERCALL)() ;
using namespace std;
struct TimerEventNode{
	UINT32 _cycleTime;
	UINT32 _exeTime;
	TIMERCALL _func;
	TimerEventNode  *next;
	bool _destroy{false};
};

struct TimerEventList{
	TimerEventNode head;
	TimerEventNode* group;
};

class CycleTimeManage
{
	public:
	
		inline  void PushTimerNode(TimerEventList*list,TimerEventNode* node){
			list->group->next = node;
			list->group = node;
			node->next= nullptr;
		};
		inline void AddTimerNode(TimerEventNode* node)
		{
			UINT32 timeNow = _timer;
			UINT32  exeTime = node->_exeTime;
			//cout<<"exeTime= "<<exeTime<<endl;
			int v1 = exeTime|TIMER_FAST_MASK;
			int v2 = timeNow|TIMER_FAST_MASK;
			if(v1 == v2)
			{
				int index = exeTime&TIMER_FAST_MASK;
				PushTimerNode(&_fast[index],node);
			//	std::cout<<"push fast"<<index<<std::endl;
			}else
			{
				int index =0;
				UINT32 mask = TIMER_FAST<<TIMER_SLOW_BIT;
				for(;index<3;index++)
				{
					if((exeTime | (mask-1) ) == (timeNow|(mask-1))){
						break;
					}
					mask<<=TIMER_SLOW_BIT;
				}
				int bit = (exeTime>>( TIMER_FAST_BIT+ index*TIMER_SLOW_BIT))&TIMER_SLOW_MASK;
				//cout<<"push  slow "<<index <<" bit "<<bit<<" mask = "<<mask <<endl;
				PushTimerNode(&_slow[index][bit],node);
			}
			//cout<<"now"<<_timer<<endl;

		}

		inline bool AddTimerEvent(TIMERCALL func,int time){
			UINT32 timeNow = _timer;
			auto node = new TimerEventNode();
			node->_func =func ;
			node->_cycleTime = time;
			node->_exeTime = time + timeNow;
			AddTimerNode(node);
			return true;
		};


		inline  TimerEventNode* PopTimerNode( TimerEventList*list){
			auto  node = list->head.next;
			list->head.next= nullptr;
			list->group= &(list->head);
			return node;
		}

		inline void Destroy(TimerEventNode* node){
			if(node != nullptr)
			{
				delete(node);
				node = nullptr;
			}
		}

		inline void Run(TimerEventNode* node){

			while(node)
			{
				node->_func();
				auto temp = node;
				node = node->next;
				if(temp->_destroy){
					Destroy(temp);
					node = nullptr;
				}
				else
				{
					temp->_exeTime = _timer+temp->_cycleTime;
				}

			}

		}

		inline void Execute(){
			int  index  = _timer&TIMER_FAST_MASK;
			if(_fast[index].head.next !=nullptr)
			{
				auto node =  PopTimerNode(&_fast[index]);
				Run(node);
				if(node != nullptr){
					AddTimerNode(node);
				}
			}
		}

		inline void TimerMove(int lv,int idx){
			auto current = PopTimerNode(&_slow[lv][idx]);
			while(current){
				auto  temp = current->next;
				AddTimerNode(current);
				current  = temp;
			}

		};

		inline void  TimerLift(){
			int mask  = TIMER_FAST;
			UINT32 now = ++_timer;
			if(now == 0){
				TimerMove(3,0);
			}
			else{
				UINT32 timer = now >>TIMER_FAST_BIT;
				int index =0;
				while((now&(mask-1))==0){
					int idx = timer & TIMER_SLOW_MASK;
					if(idx != 0){
						//cout<<"timerLift = "<<now<<"index= "<<index<<" idx= "<<idx<<endl;
						TimerMove(index,idx);
						break;
					}
					mask <<=  TIMER_SLOW_BIT;
					timer >>= TIMER_SLOW_BIT;
					++index;
				}
			}
		}



		inline void TickUpdate(){
			Execute();
			TimerLift();
			Execute();
		};

		inline void Update(){
			_clockTime.RefreshTimer();
			INT64 now = _clockTime.GetMsec();
			if(now<=_lastTime)
			{
				_lastTime = now;
			}
			else {
				UINT32 diff = now-_lastTime;
				_lastTime = now;
				for(int i = 0;i<diff;i++)
				{
					TickUpdate();
				}
			}
		};


		void Init(){
			for(int i = 0;i<TIMER_FAST;i++){
				PopTimerNode(&_fast[i]);
			}
			for(int i =0;i<4;i++)
			{
				for(int j = 0;j<TIMER_SLOW;j++)
				{
					PopTimerNode(&_slow[i][j]);	
				}
			}

			_clockTime.RefreshTimer();
			_lastTime = _clockTime.GetMsec();



		};

	private:
		INT64 _lastTime{0};
		int  _lastUpdateTime{0};
		TimerEventList _fast[TIMER_FAST];
		TimerEventList _slow[SLOW_SIZE][TIMER_SLOW];
		UINT32 _timer{0};
		ClockTime _clockTime;
};

#endif
