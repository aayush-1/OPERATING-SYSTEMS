#include<bits/stdc++.h>
#include<sys/time.h>
#include<sys/wait.h>
#include<unistd.h> 
#include<sys/mman.h>
#include<math.h>

using namespace std;

double *ans_mean;
double *ans_std_dev;
double *ans_median;

int main(int argc, char const *argv[])
{
	long N;
	cin >> N;
	double *data=(double *)malloc(sizeof(double)*(N+1));
	data[0]=(double)N;
	for (int i = 1; i <= N; i++)
	{
		cin>>data[i];
	}
	
	struct timeval *t0,*t1;
	t0=(struct timeval *)malloc(sizeof(struct timeval));
  	t1=(struct timeval *)malloc(sizeof(struct timeval));
  	int pid, pid1, pid2;
  	ans_mean=(double *)mmap(NULL,sizeof(double*), PROT_READ | PROT_WRITE| PROT_EXEC,MAP_SHARED | MAP_ANON ,-1, 0);
	ans_std_dev=(double *)mmap(NULL,sizeof(double*), PROT_READ | PROT_WRITE| PROT_EXEC,MAP_SHARED | MAP_ANON ,-1, 0);
	ans_median=(double *)mmap(NULL,sizeof(double*), PROT_READ | PROT_WRITE| PROT_EXEC,MAP_SHARED | MAP_ANON ,-1, 0);
  	pid = fork();
  	gettimeofday(t0 , NULL);
  	if (pid == 0) {
  		//first child
  		double sum=0;
  		for(int i = 1; i <= (int)data[0]; i++)
		{
		  sum += data[i];
		}
		*ans_mean = sum / (data[0]);
        } 
      
        else {
            pid1 = fork(); 
            if (pid1 == 0) { 
            	//second child
            	double *median_data=(double *)malloc(sizeof(double)*(N+1));
            	memcpy(median_data,data,sizeof(double)*(N+1));
            	int n=data[0];
            	sort(median_data+1,median_data+N+1);            	
		        if(n%2 == 0)
			        *ans_median=(median_data[n/2]+median_data[n/2+1])/2.0;
			    else
			        *ans_median=median_data[n/2+1];
			    free(median_data);
			
            } 
            else {
                pid2 = fork();
                
                if (pid2 == 0) { 
                	//third child
                	double sum=0,x=0;
					for(int i = 1; i <= (int)data[0]; i++)
			        {
					  sum += data[i];
					}
					double mean_t = sum / (data[0]);
					 
					for(int i=1; i<=(int)data[0]; i++)
					 {
					   x+=pow(data[i]-mean_t,2);
					 }
					*ans_std_dev=sqrt(x/data[0]);
                } 
                else {
                	//parent
                	wait(NULL);
                	wait(NULL);
                	wait(NULL);
                	gettimeofday(t1 , NULL);
                	float time = (t1->tv_sec - t0->tv_sec) + (t1->tv_usec - t0->tv_usec) / 1000000.0;
				    //cout<<"Elapsed time: "<<time<<endl;
				    cout<<"The average value is "<<*ans_mean<<endl;
				    cout<<"The standard deviation value is "<<*ans_std_dev<<endl;
				    cout<<"The median value is "<<*ans_median<<endl;
				    munmap(ans_mean, sizeof(double*));
				    munmap(ans_std_dev, sizeof(double*));
				    munmap(ans_median, sizeof(double*));
				    delete(t0);
        			delete(t1); 
        			free(data);
                } 
            } 
        }

	return 0;
}