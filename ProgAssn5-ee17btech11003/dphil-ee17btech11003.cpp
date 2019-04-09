#include <iostream>
#include <fstream>
#include <pthread.h> 

#include <random>
#include <stdio.h>
#include <unistd.h>
#include<mutex>
#include<chrono>
#include<ctime>
#include <condition_variable>

using namespace std;

ifstream in;

ofstream out;

pthread_mutex_t mtx;

double wait_time_w=0;	
double worst_time_w=0; 

int *state;

//condition_variable *self;
pthread_cond_t *self;

int n , h;

double ucs,urem;
double ran_expo(double mean){
    double u;
	u = rand() / (RAND_MAX + 1.0);
    return -log(1- u)*mean;
}
FILE* f=fopen("Phil-log.txt","w");

string getTime(time_t input)
{
  struct tm * timeinfo;
  timeinfo = localtime (&input);
  static char output[10];
  sprintf(output,"%.2d:%.2d:%.2d",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
  string tim(output);
  return tim;
}

int test(int i) {
	if ((state[(i + n - 1) % n] != 2) && (state[i] == 0) && (state[(i + 1) % n] != 2)) {
		state[i] = 2;
		pthread_cond_signal(&(self[i]));
		return 1;		
	}
	return 0;
}



//hungry=0    thinking=1     eating=2

void pickup(int i) {
	pthread_mutex_lock(&(mtx));
	state[i] = 0;
	if (!test(i)) {
       	pthread_cond_wait(&(self[i]), &(mtx));
   }
   pthread_mutex_unlock(&(mtx));
}

void putdown(int i) {
	pthread_mutex_lock(&(mtx));
	state[i] = 1;
	test((i + n - 1) % n);
	test((i + 1) % n);
	pthread_mutex_unlock(&(mtx));
}




void* writer(void *ptr)
{
	long id = (long) ptr;
	int Id=id+1;
	for (int i = 0; i < h; ++i)
	{
		time_t reqtime=time(NULL);
		fprintf(f,"%dth eat request by Philosopher Thread %d at %s\n", i+1,Id, (getTime(reqtime).c_str()));

		pickup(id);

		time_t entertime=time(NULL);
		wait_time_w=wait_time_w+(entertime-reqtime);
		if(entertime-reqtime>worst_time_w)
		{
			worst_time_w=entertime-reqtime;
		}
		fprintf(f,"%dth CS Entry by Philosopher thread %d at %s\n", i+1,Id, (getTime(entertime).c_str()));
		sleep(ran_expo(ucs));	

		putdown(id);
		
		time_t exittime=time(NULL);
		fprintf(f,"%dth CS Exit by Philosopher thread %d at %s\n", i+1,Id, (getTime(exittime).c_str()));
		sleep(ran_expo(urem));


	}
}




int main(int argc, char const *argv[])
{
	in.open("inp-params.txt");
	in>>n>>h>>ucs>>urem;
	in.close();
	pthread_t p[n];

	state=(int*)malloc(n*sizeof(int));
	self=(pthread_cond_t*)malloc(n*sizeof(pthread_cond_t));	

	for (int i = 0; i < n; ++i)
	{
		state[i] = 1;
		pthread_create(&p[i], NULL, writer, (void *)(intptr_t)i);
	}
	for (int i = 0; i < n; ++i)
	{
		//p[i].join();
		pthread_join ( p[i], NULL);
	}
	fclose(f);
	wait_time_w=wait_time_w/(n*h);

	FILE* f1=fopen("Time.txt","w");
	fprintf(f1,"Average waiting time: %f\n",wait_time_w);
	fprintf(f1,"Worst-case waiting time: %f\n",worst_time_w);
	fclose(f1);

	return 0;
}