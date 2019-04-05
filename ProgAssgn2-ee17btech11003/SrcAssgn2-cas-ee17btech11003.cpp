#include<thread>
#include<atomic>
#include<ctime>
#include<iostream>
#include<sys/time.h>
#include<fstream>
#include<unistd.h>
#include<string>
#include<math.h>
#include<atomic>
using namespace std;
#define ta 2
#define tb 2

double lambda1,lambda2;
double ran_expo(double mean){
    double u;
	u = rand() / (RAND_MAX + 1.0);
    return -log(1- u)*mean;
}

ifstream in;
ofstream out;
double wait_time=0;	
double worst_time=0; 
int n,k;
FILE* f=fopen("CAS-Log.txt","w");

atomic<int> locki(0);
string getTime(time_t input)
{
  struct tm * timeinfo;
  timeinfo = localtime (&input);
  static char output[10];
  sprintf(output,"%.2d:%.2d:%.2d",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
  string tim(output);
  return tim;
}

void testCS(int a)
{
	int id=a;

	for(int i=0;i<k;i++)
	{
		time_t t0=time(NULL);

		fprintf(f,"%dth CS Request at %s by thread %d\n", i+1, (getTime(t0).c_str()),a );
		while(1){
		int x=0,y=1;
		if(locki.compare_exchange_strong(x,y)) break;
		}
		time_t t1=time(NULL);
		wait_time=wait_time+(t1-t0);
		if(t1-t0>worst_time)
		{
			worst_time=t1-t0;
		}
		fprintf(f,"%dth CS Entry at %s by thread %d\n", i+1, (getTime(t1).c_str()),a );
		sleep(ran_expo(lambda1));
		locki=0;
		time_t t2=time(NULL);
		fprintf(f ,"%dth CS Exit at %s by thread %d\n", i+1, (getTime(t2).c_str()),a );
		sleep(ran_expo(lambda2));

	}
}

int main(int argc, char const *argv[])
{
	
	in.open("inp-params.txt");
	in>>n>>k;
	in>>lambda1,lambda2;
	in.close();
	thread p[n];

	for(int i=0;i<n;i++)
	{
		p[i]=thread(testCS,i+1);
	}
	for(int i=0;i<n;i++)
	{
		p[i].join();
	}

	wait_time=(wait_time)/(k*n);
	//printf("%f\n",wait_time);
	//printf("%f\n",worst_time);
	fclose(f);
 
	return 0;
}