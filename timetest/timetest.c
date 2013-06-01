#include <stdio.h>
#include <sys/time.h>

printtime(struct timespec *t)
{
printf("secs : %8d,  nsecs   :  %8d \n ",t->tv_sec,t->tv_nsec);
}



struct timespec *diff_time(struct timespec *t1,struct timespec *t2)
{
static struct timespec t;
t.tv_sec = t1->tv_sec - t2->tv_sec;
t.tv_nsec = t1->tv_nsec - t2->tv_nsec;
 return &t;
}






int main(void)
{
struct timespec t1 , t2 ,t3  ;

clock_gettime(CLOCK_MONOTONIC,&t1);
clock_gettime(CLOCK_MONOTONIC,&t2);
printtime(&t1);
printtime(&t2);
printtime(diff_time(&t2,&t1));

clock_gettime(CLOCK_MONOTONIC,&t3);
printtime(&t3);
printtime(diff_time(&t3,&t2));


printf("clock_gettime(CLOCK_MONOTONIC,&t2);\nsleep(1);\nclock_gettime(CLOCK_MONOTONIC,&t3);\n");



clock_gettime(CLOCK_MONOTONIC,&t2);
sleep(1);
clock_gettime(CLOCK_MONOTONIC,&t3);

printtime(diff_time(&t3,&t2));
}
