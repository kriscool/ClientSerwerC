#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
const int i =1;
#define is_bigendian() ((*(char*)&i)==0)
char buffer[256];
int num[4];
double r;
int reqID;
time_t timer;
int flagendian;

void serializationID(){
	int i;
	num[0]=1;
	char *c0 = (char*)&num[0];
	char *c1 = (char*)&num[1];
	char *c2 = (char*)&num[2];
	char *c3 = (char*)&num[3];
	char *req = (char *)&reqID;
		char *rr = (char *)&r;
	char *ti = (char *)&timer;
	for(i = 0;i<4;i++){
		buffer[0 + i] = c0[i];
	}
	
	for(i = 0;i<4;i++){
		buffer[4 + i] = c1[i];
	}
	
	for(i = 0;i<4;i++){
		buffer[8 + i] = c2[i];
	}
	
	for(i = 0;i<4;i++){
		buffer[12 + i] = c3[i];
	}
	
	for(i = 0;i<4;i++){
		buffer[16 + i] = req[i];
	}
	
	if(num[3] == 1){
			for(i = 0;i<8;i++){
		buffer[20 + i] = rr[i];
	}
	
	}else
	{
			for(i = 0;i<8;i++){
		buffer[20 + i] = ti[i];
	}
    
	}
		
}

void des(){
	int i;
	char *c0=(char*)&num[0];
	char *c1=(char*)&num[1];
	char *c2=(char*)&num[2];
	char *c3=(char*)&num[3];
	char *c4=(char*)&reqID;
	char *rr = (char *)&r;
	char *tt = (char *)&timer;
	for(i = 0;i<4;i++){
	c0[i]=buffer[i];
	}

	for(i = 0;i<4;i++){
	c1[i]=buffer[4+i];
	}

	for(i = 0;i<4;i++){
	c2[i]=buffer[8+i];
	}

	for(i = 0;i<4;i++){
	c3[i]=buffer[12+i];
	}

	for(i = 0;i<4;i++){
	c4[i]=buffer[16+i];
	}
	
	if(num[3] == 1){
	for(i = 0;i<8;i++){
	rr[i]=buffer[20+i];
	}
	}

}


int change_endianLitleToBig(int num){
if(flagendian == 0){
	int b0,b1,b2,b3;
	b0=(num & 0x000000FF)>>0;
	b1=(num & 0x0000FF00)>>8;
	b2=(num & 0x00FF0000)>>16;
	b3=(num & 0xFF000000)>>24;
	return((b0<<24)|(b1<<16)|(b2<<8)|(b3<<0));
}else{
return num;
}
}

double reverseEndian(char *c){
	double i;
	char *p = (char *)&i;
if(flagendian == 0){
		p[0]=c[7];
		p[1]=c[6];
		p[2]=c[5];
		p[3]=c[4];
		p[4]=c[3];
		p[5]=c[2];
		p[6]=c[1];
		p[7]=c[0];
}else{
		p[0]=c[0];
		p[1]=c[1];
		p[2]=c[2];
		p[3]=c[3];
		p[4]=c[4];
		p[5]=c[5];
		p[6]=c[6];
		p[7]=c[7];}
	
			   
		return i;
}

void endianAll(){
	int i;
	for(i = 0;i<4;i++){
	 num[0]=change_endianLitleToBig(num[0]);
	}
	r = reverseEndian((char*)&r);
	reqID = change_endianLitleToBig(reqID);
}

void endian(){
	unsigned int i =1;
	char *c = (char*)&i;
	if(*c)
		flagendian = 0;
	else
		flagendian = 1;
}
void getime(){
	timer =time(NULL);
}
int
main ()
{
	
int server_sockfd, client_sockfd;
socklen_t server_len, client_len;
struct sockaddr_in server_address;
struct sockaddr_in client_address;
	endian();
server_sockfd = socket (AF_INET, SOCK_STREAM, 0);
server_address.sin_family = AF_INET;
server_address.sin_addr.s_addr = htonl (INADDR_ANY);
server_address.sin_port = htons (9734);
server_len = sizeof (server_address);
bind (server_sockfd, (struct sockaddr *)
&server_address, server_len);
/*  Create a connection queue and wait for clients.  */
listen (server_sockfd, 5);
while (1)
{
//double ch;	
	char *ch;
printf ("server waiting\n");
/*  Accept connection.  */
client_len = sizeof (client_address);
client_sockfd = accept (server_sockfd,
(struct sockaddr *)
&client_address,
&client_len);
/*  We can now read/write to the client on
client_sockfd.
The five second delay is just for this
demonstration.  */
read (client_sockfd, &buffer, 256);
sleep (1);
des();
endianAll();
if(num[3] == 1){
	r = sqrt(r);
}else{
	getime();
}
endianAll();
serializationID();
write (client_sockfd, &buffer, 256);
close (client_sockfd);
}
}

