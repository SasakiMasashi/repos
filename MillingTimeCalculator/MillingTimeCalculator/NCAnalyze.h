class NonZeroByteFileSelector
{
private:	
	struct IsNonZeroByteFile : std::unary_function<string, bool>
	{
		bool operator()(const string& FilePath) const
		{
			int fh;
			if( (fh = _open(FilePath.c_str(),_O_RDONLY)) != -1 )
			{
				bool iszero = (0 == _filelength( fh ));
				_close( fh );
				return !iszero;
			}
			_close( fh );
			return false;
		}
	};

public:
	typedef std::vector<string>::const_iterator SourceIterator;
	typedef boost::filter_iterator<IsNonZeroByteFile, SourceIterator> const_iterator;

	NonZeroByteFileSelector(const std::vector<string>& vNCFilePath)
		:m_Begin(IsNonZeroByteFile(), vNCFilePath.begin(), vNCFilePath.end())
		,m_End(IsNonZeroByteFile(), vNCFilePath.end(), vNCFilePath.end())
	{
	}

	const_iterator begin() const { return m_Begin; }
	const_iterator end() const { return m_End; }

private:
	const_iterator m_Begin;
	const_iterator m_End;
};

namespace GCode
{
	class CLineState
	{
	public:
		enum InterpolationType
		{
			G00,
			G01,
			G02,
			G03
		};

		enum PlaneType
		{
			G17,
			G18,
			G19
		};

		CLineState() 
			:m_X(boost::none_t()), 
			 m_Y(boost::none_t()), 
			 m_Z(boost::none_t()), 
			 m_F(boost::none_t()),
			 m_I(boost::none_t()), 
			 m_J(boost::none_t()), 
			 m_K(boost::none_t()),
			 m_IType(boost::none_t()),
			 m_PType(boost::none_t())
		{
		}

		CLineState(const CLineState& State)
		{
			SetState(State, *this);
		}

		~CLineState(){}

		const CLineState& operator=(const CLineState& State)
		{
			SetState(State, *this);
			return *this;
		}

		const CLineState& operator+=(const CLineState& State)
		{
			*this = (*this + State);
			return *this;
		}

		CLineState operator+(const CLineState& State) const
		{
			CLineState ResultState = *this;
			AddState(State, ResultState);
			return ResultState;
		}

	private:
		void AddState(const CLineState& From, CLineState& To) const
		{
			if(From.m_X)		To.m_X		= From.m_X.get();
			if(From.m_Y)		To.m_Y		= From.m_Y.get();
			if(From.m_Z)		To.m_Z		= From.m_Z.get();
			if(From.m_F)		To.m_F		= From.m_F.get();			
			if(From.m_I)		To.m_I		= From.m_I.get();
			if(From.m_J)		To.m_J		= From.m_J.get();
			if(From.m_K)		To.m_K		= From.m_K.get();
			if(From.m_IType)	To.m_IType	= From.m_IType.get();
			if(From.m_PType)	To.m_PType	= From.m_PType.get();
		}

		void SetState(const CLineState& From, CLineState& To) const
		{
			AddState(From, To);

			if(!From.m_X)		To.m_X		= boost::none_t();
			if(!From.m_Y)		To.m_Y		= boost::none_t();
			if(!From.m_Z)		To.m_Z		= boost::none_t();
			if(!From.m_F)		To.m_F		= boost::none_t();	
			if(!From.m_I)		To.m_I		= boost::none_t();
			if(!From.m_J)		To.m_J		= boost::none_t();
			if(!From.m_K)		To.m_K		= boost::none_t();
			if(!From.m_IType)	To.m_IType	= boost::none_t();
			if(!From.m_PType)	To.m_PType	= boost::none_t();

			/*To.m_X		= From.m_X;
			To.m_Y		= From.m_Y;
			To.m_Z		= From.m_Z;
			To.m_F		= From.m_F;
			To.m_I		= From.m_I;
			To.m_J		= From.m_J;
			To.m_K		= From.m_K;
			To.m_IType	= From.m_IType;
			To.m_PType	= From.m_PType;*/
		}

	public:
		void SetValue(const char C, const optional<double> Val)
		{
			if(!Val)
			{
				return;
			}
			double val = Val.get();

			switch(C)
			{
			case 'X': m_X = Val; break;
			case 'Y': m_Y = Val; break;
			case 'Z': m_Z = Val; break;
			case 'F': m_F = Val; break;
			case 'I': m_I = Val; break;
			case 'J': m_J = Val; break;
			case 'K': m_K = Val; break;
			case 'G': 
				{
					if(fabs(val - 0.) < IncsCM::Eps)
					{
						m_IType.reset(G00);
						m_I = none_t();
						m_J = none_t();
						m_K = none_t();
					}
					if(fabs(val - 1.) < IncsCM::Eps)
					{
						m_IType.reset(G01);
						m_I = none_t();
						m_J = none_t();
						m_K = none_t();
					}
					if(fabs(val - 2.) < IncsCM::Eps) m_IType.reset(G02);
					if(fabs(val - 3.) < IncsCM::Eps) m_IType.reset(G03);
					if(fabs(val - 17.) < IncsCM::Eps) m_PType.reset(G17);
					if(fabs(val - 18.) < IncsCM::Eps) m_PType.reset(G18);
					if(fabs(val - 19.) < IncsCM::Eps) m_PType.reset(G19);
					break;
				}
			default:
				break;
			}
		}

		void Dump(ofstream& fout)
		{			
			if(m_X) fout << " X=" << m_X.get();
			if(m_Y) fout << " Y=" << m_Y.get();
			if(m_Z) fout << " Z=" << m_Z.get();
			if(m_F) fout << " F=" << m_F.get();
			if(m_I) fout << " I=" << m_I.get();
			if(m_J) fout << " J=" << m_J.get();
			if(m_K) fout << " K=" << m_K.get();
			if(m_IType) 
			{
				switch(m_IType.get())
				{
				case G00: fout << " G=00"; break;
				case G01: fout << " G=01"; break;
				case G02: fout << " G=02"; break;
				case G03: fout << " G=03"; break;
				}
			}
			if(m_PType) 
			{
				switch(m_PType.get())
				{
				case G17: fout << " G=17"; break;
				case G18: fout << " G=18"; break;
				case G19: fout << " G=19"; break;
				}
			}
		}

	public:
		boost::optional<double> m_X;
		boost::optional<double> m_Y;
		boost::optional<double> m_Z;
		boost::optional<double> m_F;
		boost::optional<double> m_I;
		boost::optional<double> m_J;
		boost::optional<double> m_K;
		boost::optional<InterpolationType> m_IType;
		boost::optional<PlaneType> m_PType;
	};

	//class GCodeLineParser;

	class CGcodeLine
	{
	public:
		CGcodeLine(const string& Line, const CLineState& RelativeState, const CLineState& AbsoluteState)
			:m_Line(Line), 
			 m_RelativeState(RelativeState), 
			 m_AbsoluteState(AbsoluteState)
		{}

		/*explicit CGcodeLine(const string& Line)
			:m_Line(Line),
			 m_AbsoluteState()
		{
			m_RelativeState = GCodeLineParser::Parse(Line);
		}*/

		/*explicit CGcodeLine( const CLineState& RelativeState)
			:m_RelativeState(RelativeState),
			 m_AbsoluteState()
		{
			m_Line = CreateLine();
		}*/

	public:
		// 1行の中に各文字が出現するかどうか
		bool HasX() const { return (!!m_RelativeState.m_X); }
		bool HasY() const { return (!!m_RelativeState.m_Y); }
		bool HasZ() const { return (!!m_RelativeState.m_Z); }
		bool HasF() const { return (!!m_RelativeState.m_F); }
		bool HasI() const { return (!!m_RelativeState.m_I); }
		bool HasJ() const { return (!!m_RelativeState.m_J); }
		bool HasK() const { return (!!m_RelativeState.m_K); }
		bool HasG00() const { return (!!m_RelativeState.m_IType) && (m_RelativeState.m_IType.get() == CLineState::G00); }
		bool HasG01() const { return (!!m_RelativeState.m_IType) && (m_RelativeState.m_IType.get() == CLineState::G01); }
		bool HasG02() const { return (!!m_RelativeState.m_IType) && (m_RelativeState.m_IType.get() == CLineState::G02); }
		bool HasG03() const { return (!!m_RelativeState.m_IType) && (m_RelativeState.m_IType.get() == CLineState::G03); }
		bool HasG17() const { return (!!m_RelativeState.m_PType) && (m_RelativeState.m_IType.get() == CLineState::G17); }
		bool HasG18() const { return (!!m_RelativeState.m_PType) && (m_RelativeState.m_IType.get() == CLineState::G18); }
		bool HasG19() const { return (!!m_RelativeState.m_PType) && (m_RelativeState.m_IType.get() == CLineState::G19); }
		bool HasInterplationCommand() const { return (!!m_RelativeState.m_IType);}
		bool HasPlaneSpecifyCommand() const { return (!!m_RelativeState.m_PType);}

		// その行までに定まった値
		bool HasAbsX() const { return (!!m_AbsoluteState.m_X); }
		bool HasAbsY() const { return (!!m_AbsoluteState.m_Y); }
		bool HasAbsZ() const { return (!!m_AbsoluteState.m_Z); }
		bool HasAbsF() const { return (!!m_AbsoluteState.m_F); }
		bool HasAbsI() const { return (!!m_AbsoluteState.m_I); }
		bool HasAbsJ() const { return (!!m_AbsoluteState.m_J); }
		bool HasAbsK() const { return (!!m_AbsoluteState.m_K); }
		bool HasAbsG00() const { return (!!m_AbsoluteState.m_IType) && (m_AbsoluteState.m_IType.get() == CLineState::G00); }
		bool HasAbsG01() const { return (!!m_AbsoluteState.m_IType) && (m_AbsoluteState.m_IType.get() == CLineState::G01); }
		bool HasAbsG02() const { return (!!m_AbsoluteState.m_IType) && (m_AbsoluteState.m_IType.get() == CLineState::G02); }
		bool HasAbsG03() const { return (!!m_AbsoluteState.m_IType) && (m_AbsoluteState.m_IType.get() == CLineState::G03); }
		bool HasAbsG17() const { return (!!m_AbsoluteState.m_PType) && (m_AbsoluteState.m_IType.get() == CLineState::G17); }
		bool HasAbsG18() const { return (!!m_AbsoluteState.m_PType) && (m_AbsoluteState.m_IType.get() == CLineState::G18); }
		bool HasAbsG19() const { return (!!m_AbsoluteState.m_PType) && (m_AbsoluteState.m_IType.get() == CLineState::G19); }
		bool HasAbsInterplationCommand() const { return (!!m_AbsoluteState.m_IType);}
		bool HasAbsPlaneSpecifyCommand() const { return (!!m_AbsoluteState.m_PType);}

		// 以下値が取れることを確認してから使用してください
		double GetX() const { return m_AbsoluteState.m_X.get(); }
		double GetY() const { return m_AbsoluteState.m_Y.get(); }
		double GetZ() const { return m_AbsoluteState.m_Z.get(); }
		double GetI() const { return m_AbsoluteState.m_I.get(); }
		double GetJ() const { return m_AbsoluteState.m_J.get(); }
		double GetK() const { return m_AbsoluteState.m_K.get(); }
		double GetF() const { return m_AbsoluteState.m_F.get(); }

		CLineState::InterpolationType GetInterpolationType() const { return m_AbsoluteState.m_IType.get(); }
		std::string GetInterpolationTypeString() const
		{
			switch(GetInterpolationType())
			{
			case CLineState::G00: return string("G00");
			case CLineState::G01: return string("G01");
			case CLineState::G02: return string("G02");
			case CLineState::G03: return string("G03");
			}
			return string("none");
		}

		CLineState::PlaneType GetPlaneType() const { return m_AbsoluteState.m_PType.get(); }
		std::string GetPlaneTypeString() const
		{
			switch(GetPlaneType())
			{
			case CLineState::G17: return string("G17");
			case CLineState::G18: return string("G18");
			case CLineState::G19: return string("G19");
			}
			return string("none");
		}

		string CreateLine(const int DecimalPlace) const
		{
			stringstream sst;
			if(HasInterplationCommand()) sst << GetInterpolationTypeString();
			if(HasX())	sst << "X" << setprecision(DecimalPlace) << setiosflags(ios::fixed) << GetX();
			if(HasY())	sst << "Y" << setprecision(DecimalPlace) << setiosflags(ios::fixed) << GetY();
			if(HasZ())	sst << "Z" << setprecision(DecimalPlace) << setiosflags(ios::fixed) << GetZ();
			if(HasI())	sst << "I" << setprecision(DecimalPlace) << setiosflags(ios::fixed) << GetI();
			if(HasJ())	sst << "J" << setprecision(DecimalPlace) << setiosflags(ios::fixed) << GetJ();
			if(HasK())	sst << "K" << setprecision(DecimalPlace) << setiosflags(ios::fixed) << GetK();
			if(HasF())	sst << "F" << setprecision(DecimalPlace) << setiosflags(ios::fixed) << GetF();
			return sst.str();
		}

		void ChangeRelativeState(const CLineState& AddState, const int DecimalPlace)
		{
			ofstream fout("ChangeRelativeState.txt", ios::app);
			Dump(fout);

			m_RelativeState += AddState;
			m_AbsoluteState += AddState;
			m_Line = CreateLine(DecimalPlace);
			ClearAbsoluteState();

			Dump(fout);
		}

		void SetValue(const char ch, const double value, const int DecimalPlace)
		{
			m_RelativeState.SetValue(ch, value);
			m_Line = CreateLine(DecimalPlace);
			ClearAbsoluteState();
		}

		bool IsValid() const { return (HasX() || HasY() || HasZ()); }

		void ClearAbsoluteState() {	m_AbsoluteState = CLineState(); }

		std::string GetLineSt() const { return m_Line; }
		const CLineState& GetRelativeState() const { return m_RelativeState; }
		const CLineState& GetAbsoluteState() const { return m_AbsoluteState; }
		void SetRelativeState(const CLineState& State) { m_RelativeState = State; }
		void SetAbsoluteState(const CLineState& State) { m_AbsoluteState = State; }

		void WriteLine(ofstream& fout)
		{
			fout << m_Line << "\n";
		}

		void Dump(ofstream& fout)
		{
			fout << m_Line << ",";
			fout << "\tRelative( ";
			m_RelativeState.Dump(fout);
			fout << " ), ";
			fout << "\tAbsolute( ";
			m_AbsoluteState.Dump(fout);
			fout << " ) \n";
		}

	private:
		std::string m_Line;
		CLineState	m_RelativeState;
		CLineState	m_AbsoluteState;
	};

	class GCodeLineParser
	{
		static optional<double> GetDigit(const string& Line, string::size_type pos)
		{
			if(string::npos == pos || pos >= Line.size())
			{
				return none_t();
			}
			
			string::size_type start = pos;

			char c = 0;
			//while(isdigit(c = char(Line[pos]) || c == '.')) pos++;
			while(pos < Line.size() && !isalpha(c = char(Line[pos]))) pos++;

			string::size_type end = pos;

			string digit = Line.substr(start, end - start);
			if(digit.empty())
			{
				//ERR_LOG(IncsCM::ICM_NOERROR, 0, "Line=%s, startpos=%d, endpos=%d, substr=empty", Line.c_str(), (int)start, (int)end);
				return none_t();
			}

			//ERR_LOG(IncsCM::ICM_NOERROR, 0, "Line=%s, startpos=%d, endpos=%d, substr=%s", Line.c_str(), (int)start, (int)end, digit.c_str());

			return atof(digit.c_str());
		}

	public:
		static CLineState Parse(const string& Line)
		{
			//ERR_LOG(IncsCM::ICM_NOERROR, 0, "GCodeHandler-Parse-start");
			string::size_type pos = 0;
			CLineState State;

			char C = 0;
			while(pos < Line.size())
			{
				C = char(Line[pos]);
				if(C == '(' || C == ':')
				{
					break;
				}

				pos++;

				if(isalpha(C))
				{
					State.SetValue(C, GetDigit(Line, pos));
				}
			}
			//ERR_LOG(IncsCM::ICM_NOERROR, 0, "GCodeHandler-Parse-end");
			return State;
		}
	};

	class GCodeHandler
	{
	public:
		explicit GCodeHandler(const std::string& NCFilePath)
			:m_NCFilePath(NCFilePath), m_HeaderSt("NC_HEADER"), m_FooterSt("NC_FOOTER"), m_DecimalPlaceSt("NCDECIMALPLACE")
		{}

		GCodeHandler(const std::string& NCFilePath, const std::string& HeaderSt, const std::string& FooterSt)
			:m_NCFilePath(NCFilePath), m_HeaderSt(HeaderSt), m_FooterSt(FooterSt), m_DecimalPlaceSt("NCDECIMALPLACE")
		{}

	public:

		struct GCodeStructure
		{
			typedef vector<CGcodeLine> GCodeSection;

			GCodeStructure() : m_DecimalPlace(3)
			{}

			GCodeSection m_vHeaderLines;
			GCodeSection m_vContentLines;
			GCodeSection m_vFooterLines;
			int m_DecimalPlace;
		};

		GCodeStructure::GCodeSection& GetHeader()	{ return m_Structure.m_vHeaderLines; }
		GCodeStructure::GCodeSection& GetContent() { return m_Structure.m_vContentLines; }
		GCodeStructure::GCodeSection& GetFooter()	{ return m_Structure.m_vFooterLines; }

		int GetDecimalPlace() const { return m_Structure.m_DecimalPlace; }

	private:

		int getNumLine(const string& FilePath)
		{
			ifstream fin(FilePath.c_str());
			if(!fin) 
				return 0;

			int num = 0;
			char buf[256];
			while(fin.getline(buf, 256)) 
				num++;

			return num;
		}

		int getDecimalPlaceNum(const string& Line, const string& DecimalPlaceSt)
		{
			int decimalplace = 3;
			string::size_type pos = Line.find(")");
			if(string::npos != pos)
			{
				char num = (char)Line[pos - 1];
				decimalplace = atoi(&num);
			}
			return decimalplace;
		}

		IncsCM::StatusType 
		Load(const string& NCFilePath, 
			 const string& HeaderSt, 
			 const string& FooterSt,
			 const string& DecimalPlaceSt,
			 vector<CGcodeLine>& vHeaderLines, 
			 vector<CGcodeLine>& vContentLine,
			 vector<CGcodeLine>& vFooterLine,
			 int& DecimalPlace)
		{
			//ERR_LOG(IncsCM::ICM_NOERROR, 0, "GCodeHandler-Load-start NCFilePath=%s", NCFilePath.c_str());

			vHeaderLines.clear();
			vContentLine.clear();
			vFooterLine.clear();
			int num = getNumLine(NCFilePath);
			vContentLine.reserve(num);

			//ERR_LOG(IncsCM::ICM_NOERROR, 0, "GCodeHandler-Load-2 rownum=%d", num);

			ifstream fin(NCFilePath.c_str());
			if(!fin)
			{
				return IncsCM::BAD_PARAM;
			}

			//ERR_LOG(IncsCM::ICM_NOERROR, 0, "GCodeHandler-Load-3");

			bool HeaderPart = true;
			bool FooterPart = false;

			CLineState Current;
			while(!fin.eof())
			{
				string stLine;
				getline(fin, stLine);
				if(stLine.empty())
				{
					continue;
				}

				if(string::npos != stLine.find(HeaderSt))
				{			
					vHeaderLines.push_back(CGcodeLine(stLine, CLineState(), Current));
					HeaderPart = false;
					continue;
				}
				if(string::npos != stLine.find(FooterSt))
				{
					FooterPart = true;  
					vFooterLine.push_back(CGcodeLine(stLine, CLineState(), Current));
					continue;
				}
				if(string::npos != stLine.find(DecimalPlaceSt))
				{
					DecimalPlace = getDecimalPlaceNum(stLine, DecimalPlaceSt);
					vContentLine.push_back(CGcodeLine(stLine, CLineState(), Current));
					continue;
				}

				CLineState RelState = GCodeLineParser::Parse(stLine);
				Current += RelState;
				CGcodeLine Line(stLine, RelState, Current);

				/*{
					ofstream fout("Line.txt", ios::app);
					Line.Dump(fout);
				}*/

				if(HeaderPart)					vHeaderLines.push_back(Line);
				if(!HeaderPart && !FooterPart)	vContentLine.push_back(Line);
				if(FooterPart)					vFooterLine.push_back(Line);
			}

			fin.close();

			//ERR_LOG(IncsCM::ICM_NOERROR, 0, "GCodeHandler-Load-end");

			return IncsCM::ICM_NOERROR;
		}

	public:
		IncsCM::StatusType CreateGCodeStructure()
		{
			IncsCM::StatusType Status = Load(m_NCFilePath, 
											 m_HeaderSt, 
											 m_FooterSt,
											 m_DecimalPlaceSt,
											 m_Structure.m_vHeaderLines,
											 m_Structure.m_vContentLines,
											 m_Structure.m_vFooterLines,
											 m_Structure.m_DecimalPlace);
			if(IncsCM::ICM_NOERROR != Status)
			{
				return Status;
			}

			return IncsCM::ICM_NOERROR;
		}

		void WriteOutHeader(ofstream& fout)
		{
			for(size_t i = 0; i < m_Structure.m_vHeaderLines.size(); i++)
			{
				m_Structure.m_vHeaderLines[i].WriteLine(fout);
			}
		}

		void WriteOutContent(ofstream& fout)
		{
			for(size_t i = 0; i < m_Structure.m_vContentLines.size(); i++)
			{
				m_Structure.m_vContentLines[i].WriteLine(fout);
			}
		}

		void WriteOutFooter(ofstream& fout)
		{
			for(size_t i = 0; i < m_Structure.m_vFooterLines.size(); i++)
			{
				m_Structure.m_vFooterLines[i].WriteLine(fout);
			}
		}

		void SetSeparatorSt(const string& HeaderSt, const string& FooterSt)
		{
			m_HeaderSt = HeaderSt;
			m_FooterSt = FooterSt;
		}

		IncsCM::StatusType ChangeFirstEvasionHeight(const double Height)
		{
			//ERR_LOG(IncsCM::ICM_NOERROR, 0, "ChangeFirstEvasionHeight-start");

			// 空の場合は変更できない
			if(m_Structure.m_vContentLines.empty())
			{
				//ERR_LOG(IncsCM::ICM_NOERROR, 0, "ChangeFirstEvasionHeight-vContentLinesが空");
				return IncsCM::ICM_NOERROR;
			}

			// 早送りから始まらなければならない
			if(CLineState::G00 != m_Structure.m_vContentLines.front().GetInterpolationType())
			{
				ERR_LOG(IncsCM::ICM_NOERROR, 0, "ChangeFirstEvasionHeight-vContentLinesは早送りからはじまる必要がある");
				return IncsCM::BAD_PARAM;
			}

			// contentの中からZの記述のある最初の行を探す
			size_t pos = 0;
			while((pos < m_Structure.m_vContentLines.size()) 
				  && (CLineState::G00 == m_Structure.m_vContentLines[pos].GetInterpolationType())
				  && !m_Structure.m_vContentLines[pos].HasZ())
				pos++;

			if(pos == m_Structure.m_vContentLines.size())
			{
				ERR_LOG(IncsCM::ICM_NOERROR, 0, "ChangeFirstEvasionHeight-pos == m_Structure.m_vContentLines.size");
				return IncsCM::BAD_PARAM;
			}

			if(CLineState::G00 != m_Structure.m_vContentLines[pos].GetInterpolationType())
			{
				// ここにくるときは加工開始Z値と工具移動高さが同じでZが省略されているとき
				//ERR_LOG(IncsCM::ICM_NOERROR, 0, "ChangeFirstEvasionHeight-Zを探したらG00でないところまできた");

				pos = 0;
				while(pos < m_Structure.m_vContentLines.size()
					  && !m_Structure.m_vContentLines[pos].IsValid())
					pos++;

				CLineState ZState, XYState;
				ZState.SetValue('Z', Height);
				XYState.SetValue('X', m_Structure.m_vContentLines.front().GetX());
				XYState.SetValue('Y', m_Structure.m_vContentLines.front().GetY());
				CGcodeLine ZLine(string(), ZState, ZState);
				CGcodeLine XYLine(string(), XYState, XYState);
				ZLine.ChangeRelativeState(ZState, m_Structure.m_DecimalPlace);
				XYLine.ChangeRelativeState(XYState, m_Structure.m_DecimalPlace);
				m_Structure.m_vContentLines.insert(m_Structure.m_vContentLines.begin() + pos, ZLine);
				m_Structure.m_vContentLines.insert(m_Structure.m_vContentLines.begin() + pos + 1, XYLine);
			}
			else
			{
				//ERR_LOG(IncsCM::ICM_NOERROR, 0, "ChangeFirstEvasionHeight-ZをG00内で発見");

				size_t zpos = pos;

				CLineState HeightState;
				HeightState.SetValue('Z', Height);
				m_Structure.m_vContentLines[zpos].ChangeRelativeState(HeightState, m_Structure.m_DecimalPlace);
			}

			// 最初の早送り部分だけ絶対評価系を再構築
			//ERR_LOG(IncsCM::ICM_NOERROR, 0, "ChangeFirstEvasionHeight-最初の早送り部分だけ絶対評価系を再構築");

			CLineState Current = m_Structure.m_vHeaderLines.back().GetAbsoluteState();
			for(size_t i = 0; i < m_Structure.m_vContentLines.size(); i++)
			{
				if(CLineState::G00 != m_Structure.m_vContentLines[i].GetInterpolationType())
				{
					break;
				}

				Current += m_Structure.m_vContentLines[i].GetRelativeState();
				m_Structure.m_vContentLines[i].SetAbsoluteState(Current);				
			}

			//ERR_LOG(IncsCM::ICM_NOERROR, 0, "ChangeFirstEvasionHeight-end");
			return IncsCM::ICM_NOERROR;
		}

		IncsCM::StatusType ChangeLastEvasionHeight(const double Height)
		{
			//ERR_LOG(IncsCM::ICM_NOERROR, 0, "ChangeLastEvasionHeight-start");

			// 空の場合は変更できない
			if(m_Structure.m_vContentLines.empty())
			{
				//ERR_LOG(IncsCM::ICM_NOERROR, 0, "ChangeLastEvasionHeight-vContentLinesが空");
				return IncsCM::ICM_NOERROR;
			}

			// G00で終わらなければならない
			if(CLineState::G00 != m_Structure.m_vContentLines.back().GetInterpolationType())
			{
				ERR_LOG(IncsCM::ICM_NOERROR, 0, "ChangeLastEvasionHeight-G00で終わらなければならない");
				return IncsCM::BAD_PARAM;
			}

			int pos = m_Structure.m_vContentLines.size() - 1;
			while(pos >= 0
				  && (CLineState::G00 == m_Structure.m_vContentLines[pos].GetInterpolationType())
				  && (!m_Structure.m_vContentLines[pos].HasZ()))
				pos--;

			if(pos < 0)
			{
				ERR_LOG(IncsCM::ICM_NOERROR, 0, "ChangeLastEvasionHeight-pos < 0");
				return IncsCM::BAD_PARAM;
			}

			int zpos = pos;

			CLineState HeightState;
			HeightState.SetValue('Z', Height);
			m_Structure.m_vContentLines[zpos].ChangeRelativeState(HeightState, m_Structure.m_DecimalPlace);

			// pos以降の最初の早送り部分だけ絶対評価系を再構築
			CLineState Current = m_Structure.m_vContentLines[zpos -1].GetAbsoluteState();
			for(size_t i = zpos; i < m_Structure.m_vContentLines.size(); i++)
			{
				if(CLineState::G00 != m_Structure.m_vContentLines[i].GetInterpolationType())
				{
					break;
				}

				Current += m_Structure.m_vContentLines[i].GetRelativeState();
				m_Structure.m_vContentLines[i].SetAbsoluteState(Current);				
			}

			//ERR_LOG(IncsCM::ICM_NOERROR, 0, "ChangeLastEvasionHeight-end");
			return IncsCM::ICM_NOERROR;
		}

		IncsCM::StatusType GetEndXYZOfFirstEvasion(double& X, double& Y, double& Z)
		{
			// 空の場合は取得できない
			if(m_Structure.m_vContentLines.empty())
			{
				return IncsCM::BAD_PARAM;
			}

			// 早送りから始まらなければならない
			if(CLineState::G00 != m_Structure.m_vContentLines.front().GetInterpolationType())
			{
				return IncsCM::BAD_PARAM;
			}

			// G00でないところまで進める
			size_t pos = 0;
			while((pos < m_Structure.m_vContentLines.size()) 
				  && (CLineState::G00 == m_Structure.m_vContentLines[pos].GetInterpolationType()))
				pos++;

			if(pos == m_Structure.m_vContentLines.size())
			{
				return IncsCM::BAD_PARAM;
			}
			
			X = m_Structure.m_vContentLines[pos - 1].GetX();
			Y = m_Structure.m_vContentLines[pos - 1].GetY();
			Z = m_Structure.m_vContentLines[pos - 1].GetZ();

			return IncsCM::ICM_NOERROR;
		}

		IncsCM::StatusType GetStartXYZOfLastEvasion(double& X, double& Y, double& Z)
		{
			// 空の場合は取得できない
			if(m_Structure.m_vContentLines.empty())
			{
				return IncsCM::BAD_PARAM;
			}

			// G00で終わらなければならない
			if(CLineState::G00 != m_Structure.m_vContentLines.back().GetInterpolationType())
			{
				return IncsCM::BAD_PARAM;
			}

			int pos = m_Structure.m_vContentLines.size() - 1;
			while(pos >= 0
				  && (CLineState::G00 == m_Structure.m_vContentLines[pos].GetInterpolationType()))
				pos--;

			if(pos < 0)
			{
				return IncsCM::BAD_PARAM;
			}

			X = m_Structure.m_vContentLines[pos].GetX();
			Y = m_Structure.m_vContentLines[pos].GetY();
			Z = m_Structure.m_vContentLines[pos].GetZ();

			return IncsCM::ICM_NOERROR;
		}

		void DeleteLastInitialXYLine()
		{
			const double InitialX = m_Structure.m_vContentLines.front().GetX();
			const double InitialY = m_Structure.m_vContentLines.front().GetY();

			int i = m_Structure.m_vContentLines.size() - 1;
		
			while((m_Structure.m_vContentLines[i].GetX() == InitialX) 
				  && (m_Structure.m_vContentLines[i].GetY() == InitialY))
				i--;
			i++;

			m_Structure.m_vContentLines.erase(m_Structure.m_vContentLines.begin() + i);			
		}

		void Dump(ofstream& fout)
		{
			for(size_t i = 0; i < m_Structure.m_vHeaderLines.size(); i++)
			{
				m_Structure.m_vHeaderLines[i].Dump(fout);
			}

			for(size_t i = 0; i < m_Structure.m_vContentLines.size(); i++)
			{
				m_Structure.m_vContentLines[i].Dump(fout);
			}

			for(size_t i = 0; i < m_Structure.m_vFooterLines.size(); i++)
			{
				m_Structure.m_vFooterLines[i].Dump(fout);
			}
		}

	private:
		const std::string m_NCFilePath;
		std::string m_HeaderSt;
		std::string m_FooterSt;
		std::string m_DecimalPlaceSt;

		GCodeStructure m_Structure;
	};
};

class ValidGCodeLineSelector
{
private:	
	struct IsValidGCodeLine : std::unary_function<string, bool>
	{
		bool operator()(const GCode::CGcodeLine& Line) const
		{
			return Line.IsValid();
		}
	};

public:
	typedef std::vector<GCode::CGcodeLine>::iterator SourceIterator;
	typedef boost::filter_iterator<IsValidGCodeLine, SourceIterator> iterator;

	ValidGCodeLineSelector(std::vector<GCode::CGcodeLine>& vGCodeLine)
		:m_Begin(IsValidGCodeLine(), vGCodeLine.begin(), vGCodeLine.end())
		,m_End(IsValidGCodeLine(), vGCodeLine.end(), vGCodeLine.end())
	{}

	iterator begin() { return m_Begin; }
	iterator end() { return m_End; }

private:
	iterator m_Begin;
	iterator m_End;
};

static void
GetHeaderFooterString(string& HeaderSt, string& FooterSt)
{
	// 取りあえずFunucとしておく
	HeaderSt = "(NC_HEADER)";
	FooterSt = "(NC_FOOTER)"
}

//static void
//GetHeaderFooterString(const ICMProcessCondition& Condition, string& HeaderSt, string& FooterSt)
//{
//	// 取りあえずFunucとしておく
//	HeaderSt = "(NC_HEADER)";
//	FooterSt = "(NC_FOOTER)"
//
//	// 機械仕様ファイルから対応する拡張子を取得する
//	string strHeader,strFooter,strComment;
//	CICMGCodeAddition_File* GCodeAdditionFile = NULL;
//	CI3DAutoPtr<CICMGCodeAddition_File*> apGCodeAdditionFile(&GCodeAdditionFile);
//	IncsCM::MakeGCodeAddition(Condition, GCodeAdditionFile);
//	if(NULL == GCodeAdditionFile)
//	{
//		// 機械仕様ファイルが無い場合
//		strHeader  = (IncsCM::NCControllerType.GetHeader(IncsCM::FANUC).c_str());
//		strFooter  = (IncsCM::NCControllerType.GetFooter(IncsCM::FANUC).c_str());
//		strComment = (IncsCM::NCControllerType.GetComment(IncsCM::FANUC).c_str());
//	}
//	else
//	{
//		// 機械仕様ファイルがある場合
//		strHeader  = (IncsCM::NCControllerType.GetHeader(GCodeAdditionFile->GetParseMachineFile()->GetCNC().GetControllerType()).c_str());
//		strFooter  = (IncsCM::NCControllerType.GetFooter(GCodeAdditionFile->GetParseMachineFile()->GetCNC().GetControllerType()).c_str());
//		strComment = (IncsCM::NCControllerType.GetComment(GCodeAdditionFile->GetParseMachineFile()->GetCNC().GetControllerType()).c_str());
//	}
//	HeaderSt = strHeader;
//	FooterSt = strFooter;
//}

