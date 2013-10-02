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
	// ロケール
	_setmode(_fileno(stdout), _O_BINARY);	// cout
	_setmode(_fileno(stderr), _O_BINARY);	// cerr
	setlocale(LC_ALL,"");

	if(argc != 2)
	{
		return -1;
	}

	// フォルダからNCファイル名を集める
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

		// GCode情報への変換
		GCode::GCodeHandler Handler(NCFilePath);
		GCode::StatusType Status = Handler.CreateGCodeStructure();
		if(GCode::NOERROR != Status)
		{
			return -1;
		}

		// GCode情報から点列を生成
		GCode::CPointCreator Creator(Handler);
		GCode::CPointSeq* PointSeq = Creator.Create();
		if(!PointSeq)
		{
			return -1;
		}

		// 点列操作
		GCode::CPointSeqHandler PointSeqHandler(PointSeq);
		PointSeqHandler.SepareteBySpeed();	// 点列を速度別に分離
		PointSeqHandler.CalcLength();		// 速度毎に距離を計算
		PointSeqHandler.CalcMillingTime();	// 速度毎に加工時間を計算

		// 計測結果を出力
		GCode::MeasureResult Result;
		PointSeqHandler.GetDisplayResult(Result);
		Result.m_NCFileName = vNCFileName[i];
		Result.m_NCFolderPath = FolderPath;
		vResult.push_back(Result);
	}

	// 結果を出力
	DisplaymeasureResult(FolderPath, vResult);

	return 0;
}