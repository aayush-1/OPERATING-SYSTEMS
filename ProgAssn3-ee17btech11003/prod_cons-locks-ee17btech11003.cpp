#include <iostream>
#include <fstream>
#include <thread>

#include <random>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include<mutex>
#include<ctime>

using namespace std;

ifstream in;

ofstream out;

double wait_time_p=0;	
double avg_time_p=0;


double wait_time_c=0;	
double avg_time_c=0;

double up,uc;
double ran_expo(double mean){
    double u;
	u = rand() / (RAND_MAX + 1.0);
    return -log(1- u)*mean;
}

int np , nc ;
int cntp,cntc , capac;

mutex cap , mtx;
FILE* f=fopen("prod_cons-locks-LOG.txt","w");


string getTime(time_t input)
{
  struct tm * timeinfo;
  timeinfo = localtime (&input);
  static char output[10];
  sprintf(output,"%.2d:%.2d:%.2d",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
  string tim(output);
  return tim;
}
int a=-1;

void producer(int id)
{
	int ID=id+1;

	
	for (int i = 0; i < cntp; ++i)
	{
		time_t t0=time(NULL);
		while(1){
		cap.lock();
			if(a<capac){
				a++;
				time_t t1=time(NULL);
				wait_time_p=wait_time_p+(t1-t0);
				fprintf(f,"%dth item produced by thread %d at %s into buffer location %d\n", i+1,ID+1, (getTime(t1).c_str()),a );
				cap.unlock();
				break;
			}

		

		cap.unlock();
		}
   		usleep(ran_expo(up));
	}
}

void consumer(int id)
{
	int ID=id+1;

	for (int i = 0; i < cntc; ++i)
	{
		time_t t0=time(NULL);
		while(1){
		cap.lock();
			if(a>-1){
				a--;
				time_t t1=time(NULL);
				wait_time_c=wait_time_c+(t1-t0);
				fprintf(f,"%dth item consumed by thread %d at %s into buffer location %d\n", i+1,ID+1, (getTime(t1).c_str()),a+1 );
				cap.unlock();
				break;
			}

		
		cap.unlock();
		}

		usleep(ran_expo(uc));
	}
}







int main ( )
{
	in.open("inp-params.txt");
	in>>capac>>np>>nc>>cntp>>cntc>>up>>uc;
	in.close();
	thread p[np];
	thread c[nc];

	for (int i = 0; i < np; ++i)
	{
		p[i]=thread(producer,i);

	}
	for (int i = 0; i < nc; ++i)
	{	
		c[i]=thread(consumer,i);
		
	}
	for (int i = 0; i < np; ++i)
	{
		p[i].join();
	}
	for (int i = 0; i < nc; ++i)
	{
		c[i].join();
	}

	fclose(f);
	avg_time_p=wait_time_p/(np*cntp);
	avg_time_c=wait_time_c/(np*cntc);

	return 0;
}



