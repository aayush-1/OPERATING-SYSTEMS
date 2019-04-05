#include <iostream>
#include <fstream>
#include <thread>

#include <random>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include<chrono>
#include<ctime>

using namespace std;

ifstream ini;



double wait_time_w=0;	
double worst_time_w=0; 

double wait_time_r=0;	
double worst_time_r=0; 


double ucs,urem;
double ran_expo(double mean){
    double u;
	u = rand() / (RAND_MAX + 1.0);
    return -log(1- u)*mean;
}
int nw , nr,kw,kr ;

int ctrin=0;
int ctrout=0;
int wait=0;

sem_t in,out,rw_mutex;

FILE* f=fopen("FairRW-log.txt","w");

string getTime(time_t input)
{
  struct tm * timeinfo;
  timeinfo = localtime (&input);
  static char output[10];
  sprintf(output,"%.2d:%.2d:%.2d",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
  string tim(output);
  return tim;
}

int read_count=0; 


void writer(int id)
{
	int Id=id+1;
	for (int i = 0; i < kw; ++i)
	{
		time_t reqtime=time(NULL);
		fprintf(f,"%dth CS Request by Writer thread %d at %s\n", i+1,Id, (getTime(reqtime).c_str()));
		sem_wait(&in);
		sem_wait(&out);
		if (ctrin==ctrout)
		{
			sem_post(&out);
		}
		else
		{
			wait=1;
			sem_post(&out);
			sem_wait(&rw_mutex);
			wait=0;
		}
		time_t entertime=time(NULL);
		wait_time_w=wait_time_w+(entertime-reqtime);
		if(entertime-reqtime>worst_time_w)
		{
			worst_time_w=entertime-reqtime;
		}
		fprintf(f,"%dth CS Entry by Writer thread %d at %s\n", i+1,Id, (getTime(entertime).c_str()));
		sleep(ran_expo(ucs));
		sem_post(&in);
		time_t exittime=time(NULL);
		fprintf(f,"%dth CS Exit by Writer thread %d at %s\n", i+1,Id, (getTime(exittime).c_str()));
		sleep(ran_expo(urem));


	}
}


void reader(int id)
{
	int Id=id+1;

	for (int i = 0; i < kr; ++i)
	{
		time_t reqtime=time(NULL);
		fprintf(f,"%dth CS Request by Reader thread %d at %s\n", i+1,Id, (getTime(reqtime).c_str()));
		sem_wait(&in);
		ctrin++;
		sem_post(&in);

		time_t entertime=time(NULL);
		wait_time_r=wait_time_r+(entertime-reqtime);
		if(entertime-reqtime>worst_time_r)
		{
			worst_time_r=entertime-reqtime;
		}
		fprintf(f,"%dth CS Entry by Reader thread %d at %s\n", i+1,Id, (getTime(entertime).c_str()));
		sleep(ran_expo(ucs));
		sem_wait(&out);
		ctrout++;
		if(wait==1 && ctrin==ctrout)
		{
			sem_post(&rw_mutex);
		}
		sem_post(&out);
		time_t exittime=time(NULL);
		fprintf(f,"%dth CS Exit by Reader thread %d at %s\n", i+1,Id, (getTime(exittime).c_str()));
		sleep(ran_expo(urem));


	}
}

int main(int argc, char const *argv[])
{
	ini.open("inp-params.txt");
	ini>>nw>>nr>>kw>>kr>>ucs>>urem;
	ini.close();
	thread p[nw];
	thread c[nr];

	sem_init(&in,0,1);
	sem_init(&out,0,1);
	sem_init(&rw_mutex,0,0);
	for (int i = 0; i < nw; ++i)
	{
		p[i]=thread(writer,i);

	}
	for (int i = 0; i < nr; ++i)
	{	
		c[i]=thread(reader,i);
		
	}
	for (int i = 0; i < nw; ++i)
	{
		p[i].join();
	}
	for (int i = 0; i < nr	; ++i)
	{
		c[i].join();
	}

	fclose(f);
	wait_time_w=wait_time_w/(nw*kw);
	wait_time_r=wait_time_r/(nr*kr);

	FILE* f1=fopen("Average_time.txt","a");
	fprintf(f1,"Average time taken by Reader thread in Fair-RW Algorithm: %f\n",wait_time_r);
	fprintf(f1,"Average time taken by Writer thread in Fair-RW Algorithm: %f\n",wait_time_w);
	fclose(f1);

	return 0;
}