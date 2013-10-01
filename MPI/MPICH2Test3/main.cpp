
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

enum MessageType
{
	MyData = 0,
	FindMax = 1
};

const int bufsize = 1024;

void SendMessage(int rank, int number, int toRank, int tagNumber)
{
	int position = 0;
	char buf[bufsize];
	MPI_Pack(&rank, 1, MPI_INT, buf, bufsize, &position, MPI_COMM_WORLD);
	MPI_Pack(&number, 1, MPI_INT, buf, bufsize, &position, MPI_COMM_WORLD);

	MPI_Send(buf, bufsize, MPI_PACKED, toRank, tagNumber, MPI_COMM_WORLD);
}

void RecieveMessage(int& rank, int& number, int fromRank, int tagNumber)
{
	MPI_Status status;
	char buf[bufsize];
	MPI_Recv(buf, bufsize, MPI_PACKED, fromRank, tagNumber, MPI_COMM_WORLD, &status);
	int position = 0;
	MPI_Unpack(buf, bufsize, &position, &rank, 1, MPI_INT, MPI_COMM_WORLD);
	MPI_Unpack(buf, bufsize, &position, &number, 1, MPI_INT, MPI_COMM_WORLD);
}

void SendMessage(MessageType Type, int Number, int toRank, int tagnumber)
{
	int position = 0;
	char buf[bufsize];
	int messagetype = (int)Type;
	MPI_Pack(&messagetype, 1, MPI_INT, buf, bufsize, &position, MPI_COMM_WORLD);
	MPI_Pack(&Number, 1, MPI_INT, buf, bufsize, &position, MPI_COMM_WORLD);

	MPI_Send(buf, bufsize, MPI_PACKED, toRank, tagnumber, MPI_COMM_WORLD);
}

void RecieveMessage(MessageType& Type, int& Number, int fromRank, int tagnumber)
{
	MPI_Status status;
	char buf[bufsize];
	MPI_Recv(buf, bufsize, MPI_PACKED, fromRank, tagnumber, MPI_COMM_WORLD, &status);
	int position = 0;
	int messagetype = 0;
	MPI_Unpack(buf, bufsize, &position, &messagetype, 1, MPI_INT, MPI_COMM_WORLD);
	MPI_Unpack(buf, bufsize, &position, &Number, 1, MPI_INT, MPI_COMM_WORLD);

	if(messagetype == 0)
	{
		Type = MessageType::MyData;
	}
	else if(messagetype == 1)
	{
		Type = MessageType::FindMax;
	}
}

void DUMP(const int rank, const string& st)
{
	stringstream sst;
	sst << "MyRank :"
		<< rank
		<< " "
		<< st;
	cout << sst.str().c_str() << endl;
}

void DumpNumber(const int rank, const int Number)
{
	stringstream sst;
	sst << "MyRank :"
		<< rank
		<< " MyNumber :"
		<< Number;
	cout << sst.str().c_str() << endl;
}

//#define DumpSend
//#define DumpRecieve

void DumpBeforeSendMessage(int fromRank, int toRank, MessageType Type, int deliverNumber)
{
#ifdef DumpSend
	string typest;
	if(Type == MessageType::MyData)
	{
		typest = "MyData";
	}
	else if(Type == MessageType::FindMax)
	{
		typest = "FindMax";
	}

	stringstream sst;
	sst << "myRank; "
		<< fromRank
		<< " toRank: "
		<< toRank
		<< " MessageType: "
		<< typest
		<< " deliverNumber: "
		<< deliverNumber
		<< " のSendMessage前";
	cout << sst.str() << endl;
#endif
}

void DumpAfterSendMessage(int fromRank, int toRank)
{
#ifdef DumpSend
	stringstream sst;
	sst << "myRank; "
		<< fromRank
		<< " toRank: "
		<< toRank
		<< " のSendMessage後";
	cout << sst.str() << endl;
#endif
}

void DumpBeforeRecieveMessage(int fromRank, int toRank)
{
#ifdef DumpRecieve
	stringstream sst;
	sst << "fromRank; "
		<< fromRank
		<< " myRank: "
		<< toRank
		<< " のRecieveMessage前";
	cout << sst.str() << endl;
#endif
}

void DumpAfterRecieveMessage(int fromRank, int toRank, MessageType Type, int deliverNumber)
{
#ifdef DumpRecieve
	string typest;
	if(Type == MessageType::MyData)
	{
		typest = "MyData";
	}
	else if(Type == MessageType::FindMax)
	{
		typest = "FindMax";
	}

	stringstream sst;
	sst << "fromRank; "
		<< fromRank
		<< " myRank: "
		<< toRank
		<< " MessageType: "
		<< typest
		<< " deliverNumber: "
		<< deliverNumber
		<< " のRecieveMessage後";
	cout << sst.str() << endl;
#endif
}

int main(int argc, char* argv[])
{
	MPI::Init(argc, argv);

	MPI_Status status;
	
	const int numProc = MPI::COMM_WORLD.Get_size();
	const int myRank = MPI::COMM_WORLD.Get_rank();

	srand((unsigned)time(NULL) * (unsigned)(myRank + 1));
	int myNumber = rand();

	// 各々のNumberをprocess0に送信
	if(0 != myRank)
	{
		SendMessage(myRank, myNumber, 0, 2);
	}
	else
	{		
		cout << "Process : 0 Number: " << myNumber << endl;
		for(int i = 1; i < numProc; i++)
		{
			int Rank = 0;
			int Number = 0;
			RecieveMessage(Rank, Number, i, 2);
			cout << "Process : " << Rank << " Number: " << Number << endl;
		}
	}

	int deliverNumber = 0;
	int maxNumber = 0;
	
	if(0 == myRank) // 始動プロセス
	{
		// (1) myNumber → deliverNumber
		deliverNumber = myNumber;

		bool runStep2 = true;

		while(1)
		{
			// (2) (MyData, deliverNumber)をProcess1に送信
			if(runStep2)
			{
				DumpBeforeSendMessage(myRank, myRank + 1, MessageType::MyData, deliverNumber);
				SendMessage(MessageType::MyData, deliverNumber, myRank + 1, 0);
				DumpAfterSendMessage(myRank, myRank + 1);
			}

			// (3) メッセージをProcess (numProc - 1)から受け取るのを待つ
			MessageType Type;
			DumpBeforeRecieveMessage(numProc -1, myRank);
			RecieveMessage(Type, deliverNumber, numProc -1, 0);
			DumpAfterRecieveMessage(numProc -1, myRank, Type, deliverNumber);

			// (4) 受け取ったデータがMyDataのとき
			if(Type == MessageType::MyData)
			{
			// (5) 受け取った値がmyNumberと等しいとき
				if(deliverNumber == myNumber)
				{
			// (6) 受け取った値をが最大値
					maxNumber = deliverNumber;

			// (7) (FindMax, deliverNumber)をProcess1に送信する
					DumpBeforeSendMessage(myRank, myRank + 1, MessageType::FindMax, deliverNumber);
					SendMessage(MessageType::FindMax, deliverNumber, myRank + 1, 0);
					DumpAfterSendMessage(myRank, myRank + 1);

					runStep2 = false;
					continue;
				}
			// (8) 
				else if(deliverNumber > myNumber)
				{
					runStep2 = true;
					continue;
				}

			// (9)
				else if(deliverNumber < myNumber)
				{
					runStep2 = false;
					continue;
				}
			}
			// (10) 受け取ったデータがFindMaxのとき
			else if(Type == MessageType::FindMax)
			{
			// (11)
				if(myNumber == deliverNumber)
				{
					break;
				}

			// (12)
				else if(myNumber < deliverNumber)
				{

			// (13)
					maxNumber = deliverNumber;

			// (14)
					DumpBeforeSendMessage(myRank, myRank + 1, MessageType::FindMax, deliverNumber);
					SendMessage(MessageType::FindMax, deliverNumber, myRank + 1, 0);
					DumpAfterSendMessage(myRank, myRank + 1);
					break;
				}

				// ここに来ることはない
				break;
			}
		}
	}
	else // 始動プロセスでない場合
	{
		while(1)
		{
			// (1) 前のプロセスからの受信待ち
			MessageType Type;
			DumpBeforeRecieveMessage(myRank -1, myRank);
			RecieveMessage(Type, deliverNumber, myRank - 1, 0);
			DumpAfterRecieveMessage(myRank - 1, myRank, Type, deliverNumber);

			// (2)
			if(Type == MessageType::MyData)
			{
			// (3)
				if(deliverNumber == myNumber)
				{
			// (4)
					maxNumber = deliverNumber;
			// (5)
					DumpBeforeSendMessage(myRank, (myRank + 1) % numProc, MessageType::FindMax, deliverNumber);
					SendMessage(MessageType::FindMax, deliverNumber, (myRank + 1) % numProc, 0);
					DumpAfterSendMessage(myRank, (myRank + 1) % numProc);
					continue;
				}
			// (6)
				else if(myNumber < deliverNumber)
				{
			// (7)
					DumpBeforeSendMessage(myRank, (myRank + 1) % numProc, MessageType::MyData, deliverNumber);
					SendMessage(MessageType::MyData, deliverNumber, (myRank + 1) % numProc, 0);
					DumpAfterSendMessage(myRank, (myRank + 1) % numProc);
					continue;
				}
			// (8)
				else if(myNumber > deliverNumber)
				{
					DumpBeforeSendMessage(myRank, (myRank + 1) % numProc, MessageType::MyData, myNumber);
					SendMessage(MessageType::MyData, myNumber, (myRank + 1) % numProc, 0);
					DumpAfterSendMessage(myRank, (myRank + 1) % numProc);
					continue;
				}
			}
			// (9)
			else if(Type == MessageType::FindMax)
			{
			// (10)
				if(myNumber == deliverNumber)
				{
					break;
				}
			// (11)
				else if(myNumber < deliverNumber)
				{
			// (12)
					maxNumber = deliverNumber;
			// (13)
					DumpBeforeSendMessage(myRank, (myRank + 1) % numProc, MessageType::FindMax, deliverNumber);
					SendMessage(MessageType::FindMax, deliverNumber, (myRank + 1) % numProc, 0);
					DumpAfterSendMessage(myRank, (myRank + 1) % numProc);
					break;
				}
			}
		}
	}

	// 最大値ををprocess0に送信
	if(0 != myRank)
	{
		SendMessage(myRank, maxNumber, 0, 3);
	}
	else
	{
		cout << "Process : 0 での最大値は: " << maxNumber << endl;
		for(int i = 1; i < numProc; i++)
		{
			int Rank = 0;
			int Number = 0;
			RecieveMessage(Rank, Number, i, 3);
			cout << "Process : " << Rank << " での最大値は: " << Number << endl;
		}
	}

	MPI::Finalize();
	
	return 0;
}