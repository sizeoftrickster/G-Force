/*
* Copyright © 2022 s1zeoftr1ckster. Contacts: <sizeoftrickster@gmail.com>
* License: http://opensource.org/licenses/MIT
*/

#ifndef _DIRECT3D9_HOOK_H_
#define _DIRECT3D9_HOOK_H_

#include <d3d9.h>
#include <memory> // std::shared_ptr
#include <kthook/kthook.hpp> // kthook::kthook_signal
#include "Direct3D9Wrapper.hpp"

using PresentPrototype = HRESULT( __stdcall* )( IDirect3DDevice9*, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA* );
using ResetPrototype = HRESULT( __stdcall* )( IDirect3DDevice9*, D3DPRESENT_PARAMETERS* );

class Direct3D9Hook
{
	std::shared_ptr<class GForce> gForce;

	kthook::kthook_signal<PresentPrototype> presentHook{ Direct3D9Wrapper::GetVTableFunctionAddress( 17 ) };
	kthook::kthook_signal<ResetPrototype> resetHook{ Direct3D9Wrapper::GetVTableFunctionAddress( 16 ) };
public:
	Direct3D9Hook();
	~Direct3D9Hook();
private:
	std::optional<HRESULT> PresentHooked( const decltype( presentHook )& hook, IDirect3DDevice9*, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA* );
	std::optional<HRESULT> ResetHooked( const decltype( resetHook )& hook, IDirect3DDevice9*, D3DPRESENT_PARAMETERS* );
}; // class Direct3D9Hook

#endif // !_DIRECT3D9_HOOK_H_
