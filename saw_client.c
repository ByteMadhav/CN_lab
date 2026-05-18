#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<arpa/inet.h>
#include<string.h>

#define FRAMES 11
int main(){
	int sock=socket(AF_INET,SOCK_DGRAM,0);
	
	struct sockaddr_in serv;
	memset(&serv,0,sizeof(serv));
	serv.sin_family=AF_INET;
	serv.sin_port=htons(8000);
	serv.sin_addr.s_addr=inet_addr("127.0.0.1");
	
	struct timeval tv={1,0};
	setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof(tv));
	
	int seq=0;
	char buf[10];
	while(seq<FRAMES){
		sprintf(buf,"%d",seq);
		sendto(sock,buf,strlen(buf),0,(struct sockaddr*)&serv,sizeof(serv));
		printf("Frame %d sent\n",seq);
		
		int n=recvfrom(sock,buf,sizeof(buf),0,NULL,NULL);
		
		if(n>0){
			buf[n]='\0';
			int frame=atoi(buf);
			if(frame==seq){
				printf("ACK received for frame %d\n",frame);
				seq++;			
			}
		}
		else{
			printf("ACK not received, retransmitting frame %d\n",seq);
		}
	}
	
	close(sock);
	return 0;
	
}
