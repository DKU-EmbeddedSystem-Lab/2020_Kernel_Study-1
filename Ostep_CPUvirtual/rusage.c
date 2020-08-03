#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main (){


	struct rusage * my_r = (struct rusage *)malloc(sizeof(struct rusage));
	int w = RUSAGE_SELF;

	unsigned int a =10;
	unsigned int b =69990000;
	for(; a<b; a+=1);
//	yield();
	if(getrusage(w , my_r) <0)
		write(STDERR_FILENO, "EXperiment FAILED\n\0", 64);
	else{
		printf("struct timeval\n");
		printf("  ru_utime->sec   %ld\n",my_r->ru_utime.tv_sec);
		printf("  ru_utime->usec  %ld\n",my_r->ru_utime.tv_usec);
		printf("page fault : %ld\n", my_r->ru_majflt);
		printf("voluntary context switches  	%ld\n", my_r->ru_nvcsw);
		printf("involuntary context switches	%ld\n", my_r->ru_nivcsw);
	
	}
}