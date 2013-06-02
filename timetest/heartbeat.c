#include <stdio.h>
#include <sys/time.h>

printtime(struct timespec *t)
{
printf("secs : %8d,  nsecs   :  %8d \n ",t->tv_sec,t->tv_nsec);
}


copytime(struct timespec *t1,struct timespec *t2)//copy t1 -> t2
{
t2->tv_sec = t1->tv_sec;
t2->tv_nsec = t1->tv_nsec;
}



struct timespec *diff_time(struct timespec *t1,struct timespec *t2)
{
static struct timespec t;
t.tv_nsec = t1->tv_nsec - t2->tv_nsec;
t.tv_sec = t1->tv_sec - t2->tv_sec;

if (t.tv_nsec < 0)
	{
	t.tv_sec++;
	t.tv_nsec += 1000000000;
	}
 return &t;
}






int main(void)
{
struct timespec t1 , t2 ,t3  ;

clock_gettime(CLOCK_MONOTONIC,&t1);
printtime(&t1);

while(1)
	{
	do 
	  clock_gettime(CLOCK_MONOTONIC,&t2);
	while( diff_time(&t2,&t1)->tv_nsec < 1000000);
	printtime(&t2);
	copytime(&t2,&t1);//copy t2 -> t1
	}
	
}
