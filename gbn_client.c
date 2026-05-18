#include<stdio.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/time.h>
#include<sys/socket.h>
#include<string.h>

#define FRAMES 11
#define WINDOW 4

int main(){
	
	int sock=socket(AF_INET,SOCK_DGRAM,0);
	
	struct sockaddr_in serv;
	memset(&serv,0,sizeof(serv));
	
	serv.sin_family=AF_INET;
	serv.sin_port=htons(8000);
	serv.sin_addr.s_addr=inet_addr("127.0.0.1");
	
	struct timeval tv={1,0};
	
	setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof(tv));
	
	int base=0; //first unacknowledged frame
	int next_seq=0;
	char buf[10];
	
	while (base<FRAMES){
		
		while(next_seq<base+WINDOW && next_seq<FRAMES){
			printf("Sending frame %d\n",next_seq);
			sprintf(buf,"%d",next_seq);
			sendto(sock,buf,strlen(buf),0,(struct sockaddr*)&serv,sizeof(serv));
			next_seq++;
		}
		
		int n=recvfrom(sock,buf,sizeof(buf),0,NULL,NULL);
		
		if(n>0){
			buf[n]='\0';
			int ack_num=atoi(buf);
			printf("ACK received for frame %d \n",ack_num);
			
			if(ack_num >= base){
				base=ack_num+1;
			}
		}
		else{
			next_seq=base;
		}
	}
	close(sock);
	return 0;
}
