#pragma once
#ifndef ELEVATOR_H_  
#define ELEVATOR_H_  
#include "iostream"
#include <windows.h>
#define MAX 100    //假设本楼最多100人
#include <list>
#include <algorithm>

#define Max_num 16

using namespace std;
/*
电梯一共有七个状态，即正在开门（Opening）、已开门（Opened）、正在关门（Closing）、
已关门（Closed）、等待（Waiting）、移动（Moving）、减速（Decelerate）。
加上一个加速，并将移动分成两部分
*/

extern int count1 = 1;

enum Status {
	Opening = 1, Opend, Closing, Closed, Waiting, Down_Moving,Up_Moving, Decelerate, Accelerate
};

enum Status_Man {
	Waiting_peo = 0, Non_Waiting,NON_PEOPLE
};

/**
*   定义一个乘客的所有信息
*/

class Man {
public:
	int num;
	int from;
	int to;            //去哪里
	int Time;          //等待时间
	Status_Man Status;  //只有等待和不等待
};

/**
*   自定义的一个模拟电梯
*/

class Elevator {
public:
	list<Man> Pass;  //还没有坐电梯的
	list<Man> now_Pass;
	int time;
	int now_Floor;
	int next_Floor;
	Status now_Status;
	int before;  // 1:之前是向上的  0:之前向下

	int parm_Floor;   
	int now_Status_Time; //当前状态已花时间
	
	Elevator();
	void Random_Man();
	void Time_();

	void entryElevator();
	void leaveElevator();
	int canBackToOne();
	
	void manLeave();
	int isPause();
	void Pause();
	int pre_Moving();
	int canClose();
	void Close();
	int UpStairs();
	int DownStairs();
};

Elevator::Elevator() {
	time = 0;
	now_Floor = 1;
	next_Floor = 1;
	now_Status = Waiting;
	now_Status_Time = 0;
	parm_Floor = 1;
	before = 1;
}

void Elevator::Time_() {
	now_Status_Time++;
	time++;
	Sleep(100);   //1t
	Random_Man();
	manLeave();
	pre_Moving();
	
	if (isPause()) {            //判断是否开门
		Pause();
	}

	if (canClose()) {           //判断是否关门
		Close();
	}
	if (canBackToOne()) {
		next_Floor=1;
		cout << "等待时间超300t,现在回一楼\n";
	}
	
	//if (!Pass.empty())
		//cout << Pass.size();
	//for_each(Pass.begin(), Pass.end(), add_Time);


}
// 删除 Pass.erase(iter);


void Elevator::Pause() {
	cout << "1";
	now_Status_Time = 0;
	now_Status = Opening;
	cout << "电梯在开门";
	cout << "\t\t\t现在时间:" << time << "\n";
	for (int j = 0; j < 20; j++)
	{
		Random_Man();   //开门的时候也随机来人
		Sleep(100);     //开门
		time++;
		now_Status_Time++;
	}

	leaveElevator();    //人离开电梯
	entryElevator();    //人进入电梯
	now_Floor = next_Floor;
	cout << "现在楼层在" << now_Floor;
	cout << "\t\t现在时间:" << time << "\n";
}

void Elevator::Close() {
	cout << "电梯在关门\n";
	cout << "2";
	cout << "\t\t\t\t现在时间:" << time << "\n";
	now_Status = Closing;
	for (int j = 0; j < 20; j++)
	{
		Random_Man();   //关门的时候也随机来人
		Sleep(100);     //关门
		time++;
		now_Status_Time++;
	}
	now_Status = Closed;
	cout << "电梯加速中\n";

	int sk = pre_Moving();
	if (sk == now_Floor)
		now_Status = Waiting;
	else if (sk > now_Floor)
	{
		now_Status = Up_Moving;
	}
	else
		now_Status = Down_Moving;
	now_Status_Time = 0;
}

void Elevator::manLeave() {
	list<Man>::iterator iter = Pass.begin();
	int flag = 0;
	for (; iter != Pass.end(); iter++)
		if ((iter->Time++) > 300)             // 每个人的时间状态+1
		{
			list<Man>::iterator iter0 = Pass.end();
			iter0--;

			cout <<"\t\t\t\t现在时间:"<<time<<	"\t\t\t"<<iter0->num<<"号乘客等待时间过长离开\n";
			
			flag = 1;
			break;
		}
	if (flag == 1)
		Pass.pop_back();  //人离开
}

int Elevator::isPause() {
	if (now_Status == Opend)
		return 0;
	int parm_time;
	if (now_Floor > next_Floor) {
		before = 0;
		// 下降时，每一层需要 61t，减速需要 23t
		parm_time = (now_Floor - next_Floor) * 61 + 23 + 15;
		if ((now_Status_Time - 15 ) % 61 == 0) {
			
			cout << "电梯准备去" << next_Floor << "楼   ";
			cout << "电梯经过" << now_Floor - ((now_Status_Time - 15) / 61) << "楼  ↓";
			cout << "\t现在时间:" << time << "\n";
			parm_Floor = now_Floor - (now_Status_Time - 15) / 61;
		}
	}
	else {
		// 上升时，每一层需要 51t，减速需要 14t
		parm_time = (next_Floor - now_Floor) * 51 + 14 + 15;
		before = 1;
		if ((now_Status_Time - 15) % 51 == 0) {
			//cout << now_Status_Time<<"\n";
			
			cout << "电梯准备去" << next_Floor << "楼   ";
			cout << "电梯经过" << now_Floor + ((now_Status_Time - 15) / 51) << "楼  ↑";
			cout << "\t现在时间:" << time << "\n";
			parm_Floor = now_Floor + (now_Status_Time - 15) / 51;
		}
	}
	//(now_Status == Down_Moving || now_Status == Up_Moving)&&
	if (parm_time == now_Status_Time) {

		cout << "\t\t\t\t现在时间:" << time << "\n";
		cout << "电梯已到" << next_Floor << "楼\n";
		return 1;
	}
	else
		return 0;

}

int Elevator::canBackToOne() {
	if (now_Status == Waiting && (now_Status_Time+1) % 300 == 0) 
		return 1;
	else 
		return 0;

}

int Elevator::canClose() {
	if (now_Status == Opend && (now_Status_Time) % 40 == 0) 
		return 1;
	else
		return 0;
}

void Elevator::entryElevator() {

	list<Man>::iterator iter0 = Pass.begin();
	list<Man>::iterator it = Pass.end();

	Man *dd=(Man *)malloc(sizeof(Man));
	--it;
	for (; iter0 != Pass.end(); )
	{
		if (iter0->from == now_Floor && now_Pass.size()<=Max_num)
		{
			cout << "\t\t\t\t现在时间:" << time ;
			cout << "\t\t" <<iter0->num<<"号乘客从" << iter0->from << "楼进入电梯，想去"<<iter0->to << "楼\n";
			cout << "电梯进人ing\n";
			for (int k = 0; k < 25; k++) {
				Sleep(100);
				Random_Man();
				time++;
				now_Status_Time++;
			}
			
			dd->from = iter0->from;
			dd->to = iter0->to;
			dd->Status = iter0->Status;
			dd->Time = iter0->Time;
			dd->num = iter0->num;
			now_Pass.push_front(*dd);

			iter0= Pass.erase(iter0);
			
		}
		else if (now_Pass.size() > Max_num)
		{
			cout << "电梯容量已满\n";
			break;
		}
		else {
			iter0++;
		}
	}
}

void Elevator::leaveElevator() {
	if (now_Pass.size() == 0)
		return;

	list<Man>::iterator iter0 = now_Pass.begin();

	list<Man>::iterator it = now_Pass.end();
	--it;

	for (; iter0 != now_Pass.end(); iter0++)
	{
		if (iter0->to == now_Floor)
		{
			cout << "\t\t\t\t现在时间:" << time << "\n";
			cout << "\t\t\t\t\t\t" << iter0->num<<"号乘客离开电梯，现在还有"<<now_Pass.size()<<"位乘客\n";

			for (int k = 0; k < 25; k++) {
				Sleep(100);
				Random_Man();
				manLeave();
				pre_Moving();
			}
			if (iter0 == it) {
				now_Pass.pop_back();
				break;
			}
			else
				now_Pass.erase(iter0);
		}
	}
}

int Elevator::pre_Moving() {
	int nex;

	if (before == 1) {
		nex = UpStairs();
		nex = nex < next_Floor ? nex : next_Floor;
		list<Man>::iterator iter1 = Pass.begin();

		for (; iter1 != Pass.end(); iter1++)
		{
			if (iter1->from > parm_Floor && iter1->from <= nex && iter1->from<iter1->to )
				nex = iter1->from;
		}

		if (now_Floor == nex) {
			nex = DownStairs();
			list<Man>::iterator iter = Pass.begin();
			for (; iter != Pass.end(); iter++)
			{
				if (iter->from > iter->to && iter->from >= nex )
					nex = iter->from;
			}
		}
	
		//cout << "2nex" << nex << "         now_FLLOr" << now_Floor << "          parm" << parm_Floor << "\n";
	}
	else {
		nex = DownStairs();  //电梯内乘客所想去的最高楼层
		nex = nex > next_Floor ? nex : next_Floor; //与目的楼层作比较，选取最小值
		list<Man>::iterator iter1 = Pass.begin(); 

		for (; iter1 != Pass.end(); iter1++)   //遍历电梯外乘客
		{
			if (iter1->from < parm_Floor && iter1->from >= nex && iter1->from > iter1->to)
				nex = iter1->from;
		}

		if (now_Floor == nex) {
			nex = UpStairs();
			list<Man>::iterator iter = Pass.begin();
			for (; iter != Pass.end(); iter++)
			{
				if (iter->from < iter->to && iter->from < nex)
					nex = iter->from;
			}
		}
	}
	/*
	if(nex!=now_Floor)
		cout << "现在去" <<nex<< "楼\n";
	*/
	if (nex > now_Floor)
		now_Status = Up_Moving;
	if (nex < now_Floor)
		now_Status = Down_Moving;
	next_Floor = nex;
	return nex;

}

int Elevator::UpStairs() {
	list<Man>::iterator iter = now_Pass.begin();
	list<Man>::iterator iter0 = Pass.begin();

	//now_Status = Up_Moving;
	//选择下一个楼
	int next_F=now_Floor;
	
	for (; iter != now_Pass.end(); iter++)
	{
		if (iter->to > now_Floor && iter->to <= next_F)
			next_F = iter->to;
	}

	return next_F;
}

int Elevator::DownStairs() {
	list<Man>::iterator iter = now_Pass.begin();
	//list<Man>::iterator iter0 = Pass.begin();
	int next_F=now_Floor;
	//now_Status = Down_Moving;
	//选择下一个楼
	if (now_Pass.size()!=0)
		next_F = now_Pass.begin()->to;

	for (; iter != now_Pass.end(); iter++)
	{
		if (iter->to < now_Floor && iter->to >= next_F)
			next_F = iter->to;
	}
	//cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << next_F << "\n";
	return next_F;
}


void Elevator::Random_Man() {
	int N =rand() % 100;   //平均每100t生成一个人
	
	//printf("\n随机数为%d\n", N);
	if (!N)
	{
		cout << now_Status<<"****\n";
		if (now_Status == Waiting) {
			cout << "电梯开始加速\n";
			now_Status_Time = 0;
		}
		Man add_Man;
		int floor_from = 1+(rand() % 9);
		int floor_to = 1 + (rand() % 9);
		if (floor_from == floor_to)
			return;
		add_Man.from = floor_from;
		add_Man.to = floor_to;
		add_Man.Time = 0;
		add_Man.Status = Waiting_peo;
		add_Man.num = count1++;
		cout << "\t\t\t\t现在时间:" << time ;
		
		cout << "\t\t" << "乘客"<< add_Man.num <<"想做电梯，来自第" << floor_from <<"楼，去第"<< floor_to<<"楼\n";
		Pass.push_front(add_Man);
		manLeave();
		pre_Moving();
	}
}

#endif  
