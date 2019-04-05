#include<iostream>
#include<cstdlib>
using namespace std;
#include<queue>
#include<fstream>
#include<string>
#include<algorithm>
#include<bits/stdc++.h>


class node
{
   public:
   string process;
   int period;
   int running_time;
   int k;
   int deadline;
   int executed_time;
   int arrival_time;

   bool operator< (const node& p1)
	{
	return period<p1.period;
	}

};
class node *vertex;


ifstream in;
ofstream out,out1;


int main()
{
	in.open("inp-params.txt");
	int n;
	in>>n;
	vertex=(class node*)malloc(sizeof(class node)*(n)); 
	for(int i=0;i<n;i++)
	{
		string a;
		in>>a;
		vertex[i].process=a;

		in>>vertex[i].running_time;

		in>>vertex[i].period;

		in>>vertex[i].k;

		vertex[i].deadline=vertex[i].period;
		vertex[i].arrival_time=0;
		vertex[i].executed_time=0;
	}
	in.close();

	sort(vertex,vertex+n);
	
	int total_task=0;
	int completed_task=0;
	int missed_deadline_task=0;
	double net_waiting_time=0;
	out.open("RM-Log.txt");
	int current_task;
	int previous_task=1;
	int inactive=0;
	int end_time=0;

	for(int i=0;i<n;i++)
	{
		if(end_time<vertex[i].period*vertex[i].k)
		{
			end_time=vertex[i].period*vertex[i].k;
		}
		out<<"Process "<<vertex[i].process<<": processing time="<<vertex[i].running_time<<"; deadline:"<<vertex[i].deadline<<"; period:"<<vertex[i].period<<" joined the system at time 0"<<endl;
	}

	
	for(int time=1;time<=end_time;time++)
	{
		current_task=-1;
		int i=0;
		while(i<n)
		{
			int a;
			if(time<vertex[i].k*vertex[i].period)
			{
				a=time;
			}
			else if(time>=vertex[i].k*vertex[i].period)
			{
				a=vertex[i].k*vertex[i].period;
			}

			if(vertex[i].arrival_time<a)
			{
				if(inactive==1)
				{
					out<<"CPU is idle till time "<<time-1<<endl;
					inactive = 0;
				}
				if(previous_task!=i)
				{
					if(vertex[previous_task].executed_time!=0)
					{
						out<<"Process "<<vertex[previous_task].process<<" is preempted by Process "<<vertex[i].process<<" at time "<<time -1<<". Remaining processing time:"<<vertex[previous_task].running_time-vertex[previous_task].executed_time<<endl;
					}
					if(vertex[i].executed_time == 0)
					{
						out<<"Process "<<vertex[i].process<<" starts execution at time "<<time<<endl;
					}
					if(vertex[i].executed_time!=0)
					{
						out<<"Process "<<vertex[i].process<<" resumes its execution at time "<<time<<endl;
					}
				}
				else
				{
					if(vertex[previous_task].executed_time == 0)
					{
						out<<"Process "<<vertex[i].process<<" starts execution at time "<<time<<endl;
					}
				}
				current_task=i;
				previous_task=i;
				
				break;

			}
			i++;
		}

		if(current_task>-1)
		{
			
			vertex[current_task].executed_time++;
			if(vertex[current_task].executed_time==vertex[current_task].running_time)
			{
				out<<"Process "<<vertex[current_task].process<<" finishes execution at time "<<time<<endl;
				completed_task++;
				net_waiting_time += time-vertex[current_task].arrival_time-vertex[current_task].executed_time;
				vertex[current_task].arrival_time=vertex[current_task].arrival_time+vertex[current_task].period;
				vertex[current_task].deadline=vertex[current_task].deadline+vertex[current_task].period;

				vertex[current_task].executed_time=0;
			}	
		}

		for(int j=0;j<n;j++)
		{
			if(vertex[j].deadline <= time)
			{
				if(vertex[j].arrival_time/vertex[j].period<vertex[j].k)
				{
					out<<"Process "<<vertex[j].process<<" missed its deadline at time "<<time<<endl;
					vertex[j].executed_time = 0;
					missed_deadline_task ++;
					net_waiting_time+=time-vertex[j].arrival_time-vertex[j].executed_time;
					vertex[j].arrival_time=vertex[j].arrival_time+vertex[j].period;
					vertex[j].deadline=vertex[j].deadline+vertex[j].period;
				}

				
			}
		}

		if(i==n)
		{
			inactive=1;
		}

	}
	out.close();
	for(int i=0;i<n;i++)
	{
		total_task = total_task + vertex[i].k;
	}

	out1.open("RM-Stats.txt");
	out1<<"Number of processes that came into the system: "<<total_task<<endl;
	out1<<"Number of processes that successfully completed: "<<completed_task<<endl;
	out1<<"Number of processes that missed their deadlines: "<<missed_deadline_task<<endl;
	out1<<"Average waiting time = "<<(net_waiting_time)/total_task<<endl;
	out1.close();









	return 0;
}

