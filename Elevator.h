#pragma once
#ifndef ELEVATOR_H_  
#define ELEVATOR_H_  
#include "iostream"
#include <windows.h>
#define MAX 100    //���豾¥���100��
#include <list>
#include <algorithm>

#define Max_num 16

using namespace std;
/*
����һ�����߸�״̬�������ڿ��ţ�Opening�����ѿ��ţ�Opened�������ڹ��ţ�Closing����
�ѹ��ţ�Closed�����ȴ���Waiting�����ƶ���Moving�������٣�Decelerate����
����һ�����٣������ƶ��ֳ�������
*/

extern int count1 = 1;

enum Status {
	Opening = 1, Opend, Closing, Closed, Waiting, Down_Moving,Up_Moving, Decelerate, Accelerate
};

enum Status_Man {
	Waiting_peo = 0, Non_Waiting,NON_PEOPLE
};

/**
*   ����һ���˿͵�������Ϣ
*/

class Man {
public:
	int num;
	int from;
	int to;            //ȥ����
	int Time;          //�ȴ�ʱ��
	Status_Man Status;  //ֻ�еȴ��Ͳ��ȴ�
};

/**
*   �Զ����һ��ģ�����
*/

class Elevator {
public:
	list<Man> Pass;  //��û�������ݵ�
	list<Man> now_Pass;
	int time;
	int now_Floor;
	int next_Floor;
	Status now_Status;
	int before;  // 1:֮ǰ�����ϵ�  0:֮ǰ����

	int parm_Floor;   
	int now_Status_Time; //��ǰ״̬�ѻ�ʱ��
	
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
	
	if (isPause()) {            //�ж��Ƿ���
		Pause();
	}

	if (canClose()) {           //�ж��Ƿ����
		Close();
	}
	if (canBackToOne()) {
		next_Floor=1;
		cout << "�ȴ�ʱ�䳬300t,���ڻ�һ¥\n";
	}
	
	//if (!Pass.empty())
		//cout << Pass.size();
	//for_each(Pass.begin(), Pass.end(), add_Time);


}
// ɾ�� Pass.erase(iter);


void Elevator::Pause() {
	cout << "1";
	now_Status_Time = 0;
	now_Status = Opening;
	cout << "�����ڿ���";
	cout << "\t\t\t����ʱ��:" << time << "\n";
	for (int j = 0; j < 20; j++)
	{
		Random_Man();   //���ŵ�ʱ��Ҳ�������
		Sleep(100);     //����
		time++;
		now_Status_Time++;
	}

	leaveElevator();    //���뿪����
	entryElevator();    //�˽������
	now_Floor = next_Floor;
	cout << "����¥����" << now_Floor;
	cout << "\t\t����ʱ��:" << time << "\n";
}

void Elevator::Close() {
	cout << "�����ڹ���\n";
	cout << "2";
	cout << "\t\t\t\t����ʱ��:" << time << "\n";
	now_Status = Closing;
	for (int j = 0; j < 20; j++)
	{
		Random_Man();   //���ŵ�ʱ��Ҳ�������
		Sleep(100);     //����
		time++;
		now_Status_Time++;
	}
	now_Status = Closed;
	cout << "���ݼ�����\n";

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
		if ((iter->Time++) > 300)             // ÿ���˵�ʱ��״̬+1
		{
			list<Man>::iterator iter0 = Pass.end();
			iter0--;

			cout <<"\t\t\t\t����ʱ��:"<<time<<	"\t\t\t"<<iter0->num<<"�ų˿͵ȴ�ʱ������뿪\n";
			
			flag = 1;
			break;
		}
	if (flag == 1)
		Pass.pop_back();  //���뿪
}

int Elevator::isPause() {
	if (now_Status == Opend)
		return 0;
	int parm_time;
	if (now_Floor > next_Floor) {
		before = 0;
		// �½�ʱ��ÿһ����Ҫ 61t��������Ҫ 23t
		parm_time = (now_Floor - next_Floor) * 61 + 23 + 15;
		if ((now_Status_Time - 15 ) % 61 == 0) {
			
			cout << "����׼��ȥ" << next_Floor << "¥   ";
			cout << "���ݾ���" << now_Floor - ((now_Status_Time - 15) / 61) << "¥  ��";
			cout << "\t����ʱ��:" << time << "\n";
			parm_Floor = now_Floor - (now_Status_Time - 15) / 61;
		}
	}
	else {
		// ����ʱ��ÿһ����Ҫ 51t��������Ҫ 14t
		parm_time = (next_Floor - now_Floor) * 51 + 14 + 15;
		before = 1;
		if ((now_Status_Time - 15) % 51 == 0) {
			//cout << now_Status_Time<<"\n";
			
			cout << "����׼��ȥ" << next_Floor << "¥   ";
			cout << "���ݾ���" << now_Floor + ((now_Status_Time - 15) / 51) << "¥  ��";
			cout << "\t����ʱ��:" << time << "\n";
			parm_Floor = now_Floor + (now_Status_Time - 15) / 51;
		}
	}
	//(now_Status == Down_Moving || now_Status == Up_Moving)&&
	if (parm_time == now_Status_Time) {

		cout << "\t\t\t\t����ʱ��:" << time << "\n";
		cout << "�����ѵ�" << next_Floor << "¥\n";
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
			cout << "\t\t\t\t����ʱ��:" << time ;
			cout << "\t\t" <<iter0->num<<"�ų˿ʹ�" << iter0->from << "¥������ݣ���ȥ"<<iter0->to << "¥\n";
			cout << "���ݽ���ing\n";
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
			cout << "������������\n";
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
			cout << "\t\t\t\t����ʱ��:" << time << "\n";
			cout << "\t\t\t\t\t\t" << iter0->num<<"�ų˿��뿪���ݣ����ڻ���"<<now_Pass.size()<<"λ�˿�\n";

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
		nex = DownStairs();  //�����ڳ˿�����ȥ�����¥��
		nex = nex > next_Floor ? nex : next_Floor; //��Ŀ��¥�����Ƚϣ�ѡȡ��Сֵ
		list<Man>::iterator iter1 = Pass.begin(); 

		for (; iter1 != Pass.end(); iter1++)   //����������˿�
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
		cout << "����ȥ" <<nex<< "¥\n";
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
	//ѡ����һ��¥
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
	//ѡ����һ��¥
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
	int N =rand() % 100;   //ƽ��ÿ100t����һ����
	
	//printf("\n�����Ϊ%d\n", N);
	if (!N)
	{
		cout << now_Status<<"****\n";
		if (now_Status == Waiting) {
			cout << "���ݿ�ʼ����\n";
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
		cout << "\t\t\t\t����ʱ��:" << time ;
		
		cout << "\t\t" << "�˿�"<< add_Man.num <<"�������ݣ����Ե�" << floor_from <<"¥��ȥ��"<< floor_to<<"¥\n";
		Pass.push_front(add_Man);
		manLeave();
		pre_Moving();
	}
}

#endif  
