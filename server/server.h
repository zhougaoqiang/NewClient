
#include <WinSock2.h>
#include <stdio.h>
#include <vector>
#include <WS2tcpip.h>
#include "sqliteLearning.h"
#include <iostream>


using namespace std;

#pragma comment (lib, "WS2_32")  //���ӵ�WS2_32.lib
#pragma warning(disable:4996)  //�ر�4996����

#define SERVER_IP "192.168.1.140"  //Ĭ�Ϸ����IP��ַ
#define SERVER_PORT 8888  //�������˿ں�


class server
{
private:
	int listener;//�����׽���
	sockaddr_in serverAddress; //IPV4��ַ��ʽ
	vector<int> socnum;  //��Ŵ������׽���
	char username[100];
	char password[100];


public:
	server();
	void init();
	void process();
	bool passwordValidation(int Clientfd, int i);
};

server::server()
{
	listener = 0;
	serverAddress.sin_family = PF_INET;
	serverAddress.sin_port = SERVER_PORT;
	serverAddress.sin_addr.s_addr = inet_addr(SERVER_IP);   //���ַ�������ת��Ϊuint32_t
}

void server::init()
{
	int Ret;
	WSADATA wsaData; //���ڳ�ʼ���׽��ֻ���
	//��ʼ��Winsock����
	if ((Ret = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
	{
		cout << "WSAStartup() failed with error: " << Ret << endl;
		WSACleanup();
	}

	listener = socket(AF_INET, SOCK_STREAM, 0);  //����IPV4��TCP����
	if (listener == -1)
	{
		cout << "Error at socket(): " << WSAGetLastError() << endl;
		cout << "Listener failed" << endl;
		exit(1);
	}

	cout << "�����ɹ�" << endl;

	unsigned long ul = 1;
	if (ioctlsocket(listener, FIONBIO, (unsigned long*)&ul) == -1)
	{
		perror("ioctl failed");
		exit(1);
	}

	///////////////////////////////////////////////////////////////////
	//�м�Ĳ����󶨵ĵ�ַ�����IPV4����///////////////////
	//struct sockaddr_in {
	//  sa_family_t    sin_family; /* address family: AF_INET */
	//  in_port_t      sin_port;   /* port in network byte order */
	//  struct in_addr sin_addr;   /* internet address */
	//};
	//Internet address.
	//struct in_addr {
	//  uint32_t       s_addr;     /* address in network byte order */
	//}
	/////////////////////////////////////////////////////////////////

	if (bind(listener, (struct sockaddr *)&serverAddress, sizeof(serverAddress))<0)
	{
		perror("bind error");
		exit(1);
	}

	if (listen(listener, 6) < 0)
	{
		perror("Listen failed");
		exit(1);
	}

	socnum.push_back(listener); //�������׽��ּ���

}

void server::process()
{
	int mount = 0;
	fd_set fds;
	FD_ZERO(&fds); //��fds����
	init();
	tableCreation();
	char Input1[] = "zhougaoqiang";
	char Input2[] = "123456";
	char Input3[] = "10.10.10.10";
	tableInsertion(Input1, Input2, Input3);

	//������ǲ��ϵļ��
	while (1)
	{
		mount = socnum.size();
		//��fdsÿ�ζ����¸�ֵ
		for (int i = 0; i < mount; ++i)
		{
			FD_SET(socnum[i], &fds);
		}

		struct timeval timeout = { 1,0 };//ÿ��select�ȴ�3��
		switch (select(0, &fds, NULL, NULL, &timeout))
		{
		case -1:
			{
				perror("select\n");
				cout << "Error at socket(): " << WSAGetLastError() << endl;
				cout << mount << endl;
				/*          
				for (int i = 0; i < mount; ++i)
				{
				printf("%d\n", socnum[i]);
				}
				*/
				Sleep(1000);
				break;
			}
		case 0:
			{
				//printf("select timeout......");
				break;
			}
		default:
			{
				//�������е�ÿһ���׽��ֶ���ʣ����׽��ֽ��бȽϵõ���ǰ������
				for (int i = 0; i < mount; ++i)
				{
					//�����һ���׽��ֿɶ�����Ϣ����Ҫ��������
					if (i == 0 && FD_ISSET(socnum[i], &fds))
					{
						struct sockaddr_in client_address;
						socklen_t client_addrLength = sizeof(struct sockaddr_in);
						//����һ���û����׽���
						int clientfd = accept(listener, (struct sockaddr*)&client_address, &client_addrLength);
						//����û�������������ʾ��Ϣ����֪ͨ�û����ӳɹ�
						socnum.push_back(clientfd);
						cout << "���ӳɹ�" << endl;

						if (passwordValidation(clientfd, i) == true)
							cout << "��½�ɹ�\n";
						else
						{
							//FD_CLR(socnum[i], &fds);  //���б���ɾ��
							//socnum.erase(socnum.begin() + i); //��vector������ɾ��
							cout << "Ҫ�����µ�½" << endl;
						}
						char ID[1024];
						sprintf(ID, "You ID is: %d", clientfd);
						char buf[30] = "\nWelcome to yshn's chatroom\n";
						strcat(ID, buf);
						send(clientfd, ID, sizeof(ID) - 1, 0); //��ȥ���һ����\0��
					}
					if (i != 0 && FD_ISSET(socnum[i], &fds))
					{
						char buf[1024];
						memset(buf, '\0', sizeof(buf));
						int size = recv(socnum[i], buf, sizeof(buf) - 1, 0);
						//����Ƿ����
						if (size == 0 || size == -1)
						{
							cout << "Remote client close, size is %d" << size << endl;
							//closesocket(socnum[i]); //�ȹر�����׽���
							FD_CLR(socnum[i], &fds);  //���б���ɾ��
							socnum.erase(socnum.begin() + i); //��vector������ɾ��
						}
						else  //����û�е���
						{
							printf("client %d says: \n", socnum[i], buf);
							//���͸�ÿ���û�
							for (int j = 1; j < mount; j++)
							{
								char client[1024];
								sprintf(client, "client %d:", socnum[i]);
								strcat(client, buf);
								send(socnum[j], client, sizeof(client) - 1, 0); //���
							}
						}
					}
				}
				break;
			}
		}
	}
}

bool server::passwordValidation(int Clientfd, int i) {   //��½��֤,   i:����
	char RecvUsername[100];
	char RecvPassword[100];
	char accountCreation[100];
	char failed[] = "failed";
	char success[] = "passed";
	char accountSuccessCreate[] = "New account creation";
	memset(RecvUsername, '\0', sizeof(RecvUsername));
	memset(RecvPassword, '\0', sizeof(RecvPassword));
	memset(accountCreation, '\0', sizeof(accountCreation));


	int size = 0;
	while (size <= 0)
	{
		size = recv(Clientfd, accountCreation, sizeof(accountCreation), 0);  //�����û�״̬
	}
	size = 0;
	while (size <= 0)
	{
		size = recv(Clientfd, RecvUsername, sizeof(RecvUsername), 0);  //�����û���
	}
	size = 0;
	while (size <= 0)
	{
		size = recv(Clientfd, RecvPassword, sizeof(RecvPassword), 0);  //��������
	}

	/*
	if (strcmp(username, RecvUsername) == 0 && strcmp(password, RecvPassword) == 0)
	{
		send(Clientfd, success, strlen(success), 0);
		return true;
	}
	else
	{
		send(Clientfd, failed, strlen(failed), 0);
		return false;
	}
	*/

	cout << "�����û���Ϣ�ɹ�" << endl;


	char tempInput[] = "10.10.10.10";
	if (strcmp(accountSuccessCreate, accountCreation) == 0)
	{
		if (tableInsertion(RecvUsername, RecvPassword, tempInput)==true)
		{
			cout << "true" <<endl;
			char tempsend1[100] = "User account creation successfully";
			send(Clientfd, tempsend1, strlen(tempsend1), 0);
			return true;
		} 
		else
		{
			char tempsend2[100]="User account creation failed";
			send(Clientfd, tempsend2, strlen(tempsend2), 0);
			return false;
		}
	}
	else
	{
		if (tableSelection(RecvUsername, RecvPassword) == true)
		{
			send(Clientfd, success, strlen(success), 0);
			return true;
		}
		else
		{
			send(Clientfd, failed, strlen(failed), 0);
			return false;
		}
	}
}