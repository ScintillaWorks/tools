// AAClr - tool to adjust the aero colors according to the desktop wallpaper

// Copyright (C) 2011-2012, 2015 - Stefan Kueng

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//

#include "stdafx.h"
#include "resource.h"
#include "NotifySlider.h"
#include "Utils.h"
#include <CommCtrl.h>


CNotifySlider::CNotifySlider(HWND hParent)
    : xPos(0)
    , yPos(0)
    , m_hParent(hParent)
    , currentBright(0)
{
}

CNotifySlider::~CNotifySlider(void)
{
}

LRESULT CNotifySlider::DlgFunc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (uMsg)
    {
    case WM_INITDIALOG:
        {
            InitDialog(hwndDlg, IDI_AACLR);

            currentBright = CUtils::GetBrightness();
            HWND hSlider = GetDlgItem(*this, IDC_BRIGHT);
            SendMessage(hSlider, TBM_SETRANGE, 0, MAKELONG(0, 100));
            SendMessage(hSlider, TBM_SETPOS, 1, 100-currentBright);

            RECT rc;
            GetWindowRect(hwndDlg, &rc);
            SetWindowPos(hwndDlg, NULL, xPos, yPos-(rc.bottom-rc.top)-50, 0, 0, SWP_NOSIZE);
            return TRUE;
        }
        break;
    case WM_VSCROLL:
        {
            HWND hSlider = GetDlgItem(*this, IDC_BRIGHT);
            DWORD pos = (DWORD)SendMessage(hSlider, TBM_GETPOS, 0, 0);
            CUtils::SetBrightness(100-pos);
        }
        break;
    case WM_ACTIVATE:
        if (LOWORD(wParam) == WA_INACTIVE)
            EndDialog(*this, 0);
        break;
    case WM_COMMAND:
        return DoCommand(LOWORD(wParam), HIWORD(wParam));
    default:
        return FALSE;
    }
    return FALSE;
}

LRESULT CNotifySlider::DoCommand(int id, int /*msg*/)
{
    switch (id)
    {
    case IDOK:
    case IDCANCEL:
        EndDialog(*this, id);
        break;
    }
    return 1;
}
