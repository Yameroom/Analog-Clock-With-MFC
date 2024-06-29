#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afx.h"
#include "resource.h"

// CAnalogClockDlg dialog
class CAnalogClockDlg : public CDialogEx
{
    // Construction
public:
    CAnalogClockDlg(CWnd* pParent = nullptr);   // standard constructor

    // Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ANALOGCLOCK_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    DECLARE_MESSAGE_MAP()

    // Custom functions
    void DrawHand(CDC* pDC, int pos, int length, int thickness, COLORREF color);
    void DrawClockFace(CDC* pDC);

private:
    int cx, cy;
    int secHAND, minHAND, hrHAND;
    int hrOffset;
};
