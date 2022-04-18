
// SynchronizerDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "Synchronizer.h"
#include "SynchronizerDlg.h"
#include "afxdialogex.h"
#include "definitions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CSynchronizerDlg



CSynchronizerDlg::CSynchronizerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SYNCHRONIZER_DIALOG, pParent)
	, TextMain(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSynchronizerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITMAIN, EditMain);
	DDX_Text(pDX, IDC_EDITMAIN, TextMain);
	DDX_Control(pDX, IDC_EDITMAIN, EditMain);
	DDX_Text(pDX, IDC_EDITMAIN, TextMain);
}

BEGIN_MESSAGE_MAP(CSynchronizerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDITMAIN, &CSynchronizerDlg::OnEnChangeEditmain)
END_MESSAGE_MAP()


// Обработчики сообщений CSynchronizerDlg
DWORD WINAPI PrintText(LPVOID param);

BOOL CSynchronizerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	hwnd = CWnd::GetSafeHwnd();

	StartEvent = CreateEvent(NULL, TRUE, TRUE, Event1Name);
	EditEvent = CreateEvent(NULL, TRUE, FALSE, Event2Name);
	ExitEvent = CreateEvent(NULL, FALSE, FALSE, Event3Name);
	Events[0] = EditEvent;
	Events[1] = ExitEvent;

	if (WaitForSingleObject(StartEvent, 0) == WAIT_OBJECT_0) {
		IsMain = true;

		ResetEvent(StartEvent);
	}
	else {
		IsMain = false;

		PrintWaitingThread = CreateThread(NULL, 0, PrintText, &EditMain, 0, NULL);

		EditMain.SetReadOnly();
	}

	HANDLE fileMap = CreateFileMappingA(NULL, NULL, PAGE_READWRITE, 0, BUFFERSIZE, FileMapName);
	if (fileMap == NULL) {
		AfxMessageBox(L"Ошибка создания FileMap. Код ошибки: " + GetLastError());
		return FALSE;
	}

	TextInMemory = (wchar_t*)MapViewOfFile(fileMap, FILE_MAP_ALL_ACCESS, 0, 0, BUFFERSIZE);
	if (TextInMemory == NULL) {
		AfxMessageBox(L"Не получилось просмотреть FileMap. Код ошибки:  " + GetLastError());
		return FALSE;
	}

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CSynchronizerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CSynchronizerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

DWORD WINAPI PrintText(LPVOID param) {

	while (true)
	{
		int index = WaitForMultipleObjects(2, Events, FALSE, INFINITE) - WAIT_OBJECT_0;

		if (index == 0) {
			SetWindowTextW(GetDlgItem(hwnd, IDC_EDITMAIN), TextInMemory);
		}
		else {
			IsMain = true;

			CEdit* edit = (CEdit*)param;
			edit->SetReadOnly(FALSE);

			return 0;
		}
	}
}

void CSynchronizerDlg::OnEnChangeEditmain()
{
	UpdateData();

	wcscpy(TextInMemory, TextMain);

	PulseEvent(EditEvent);
}

void CSynchronizerDlg::OnClose()
{
	if (IsMain) {
		SetEvent(ExitEvent);
	}

	EndDialog(IDOK);
}