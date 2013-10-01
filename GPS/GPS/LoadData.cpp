#include "LoadData.h"

#include <io.h>
#include <iostream>
#include <string>
#include <xqilla/xqilla-simple.hpp>

#include<windows.h>
#include "ExecutionManager.h"

////////////////////////////
//wstringをstringへ変換
static std::string 
ConvertWideToMulti(std::wstring const &wstr, int codeMulti = CP_ACP)
{
	int sizeMulti = ::WideCharToMultiByte(codeMulti, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
	if(sizeMulti == 0)return "";
	std::string str;
	str.resize(sizeMulti);
	::WideCharToMultiByte(codeMulti, 0, wstr.c_str(), -1, &str[0], sizeMulti, NULL, NULL);
	return str;
}

////////////////////////////
//stringをwstringへ変換
static std::wstring 
ConvertMultiToWide(std::string const &str, int codeMulti = CP_ACP)
{
	int sizeWide = ::MultiByteToWideChar(codeMulti, 0, str.c_str(), -1, NULL, 0 );
	if(sizeWide == 0)return L"";
	std::wstring wstr;
	wstr.resize(sizeWide);
	::MultiByteToWideChar(codeMulti, 0, str.c_str(), -1, &wstr[0], sizeWide );
	return wstr;
}

////////////////////////////
//属性値の取得に使用：Action="Value"と書かれていたときにValueの部分を取得する
//XPathの記述を工夫すればできると思うのだがやり方がよく分からないため暫定対応
static string
GetValue(const string& Attribute)
{
	std::string::size_type pos = Attribute.rfind("=");
	if(pos == string::npos)
	{
		return string();
	}
	return Attribute.substr(pos + 2, Attribute.length() - pos - 4);
}


namespace GPS
{
	void CreateContext(const wstring& XMLFilePath,
					   DynamicContext* context)
	{
		Sequence seq = context->resolveDocument((XMLCh*)(XMLFilePath.c_str()));
		if(!seq.isEmpty() && seq.first()->isNode()) {
			context->setContextItem(seq.first());
			context->setContextPosition(1);
			context->setContextSize(1);
		}
	}


	// やり方がわからないので何度もファイルを開きなおす
	//void GetQueryResult(const string& XMLFilePath,
	//					const string& QueryString,
	//					vector<string>& vResult)
	//{
	//	vResult.clear();

	//	const wstring wQuery = ConvertMultiToWide(QueryString);
	//	const wstring wXMLFilePath = ConvertMultiToWide(XMLFilePath);

	//	XQilla xqilla;
	//	AutoDelete<XQQuery> query(xqilla.parse((XMLCh*)(wQuery.c_str())));
	//	AutoDelete<DynamicContext> context(query->createDynamicContext());
	//	CreateContext(wXMLFilePath, context.get());
	//	Result result = query->execute(context);
	//	Item::Ptr item;
	//	while(item = result->next(context)) {
	//		vResult.push_back(ConvertWideToMulti(item->asString(context)));
	//	}
	//}

	void GetQueryResult(const string& QueryString,
						DynamicContext* context,
						vector<string>& vResult)
	{
		vResult.clear();

		const wstring wQuery = ConvertMultiToWide(QueryString);

		XQilla xqilla;
		AutoDelete<XQQuery> query(xqilla.parse((XMLCh*)(wQuery.c_str())));
		Result result = query->execute(context);
		Item::Ptr item;
		while(item = result->next(context)) {
			vResult.push_back(ConvertWideToMulti(item->asString(context)).c_str());
		}
	}

	void GetQueryResult(const string& QueryString,
						DynamicContext* context,
						string& Result)
	{
		vector<string> vResult;
		GetQueryResult(QueryString, context, vResult);
		if(1 != vResult.size())
		{
			return;
		}
		Result = vResult.front();
	}

	string CreateCommandLine(const string& ExeFilePath, vector<string>& vArgument)
	{
		string CommandLine = ExeFilePath;
		for(size_t i = 0; i < vArgument.size() ;i++)
			CommandLine = CommandLine + " " + vArgument[i];
		return CommandLine;
	}

	//bool LoadData(const string& XMLFilePath,
	//			  set<State>& InitialStates, 
	//				set<State>& FinalStates, 
	//				set<Operation>& Opes)
	//{
	//	if(0 != _access(XMLFilePath.c_str(), 0))
	//	{
	//		return false;
	//	}

	//	////////////////////////////////
	//	//Operation
	//	////////////////////////////////
	//	// ActionNameを取得
	//	/*vector<string> vActionName;
	//	GetQueryResult(XMLFilePath, "Root/Operation/@Action", vActionName);*/

	//	/////////////////////-DynamicContextの作り方がよく分からないのでここで直書きにして、それを後で使いまわす
	//	vector<string> vActionName;
	//	const wstring wQuery = ConvertMultiToWide(string("Root/Operation/@Action"));
	//	const wstring wXMLFilePath = ConvertMultiToWide(XMLFilePath);
	//	XQilla xqilla;
	//	AutoDelete<XQQuery> query(xqilla.parse((XMLCh*)(wQuery.c_str())));
	//	AutoDelete<DynamicContext> context(query->createDynamicContext());
	//	CreateContext(wXMLFilePath, context.get());
	//	Result result = query->execute(context);
	//	Item::Ptr item;
	//	while(item = result->next(context)) {
	//		vActionName.push_back(ConvertWideToMulti(item->asString(context)));
	//	}
	//	////////////////////

	//	for(size_t i = 0; i < vActionName.size(); i++)
	//	{
	//		const string ActionName = GetValue(vActionName[i]);

	//		// Action
	//		Action Op_act(ActionName);

	//		set<State> Op_pre, Op_add, Op_del;
	//		// PreState
	//		{
	//			vector<string> vPreState;
	//			const string Query_Add = "Root/Operation[@Action=\"" + ActionName + "\"]/State/PreState/text()";
	//			GetQueryResult(XMLFilePath, Query_Add, context, vPreState);				
	//			for(size_t j = 0; j < vPreState.size(); j++)
	//				Op_pre.insert(State(vPreState[j]));
	//		}

	//		// AddState
	//		{
	//			vector<string> vAddState;
	//			const string Query_Add = "Root/Operation[@Action=\"" + ActionName + "\"]/State/AddState/text()";
	//			GetQueryResult(XMLFilePath, Query_Add, context, vAddState);
	//			for(size_t j = 0; j < vAddState.size(); j++)
	//				Op_add.insert(State(vAddState[j]));
	//		}

	//		// DelState
	//		{
	//			vector<string> vDelState;
	//			const string Query_Add = "Root/Operation[@Action=\"" + ActionName + "\"]/State/DelState/text()";
	//			GetQueryResult(XMLFilePath, Query_Add, context, vDelState);
	//			for(size_t j = 0; j < vDelState.size(); j++)
	//				Op_del.insert(State(vDelState[j]));
	//		}
	//		Operation Ope(Op_act, Op_pre, Op_add, Op_del);

	//		// Execution
	//		{
	//			string ExeFilePath;
	//			vector<string> vExeFilePath;
	//			const string Query_ExeFile = "Root/Operation[@Action=\"" + ActionName + "\"]/Execution/ExeFilePath/text()";
	//			GetQueryResult(XMLFilePath, Query_ExeFile, context, vExeFilePath);
	//			if(!vExeFilePath.empty())
	//			{
	//				ExeFilePath = vExeFilePath.front();
	//				vector<string> vArgu;
	//				const string Query_ExeArgu = "Root/Operation[@Action=\"" + ActionName + "\"]/Execution/Argument/text()";
	//				GetQueryResult(XMLFilePath, Query_ExeArgu, context, vArgu);
	//				string CommandLine = CreateCommandLine(ExeFilePath, vArgu);
	//				Ope.SetCommandLine(CommandLine);
	//			}
	//		}

	//		Opes.insert(Ope);
	//		//Opes.insert(Operation(Op_act, Op_pre, Op_add, Op_del));
	//	}

	//	////////////////////////////////
	//	//InitialState
	//	////////////////////////////////
	//	{
	//		vector<string> vInitialState;
	//		const string Query_init = "Root/InitialState/text()";
	//		GetQueryResult(XMLFilePath, Query_init, context, vInitialState);
	//		for(size_t j = 0; j < vInitialState.size(); j++)
	//			InitialStates.insert(State(vInitialState[j]));
	//	}	

	//	////////////////////////////////
	//	//FinalState
	//	////////////////////////////////
	//	{
	//		vector<string> vFinalState;
	//		const string Query_fin = "Root/FinalState/text()";
	//		GetQueryResult(XMLFilePath, Query_fin, context, vFinalState);
	//		for(size_t j = 0; j < vFinalState.size(); j++)
	//			FinalStates.insert(State(vFinalState[j]));
	//	}

	//	return true;
	//}

	static bool
	CreateExecutionMode(const string& ExecutionModeString,
						GPS::ExecutionManager::ExecutionMode& mode)
	{
		if(0 == ExecutionModeString.compare("ApplyMode")){
			mode = GPS::ExecutionManager::ApplyMode;
			return true;
		}
		else if(0 == ExecutionModeString.compare("BatFileMode")){
			mode = GPS::ExecutionManager::BatFileMode;
			return true;
		}
		return false;
	}

	bool LoadData(const string& XMLFilePath,
				  set<State>& InitialStates, 
				  set<State>& FinalStates, 
				  set<Operation>& Opes,
				  ExecutionManager& ExeManager)
	{
		if(0 != _access(XMLFilePath.c_str(), 0))
		{
			return false;
		}

		////////////////////////////////
		//Operation
		////////////////////////////////
		// ActionNameを取得
		/*vector<string> vActionName;
		GetQueryResult(XMLFilePath, "Root/Operation/@Action", vActionName);*/

		/////////////////////-DynamicContextの作り方がよく分からないのでここで直書きにして、それを後で使いまわす
		vector<string> vActionName;
		const wstring wQuery = ConvertMultiToWide(string("Root/Operation/@Action"));
		const wstring wXMLFilePath = ConvertMultiToWide(XMLFilePath);
		XQilla xqilla;
		AutoDelete<XQQuery> query(xqilla.parse((XMLCh*)(wQuery.c_str())));
		AutoDelete<DynamicContext> context(query->createDynamicContext());
		CreateContext(wXMLFilePath, context.get());
		Result result = query->execute(context);
		Item::Ptr item;
		while(item = result->next(context)) {
			vActionName.push_back(ConvertWideToMulti(item->asString(context)));
		}
		////////////////////

		for(size_t i = 0; i < vActionName.size(); i++)
		{
			const string ActionName = GetValue(vActionName[i]);

			// Action
			Action Op_act(ActionName);

			set<State> Op_pre, Op_add, Op_del;
			// PreState
			{
				vector<string> vPreState;
				const string Query_Add = "Root/Operation[@Action=\"" + ActionName + "\"]/Condition/PreCondition/Positive/text()";
				GetQueryResult(Query_Add, context, vPreState);				
				for(size_t j = 0; j < vPreState.size(); j++)
					Op_pre.insert(State(vPreState[j]));
			}

			// AddState
			{
				vector<string> vAddState;
				const string Query_Add = "Root/Operation[@Action=\"" + ActionName + "\"]/Condition/PostCondition/Add/text()";
				GetQueryResult(Query_Add, context, vAddState);
				for(size_t j = 0; j < vAddState.size(); j++)
					Op_add.insert(State(vAddState[j]));
			}

			// DelState
			{
				vector<string> vDelState;
				const string Query_Add = "Root/Operation[@Action=\"" + ActionName + "\"]/Condition/PostCondition/Del/text()";
				GetQueryResult(Query_Add, context, vDelState);
				for(size_t j = 0; j < vDelState.size(); j++)
					Op_del.insert(State(vDelState[j]));
			}
			Operation Ope(Op_act, Op_pre, Op_add, Op_del);

			// Execution
			{
				// ExeFilePath
				string ExeFilePath;
				vector<string> vExeFilePath;
				const string Query_ExeFile = "Root/Operation[@Action=\"" + ActionName + "\"]/Execution/ExeFilePath/text()";
				GetQueryResult(Query_ExeFile, context, vExeFilePath);
				if(!vExeFilePath.empty())
				{
					ExeFilePath = vExeFilePath.front();

					// Argument
					vector<string> vArgu;
					const string Query_ExeArgu = "Root/Operation[@Action=\"" + ActionName + "\"]/Execution/Argument/text()";
					GetQueryResult(Query_ExeArgu, context, vArgu);
					string CommandLine = CreateCommandLine(ExeFilePath, vArgu);
					Ope.SetCommandLine(CommandLine);
				}
				else
				{
					// CommandLine
					string CommandLine;
					const string Query_ComFile = "Root/Operation[@Action=\"" + ActionName + "\"]/Execution/CommandLine/text()";
					GetQueryResult(Query_ComFile, context, CommandLine);
					Ope.SetCommandLine(CommandLine);
				}
			}

			Opes.insert(Ope);
			//Opes.insert(Operation(Op_act, Op_pre, Op_add, Op_del));
		}

		////////////////////////////////
		//InitialCondition
		////////////////////////////////
		{
			vector<string> vInitialState;
			const string Query_init = "Root/InitialCondition/text()";
			GetQueryResult(Query_init, context, vInitialState);
			for(size_t j = 0; j < vInitialState.size(); j++)
				InitialStates.insert(State(vInitialState[j]));
		}	

		////////////////////////////////
		//FinalCondition
		////////////////////////////////
		{
			vector<string> vFinalState;
			const string Query_fin = "Root/FinalCondition/text()";
			GetQueryResult(Query_fin, context, vFinalState);
			for(size_t j = 0; j < vFinalState.size(); j++)
				FinalStates.insert(State(vFinalState[j]));
		}

		////////////////////////////////
		// Output
		////////////////////////////////
		{
			string ExecutionModest;
			const string Query_ExeFile = "Root/Output/ExecutionMode/text()";
			GetQueryResult(Query_ExeFile, context, ExecutionModest);
			if(ExecutionModest.empty())	
				ExeManager.SetExecutionMode(GPS::ExecutionManager::Non);
			else{
				cout << "ExecutionModest: " << ExecutionModest << endl;
				GPS::ExecutionManager::ExecutionMode Mode;
				if(CreateExecutionMode(ExecutionModest, Mode)){
					ExeManager.SetExecutionMode(Mode);
				}else{
					ExeManager.SetExecutionMode(GPS::ExecutionManager::Non);
				}
			}

		}

		return true;
	}
}