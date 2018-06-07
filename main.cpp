#include<iostream>
#include<set>
#include<string>
#include <tuple>
#include <iostream>
#include <string>
#include <stdexcept>
#include "ClickTime.h"
using namespace std;
struct Data{
	std::string name;
	bool operator<(const Data & d1)const {
		return name<d1.name;
	}
	int m ;
	//bool operator()(const Data& d1)const{
	//	return name == d1.name;
	//}
};
void Print(){
	ClockTime ct;
	cout<<"ff "<<ct.GetMsec()<<endl;


}
void TimeTest(){

	srandom(::time(NULL));
	CycleTimeManage cm;
	cm.Init();
	int i = 1000;
	while(i--)
	{
		//int time  = random()%1000;
		//cm.AddTimerEvent(Print,time);
	}

	cm.AddTimerEvent(Print,1000);

	while(true)
	{
		cm.Update();
		usleep(100000);
	
	}
	
}

void array1(){
	char data[1000000];
	unsigned int sum = 0;
	for(int i = 0;i<1000000;++i){
		sum +=data[i];
	}
	cout<<sum<<endl;
}

void array2(){
	unsigned int sum = 0;
	int a = 6;
	while(--a)
	{
	char data[1600000];
	for(int i=0;i<1600000;i+=16)
	{
		sum += data[i];
	}
	}
	cout<<sum<<endl;
}

void ArrayTest()
{
	ClockTime ct;
	int64_t time1 = ct.GetMsec();
	array1();
	ct.RefreshTimer();
	int64_t time2 = ct.GetMsec();
	cout<<time2-time1<<endl;
	array2();
	ct.RefreshTimer();
	int64_t time3 = ct.GetMsec();
	cout<<time3-time2<<endl;
}




int main(){

	TimeTest();

//	ArrayTest();




	//tuple<int,set<int>> a1;
	//set<int> ff;
	//a1 = std::make_tuple(10,ff);




//	int a = 10;
//	int b = 0;
//
//	float x = a/b*1.0f;
//	cout<<x<<endl;
//


	//set<Data> playerName;
	//Data d1;
	//d1.name = "a1";		
	//playerName.insert(d1);

	//d1.name = "a2";		
	//playerName.insert(d1);
	//d1.name = "a4";		
	//playerName.insert(d1);
	//
	//d1.name = "a0";
	//set<Data>::iterator it =playerName.find(d1); 
	//if(it != playerName.end())
	//{
	// cout<<it->name.c_str()<<endl;
	//}





	return 0;
}
