
// ScreenControlDlg.cpp : implementation file
//


#include "pch.h"
#include "framework.h"
#include "ScreenControl.h"
#include "ScreenControlDlg.h"
#include "afxdialogex.h"

#include <vector>

#include "MagnifierWindow.h"
#include "SystemMetrics.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CString ScreenComboBox::formatItem(const SCREEN_DATA& sd) {
	CString str;
	str.Format(L"%s: %dx%d", sd.deviceName, sd.resolution.x, sd.resolution.y);
	return str;
}


// LoggingConsole* m_global_console = NULL;


// CUpDownEdit

BEGIN_MESSAGE_MAP(CUpDownEdit, CEdit)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


CUpDownEdit::CUpDownEdit() : CEdit(), m_spin_ctrl(0) {
}

void CUpDownEdit::SetSpinControl(CSpinButtonCtrl& sc) {
	m_spin_ctrl = &sc;
	SetValue(sc.GetPos());
}

void CUpDownEdit::SetValue(int value) {
	//float factor = (float) value / 10;
	CString str;
	//str.Format(L"%d%%", (int) (factor*100));
	str.Format(L"%d%%", value * 10);
	SetWindowText(str);
}

BOOL CUpDownEdit::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
	// nFlags: modifier keys which are currently pressed
	// zDelta: Turned distance
	// pt: current pointer position#

	if (m_spin_ctrl == 0) {
		return TRUE;
	}

	if (zDelta > 0) {
		Logger->log(L"UP!!");
		int newPos = m_spin_ctrl->GetPos() + 1;
		m_spin_ctrl->SetPos(newPos);
		SetValue(newPos);
	}
	else {
		Logger->log(L"DOWN!!");
		int newPos = m_spin_ctrl->GetPos() - 1;
		m_spin_ctrl->SetPos(newPos);
		SetValue(newPos);
	}

	return TRUE;
}


// CScreenControlDlg dialog


CScreenControlDlg::CScreenControlDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SCREENCONTROL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CScreenControlDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_SPIN1, m_idc_spin1);
	DDX_Control(pDX, IDC_LABEL_ZOOM, m_idc_zoom_label);
	DDX_Control(pDX, IDC_ZOOM_ENABLED, m_idc_zoom);
	DDX_Control(pDX, IDC_LOG, m_console);
	DDX_Control(pDX, IDC_SCREEN_LIST, m_screen_list);
	DDX_Control(pDX, IDC_LABEL_FACTOR, m_factor_label);
	DDX_Control(pDX, IDC_BUTTON_DIR0, m_btn_dir0);
	DDX_Control(pDX, IDC_BUTTON_DIR90, m_btn_dir90);
	DDX_Control(pDX, IDC_BUTTON_DIR180, m_btn_dir180);
	DDX_Control(pDX, IDC_BUTTON_DIR270, m_btn_dir270);
}

BEGIN_MESSAGE_MAP(CScreenControlDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CScreenControlDlg::OnBnClickedButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_DIR0, &CScreenControlDlg::OnBnClickedButtonDir0)
	ON_BN_CLICKED(IDC_BUTTON_DIR90, &CScreenControlDlg::OnBnClickedButtonDir90)
	ON_BN_CLICKED(IDC_BUTTON_DIR180, &CScreenControlDlg::OnBnClickedButtonDir180)
	ON_BN_CLICKED(IDC_BUTTON_DIR270, &CScreenControlDlg::OnBnClickedButtonDir270)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CScreenControlDlg::OnDeltaposSpin1)
	ON_EN_CHANGE(IDC_LABEL_FACTOR, &CScreenControlDlg::OnEnChangeLabelZoom)
	ON_BN_CLICKED(IDC_ZOOM_ENABLED, &CScreenControlDlg::OnBnClickedZoomEnabled)
 	// ON_WM_VSCROLL(IDC_SPIN1, &CScreenControlDlg::OnChangeZoom)
END_MESSAGE_MAP()


const wchar_t* getMessageName(UINT id) {
	switch (id) {
		case WM_PAINT: return _T("WM_PAINT");
		case WM_QUERYDRAGICON: return _T("WM_QUERYDRAGICON");
		case WM_MOUSEWHEEL: return _T("WM_MOUSEWHEEL");
		case WM_NOTIFY: return _T("WM_NOTIFY");

		case BN_CLICKED: return _T("BN_CLICKED");
	}

	// TODO: Not re-entrant!
	static wchar_t buffer[32];
	wsprintf(buffer, L"%04x", id);
	return buffer;
}

// CScreenControlDlg message handlers

BOOL CScreenControlDlg::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Initialize message console
	CFont m_Font;
	m_Font.CreateFont(
		14,		// Specifies the desired height (in logical units) of the font.
		0,		// Specifies the average width (in logical units) of characters in the font. If nWidth is 0, the aspect ratio of the device will be considered.
		0,		// Specifies the angle (in 0.1-degree units) between the escapement vector and the x-axis of the display surface.
		0,		// Specifies the angle (in 0.1-degree units) between the baseline of a character and the x-axis.
		FW_NORMAL, // Specifies the font weight (in inked pixels per 1000)
		FALSE,	// Specifies whether the font is italic.
		FALSE,	// Specifies whether the font is underlined.
		FALSE,	// Specifies whether characters in the font are struck out.
		DEFAULT_CHARSET, // Specifies the font's character set
		OUT_CHARACTER_PRECIS,	// Specifies the desired output precision
		CLIP_DEFAULT_PRECIS,	// Specifies the desired clipping precision. 
		PROOF_QUALITY,			// Specifies the font's output quality
		DEFAULT_PITCH | FF_SWISS,// Specifies the pitch and family of the font
		_T("Courier New"));		// Specifies the typeface name of the font
	m_console.SetFont(&m_Font, TRUE);
	m_console.setBgColor(RGB(51, 102, 0));
	m_console.setFgColor(RGB(0, 255, 255));

// 	m_global_console = &m_console;	// TODO!!!!

	// Initialize buttons
	CPngImage image0;
	image0.Load(IDB_ICON_DIR0, nullptr);
	m_btn_dir0.SetBitmap(image0);

	CPngImage image90;
	image90.Load(IDB_ICON_DIR90, nullptr);
	m_btn_dir90.SetBitmap(image90);

	CPngImage image180;
	image180.Load(IDB_ICON_DIR180, nullptr);
	m_btn_dir180.SetBitmap(image180);

	CPngImage image270;
	image270.Load(IDB_ICON_DIR270, nullptr);
	m_btn_dir270.SetBitmap(image270);

	// initialize Zoom spin control: 100-300%, 1ßß% is default
	m_idc_spin1.SetRange(10, 30);
	m_idc_spin1.SetPos(10);
	m_factor_label.SetSpinControl(m_idc_spin1);

	// Initialize screen selection dropdown
	auto screenData = getScreenData();
	m_screen_list.setItems(screenData);
	m_screen_list.SetCurSel(0);

	// TODO: What are these coordinates???
	CRect rect(190, 40, 450, 230);
	m_screenlayout.Create(0, L"", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, rect, this, 12345);
	m_screenlayout.setScreenData(screenData);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CScreenControlDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CScreenControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CScreenControlDlg::OnBnClickedButtonExit()
{
	EndDialog(IDCANCEL);

}

HHOOK hMouseHook = 0;

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	static LONG lx = 999999, ly;

	if (nCode == HC_ACTION) {
		MSLLHOOKSTRUCT* data = (MSLLHOOKSTRUCT*)lParam;

		LONG mx = data->pt.x;
		LONG my = data->pt.y;
		if (lx != 999999) {
			// normal movement
			//mx = lx + (mx - lx);
			//my = ly + (my - ly);

			// inverted movement
			mx = ly - (data->pt.x - ly);
			my = lx - (data->pt.y - lx);

			// exchange x/y axes
			//my = lx + (mx - lx);
			//mx = ly + (my - ly);
		}

		SetCursorPos(mx, my);

		// get and store new cursor position
		//POINT point;
		//GetCursorPos(&point);
		//lx = point.x;
		//ly = point.y;

		lx = my;
		ly = mx;

		data->pt.x = mx;
		data->pt.y = my;

		CallNextHookEx(0, nCode, wParam, lParam);
		return 1;
	}

	return CallNextHookEx(0, nCode, wParam, lParam);
}



CString geterror(LONG err) {
	switch (err) {
	case DISP_CHANGE_SUCCESSFUL: return CString("DISP_CHANGE_SUCCESSFUL");
	case DISP_CHANGE_BADDUALVIEW: return CString("DISP_CHANGE_BADDUALVIEW");
	case DISP_CHANGE_BADFLAGS: return CString("DISP_CHANGE_BADFLAGS");
	case DISP_CHANGE_BADMODE: return CString("DISP_CHANGE_BADMODE");
	case DISP_CHANGE_BADPARAM: return CString("DISP_CHANGE_BADPARAM");
	case DISP_CHANGE_FAILED: return CString("DISP_CHANGE_FAILED");
	case DISP_CHANGE_NOTUPDATED: return CString("DISP_CHANGE_NOTUPDATED ");
	case DISP_CHANGE_RESTART: return CString("DISP_CHANGE_RESTART");
	}
	return CString("UNKNOWN ERROR!");
}


CString getStateFlags(DWORD state) {
	CString result;

	result.Format(L"0x%08x", state);

	if (state & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP) {
		result = result + L"(DISPLAY_DEVICE_ATTACHED_TO_DESKTOP)";
	}
	return result;
}




void CScreenControlDlg::OnBnClickedButtonDir0()
{
	SCREEN_DATA sd = m_screen_list.getSelectedItem();
	CString str;
	str.Format(L"Using %s ...", sd.deviceName);
	m_console.log(str);

	DEVMODE myDevMode;
	memset(&myDevMode, 0, sizeof(DEVMODE));
	myDevMode.dmSize = sizeof(DEVMODE);

	EnumDisplaySettings(
		sd.deviceName,
		ENUM_CURRENT_SETTINGS,
		&myDevMode
	);

	m_console.log(L"Portrait ...");
	// if switching from vertical to horizontal then exchange x- and y-resolution
	DWORD curOrientation = myDevMode.dmDisplayOrientation;
	if (curOrientation == DMDO_90 || curOrientation == DMDO_270) {
		DWORD curWidth = myDevMode.dmPelsWidth;
		DWORD curHeight = myDevMode.dmPelsHeight;
		myDevMode.dmPelsWidth = curHeight;
		myDevMode.dmPelsHeight = curWidth;
	}

	myDevMode.dmFields |= DM_DISPLAYORIENTATION;
	myDevMode.dmDisplayOrientation = DMDO_DEFAULT;
	LONG res = ChangeDisplaySettingsEx(
		sd.deviceName,
		&myDevMode,
		NULL,
		0,
		NULL);

	CString err = geterror(res);

	UnhookWindowsHookEx(hMouseHook);
	hMouseHook = 0;
}

void CScreenControlDlg::OnBnClickedButtonDir90()
{
	SCREEN_DATA sd = m_screen_list.getSelectedItem();
	CString str;
	str.Format(L"Using %s ...", sd.deviceName);
	m_console.log(str);

	DEVMODE myDevMode;
	myDevMode.dmSize = sizeof(DEVMODE);
	memset(&myDevMode, 0, sizeof(DEVMODE));

	EnumDisplaySettings(
		sd.deviceName,
		ENUM_CURRENT_SETTINGS,
		&myDevMode
	);

	// if switching from vertical to horizontal then exchange x- and y-resolution
	DWORD curOrientation = myDevMode.dmDisplayOrientation;
	if (curOrientation == DMDO_DEFAULT || curOrientation == DMDO_180) {
		DWORD curWidth = myDevMode.dmPelsWidth;
		DWORD curHeight = myDevMode.dmPelsHeight;
		myDevMode.dmPelsWidth = curHeight;
		myDevMode.dmPelsHeight = curWidth;
	}

	myDevMode.dmFields |= DM_DISPLAYORIENTATION;
	myDevMode.dmDisplayOrientation = DMDO_90;
	LONG res = ChangeDisplaySettingsEx(
		sd.deviceName,
		&myDevMode,
		NULL,
		0,
		NULL);
	CString err = geterror(res);
}


void CScreenControlDlg::OnBnClickedButtonDir180()
{
	SCREEN_DATA sd = m_screen_list.getSelectedItem();
	CString str;
	str.Format(L"Using %s ...", sd.deviceName);
	m_console.log(str);

	DEVMODE myDevMode;
	myDevMode.dmSize = sizeof(DEVMODE);
	memset(&myDevMode, 0, sizeof(DEVMODE));

	EnumDisplaySettings(
		sd.deviceName,
		ENUM_CURRENT_SETTINGS,
		&myDevMode
	);

	// if switching from vertical to horizontal then exchange x- and y-resolution
	DWORD curOrientation = myDevMode.dmDisplayOrientation;
	if (curOrientation == DMDO_90|| curOrientation == DMDO_270) {
		DWORD curWidth = myDevMode.dmPelsWidth;
		DWORD curHeight = myDevMode.dmPelsHeight;
		myDevMode.dmPelsWidth = curHeight;
		myDevMode.dmPelsHeight = curWidth;
	}

	myDevMode.dmFields |= DM_DISPLAYORIENTATION;
	myDevMode.dmDisplayOrientation = DMDO_180;
	LONG res = ChangeDisplaySettingsEx(
		sd.deviceName,
		&myDevMode,
		NULL,
		0,
		NULL);

	CString err = geterror(res);
}


void CScreenControlDlg::OnBnClickedButtonDir270()
{
	SCREEN_DATA sd = m_screen_list.getSelectedItem();
	CString str;
	str.Format(L"Using %s ...", sd.deviceName);
	m_console.log(str);

	DEVMODE myDevMode;
	myDevMode.dmSize = sizeof(DEVMODE);
	memset(&myDevMode, 0, sizeof(DEVMODE));

	EnumDisplaySettings(
		sd.deviceName,
		ENUM_CURRENT_SETTINGS,
		&myDevMode
	);

	// if switching from vertical to horizontal then exchange x- and y-resolution
	DWORD curOrientation = myDevMode.dmDisplayOrientation;
	if (curOrientation == DMDO_DEFAULT || curOrientation == DMDO_180) {
		DWORD curWidth = myDevMode.dmPelsWidth;
		DWORD curHeight = myDevMode.dmPelsHeight;
		myDevMode.dmPelsWidth = curHeight;
		myDevMode.dmPelsHeight = curWidth;
	}

	myDevMode.dmFields |= DM_DISPLAYORIENTATION;
	myDevMode.dmDisplayOrientation = DMDO_270;
	LONG res = ChangeDisplaySettingsEx(
		sd.deviceName,
		&myDevMode, 
		NULL,
		0,
		NULL);

	CString err = geterror(res);
}

/**
 * 
 */
void CScreenControlDlg::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult){
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	// calculate the (new) current position
	const int nNewPos = pNMUpDown->iPos + pNMUpDown->iDelta;
	int nLower, nUpper;
	m_idc_spin1.GetRange(nLower, nUpper);
	if ((nNewPos < nLower) || (nNewPos > nUpper)) {
		*pResult = 1;
		return;
	}


	m_factor_label.SetValue(nNewPos);
	*pResult = 0;
}


// Handler on change value of spin buddy
void CScreenControlDlg::OnEnChangeLabelZoom() {
	if (m_idc_zoom.GetCheck()) {
		int pos = m_idc_spin1.GetPos();
		float factor = (float)pos / 10;

		CString str;
		str.Format(L"Mag factor: %f", factor);
		m_console.log(str);

		SetMagFactor(factor);
	}
}


void CScreenControlDlg::OnBnClickedZoomEnabled() {
	CString str;
	str.Format(L"Zoom enabled: %d", m_idc_zoom.GetCheck());
	m_console.log(str);
	if (m_idc_zoom.GetCheck()) {
		HWND hwnd = GetSafeHwnd();
		SetupMagnifier(GetModuleHandle(NULL), hwnd); //  , m_console);

		SetWindowPos(&wndTop, 0, 0, 0, 0,
			SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
	}
	else {
		StopMagnifier();
	}
}
