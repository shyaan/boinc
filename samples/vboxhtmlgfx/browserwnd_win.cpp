// This file is part of BOINC.
// http://boinc.berkeley.edu
// Copyright (C) 2010-2012 University of California
//
// BOINC is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// BOINC is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with BOINC.  If not, see <http://www.gnu.org/licenses/>.

#define _ATL_FREE_THREADED
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
#include <AtlBase.h>
#include <AtlCom.h>
#include <AtlCtl.h>
#include <AtlWin.h>
#include <AtlStr.h>
#include <AtlFile.h>
#include <AtlTypes.h>
#include <exdisp.h>
#include <exdispid.h>
#include <string>
#include "win_util.h"
#include "version.h"
#include "boinc_api.h"
#include "diagnostics.h"
#include "filesys.h"
#include "vboxhtmlgfx_win.h"
#include "vboxlogging.h"
#include "vboxcheckpoint.h"
#include "browserctrl_win.h"
#include "browserwnd_win.h"


CHTMLBrowserWnd::CHTMLBrowserWnd()
{
    m_pBrowserHost = NULL;
}

CHTMLBrowserWnd::~CHTMLBrowserWnd()
{
}

LRESULT CHTMLBrowserWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    HRESULT hr;
	RECT rcClient;
    CComPtr<IUnknown> pCtrl;
    CComVariant v;


    // Create Control Host
    hr = CComObject<CHTMLBrowserHost>::CreateInstance(&m_pBrowserHost);
    ATLASSERT(SUCCEEDED(hr));

    // Create Control Window
	GetClientRect(&rcClient);
    m_pBrowserHost->Create(m_hWnd, rcClient, NULL, WS_CHILD | WS_VISIBLE);
    ATLASSERT(m_pBrowserHost->m_hWnd != NULL);

    // Create Control
    m_pBrowserHost->CreateControlEx(
        L"Shell.Explorer",
        m_pBrowserHost->m_hWnd,
        NULL,
        &pCtrl,
        __uuidof(DWebBrowserEvents2),
        (IUnknown*)(IDispEventImpl<1, CHTMLBrowserWnd, &__uuidof(DWebBrowserEvents2), &LIBID_SHDocVw, 1, 1>*)this
    );


    // Get an IWebBrowser2 interface on the control and navigate to a page.
    m_pBrowserCtrl = pCtrl;

    if (m_pBrowserCtrl) {
        m_pBrowserCtrl->Navigate(CComBSTR("http://www.romwnet.org/"), &v, &v, &v, &v);
    }

    bHandled = TRUE;
	return 0;
}

LRESULT CHTMLBrowserWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    DestroyWindow();
	bHandled = TRUE;
	return 0;
}

LRESULT CHTMLBrowserWnd::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;
	return 0;
}

LRESULT CHTMLBrowserWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    m_pBrowserHost->MoveWindow(0, 0, LOWORD(lParam), HIWORD(lParam));
	bHandled = TRUE;
	return 0;
}

void CHTMLBrowserWnd::OnNavigateComplete(IDispatch* pDisp, VARIANT* URL)
{
    vboxlog_msg("URL Change Detected. (%S)", URL->bstrVal);
}

