#include<iostream>
#include<Windows.h>
#include<fstream>
#include<string>

using namespace std;

/*
	读取windows注册表中给定键的值
	参数1:	要打开的基键
	参数2: 要读取值的子键路径
	参数3: 要读取的值的名称
	使用 Windows API 函数 RegOpenKeyEx 和 RegQueryValueEx 来打开和读取子键。如果读取值成功，则返回该值的字符串形式；否则返回一个空字符串
*/
string regKeyValue(HKEY hKey, const string& subKey, const string& valueName)
{
	HKEY hSubKey;
	if (RegOpenKeyEx(hKey, subKey.c_str(), 0, KEY_READ, &hSubKey) != ERROR_SUCCESS)
	{
		return "";
	}
	
	DWORD type;
	DWORD dataSize;

	if (RegQueryValueEx(hSubKey, valueName.c_str(), NULL, &type, NULL, &dataSize) != ERROR_SUCCESS || type != REG_SZ)
	{
		RegCloseKey(hSubKey);
		return "";
	}

	char* data = new char[dataSize];
	if (RegQueryValueEx(hSubKey, valueName.c_str(), NULL, NULL, (LPBYTE)data, &dataSize) != ERROR_SUCCESS)
	{
		RegCloseKey(hSubKey);
		delete[] data;
		return "";
	}

	string result(data);
	RegCloseKey(hSubKey);
	delete[] data;
	return result;
}

bool isGitForWindowsInstalled()
{
	string path = regKeyValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\GitForWindows", "InstallPath");
	return !path.empty();
}

/* 下载文件回调函数 */
static int downLoadFileCallBack(void* ptr, size_t sixe, size_t nmemb, void* stearm)
{
	size_t written = fwrite(ptr, sixe, nmemb, (FILE*)stearm);
	return static_cast<int>(written);
}

/* 下载文件函数,libcurl库实现,由于url和fileName都是以字符串形式传入,所以处理的是字符串 */
void downLoadFile(const char* url, const char* fileName)
{

}

void downLoadGitForWindows()
{
	const char* url = "https://github.com/git-for-windows/git/releases/download/v2.40.1.windows.1/Git-2.40.1-64-bit.exe";
	const char* fileName = "Git";
	downLoadFile(url, fileName);
}
