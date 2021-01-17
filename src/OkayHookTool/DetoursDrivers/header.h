#include <windows.h>
#include <winsock2.h>
#include <tchar.h>
#include <locale.h>

#include <string>
#include <vector>

#include <detours.h>

#ifndef snprintf
#define snprintf _snprintf
#endif

#define LOGG_FILE_NAME	"c:\\file_"
#define LOGG_FILE_EXT	".log"

static int G_nFD = 0;
static long G_ltv_sec = 0;

#define MAX_BASE_SIZE   128
#define MAX_NAME_SIZE   256
#define MAX_CMDS_SIZE   1024
#define MAX_DATA_SIZE   16384

// TODO: reference additional headers your program requires here

class ConsoleDebugUtils {
private:
	FILE* pFIN;
	FILE* pFOUT;
	FILE* pFERR;
public:
	//初始化调试窗口显示
	void InitDebugConsole()
	{
		if (!AllocConsole())
		{
			SetConsoleTitle(TEXT("TraceDebugWindow"));

			pFIN = _tfreopen(TEXT("CONIN$"), TEXT("rb"), stdin);
			pFOUT = _tfreopen(TEXT("CONOUT$"), TEXT("wb"), stdout);
			pFERR = _tfreopen(TEXT("CONOUT$"), TEXT("wb"), stderr);
			_tsetlocale(LC_ALL, TEXT("chs"));
		}
	}

	//释放掉调试窗口显示
	void ExitDebugConsole()
	{
		if (pFERR)
		{
			fclose(pFERR);
			pFERR = NULL;
		}
		if (pFOUT)
		{
			fclose(pFOUT);
			pFOUT = NULL;
		}
		if (pFIN)
		{
			fclose(pFIN);
			pFIN = NULL;
		}
		FreeConsole();
	}
public:
	static ConsoleDebugUtils* Instance()
	{
		static ConsoleDebugUtils cduInstance;
		return &cduInstance;
	}
};
class StringConvertUtils {

#if !defined(_UNICODE) && !defined(UNICODE)
#define tstring std::string
#else
#define tstring std::wstring
#endif

public:
	//	ANSI to Unicode
	std::wstring AToW(const std::string str)
	{
		int len = 0;
		len = str.length();
		int unicodeLen = ::MultiByteToWideChar(CP_ACP,
			0,
			str.c_str(),
			-1,
			NULL,
			0);
		wchar_t* pUnicode;
		pUnicode = new  wchar_t[(unicodeLen + 1)];
		memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
		::MultiByteToWideChar(CP_ACP,
			0,
			str.c_str(),
			-1,
			(LPWSTR)pUnicode,
			unicodeLen);
		std::wstring rt;
		rt = (wchar_t*)pUnicode;
		delete pUnicode;
		return rt;
	}
	//Unicode to ANSI
	std::string WToA(const std::wstring str)
	{
		char* pElementText;
		int iTextLen;
		iTextLen = WideCharToMultiByte(CP_ACP,
			0,
			str.c_str(),
			-1,
			NULL,
			0,
			NULL,
			NULL);
		pElementText = new char[iTextLen + 1];
		memset((void*)pElementText, 0, sizeof(char) * (iTextLen + 1));
		::WideCharToMultiByte(CP_ACP,
			0,
			str.c_str(),
			-1,
			pElementText,
			iTextLen,
			NULL,
			NULL);
		std::string strText;
		strText = pElementText;
		delete[] pElementText;
		return strText;
	}
	//UTF-8 to Unicode
	std::wstring UTF8ToW(const std::string str)
	{
		int len = 0;
		len = str.length();
		int unicodeLen = ::MultiByteToWideChar(CP_UTF8,
			0,
			str.c_str(),
			-1,
			NULL,
			0);
		wchar_t* pUnicode;
		pUnicode = new wchar_t[unicodeLen + 1];
		memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
		::MultiByteToWideChar(CP_UTF8,
			0,
			str.c_str(),
			-1,
			(LPWSTR)pUnicode,
			unicodeLen);
		std::wstring rt;
		rt = (wchar_t*)pUnicode;
		delete pUnicode;
		return rt;
	}
	//Unicode to UTF-8
	std::string WToUTF8(const std::wstring str)
	{
		char* pElementText;
		int iTextLen;
		iTextLen = WideCharToMultiByte(CP_UTF8,
			0,
			str.c_str(),
			-1,
			NULL,
			0,
			NULL,
			NULL);
		pElementText = new char[iTextLen + 1];
		memset((void*)pElementText, 0, sizeof(char) * (iTextLen + 1));
		::WideCharToMultiByte(CP_UTF8,
			0,
			str.c_str(),
			-1,
			pElementText,
			iTextLen,
			NULL,
			NULL);
		std::string strText;
		strText = pElementText;
		delete[] pElementText;
		return strText;
	}

	std::string TToA(tstring tsT)
	{
		std::string str = "";

#if !defined(UNICODE) && !defined(_UNICODE)
		str = tsT;
#else
		str = UnicodeToANSI(tsT);
#endif

		return str;
	}
	std::wstring TToW(tstring tsT)
	{
		std::wstring wstr = L"";

#if !defined(UNICODE) && !defined(_UNICODE)
		wstr = AToW(tsT);
#else
		wstr = tsT;
#endif

		return wstr;
	}
	tstring AToT(std::string str)
	{
		tstring ts = _T("");

#if !defined(UNICODE) && !defined(_UNICODE)
		ts = str;
#else
		ts = ANSIToUnicode(str);
#endif

		return ts;
	}
	tstring WToT(std::wstring wstr)
	{
		tstring ts = _T("");

#if !defined(UNICODE) && !defined(_UNICODE)
		ts = WToA(wstr);
#else
		ts = wstr;
#endif

		return ts;
	}
public:
	static StringConvertUtils* Instance()
	{
		static StringConvertUtils scuInstance;
		return &scuInstance;
	}
};

//整型转换为字符串类型
__inline static std::string STRING_FROM_INT(int v) { CHAR tzV[MAX_BASE_SIZE] = { 0 }; memset(tzV, 0, sizeof(tzV)); snprintf(tzV, sizeof(tzV) / sizeof(CHAR), ("%d"), v); return std::string(tzV); }
__inline static std::string STRING_FROM_LONG(long v) { CHAR tzV[MAX_BASE_SIZE] = { 0 }; memset(tzV, 0, sizeof(tzV)); snprintf(tzV, sizeof(tzV) / sizeof(CHAR), ("%ld"), v); return std::string(tzV); }
__inline static std::string STRING_FROM_FLOART(float v) { CHAR tzV[MAX_BASE_SIZE] = { 0 }; memset(tzV, 0, sizeof(tzV)); snprintf(tzV, sizeof(tzV) / sizeof(CHAR), ("%lf"), v); return std::string(tzV); }
__inline static std::string STRING_FROM_HEX(unsigned long v) { CHAR tzV[MAX_BASE_SIZE] = { 0 }; memset(tzV, 0, sizeof(tzV)); snprintf(tzV, sizeof(tzV) / sizeof(CHAR), ("0x%x"), v); return std::string(tzV); }
__inline static std::wstring WSTRING_FROM_INT(int v) { WCHAR tzV[MAX_BASE_SIZE] = { 0 }; memset(tzV, 0, sizeof(tzV)); _snwprintf(tzV, sizeof(tzV) / sizeof(WCHAR), (L"%d"), v); return std::wstring(tzV); }
__inline static std::wstring WSTRING_FROM_LONG(long v) { WCHAR tzV[MAX_BASE_SIZE] = { 0 }; memset(tzV, 0, sizeof(tzV)); _snwprintf(tzV, sizeof(tzV) / sizeof(WCHAR), (L"%ld"), v); return std::wstring(tzV); }
__inline static std::wstring WSTRING_FROM_FLOART(float v) { WCHAR tzV[MAX_BASE_SIZE] = { 0 }; memset(tzV, 0, sizeof(tzV)); _snwprintf(tzV, sizeof(tzV) / sizeof(WCHAR), (L"%lf"), v); return std::wstring(tzV); }
__inline static std::wstring WSTRING_FROM_HEX(unsigned long v) { WCHAR tzV[MAX_BASE_SIZE] = { 0 }; memset(tzV, 0, sizeof(tzV)); _snwprintf(tzV, sizeof(tzV) / sizeof(WCHAR), (L"0x%x"), v); return std::wstring(tzV); }

__inline static tstring TSTRING_FROM_INT(int v) { _TCHAR tzV[MAX_BASE_SIZE] = { 0 }; memset(tzV, 0, sizeof(tzV)); _sntprintf(tzV, sizeof(tzV) / sizeof(_TCHAR), _T("%d"), v); return tstring(tzV); }
__inline static tstring TSTRING_FROM_LONG(long v) { _TCHAR tzV[MAX_BASE_SIZE] = { 0 }; memset(tzV, 0, sizeof(tzV)); _sntprintf(tzV, sizeof(tzV) / sizeof(_TCHAR), _T("%ld"), v); return tstring(tzV); }
__inline static tstring TSTRING_FROM_FLOART(float v) { _TCHAR tzV[MAX_BASE_SIZE] = { 0 }; memset(tzV, 0, sizeof(tzV)); _sntprintf(tzV, sizeof(tzV) / sizeof(_TCHAR), _T("%lf"), v); return tstring(tzV); }
__inline static tstring TSTRING_FROM_HEX(unsigned long v) { _TCHAR tzV[MAX_BASE_SIZE] = { 0 }; memset(tzV, 0, sizeof(tzV)); _sntprintf(tzV, sizeof(tzV) / sizeof(_TCHAR), _T("0x%x"), v); return tstring(tzV); }

#include <time.h>
//根据秒时间获取日期
__inline static std::string DATE_FROM_TIME(time_t tv_sec) { CHAR tzV[MAX_BASE_SIZE] = { 0 }; struct tm * tm = localtime(&tv_sec); memset(tzV, 0, sizeof(tzV)); strftime(tzV, sizeof(tzV) / sizeof(CHAR), ("%Y%m%d"), tm); return std::string(tzV); }
//根据秒时间获取精确微秒时间
__inline static std::string STRING_FROM_TIME(struct timeval * ptv) { time_t tt = ptv->tv_sec; struct tm * tm = localtime((const time_t *)&tt); CHAR tzV[MAX_BASE_SIZE] = { 0 }; memset(tzV, 0, sizeof(tzV)); strftime(tzV, sizeof(tzV) / sizeof(CHAR), ("%Y-%m-%d %H:%M:%S"), tm); return std::string(tzV) + (".") + STRING_FROM_LONG(ptv->tv_usec); }

//根据秒时间获取日期
__inline static std::wstring W_DATE_FROM_TIME(time_t tv_sec) { WCHAR tzV[MAX_BASE_SIZE] = { 0 }; struct tm * tm = localtime(&tv_sec); memset(tzV, 0, sizeof(tzV)); wcsftime(tzV, sizeof(tzV) / sizeof(_TCHAR), (L"%Y%m%d"), tm); return std::wstring(tzV); }
//根据秒时间获取精确微秒时间
__inline static std::wstring WSTRING_FROM_TIME(struct timeval * ptv) { time_t tt = ptv->tv_sec; struct tm * tm = localtime((const time_t *)&tt); WCHAR tzV[MAX_BASE_SIZE] = { 0 }; memset(tzV, 0, sizeof(tzV)); wcsftime(tzV, sizeof(tzV) / sizeof(WCHAR), (L"%Y-%m-%d %H:%M:%S"), tm); return std::wstring(tzV) + (L".") + WSTRING_FROM_LONG(ptv->tv_usec); }

//根据秒时间获取日期
__inline static tstring T_DATE_FROM_TIME(time_t tv_sec) { _TCHAR tzV[MAX_BASE_SIZE] = { 0 }; struct tm * tm = localtime(&tv_sec); memset(tzV, 0, sizeof(tzV)); _tcsftime(tzV, sizeof(tzV) / sizeof(_TCHAR), _T("%Y%m%d"), tm); return tstring(tzV); }
//根据秒时间获取精确微秒时间
__inline static tstring TSTRING_FROM_TIME(struct timeval * ptv) { time_t tt = ptv->tv_sec; struct tm * tm = localtime((const time_t *)&tt); _TCHAR tzV[MAX_BASE_SIZE] = { 0 }; memset(tzV, 0, sizeof(tzV)); _tcsftime(tzV, sizeof(tzV) / sizeof(_TCHAR), _T("%Y-%m-%d %H:%M:%S"), tm); return tstring(tzV) + _T(".") + STRING_FROM_LONG(ptv->tv_usec); }

#define BUFFERSIZE 256
#define VERBOSE 1
#define PCHAR CHAR*
#define PWCHAR WCHAR*

#include <fcntl.h>
#ifndef fsync
#include <io.h>
#define fsync _commit
#endif // !fsync
#ifndef gettimeofday
__inline static int gettimeofday(struct timeval* tv)
{
	typedef union {
		FILETIME filetime;
		unsigned long long nanotime;
	} NANOTIME;
	NANOTIME nt = { 0 };
	::GetSystemTimeAsFileTime(&nt.filetime);
	tv->tv_usec = (long)((nt.nanotime / 10ULL) % 1000000ULL);
	tv->tv_sec = (long)((nt.nanotime - 116444736000000000ULL) / 10000000ULL);

	return (0);
}
#endif // !gettimeofday

//记录日志接口
__inline static void DebugPrint(int fd, const void * data, unsigned long size)
{
	write(fd, data, size);
	fsync(fd);
}

//记录日志接口
__inline static void DebugPrint(int fd, const char * data, struct timeval * tv)
{
	std::string strLog = std::string("[") + STRING_FROM_TIME(tv) + "] " + data + "\n";
	DebugPrint(fd, strLog.c_str(), strLog.length());
}

//记录日志接口
__inline static void DebugPrint(int & fd, const char * logfilename, const char * pLogInfo)
{
	struct  timeval  tv = { 0 };
	gettimeofday(&tv);
	if (fd <= 0)
	{
		//不是今天
		if (DATE_FROM_TIME(G_ltv_sec).compare(DATE_FROM_TIME(tv.tv_sec)))
		{
			G_ltv_sec = tv.tv_sec;

			//fd = _topen(logfilename, O_CREAT | O_TRUNC | O_RDWR);
			if (access((logfilename + DATE_FROM_TIME(G_ltv_sec) + LOGG_FILE_EXT).c_str(), 0) == 0)
			{
				fd = _topen((logfilename + DATE_FROM_TIME(G_ltv_sec) + LOGG_FILE_EXT).c_str(), O_CREAT | O_APPEND | O_RDWR, 0777);
			}
			else
			{
				fd = _topen((logfilename + DATE_FROM_TIME(G_ltv_sec) + LOGG_FILE_EXT).c_str(), O_CREAT | O_TRUNC | O_RDWR, 0777);
			}
		}

		if (fd <= 0)
		{
			//fd = _topen(logfilename, O_CREAT | O_APPEND | O_RDWR);
			fd = _topen((logfilename + DATE_FROM_TIME(G_ltv_sec) + LOGG_FILE_EXT).c_str(), O_CREAT | O_APPEND | O_RDWR, 0777);
		}
	}
	if (fd > 0)
	{
		DebugPrint(fd, pLogInfo, &tv);
	}
}
//记录日志接口
__inline static void DebugPrintC(const char * pLogInfo)
{
	DebugPrint(G_nFD, LOGG_FILE_NAME, pLogInfo);
}
//记录日志接口
__inline static void DebugPrintString(const std::string& strLogInfo)
{
	DebugPrintC(strLogInfo.c_str());
}

/*
* surcharge log()
*/
__inline static void logV(PCHAR param1, PCHAR param2) {
	char out[BUFFERSIZE];
	sprintf_s(out, BUFFERSIZE, "[HOOK] %s(%s)", param1, param2);
	out[BUFFERSIZE - 1] = '\0';
	printf("%s\n", out);
	OutputDebugStringA(out);
}

__inline static void logV(PWCHAR param1, PWCHAR param2) {
	char out[BUFFERSIZE];
	sprintf_s(out, BUFFERSIZE, "[HOOK] %ws(%ws)", param1, param2);
	out[BUFFERSIZE - 1] = '\0';
	printf("%s\n", out);
	OutputDebugStringA(out);
}

__inline static void log(PCHAR param1, ULONG param2) {
	char out[BUFFERSIZE];
	sprintf_s(out, BUFFERSIZE, "[Hook] %s(%d)", param1, param2);
	out[BUFFERSIZE - 1] = '\0';
	printf("%s\n", out);
	OutputDebugStringA(out);
}
void log(PWCHAR param1, ULONG param2) {
	char out[BUFFERSIZE];
	sprintf_s(out, BUFFERSIZE, "[Hook] %ws(%d)", param1, param2);
	out[BUFFERSIZE - 1] = '\0';
	printf("%s\n", out);
	OutputDebugStringA(out);
}

__inline static void log(PCHAR param1, UINT param2) {
	char out[BUFFERSIZE];
	sprintf_s(out, BUFFERSIZE, "[Hook] %s(%d)", param1, param2);
	out[BUFFERSIZE - 1] = '\0';
	printf("%s\n", out);
	OutputDebugStringA(out);
}
__inline static void log(PWCHAR param1, UINT param2) {
	char out[BUFFERSIZE];
	sprintf_s(out, BUFFERSIZE, "[Hook] %ws(%d)", param1, param2);
	out[BUFFERSIZE - 1] = '\0';
	printf("%s\n", out);
	OutputDebugStringA(out);
}

__inline static void log(PCHAR param1, CONST PCHAR param2) {
	char out[BUFFERSIZE];
	sprintf_s(out, BUFFERSIZE, "[Hook] %s(%s)", param1, param2);
	out[BUFFERSIZE - 1] = '\0';
	printf("%s\n", out);
	OutputDebugStringA(out);
}
__inline static void log(PWCHAR param1, CONST PWCHAR param2) {
	char out[BUFFERSIZE];
	sprintf_s(out, BUFFERSIZE, "[Hook] %ws(%ws)", param1, param2);
	out[BUFFERSIZE - 1] = '\0';
	printf("%s\n", out);
	OutputDebugStringA(out);
}

__inline static void log(PCHAR param1, HINSTANCE param2, CONST PCHAR param3) {
	char out[BUFFERSIZE];
	sprintf_s(out, BUFFERSIZE, "[Hook] %s(%p, %s)", param1, param2, param3);
	out[BUFFERSIZE - 1] = '\0';
	printf("%s\n", out);
	OutputDebugStringA(out);
}

__inline static void log(PWCHAR param1, HINSTANCE param2, CONST PWCHAR param3) {
	char out[BUFFERSIZE];
	sprintf_s(out, BUFFERSIZE, "[Hook] %ws(%p, %ws)", param1, param2, param3);
	out[BUFFERSIZE - 1] = '\0';
	printf("%s\n", out);
	OutputDebugStringA(out);
}

typedef enum ENUM_TYPE_ARGS {
	ET_ARGS_NULL = 0,
	ET_ARGS_CHAR,
	ET_ARGS_WCHAR,
	ET_ARGS_PCHAR,
	ET_ARGS_PWCHAR,
	ET_ARGS_INT,
	ET_ARGS_UINT,
	ET_ARGS_LONG,
	ET_ARGS_ULONG,
	ET_ARGS_LONGLONG,
	ET_ARGS_ULONGLONG,
	ET_ARGS_FLOAT,
	ET_ARGS_DOUBLE,
};
class CCommonParams {
public:
	ENUM_TYPE_ARGS type;
	std::string data;
};
typedef std::vector<CCommonParams> COMMONPARAMSVECTOR;
typedef std::vector<CCommonParams>::iterator COMMONPARAMSVECTORIT;

#define STRING_FORMAT(format) char cValue[MAX_BASE_SIZE] = { 0 }; memcpy(&tValue, it->data.c_str(), sizeof(tValue)); snprintf(cValue, sizeof(cValue), format, tValue); strTemp = cValue;

__inline static void log(COMMONPARAMSVECTOR * pCPV) {
	 
	if (pCPV)
	{
		std::string strTemp = "";
		std::string strData = "";		
		COMMONPARAMSVECTORIT it = pCPV->begin();
		COMMONPARAMSVECTORIT itEnd = pCPV->end();
		strData.append("(");
		for (; it != itEnd; it++)
		{
			strTemp = "";
			switch (it->type)
			{
			case ET_ARGS_PCHAR:
			{
				strTemp = it->data;
			}
			break;
			case ET_ARGS_PWCHAR:
			{
				strTemp = StringConvertUtils::Instance()->WToA((PWCHAR)(it->data.c_str()));
			}
			break;
			case ET_ARGS_CHAR:
			{
				strTemp = it->data;
			}
			break;
			case ET_ARGS_WCHAR:
			{
				strTemp = StringConvertUtils::Instance()->WToA((PWCHAR)(it->data.c_str()));
			}
			break;
			case ET_ARGS_INT:
			{
				int tValue = 0;	STRING_FORMAT("%d")
			}
			break;
			case ET_ARGS_UINT:
			{
				int tValue = 0;	STRING_FORMAT("0x%x")
			}
			break;
			case ET_ARGS_LONG:
			{
				long tValue = 0; STRING_FORMAT("%ld")
			}
			break;
			case ET_ARGS_ULONG:
			{
				long tValue = 0; STRING_FORMAT("0x%x")
			}
			break;			
			case ET_ARGS_LONGLONG:
			{
				__int64 tValue = 0;	STRING_FORMAT("%lld")
			}
			break;
			case ET_ARGS_ULONGLONG:
			{
				__int64 tValue = 0;	STRING_FORMAT("0x%I64x")
			}
			break;
			case ET_ARGS_FLOAT:
			{
				float tValue = 0.0f; STRING_FORMAT("%lf")
			}
			break;
			case ET_ARGS_DOUBLE:
			{
				double tValue = 0.0f; STRING_FORMAT("%lf")
			}
			break;
			default:
				break;
			}

			if (strData.length() <= sizeof(char))
			{
				strData.append(strTemp);
			}
			else
			{
				strData.append(",\"").append(strTemp).append("\"");
			}
		}
		strData.append(")");
		DebugPrintString(strData);
		printf("%s\n", strData.c_str());
		OutputDebugStringA(strData.c_str());
	}
}
/*
* friendly messages
*/
__inline static void success(PCHAR param1) {
	logV(param1, "SUCCESS");
}
__inline static void failed(PCHAR param1) {
	logV(param1, "FAILED");
}
__inline static void removed(PCHAR param1) {
	logV(param1, "REMOVED");
}
__inline static void success(PWCHAR param1) {
	logV(param1, L"SUCCESS");
}
__inline static void failed(PWCHAR param1) {
	logV(param1, L"FAILED");
}
__inline static void removed(PWCHAR param1) {
	logV(param1, L"REMOVED");
}
/*
* detour main functions
*/
__inline static void detourMonitorAttach(PCHAR func, PVOID* orig, PVOID fake) {
	DetourRestoreAfterWith();
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(orig, fake);
	if (DetourTransactionCommit() == 0) {
		if (VERBOSE)
			success(func);
	}
	else {
		if (VERBOSE)
			failed(func);
	}
}

__inline static void detourMonitorDetach(PCHAR func, PVOID* orig, PVOID fake) {
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(orig, fake);
	DetourTransactionCommit();
	if (VERBOSE)
		removed(func);
}
