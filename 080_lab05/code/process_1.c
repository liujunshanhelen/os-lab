#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>
#include <sys/wait.h>
#include <sys/types.h>

#define HZ	100

void cpuio_bound(int last, int cpu_time, int io_time);

int main(int argc, char * argv[])
{
	pid_t father,child_1,child_2,son3,tmp1,tmp2,tmp3;
	tmp1=fork();
	if(tmp1==0)			/* child_1 */
	{
		child_1=getpid();
		printf("The child_1's pid:%d\n",child_1);
		printf("I am child_1\n");
		cpuio_bound(10, 3, 2);
		printf("child_1 is finished\n");
	}
	else if(tmp1>0)
	{
		child_1=tmp1;
		tmp2=fork();
		if(tmp2==0)		
		{
			child_2=getpid();
			printf("The child_2's pid:%d\n",child_2);
			printf("I am child_2\n");
			cpuio_bound(5, 1, 2);
			printf("child_2 is finished\n");
		}
		else if(tmp2>0)		
		{
			child_2=tmp2;
			father=getpid();
			printf("The father get child_1's pid:%d\n",tmp1);
			printf("The father get child_2's pid:%d\n",tmp2);
			wait((int *)NULL);
			wait((int *)NULL);
			printf("Now is the father's pid:%d\n",father);
		}
		else
			printf("Creat child_2 failed\n");
	}
	else
		printf("Creat child_1 failed\n");
	return 0;
}
void cpuio_bound(int last, int cpu_time, int io_time)
{
	struct tms start_time, current_time;
	clock_t utime, stime;
	int sleep_time;

	while (last > 0)
	{
		times(&start_time);
		do
		{
			times(&current_time);
			utime = current_time.tms_utime - start_time.tms_utime;
			stime = current_time.tms_stime - start_time.tms_stime;
		} while ( ( (utime + stime) / HZ )  < cpu_time );
		last -= cpu_time;

		if (last <= 0 )
			break;
		sleep_time=0;
		while (sleep_time < io_time)
		{
			sleep(1);
			sleep_time++;
		}
		last -= sleep_time;
	}
}
