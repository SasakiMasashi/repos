#include "GPSRunner.h"
#include <windows.h>
#include <conio.h>
#include "Dump.h"
#include <fstream>

//#define DumpData2

namespace GPS
{
	void GetOpesWhichAddhasSpecifiedState(const State& SpecifiedState, 
										  const set<Operation>& Opes, 
										  set<const Operation*>& OpesWhichAddhasSpecifiedState,
										  set<const Operation*>& SearchingOperations)
	{
		set<Operation>::const_iterator cit = Opes.begin();
		for(; cit != Opes.end(); cit++){
			const set<State>& AddState = cit->GetAddStates();
			
			// ���ݒ�������Opration�͑Ώۂ���͂���
			if(SearchingOperations.end() != SearchingOperations.find(&(*cit)))
				continue;

			if(IsMember(SpecifiedState, AddState))
			{
				OpesWhichAddhasSpecifiedState.insert(&(*cit));
				//SearchingOperations.insert(&(*cit));
			}
		}
#ifdef DumpData2
		static int Counter = 0;
		Counter++;
		ofstream fout("C:\\test.txt", ios::app);
		fout << "OpesWhichAddhasSpecifiedState: " << Counter << endl;
		GPS::Dump(OpesWhichAddhasSpecifiedState, fout);
#endif
	}
	
	bool
	IsAllPreStatesRegistered(const Operation& op, const set<State>& set)
	{
		return IsSubSet(op.GetPreStates(), set);
	}

	/////////////////////////////////////////////////////////////////
	// Ope��States�ɑ΂��ēK�p����B
	// �K�p�\���ǂ����͊��Ƀ`�F�b�N���Ă��邱�Ƃ��O��
	/////////////////////////////////////////////////////////////////
	void ApplyOn(const Operation& Ope,
				 set<State>& States,
				 RunActionHistory& ActionHistory,
				 set<const Operation*>& SearchingOperations)
	{
		ActionHistory.Add(&Ope);
		AddSet(Ope.GetAddStates(), States);
		DelSet(Ope.GetDelStates(), States);

		// ���s���ꂽOperation��o�^����͂���
		SearchingOperations.erase(&Ope);

#ifdef DumpData2
		ofstream fout("C:\\test.txt", ios::app);
		fout << "ApplyOn: " << Ope.GetAction().GetName() << endl;
#endif
	}

	/////////////////////////////////////////////////////////////////
	// �J�n��Ԃ̏W��States����A�S�Ă̏I�����FinalStates�܂ŁA
	// Opes��p���ē��B�ł����true��Ԃ��B
	// �I����Ԃ̂����ЂƂł����B�ł��Ȃ����̂������false��Ԃ��B
	/////////////////////////////////////////////////////////////////
	bool AchieveAll(set<State>& States,
					  const set<State>& FinalStates,
					  const set<Operation>& Opes,
					  RunActionHistory& ActionHistory,
					  set<const Operation*>& SearchingOperations)
	{
#ifdef DumpData2
		static int Counter2 = 0;
		Counter2++;
		ofstream fout("C:\\test.txt", ios::app);
		fout << "AchieveAll-FinalStates: " << Counter2 << endl;
		GPS::Dump(FinalStates, fout);
#endif
		set<State>::const_iterator it = FinalStates.begin();
		for(; it != FinalStates.end(); it++){
			if(!Achieve(States, *it, Opes, ActionHistory, SearchingOperations))
				return false;
		}
		return true;
	}

	/////////////////////////////////////////////////////////////////
	// �J�n��Ԃ̏W��States����A�I�����FinalState�܂ŁA
	// Opes��p���ē��B�ł����true��Ԃ��B
	/////////////////////////////////////////////////////////////////
	bool Achieve(set<State>& States,
					  const State& FinalState,
					  const set<Operation>& Opes,
					  RunActionHistory& ActionHistory,
					  set<const Operation*>& SearchingOperations)
	{
		if(IsMember(FinalState, States))
			return true;

		set<const Operation*> OpesWhichAddhasFinal;
		GetOpesWhichAddhasSpecifiedState(FinalState, Opes, OpesWhichAddhasFinal, SearchingOperations);
		set<const Operation*>::const_iterator it = OpesWhichAddhasFinal.begin();
		for(; it != OpesWhichAddhasFinal.end(); it++){
			SearchingOperations.insert(*it);
			if(AchieveAll(States, (*it)->GetPreStates(), Opes, ActionHistory, SearchingOperations)){
				ApplyOn(*(*it), States, ActionHistory, SearchingOperations);
				return true;
			}
		}
		return false;
	}

	StatusType
	GPSRunner::Do(const set<State>& InitialStates,
				  const set<State>& FinalStates,
				  const set<Operation>& Opes,
				  RunActionHistory& ActionHistory)
	{
		set<const Operation*> SearchingOperations;
		set<State> tmpStates = InitialStates;
		set<State>::const_iterator it = FinalStates.begin();
		if(AchieveAll(tmpStates, FinalStates, Opes, ActionHistory, SearchingOperations))
			ActionHistory.SetAchievedFlag(true);
		return GPS_NOERROR;
	}

	StatusType
	CreateProcess(const string& CommandLine)
	{
		PROCESS_INFORMATION ProcessInfo;
		STARTUPINFO SI		= { sizeof(STARTUPINFO) };
		SI.dwFlags			= STARTF_USESHOWWINDOW;
		SI.wShowWindow		= SW_SHOWNORMAL;
		if(0 == CreateProcess(NULL, 
							  const_cast<char*>(CommandLine.c_str()),	
							  NULL,										
							  NULL,										
							  FALSE,									
							  0,										
							  NULL,										
							  NULL,										
							  &SI,										
							  &ProcessInfo))							
		{
			return GPS_ERROR;
		}
		return GPS_NOERROR;
	}

	string 
	CreateBatFilePath(const string& XMLFilePath)
	{
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];

		errno_t status = _splitpath_s(XMLFilePath.c_str(), drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, ext, _MAX_EXT);
		if(status != 0)
			return string();

		char path_buffer[_MAX_PATH];
		status = _makepath_s(path_buffer, _MAX_PATH, drive, dir, fname, "bat");
		if(status != 0)
			return string();
		return string(path_buffer);
	}

	void 
	GPSRunner::OutputResult(const set<State>& InitialStates,
							const set<State>& FinalState,
							RunActionHistory& ActionHistory, 
							ostream& out,
							ExecutionManager& ExeManager)
	{
		out << "�������: " << endl;;
		set<State>::const_iterator cit = InitialStates.begin();
		for(; cit != InitialStates.end(); cit++){
			out << "\tState:  " << cit->GetName() << endl;
		}
		out << endl;

		set<State>::const_iterator it = FinalState.begin();
		for(; it != FinalState.end(); it++)
		{
			out << "�I�����: " << endl;
			out << "\tState:  " << it->GetName() << endl;
			out	<< "--------------------------------------------" << endl;
		}

		if(!ActionHistory.GetAchievedFlag())
		{
			cout << "������Ԃ���I����Ԃ������܂���ł����B" << endl;
			cout << "�ȉ��r���܂ł�Action�̎��s���F" << endl;
		}
		else
		{
			cout << "������Ԃ���I����Ԃ𓱂����Ƃɐ������܂����B" << endl;
			cout << "�ȉ�Action�̎��s���F" << endl;
		}
	
		// Action�̗����̏o��
		const vector<const Operation*>& vOpe = ActionHistory.GetOperations();
		for(size_t i = 0; i < vOpe.size(); i++)
		{
			out << "\tAction:  " << vOpe[i]->GetAction().GetName().c_str() << endl;
		}

		// 
		if(GPS::ExecutionManager::Non == ExeManager.GetExecutionMode())
		{
			return;
		}

		cout << "\nAction�ɓo�^���ꂽ���ڂ̎��s�J�n�F" << endl;
		if(GPS::ExecutionManager::ApplyMode == ExeManager.GetExecutionMode())
		{
			for(size_t i = 0; i < vOpe.size(); i++){
				if(vOpe[i]->GetCommandLineFlag()){
					CreateProcess(vOpe[i]->GetCommandLineSt());
					out << "\t" << vOpe[i]->GetAction().GetName().c_str() << " �� ���s��" << endl;
					//Sleep(1000);
				}
			}
		}
		else if(GPS::ExecutionManager::BatFileMode == ExeManager.GetExecutionMode())
		{
			string FileFolder, FileName;
			string FilePath = CreateBatFilePath(this->GetXMLFilePath());
			ofstream fout(FilePath.c_str());
			for(size_t i = 0; i < vOpe.size(); i++){
				if(vOpe[i]->GetCommandLineFlag()){
					fout << vOpe[i]->GetCommandLineSt() << endl;
				}
			}
			out << "bat�t�@�C�����쐬����܂����FbatFilePath = " << FilePath << endl;
		}
		else
		{
			cout << "\n�o�^����Ă��Ȃ����s���[�h�ł��B" << endl;
		}
		cout << "Action�ɓo�^���ꂽ���ڂ̎��s�I���F" << endl;
	}
}