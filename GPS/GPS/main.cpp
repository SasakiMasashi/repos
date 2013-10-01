#include "GPSRunner.h"
#include "TestData1.h"
#include <conio.h>
#include "LoadData.h"
#include "Dump.h"

#define DumpData

//using namespace GPS;

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		cerr << "�����̌�����������" << endl;
		_getch();
		return -1;
	}

	set<GPS::State> InitialStates;
	set<GPS::State> FinalStates;
	set<GPS::Operation> Opes;

	//CreateTestData(InitialStates, FinalStates, Opes);
	//CreateTestData1(InitialStates, FinalStates, Opes);
	//CreateTestData2(InitialStates, FinalStates, Opes);
	//CreateTestData3(InitialStates, FinalStates, Opes);

	//const string XMLFilePath = "D:\\Develop\\MyProject\\GPS\\GPS\\MonkeyBanana.xml";
	ExecutionManager ExeManager;
	const string XMLFilePath = string(argv[1]);
	if(!GPS::LoadData(XMLFilePath, InitialStates, FinalStates, Opes, ExeManager))
	{
		cerr << "XML�t�@�C���̓ǂݍ��݂Ɏ��s�B" << endl;
		_getch();
		return -1;
	}

#ifdef DumpData
	cout << "InitialStates: " << endl;
	GPS::Dump(InitialStates, cout);
	cout << "FinalStates: " << endl;
	GPS::Dump(FinalStates, cout);
	GPS::Dump(Opes, cout);
	clog << "main-2:XMLFilePath:" << XMLFilePath.c_str() << "  Press" << endl;
	clog << "************************************************************" << endl;
#endif

	RunActionHistory ActionHistory;
	GPS::GPSRunner Runner;
	Runner.SetXMLFilePath(XMLFilePath);
	GPS::StatusType Status = Runner.Do(InitialStates, FinalStates, Opes, ActionHistory);
	if(GPS_NOERROR != Status)
	{
		cerr << "Runner��Do�Ŏ��s���܂����B" << endl;
		_getch();
		return -1;
	}

	Runner.OutputResult(InitialStates, FinalStates, ActionHistory, cout, ExeManager);

	clog << "push any key!" << endl;
	_getch();

	return 0;
}