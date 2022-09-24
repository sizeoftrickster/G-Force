#include "Direct3D9Hook.h"
#include "OverloadingSystem.h"

Direct3D9Hook::Direct3D9Hook() {
	overloadingSystem = std::make_shared<OverloadingSystem>();
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
	overloadingSystem->OnDraw();
	return std::nullopt;
}

std::optional<HRESULT> Direct3D9Hook::ResetHooked( const decltype( resetHook )& hook, IDirect3DDevice9*, D3DPRESENT_PARAMETERS* ) {
	overloadingSystem->OnReset();
	return std::nullopt;
}