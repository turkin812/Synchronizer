
// SynchronizerDlg.h: файл заголовка
//

#pragma once


// Диалоговое окно CSynchronizerDlg
class CSynchronizerDlg : public CDialogEx
{
// Создание
public:
	CSynchronizerDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SYNCHRONIZER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditmain();
	CEdit EditMain;
	CString TextMain;
};
