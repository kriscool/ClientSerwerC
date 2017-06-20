
/*  Make the necessary includes and set up the variables.  */
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>	
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
const int i =1;
#define is_bigendian() ((*(char*)&i)==0)
char buffer[30];
double r;
int num[4] = {0,0,0,0};
int reqID=1;
time_t czas;
int flagendian;
void serializationRoot(){
	int i;
	char *c0 = (char*)&num[0];
	char *c1 = (char*)&num[1];
	char *c2 = (char*)&num[2];
	char *c3 = (char*)&num[3];
	char *req = (char *)&reqID;
	char *rr = (char *)&r;
	char *ti = (char *)&czas;
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
	if(num[3]== 1){
	for(i = 0;i<8;i++){
		buffer[20 + i] = rr[i];
	}}else{
	
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
	char *tt = (char *)&czas;
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
	}else{
		for(i = 0;i<8;i++){
	tt[i]=buffer[20+i];
	}
	}

}

int change_endianLitleToBig(int num){
if(flagendian ==0){
	int b0,b1,b2,b3;
	b0=(num & 0x000000FF)>>0;
	b1=(num & 0x0000FF00)>>8;
	b2=(num & 0x00FF0000)>>16;
	b3=(num & 0xFF000000)>>24;
	return((b0<<24)|(b1<<16)|(b2<<8)|(b3<<0));}else{return num;}

}

double reverseEndian(char *c){
	double i;
	char *p = (char *)&i;
	if(flagendian ==0){
		p[0]=c[7];
		p[1]=c[6];
		p[2]=c[5];
		p[3]=c[4];
		p[4]=c[3];
		p[5]=c[2];
		p[6]=c[1];
		p[7]=c[0];}else{
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
	
	

void clearBuff(){
	int i;
	for(i =0;i<256;i++){
		buffer[i]= '\0';	
	}
}


void printe(){

	printf("ReqID :%d ",reqID);
	printf("%d ",num[0]);
	printf("%d ",num[1]);
	printf("%d ",num[2]);
	printf("%d ",num[3]);
	if(num[3] == 1){
	printf("Root%f\n",r);
	}else{
	char * data;
	*&data = ctime(&czas);
	printf("Time:%s \n" ,*&data);
	}
	
}

void endian(){
	unsigned int i =1;
	char *c = (char*)&i;
	if(*c)
		flagendian = 0;
	else
		flagendian = 1;
}
void endianAll(){
	int i;
	for(i = 0;i<4;i++){
	 num[0]=change_endianLitleToBig(num[0]);
	}
	r = reverseEndian((char*)&r);
	reqID = change_endianLitleToBig(reqID);
}
void con(){
	int sockfd;
	socklen_t len;
	struct sockaddr_in address;
	int result;
	int i;
	
	endianAll();
	/*  Create a socket for the client.  */
	sockfd = socket (AF_INET, SOCK_STREAM, 0);
	/*  Name the socket, as agreed with the server.  */
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr ("127.0.0.1");
	address.sin_port = htons (9734);
	len = sizeof (address);
	/*  Now connect our socket to the server's socket.  */
	result = connect (sockfd, (struct sockaddr *) &address,len);
	if (result == -1)
	{
	perror ("oops: netclient");
	exit (1);
	}
	/*  We can now read/write via sockfd.  */
	serializationRoot();
	write (sockfd, &buffer, 30);
	clearBuff();
	read (sockfd, &buffer, 30);
	des();
	endianAll();
	printe();

	close (sockfd);
}

void czasss(){
	num[3]=2;
	con();
}

void pierwiastek(){
	num[3]=1;
	printf("Podaj liczbe");
	scanf("%lf",&r);
	con();
}


int main ()
{
	
	int choice;
	endian();
	
	
while(1){
	
	printf("Podaj co chcesz zrobić:\n 1:Pierwiastek z liczby \n 2:Czas\n 3:exit\n");
	scanf("%d",&choice);
		  switch(choice)
{
case 1:
   	pierwiastek();
	reqID++;
    break;
   
case 2:
    czasss();
		reqID++;
    break;
case 3:
    exit (0);
    break;
default:
        printf("Zla wartosc bye bye ");
		exit (0);
        break;
   
} 
	
}
	


}
