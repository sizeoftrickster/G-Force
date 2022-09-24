/*
* Copyright © 2022 s1zeoftr1ckster. Contacts: <sizeoftrickster@gmail.com>
* License: http://opensource.org/licenses/MIT
*/

#include "Direct3D9Hook.h"
#include "GForce.h"

Direct3D9Hook::Direct3D9Hook() {
	gForce = std::make_shared<GForce>();
	presentHook.before += [this]( auto&&... args ) { return Direct3D9Hook::PresentHooked( args... ); };
	resetHook.before += [this]( auto&&... args ) { return Direct3D9Hook::ResetHooked( args... ); };
	presentHook.install();
	resetHook.install();
}

Direct3D9Hook::~Direct3D9Hook() {
	resetHook.remove();
	presentHook.remove();
}

std::optional<HRESULT> Direct3D9Hook::PresentHooked( const decltype( presentHook )& hook, IDirect3DDevice9*, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA* ) {
	gForce->OnDraw();
	return std::nullopt;
}

std::optional<HRESULT> Direct3D9Hook::ResetHooked( const decltype( resetHook )& hook, IDirect3DDevice9*, D3DPRESENT_PARAMETERS* ) {
	gForce->OnReset();
	return std::nullopt;
}