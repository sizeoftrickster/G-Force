/*
* Copyright © 2022 s1zeoftr1ckster. Contacts: <sizeoftrickster@gmail.com>
* License: http://opensource.org/licenses/MIT
*/

#ifndef _DIRECT3D9_WRAPPER_H_
#define _DIRECT3D9_WRAPPER_H_

#include <cstddef>
#include <cstdint>
#include <string>
#include <d3d9.h>

class Direct3D9Wrapper
{
    static constexpr auto pGameDevice{ 0xC97C28 };
    static constexpr auto pGameHWND{ 0xC17054 };
public:
    static IDirect3DDevice9* GetGameDevice() {
        return *reinterpret_cast<IDirect3DDevice9**>( pGameDevice );
    }
    static HWND GetGameHWND() {
        return **reinterpret_cast<HWND**>( pGameHWND );
    }
    static std::uintptr_t GetVTableFunctionAddress( std::uint32_t VTableIndex ) {
        return ( *reinterpret_cast<std::uintptr_t**>( GetGameDevice() ) )[VTableIndex];
    }
}; // class Direct3D9Wrapper

#endif // !_DIRECT3D9_WRAPPER_H_