#include<stdio.h>

struct router{
	unsigned cost[20];
	unsigned from[20];
}rt[10];

int main(){
	int costmat[20][20];
	int n,i,j,k,routers;
	
	printf("Enter number of routers:");
	scanf("%d",&routers);
	
	printf("Enter cost matrix:\n");
	for(i=0;i<routers;i++){
		for(j=0;j<routers;j++){
			scanf("%d",&costmat[i][j]);
			costmat[i][i]=0;
			rt[i].cost[j]=costmat[i][j];
			rt[i].from[j]=j;
		}
	}
	
	for(i=0;i<routers;i++){
		printf("\n");
		for(j=0;j<routers;j++){
			printf("%d\t",rt[i].cost[j]);
		}
	}
	int count;
	do{
		count=0;
		for(i=0;i<routers;i++){
			for(j=0;j<routers;j++){
				for(k=0;k<routers;k++){
					if(rt[i].cost[j]>costmat[i][k]+rt[k].cost[j]){
						rt[i].cost[j]=costmat[i][k]+rt[k].cost[j];
						rt[i].from[j]=k;
						count++;
					}
				}
			}
		}
	}while(count!=0);
	for(i=0;i<routers;i++){
		printf("\n For router %d\n",i+1);
		for(j=0;j<routers;j++){
			printf("\n Router %d via %d distance:%d",j+1,(rt[i].from[j])+1,rt[i].cost[j]);
		}
		printf("\n");
	}
}
