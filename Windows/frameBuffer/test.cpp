#include <ntstatus.h>
#define WIN32_NO_STATUS
#include <windows.h>
#include <windowsx.h>
#include <winternl.h>

#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <vector>
#include <string>

#include <stdio.h>
#include <io.h>
#include <fcntl.h>

#include <iostream>
#include <map>

#include <d3d11.h>
#include <d3dcompiler.h>

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "D3DCompiler.lib")

using namespace std;

static std::map<UINT, const char*> wmTranslation = {
{0, "WM_NULL" },
{1, "WM_CREATE" },
{2, "WM_DESTROY" },
{3, "WM_MOVE" },
{5, "WM_SIZE" },
{6, "WM_ACTIVATE" },
{7, "WM_SETFOCUS" },
{8, "WM_KILLFOCUS" },
{10, "WM_ENABLE" },
{11, "WM_SETREDRAW" },
{12, "WM_SETTEXT" },
{13, "WM_GETTEXT" },
{14, "WM_GETTEXTLENGTH" },
{15, "WM_PAINT" },
{16, "WM_CLOSE" },
{17, "WM_QUERYENDSESSION" },
{18, "WM_QUIT" },
{19, "WM_QUERYOPEN" },
{20, "WM_ERASEBKGND" },
{21, "WM_SYSCOLORCHANGE" },
{22, "WM_ENDSESSION" },
{24, "WM_SHOWWINDOW" },
{25, "WM_CTLCOLOR" },
{26, "WM_WININICHANGE" },
{27, "WM_DEVMODECHANGE" },
{28, "WM_ACTIVATEAPP" },
{29, "WM_FONTCHANGE" },
{30, "WM_TIMECHANGE" },
{31, "WM_CANCELMODE" },
{32, "WM_SETCURSOR" },
{33, "WM_MOUSEACTIVATE" },
{34, "WM_CHILDACTIVATE" },
{35, "WM_QUEUESYNC" },
{36, "WM_GETMINMAXINFO" },
{38, "WM_PAINTICON" },
{39, "WM_ICONERASEBKGND" },
{40, "WM_NEXTDLGCTL" },
{42, "WM_SPOOLERSTATUS" },
{43, "WM_DRAWITEM" },
{44, "WM_MEASUREITEM" },
{45, "WM_DELETEITEM" },
{46, "WM_VKEYTOITEM" },
{47, "WM_CHARTOITEM" },
{48, "WM_SETFONT" },
{49, "WM_GETFONT" },
{50, "WM_SETHOTKEY" },
{51, "WM_GETHOTKEY" },
{55, "WM_QUERYDRAGICON" },
{57, "WM_COMPAREITEM" },
{61, "WM_GETOBJECT" },
{65, "WM_COMPACTING" },
{68, "WM_COMMNOTIFY" },
{70, "WM_WINDOWPOSCHANGING" },
{71, "WM_WINDOWPOSCHANGED" },
{72, "WM_POWER" },
{73, "WM_COPYGLOBALDATA" },
{74, "WM_COPYDATA" },
{75, "WM_CANCELJOURNAL" },
{78, "WM_NOTIFY" },
{80, "WM_INPUTLANGCHANGEREQUEST" },
{81, "WM_INPUTLANGCHANGE" },
{82, "WM_TCARD" },
{83, "WM_HELP" },
{84, "WM_USERCHANGED" },
{85, "WM_NOTIFYFORMAT" },
{123, "WM_CONTEXTMENU" },
{124, "WM_STYLECHANGING" },
{125, "WM_STYLECHANGED" },
{126, "WM_DISPLAYCHANGE" },
{127, "WM_GETICON" },
{128, "WM_SETICON" },
{129, "WM_NCCREATE" },
{130, "WM_NCDESTROY" },
{131, "WM_NCCALCSIZE" },
{132, "WM_NCHITTEST" },
{133, "WM_NCPAINT" },
{134, "WM_NCACTIVATE" },
{135, "WM_GETDLGCODE" },
{136, "WM_SYNCPAINT" },
{160, "WM_NCMOUSEMOVE" },
{161, "WM_NCLBUTTONDOWN" },
{162, "WM_NCLBUTTONUP" },
{163, "WM_NCLBUTTONDBLCLK" },
{164, "WM_NCRBUTTONDOWN" },
{165, "WM_NCRBUTTONUP" },
{166, "WM_NCRBUTTONDBLCLK" },
{167, "WM_NCMBUTTONDOWN" },
{168, "WM_NCMBUTTONUP" },
{169, "WM_NCMBUTTONDBLCLK" },
{171, "WM_NCXBUTTONDOWN" },
{172, "WM_NCXBUTTONUP" },
{173, "WM_NCXBUTTONDBLCLK" },
{176, "EM_GETSEL" },
{177, "EM_SETSEL" },
{178, "EM_GETRECT" },
{179, "EM_SETRECT" },
{180, "EM_SETRECTNP" },
{181, "EM_SCROLL" },
{182, "EM_LINESCROLL" },
{183, "EM_SCROLLCARET" },
{185, "EM_GETMODIFY" },
{187, "EM_SETMODIFY" },
{188, "EM_GETLINECOUNT" },
{189, "EM_LINEINDEX" },
{190, "EM_SETHANDLE" },
{191, "EM_GETHANDLE" },
{192, "EM_GETTHUMB" },
{193, "EM_LINELENGTH" },
{194, "EM_REPLACESEL" },
{195, "EM_SETFONT" },
{196, "EM_GETLINE" },
{197, "EM_LIMITTEXT" },
{197, "EM_SETLIMITTEXT" },
{198, "EM_CANUNDO" },
{199, "EM_UNDO" },
{200, "EM_FMTLINES" },
{201, "EM_LINEFROMCHAR" },
{202, "EM_SETWORDBREAK" },
{203, "EM_SETTABSTOPS" },
{204, "EM_SETPASSWORDCHAR" },
{205, "EM_EMPTYUNDOBUFFER" },
{206, "EM_GETFIRSTVISIBLELINE" },
{207, "EM_SETREADONLY" },
{209, "EM_SETWORDBREAKPROC" },
{209, "EM_GETWORDBREAKPROC" },
{210, "EM_GETPASSWORDCHAR" },
{211, "EM_SETMARGINS" },
{212, "EM_GETMARGINS" },
{213, "EM_GETLIMITTEXT" },
{214, "EM_POSFROMCHAR" },
{215, "EM_CHARFROMPOS" },
{216, "EM_SETIMESTATUS" },
{217, "EM_GETIMESTATUS" },
{224, "SBM_SETPOS" },
{225, "SBM_GETPOS" },
{226, "SBM_SETRANGE" },
{227, "SBM_GETRANGE" },
{228, "SBM_ENABLE_ARROWS" },
{230, "SBM_SETRANGEREDRAW" },
{233, "SBM_SETSCROLLINFO" },
{234, "SBM_GETSCROLLINFO" },
{235, "SBM_GETSCROLLBARINFO" },
{240, "BM_GETCHECK" },
{241, "BM_SETCHECK" },
{242, "BM_GETSTATE" },
{243, "BM_SETSTATE" },
{244, "BM_SETSTYLE" },
{245, "BM_CLICK" },
{246, "BM_GETIMAGE" },
{247, "BM_SETIMAGE" },
{248, "BM_SETDONTCLICK" },
{255, "WM_INPUT" },
{256, "WM_KEYDOWN" },
{256, "WM_KEYFIRST" },
{257, "WM_KEYUP" },
{258, "WM_CHAR" },
{259, "WM_DEADCHAR" },
{260, "WM_SYSKEYDOWN" },
{261, "WM_SYSKEYUP" },
{262, "WM_SYSCHAR" },
{263, "WM_SYSDEADCHAR" },
{264, "WM_KEYLAST" },
{265, "WM_UNICHAR" },
{265, "WM_WNT_CONVERTREQUESTEX" },
{266, "WM_CONVERTREQUEST" },
{267, "WM_CONVERTRESULT" },
{268, "WM_INTERIM" },
{269, "WM_IME_STARTCOMPOSITION" },
{270, "WM_IME_ENDCOMPOSITION" },
{271, "WM_IME_COMPOSITION" },
{271, "WM_IME_KEYLAST" },
{272, "WM_INITDIALOG" },
{273, "WM_COMMAND" },
{274, "WM_SYSCOMMAND" },
{275, "WM_TIMER" },
{276, "WM_HSCROLL" },
{277, "WM_VSCROLL" },
{278, "WM_INITMENU" },
{279, "WM_INITMENUPOPUP" },
{280, "WM_SYSTIMER" },
{287, "WM_MENUSELECT" },
{288, "WM_MENUCHAR" },
{289, "WM_ENTERIDLE" },
{290, "WM_MENURBUTTONUP" },
{291, "WM_MENUDRAG" },
{292, "WM_MENUGETOBJECT" },
{293, "WM_UNINITMENUPOPUP" },
{294, "WM_MENUCOMMAND" },
{295, "WM_CHANGEUISTATE" },
{296, "WM_UPDATEUISTATE" },
{297, "WM_QUERYUISTATE" },
{306, "WM_CTLCOLORMSGBOX" },
{307, "WM_CTLCOLOREDIT" },
{308, "WM_CTLCOLORLISTBOX" },
{309, "WM_CTLCOLORBTN" },
{310, "WM_CTLCOLORDLG" },
{311, "WM_CTLCOLORSCROLLBAR" },
{312, "WM_CTLCOLORSTATIC" },
{512, "WM_MOUSEFIRST" },
{512, "WM_MOUSEMOVE" },
{513, "WM_LBUTTONDOWN" },
{514, "WM_LBUTTONUP" },
{515, "WM_LBUTTONDBLCLK" },
{516, "WM_RBUTTONDOWN" },
{517, "WM_RBUTTONUP" },
{518, "WM_RBUTTONDBLCLK" },
{519, "WM_MBUTTONDOWN" },
{520, "WM_MBUTTONUP" },
{521, "WM_MBUTTONDBLCLK" },
{521, "WM_MOUSELAST" },
{522, "WM_MOUSEWHEEL" },
{523, "WM_XBUTTONDOWN" },
{524, "WM_XBUTTONUP" },
{525, "WM_XBUTTONDBLCLK" },
{528, "WM_PARENTNOTIFY" },
{529, "WM_ENTERMENULOOP" },
{530, "WM_EXITMENULOOP" },
{531, "WM_NEXTMENU" },
{532, "WM_SIZING" },
{533, "WM_CAPTURECHANGED" },
{534, "WM_MOVING" },
{536, "WM_POWERBROADCAST" },
{537, "WM_DEVICECHANGE" },
{544, "WM_MDICREATE" },
{545, "WM_MDIDESTROY" },
{546, "WM_MDIACTIVATE" },
{547, "WM_MDIRESTORE" },
{548, "WM_MDINEXT" },
{549, "WM_MDIMAXIMIZE" },
{550, "WM_MDITILE" },
{551, "WM_MDICASCADE" },
{552, "WM_MDIICONARRANGE" },
{553, "WM_MDIGETACTIVE" },
{560, "WM_MDISETMENU" },
{561, "WM_ENTERSIZEMOVE" },
{562, "WM_EXITSIZEMOVE" },
{563, "WM_DROPFILES" },
{564, "WM_MDIREFRESHMENU" },
{640, "WM_IME_REPORT" },
{641, "WM_IME_SETCONTEXT" },
{642, "WM_IME_NOTIFY" },
{643, "WM_IME_CONTROL" },
{644, "WM_IME_COMPOSITIONFULL" },
{645, "WM_IME_SELECT" },
{646, "WM_IME_CHAR" },
{648, "WM_IME_REQUEST" },
{656, "WM_IMEKEYDOWN" },
{656, "WM_IME_KEYDOWN" },
{657, "WM_IMEKEYUP" },
{657, "WM_IME_KEYUP" },
{672, "WM_NCMOUSEHOVER" },
{673, "WM_MOUSEHOVER" },
{674, "WM_NCMOUSELEAVE" },
{675, "WM_MOUSELEAVE" },
{768, "WM_CUT" },
{769, "WM_COPY" },
{770, "WM_PASTE" },
{771, "WM_CLEAR" },
{772, "WM_UNDO" },
{773, "WM_RENDERFORMAT" },
{774, "WM_RENDERALLFORMATS" },
{775, "WM_DESTROYCLIPBOARD" },
{776, "WM_DRAWCLIPBOARD" },
{777, "WM_PAINTCLIPBOARD" },
{778, "WM_VSCROLLCLIPBOARD" },
{779, "WM_SIZECLIPBOARD" },
{780, "WM_ASKCBFORMATNAME" },
{781, "WM_CHANGECBCHAIN" },
{782, "WM_HSCROLLCLIPBOARD" },
{783, "WM_QUERYNEWPALETTE" },
{784, "WM_PALETTEISCHANGING" },
{785, "WM_PALETTECHANGED" },
{786, "WM_HOTKEY" },
{791, "WM_PRINT" },
{792, "WM_PRINTCLIENT" },
{793, "WM_APPCOMMAND" },
{856, "WM_HANDHELDFIRST" },
{863, "WM_HANDHELDLAST" },
{864, "WM_AFXFIRST" },
{895, "WM_AFXLAST" },
{896, "WM_PENWINFIRST" },
{897, "WM_RCRESULT" },
{898, "WM_HOOKRCRESULT" },
{899, "WM_GLOBALRCCHANGE" },
{899, "WM_PENMISCINFO" },
{900, "WM_SKB" },
{901, "WM_HEDITCTL" },
{901, "WM_PENCTL" },
{902, "WM_PENMISC" },
{903, "WM_CTLINIT" },
{904, "WM_PENEVENT" },
{911, "WM_PENWINLAST" },
{1024, "DDM_SETFMT" },
{1024, "DM_GETDEFID" },
{1024, "NIN_SELECT" },
{1024, "TBM_GETPOS" },
{1024, "WM_PSD_PAGESETUPDLG" },
{1024, "WM_USER" },
{1025, "CBEM_INSERTITEMA" },
{1025, "DDM_DRAW" },
{1025, "DM_SETDEFID" },
{1025, "HKM_SETHOTKEY" },
{1025, "PBM_SETRANGE" },
{1025, "RB_INSERTBANDA" },
{1025, "SB_SETTEXTA" },
{1025, "TB_ENABLEBUTTON" },
{1025, "TBM_GETRANGEMIN" },
{1025, "TTM_ACTIVATE" },
{1025, "WM_CHOOSEFONT_GETLOGFONT" },
{1025, "WM_PSD_FULLPAGERECT" },
{1026, "CBEM_SETIMAGELIST" },
{1026, "DDM_CLOSE" },
{1026, "DM_REPOSITION" },
{1026, "HKM_GETHOTKEY" },
{1026, "PBM_SETPOS" },
{1026, "RB_DELETEBAND" },
{1026, "SB_GETTEXTA" },
{1026, "TB_CHECKBUTTON" },
{1026, "TBM_GETRANGEMAX" },
{1026, "WM_PSD_MINMARGINRECT" },
{1027, "CBEM_GETIMAGELIST" },
{1027, "DDM_BEGIN" },
{1027, "HKM_SETRULES" },
{1027, "PBM_DELTAPOS" },
{1027, "RB_GETBARINFO" },
{1027, "SB_GETTEXTLENGTHA" },
{1027, "TBM_GETTIC" },
{1027, "TB_PRESSBUTTON" },
{1027, "TTM_SETDELAYTIME" },
{1027, "WM_PSD_MARGINRECT" },
{1028, "CBEM_GETITEMA" },
{1028, "DDM_END" },
{1028, "PBM_SETSTEP" },
{1028, "RB_SETBARINFO" },
{1028, "SB_SETPARTS" },
{1028, "TB_HIDEBUTTON" },
{1028, "TBM_SETTIC" },
{1028, "TTM_ADDTOOLA" },
{1028, "WM_PSD_GREEKTEXTRECT" },
{1029, "CBEM_SETITEMA" },
{1029, "PBM_STEPIT" },
{1029, "TB_INDETERMINATE" },
{1029, "TBM_SETPOS" },
{1029, "TTM_DELTOOLA" },
{1029, "WM_PSD_ENVSTAMPRECT" },
{1030, "CBEM_GETCOMBOCONTROL" },
{1030, "PBM_SETRANGE32" },
{1030, "RB_SETBANDINFOA" },
{1030, "SB_GETPARTS" },
{1030, "TB_MARKBUTTON" },
{1030, "TBM_SETRANGE" },
{1030, "TTM_NEWTOOLRECTA" },
{1030, "WM_PSD_YAFULLPAGERECT" },
{1031, "CBEM_GETEDITCONTROL" },
{1031, "PBM_GETRANGE" },
{1031, "RB_SETPARENT" },
{1031, "SB_GETBORDERS" },
{1031, "TBM_SETRANGEMIN" },
{1031, "TTM_RELAYEVENT" },
{1032, "CBEM_SETEXSTYLE" },
{1032, "PBM_GETPOS" },
{1032, "RB_HITTEST" },
{1032, "SB_SETMINHEIGHT" },
{1032, "TBM_SETRANGEMAX" },
{1032, "TTM_GETTOOLINFOA" },
{1033, "CBEM_GETEXSTYLE" },
{1033, "CBEM_GETEXTENDEDSTYLE" },
{1033, "PBM_SETBARCOLOR" },
{1033, "RB_GETRECT" },
{1033, "SB_SIMPLE" },
{1033, "TB_ISBUTTONENABLED" },
{1033, "TBM_CLEARTICS" },
{1033, "TTM_SETTOOLINFOA" },
{1034, "CBEM_HASEDITCHANGED" },
{1034, "RB_INSERTBANDW" },
{1034, "SB_GETRECT" },
{1034, "TB_ISBUTTONCHECKED" },
{1034, "TBM_SETSEL" },
{1034, "TTM_HITTESTA" },
{1034, "WIZ_QUERYNUMPAGES" },
{1035, "CBEM_INSERTITEMW" },
{1035, "RB_SETBANDINFOW" },
{1035, "SB_SETTEXTW" },
{1035, "TB_ISBUTTONPRESSED" },
{1035, "TBM_SETSELSTART" },
{1035, "TTM_GETTEXTA" },
{1035, "WIZ_NEXT" },
{1036, "CBEM_SETITEMW" },
{1036, "RB_GETBANDCOUNT" },
{1036, "SB_GETTEXTLENGTHW" },
{1036, "TB_ISBUTTONHIDDEN" },
{1036, "TBM_SETSELEND" },
{1036, "TTM_UPDATETIPTEXTA" },
{1036, "WIZ_PREV" },
{1037, "CBEM_GETITEMW" },
{1037, "RB_GETROWCOUNT" },
{1037, "SB_GETTEXTW" },
{1037, "TB_ISBUTTONINDETERMINATE" },
{1037, "TTM_GETTOOLCOUNT" },
{1038, "CBEM_SETEXTENDEDSTYLE" },
{1038, "RB_GETROWHEIGHT" },
{1038, "SB_ISSIMPLE" },
{1038, "TB_ISBUTTONHIGHLIGHTED" },
{1038, "TBM_GETPTICS" },
{1038, "TTM_ENUMTOOLSA" },
{1039, "SB_SETICON" },
{1039, "TBM_GETTICPOS" },
{1039, "TTM_GETCURRENTTOOLA" },
{1040, "RB_IDTOINDEX" },
{1040, "SB_SETTIPTEXTA" },
{1040, "TBM_GETNUMTICS" },
{1040, "TTM_WINDOWFROMPOINT" },
{1041, "RB_GETTOOLTIPS" },
{1041, "SB_SETTIPTEXTW" },
{1041, "TBM_GETSELSTART" },
{1041, "TB_SETSTATE" },
{1041, "TTM_TRACKACTIVATE" },
{1042, "RB_SETTOOLTIPS" },
{1042, "SB_GETTIPTEXTA" },
{1042, "TB_GETSTATE" },
{1042, "TBM_GETSELEND" },
{1042, "TTM_TRACKPOSITION" },
{1043, "RB_SETBKCOLOR" },
{1043, "SB_GETTIPTEXTW" },
{1043, "TB_ADDBITMAP" },
{1043, "TBM_CLEARSEL" },
{1043, "TTM_SETTIPBKCOLOR" },
{1044, "RB_GETBKCOLOR" },
{1044, "SB_GETICON" },
{1044, "TB_ADDBUTTONSA" },
{1044, "TBM_SETTICFREQ" },
{1044, "TTM_SETTIPTEXTCOLOR" },
{1045, "RB_SETTEXTCOLOR" },
{1045, "TB_INSERTBUTTONA" },
{1045, "TBM_SETPAGESIZE" },
{1045, "TTM_GETDELAYTIME" },
{1046, "RB_GETTEXTCOLOR" },
{1046, "TB_DELETEBUTTON" },
{1046, "TBM_GETPAGESIZE" },
{1046, "TTM_GETTIPBKCOLOR" },
{1047, "RB_SIZETORECT" },
{1047, "TB_GETBUTTON" },
{1047, "TBM_SETLINESIZE" },
{1047, "TTM_GETTIPTEXTCOLOR" },
{1048, "RB_BEGINDRAG" },
{1048, "TB_BUTTONCOUNT" },
{1048, "TBM_GETLINESIZE" },
{1048, "TTM_SETMAXTIPWIDTH" },
{1049, "RB_ENDDRAG" },
{1049, "TB_COMMANDTOINDEX" },
{1049, "TBM_GETTHUMBRECT" },
{1049, "TTM_GETMAXTIPWIDTH" },
{1050, "RB_DRAGMOVE" },
{1050, "TBM_GETCHANNELRECT" },
{1050, "TB_SAVERESTOREA" },
{1050, "TTM_SETMARGIN" },
{1051, "RB_GETBARHEIGHT" },
{1051, "TB_CUSTOMIZE" },
{1051, "TBM_SETTHUMBLENGTH" },
{1051, "TTM_GETMARGIN" },
{1052, "RB_GETBANDINFOW" },
{1052, "TB_ADDSTRINGA" },
{1052, "TBM_GETTHUMBLENGTH" },
{1052, "TTM_POP" },
{1053, "RB_GETBANDINFOA" },
{1053, "TB_GETITEMRECT" },
{1053, "TBM_SETTOOLTIPS" },
{1053, "TTM_UPDATE" },
{1054, "RB_MINIMIZEBAND" },
{1054, "TB_BUTTONSTRUCTSIZE" },
{1054, "TBM_GETTOOLTIPS" },
{1054, "TTM_GETBUBBLESIZE" },
{1055, "RB_MAXIMIZEBAND" },
{1055, "TBM_SETTIPSIDE" },
{1055, "TB_SETBUTTONSIZE" },
{1055, "TTM_ADJUSTRECT" },
{1056, "TBM_SETBUDDY" },
{1056, "TB_SETBITMAPSIZE" },
{1056, "TTM_SETTITLEA" },
{1057, "MSG_FTS_JUMP_VA" },
{1057, "TB_AUTOSIZE" },
{1057, "TBM_GETBUDDY" },
{1057, "TTM_SETTITLEW" },
{1058, "RB_GETBANDBORDERS" },
{1059, "MSG_FTS_JUMP_QWORD" },
{1059, "RB_SHOWBAND" },
{1059, "TB_GETTOOLTIPS" },
{1060, "MSG_REINDEX_REQUEST" },
{1060, "TB_SETTOOLTIPS" },
{1061, "MSG_FTS_WHERE_IS_IT" },
{1061, "RB_SETPALETTE" },
{1061, "TB_SETPARENT" },
{1062, "RB_GETPALETTE" },
{1063, "RB_MOVEBAND" },
{1063, "TB_SETROWS" },
{1064, "TB_GETROWS" },
{1065, "TB_GETBITMAPFLAGS" },
{1066, "TB_SETCMDID" },
{1067, "RB_PUSHCHEVRON" },
{1067, "TB_CHANGEBITMAP" },
{1068, "TB_GETBITMAP" },
{1069, "MSG_GET_DEFFONT" },
{1069, "TB_GETBUTTONTEXTA" },
{1070, "TB_REPLACEBITMAP" },
{1071, "TB_SETINDENT" },
{1072, "TB_SETIMAGELIST" },
{1073, "TB_GETIMAGELIST" },
{1074, "TB_LOADIMAGES" },
{1074, "EM_CANPASTE" },
{1074, "TTM_ADDTOOLW" },
{1075, "EM_DISPLAYBAND" },
{1075, "TB_GETRECT" },
{1075, "TTM_DELTOOLW" },
{1076, "EM_EXGETSEL" },
{1076, "TB_SETHOTIMAGELIST" },
{1076, "TTM_NEWTOOLRECTW" },
{1077, "EM_EXLIMITTEXT" },
{1077, "TB_GETHOTIMAGELIST" },
{1077, "TTM_GETTOOLINFOW" },
{1078, "EM_EXLINEFROMCHAR" },
{1078, "TB_SETDISABLEDIMAGELIST" },
{1078, "TTM_SETTOOLINFOW" },
{1079, "EM_EXSETSEL" },
{1079, "TB_GETDISABLEDIMAGELIST" },
{1079, "TTM_HITTESTW" },
{1080, "EM_FINDTEXT" },
{1080, "TB_SETSTYLE" },
{1080, "TTM_GETTEXTW" },
{1081, "EM_FORMATRANGE" },
{1081, "TB_GETSTYLE" },
{1081, "TTM_UPDATETIPTEXTW" },
{1082, "EM_GETCHARFORMAT" },
{1082, "TB_GETBUTTONSIZE" },
{1082, "TTM_ENUMTOOLSW" },
{1083, "EM_GETEVENTMASK" },
{1083, "TB_SETBUTTONWIDTH" },
{1083, "TTM_GETCURRENTTOOLW" },
{1084, "EM_GETOLEINTERFACE" },
{1084, "TB_SETMAXTEXTROWS" },
{1085, "EM_GETPARAFORMAT" },
{1085, "TB_GETTEXTROWS" },
{1086, "EM_GETSELTEXT" },
{1086, "TB_GETOBJECT" },
{1087, "EM_HIDESELECTION" },
{1087, "TB_GETBUTTONINFOW" },
{1088, "EM_PASTESPECIAL" },
{1088, "TB_SETBUTTONINFOW" },
{1089, "EM_REQUESTRESIZE" },
{1089, "TB_GETBUTTONINFOA" },
{1090, "EM_SELECTIONTYPE" },
{1090, "TB_SETBUTTONINFOA" },
{1091, "EM_SETBKGNDCOLOR" },
{1091, "TB_INSERTBUTTONW" },
{1092, "EM_SETCHARFORMAT" },
{1092, "TB_ADDBUTTONSW" },
{1093, "EM_SETEVENTMASK" },
{1093, "TB_HITTEST" },
{1094, "EM_SETOLECALLBACK" },
{1094, "TB_SETDRAWTEXTFLAGS" },
{1095, "EM_SETPARAFORMAT" },
{1095, "TB_GETHOTITEM" },
{1096, "EM_SETTARGETDEVICE" },
{1096, "TB_SETHOTITEM" },
{1097, "EM_STREAMIN" },
{1097, "TB_SETANCHORHIGHLIGHT" },
{1098, "EM_STREAMOUT" },
{1098, "TB_GETANCHORHIGHLIGHT" },
{1099, "EM_GETTEXTRANGE" },
{1099, "TB_GETBUTTONTEXTW" },
{1100, "EM_FINDWORDBREAK" },
{1100, "TB_SAVERESTOREW" },
{1101, "EM_SETOPTIONS" },
{1101, "TB_ADDSTRINGW" },
{1102, "EM_GETOPTIONS" },
{1102, "TB_MAPACCELERATORA" },
{1103, "EM_FINDTEXTEX" },
{1103, "TB_GETINSERTMARK" },
{1104, "EM_GETWORDBREAKPROCEX" },
{1104, "TB_SETINSERTMARK" },
{1105, "EM_SETWORDBREAKPROCEX" },
{1105, "TB_INSERTMARKHITTEST" },
{1106, "EM_SETUNDOLIMIT" },
{1106, "TB_MOVEBUTTON" },
{1107, "TB_GETMAXSIZE" },
{1108, "EM_REDO" },
{1108, "TB_SETEXTENDEDSTYLE" },
{1109, "EM_CANREDO" },
{1109, "TB_GETEXTENDEDSTYLE" },
{1110, "EM_GETUNDONAME" },
{1110, "TB_GETPADDING" },
{1111, "EM_GETREDONAME" },
{1111, "TB_SETPADDING" },
{1112, "EM_STOPGROUPTYPING" },
{1112, "TB_SETINSERTMARKCOLOR" },
{1113, "EM_SETTEXTMODE" },
{1113, "TB_GETINSERTMARKCOLOR" },
{1114, "EM_GETTEXTMODE" },
{1114, "TB_MAPACCELERATORW" },
{1115, "EM_AUTOURLDETECT" },
{1115, "TB_GETSTRINGW" },
{1116, "EM_GETAUTOURLDETECT" },
{1116, "TB_GETSTRINGA" },
{1117, "EM_SETPALETTE" },
{1118, "EM_GETTEXTEX" },
{1119, "EM_GETTEXTLENGTHEX" },
{1120, "EM_SHOWSCROLLBAR" },
{1121, "EM_SETTEXTEX" },
{1123, "TAPI_REPLY" },
{1124, "ACM_OPENA" },
{1124, "BFFM_SETSTATUSTEXTA" },
{1124, "CDM_FIRST" },
{1124, "CDM_GETSPEC" },
{1124, "EM_SETPUNCTUATION" },
{1124, "IPM_CLEARADDRESS" },
{1124, "WM_CAP_UNICODE_START" },
{1125, "ACM_PLAY" },
{1125, "BFFM_ENABLEOK" },
{1125, "CDM_GETFILEPATH" },
{1125, "EM_GETPUNCTUATION" },
{1125, "IPM_SETADDRESS" },
{1125, "PSM_SETCURSEL" },
{1125, "UDM_SETRANGE" },
{1125, "WM_CHOOSEFONT_SETLOGFONT" },
{1126, "ACM_STOP" },
{1126, "BFFM_SETSELECTIONA" },
{1126, "CDM_GETFOLDERPATH" },
{1126, "EM_SETWORDWRAPMODE" },
{1126, "IPM_GETADDRESS" },
{1126, "PSM_REMOVEPAGE" },
{1126, "UDM_GETRANGE" },
{1126, "WM_CAP_SET_CALLBACK_ERRORW" },
{1126, "WM_CHOOSEFONT_SETFLAGS" },
{1127, "ACM_OPENW" },
{1127, "BFFM_SETSELECTIONW" },
{1127, "CDM_GETFOLDERIDLIST" },
{1127, "EM_GETWORDWRAPMODE" },
{1127, "IPM_SETRANGE" },
{1127, "PSM_ADDPAGE" },
{1127, "UDM_SETPOS" },
{1127, "WM_CAP_SET_CALLBACK_STATUSW" },
{1128, "BFFM_SETSTATUSTEXTW" },
{1128, "CDM_SETCONTROLTEXT" },
{1128, "EM_SETIMECOLOR" },
{1128, "IPM_SETFOCUS" },
{1128, "PSM_CHANGED" },
{1128, "UDM_GETPOS" },
{1129, "CDM_HIDECONTROL" },
{1129, "EM_GETIMECOLOR" },
{1129, "IPM_ISBLANK" },
{1129, "PSM_RESTARTWINDOWS" },
{1129, "UDM_SETBUDDY" },
{1130, "CDM_SETDEFEXT" },
{1130, "EM_SETIMEOPTIONS" },
{1130, "PSM_REBOOTSYSTEM" },
{1130, "UDM_GETBUDDY" },
{1131, "EM_GETIMEOPTIONS" },
{1131, "PSM_CANCELTOCLOSE" },
{1131, "UDM_SETACCEL" },
{1132, "EM_CONVPOSITION" },
{1132, "EM_CONVPOSITION" },
{1132, "PSM_QUERYSIBLINGS" },
{1132, "UDM_GETACCEL" },
{1133, "MCIWNDM_GETZOOM" },
{1133, "PSM_UNCHANGED" },
{1133, "UDM_SETBASE" },
{1134, "PSM_APPLY" },
{1134, "UDM_GETBASE" },
{1135, "PSM_SETTITLEA" },
{1135, "UDM_SETRANGE32" },
{1136, "PSM_SETWIZBUTTONS" },
{1136, "UDM_GETRANGE32" },
{1136, "WM_CAP_DRIVER_GET_NAMEW" },
{1137, "PSM_PRESSBUTTON" },
{1137, "UDM_SETPOS32" },
{1137, "WM_CAP_DRIVER_GET_VERSIONW" },
{1138, "PSM_SETCURSELID" },
{1138, "UDM_GETPOS32" },
{1139, "PSM_SETFINISHTEXTA" },
{1140, "PSM_GETTABCONTROL" },
{1141, "PSM_ISDIALOGMESSAGE" },
{1142, "MCIWNDM_REALIZE" },
{1142, "PSM_GETCURRENTPAGEHWND" },
{1143, "MCIWNDM_SETTIMEFORMATA" },
{1143, "PSM_INSERTPAGE" },
{1144, "EM_SETLANGOPTIONS" },
{1144, "MCIWNDM_GETTIMEFORMATA" },
{1144, "PSM_SETTITLEW" },
{1144, "WM_CAP_FILE_SET_CAPTURE_FILEW" },
{1145, "EM_GETLANGOPTIONS" },
{1145, "MCIWNDM_VALIDATEMEDIA" },
{1145, "PSM_SETFINISHTEXTW" },
{1145, "WM_CAP_FILE_GET_CAPTURE_FILEW" },
{1146, "EM_GETIMECOMPMODE" },
{1147, "EM_FINDTEXTW" },
{1147, "MCIWNDM_PLAYTO" },
{1147, "WM_CAP_FILE_SAVEASW" },
{1148, "EM_FINDTEXTEXW" },
{1148, "MCIWNDM_GETFILENAMEA" },
{1149, "EM_RECONVERSION" },
{1149, "MCIWNDM_GETDEVICEA" },
{1149, "PSM_SETHEADERTITLEA" },
{1149, "WM_CAP_FILE_SAVEDIBW" },
{1150, "EM_SETIMEMODEBIAS" },
{1150, "MCIWNDM_GETPALETTE" },
{1150, "PSM_SETHEADERTITLEW" },
{1151, "EM_GETIMEMODEBIAS" },
{1151, "MCIWNDM_SETPALETTE" },
{1151, "PSM_SETHEADERSUBTITLEA" },
{1152, "MCIWNDM_GETERRORA" },
{1152, "PSM_SETHEADERSUBTITLEW" },
{1153, "PSM_HWNDTOINDEX" },
{1154, "PSM_INDEXTOHWND" },
{1155, "MCIWNDM_SETINACTIVETIMER" },
{1155, "PSM_PAGETOINDEX" },
{1156, "PSM_INDEXTOPAGE" },
{1157, "DL_BEGINDRAG" },
{1157, "MCIWNDM_GETINACTIVETIMER" },
{1157, "PSM_IDTOINDEX" },
{1158, "DL_DRAGGING" },
{1158, "PSM_INDEXTOID" },
{1159, "DL_DROPPED" },
{1159, "PSM_GETRESULT" },
{1160, "DL_CANCELDRAG" },
{1160, "PSM_RECALCPAGESIZES" },
{1164, "MCIWNDM_GET_SOURCE" },
{1165, "MCIWNDM_PUT_SOURCE" },
{1166, "MCIWNDM_GET_DEST" },
{1167, "MCIWNDM_PUT_DEST" },
{1168, "MCIWNDM_CAN_PLAY" },
{1169, "MCIWNDM_CAN_WINDOW" },
{1170, "MCIWNDM_CAN_RECORD" },
{1171, "MCIWNDM_CAN_SAVE" },
{1172, "MCIWNDM_CAN_EJECT" },
{1173, "MCIWNDM_CAN_CONFIG" },
{1174, "IE_GETINK" },
{1174, "IE_MSGFIRST" },
{1174, "MCIWNDM_PALETTEKICK" },
{1175, "IE_SETINK" },
{1176, "IE_GETPENTIP" },
{1177, "IE_SETPENTIP" },
{1178, "IE_GETERASERTIP" },
{1179, "IE_SETERASERTIP" },
{1180, "IE_GETBKGND" },
{1181, "IE_SETBKGND" },
{1182, "IE_GETGRIDORIGIN" },
{1183, "IE_SETGRIDORIGIN" },
{1184, "IE_GETGRIDPEN" },
{1185, "IE_SETGRIDPEN" },
{1186, "IE_GETGRIDSIZE" },
{1187, "IE_SETGRIDSIZE" },
{1188, "IE_GETMODE" },
{1189, "IE_SETMODE" },
{1190, "IE_GETINKRECT" },
{1190, "WM_CAP_SET_MCI_DEVICEW" },
{1191, "WM_CAP_GET_MCI_DEVICEW" },
{1204, "WM_CAP_PAL_OPENW" },
{1205, "WM_CAP_PAL_SAVEW" },
{1208, "IE_GETAPPDATA" },
{1209, "IE_SETAPPDATA" },
{1210, "IE_GETDRAWOPTS" },
{1211, "IE_SETDRAWOPTS" },
{1212, "IE_GETFORMAT" },
{1213, "IE_SETFORMAT" },
{1214, "IE_GETINKINPUT" },
{1215, "IE_SETINKINPUT" },
{1216, "IE_GETNOTIFY" },
{1217, "IE_SETNOTIFY" },
{1218, "IE_GETRECOG" },
{1219, "IE_SETRECOG" },
{1220, "IE_GETSECURITY" },
{1221, "IE_SETSECURITY" },
{1222, "IE_GETSEL" },
{1223, "IE_SETSEL" },
{1224, "CDM_LAST" },
{1224, "EM_SETBIDIOPTIONS" },
{1224, "IE_DOCOMMAND" },
{1224, "MCIWNDM_NOTIFYMODE" },
{1225, "EM_GETBIDIOPTIONS" },
{1225, "IE_GETCOMMAND" },
{1226, "EM_SETTYPOGRAPHYOPTIONS" },
{1226, "IE_GETCOUNT" },
{1227, "EM_GETTYPOGRAPHYOPTIONS" },
{1227, "IE_GETGESTURE" },
{1227, "MCIWNDM_NOTIFYMEDIA" },
{1228, "EM_SETEDITSTYLE" },
{1228, "IE_GETMENU" },
{1229, "EM_GETEDITSTYLE" },
{1229, "IE_GETPAINTDC" },
{1229, "MCIWNDM_NOTIFYERROR" },
{1230, "IE_GETPDEVENT" },
{1231, "IE_GETSELCOUNT" },
{1232, "IE_GETSELITEMS" },
{1233, "IE_GETSTYLE" },
{1243, "MCIWNDM_SETTIMEFORMATW" },
{1244, "EM_OUTLINE" },
{1244, "EM_OUTLINE" },
{1244, "MCIWNDM_GETTIMEFORMATW" },
{1245, "EM_GETSCROLLPOS" },
{1245, "EM_GETSCROLLPOS" },
{1246, "EM_SETSCROLLPOS" },
{1246, "EM_SETSCROLLPOS" },
{1247, "EM_SETFONTSIZE" },
{1247, "EM_SETFONTSIZE" },
{1248, "EM_GETZOOM" },
{1248, "MCIWNDM_GETFILENAMEW" },
{1249, "EM_SETZOOM" },
{1249, "MCIWNDM_GETDEVICEW" },
{1250, "EM_GETVIEWKIND" },
{1251, "EM_SETVIEWKIND" },
{1252, "EM_GETPAGE" },
{1252, "MCIWNDM_GETERRORW" },
{1253, "EM_SETPAGE" },
{1254, "EM_GETHYPHENATEINFO" },
{1255, "EM_SETHYPHENATEINFO" },
{1259, "EM_GETPAGEROTATE" },
{1260, "EM_SETPAGEROTATE" },
{1261, "EM_GETCTFMODEBIAS" },
{1262, "EM_SETCTFMODEBIAS" },
{1264, "EM_GETCTFOPENSTATUS" },
{1265, "EM_SETCTFOPENSTATUS" },
{1266, "EM_GETIMECOMPTEXT" },
{1267, "EM_ISIME" },
{1268, "EM_GETIMEPROPERTY" },
{1293, "EM_GETQUERYRTFOBJ" },
{1294, "EM_SETQUERYRTFOBJ" },
{1536, "FM_GETFOCUS" },
{1537, "FM_GETDRIVEINFOA" },
{1538, "FM_GETSELCOUNT" },
{1539, "FM_GETSELCOUNTLFN" },
{1540, "FM_GETFILESELA" },
{1541, "FM_GETFILESELLFNA" },
{1542, "FM_REFRESH_WINDOWS" },
{1543, "FM_RELOAD_EXTENSIONS" },
{1553, "FM_GETDRIVEINFOW" },
{1556, "FM_GETFILESELW" },
{1557, "FM_GETFILESELLFNW" },
{1625, "WLX_WM_SAS" },
{2024, "SM_GETSELCOUNT" },
{2024, "UM_GETSELCOUNT" },
{2024, "WM_CPL_LAUNCH" },
{2025, "SM_GETSERVERSELA" },
{2025, "UM_GETUSERSELA" },
{2025, "WM_CPL_LAUNCHED" },
{2026, "SM_GETSERVERSELW" },
{2026, "UM_GETUSERSELW" },
{2027, "SM_GETCURFOCUSA" },
{2027, "UM_GETGROUPSELA" },
{2028, "SM_GETCURFOCUSW" },
{2028, "UM_GETGROUPSELW" },
{2029, "SM_GETOPTIONS" },
{2029, "UM_GETCURFOCUSA" },
{2030, "UM_GETCURFOCUSW" },
{2031, "UM_GETOPTIONS" },
{2032, "UM_GETOPTIONS2" },
{4096, "LVM_FIRST" },
{4096, "LVM_GETBKCOLOR" },
{4097, "LVM_SETBKCOLOR" },
{4098, "LVM_GETIMAGELIST" },
{4099, "LVM_SETIMAGELIST" },
{4100, "LVM_GETITEMCOUNT" },
{4101, "LVM_GETITEMA" },
{4102, "LVM_SETITEMA" },
{4103, "LVM_INSERTITEMA" },
{4104, "LVM_DELETEITEM" },
{4105, "LVM_DELETEALLITEMS" },
{4106, "LVM_GETCALLBACKMASK" },
{4107, "LVM_SETCALLBACKMASK" },
{4108, "LVM_GETNEXTITEM" },
{4109, "LVM_FINDITEMA" },
{4110, "LVM_GETITEMRECT" },
{4111, "LVM_SETITEMPOSITION" },
{4112, "LVM_GETITEMPOSITION" },
{4113, "LVM_GETSTRINGWIDTHA" },
{4114, "LVM_HITTEST" },
{4115, "LVM_ENSUREVISIBLE" },
{4116, "LVM_SCROLL" },
{4117, "LVM_REDRAWITEMS" },
{4118, "LVM_ARRANGE" },
{4119, "LVM_EDITLABELA" },
{4120, "LVM_GETEDITCONTROL" },
{4121, "LVM_GETCOLUMNA" },
{4122, "LVM_SETCOLUMNA" },
{4123, "LVM_INSERTCOLUMNA" },
{4124, "LVM_DELETECOLUMN" },
{4125, "LVM_GETCOLUMNWIDTH" },
{4126, "LVM_SETCOLUMNWIDTH" },
{4127, "LVM_GETHEADER" },
{4129, "LVM_CREATEDRAGIMAGE" },
{4130, "LVM_GETVIEWRECT" },
{4131, "LVM_GETTEXTCOLOR" },
{4132, "LVM_SETTEXTCOLOR" },
{4133, "LVM_GETTEXTBKCOLOR" },
{4134, "LVM_SETTEXTBKCOLOR" },
{4135, "LVM_GETTOPINDEX" },
{4136, "LVM_GETCOUNTPERPAGE" },
{4137, "LVM_GETORIGIN" },
{4138, "LVM_UPDATE" },
{4139, "LVM_SETITEMSTATE" },
{4140, "LVM_GETITEMSTATE" },
{4141, "LVM_GETITEMTEXTA" },
{4142, "LVM_SETITEMTEXTA" },
{4143, "LVM_SETITEMCOUNT" },
{4144, "LVM_SORTITEMS" },
{4145, "LVM_SETITEMPOSITION32" },
{4146, "LVM_GETSELECTEDCOUNT" },
{4147, "LVM_GETITEMSPACING" },
{4148, "LVM_GETISEARCHSTRINGA" },
{4149, "LVM_SETICONSPACING" },
{4150, "LVM_SETEXTENDEDLISTVIEWSTYLE" },
{4151, "LVM_GETEXTENDEDLISTVIEWSTYLE" },
{4152, "LVM_GETSUBITEMRECT" },
{4153, "LVM_SUBITEMHITTEST" },
{4154, "LVM_SETCOLUMNORDERARRAY" },
{4155, "LVM_GETCOLUMNORDERARRAY" },
{4156, "LVM_SETHOTITEM" },
{4157, "LVM_GETHOTITEM" },
{4158, "LVM_SETHOTCURSOR" },
{4159, "LVM_GETHOTCURSOR" },
{4160, "LVM_APPROXIMATEVIEWRECT" },
{4161, "LVM_SETWORKAREAS" },
{4162, "LVM_GETSELECTIONMARK" },
{4163, "LVM_SETSELECTIONMARK" },
{4164, "LVM_SETBKIMAGEA" },
{4165, "LVM_GETBKIMAGEA" },
{4166, "LVM_GETWORKAREAS" },
{4167, "LVM_SETHOVERTIME" },
{4168, "LVM_GETHOVERTIME" },
{4169, "LVM_GETNUMBEROFWORKAREAS" },
{4170, "LVM_SETTOOLTIPS" },
{4171, "LVM_GETITEMW" },
{4172, "LVM_SETITEMW" },
{4173, "LVM_INSERTITEMW" },
{4174, "LVM_GETTOOLTIPS" },
{4179, "LVM_FINDITEMW" },
{4183, "LVM_GETSTRINGWIDTHW" },
{4191, "LVM_GETCOLUMNW" },
{4192, "LVM_SETCOLUMNW" },
{4193, "LVM_INSERTCOLUMNW" },
{4211, "LVM_GETITEMTEXTW" },
{4212, "LVM_SETITEMTEXTW" },
{4213, "LVM_GETISEARCHSTRINGW" },
{4214, "LVM_EDITLABELW" },
{4235, "LVM_GETBKIMAGEW" },
{4236, "LVM_SETSELECTEDCOLUMN" },
{4237, "LVM_SETTILEWIDTH" },
{4238, "LVM_SETVIEW" },
{4239, "LVM_GETVIEW" },
{4241, "LVM_INSERTGROUP" },
{4243, "LVM_SETGROUPINFO" },
{4245, "LVM_GETGROUPINFO" },
{4246, "LVM_REMOVEGROUP" },
{4247, "LVM_MOVEGROUP" },
{4250, "LVM_MOVEITEMTOGROUP" },
{4251, "LVM_SETGROUPMETRICS" },
{4252, "LVM_GETGROUPMETRICS" },
{4253, "LVM_ENABLEGROUPVIEW" },
{4254, "LVM_SORTGROUPS" },
{4255, "LVM_INSERTGROUPSORTED" },
{4256, "LVM_REMOVEALLGROUPS" },
{4257, "LVM_HASGROUP" },
{4258, "LVM_SETTILEVIEWINFO" },
{4259, "LVM_GETTILEVIEWINFO" },
{4260, "LVM_SETTILEINFO" },
{4261, "LVM_GETTILEINFO" },
{4262, "LVM_SETINSERTMARK" },
{4263, "LVM_GETINSERTMARK" },
{4264, "LVM_INSERTMARKHITTEST" },
{4265, "LVM_GETINSERTMARKRECT" },
{4266, "LVM_SETINSERTMARKCOLOR" },
{4267, "LVM_GETINSERTMARKCOLOR" },
{4269, "LVM_SETINFOTIP" },
{4270, "LVM_GETSELECTEDCOLUMN" },
{4271, "LVM_ISGROUPVIEWENABLED" },
{4272, "LVM_GETOUTLINECOLOR" },
{4273, "LVM_SETOUTLINECOLOR" },
{4275, "LVM_CANCELEDITLABEL" },
{4276, "LVM_MAPINDEXTOID" },
{4277, "LVM_MAPIDTOINDEX" },
{4278, "LVM_ISITEMVISIBLE" },
{8192, "OCM__BASE" },
{8197, "LVM_SETUNICODEFORMAT" },
{8198, "LVM_GETUNICODEFORMAT" },
{8217, "OCM_CTLCOLOR" },
{8235, "OCM_DRAWITEM" },
{8236, "OCM_MEASUREITEM" },
{8237, "OCM_DELETEITEM" },
{8238, "OCM_VKEYTOITEM" },
{8239, "OCM_CHARTOITEM" },
{8249, "OCM_COMPAREITEM" },
{8270, "OCM_NOTIFY" },
{8465, "OCM_COMMAND" },
{8468, "OCM_HSCROLL" },
{8469, "OCM_VSCROLL" },
{8498, "OCM_CTLCOLORMSGBOX" },
{8499, "OCM_CTLCOLOREDIT" },
{8500, "OCM_CTLCOLORLISTBOX" },
{8501, "OCM_CTLCOLORBTN" },
{8502, "OCM_CTLCOLORDLG" },
{8503, "OCM_CTLCOLORSCROLLBAR" },
{8504, "OCM_CTLCOLORSTATIC" },
{8720, "OCM_PARENTNOTIFY" },
{32768, "WM_APP" },
{52429, "WM_RASDIALEVENT" },
};

const char* getMessageNameFromId(UINT id);
string GetErrorMessage(DWORD dwErrorCode);

// global declarations
IDXGISwapChain *swapchain;             // the pointer to the swap chain interface
ID3D11Device *dev;                     // the pointer to our Direct3D device interface
ID3D11DeviceContext *devcon;           // the pointer to our Direct3D device context
ID3D11InputLayout *pLayout;            // the pointer to the input layout
ID3D11VertexShader *pVS;               // the pointer to the vertex shader
ID3D11PixelShader *pPS;                // the pointer to the pixel shader
ID3D11Buffer *pVBuffer;                // the pointer to the vertex buffer

struct VERTEX{FLOAT X, Y, Z; float Color[4];};

// function prototypes
void InitD3D(HWND hWnd);     // sets up and initializes Direct3D
void CleanD3D(void);         // closes Direct3D and releases memory
void InitGraphics(void);    // creates the shape to render
void InitPipeline(void);    // loads and prepares the shaders


void createConsole();

// Global variables
static const int SCREEN_WIDTH=800;
static const int SCREEN_HEIGHT=600;

// The main window class name.
static TCHAR szWindowClass[] = _T("win32app");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Win32 Guided Tour Application");

HINSTANCE hInst;

std::vector<std::string> lines;

// NOTE: These structures/classes are not public yet and might or might not be available on a particular system
#define SystemKernelVaShadowInformation     (SYSTEM_INFORMATION_CLASS)196
typedef struct _SYSTEM_KERNEL_VA_SHADOW_INFORMATION
{
    struct
    {
        ULONG KvaShadowEnabled : 1;
        ULONG KvaShadowUserGlobal : 1;
        ULONG KvaShadowPcid : 1;
        ULONG KvaShadowInvpcid : 1;
        ULONG Reserved : 28;
    } KvaShadowFlags;
} SYSTEM_KERNEL_VA_SHADOW_INFORMATION, *PSYSTEM_KERNEL_VA_SHADOW_INFORMATION;



ID3D11RenderTargetView *backbuffer;    // global declaration

/**
 * Initialize and prepare Direct3D for use.
 *
 * @param hWnd The handle to the window which is used to render the graphics.
 */
void InitD3D(HWND hWnd) {
    cout << "Initializing DirectX ..." << endl;

    // Direct3D initialization
    // ...
    // create a struct to hold information about the swap chain
    DXGI_SWAP_CHAIN_DESC scd;

    // clear out the struct for use
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    // fill the swap chain description struct
    scd.BufferCount = 1;                                    // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
    scd.BufferDesc.Width = SCREEN_WIDTH;                    // set the back buffer width
    scd.BufferDesc.Height = SCREEN_HEIGHT;                  // set the back buffer height
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
    scd.OutputWindow = hWnd;                                // the window to be used
    scd.SampleDesc.Count = 4;                               // how many multisamples
    scd.Windowed = TRUE;                                    // windowed/full-screen mode
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;     // allow full-screen switching

    // create a device, device context and swap chain using the information in the scd struct
    D3D11CreateDeviceAndSwapChain(
        NULL,                    // A pointer to the video adapter to use when creating a device. (NULL = first)
        D3D_DRIVER_TYPE_HARDWARE,// The driver type to create (Usually HARDWARE)
        NULL,                    // A handle to a DLL that implements a software rasterizer. 
        0,                       // The runtime layers to enable
        NULL,                    // A pointer to an array of D3D_FEATURE_LEVELs, which determine the order of feature levels to attempt to create
        0,                       // Number of elements in pFeatureLevels
        D3D11_SDK_VERSION,       // The SDK version
        &scd,                    // A pointer to a swap chain description
        &swapchain,              // Returns the address of a pointer to the IDXGISwapChain object that represents the swap chain used for rendering.
        &dev,                    // Returns the address of a pointer to an ID3D11Device object that represents the device created.
        NULL,                    // Returns a pointer to a D3D_FEATURE_LEVEL, which represents the first element in an array of feature levels supported by the device.
        &devcon);                // Returns the address of a pointer to an ID3D11DeviceContext object that represents the device context.

    // check for Threading Support
    D3D11_FEATURE_DATA_THREADING threadSupport;
    HRESULT res = dev->CheckFeatureSupport(D3D11_FEATURE_THREADING, &threadSupport, sizeof(threadSupport));
    if (res != S_OK) {
        DWORD err = GetLastError();
        cout << "Error " << err << ": " << GetErrorMessage(err) << endl;
    }
    cout << "DriverConcurrentCreates: " << threadSupport.DriverConcurrentCreates << endl;
    cout << "DriverCommandLists: " << threadSupport.DriverCommandLists << endl;




    // set the render target

    // get the address of the back buffer
    ID3D11Texture2D *pBackBuffer;
    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    // use the back buffer address to create the render target
    // backbuffer is ID3D11RenderTargetView*
    dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
    pBackBuffer->Release();

    // set the render target as the back buffer
    devcon->OMSetRenderTargets(1, &backbuffer, NULL);



    // Set the viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = SCREEN_WIDTH;
    viewport.Height = SCREEN_HEIGHT;

    devcon->RSSetViewports(1, &viewport);

    InitPipeline();
    InitGraphics();
}

static int frameNo = 0;

// this is the function used to render a single frame
void RenderFrame() {
    cout << "Rendering frame " << frameNo << endl;

    // clear the back buffer to a solid color
    float red = (float) frameNo / 255.0;
    cout << "  RED: " << red << endl;

    float color[4] = { red, 0.2f, 0.4f, 1.0f };
    devcon->ClearRenderTargetView(backbuffer, color);

    // select which vertex buffer to display
    UINT stride = sizeof(VERTEX);
    UINT offset = 0;
    devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);

    // select which primtive type we are using
    devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // draw the vertex buffer to the back buffer
    devcon->Draw(3, 0);

    // do 3D rendering on the back buffer here

    // switch the back buffer and the front buffer
    swapchain->Present(0, 0);

    frameNo = (frameNo + 1) % 256;
}

// this is the function that cleans up Direct3D and COM
void CleanD3D() {
    cout << "Cleaning up D3d ..." << endl;

    swapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode

    // close and release all existing COM objects
    swapchain->Release();
    backbuffer->Release();
    dev->Release();
    devcon->Release();
}

// this is the function that creates the shape to render
void InitGraphics() {
    cout << "Initializing graphic ..." << endl;

    // create a triangle using the VERTEX struct
    VERTEX OurVertices[] = {
        // x        y     z     r     g     b     a
//        { 0.0f,   0.5f, 0.0f, {1.0f, 0.0f, 0.0f, 1.0f}},  //       *
//        { 0.5f, -0.5f, 0.0f, {0.0f, 0.0f, 1.0f, 1.0f}},  //          *
//        {-0.5f,  -0.5,  0.0f, {0.0f, 1.0f, 0.0f, 1.0f}},  //     *
        { 0.0f,   1.0f, 0.0f, {1.0f, 0.0f, 0.0f, 1.0f}},  //       *
        { 1.0f, -1.0f, 0.0f, {0.0f, 0.0f, 1.0f, 1.0f}},  //          *
        {-1.0f,  -1.0,  0.0f, {0.0f, 1.0f, 0.0f, 1.0f}},  //     *
    };

    // create the vertex buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
    bd.ByteWidth = sizeof(VERTEX) * 4;             // size is the VERTEX struct * 3
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

    dev->CreateBuffer(&bd, NULL, &pVBuffer);       // create the buffer

    // copy the vertices into the buffer
    D3D11_MAPPED_SUBRESOURCE ms;
    devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
    memcpy(ms.pData, OurVertices, sizeof(OurVertices));                 // copy the data
    devcon->Unmap(pVBuffer, NULL);                                      // unmap the buffer
}

string GetErrorMessage(DWORD dwErrorCode) {
    char buf[256];
    DWORD cchMsg = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL,  /* (not used with FORMAT_MESSAGE_FROM_SYSTEM) */
                                 dwErrorCode,
                                 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                 buf,
                                 sizeof(buf) / sizeof(wchar_t),
                                 NULL);

    return string(buf);
}

// this function loads and prepares the shaders
void InitPipeline() {
    cout << "Initializing Pipeline ..." << endl;

    // load and compile the two shaders
    cout << "Compiling VShader ..." << endl;
    ID3D10Blob *VS = 0;
    ID3DBlob *vsErrors = 0;
    HRESULT res = D3DCompileFromFile(
                    L"shaders.shader",  // Shader code source file name
                    0,                  // Optional parameter to specify shader macros
                    0,                  // Optional parameter to specify #include file handler
                    "VShader",          // Shader entry point function name
                    "vs_4_0",           // The shader target
                    0,                  // Shader compile options flags
                    0,                  // Effect compile options flags
                    &VS,                // Access to the compiled code
                    &vsErrors);         // Access to compiler error messages
    if (res != S_OK) {
        DWORD err = GetLastError();
        cout << "Error " << err << ": " << GetErrorMessage(err) << endl;
    }

    ID3D10Blob *PS = 0;
    ID3DBlob *psErrors = 0;
    cout << "Compiling PShader ..." << endl;
    res = D3DCompileFromFile(
                    L"shaders.shader", 
                    0, 
                    0, 
                    "PShader", 
                    "ps_4_0", 
                    0, 
                    0, 
                    &PS, 
                    &psErrors);
    if (res != S_OK) {
        DWORD err = GetLastError();
        cout << "Error " << err << ": " << GetErrorMessage(err) << endl;
    }

    cout << "Create shaders ..." << endl;

    // encapsulate both shaders into shader objects
    dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
    dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

    // set the shader objects
    devcon->VSSetShader(pVS, 0, 0);
    devcon->PSSetShader(pPS, 0, 0);

    // create the input layout object
    D3D11_INPUT_ELEMENT_DESC ied[] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    dev->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
    devcon->IASetInputLayout(pLayout);
}


// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow) {
    createConsole();

    cout << "Creating main window ..." << endl;
    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    
    // "To remove the window's background, all we need to do is comment one of 
    //  the members of the WINDOWCLASSEX struct that was used to set the 
    //  background color. Doing this leaves the background color untouched, which 
    //  means it won't be visible as a window for a second or two before the game 
    //  starts (important to making your game look professional)."
    // => It seems that this is purely aesthetic? Especially if the app starts in 
    // full screen mode?
    // wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);

    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

    if (!RegisterClassEx(&wcex)) {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Win32 Guided Tour"),
            NULL);

        return 1;
    }

	char buffer[1024]; // TODO

	// obtain information to display
	SYSTEM_KERNEL_VA_SHADOW_INFORMATION kvaInfo;
	NTSTATUS status = NtQuerySystemInformation(SystemKernelVaShadowInformation,
                                      &kvaInfo,
                                      sizeof(kvaInfo),
                                      NULL);
	if (!NT_SUCCESS(status)) {
		std::string msg = "Unknown";
		if (status == STATUS_INVALID_INFO_CLASS) {
			msg = "STATUS_INVALID_INFO_CLASS";
		}

		sprintf(buffer, "Call to NtQuerySystemInformation failed: %08X (%s)", status, msg.c_str());
        MessageBox(NULL,
            _T(buffer),
            _T("Win32 Guided Tour"),
            NULL);
        return 1;
	}

	lines.push_back("Line 1");
	lines.push_back("Line 2");
	lines.push_back("Line 3");

    hInst = hInstance; // Store instance handle in our global variable

    cout << "Desired client area: " << SCREEN_WIDTH << "x" << SCREEN_HEIGHT << endl;
    RECT wSize = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};   // note: left/top/right/bottom!
                                                        // (means, already considers the +1 when calulating the size later)
    AdjustWindowRect(&wSize, WS_OVERLAPPEDWINDOW, FALSE);
    LONG wWidth = wSize.right - wSize.left;
    LONG wHeight = wSize.bottom - wSize.top;
    cout << "Window Size: " << wWidth << "x" << wHeight << endl;

    HWND hWnd = CreateWindow(
        szWindowClass,          // The name of the application
        szTitle,                // The text that appears in the title bar
        WS_OVERLAPPEDWINDOW,    // The type of window to create
        800, // CW_USEDEFAULT,  // Initial X position
        200, // CW_USEDEFAULT,  // Initial Y position
        wWidth,                 // Initial width
        wHeight,                // Initial height
        NULL,                   // The parent of this window
        NULL,                   // The menu bar for this application
        hInstance,              // The first parameter from WinMain
        NULL                    // Not used in this application
    );

    if (!hWnd) {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Win32 Guided Tour"),
            NULL);

        return 1;
    }

    // The parameters to ShowWindow explained:
    // hWnd: the value returned from CreateWindow
    // nCmdShow: the fourth parameter from WinMain
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    InitD3D(hWnd);

    // Main message loop:
    SetTimer(NULL, NULL, 20, NULL);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        printf("MSG %d: %s\n", msg.message, getMessageNameFromId(msg.message));

//        if (msg.message == WM_TIMER) {
//        } else {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
//        }

        if (msg.message == WM_TIMER) {
            RenderFrame();
        }
    }

    // clean up DirectX and COM
    CleanD3D();

    printf("Press <RETURN> to terminate the application ...\n");
    getchar();
    return (int) msg.wParam;
    
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    printf("WNDPROC MSG %d: %s\n", message, getMessageNameFromId(message));

    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
    case WM_PAINT: {
        hdc = BeginPaint(hWnd, &ps);

        // Here your application is laid out.
        // For this introduction, we just print out "Hello, World!"
        // in the top left corner.
		std::vector<std::string>::iterator line = lines.begin();
		int y = 5;
		while(line != lines.end()) {
			const TCHAR* text = (*line).c_str();
			TextOut(hdc, 5, y, text, _tcslen(text));
			y += 20;
			line++;
		}

        // End application-specific layout section.

        EndPaint(hWnd, &ps);
		}
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}


void createConsole() {
    // Create a console - each Win32 application can only have one console
    AllocConsole();

    // get the global HANDLE for the console stdout
    HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
    freopen("CONOUT$", "w", stdout);

    // get the global HANDLE for the console stdin
    HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
    freopen("CONIN$", "r", stdin);

    ios::sync_with_stdio(true);

    printf("Console initialized.\n");
}

const char* getMessageNameFromId(UINT id) {
    const char* translatedMessage = wmTranslation[id];
    if (translatedMessage == NULL) {
        translatedMessage = "UnknownMessage";
    }

    return translatedMessage;
}
