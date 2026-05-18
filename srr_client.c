#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/time.h>
#include<sys/socket.h>

#define FRAMES 12
#define WINDOW 4

int main(){

	int serv_sock=socket(AF_INET,SOCK_DGRAM,0);
	struct sockaddr_in serv_addr;
	struct timeval tv={1,0};
	
	memset(&serv_addr,0,sizeof(serv_addr));
	
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(8000);
	serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	
	
	setsockopt(serv_sock,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof(tv));
	
	int base=0;
	int ack[FRAMES]={0};
	int sent[FRAMES]={0};
	char buf[10];
	
	while(base<FRAMES){
		for(int seq=base; seq<base+WINDOW && seq<FRAMES ; seq++){
			if(!sent[seq]){
				sprintf(buf,"%d",seq);
				sendto(serv_sock,buf,strlen(buf),0,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
				printf("Sent frame %d\n",seq);
				sent[seq]=1;
				
			}
		}
		
		int n=recvfrom(serv_sock,buf,sizeof(buf),0,NULL,NULL);
		
		if (n>0){
			//ACK received
			buf[n]='\0';
			int ack_num=atoi(buf);
			ack[ack_num]=1;
			printf("Ack received for frame %d\n",ack_num);
			
			while(base<FRAMES && ack[base]==1){
				base++;
			}
		}
		else{
		
			
			for(int seq=base; seq<base+WINDOW && seq<FRAMES ; seq++){
				if(!ack[seq]){
					sprintf(buf,"%d",seq);
					sendto(serv_sock,buf,strlen(buf),0,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
					printf("Resent frame %d\n",seq);
					sent[seq]=1;
				
				}
			}
		}
	}
	
	close(serv_sock);
	return 0;
}
