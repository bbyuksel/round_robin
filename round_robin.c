#include <stdio.h>
#include <stdlib.h>
typedef struct {
	int burst;
	int wait;
	int finished;
}PROCESS;

void round_robin(FILE *fp,PROCESS a[],int n,int q,int time )
{	
	int x,j;
	int timeCancelled =0;
	int sayac=0;
	double avg=0;
	
	for(x=0;x<n;x++)
	{
		if(a[x].burst==0) sayac++;		
	}
	if(sayac==n-1)
	{
		timeCancelled = 1;
	}		
	for(x=0;x<n;x++)
	{
		if(a[x].burst!=0)
		{		
			if(timeCancelled != 1)
			{
				a[x].wait+=time - a[x].finished;
			}
				
			if(a[x].burst>=q)
			{
				printf("P%d : %d-%d\n",x+1,time,time+q);
				fprintf(fp,"P%d : %d-%d\n",x+1,time,time+q);
				a[x].burst-=q;
				time+=q;
			}
			else
			{
				printf("P%d : %d-%d\n",x+1,time,time+a[x].burst);			
				fprintf(fp,"P%d : %d-%d\n",x+1,time,time+a[x].burst);			
				time+=a[x].burst;
				a[x].burst=0;
			}
			
			a[x].finished = time;
		}
	}
	
	sayac=0;	
	for(x=0;x<n;x++)
	{
		if(a[x].burst==0) sayac++;		
	}
	if(sayac==n)
	{
		for(x=0;x<n;x++)
		{
			avg+=a[x].wait;
		}
		printf("AVG TIME : %.2f msn",avg/n);
		fprintf(fp,"AVG TIME : %.2f msn",avg/n);
		
	}
	else
	{
		round_robin(fp,a,n,q,time);
	}
}
int main(void)
{
	FILE *fp;
	PROCESS *p;
	int n,i,q,time=0;
	double avg=0;
	printf("Process number: ");
	scanf("%d",&n);
	p=(PROCESS*)malloc(sizeof(PROCESS)*n);	
	for(i=0;i<n;i++)
	{
		printf("P-%d: ",i+1);
		p[i].wait=0;
		scanf("%d",&p[i].burst);
		p[i].finished = 0;
		p[i].wait =0;
	}
	printf("Quantum time: ");
	scanf("%d",&q);
	
	fp=fopen("cikti.txt","w");
	round_robin(fp,p,n,q,time);
	fclose(fp);
}
