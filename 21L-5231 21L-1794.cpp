/*21L-5231
//Qasim Naveed
//
//21L-1794
//Laraib Akhtar
//BCS-4B*/


#include <iostream>
//#include <semaphore.h>
//#include <pthread.h>
//#include <unistd.h>
#include <string.h>
#include <queue>
#include <list>
#include <algorithm>
#include <iterator>
#include <vector>

using namespace std;

//sem_t* forks;
//struct Philosopher
//{
//	int id;
//	int size;
//};
//
//void* func(void* param)
//{
//	Philosopher* ph = (Philosopher*)param;
//	int philosopher = ph->id;
//	int n = ph->size;
//	if (philosopher == n - 1)
//	{
//		sem_wait(&forks[(philosopher + 1) % n]);
//		printf("Philosopher %d picks right fork(%d)\n", philosopher, (philosopher + 1) % n);
//		sem_wait(&forks[philosopher]);
//		printf("Philosopher %d picks left fork(%d)\n", philosopher, philosopher);
//	}
//	else
//	{
//		sem_wait(&forks[philosopher]);
//		printf("Philosopher %d picks left fork(%d)\n", philosopher, philosopher);
//		sem_wait(&forks[(philosopher + 1) % n]);
//		printf("Philosopher %d picks right fork(%d)\n", philosopher, (philosopher + 1) % n);
//	}
//
//	printf("\nPhilosopher %d eats\n\n", philosopher);
//	sleep(5);
//	printf("\nPhilosopher %d has finished eating\n\n", philosopher);
//
//	sem_post(&forks[(philosopher + 1) % n]);
//	printf("Philosopher %d puts right fork(%d)\n", philosopher, (philosopher + 1) % n);
//	sem_post(&forks[philosopher]);
//	printf("Philosopher %d puts left fork(%d)\n", philosopher, philosopher);
//
//	pthread_exit(NULL);
//}
//
//void semaphoreMain()
//{
//	struct Philosopher* ph;
//	int n = -1;
//	while (n)
//	{
//		do
//		{
//			cout << "================================\n";
//			cout << "Enter the Number of Philosophers:\n";
//			cout << "Enter 0 to return to Main Menu\n";
//			cout << "Enter Choice: ";
//			cin >> n;
//			cout << "================================\n";
//		} while (n == 1);
//		if (!n)
//			return;
//
//		ph = new Philosopher[n];
//		pthread_t* t = new pthread_t[n];
//		forks = new sem_t[n];
//
//		for (int i = 0; i < n; i++)
//			sem_init(&forks[i], 0, 1);
//
//		for (int i = 0; i < n; i++)
//		{
//			ph[i].id = i;
//			ph[i].size = n;
//			pthread_create(&t[i], NULL, &func, (void*)&ph[i]);
//		}
//		for (int i = 0; i < n; i++)
//			pthread_join(t[i], NULL);
//
//		delete[] ph;
//		delete[] t;
//
//		for (int i = 0; i < n; i++)
//			sem_destroy(&forks[i]);
//		delete[] forks;
//	}
//}

class Processes
{
	int** need;
	int** max;
	int** allocation;
	int* available;
	int* finish;
	int* work;
	int* resources;
	int res;
	int pro;
	int deadlocks;
	int remaining;
	int* request;
	queue<int> q;

	void print2D(int**& arr, string s)
	{
		cout << "\n==========" << s << "==========\n";

		for (int i = 0; i < pro; i++)
		{
			for (int j = 0; j < res; j++)
				cout << arr[i][j] << "\t";
			cout << endl;
		}
	}
	void print1D(int*& arr, string s)
	{
		cout << "\n==========" << s << "==========\n";
		for (int i = 0; i < res; i++)
			cout << arr[i] << " ";
		cout << endl;
	}
	int safety(int i)
	{
		int count = 0;
		for (int j = 0; j < res; j++)
			if (need[i][j] <= work[j])
				count++;

		if (count == res)
		{
			for (int j = 0; j < res; j++)
				work[j] += allocation[i][j];
			for (int i = 0; i < res; i++)
				cout << work[i] << " ";
			cout << endl;
			deadlocks = 0;
			remaining--;
			return 1;
		}

		deadlocks++;
		return 0;
	}
	int step2()
	{
		bool flag = true;
		for (int i = 0; i < res; i++)
			if (request[i] > work[i])
				flag = false;
		return flag;
	}
	int Request(int i)
	{
		bool flag = true;

		for (int j = 0; j < res; j++)
			if (request[j] > need[i][j])
				flag = false;
		if (!flag)
		{
			cout << "Error";
			return false;
		}
		int count = 0;
		int j = 0;
		deadlocks = 0;
		remaining = pro;
		if (!step2())
		{
			cout << "Wait...\n";
			flag = false;
			while (count < (pro - 1) && deadlocks < remaining && !flag)
			{
				if (!finish[j] && j != i && safety(j))
				{
					finish[j] = 1;
					cout << "P[" << j << "] finishes\n";
					count++;
					q.push(j);
					if (step2())
						flag = true;
				}

				j = (j + 1) % pro;
			}
			cout << endl;
		}
		if (flag)
		{
			int* temp = new int[res];
			for (int j = 0; j < res; j++)
			{
				temp[j] = available[j];
				available[j] = work[j];
			}
			for (int j = 0; j < res; j++)
			{
				available[j] -= request[j];
				work[j] -= request[j];
				allocation[i][j] += request[j];
				need[i][j] -= request[j];
			}

			while (count < pro && deadlocks < remaining)
			{
				if (!finish[j] && safety(j))
				{
					finish[j] = 1;
					cout << "P[" << j << "] finishes\n";
					count++;
					q.push(j);
				}

				j = (j + 1) % pro;
			}
			cout << endl;

			if (count == pro)
			{
				cout << "The Sequence is Safe\n";
				int x;
				while (!q.empty())
				{
					x = q.front();
					q.pop();
					cout << "P[" << x << "] ";
				}
				cout << endl;
				return true;
			}
			else
			{
				cout << "The Sequence is not Safe\n";
				for (int j = 0; j < res; j++)
				{
					available[j] = temp[j];
					allocation[i][j] -= request[j];
					need[i][j] += request[j];
				}
			}
			delete[] temp;
			return false;
		}
		return false;
	}

	void reset()
	{
		for (int i = 0; i < res; i++)
			work[i] = available[i];
		for (int i = 0; i < pro; i++)
			finish[i] = 0;
		while (!q.empty())
		{
			q.pop();
		}
	}

public:
	Processes(int r, int p)
	{
		deadlocks = 0;
		res = r;
		pro = p;
		remaining = p;
		need = new int* [pro];
		max = new int* [pro];
		allocation = new int* [pro];
		finish = new int[pro];
		for (int i = 0; i < pro; i++)
		{
			need[i] = new int[res];
			max[i] = new int[res];
			allocation[i] = new int[res];
			finish[i] = 0;
		}

		available = new int[res];
		work = new int[res];
		resources = new int[res];
		request = new int[res];
		for (int i = 0; i < res; i++)
		{
			available[i] = 0;
			work[i] = 0;
			resources[i] = 0;
		}
	}

	void initialize()
	{
		for (int i = 0; i < res; i++)
		{
			do
			{
				cout << "Enter size of resource " << i << ": ";
				cin >> resources[i];
			} while (resources[i] < 0);
			available[i] = resources[i];
		}

		for (int i = 0; i < pro; i++)
		{
			for (int j = 0; j < res; j++)
			{
				do
				{
					cout << "Enter max of process[" << i << "][" << j << "]: ";
					cin >> max[i][j];
				} while (max[i][j] < 0);
			}
		}

		for (int i = 0; i < pro; i++)
		{
			for (int j = 0; j < res; j++)
			{
				do
				{
					cout << "(Available: " << available[j] << ") ";
					cout << "(Max: " << max[i][j] << ")" << endl;
					cout << "Enter allocation of process[" << i << "][" << j << "]: ";
					cin >> allocation[i][j];
				} while (allocation[i][j] < 0 || allocation[i][j] > available[j] || allocation[i][j] > max[i][j]);
				available[j] -= allocation[i][j];
				work[j] = available[j];
			}
		}

		for (int i = 0; i < pro; i++)
			for (int j = 0; j < res; j++)
				need[i][j] = max[i][j] - allocation[i][j];
	}

	void printAll()
	{
		print2D(max, "Max");
		print2D(need, "Need");
		print2D(allocation, "Allocation");
		print1D(resources, "Resources");
		print1D(available, "Available");
	}

	bool SafetyAlgorithm()
	{
		deadlocks = 0;
		remaining = pro;
		int count = 0;
		int i = 0;
		cout << "Checking if the sequence is safe...\n";
		while (count < pro && deadlocks < remaining)
		{
			if (!finish[i] && safety(i))
			{
				finish[i] = 1;
				cout << "P[" << i << "] finishes\n";
				q.push(i);
				count++;
			}
			i = (i + 1) % pro;
		}
		cout << endl;
		if (count == pro)
		{
			cout << "The Sequence is Safe\n";
			int x;
			while (!q.empty())
			{
				x = q.front();
				q.pop();
				cout << "P[" << x << "] ";
			}
			cout << endl;
			return true;
		}
		else
			cout << "The Sequence is not Safe\n";
		return false;
	}

	void resourceRequestAlgorithm()
	{
		reset();
		for (int i = 0; i < res; i++)
		{
			do
			{
				cout << "Enter request for resource " << i << ": ";
				cin >> request[i];
			} while (request[i] < 0);
		}
		int x;
		do
		{
			cout << "Enter which process wants to make a request: ";
			cin >> x;
		} while (x < 0 && x >= pro);

		if (Request(x))
			cout << "Request granted\n";
		else
			cout << "Request denied\n";
	}

	~Processes()
	{
		delete[] available;
		delete[] finish;
		delete[] work;
		delete[] resources;
		delete[] request;
		for (int i = 0; i < pro; i++)
		{
			delete[] need[i];
			delete[] max[i];
			delete[] allocation[i];
		}
		delete[] need;
		delete[] max;
		delete[] allocation;
	}
};

void deadlockMain()
{
	int res, pro;
	do
	{
		cout << "Enter number of processes(must be greater than 0): ";
		cin >> pro;
	} while (pro <= 0);

	do
	{
		cout << "Enter number of resources(must be greater than 0): ";
		cin >> res;
	} while (res <= 0);

	Processes p(res, pro);

	p.initialize();
	p.printAll();
	int choice = p.SafetyAlgorithm();

	while (choice)
	{
		cout << "Do you want to make a request? Press 0 to exit.. ";
		cin >> choice;
		if (choice)
		{
			p.resourceRequestAlgorithm();
			p.printAll();
		}
	}
}

class cpuprocess
{
public:
	double arrival;
	double burst;
	double pid;
	double wait;
	double finish;
	double remain;
	double	turnaround;
	float feedback;
	double priority;
	double completed;
	int queue;

	cpuprocess(int id, int b, int a, int p = -1, int w = 0, int q = 0)
	{
		pid = id;
		burst = b;
		arrival = a;

		wait = w;
		finish = 0;
		turnaround = 0;
		feedback = (w + b) / b;
		priority = p;
		remain = b;
		completed = 0;
		queue = q;
	}
};

class cpuprocessprior : public cpuprocess
{
public:
	cpuprocessprior(int id, int b, int a, int p = -1, int w = 0) : cpuprocess(id, b, a, p, w) {}


};

bool operator < (const cpuprocessprior& this1, const cpuprocessprior& c)
{
	if (this1.priority < c.priority)
	{
		return true;
	}
	else if (this1.priority == c.priority)
	{
		if (this1.arrival > c.arrival)
		{
			return true;
		}
		else if (this1.arrival == c.arrival)
		{
			if (this1.pid > c.pid)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else {
			return false;
		}
	}
	else
		return false;
}

bool operator > (const cpuprocessprior& this1, const cpuprocessprior& c)
{
	if (this1.priority > c.priority)
	{
		return true;
	}
	else if (this1.priority == c.priority)
	{
		if (this1.arrival < c.arrival)
		{
			return true;
		}
		else if (this1.arrival == c.arrival)
		{
			if (this1.pid < c.pid)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else {
			return false;
		}
	}
	else
		return false;
}

class cpuprocesssrjf : public cpuprocess
{
public:
	cpuprocesssrjf(int id, int b, int a, int p = -1, int w = 0, int q = 0) : cpuprocess(id, b, a, p, w, q) {}


};

bool operator < (const cpuprocesssrjf& this1, const cpuprocesssrjf& c)
{
	if (this1.remain > c.remain)
	{
		return true;
	}
	else if (this1.remain == c.remain)
	{
		if (this1.arrival > c.arrival)
		{
			return true;
		}
		else if (this1.arrival == c.arrival)
		{
			if (this1.pid > c.pid)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else {
			return false;
		}
	}
	else
		return false;
}

bool operator > (const cpuprocesssrjf& this1, const cpuprocesssrjf& c)
{
	if (this1.remain > c.remain)
	{
		return true;
	}
	else if (this1.remain == c.remain)
	{
		if (this1.arrival < c.arrival)
		{
			return true;
		}
		else if (this1.arrival == c.arrival)
		{
			if (this1.pid < c.pid)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else {
			return false;
		}
	}
	else
		return false;
}

void longestjob()
{
	cout << "Enter no of processes: ";

	int n;

	cin >> n;

	vector<cpuprocess> list;
	int* finished = new int[n];

	for (int i = 0; i < n; i++)
	{
		int b, a;

		cout << "Enter burst time of " << i + 1 << " process: ";
		cin >> b;

		cout << "Enter arrival time of " << i + 1 << " process: ";
		cin >> a;

		cpuprocess temp(i + 1, b, a);
		finished[i] = 0;
		list.push_back(temp);
	}


	for (int i = 0; i < n - 1; i++)
	{
		int min = i;
		for (int j = i + 1; j < n; j++)
		{
			if (list[min].arrival > list[j].arrival)
				min = j;
			else if (list[min].arrival == list[j].arrival)
			{
				if (list[min].burst < list[j].burst)
					min = j;
			}
		}

		cpuprocess temp(0, 1, 0);
		temp = list[i];
		list[i] = list[min];
		list[min] = temp;
	}

	int i = 0;
	int j = 0;

	float avgturn = 0, avgfin = 0, avgwait = 0, avgfeed = 0;

	while (i != n)
	{
		if (list[i].arrival <= j)
		{
			int temp = i;
			int max = i;
			int store = i;

			while (temp < n && list[temp].arrival <= j)
			{
				if (list[max].burst < list[temp].burst && finished[temp] == 0)
					max = temp;
				temp++;
			}

			i = max;

			list[i].finish = j + list[i].burst;
			list[i].wait = list[i].finish - list[i].arrival - list[i].burst;
			list[i].turnaround = list[i].finish - list[i].arrival;
			list[i].remain = 0;
			list[i].feedback = (list[i].wait + list[i].burst) / list[i].burst;
			j += list[i].burst;

			avgturn += list[i].turnaround;
			avgfin += list[i].wait;
			avgfeed += list[i].feedback;
			avgwait += list[i].wait;

			cout << "Process " << list[i].pid << " finished\n";
			cout << "Process " << list[i].pid << " finished time is " << list[i].finish << " wait time is " << list[i].wait << " turnaround time is " << list[i].turnaround << " feedback ratio is " << list[i].feedback << endl;
			finished[i] = 1;
			i = store;

			while (i != n && finished[i] == 1)
				i++;

		}
		else
			j++;

	}

	avgturn /= n;
	avgfin /= n;
	avgfeed /= n;
	avgwait /= n;

	cout << "\n avg turnaround time is: " << avgturn << " avg finish time is: " << avgfin << " avg feedback ratio is: " << avgfeed << " avg wait time is: " << avgwait << endl;
}

void lowestfeedbackratio()
{
	cout << "Enter no of processes: ";

	int n;

	cin >> n;

	vector<cpuprocess> list;
	int* finished = new int[n];

	for (int i = 0; i < n; i++)
	{
		int b, a, w = 0;

		cout << "Enter burst time of " << i + 1 << " process: ";
		cin >> b;

		cout << "Enter arrival time of " << i + 1 << " process: ";
		cin >> a;

		cout << "Enter wait time of " << i + 1 << " process: ";
		cin >> w;

		cpuprocess temp(i + 1, b, a, -1, w);

		list.push_back(temp);

		finished[i] = 0;

		cout << "Feedback ratio at start of process " << i + 1 << " is: " << list[i].feedback << endl;
	}

	for (int i = 0; i < n - 1; i++)
	{
		int min = i;
		for (int j = i + 1; j < n; j++)
		{
			if (list[min].arrival > list[j].arrival)
				min = j;
			else if (list[min].arrival == list[j].arrival)
			{
				if (list[min].feedback > list[j].feedback)
					min = j;
			}
		}

		cpuprocess temp(0, 1, 0);
		temp = list[i];
		list[i] = list[min];
		list[min] = temp;
	}

	int i = 0;
	int j = 0;

	float avgturn = 0, avgfin = 0, avgwait = 0, avgfeed = 0;

	while (i != n)
	{
		if (list[i].arrival <= j)
		{

			int temp = i;
			int max = i;
			int store = i;

			while (temp < n && list[temp].arrival <= j && finished[temp] == 0)
			{
				if (list[max].feedback > list[temp].feedback)
					max = temp;
				temp++;
			}

			i = max;

			list[i].finish = j + list[i].burst;
			list[i].wait = list[i].finish - list[i].arrival - list[i].burst;
			list[i].turnaround = list[i].finish - list[i].arrival;
			list[i].remain = 0;
			list[i].feedback = (list[i].wait + list[i].burst) / list[i].burst;
			j += list[i].burst;

			avgturn += list[i].turnaround;
			avgfin += list[i].wait;
			avgfeed += list[i].feedback;
			avgwait += list[i].wait;

			cout << "Process " << list[i].pid << " finished\n";
			cout << "Process " << list[i].pid << " finished time is " << list[i].finish << " wait time is " << list[i].wait << " turnaround time is " << list[i].turnaround << " feedback ratio is " << list[i].feedback << endl;
			finished[i] = 1;
			i = store;

			for (int k = 0; k < n; k++)
			{
				if (finished[k] == 0 && list[k].arrival <= j)
				{
					list[k].wait += j - list[k].arrival;
					list[k].feedback = (list[k].wait + list[k].burst) / list[k].burst;
				}
			}

			while (i != n && finished[i] == 1)
				i++;
		}
		else
			j++;

	}

	avgturn /= n;
	avgfin /= n;
	avgfeed /= n;
	avgwait /= n;

	cout << "\n avg turnaround time is: " << avgturn << " avg finish time is: " << avgfin << " avg feedback ratio is: " << avgfeed << " avg wait time is: " << avgwait << endl;
}

void mlq()
{
	cout << "Enter no. of processes: ";
	int n;
	cin >> n;

	vector<cpuprocessprior> l1;
	vector<cpuprocess> l2;
	vector<cpuprocesssrjf> l3;

	int n1 = 0, n2 = 0, n3 = 0;

	for (int i = 0; i < n; i++)
	{
		cout << "Enter the queue in which u want to put it: ";
		int ch;

		cin >> ch;

		if (ch == 1)
		{
			int b, a, p;

			cout << "Enter burst time of " << i + 1 << " process: ";
			cin >> b;

			cout << "Enter arrival time of " << i + 1 << " process: ";
			cin >> a;

			cout << "Enter priority of " << i + 1 << " process: ";
			cin >> p;

			cpuprocessprior temp(i + 1, b, a, p);

			l1.push_back(temp);
			n1++;
		}
		else if (ch == 2)
		{
			int b, a, p;

			cout << "Enter burst time of " << i + 1 << " process: ";
			cin >> b;

			cout << "Enter arrival time of " << i + 1 << " process: ";
			cin >> a;

			cpuprocess temp(i + 1, b, a);

			l2.push_back(temp);
			n2++;
		}
		else if (ch == 3)
		{
			int b, a, p;

			cout << "Enter burst time of " << i + 1 << " process: ";
			cin >> b;

			cout << "Enter arrival time of " << i + 1 << " process: ";
			cin >> a;

			cpuprocesssrjf temp(i + 1, b, a);

			l3.push_back(temp);
			n3++;
		}
	}

	priority_queue<cpuprocessprior> q1;
	queue<cpuprocess> q2;
	priority_queue<cpuprocesssrjf> q3;

	for (int i = 0; i < n1 - 1; i++)
	{
		int min = i;
		for (int j = i + 1; j < n1; j++)
		{
			if (l1[min].arrival > l1[j].arrival)
				min = j;
			else if (l1[min].arrival == l1[j].arrival)
			{
				if (l1[min].priority < l1[j].priority)
					min = j;
			}
		}

		cpuprocessprior temp(0, 1, 0);
		temp = l1[i];
		l1[i] = l1[min];
		l1[min] = temp;

	}

	for (int i = 0; i < n2 - 1; i++)
	{
		int min = i;
		for (int j = i + 1; j < n2; j++)
		{
			if (l2[min].arrival > l2[j].arrival)
				min = j;
			else if (l2[min].arrival == l2[j].arrival)
			{
				if (l2[min].burst < l2[j].burst)
					min = j;
			}
		}

		cpuprocess temp(0, 1, 0);
		temp = l2[i];
		l2[i] = l2[min];
		l2[min] = temp;


	}

	for (int i = 0; i < n3 - 1; i++)
	{
		int min = i;
		for (int j = i + 1; j < n3; j++)
		{
			if (l3[min].arrival > l3[j].arrival)
				min = j;
			else if (l3[min].arrival == l3[j].arrival)
			{
				if (l3[min].burst < l3[j].burst)
					min = j;
			}
		}

		cpuprocesssrjf temp(0, 1, 0);
		temp = l3[i];
		l3[i] = l3[min];
		l3[min] = temp;


	}

	int total = 0;
	int j = 0;
	cpuprocess current(0, 1, 0, 0, 0);

	int c1 = 0, c2 = 0, c3 = 0;
	int x = 0, y = 0, z = 0;
	float avgturn = 0, avgfin = 0, avgwait = 0, avgfeed = 0;
	int slice = 5;
	int q3e = 1;
	while (total != n)
	{
		while (x != n1 && l1[x].arrival <= j)
		{
			q1.push(l1[x]);
			x++;
			c1 = 1;
		}
		while (y != n2 && l2[y].arrival <= j)
		{
			q2.push(l2[y]);
			y++;
		}
		while (z != n3 && l3[z].arrival <= j)
		{
			q3.push(l3[z]);
			z++;
			q3e = 1;
		}

		if (q1.empty())
		{
			c1 = 0;
		}

		if (!q1.empty() && q1.top().arrival <= j)
		{
			if (c2 == 1)
			{
				current.remain -= 1;
				c2 = 0;
				slice = 5;

				if (current.remain != 0)
					q2.push(current);
				else
				{
					current.finish = j;
					current.wait = current.finish - current.arrival - current.burst;
					current.turnaround = current.finish - current.arrival;
					current.remain = 0;
					current.feedback = (current.wait + current.burst) / current.burst;

					avgturn += current.turnaround;
					avgfin += current.wait;
					avgfeed += current.feedback;
					avgwait += current.wait;
					total++;
					cout << "Process " << current.pid << " finished\n";
					cout << "Process " << current.pid << " finished time is " << current.finish << " wait time is " << current.wait << " turnaround time is " << current.turnaround << " feedback ratio is " << current.feedback << endl;
				}
			}
			if (c3 == 1)
			{
				current.remain -= 1;
				c3 = 0;
				if (current.remain != 0)
				{
					cpuprocesssrjf w(0, 1, 0, 0, 0);
					w.arrival = current.arrival;
					w.burst = current.burst;
					w.feedback = current.feedback;
					w.finish = current.finish;
					w.completed = current.completed;
					w.pid = current.pid;
					w.priority = current.priority;
					w.wait = current.wait;
					w.turnaround = current.turnaround;
					w.remain = current.remain;
					q3.push(w);

				}
				else
				{
					current.finish = j;
					current.wait = current.finish - current.arrival - current.burst;
					current.turnaround = current.finish - current.arrival;
					current.remain = 0;
					current.feedback = (current.wait + current.burst) / current.burst;
					total++;

					avgturn += current.turnaround;
					avgfin += current.wait;
					avgfeed += current.feedback;
					avgwait += current.wait;

					cout << "Process " << current.pid << " finished\n";
					cout << "Process " << current.pid << " finished time is " << current.finish << " wait time is " << current.wait << " turnaround time is " << current.turnaround << " feedback ratio is " << current.feedback << endl;
				}
			}

			cpuprocessprior d = q1.top();
			q1.pop();

			d.finish = j + d.burst;
			d.wait = d.finish - d.arrival - d.burst;
			d.turnaround = d.finish - d.arrival;
			d.remain = 0;
			d.feedback = (d.wait + d.burst) / d.burst;
			j += d.burst;

			avgturn += d.turnaround;
			avgfin += d.wait;
			avgfeed += d.feedback;
			avgwait += d.wait;

			cout << "Process " << d.pid << " finished\n";
			cout << "Process " << d.pid << " finished time is " << d.finish << " wait time is " << d.wait << " turnaround time is " << d.turnaround << " feedback ratio is " << d.feedback << endl;

			total++;
		}

		while (x != n1 && l1[x].arrival <= j)
		{
			q1.push(l1[x]);
			x++;
			c1 = 1;
		}
		while (y != n2 && l2[y].arrival <= j)
		{
			q2.push(l2[y]);
			y++;
		}
		while (z != n3 && l3[z].arrival <= j)
		{
			q3.push(l3[z]);
			z++;
			q3e = 1;
		}
		if (q1.empty())
		{
			c1 = 0;
		}
		if (c1 == 0 && ((c2 == 1) || (!q2.empty() && q2.front().arrival <= j)))
		{
			if (c3 == 1)
			{
				current.remain -= 1;
				c3 = 0;
				if (current.remain != 0)
				{
					cpuprocesssrjf w(0, 1, 0, 0, 0);
					w.arrival = current.arrival;
					w.burst = current.burst;
					w.feedback = current.feedback;
					w.finish = current.finish;
					w.completed = current.completed;
					w.pid = current.pid;
					w.priority = current.priority;
					w.wait = current.wait;
					w.turnaround = current.turnaround;
					w.remain = current.remain;
					q3.push(w);

				}
				else
				{
					current.finish = j;
					current.wait = current.finish - current.arrival - current.burst;
					current.turnaround = current.finish - current.arrival;
					current.remain = 0;
					current.feedback = (current.wait + current.burst) / current.burst;
					total++;

					avgturn += current.turnaround;
					avgfin += current.wait;
					avgfeed += current.feedback;
					avgwait += current.wait;

					cout << "Process " << current.pid << " finished\n";
					cout << "Process " << current.pid << " finished time is " << current.finish << " wait time is " << current.wait << " turnaround time is " << current.turnaround << " feedback ratio is " << current.feedback << endl;
				}
			}
			if (c2 == 0)
			{
				c2 = 1;
				current = q2.front();
				q2.pop();
				j++;
			}
			else
			{
				slice--;
				current.remain -= 1;

				if (current.remain == 0)
				{
					current.finish = j;
					current.wait = current.finish - current.arrival - current.burst;
					current.turnaround = current.finish - current.arrival;
					current.remain = 0;
					current.feedback = (current.wait + current.burst) / current.burst;
					c2 = 0;
					avgturn += current.turnaround;
					avgfin += current.wait;
					avgfeed += current.feedback;
					avgwait += current.wait;
					total++;
					cout << "Process " << current.pid << " finished\n";
					cout << "Process " << current.pid << " finished time is " << current.finish << " wait time is " << current.wait << " turnaround time is " << current.turnaround << " feedback ratio is " << current.feedback << endl;


					slice = 5;
					if (!q2.empty())
					{
						current = q2.front();
						q2.pop();
						c2 = 1;
					}
				}
				else if (slice == 0)
				{
					slice = 5;
					q2.push(current);
					current = q2.front();
					q2.pop();
					c2 = 1;
				}
				if (c2 == 1)
					j++;
			}
		}
		if (c1 == 0 && c2 == 0 && ((c3 == 1) || (!q3.empty() && q3.top().arrival <= j)))
		{
			if (c3 == 0)
			{
				current = q3.top();
				q3.pop();
				c3 = 1;
				q3e = 0;
				j++;
			}
			else if (c3 == 1)
			{
				current.remain -= 1;
				if (current.remain == 0)
				{
					current.finish = j;
					current.wait = current.finish - current.arrival - current.burst;
					current.turnaround = current.finish - current.arrival;
					current.remain = 0;
					current.feedback = (current.wait + current.burst) / current.burst;
					c3 = 0;
					avgturn += current.turnaround;
					avgfin += current.wait;
					avgfeed += current.feedback;
					avgwait += current.wait;
					total++;
					cout << "Process " << current.pid << " finished\n";
					cout << "Process " << current.pid << " finished time is " << current.finish << " wait time is " << current.wait << " turnaround time is " << current.turnaround << " feedback ratio is " << current.feedback << endl;
				}
				else
				{
					if (q3e == 1)
					{

						cpuprocesssrjf w(0, 1, 0, 0, 0);
						w.arrival = current.arrival;
						w.burst = current.burst;
						w.feedback = current.feedback;
						w.finish = current.finish;
						w.completed = current.completed;
						w.pid = current.pid;
						w.priority = current.priority;
						w.wait = current.wait;
						w.turnaround = current.turnaround;
						w.remain = current.remain;
						q3.push(w);
						c3 = 0;
					}
				}
				if (c3 == 0 && !q3.empty())
				{
					current = q3.top();
					q3.pop();
					c3 = 1;
					q3e = 0;
				}
				if (c3 == 1)
					j++;
			}
		}
		if (c1 == 0 && c2 == 0 && c3 == 0)
			j++;
	}

	avgturn /= n;
	avgfin /= n;
	avgfeed /= n;
	avgwait /= n;

	cout << "\n avg turnaround time is: " << avgturn << " avg finish time is: " << avgfin << " avg feedback ratio is: " << avgfeed << " avg wait time is: " << avgwait << endl;
}

void mlfq()
{
	cout << "Enter no. of processes: ";
	int n;
	cin >> n;

	vector<cpuprocess> l;
	vector<cpuprocess> l1;
	vector<cpuprocesssrjf> l2;
	vector<cpuprocesssrjf> l3;

	int* cputime = new int[n];
	int* waittime = new int[n];
	int* completed = new int[n];
	int* check = new int[n];


	int n1 = 0, n2 = 0, n3 = 0;

	for (int i = 0; i < n; i++)
	{
		cout << "Enter the queue in which u want to put it: ";
		int ch;

		cputime[i] = 0;
		waittime[i] = 0;
		completed[i] = 0;
		check[i] = 0;

		cin >> ch;

		if (ch == 1)
		{
			int b, a, p;

			cout << "Enter burst time of " << i + 1 << " process: ";
			cin >> b;

			cout << "Enter arrival time of " << i + 1 << " process: ";
			cin >> a;

			cout << "Enter priority of " << i + 1 << " process: ";
			cin >> p;

			cpuprocess temp(i + 1, b, a, p, 1, 1);
			l.push_back(temp);
			l1.push_back(temp);
			n1++;
		}
		else if (ch == 2)
		{
			int b, a, p;

			cout << "Enter burst time of " << i + 1 << " process: ";
			cin >> b;

			cout << "Enter arrival time of " << i + 1 << " process: ";
			cin >> a;

			cpuprocesssrjf temp(i + 1, b, a, 0, 2, 2);
			l.push_back(temp);

			l2.push_back(temp);
			n2++;
		}
		else if (ch == 3)
		{
			int b, a, p;

			cout << "Enter burst time of " << i + 1 << " process: ";
			cin >> b;

			cout << "Enter arrival time of " << i + 1 << " process: ";
			cin >> a;

			cpuprocesssrjf temp(i + 1, b, a, 0, 3, 3);
			l.push_back(temp);

			l3.push_back(temp);
			n3++;
		}
	}

	queue<cpuprocess> q1;
	priority_queue<cpuprocesssrjf> q2;
	priority_queue<cpuprocesssrjf> q3;

	for (int i = 0; i < n1 - 1; i++)
	{
		int min = i;
		for (int j = i + 1; j < n1; j++)
		{
			if (l1[min].arrival > l1[j].arrival)
				min = j;
			else if (l1[min].arrival == l1[j].arrival)
			{
				if (l1[min].burst > l1[j].burst)
					min = j;
			}
		}

		cpuprocess temp(0, 1, 0);
		temp = l1[i];
		l1[i] = l1[min];
		l1[min] = temp;

	}

	for (int i = 0; i < n2 - 1; i++)
	{
		int min = i;
		for (int j = i + 1; j < n2; j++)
		{
			if (l2[min].arrival > l2[j].arrival)
				min = j;
			else if (l2[min].arrival == l2[j].arrival)
			{
				if (l2[min].burst > l2[j].burst)
					min = j;
			}
		}

		cpuprocesssrjf temp(0, 1, 0);
		temp = l2[i];
		l2[i] = l2[min];
		l2[min] = temp;


	}

	for (int i = 0; i < n3 - 1; i++)
	{
		int min = i;
		for (int j = i + 1; j < n3; j++)
		{
			if (l3[min].arrival > l3[j].arrival)
				min = j;
			else if (l3[min].arrival == l3[j].arrival)
			{
				if (l3[min].burst > l3[j].burst)
					min = j;
			}
		}

		cpuprocesssrjf temp(0, 1, 0);
		temp = l3[i];
		l3[i] = l3[min];
		l3[min] = temp;


	}

	int total = 0;
	int j = 0;
	cpuprocess current(0, 1, 0, 0, 0);

	int c1 = 0, c2 = 0, c3 = 0;
	int x = 0, y = 0, z = 0;
	float avgturn = 0, avgfin = 0, avgwait = 0, avgfeed = 0;
	int slice = 5;
	int q3e = 1;
	int c1e = 0;
	int q2emp = 0;
	int q3emp = 0;
	j = 0;
	while (total != n)
	{
		if (current.remain) { 
			current.remain = current.remain; 
		}
		c1e = 0;
		while (x != n1 && l1[x].arrival <= j)
		{
			q1.push(l1[x]);
			x++;
			c1 = 1;
		}
		while (y != n2 && l2[y].arrival <= j)
		{
			q2.push(l2[y]);
			y++;
			q2emp = 1;
		}
		while (z != n3 && l3[z].arrival <= j)
		{
			q3.push(l3[z]);
			z++;
			q3e = 1;
			q3emp = 1;
		}

		if (q1.empty())
		{
			c1 = 0;
		}

		if (!q1.empty() && q1.front().arrival <= j)
		{
			if (c3 == 1)
			{
				current.remain -= 1;
				c3 = 0;
				slice = 5;

				for (int i = 0; i < n; i++)
				{
					if (completed[i] == 0 && l[i].arrival <= j)
					{
						if (current.remain == 0 && current.pid == i + 1)
						{
							continue;
						}
						else
						{
							waittime[i] += 1; check[i] = 1;
						}

						if (l[i].queue == 2 && waittime[i] >= 7)
						{
							queue<cpuprocesssrjf> t;

							while (q2.top().pid != i + 1)
							{
								t.push(q2.top());
								q2.pop();
							}

							if (q2.top().remain <= 5)
							{
								cout << "promoting " << q2.top().pid << " because of aging into queue 1\n";
								q1.push(q2.top());
								q2.pop();
								waittime[i] = 0;
								cputime[i] = 0;
								l[i].queue = 1;
							}

							while (!t.empty())
							{
								q2.push(t.front());
								t.pop();
							}
						}
						else if (l[i].queue == 3 && waittime[i] >= 15)
						{
							if (current.pid == i + 1)
							{
								cout << "promoting " << current.pid << " because of aging into queue 2\n";
								cpuprocesssrjf w(0, 1, 0, 0, 0);
								w.arrival = current.arrival;
								w.burst = current.burst;
								w.feedback = current.feedback;
								w.finish = current.finish;
								w.completed = current.completed;
								w.pid = current.pid;
								w.priority = current.priority;
								w.wait = current.wait;
								w.turnaround = current.turnaround;
								w.remain = current.remain;
								q2.push(w);
								waittime[i] = 0;
								cputime[i] = 0;

								l[i].queue = 2;
							}
							else
							{
								queue<cpuprocesssrjf> t;

								while (q3.top().pid != i + 1)
								{
									t.push(q3.top());
									q3.pop();
								}

								cout << "promoting " << q3.top().pid << " because of aging into queue 2\n";
								q2.push(q3.top());
								q3.pop();
								waittime[i] = 0;
								cputime[i] = 0;

								l[i].queue = 2;

								while (!t.empty())
								{
									q3.push(t.front());
									t.pop();
								}
							}
						}
					}
				}

				if (current.remain != 0)
				{

					cpuprocesssrjf w(0, 1, 0, 0, 0);
					w.arrival = current.arrival;
					w.burst = current.burst;
					w.feedback = current.feedback;
					w.finish = current.finish;
					w.completed = current.completed;
					w.pid = current.pid;
					w.priority = current.priority;
					w.wait = current.wait;
					w.turnaround = current.turnaround;
					w.remain = current.remain;
					q3.push(w);
				}
				else
				{
					current.finish = j;
					current.wait = current.finish - current.arrival - current.burst;
					current.turnaround = current.finish - current.arrival;
					current.remain = 0;
					current.feedback = (current.wait + current.burst) / current.burst;
					int pid = current.pid;
					completed[pid - 1] = 1;
					avgturn += current.turnaround;
					avgfin += current.wait;
					avgfeed += current.feedback;
					avgwait += current.wait;
					total++;
					cout << "Process " << current.pid << " finished\n";
					cout << "Process " << current.pid << " finished time is " << current.finish << " wait time is " << current.wait << " turnaround time is " << current.turnaround << " feedback ratio is " << current.feedback << endl;
				}
			}
			if (c2 == 1)
			{
				current.remain -= 1;
				c2 = 0;
				int pid = current.pid;
				cputime[pid - 1] += 1;

				for (int i = 0; i < n; i++)
				{
					if (completed[i] == 0 && l[i].arrival <= j)
					{
						if (current.remain == 0 && current.pid == i + 1)
						{
							continue;
						}
						else
						{
							waittime[i] += 1; check[i] = 1;
						}

						if (l[i].queue == 2 && waittime[i] >= 7)
						{
							if (current.pid == i + 1)
							{
								if (current.remain <= 5)
								{
									cout << "Promoting " << i + 1 << " into queue1\n";
									cpuprocesssrjf w(0, 1, 0, 0, 0);
									w.arrival = current.arrival;
									w.burst = current.burst;
									w.feedback = current.feedback;
									w.finish = current.finish;
									w.completed = current.completed;
									w.pid = current.pid;
									w.priority = current.priority;
									w.wait = current.wait;
									w.turnaround = current.turnaround;
									w.remain = current.remain;
									q1.push(w);
									waittime[i] = 0;
									cputime[i] = 0;
									l[i].queue = 1;
								}
							}
							else
							{
								queue<cpuprocesssrjf> t;

								while (q2.top().pid != i + 1)
								{
									t.push(q2.top());
									q2.pop();
								}

								if (q2.top().remain <= 5)
								{
									cout << "promoting " << q2.top().pid << " because of aging into queue 1\n";
									q1.push(q2.top());
									q2.pop();
									waittime[i] = 0;
									cputime[i] = 0;
									l[i].queue = 1;
								}

								while (!t.empty())
								{
									q2.push(t.front());
									t.pop();
								}
							}
						}
						else if (l[i].queue == 3 && waittime[i] >= 15)
						{
							queue<cpuprocesssrjf> t;

							while (q3.top().pid != i + 1)
							{
								t.push(q3.top());
								q3.pop();
							}

							cout << "promoting " << q3.top().pid << " because of aging into queue 2\n";
							q2.push(q3.top());
							q3.pop();
							waittime[i] = 0;
							cputime[i] = 0;
							l[i].queue = 2;

							while (!t.empty())
							{
								q3.push(t.front());
								t.pop();
							}
						}
					}
				}

				if (current.remain != 0)
				{
					if (cputime[pid - 1] >= 3)
					{
						cpuprocesssrjf w(0, 1, 0, 0, 0);
						w.arrival = current.arrival;
						w.burst = current.burst;
						w.feedback = current.feedback;
						w.finish = current.finish;
						w.completed = current.completed;
						w.pid = current.pid;
						w.priority = current.priority;
						w.wait = current.wait;
						w.turnaround = current.turnaround;
						w.remain = current.remain;
						q3.push(w);

						cout << "Demoting process" << pid << " from queue 2 to queue 3\n";

						cputime[pid - 1] = 0;
						waittime[pid - 1] = 0;
						int pid = current.pid;
						l[pid - 1].queue = 3;
					}
					else
					{

						cpuprocesssrjf w(0, 1, 0, 0, 0);
						w.arrival = current.arrival;
						w.burst = current.burst;
						w.feedback = current.feedback;
						w.finish = current.finish;
						w.completed = current.completed;
						w.pid = current.pid;
						w.priority = current.priority;
						w.wait = current.wait;
						w.turnaround = current.turnaround;
						w.remain = current.remain;
						q2.push(w);
					}
				}
				else
				{
					current.finish = j;
					current.wait = current.finish - current.arrival - current.burst;
					current.turnaround = current.finish - current.arrival;
					current.remain = 0;
					current.feedback = (current.wait + current.burst) / current.burst;
					total++;
					int pid = current.pid;
					completed[pid - 1] = 1;
					avgturn += current.turnaround;
					avgfin += current.wait;
					avgfeed += current.feedback;
					avgwait += current.wait;

					cout << "Process " << current.pid << " finished\n";
					cout << "Process " << current.pid << " finished time is " << current.finish << " wait time is " << current.wait << " turnaround time is " << current.turnaround << " feedback ratio is " << current.feedback << endl;
				}
			}

			cpuprocess d = q1.front();
			q1.pop();
			c1 = 1;

			while (d.remain > 5)
			{
				cout << "demoting the process " << d.pid << " from queue 1 and putting in q2\n";

				current = d;

				cpuprocesssrjf w(0, 1, 0, 0, 0);
				w.arrival = current.arrival;
				w.burst = current.burst;
				w.feedback = current.feedback;
				w.finish = current.finish;
				w.completed = current.completed;
				w.pid = current.pid;
				w.priority = current.priority;
				w.wait = current.wait;
				w.turnaround = current.turnaround;
				w.remain = current.remain;
				int pid = d.pid;
				l[pid - 1].queue = 2;
				q2.push(w);
				if (!q1.empty())
				{
					d = q1.front();
					q1.pop();
					c1 = 1;
				}
				else
				{
					c1 = 0;
					break;
				}
			}
			if (c1 == 1)
			{
				d.finish = j + d.remain;
				d.wait = d.finish - d.arrival - d.burst;
				d.turnaround = d.finish - d.arrival;
				d.feedback = (d.wait + d.burst) / d.burst;
				j += d.remain;
				int pid = d.pid;
				completed[pid - 1] = 1;
				avgturn += d.turnaround;
				avgfin += d.wait;
				avgfeed += d.feedback;
				avgwait += d.wait;
				c1e = 1;

				cout << "Process " << d.pid << " finished\n";
				cout << "Process " << d.pid << " finished time is " << d.finish << " wait time is " << d.wait << " turnaround time is " << d.turnaround << " feedback ratio is " << d.feedback << endl;

				total++;

				while (x != n1 && l1[x].arrival <= j)
				{
					q1.push(l1[x]);
					x++;
					c1 = 1;
				}
				while (y != n2 && l2[y].arrival <= j)
				{
					q2.push(l2[y]);
					y++;
					q2emp = 1;
				}
				while (z != n3 && l3[z].arrival <= j)
				{
					q3.push(l3[z]);
					z++;
					q3e = 1;
					q3emp = 1;
				}

				for (int i = 0; i < n; i++)
				{
					if (completed[i] == 0 && l[i].arrival <= j)
					{
						if (check[i] == 0)
						{
							waittime[i] += d.remain - l[i].arrival;
							check[i] = 1;
						}
						else
							waittime[i] += d.remain;

						if (l[i].queue == 2 && waittime[i] >= 7)
						{
							queue<cpuprocesssrjf> t;

							while (q2.top().pid != i + 1)
							{
								t.push(q2.top());
								q2.pop();
							}

							if (q2.top().remain <= 5)
							{
								cout << "promoting " << q2.top().pid << " because of aging into queue 1\n";
								q1.push(q2.top());
								q2.pop();
								waittime[i] = 0;
								cputime[i] = 0;
								l[i].queue = 1;
							}

							while (!t.empty())
							{
								q2.push(t.front());
								t.pop();
							}
						}
						else if (l[i].queue == 3 && waittime[i] >= 15)
						{
							queue<cpuprocesssrjf> t;

							while (q3.top().pid != i + 1)
							{
								t.push(q3.top());
								q3.pop();
							}

							cout << "promoting " << q3.top().pid << " because of aging into queue 2\n";
							q2.push(q3.top());
							q3.pop();
							waittime[i] = 0;
							l[i].queue = 2;
							cputime[i] = 0;

							while (!t.empty())
							{
								q3.push(t.front());
								t.pop();
							}
						}
					}
				}
				d.remain = 0;

			}
		}

		if (q1.empty())
		{
			c1 = 0;
		}

		if (c1 == 0 && ((c2 == 1) || (!q2.empty() && q2.top().arrival <= j)))
		{
			if (c3 == 1)
			{
				current.remain -= 1;
				c3 = 0;
				slice = 5;

				for (int i = 0; i < n; i++)
				{
					if (completed[i] == 0 && l[i].arrival <= j)
					{
						if (current.remain == 0 && current.pid == i + 1)
						{
							continue;
						}
						else
						{
							waittime[i] += 1; check[i] = 1;
						}

						if (l[i].queue == 2 && waittime[i] >= 7)
						{
							queue<cpuprocesssrjf> t;

							while (q2.top().pid != i + 1)
							{
								t.push(q2.top());
								q2.pop();
							}

							if (q2.top().remain <= 5)
							{
								cout << "promoting " << q2.top().pid << " because of aging into queue 1\n";
								q1.push(q2.top());
								q2.pop();
								waittime[i] = 0;
								cputime[i] = 0;
								l[i].queue = 1;

							}

							while (!t.empty())
							{
								q2.push(t.front());
								t.pop();
							}
						}
						else if (l[i].queue == 3 && waittime[i] >= 15)
						{
							if (current.pid == i + 1)
							{
								cout << "promoting " << current.pid << " because of aging into queue 2\n";
								cpuprocesssrjf w(0, 1, 0, 0, 0);
								w.arrival = current.arrival;
								w.burst = current.burst;
								w.feedback = current.feedback;
								w.finish = current.finish;
								w.completed = current.completed;
								w.pid = current.pid;
								w.priority = current.priority;
								w.wait = current.wait;
								w.turnaround = current.turnaround;
								w.remain = current.remain;
								q2.push(w);
								waittime[i] = 0;
								l[i].queue = 2;

								cputime[i] = 0;
							}
							else
							{
								queue<cpuprocesssrjf> t;

								while (q3.top().pid != i + 1)
								{
									t.push(q3.top());
									q3.pop();
								}

								cout << "promoting " << q3.top().pid << " because of aging into queue 2\n";
								q2.push(q3.top());
								q3.pop();
								waittime[i] = 0;
								cputime[i] = 0;
								l[i].queue = 2;


								while (!t.empty())
								{
									q3.push(t.front());
									t.pop();
								}
							}
						}
					}
				}

				if (current.remain != 0)
				{
					cpuprocesssrjf w(0, 1, 0, 0, 0);
					w.arrival = current.arrival;
					w.burst = current.burst;
					w.feedback = current.feedback;
					w.finish = current.finish;
					w.completed = current.completed;
					w.pid = current.pid;
					w.priority = current.priority;
					w.wait = current.wait;
					w.turnaround = current.turnaround;
					w.remain = current.remain;
					q3.push(w);
				}
				else
				{
					current.finish = j;
					current.wait = current.finish - current.arrival - current.burst;
					current.turnaround = current.finish - current.arrival;
					current.remain = 0;
					current.feedback = (current.wait + current.burst) / current.burst;
					int pid = current.pid;
					completed[pid - 1] = 1;
					avgturn += current.turnaround;
					avgfin += current.wait;
					avgfeed += current.feedback;
					avgwait += current.wait;
					total++;
					cout << "Process " << current.pid << " finished\n";
					cout << "Process " << current.pid << " finished time is " << current.finish << " wait time is " << current.wait << " turnaround time is " << current.turnaround << " feedback ratio is " << current.feedback << endl;
				}

				if (!q1.empty())
				{

					continue;
				}
			}
			if (c2 == 0)
			{
				c2 = 1;
				current = q2.top();
				q2.pop();
				
					j++;
			}
			else
			{
				current.remain -= 1;

				for (int i = 0; i < n; i++)
				{
					if (completed[i] == 0 && l[i].arrival <= j)
					{
						if (current.remain == 0 && current.pid == i + 1)
						{
							continue;
						}
						else
						{
							waittime[i] += 1; check[i] = 1;
						}

						if (l[i].queue == 2 && waittime[i] >= 7)
						{
							if (current.pid == i + 1)
							{
								if (current.remain <= 5)
								{
									cout << "promoting " << i + 1 << " because of aging into queue 1\n";

									cpuprocesssrjf w(0, 1, 0, 0, 0);
									w.arrival = current.arrival;
									w.burst = current.burst;
									w.feedback = current.feedback;
									w.finish = current.finish;
									w.completed = current.completed;
									w.pid = current.pid;
									w.priority = current.priority;
									w.wait = current.wait;
									w.turnaround = current.turnaround;
									w.remain = current.remain;
									q1.push(w);
									waittime[i] = 0;
									cputime[i] = 0;
									l[i].queue = 1;
									c2 = 0;
								}
							}
							else
							{
								queue<cpuprocesssrjf> t;

								while (q2.top().pid != i + 1)
								{
									t.push(q2.top());
									q2.pop();
								}

								if (q2.top().remain <= 5)
								{
									cout << "promoting " << q2.top().pid << " because of aging into queue 1\n";
									q1.push(q2.top());
									q2.pop();
									waittime[i] = 0;
									cputime[i] = 0;
									l[i].queue = 1;

								}

								while (!t.empty())
								{
									q2.push(t.front());
									t.pop();
								}
							}
						}
						else if (l[i].queue == 3 && waittime[i] >= 15)
						{
							queue<cpuprocesssrjf> t;

							while (q3.top().pid != i + 1)
							{
								t.push(q3.top());
								q3.pop();
							}

							cout << "promoting " << q3.top().pid << " because of aging into queue 2\n";
							q2.push(q3.top());
							q3.pop();
							waittime[i] = 0;
							cputime[i] = 0;
							l[i].queue = 2;


							while (!t.empty())
							{
								q3.push(t.front());
								t.pop();
							}
						}
					}
				}

				if (current.remain == 0)
				{
					current.finish = j;
					current.wait = current.finish - current.arrival - current.burst;
					current.turnaround = current.finish - current.arrival;
					current.remain = 0;
					current.feedback = (current.wait + current.burst) / current.burst;
					c2 = 0;
					avgturn += current.turnaround;
					avgfin += current.wait;
					avgfeed += current.feedback;
					int pid = current.pid;
					completed[pid - 1] = 1;
					avgwait += current.wait;
					total++;
					cout << "Process " << current.pid << " finished\n";
					cout << "Process " << current.pid << " finished time is " << current.finish << " wait time is " << current.wait << " turnaround time is " << current.turnaround << " feedback ratio is " << current.feedback << endl;
					if (!q1.empty())
					{

						continue;
					}
				}
				else
				{
					int pid = current.pid;
					cputime[pid - 1] += 1;
					if (cputime[pid - 1] >= 3)
					{
						cpuprocesssrjf w(0, 1, 0, 0, 0);
						w.arrival = current.arrival;
						w.burst = current.burst;
						w.feedback = current.feedback;
						w.finish = current.finish;
						w.completed = current.completed;
						w.pid = current.pid;
						w.priority = current.priority;
						w.wait = current.wait;
						w.turnaround = current.turnaround;
						w.remain = current.remain;
						int pid = current.pid;
						q3.push(w);
						l[pid - 1].queue = 3;


						cout << "Demoting process" << pid << " from queue 2 to queue 3\n";

						cputime[pid - 1] = 0;
						waittime[pid - 1] = 0;
						c2 = 0;
					}

					if (!q1.empty())
					{
						continue;
					}
				}
				if (c2 == 0 && !q2.empty())
				{
					current = q2.top();
					q2.pop();
					c2 = 1;
				}
				if (c2 == 1)
					j++;
			}
		}
		if (c1 == 0 && c2 == 0 && ((c3 == 1) || (!q3.empty() && q3.top().arrival <= j)))
		{
			if (c3 == 0)
			{
				current = q3.top();
				q3.pop();
				c3 = 1;
				q3e = 0;
			
					j++;
			}
			else if (c3 == 1)
			{
				slice--;
				current.remain -= 1;

				for (int i = 0; i < n; i++)
				{
					if (completed[i] == 0 && l[i].arrival <= j)
					{
						if (current.remain == 0 && current.pid == i + 1)
						{
							continue;
						}
						else
						{
							waittime[i] += 1; check[i] = 1;
						}

						if (l[i].queue == 2 && waittime[i] >= 7)
						{
							queue<cpuprocesssrjf> t;

							while (q2.top().pid != i + 1)
							{
								t.push(q2.top());
								q2.pop();
							}

							if (q2.top().remain <= 5)
							{
								cout << "promoting " << q2.top().pid << " because of aging into queue 1\n";
								q1.push(q2.top());
								q2.pop();
								waittime[i] = 0;
								cputime[i] = 0;
								l[i].queue = 1;

							}

							while (!t.empty())
							{
								q2.push(t.front());
								t.pop();
							}
						}
						else if (l[i].queue == 3 && waittime[i] >= 15)
						{
							if (current.pid == i + 1)
							{
								cout << "promoting " << current.pid << " because of aging into queue 2\n";
								cpuprocesssrjf w(0, 1, 0, 0, 0);
								w.arrival = current.arrival;
								w.burst = current.burst;
								w.feedback = current.feedback;
								w.finish = current.finish;
								w.completed = current.completed;
								w.pid = current.pid;
								w.priority = current.priority;
								w.wait = current.wait;
								w.turnaround = current.turnaround;
								w.remain = current.remain;
								q2.push(w);
								waittime[i] = 0;
								cputime[i] = 0;
								l[i].queue = 2;
								c3 = 0;
							}
							else
							{
								queue<cpuprocesssrjf> t;

								while (q3.top().pid != i + 1)
								{
									t.push(q3.top());
									q3.pop();
								}

								cout << "promoting " << q3.top().pid << " because of aging into queue 2\n";
								q2.push(q3.top());
								q3.pop();
								waittime[i] = 0;
								cputime[i] = 0;
								l[i].queue = 2;


								while (!t.empty())
								{
									q3.push(t.front());
									t.pop();
								}
							}
						}
					}
				}

				if (current.remain == 0)
				{
					current.finish = j;
					current.wait = current.finish - current.arrival - current.burst;
					current.turnaround = current.finish - current.arrival;
					current.remain = 0;
					current.feedback = (current.wait + current.burst) / current.burst;
					c3 = 0;
					avgturn += current.turnaround;
					avgfin += current.wait;
					avgfeed += current.feedback;
					avgwait += current.wait;
					int pid = current.pid;
					completed[pid - 1] = 1;
					total++;
					cout << "Process " << current.pid << " finished\n";
					cout << "Process " << current.pid << " finished time is " << current.finish << " wait time is " << current.wait << " turnaround time is " << current.turnaround << " feedback ratio is " << current.feedback << endl;
					slice = 5;
					if (!q1.empty())
					{
						//j++;
						continue;
					}
					if (!q2.empty())
					{
						//j++;
						continue;
					}
					if (!q3.empty())
					{
						current = q3.top();
						q3.pop();
						c3 = 1;
					}
				}
				else if (slice == 0)
				{
					slice = 5;
					cpuprocesssrjf w(0, 1, 0, 0, 0);
					w.arrival = current.arrival;
					w.burst = current.burst;
					w.feedback = current.feedback;
					w.finish = current.finish;
					w.completed = current.completed;
					w.pid = current.pid;
					w.priority = current.priority;
					w.wait = current.wait;
					w.turnaround = current.turnaround;
					w.remain = current.remain;
					q3.push(w);
					if (!q1.empty())
					{
						//j++;
						continue;
					}
					if (!q2.empty())
					{
						//j++;
						continue;
					}
					current = q3.top();
					q3.pop();
					c3 = 1;
				}
				else
				{
					if (q3e == 1)
					{
						cpuprocess temp = current;
						cpuprocesssrjf w(0, 1, 0, 0, 0);
						w.arrival = current.arrival;
						w.burst = current.burst;
						w.feedback = current.feedback;
						w.finish = current.finish;
						w.completed = current.completed;
						w.pid = current.pid;
						w.priority = current.priority;
						w.wait = current.wait;
						w.turnaround = current.turnaround;
						w.remain = current.remain;
						q3.push(w);
						c3 = 0;
						if (q3.top().pid != temp.pid)
							slice = 5;
						if (!q1.empty())
						{
							//j++;
							continue;
						}
						if (!q2.empty())
						{
							//j++;
							continue;
						}
					}
				}
				if (c3 == 0 && !q3.empty())
				{
					current = q3.top();
					q3.pop();
					c3 = 1;
					q3e = 0;
				}
				if (c3 == 1)
					j++;
			}
		}
		if (c1 == 0 && c2 == 0 && c3 == 0)
			j++;
		if (q2.empty())
		{
			q2emp = 0;
		}
		else
		{
			q2emp = 1;
		}
		if (q3.empty())
		{
			q3emp = 0;
		}
		else
		{
			q3emp = 1;
		}
	}

	avgturn /= n;
	avgfin /= n;
	avgfeed /= n;
	avgwait /= n;

	cout << "\n avg turnaround time is: " << avgturn << " avg finish time is: " << avgfin << " avg feedback ratio is: " << avgfeed << " avg wait time is: " << avgwait << endl;
}

int cpumain()
{
	int ch;

	cout << "press 1 for mlq\n";
	cout << "press 2 for mlfq\n";
	cout << "press 3 for longest job\n";
	cout << "press 4 for feedback\n";
	cin >> ch;

	if (ch == 1)
	{
		mlq();
	}
	if (ch == 2)
	{
		mlfq();
	}
	if (ch == 3)
	{
		longestjob();
	}
	if (ch == 4)
	{
		lowestfeedbackratio();
	}

	return 0;
}

class dataa
{
public:
	int pid;
	int page[64];
	int pno;

	dataa()
	{
		pid = -1;
		for (int i = 0; i < 64; i++)
		{
			page[i] = -1;
		}
		pno = -1;
	}
};

struct t
{
	int id;
	int size;
	int table;
	int pages;
};

class processmm
{
public:
	t val;
	processmm* next;

	bool operator==(const processmm& rhs) const noexcept
	{
		 //logic here
		return this->val.id == rhs.val.id; // for example
	}
};

class ram
{
public:
	int size;
	int framesize;
	int addsize;
	int* free;
	dataa* frames;
	int n;
	int fn;

	ram(int a = 16)
	{
		size = a;
		framesize = 64;
		addsize = 16;
		int temp = 0;
		for (int i = a; i > 1; i /= 2)
		{
			temp++;
		}
		n = ((pow(2, 10) * pow(2, temp))) / 64;
		free = new int[n];
		for (int i = 0; i < n; i++)
		{
			free[i] = i;
		}
		random_shuffle(free, free + n - 1);

		fn = n - 1;
		frames = new dataa[n];
	}

	int getfreeframe()
	{
		int a = free[fn];
		fn--;
		return a;
	}

	void newfreeframe(int temp)
	{
		free[++fn] = temp;
	}

	dataa frameacess(int add)
	{
		return frames[add];
	}

	void frameinsert(dataa val, int add)
	{
		frames[add] = val;
	}
	~ram()
	{
		delete[] frames;
		delete[] free;
	}
};

struct tlbdata
{
	int frameno;
	int pageno;
	tlbdata* next;
};

class tlb
{
public:
	tlbdata tlbframes[5];
	int lru[5];
	int n;

	tlb()
	{
		n = 0;
	}

	bool tlbcheck(int pno, int& frame)
	{
		for (int i = 0; i < n; i++)
		{
			if (pno == tlbframes[i].pageno)
			{
				frame = tlbframes[i].frameno;
				newlru(i);
				return true;
			}
		}

		return false;
	}
	void tlbflush()
	{
		n = 0;
	}

	void tlbreplace(int pno, int frame)
	{
		cout << "replacing page " << tlbframes[lru[0]].pageno << "with frame " << tlbframes[lru[0]].frameno << endl;
		tlbframes[lru[0]].frameno = frame;
		tlbframes[lru[0]].pageno = pno;
		newlru(0);
	}

	void newlru(int no)
	{
		for (int i = 0; i < n; i++)
		{
			if (lru[i] == no)
			{
				for (int j = i + 1; j < n; j++)
				{
					lru[j - 1] = lru[j];
				}

				lru[n - 1] = no;
				break;
			}
		}
	}

	void tlbinsert(int pno, int frame)
	{
		if (n != 5)
		{
			tlbframes[n].pageno = pno;
			tlbframes[n].frameno = frame;
			lru[n] = n;
			n++;
		}
		else
		{
			cout << "\n\nTlb full using lru scheme\n";

			tlbreplace(pno, frame);
		}
	}
};

class tlbramdata
{
public:
	list<tlbdata> li[64];
	int page[64];
	int pid;
	int pno;

	tlbramdata()
	{
		pid = -1;
		for (int i = 0; i < 64; i++)
		{
			page[i] = -1;
		}
		pno = -1;
	}
};

class tlbram
{
public:
	int size;
	int framesize;
	int* free;
	tlbramdata* frames;
	int n;
	int fn;

	tlbram(int a = 16)
	{
		size = a;
		framesize = 64;
		int temp = 0;
		for (int i = a; i > 1; i /= 2)
		{
			temp++;
		}
		n = ((pow(2, 10) * pow(2, temp))) / 64;
		free = new int[n];
		for (int i = 0; i < n; i++)
		{
			free[i] = i;
		}

		random_shuffle(free, free + n - 1);
		fn = n - 1;


		frames = new tlbramdata[n];
	}

	int getfreeframe()
	{
		return free[fn--];
	}

	void newfreeframe(int temp)
	{
		free[++fn] = temp;
	}

	tlbramdata frameacess(int add)
	{
		return frames[add];
	}

	void frameinsert(tlbramdata val, int add)
	{
		frames[add] = val;
	}
	~tlbram()
	{
		delete[] frames;
		delete[] free;
	}
};

struct rrdata
{
	int clock;
	int pid;
	int pno;
	int rframe;
	int page[64];

	bool operator==(const rrdata& a) const noexcept
	{
		return this->rframe == a.rframe;
	}
};

struct repdata
{
	rrdata data;
	repdata* next;
};

void function4()
{
	cout << "\n\n----------------Memory Management Unit---------------\n\n";
	cout << "\n----------------Press 1 for 2 level page table---------------\n";
	cout << "----------------Press 2 for Hash TLB---------------\n\n";

	int n;
	cout << "Enter choice: ";
	cin >> n;

	if (n == 1)
	{
		int size;
		cout << "\n\n-------Ram frame size is 64 Byte------\n";
		cout << "-----------Address Bus is of 16 Byte------\n";
		cout << "\nEnter the size of ram (1KB - 64KB): \n";

		do
		{
			cout << "Enter your size: ";
			cin >> size;
		} while (size < 1 && size > 64);

		ram r1(size);

		cout << "Ram size is: " << r1.size << "KB" << endl;
		cout << "No. of ram frames is: " << r1.n << endl;
		cout << "No of free frame in ram is: " << r1.fn + 1 << endl;

		list<processmm> l1;

		int choice;
		int ids = 1;

		while (1)
		{
			cout << "\n\n1. Insert Process\n";
			cout << "2. Remove Process\n";
			cout << "3. Access Process\n";
			cout << "4. to exit\n";

			cout << "Enter choice: ";
			cin >> choice;

			if (choice == 1)
			{
				processmm temp;

				int s;
				cout << "Enter size of process " << ids << " in Bytes : ";
				cin >> s;

				temp.val.id = ids;
				temp.val.size = s;
				temp.val.pages = (s / 64) + (0 != s % 64);

				cout << endl << endl;
				cout << "No of process page is: " << temp.val.pages << endl;

				int tot = 1 + temp.val.pages / 64 - (0 == s % 64 && temp.val.pages >= 64);

				cout << "No of pages required to create table is: " << tot + 1 << endl;

				temp.next = NULL;

				if (r1.fn >= tot + 1 + temp.val.pages)
				{
					temp.val.table = r1.getfreeframe();

					cout << "assigning the frame for main page table " << temp.val.table << endl;
					int pp = 0;

					for (int i = 0; i < tot; i++)
					{
						int t = r1.getfreeframe();

						cout << "assigning the frame for secondary page table " << t << endl;

						r1.frames[temp.val.table].pid = 0;
						r1.frames[temp.val.table].pno = -1;
						r1.frames[temp.val.table].page[i] = t;
						
						if (i != tot - 1)
						{
							for (int j = 0; j < 64; j++, pp++)
							{
								r1.frames[r1.frames[temp.val.table].page[i]].pid = 0;
								r1.frames[r1.frames[temp.val.table].page[i]].pno = -1;
								r1.frames[r1.frames[temp.val.table].page[i]].page[j] = r1.getfreeframe();

								cout << "assigning the actual frame for pages " << pp << "\tframe is: " << r1.frames[r1.frames[temp.val.table].page[i]].page[j] << endl;

								int v1 = 64;

								if (temp.val.size % 64 != 0 && j == 63)
									v1 = temp.val.size % 64;

								for (int k = 0; k < v1; k++)
								{
									r1.frames[r1.frames[r1.frames[temp.val.table].page[i]].page[j]].pid = temp.val.id;
									r1.frames[r1.frames[r1.frames[temp.val.table].page[i]].page[j]].pno = pp;
									r1.frames[r1.frames[r1.frames[temp.val.table].page[i]].page[j]].page[k] = k;
								}
							}
						}
						else
						{
							int val = (temp.val.pages) % 64;
							if ((temp.val.pages) % 64 == 0)
								val = 64;
							for (int j = 0; j < val; j++, pp++)
							{
								r1.frames[r1.frames[temp.val.table].page[i]].pid = 0;
								r1.frames[r1.frames[temp.val.table].page[i]].pno = -1;
								r1.frames[r1.frames[temp.val.table].page[i]].page[j] = r1.getfreeframe();

								cout << "assigning the actual frame for pages " << pp << "\tframe is: " << r1.frames[r1.frames[temp.val.table].page[i]].page[j] << endl;


								int v1 = 64;

								if (j == val - 1 && temp.val.size % 64 != 0)
									v1 = temp.val.size % 64;

								for (int k = 0; k < v1; k++)
								{
									r1.frames[r1.frames[r1.frames[temp.val.table].page[i]].page[j]].pid = temp.val.id;
									r1.frames[r1.frames[r1.frames[temp.val.table].page[i]].page[j]].pno = pp;
									r1.frames[r1.frames[r1.frames[temp.val.table].page[i]].page[j]].page[k] = k;
								}
							}
						}
					}
					ids++;
					l1.push_back(temp);

					cout << "Remaining frames in ram: " << r1.fn + 1 << endl;
				}
				else
				{
					cout << "Remove some processes first current process cannot be entered\n\n";
				}
			}
			else if (choice == 2)
			{
				int a;

				cout << "Enter the id of process: ";
				cin >> a;
				processmm temp;

				for (list<processmm>::iterator i = l1.begin(); i != l1.end(); i++)
				{
					if (a == (*i).val.id)
					{
						r1.newfreeframe((*i).val.table);

						cout << "freeing frame of main table " << (*i).val.table << endl;

						int tot = 1 + (*i).val.pages / 64 - (0 == (*i).val.size % 64 && (*i).val.pages >= 64);

						for (int j = 0; j < tot; j++)
						{
							r1.newfreeframe(r1.frames[(*i).val.table].page[j]);

							cout << "freeing frame of secondary table " << r1.frames[(*i).val.table].page[j] << endl;


							if (j != tot - 1)
							{
								for (int k = 0; k < 64; k++)
								{
									r1.newfreeframe(r1.frames[r1.frames[(*i).val.table].page[j]].page[k]);

									cout << "freeing frame of actual pages " << j << "\tframe is: " << r1.frames[r1.frames[(*i).val.table].page[j]].page[k] << endl;

								}
							}
							else
							{
								int val = ((*i).val.pages) % 64;
								if ((*i).val.pages % 64 == 0)
									val = 64;
								for (int k = 0; k < val; k++)
								{
									r1.newfreeframe(r1.frames[r1.frames[(*i).val.table].page[j]].page[k]);
									cout << "freeing frame of actual pages " << j << "\tframe is: " << r1.frames[r1.frames[(*i).val.table].page[j]].page[k] << endl;
								}
							}
						}

						cout << "\n\nRam size is: " << r1.size << "KB" << endl;
						cout << "No. of ram frames is: " << r1.n << endl;
						cout << "No of free frame in ram is: " << r1.fn + 1 << endl;

						temp = *i;
					}
				}

				l1.remove(temp);
			}
			else if (choice == 3)
			{
				int a;

				cout << "Enter process number you want to access: ";
				cin >> a;

				for (list<processmm>::iterator i = l1.begin(); i != l1.end(); i++)
				{
					if (a == (*i).val.id)
					{
						int p1, p2, d, pn;

						cout << "Enter page number you want: ";
						cin >> pn;

						cout << "Enter offset: ";
						cin >> d;

						if (pn >= (*i).val.pages)
							cout << "invalid page\n";
						else if (pn == (*i).val.pages - 1 && (d >= (*i).val.size % 64 && (*i).val.size % 64 != 0) || d > 63)
							cout << "invalid offset\n";
						else
						{
							p1 = (pn) / 64;
							p2 = (pn) % 64;

							cout << "p1 is: " << p1 << endl;
							cout << "p2 is: " << p2 << endl;
							cout << "id is: " << r1.frames[r1.frames[r1.frames[(*i).val.table].page[p1]].page[p2]].pid << endl;
							cout << "pno is: " << r1.frames[r1.frames[r1.frames[(*i).val.table].page[p1]].page[p2]].pno << endl;
							cout << "offset is: " << r1.frames[r1.frames[r1.frames[(*i).val.table].page[p1]].page[p2]].page[d] << endl;
						}
					}
				}
			}
			else
				break;
		}
	}
	else if (n == 2)
	{
		int size;
		cout << "\n\n-------Ram frame size is 64 Byte------\n";
		cout << "-----------Address Bus is of 16 Byte------\n";
		cout << "\nEnter the size of ram (1KB - 64KB): \n";

		do
		{
			cout << "Enter your size: ";
			cin >> size;
		} while (size < 1 && size > 64);

		tlbram r1(size);

		tlb tlb1;

		cout << "Ram size is: " << r1.size << "KB" << endl;
		cout << "No. of ram frames is: " << r1.n << endl;
		cout << "No of free frame in ram is: " << r1.fn + 1 << endl;

		list<processmm> l1;

		int choice;
		int ids = 1;
		int prev = 0;

		while (1)
		{
			cout << "\n\n1. Insert Process\n";
			cout << "2. Remove Process\n";
			cout << "3. Access Process\n";
			cout << "4. to exit\n";

			cout << "Enter choice: ";
			cin >> choice;

			if (choice == 1)
			{
				processmm temp;

				int s;
				cout << "Enter size of process " << ids << " in Bytes : ";
				cin >> s;

				temp.val.id = ids;
				temp.val.size = s;

				if (s < 64)
					temp.val.pages = 1;
				else
					temp.val.pages = s / 64 + (0 != s % 64);

				cout << endl << endl;
				cout << "the total pages of process is: " << temp.val.pages << endl;

				temp.next = NULL;

				if (r1.fn >= 1 + temp.val.pages)
				{
					temp.val.table = r1.getfreeframe();

					cout << "assigning the frame for main page table " << temp.val.table << endl;
					r1.frames[temp.val.table].pid = 0;
					r1.frames[temp.val.table].pno = -1;

					for (int i = 0; i < temp.val.pages; i++)
					{
						int t = r1.getfreeframe();

						cout << "assigning the actual page frame " << i << "\t frame is: " << t << endl;

						int h = (i + 3) % 61;

						tlbdata f;
						f.frameno = t;
						f.pageno = i;
						f.next = NULL;

						r1.frames[temp.val.table].li[h].push_back(f);

						if (i != temp.val.pages - 1)
						{
							for (int j = 0; j < 64; j++)
							{
								r1.frames[t].pid = ids;
								r1.frames[t].pno = i;
								r1.frames[t].page[j] = j;
							}
						}
						else
						{
							int v = temp.val.size % 64;

							if (temp.val.size % 64 == 0)
								v = 64;
							for (int j = 0; j < v; j++)
							{
								r1.frames[t].pid = ids;
								r1.frames[t].pno = i;
								r1.frames[t].page[j] = j;
							}
						}
					}

					ids++;
					l1.push_back(temp);
				}
				else
				{
					cout << "Remove some processes first current process cannot be entered\n\n";
				}
			}
			else if (choice == 2)
			{
				int a;

				cout << "Enter the id of process: ";
				cin >> a;
				processmm temp;

				for (list<processmm>::iterator i = l1.begin(); i != l1.end(); i++)
				{
					if (a == (*i).val.id)
					{
						r1.newfreeframe((*i).val.table);

						cout << "freeing frame of main table " << (*i).val.table << endl;

						for (int k = 0; k < 64; k++)
						{
							while (!r1.frames[(*i).val.table].li[k].empty())
							{
								r1.newfreeframe(r1.frames[(*i).val.table].li[k].front().frameno);

								cout << "freeing the actual page frame " << r1.frames[(*i).val.table].li[k].front().frameno << endl;


								r1.frames[(*i).val.table].li[k].pop_front();
							}
						}

						cout << "Ram size is: " << r1.size << "KB" << endl;
						cout << "No. of ram frames is: " << r1.n << endl;
						cout << "No of free frame in ram is: " << r1.fn + 1 << endl;

						temp = *i;
					}
				}

				l1.remove(temp);
			}
			else if (choice == 3)
			{
				int a;

				cout << "Enter process number you want to access: ";
				cin >> a;

				for (list<processmm>::iterator i = l1.begin(); i != l1.end(); i++)
				{
					if (a == (*i).val.id)
					{
						int p, d, pn;

						cout << "Enter page number you want: ";
						cin >> pn;

						cout << "Enter offset: ";
						cin >> d;

						if (pn >= (*i).val.pages)
							cout << "invalid page\n";
						else if (pn == (*i).val.pages - 1 && (d >= (*i).val.size % 64 && (*i).val.size % 64 != 0) || d > 63)
							cout << "invalid offset\n";
						else
						{
							if (prev == a)
							{
								int c;
								if (tlb1.tlbcheck(pn, c))
								{
									cout << "tlb hit" << endl;
									cout << "id is: " << r1.frames[c].pid << endl;
									cout << "pno is: " << r1.frames[c].pno << endl;
									cout << "offset is: " << r1.frames[c].page[d] << endl;
								}
								else
								{
									p = ((pn)+3) % 61;

									for (list<tlbdata>::iterator it = r1.frames[(*i).val.table].li[p].begin(); it != r1.frames[(*i).val.table].li[p].end(); it++)
									{
										if ((*it).pageno == pn)
										{
											cout << "tlb miss" << endl;
											cout << "id is: " << r1.frames[(*it).frameno].pid << endl;
											cout << "pno is: " << r1.frames[(*it).frameno].pno << endl;
											cout << "offset is: " << r1.frames[(*it).frameno].page[d] << endl;
											tlb1.tlbinsert(pn, (*it).frameno);
										}
									}
								}
							}
							else
							{
								tlb1.tlbflush();
								prev = a;
								p = ((pn)+3) % 61;

								for (list<tlbdata>::iterator it = r1.frames[(*i).val.table].li[p].begin(); it != r1.frames[(*i).val.table].li[p].end(); it++)
								{
									if ((*it).pageno == pn)
									{
										cout << "tlb miss" << endl;
										cout << "id is: " << r1.frames[(*it).frameno].pid << endl;
										cout << "pno is: " << r1.frames[(*it).frameno].pno << endl;
										cout << "offset is: " << r1.frames[(*it).frameno].page[d] << endl;
										tlb1.tlbinsert(pn, (*it).frameno);
									}
								}
							}
						}
					}
				}
			}
			else
				break;
		}
	}
}

class CircularLinkedList
{
public:
	repdata* head;
	repdata* ptr;
	int full;
	int size;
	void setval(int a)
	{
		head = NULL;
		size = a;

		for (int i = 0; i < a; i++)
		{
			rrdata temp;
			temp.rframe = i;
			temp.clock = 0;
			temp.pid = 0;
			temp.pno = -1;
			insertatend(temp);
			full = 0;
		}
		ptr = head;
	}
	void insertatend(rrdata a)
	{
		repdata* temp = new repdata;
		temp->data = a;
		temp->next = head;

		if (head == NULL)
		{
			head = temp;
			temp->next = temp;
		}
		else
		{
			repdata* current = head;
			while (current->next != head)
			{
				current = current->next;
			}
			current->next = temp;
		}
	}
	rrdata search(rrdata a)
	{
		repdata* current = NULL;

		if (head != NULL)
		{
			current = head->next;

			if (head->data == a)
			{
				head->data.clock = 1;
				return head->data;
			}
		}

		while (current != head)
		{
			if (current->data == a)
			{
				current->data.clock = 1;
				return current->data;
			}
			current = current->next;
		}
	}
	void update(rrdata a, rrdata b)
	{
		repdata* current = NULL;

		if (head != NULL)
		{
			current = head->next;

			if (head->data == a)
			{
				head->data = b;
			}
		}

		while (current != head)
		{
			if (current->data == a)
			{
				current->data = b;
			}
			current = current->next;
		}

		if (full != size)
			full++;
	}
	~CircularLinkedList()
	{
		repdata* current;
		if (head->next != head)
		{
			current = head->next;
			while (head != current)
			{
				repdata* temp = current->next;
				delete current;
				current = temp;
			}
		}

		delete head;
	}
	rrdata secondchance()
	{
		while (1)
		{
			if (ptr->data.clock == 1)
			{
				ptr->data.clock = 0;
				cout << "gave second chance to frame" << ptr->data.rframe << "pid and page: " << ptr->data.pid << " " << ptr->data.pno << endl;
				ptr = ptr->next;
			}
			else
			{
				cout << "snatched frame" << ptr->data.rframe << "pid and page: " << ptr->data.pid << " " << ptr->data.pno << endl;
				rrdata temp = ptr->data;
				ptr = ptr->next;
				return temp;
			}
		}
	}
};

class processrep
{
public:
	int id;
	int size;
	int* table;
	int pages;
	processrep* next;

	bool operator==(const processrep& rhs) const noexcept
	{
		 //logic here
		return this->id == rhs.id; // for example
	}
};

class repram
{
public:
	int size;
	int framesize;
	int* free;
	CircularLinkedList frames;
	int n;
	int fn;

	repram(int a = 16)
	{
		size = a;
		framesize = 64;
		int temp = 0;
		for (int i = a; i > 1; i /= 2)
		{
			temp++;
		}
		n = (pow(2, temp)) / 64;
		free = new int[n];
		for (int i = 0; i < n; i++)
		{
			free[i] = i;
		}
		random_shuffle(free, free + n - 1);
		fn = n - 1;
		frames.setval(n);
	}

	int getfreeframe()
	{
		if (fn != -1)
			return free[fn--];
		else
			return -1;
	}

	void newfreeframe(int temp)
	{
		free[++fn] = temp;
		frames.full--;
	}

	rrdata frameacess(int add)
	{
		rrdata in;
		in.rframe = add;
		return frames.search(in);
	}

	void frameinsert(rrdata a, int add)
	{
		rrdata in;
		in.rframe = add;
		a.rframe = add;
		frames.update(in, a);
	}
	~repram()
	{
		delete[] free;
	}
};

void function5()
{
	int size;
	cout << "\n\n-------Ram frame size is 64 Byte------\n";
	cout << "-----------Address Bus is of 16 Byte------\n";
	cout << "\nEnter the size of ram (64B - 655536B): \n";

	do
	{
		cout << "Enter your size: ";
		cin >> size;
	} while (size < 64 || size > 65536);

	repram r1(size);

	cout << "Ram size is: " << r1.size << "" << endl;
	cout << "No. of ram frames is: " << r1.n << endl;
	cout << "No of free frame in ram is: " << r1.fn + 1 << endl;

	list<processrep> l1;

	int choice;
	int ids = 1;

	int total = 0, faults = 0;

	while (1)
	{
		cout << "\n\n1. Insert Process\n";
		cout << "2. Remove Process\n";
		cout << "3. Access Process\n";
		cout << "4. to exit\n";

		cout << "Enter choice: ";
		cin >> choice;

		if (choice == 1)
		{
			processrep temp;

			int s;
			cout << "Enter size of process " << ids << " in Bytes : ";
			cin >> s;

			temp.id = ids;
			temp.size = s;

			if (s < 64)
				temp.pages = 1;
			else
				temp.pages = s / 64 + (0 != s % 64);

			temp.table = new int[temp.pages];

			cout << endl << endl;
			cout << "No of process pages: " << temp.pages << endl;

			temp.next = NULL;

			for (int i = 0; i < temp.pages; i++)
			{
				temp.table[i] = -1;
			}

			ids++;
			l1.push_back(temp);
		}
		else if (choice == 2)
		{
			int a;

			cout << "Enter the id of process: ";
			cin >> a;
			processrep temp;

			for (list<processrep>::iterator i = l1.begin(); i != l1.end(); i++)
			{
				if (a == (*i).id)
				{
					for (int j = 0; j < (*i).pages; j++)
					{
						if ((*i).table[j] != -1)
						{
							r1.newfreeframe((*i).table[j]);
							cout << "giving away frame " << (*i).table[j] << endl;
						}


					}

					cout << "Ram size is: " << r1.size << "KB" << endl;
					cout << "No. of ram frames is: " << r1.n << endl;
					cout << "No of free frame in ram is: " << r1.fn + 1 << endl;

					temp = *i;
				}
			}

			l1.remove(temp);
		}
		else if (choice == 3)
		{
			int a;

			cout << "Enter process number you want to access: ";
			cin >> a;

			for (list<processrep>::iterator i = l1.begin(); i != l1.end(); i++)
			{
				if (a == (*i).id)
				{
					int d, pn;

					cout << "Enter page number you want: ";
					cin >> pn;

					cout << "Enter offset: ";
					cin >> d;

					if (pn >= (*i).pages)
						cout << "invalid page\n";
					else if (pn == (*i).pages - 1 && (d >= (*i).size % 64 && (*i).size % 64 != 0) || d > 63)
						cout << "invalid offset\n";
					else
					{
						if ((*i).table[pn] == -1)
						{
							cout << "\n\nPage fault occured\n";
							int v = r1.getfreeframe();

							if (v == -1)
							{
								rrdata temp = r1.frames.secondchance();

								for (list<processrep>::iterator it = l1.begin(); it != l1.end(); it++)
								{
									if ((*it).id == temp.pid)
									{
										(*it).table[temp.pno] = -1;
									}
								}

								v = temp.rframe;
							}

							(*i).table[pn] = v;

							rrdata temp;
							temp.pid = a;
							temp.pno = pn;
							temp.rframe = v;
							temp.clock = 1;

							for (int k = 0; k < 64; k++)
							{
								temp.page[k] = k;
							}

							r1.frameinsert(temp, v);
							faults++;
						}
						else
						{
							cout << "\n\nno page fault\n";
						}

						rrdata temp;

						temp = r1.frameacess((*i).table[pn]);

						cout << "\n\nram frame is: " << temp.rframe << endl;
						cout << "pid is: " << temp.pid << endl;
						cout << "page number is: " << temp.pno << endl;
						cout << "offset is: " << temp.page[d] << endl;

						total++;
					}
				}
			}
		}
		else
			break;
	}

	cout << "\n\nTotal faults: " << faults << endl;
	cout << "\n\npage faults probability: " << ((double)faults / (double)total) << endl;
	cout << "\n\npage faults percentage: " << ((double)faults / (double)total) * 100 << "%" << endl;
}


int main()
{
	int choice = -1;
	while (choice)
	{
		cout << "================================OS SIMULATOR================================\n";
		cout << "1. CPU scheduling\n";
		cout << "2. Semaphore\n";
		cout << "3. Deadlock\n";
		cout << "4. Memory Management\n";
		cout << "5. Page Replacement\n";
		cout << "0. Exit\n";
		cout << "Enter Choice: ";
		cin >> choice;
		cout << "============================================================================\n";

		switch (choice)
		{
		case 1:
			cpumain();
			break;
		case 2:
			//semaphoreMain();
			break;
		case 3:
			deadlockMain();
			break;
		case 4:
			function4();
			break;
		case 5:
			function5();
			break;

		default:
			break;
		}
	}

	return 0;
}