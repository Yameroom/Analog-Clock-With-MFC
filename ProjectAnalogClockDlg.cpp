#include "pch.h"
#include "framework.h"
#include "ProjectAnalogClock.h"
#include "ProjectAnalogClockDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAnalogClockDlg::CAnalogClockDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_ANALOGCLOCK_DIALOG, pParent), cx(0), cy(0)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    secHAND = 120;
    minHAND = 110;
    hrHAND = 80;
    hrOffset = -10;
}

void CAnalogClockDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAnalogClockDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CAnalogClockDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    SetIcon(m_hIcon, TRUE);  // atur besar icon
    SetIcon(m_hIcon, FALSE); // atur kecil icon

    SetTimer(1, 1000, NULL); // mengatur timer untuk update setiap detik

    return TRUE;
}

void CAnalogClockDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // mengatur icon untuk di tengah lingkaran
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // untuk menggambarkan iconnya
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CPaintDC dc(this);
        CRect rect;
        GetClientRect(&rect);

        cx = rect.Width() / 2;
        cy = rect.Height() / 2 + 10;

        // Mengatur warna latar belakang menjadi putih
        CBrush brushBackground(RGB(255, 255, 255));
        dc.FillRect(rect, &brushBackground);

        DrawClockFace(&dc);

        // untuk mengupdate waktu secara real time
        time_t now = time(0);
        struct tm localTime;
        localtime_s(&localTime, &now);
        int hh = localTime.tm_hour % 12;
        int mm = localTime.tm_min;
        int ss = localTime.tm_sec;

        DrawHand(&dc, ss * 6, secHAND, 1, RGB(255, 0, 0)); // jarum detik
        DrawHand(&dc, mm * 6, minHAND, 2, RGB(0, 0, 0)); // jarum menit
        DrawHand(&dc, hh * 30 + (mm / 2) + hrOffset, hrHAND, 3, RGB(0, 0, 0)); // jarum jam
    }
}

void CAnalogClockDlg::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == 1)
    {
        Invalidate(); // Memanggil ulang fungsi OnPaint untuk memperbarui gambar jam
    }
    CDialogEx::OnTimer(nIDEvent);
}

void CAnalogClockDlg::DrawHand(CDC* pDC, int pos, int length, int thickness, COLORREF color)
{
    CPen pen(PS_SOLID, thickness, color);
    CPen* pOldPen = pDC->SelectObject(&pen);

    // Menyesuaikan posisi awal jarum jam
    int startY = cy + hrOffset;
    int startX = cx;

    // Menyesuaikan panjang jarum jam
    pDC->MoveTo(startX, startY);
    pDC->LineTo(startX + static_cast<int>(length * sin(pos * 3.14159265 / 180)),
        startY - static_cast<int>(length * cos(pos * 3.14159265 / 180)));

    pDC->SelectObject(pOldPen);
}

void CAnalogClockDlg::DrawClockFace(CDC* pDC)
{
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(RGB(0, 0, 0));
    pDC->SetTextAlign(TA_CENTER | TA_BOTTOM);
    CFont font;
    font.CreatePointFont(120, _T("Arial"));
    CFont* pOldFont = pDC->SelectObject(&font);

    const double PI = 3.14159265358979323846;
    double angleStep = 2 * PI / 12;
    double startAngle = PI / 2;

    //mengatur ukaran lingkaran menyesuaikan dengan jarum detik

    int circleSize = secHAND + 20;

    CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
    CPen* pOldPen = pDC->SelectObject(&pen);
    pDC->SetBkMode(TRANSPARENT);
    pDC->SelectStockObject(NULL_BRUSH);
    pDC->Ellipse(cx - circleSize, cy - circleSize - 10, cx + circleSize, cy + circleSize - 10);
    pDC->SelectObject(pOldPen);

    for (int i = 1; i <= 12; ++i)
    {
        double angle = startAngle - i * angleStep;
        int x = cx + static_cast<int>((secHAND + 10) * cos(angle));
        int y = cy - static_cast<int>((secHAND + 10) * sin(angle));
        CString str;
        str.Format(_T("%d"), i);
        pDC->TextOut(x, y, str);
    }
    pDC->SelectObject(pOldFont);
}
