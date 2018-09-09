#include <stdio.h>
#include <winsock2.h>
#include <mstcpip.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <windows.h>
#include <process.h>

#define TCP 6
#define UDP 17

#pragma comment(lib,"ws2_32.lib")

typedef struct tTHREAD {
	SOCKET sniffer;
	int sel;
}THREAD;

unsigned WINAPI StartSniffing(void *p);			//This will sniff here and there
void selectMode(SOCKET snifferH, SOCKET snifferT, SOCKET snifferF, SOCKET snifferD);
//HTTP, Telnet, FTP, DNS 중 캡처 하고자 하는 모드 선택

void PrintHTTP(char* Buffer, int Size);				//HTTP의 정보 일 경우 출력하는 함수
void PrintTelnet(char* Buffer, int Size);			//Telnet의 정보 일 경우 출력하는 함수
void PrintFTP(char* Buffer, int Size);				//FTP의 정보 일 경우 출력하는 함수
void PrintUDP_DNS(char* Buffer, int Size);			//DNS의 정보 일 경우 출력하는 함수
void PrintData(char*, int Size, FILE *logfile);		//헤더를 제외 한 실질적인 데이터 부분을 출력하는 함수

void PrintTCP(char* Buffer, int iphdrlen, int Size, FILE *file);
void PrintUDP(char *Buffer, int iphdrlen, int Size, FILE *file);

void gotoxy(int x, int y);

typedef struct ip_hdr
{
	unsigned char ip_header_len : 4; // 4-bit header length (in 32-bit words) normally=5 (Means 20 Bytes may be 24 also)
	unsigned char ip_version : 4; // 4-bit IPv4 version
	unsigned char ip_tos; // IP type of service
	unsigned short ip_total_length; // Total length
	unsigned short ip_id; // Unique identifier

	unsigned char ip_frag_offset : 5; // Fragment offset field

	unsigned char ip_more_fragment : 1;
	unsigned char ip_dont_fragment : 1;
	unsigned char ip_reserved_zero : 1;

	unsigned char ip_frag_offset1; //fragment offset

	unsigned char ip_ttl; // Time to live
	unsigned char ip_protocol; // Protocol(TCP,UDP etc)
	unsigned short ip_checksum; // IP checksum
	unsigned int ip_srcaddr; // Source address
	unsigned int ip_destaddr; // Source address
} IPV4_HDR;
//IPV_4 구조

typedef struct udp_hdr
{
	unsigned short source_port; // Source port no.
	unsigned short dest_port; // Dest. port no.
	unsigned short udp_length; // Udp packet length
	unsigned short udp_checksum; // Udp checksum (optional)
} UDP_HDR;
//UDP 구조

// TCP header
typedef struct tcp_header
{
	unsigned short source_port; // source port
	unsigned short dest_port; // destination port
	unsigned int sequence; // sequence number - 32 bits
	unsigned int acknowledge; // acknowledgement number - 32 bits

	unsigned char ns : 1; //Nonce Sum Flag Added in RFC 3540.
	unsigned char reserved_part1 : 3; //according to rfc
	unsigned char data_offset : 4; /*The number of 32-bit words in the TCP header.
								   This indicates where the data begins.
								   The length of the TCP header is always a multiple
								   of 32 bits.*/

	unsigned char fin : 1; //Finish Flag
	unsigned char syn : 1; //Synchronise Flag
	unsigned char rst : 1; //Reset Flag
	unsigned char psh : 1; //Push Flag
	unsigned char ack : 1; //Acknowledgement Flag
	unsigned char urg : 1; //Urgent Flag

	unsigned char ecn : 1; //ECN-Echo Flag
	unsigned char cwr : 1; //Congestion Window Reduced Flag


	unsigned short window; // window
	unsigned short checksum; // checksum
	unsigned short urgent_pointer; // urgent pointer
} TCP_HDR;
//TCP 구조

FILE *logfile_HTTP, *logfile_Telnet, *logfile_TCP_FTP, *logfile_UDP_FTP, *logfile_UDP_DNS, *logfile_TCP_DNS;
int HTTPtotal = 0, Telnettotal = 0, FTPtotal = 0, DNStotal = 0;

char hex[2];

int off = 0;

int main()
{
	struct sockaddr_in dest;
	SOCKET snifferH, snifferT, snifferF, snifferD;
	struct in_addr addr;
	char in[50];
	int in_Num, i, j;

	char hostname[100];
	struct hostent *local;	//hostent는 호스트에 대한 정보를 가지고 있는 구조체
	WSADATA wsa;

	printf("Creating log file...\n");

	//각각의 패킷에 대한 저장 공간을 할당.
	logfile_HTTP = fopen("log_HTTP.txt", "w+");
	if (logfile_HTTP == NULL)
	{
		perror("Unable to create file.");
	}

	logfile_Telnet = fopen("log_Telnet.txt", "w+");
	if (logfile_Telnet == NULL)
	{
		perror("Unable to create file.");
	}

	logfile_TCP_FTP = fopen("log_TCP_FTP.txt", "w+");
	if (logfile_TCP_FTP == NULL)
	{
		perror("Unable to create file.");
	}

	logfile_UDP_FTP = fopen("log_UDP_FTP.txt", "w+");
	if (logfile_UDP_FTP == NULL)
	{
		perror("Unable to create file.");
	}

	logfile_UDP_DNS = fopen("log_UDP_DNS.txt", "w+");
	if (logfile_UDP_DNS == NULL)
	{
		perror("Unable to create file.");
	}

	logfile_TCP_DNS = fopen("log_TCP_DNS.txt", "w+");
	if (logfile_TCP_DNS == NULL)
	{
		perror("Unable to create file.");
	}

	printf("Creating log_file done\n");

	//Initialise Winsock
	printf("\nInitialising Winsock...\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		perror("WSAStartup() failed.\n");
		return 1;
	}
	printf("Initialised done.\n");

	//Create a RAW Socket
	printf("\nCreating RAW Socket...\n");
	snifferH = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
	if (snifferH == INVALID_SOCKET)
	{
		perror("Failed to create raw socket.");
		return 1;
	}

	snifferT = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
	if (snifferT == INVALID_SOCKET)
	{
		perror("Failed to create raw socket.");
		return 1;
	}

	snifferF = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
	if (snifferF == INVALID_SOCKET)
	{
		perror("Failed to create raw socket.");
		return 1;
	}

	snifferD = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
	if (snifferD == INVALID_SOCKET)
	{
		perror("Failed to create raw socket.");
		return 1;
	}
	printf("Created done.\n");

	//Retrive the local hostname
	if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR)
	{
		printf("Error : %d", WSAGetLastError());
		return 1;
	}
	printf("\nHost name : %s \n", hostname);

	//Retrive the available IPs of the local host
	local = gethostbyname(hostname);
	printf("\nAvailable Network Interfaces list\n");
	if (local == NULL)
	{
		printf("Error : %d.\n", WSAGetLastError());
		return 1;
	}

	for (i = 0; local->h_addr_list[i] != 0; ++i)	//로컬의 이더넷 리스트를 복사
	{
		memcpy(&addr, local->h_addr_list[i], sizeof(struct in_addr));
		printf("Interface Number : %d Address : %s\n", i, inet_ntoa(addr));
	}

	
	//스니핑 할 이더넷을 지정
	
	while (1) {
		printf("\n\n\nEnter the interface number you would like to sniff between %d to %d : ",0, i - 1);
		
		scanf_s("%s", &in, sizeof(in));
		
		while (getchar() != '\n');
		//남은 버퍼내용 비우기

		if (isdigit(in[0]) && strlen(in) == 1 && atoi(in) >= 0 && atoi(in) < i - 0)
			break;
		//범위에 맞는 값 들어올 때 까지 반복
		else
			printf("Invalid Value\n");
	}

	in_Num = atoi(in);

	memset(&dest, 0, sizeof(dest));
	//목적지 ip 주소 저장 구조체 0으로 초기화
	memcpy(&dest.sin_addr.s_addr, local->h_addr_list[in_Num], sizeof(dest.sin_addr.s_addr));
	//목적지의 ip 주소를 지정 한 이더넷으로 복사
	dest.sin_family = AF_INET;
	dest.sin_port = 0;

	printf("\n\nBinding socket to local system and port 0 ...");
	//스니핑 소켓과 목적지 아이피를 바인딩
	if (bind(snifferH, (struct sockaddr *)&dest, sizeof(dest)) == SOCKET_ERROR)
	{
		printf("bind(%s) failed.\n", inet_ntoa(addr));
		return 1;
	}

	if (bind(snifferT, (struct sockaddr *)&dest, sizeof(dest)) == SOCKET_ERROR)
	{
		printf("bind(%s) failed.\n", inet_ntoa(addr));
		return 1;
	}

	if (bind(snifferF, (struct sockaddr *)&dest, sizeof(dest)) == SOCKET_ERROR)
	{
		printf("bind(%s) failed.\n", inet_ntoa(addr));
		return 1;
	}

	if (bind(snifferD, (struct sockaddr *)&dest, sizeof(dest)) == SOCKET_ERROR)
	{
		printf("bind(%s) failed.\n", inet_ntoa(addr));
		return 1;
	}

	printf("\nWait.");
	for (i = 0; i < 4; i++) {
		printf(".");
		Sleep(500);
	}
	
	printf("\nBinding successful");

	//Enable this socket with the power to sniff : SIO_RCVALL is the key Receive ALL ;)
	//SIO_RCVALL을 통해 promiscuous mode 세팅
	/*
	promiscuous mode 세팅
	promiscuous mode란 난잡 모드로, 목적지가 자신이 아닌 패킷은 모두 버리게 되어 있지만,
	난잡 모드로 설정 할 경우 자신에게 오지 않는 패킷들도 모두 받아들인다.
	대부분의 패킷 캡처 프로그램은 기본적으로 난잡 모드로 설정한다.
	*/


	j = TRUE;
	//WSAIoctl()에 SIO_RCVALL 로 TRUE(1)을 전달한다.

	printf("\n\n\nSetting socket to sniff...\n");
	if (WSAIoctl(snifferH, SIO_RCVALL, &j, sizeof(j), 0, 0, (LPDWORD)&in, 0, 0) == SOCKET_ERROR)
	{
		printf("WSAIoctl() failed.\n");
		return 1;
	}
	if (WSAIoctl(snifferT, SIO_RCVALL, &j, sizeof(j), 0, 0, (LPDWORD)&in, 0, 0) == SOCKET_ERROR)
	{
		printf("WSAIoctl() failed.\n");
		return 1;
	}
	if (WSAIoctl(snifferF, SIO_RCVALL, &j, sizeof(j), 0, 0, (LPDWORD)&in, 0, 0) == SOCKET_ERROR)
	{
		printf("WSAIoctl() failed.\n");
		return 1;
	}
	if (WSAIoctl(snifferD, SIO_RCVALL, &j, sizeof(j), 0, 0, (LPDWORD)&in, 0, 0) == SOCKET_ERROR)
	{
		printf("WSAIoctl() failed.\n");
		return 1;
	}

	printf("Socket setting done.\n\n\n");

	//시작
	printf("Started Sniffing\n");
	printf("Packet Capture Statistics...\n");
	Sleep(2000);
	system("cls");
	selectMode(snifferH, snifferT, snifferF, snifferD);


	closesocket(snifferH);
	closesocket(snifferT);
	closesocket(snifferF);
	closesocket(snifferD);
	WSACleanup();
	//종료시 소켓 및 WSA 닫음
	return 0;
}

void selectMode(SOCKET snifferH, SOCKET snifferT, SOCKET snifferF, SOCKET snifferD) {
	
	int tHTTP = 0, tTelnet = 0, tFTP = 0, tDNS = 0;

	HANDLE hHTTP = NULL, hTelnet = NULL, hFTP = NULL, hDNS = NULL;
	DWORD dwHTTP = NULL, dwTelnet = NULL, dwFTP = NULL, dwDNS = NULL;

	THREAD HTTP, Telnet, FTP, DNS;
	
	char temp[50];
	int sel;
	gotoxy(0, 0);
	printf("Select sniffing mode\n");
	printf("1.HTTP\n");
	printf("2.Telnet\n");
	printf("3.FTP\n");
	printf("4.UDP_DNS\n");
	printf("5.Quit\n");

	while (1) {
		gotoxy(0, 6);
		printf("Insert num between 1 to 5 : ");
		
		scanf_s("%s", &temp, sizeof(temp));
		while (getchar() != '\n');
		
		if (isdigit(temp[0]) && strlen(temp) == 1 && atoi(temp) > 0 && atoi(temp) < 5){
			sel = atoi(temp);
			switch (sel)
			{
				//스레드 생성, 중복생성 x
			case 1:
				if (tHTTP == 1) {
					printf("이미 실행중입니다.\n");
				}
				else {
					HTTP.sel = 1;
					HTTP.sniffer = snifferH;
					tHTTP++;
					hHTTP = (HANDLE)_beginthreadex(NULL, 0, StartSniffing, (void *)&HTTP, 0, (unsigned*)&dwHTTP);
				}
				break;
			case 2:
				if (tTelnet == 1) {
					printf("이미 실행중입니다.\n");
				}
				else {
					Telnet.sel = 2;
					Telnet.sniffer = snifferT;
					tTelnet++;
					hTelnet = (HANDLE)_beginthreadex(NULL, 0, StartSniffing, (void *)&Telnet, 0, (unsigned*)&dwTelnet);
				}
				break;
			case 3:
				if (tFTP == 1) {
					printf("이미 실행중입니다.\n");
				}
				else {
					FTP.sel = 3;
					FTP.sniffer = snifferF;
					tFTP++;
					hFTP = (HANDLE)_beginthreadex(NULL, 0, StartSniffing, (void *)&FTP, 0, (unsigned*)&dwFTP);
				}
				break;
			case 4:
				if (tDNS == 1) {
					printf("이미 실행중입니다.\n");
				}
				else {
					DNS.sel = 4;
					DNS.sniffer = snifferD;
					tDNS++;
					hDNS = (HANDLE)_beginthreadex(NULL, 0, StartSniffing, (void *)&DNS, 0, (unsigned*)&dwDNS);
				}
				break;
			default:
				printf("Invalid Value\n");
				break;
			}
		}else if (isdigit(temp[0]) && strlen(temp) == 1 && atoi(temp) == 5){
			//프로그램 종료
			off = 1;

			dwHTTP = 0;
			dwTelnet = 0;
			dwFTP = 0;
			dwDNS = 0;

			while (1) {
				WaitForSingleObject(hHTTP, INFINITE);
				WaitForSingleObject(hTelnet, INFINITE);
				WaitForSingleObject(hFTP, INFINITE);
				WaitForSingleObject(hDNS, INFINITE);

				GetExitCodeThread(hHTTP, &dwHTTP);
				GetExitCodeThread(hTelnet, &dwHTTP);
				GetExitCodeThread(hFTP, &dwFTP);
				GetExitCodeThread(hDNS, &dwDNS);

				CloseHandle(hHTTP);
				CloseHandle(hTelnet);
				CloseHandle(hFTP);
				CloseHandle(hDNS);
				break;
			}
			break;
		}else
			printf("Invalid Value\n");
		//버퍼 지우기 및 범위에 맞는 값 받을 때 까지 무한룹
	}
	//atoi는 문자를 숫자로 변형하는 함수
}

unsigned WINAPI StartSniffing(void *p)
{
	THREAD* pp = (THREAD*)p;

	SOCKET sniffer = pp->sniffer;
	int sel = pp->sel;

	char *Buffer = (char *)malloc(65536);
	//최대 사이즈의 버퍼 크기 할당


	int mangobyte;

	if (Buffer == NULL)
	{
		perror("malloc() failed.\n");
		return;
	}

	do
	{
		mangobyte = recvfrom(sniffer, Buffer, 65536, 0, 0, 0);
		//전체 패킷을 받으며 받은 데이터의 크기를 저장

		if (mangobyte > 0)
		{
			switch (sel) {
			case 1:
				PrintHTTP(Buffer, mangobyte);
				break;

			case 2:
				PrintTelnet(Buffer, mangobyte);
				break;

			case 3:
				PrintFTP(Buffer, mangobyte);
				break;

			case 4:
				PrintUDP_DNS(Buffer, mangobyte);
				break;

			default:
				break;
			}
		}
		else
		{
			perror("recvfrom() failed.\n");
		}

		if (off == 1) {
			free(Buffer);
			return 1;
		}



	} while (mangobyte > 0);

	free(Buffer);
}

void PrintHTTP(char* Buffer, int Size) {

	IPV4_HDR *iphdr;
	TCP_HDR *tcpheader;
	UDP_HDR *udpheader;


	iphdr = (IPV4_HDR *)Buffer;

	unsigned short iphdrlen;

	iphdr = (IPV4_HDR *)Buffer;
	iphdrlen = iphdr->ip_header_len * 4;

	if((unsigned int)iphdr->ip_protocol == TCP){

		tcpheader = (TCP_HDR*)(Buffer + iphdrlen);

		if (tcpheader->dest_port == htons(80) || tcpheader->dest_port == htons(443) || tcpheader->source_port == htons(80) || tcpheader->source_port == htons(443)) {
			//80 : HTTP
			//443 : HTTPS - HTTP over SSL (암호화 전송)

			PrintTCP(Buffer, iphdrlen, Size, logfile_HTTP);
			++HTTPtotal;
			gotoxy(0, 7);
			printf("%d HTTP Packet Capture\r", HTTPtotal);
			gotoxy(28, 6);
		}
	}
}

void PrintTelnet(char* Buffer, int Size) {

	IPV4_HDR *iphdr;
	TCP_HDR *tcpheader;
	UDP_HDR *udpheader;


	iphdr = (IPV4_HDR *)Buffer;

	unsigned short iphdrlen;

	iphdr = (IPV4_HDR *)Buffer;
	iphdrlen = iphdr->ip_header_len * 4;

	if ((unsigned int)iphdr->ip_protocol == TCP) {

		tcpheader = (TCP_HDR*)(Buffer + iphdrlen);

		if (tcpheader->dest_port == htons(23) || tcpheader->source_port == htons(23)) {
			//23 : telnet port num

			PrintTCP(Buffer, iphdrlen, Size, logfile_Telnet);
			++Telnettotal;
			gotoxy(0, 8);
			printf("%d Telnet Packet Capture\r", Telnettotal);
			gotoxy(28, 6);
		}

	}
}


void PrintFTP(char* Buffer, int Size) {

	IPV4_HDR *iphdr;
	TCP_HDR *tcpheader;
	UDP_HDR *udpheader;


	iphdr = (IPV4_HDR *)Buffer;

	unsigned short iphdrlen;

	iphdr = (IPV4_HDR *)Buffer;
	iphdrlen = iphdr->ip_header_len * 4;

	if ((unsigned int)iphdr->ip_protocol == TCP) {
		tcpheader = (TCP_HDR*)(Buffer + iphdrlen);

		if (tcpheader->dest_port == htons(20) || tcpheader->dest_port == htons(21) || tcpheader->dest_port == htons(22) || tcpheader->dest_port == htons(990) || tcpheader->source_port == htons(20) || tcpheader->source_port == htons(21) || tcpheader->source_port == htons(22) || tcpheader->source_port == htons(990)) {
			/*
			20 : FTP 데이터 포트
			21 : FTP 제어 포트
			22 : SSH(Secure Shell) - ssh scp, sftp와 같은 프로토콜 및 포트 포워딩
			69 : TFTP - FTP UDP버전
			990 : SSL 위의 FTP (암호화 전송)
			*/

			PrintTCP(Buffer, iphdrlen, Size, logfile_TCP_FTP);
			++FTPtotal;
			gotoxy(0, 9);
			printf("%d FTP Packet Capture\r", FTPtotal);
			gotoxy(28, 6);
		}
	}else if ((unsigned int)iphdr->ip_protocol == UDP) {
		udpheader = (UDP_HDR*)(Buffer + iphdrlen);

		if (udpheader->dest_port == htons(69) || udpheader->source_port == htons(69)) {
			/*
			20 : FTP 데이터 포트
			21 : FTP 제어 포트
			22 : SSH(Secure Shell) - ssh scp, sftp와 같은 프로토콜 및 포트 포워딩
			69 : TFTP - FTP UDP버전
			990 : SSL 위의 FTP (암호화 전송)
			*/

			PrintUDP(Buffer, iphdrlen, Size, logfile_UDP_FTP);
			++FTPtotal;
			gotoxy(0, 9);
			printf("%d FTP Packet Capture\r", FTPtotal);
			gotoxy(28, 6);
		}
	}

	
}

void PrintUDP_DNS(char* Buffer, int Size) {

	IPV4_HDR *iphdr;
	TCP_HDR *tcpheader;
	UDP_HDR *udpheader;


	unsigned short iphdrlen;

	struct sockaddr_in source, dest;

	iphdr = (IPV4_HDR *)Buffer;
	iphdrlen = iphdr->ip_header_len * 4;

	memset(&source, 0, sizeof(source));
	source.sin_addr.s_addr = iphdr->ip_srcaddr;

	memset(&dest, 0, sizeof(dest));
	dest.sin_addr.s_addr = iphdr->ip_destaddr;

	iphdr = (IPV4_HDR *)Buffer;
	iphdrlen = iphdr->ip_header_len * 4;

	if((unsigned int)iphdr->ip_protocol == UDP){
		udpheader = (UDP_HDR *)(Buffer + iphdrlen);

		if (udpheader->dest_port == htons(53) || udpheader->source_port == htons(53)) {
			// 53 : TCP 및 UDP DNS 프로토콜

			PrintUDP(Buffer, iphdrlen, Size, logfile_UDP_DNS);
			++DNStotal;
			gotoxy(0, 10);
			printf("%d DNS Packet Capture\r", DNStotal);
			gotoxy(28, 6);
		}
	}else if ((unsigned int)iphdr->ip_protocol == TCP) {
		tcpheader = (TCP_HDR *)(Buffer + iphdrlen);

		if (tcpheader->dest_port == htons(53) || tcpheader->source_port == htons(53)) {
			// 53 : TCP 및 UDP DNS 프로토콜

			PrintTCP(Buffer, iphdrlen, Size, logfile_TCP_DNS);
			++DNStotal;
			gotoxy(0, 10);
			printf("%d DNS Packet Capture\r", DNStotal);
			gotoxy(28, 6);

		}
	}
}
//패킷 내용 전체 출력하는 함수
void PrintTCP(char* Buffer, int iphdrlen ,int Size, FILE *file) {

	IPV4_HDR *iphdr;
	TCP_HDR *tcpheader;
	UDP_HDR *udpheader;

	struct sockaddr_in source, dest;

	iphdr = (IPV4_HDR *)Buffer;
	iphdrlen = iphdr->ip_header_len * 4;

	tcpheader = (TCP_HDR*)(Buffer + iphdrlen);

	memset(&source, 0, sizeof(source));
	source.sin_addr.s_addr = iphdr->ip_srcaddr;

	memset(&dest, 0, sizeof(dest));
	dest.sin_addr.s_addr = iphdr->ip_destaddr;

	fprintf(file, "\n\n***********************TCP Packet*************************\n");

	fprintf(file, "\n");
	fprintf(file, "IP Header\n");
	fprintf(file, " |-IP Version : %d\n", (unsigned int)iphdr->ip_version);
	fprintf(file, " |-IP Header Length : %d DWORDS or %d Bytes\n", (unsigned int)iphdr->ip_header_len, ((unsigned int)(iphdr->ip_header_len)) * 4);
	fprintf(file, " |-Type Of Service : %d\n", (unsigned int)iphdr->ip_tos);
	fprintf(file, " |-IP Total Length : %d Bytes(Size of Packet)\n", ntohs(iphdr->ip_total_length));
	fprintf(file, " |-Identification : %d\n", ntohs(iphdr->ip_id));
	fprintf(file, " |-Reserved ZERO Field : %d\n", (unsigned int)iphdr->ip_reserved_zero);
	fprintf(file, " |-Dont Fragment Field : %d\n", (unsigned int)iphdr->ip_dont_fragment);
	fprintf(file, " |-More Fragment Field : %d\n", (unsigned int)iphdr->ip_more_fragment);
	fprintf(file, " |-TTL : %d\n", (unsigned int)iphdr->ip_ttl);
	fprintf(file, " |-Protocol : %d\n", (unsigned int)iphdr->ip_protocol);
	fprintf(file, " |-Checksum : %d\n", ntohs(iphdr->ip_checksum));
	fprintf(file, " |-Source IP : %s\n", inet_ntoa(source.sin_addr));
	fprintf(file, " |-Destination IP : %s\n", inet_ntoa(dest.sin_addr));

	fprintf(file, "\n");
	fprintf(file, "TCP Header\n");
	fprintf(file, " |-Source Port : %u\n", ntohs(tcpheader->source_port));
	fprintf(file, " |-Destination Port : %u\n", ntohs(tcpheader->dest_port));
	fprintf(file, " |-Sequence Number : %u\n", ntohl(tcpheader->sequence));
	fprintf(file, " |-Acknowledge Number : %u\n", ntohl(tcpheader->acknowledge));
	fprintf(file, " |-Header Length : %d DWORDS or %d BYTES\n"
		, (unsigned int)tcpheader->data_offset, (unsigned int)tcpheader->data_offset * 4);
	fprintf(file, " |-CWR Flag : %d\n", (unsigned int)tcpheader->cwr);
	fprintf(file, " |-ECN Flag : %d\n", (unsigned int)tcpheader->ecn);
	fprintf(file, " |-Urgent Flag : %d\n", (unsigned int)tcpheader->urg);
	fprintf(file, " |-Acknowledgement Flag : %d\n", (unsigned int)tcpheader->ack);
	fprintf(file, " |-Push Flag : %d\n", (unsigned int)tcpheader->psh);
	fprintf(file, " |-Reset Flag : %d\n", (unsigned int)tcpheader->rst);
	fprintf(file, " |-Synchronise Flag : %d\n", (unsigned int)tcpheader->syn);
	fprintf(file, " |-Finish Flag : %d\n", (unsigned int)tcpheader->fin);
	fprintf(file, " |-Window : %d\n", ntohs(tcpheader->window));
	fprintf(file, " |-Checksum : %d\n", ntohs(tcpheader->checksum));
	fprintf(file, " |-Urgent Pointer : %d\n", tcpheader->urgent_pointer);
	fprintf(file, "\n");
	fprintf(file, " DATA Dump ");
	fprintf(file, "\n");

	fprintf(file, "IP Header\n");
	PrintData(Buffer, iphdrlen, file);

	fprintf(file, "TCP Header\n");
	PrintData(Buffer + iphdrlen, tcpheader->data_offset * 4, file);

	fprintf(file, "Data Payload\n");
	PrintData(Buffer + iphdrlen + tcpheader->data_offset * 4
		, (Size - tcpheader->data_offset * 4 - iphdr->ip_header_len * 4), file);

	fprintf(file, "\n###########################################################");
}

void PrintUDP(char *Buffer, int iphdrlen, int Size, FILE *file) {

	IPV4_HDR *iphdr;
	TCP_HDR *tcpheader;
	UDP_HDR *udpheader;

	struct sockaddr_in source, dest;

	iphdr = (IPV4_HDR *)Buffer;
	iphdrlen = iphdr->ip_header_len * 4;

	udpheader = (UDP_HDR*)(Buffer + iphdrlen);

	memset(&source, 0, sizeof(source));
	source.sin_addr.s_addr = iphdr->ip_srcaddr;

	memset(&dest, 0, sizeof(dest));
	dest.sin_addr.s_addr = iphdr->ip_destaddr;

	fprintf(file, "\n\n***********************UDP Packet*************************\n");

	fprintf(file, "\n");
	fprintf(file, "IP Header\n");
	fprintf(file, " |-IP Version : %d\n", (unsigned int)iphdr->ip_version);
	fprintf(file, " |-IP Header Length : %d DWORDS or %d Bytes\n", (unsigned int)iphdr->ip_header_len, ((unsigned int)(iphdr->ip_header_len)) * 4);
	fprintf(file, " |-Type Of Service : %d\n", (unsigned int)iphdr->ip_tos);
	fprintf(file, " |-IP Total Length : %d Bytes(Size of Packet)\n", ntohs(iphdr->ip_total_length));
	fprintf(file, " |-Identification : %d\n", ntohs(iphdr->ip_id));
	fprintf(file, " |-Reserved ZERO Field : %d\n", (unsigned int)iphdr->ip_reserved_zero);
	fprintf(file, " |-Dont Fragment Field : %d\n", (unsigned int)iphdr->ip_dont_fragment);
	fprintf(file, " |-More Fragment Field : %d\n", (unsigned int)iphdr->ip_more_fragment);
	fprintf(file, " |-TTL : %d\n", (unsigned int)iphdr->ip_ttl);
	fprintf(file, " |-Protocol : %d\n", (unsigned int)iphdr->ip_protocol);
	fprintf(file, " |-Checksum : %d\n", ntohs(iphdr->ip_checksum));
	fprintf(file, " |-Source IP : %s\n", inet_ntoa(source.sin_addr));
	fprintf(file, " |-Destination IP : %s\n", inet_ntoa(dest.sin_addr));

	fprintf(file, "\nUDP Header\n");
	fprintf(file, " |-Source Port : %d\n", ntohs(udpheader->source_port));
	fprintf(file, " |-Destination Port : %d\n", ntohs(udpheader->dest_port));
	fprintf(file, " |-UDP Length : %d\n", ntohs(udpheader->udp_length));
	fprintf(file, " |-UDP Checksum : %d\n", ntohs(udpheader->udp_checksum));

	fprintf(file, "\n");
	fprintf(file, "IP Header\n");

	PrintData(Buffer, iphdrlen, file);

	fprintf(file, "UDP Header\n");

	PrintData(Buffer + iphdrlen, sizeof(UDP_HDR), file);

	fprintf(file, "Data Payload\n");

	PrintData(Buffer + iphdrlen + sizeof(UDP_HDR), (Size - sizeof(UDP_HDR) - iphdr->ip_header_len * 4), file);

	fprintf(file, "\n###########################################################");

}

/*
Print the hex values of the data
페이로드 부분의 데이터를 헥사의 데이터와 사람이 알아보는 데이터로 출력
*/
void PrintData(char* data, int Size, FILE *logfile)
{
	char a, line[17], c;
	int i, j;

	for (i = 0; i < Size; i++){
		c = data[i];
		fprintf(logfile, " %.2x", (unsigned char)c);
		a = (c >= 32 && c <= 128) ? (unsigned char)c : '.';

		line[i % 16] = a;

		if ((i != 0 && (i + 1) % 16 == 0) || i == Size - 1){
			line[i % 16 + 1] = '\0';
			
			fprintf(logfile, "          ");

			for (j = strlen(line); j < 16; j++){
				fprintf(logfile, "   ");
			}

			fprintf(logfile, "%s \n", line);
		}
	}

	fprintf(logfile, "\n");
}
//cmd 커서 위치 지정 함수
void gotoxy(int x, int y) {
	COORD Pos;
	Pos.X = x;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
