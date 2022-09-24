/*
* Copyright � 2022 s1zeoftr1ckster. Contacts: <sizeoftrickster@gmail.com>
* License: http://opensource.org/licenses/MIT
*/

#include "WndProcHook.h"
#include "Utility.hpp"
#include "GForce.h"

WndProcHook::WndProcHook() {
	// ������������� ��� wndproc
	wndprocHook.set_cb( [this]( auto&&... args ) { return WndProcHook::WndProcHooked( args... ); } );
	wndprocHook.install();
}

WndProcHook::~WndProcHook() {
	// ������� ��� wndproc
	wndprocHook.remove();
}

LRESULT WndProcHook::WndProcHooked( const decltype( wndprocHook )& hook, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
	if ( uMsg == WM_KEYDOWN && Utility::PedIsInPlane() ) {
		// ������ ���������� ��� ����������
		// p.s. ���������� ���������, �� alpha ������ ����� 0.0f, ������� ���� ������ �� ��������
		if ( GForce::GetAlpha() >= 0.75f && Utility::GenerateRandomNumberFromXToY( 1, 100 ) > 50 )
			return hook.get_trampoline()( hWnd, uMsg, NULL, lParam );
		// ����������������� �� ������� X
		if ( wParam == 0x58 ) 
			Utility::LeaveFromPlainAndGiveParachute();
	}
	return hook.get_trampoline()( hWnd, uMsg, wParam, lParam );
}