#include <string>
#include <fstream>
#include <fcntl.h>	// _O_BINARY
#include <io.h>	// _setmode, _access
#include "GCodeHandler.h"
#include "CGCodePonitCreator.h"
#include "Dump.h"
#include "Utility.h"
#include "FileUtility.h"
#include "CGCodePointSeq.h"
#include "CGCodePointSeqHandler.h"
#include "MeasureResult.h"
using namespace std;

int main(int argc, char* argv[])
{
	// ���P�[��
	_setmode(_fileno(stdout), _O_BINARY);	// cout
	_setmode(_fileno(stderr), _O_BINARY);	// cerr
	setlocale(LC_ALL,"");

	if(argc != 2)
	{
		return -1;
	}

	// �t�H���_����NC�t�@�C�������W�߂�
	const string FolderPath = string(argv[1]);
	vector<string> vNCFileName = GetAllFileName(FolderPath, "*.nc");
	if(vNCFileName.empty())
	{
		return 0;
	}

	vector<GCode::MeasureResult> vResult;

	for(size_t i = 0; i < vNCFileName.size(); ++i)
	{
		const string NCFilePath = GetFilePath(FolderPath, vNCFileName[i]);
		if(NCFilePath.empty())
		{
			continue;
		}

		// GCode���ւ̕ϊ�
		GCode::GCodeHandler Handler(NCFilePath);
		GCode::StatusType Status = Handler.CreateGCodeStructure();
		if(GCode::NOERROR != Status)
		{
			return -1;
		}

		// GCode��񂩂�_��𐶐�
		GCode::CPointCreator Creator(Handler);
		GCode::CPointSeq* PointSeq = Creator.Create();
		if(!PointSeq)
		{
			return -1;
		}

		// �_�񑀍�
		GCode::CPointSeqHandler PointSeqHandler(PointSeq);
		PointSeqHandler.SepareteBySpeed();	// �_��𑬓x�ʂɕ���
		PointSeqHandler.CalcLength();		// ���x���ɋ������v�Z
		PointSeqHandler.CalcMillingTime();	// ���x���ɉ��H���Ԃ��v�Z

		// �v�����ʂ��o��
		GCode::MeasureResult Result;
		PointSeqHandler.GetDisplayResult(Result);
		Result.m_NCFileName = vNCFileName[i];
		Result.m_NCFolderPath = FolderPath;
		vResult.push_back(Result);
	}

	// ���ʂ��o��
	DisplaymeasureResult(FolderPath, vResult);

	return 0;
}