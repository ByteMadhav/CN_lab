#include<stdio.h>

int main(){
	int in,out,n,bsize,psize,bucket=0;
	
	printf("Enter total bucket size:");
	scanf("%d",&bsize);
	
	printf("Enter number of packets:");
	scanf("%d",&n);
	
	printf("Enter outgoing rate:");
	scanf("%d",&out);
	
	while(n){
		printf("Enter incoming packet size:");
		scanf("%d",&psize);
		
		if(psize<= (bsize-bucket)){
			printf("Incoming packet with size %d\n",psize);
			bucket+=psize;
			printf("Bucket status: %d out of %d\n",bucket,bsize);
		}
		else{
			printf("Packet is too big to fit\n");
			printf("Packet is dropped!!\n");
		}
		
		bucket-=out;
		
		if(bucket<=0){
			bucket=0;
		}
		
		printf("After outgoing bucket status:%d out of %d\n",bucket,bsize);
		
		n-=1;
	}
}
