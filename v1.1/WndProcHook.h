/*
* Copyright © 2022 s1zeoftr1ckster. Contacts: <sizeoftrickster@gmail.com>
* License: http://opensource.org/licenses/MIT
*/

#ifndef _WND_PROC_HOOK_H_
#define _WND_PROC_HOOK_H_

#include <kthook/kthook.hpp> // kthook::simple

using WndProcPrototype = LRESULT( __stdcall* )( HWND, UINT, WPARAM, LPARAM );

class WndProcHook
{
	kthook::kthook_simple<WndProcPrototype> wndprocHook{ 0x747EB0 };
public:
	WndProcHook();
	~WndProcHook();
private:
	LRESULT WndProcHooked( const decltype( wndprocHook )& hook, HWND, UINT, WPARAM, LPARAM );
}; // class WndProcHook

#endif // !_WND_PROC_HOOK_H_