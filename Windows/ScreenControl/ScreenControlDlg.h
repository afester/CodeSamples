
// ScreenControlDlg.h : header file
//

#pragma once

#include <vector>
#include "LoggingConsole.h"
#include "CCanvas.h"

template <class T>
class TypedComboBox : public CComboBox {
	std::vector<T> items;

public:
	TypedComboBox() {}

	void setItems(const std::vector<T>& data) {
		items = data;

		for (std::vector<T>::const_iterator entry = items.begin(); entry != items.end(); entry++) {
			CString str = formatItem(*entry);
			AddString(str);
		}
	}

	T getSelectedItem() const {
		int nIndex = GetCurSel();
		return items[nIndex];
	}

	virtual CString formatItem(const T& item) = 0;
};


class ScreenComboBox : public TypedComboBox<SCREEN_DATA> {
public:
	ScreenComboBox() : TypedComboBox() { }

	CString formatItem(const SCREEN_DATA& item);
};



class CUpDownEdit : public CEdit {
public:
	CUpDownEdit();

	void SetSpinControl(CSpinButtonCtrl& sc);

	void SetValue(int value);

private:
	CSpinButtonCtrl* m_spin_ctrl;

protected:
	// https://learn.microsoft.com/en-us/cpp/mfc/handlers-for-standard-windows-messages?view=msvc-170
	// To override a handler defined in a base class, simply define a function with the same prototype
	// in your derived class and to make a message - map entry for the handler. Your handler "overrides"
	// any handler of the same name in any of your class's base classes.
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	DECLARE_MESSAGE_MAP()
};


// CScreenControlDlg dialog
class CScreenControlDlg : public CDialogEx
{
// Construction
public:
	CScreenControlDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCREENCONTROL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonExit();
	afx_msg void OnBnClickedButtonDir0();
	afx_msg void OnBnClickedButtonDir90();
	afx_msg void OnBnClickedButtonDir180();
	afx_msg void OnBnClickedButtonDir270();
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnChangeLabelZoom();
	afx_msg void OnBnClickedZoomEnabled();

	CButton m_btn_dir0;
	CButton m_btn_dir90;
	CButton m_btn_dir180;
	CButton m_btn_dir270;

	CEdit m_idc_zoom_label;
	CButton m_idc_zoom;
	LoggingConsole m_console;
	ScreenComboBox m_screen_list;
	
	CSpinButtonCtrl m_idc_spin1;
	CUpDownEdit m_factor_label;

	CCanvas m_screenlayout;
};
