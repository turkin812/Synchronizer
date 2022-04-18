#pragma once

bool IsMain = false;

const int BUFFERSIZE = 4096;

const CString Event1Name = L"StartEvent";
const CString Event2Name = L"EditEvent";
const CString Event3Name = L"ExitEvent";

const CString FileName = L"datafile.txt";
const LPCSTR FileMapName = "datafileMap";

HANDLE StartEvent;
HANDLE EditEvent;
HANDLE ExitEvent;
HANDLE Events[2];

HANDLE PrintWaitingThread;
HANDLE ExitWaitingThread;

CString BufferText;
wchar_t* TextInMemory;

HANDLE fileMap;

HWND hwnd;