#include<pthread.h>
#include<bits/stdc++.h>
#include<math.h>
#include<unistd.h>
#include<sys/time.h>

using namespace std;

double ans_mean;
double ans_std_dev;
double ans_median;

void *mean(void *arg)
{
  double *data_mean=(double *)arg;
  double sum=0;
  for(int i = 1; i <= (int)data_mean[0]; i++)
  {
    sum += data_mean[i];
  }
  ans_mean = sum / (data_mean[0]);
  pthread_exit(0);
}

void *median(void *arg)
{
    double *data_median=(double *)arg;
    long n=data_median[0];
    double *median_data=(double *)malloc(sizeof(double)*(n+1));
    memcpy(median_data,data_median,sizeof(double)*(n+1));    
    sort(median_data+1,median_data+n+1);
    if(n%2 == 0)
        ans_median = (median_data[(n-1)/2] + median_data[n/2])/2.0;
    else
        ans_median = median_data[n/2];
    free(median_data);
    pthread_exit(0);
}

void *std_dev(void *arg)
{
  double *data_std_dev=(double *)arg;
  double sum=0,x=0;
  for(int i = 1; i <= (int)data_std_dev[0]; i++)
  {
    sum += data_std_dev[i];
  }
  double mean_t = sum / (data_std_dev[0]);
 
  for(int i=1; i<=(int)data_std_dev[0]; i++)
  	{
  		x+=pow(data_std_dev[i]-mean_t,2);
  	}
  ans_std_dev=sqrt(x/data_std_dev[0]);
  pthread_exit(0);
}

int main(int argc, char const *argv[])
{
	pthread_t thread1,thread2,thread3;
	long N;
	cin>>N;
	double *data=(double *)malloc(sizeof(double)*(N+1));
	data[0]=(double)N;
	for (int i = 1; i <= N; i++)
	{
		cin>>data[i];
	}
	struct timeval *t0,*t1;
	t0=(struct timeval *)malloc(sizeof(struct timeval));
  	t1=(struct timeval *)malloc(sizeof(struct timeval));
	int iret1, iret2,iret3;
	gettimeofday(t0 , NULL);
	iret1 = pthread_create( &thread1, NULL, mean, (void*)data);
    iret2 = pthread_create( &thread2, NULL, median, (void*)data);
    iret3 = pthread_create( &thread3, NULL, std_dev, (void*)data);
    pthread_join( thread1, NULL);
    pthread_join( thread2, NULL);
    pthread_join( thread3, NULL);
    gettimeofday(t1 , NULL);
	cout<<"The average value is "<<ans_mean<<endl;
	cout<<"The standard deviation value is "<<ans_std_dev<<endl;
    cout<<"The median value is "<<ans_median<<endl;
    double time = (t1->tv_sec - t0->tv_sec) + (t1->tv_usec - t0->tv_usec) / 1000000.0;
  	//cout<<"Elapsed time: "<<time<<endl;
  	free(t0);
  	free(t1);
  	free(data);
	return 0;
}
