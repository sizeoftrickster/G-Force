#include "Main.h"
#include "samp/samp.hpp"
#include "Direct3D9Hook.h"
#include "RakHook.h"

void WriteMemory( LPVOID Dest, size_t Size, int Val ) {
	DWORD NewProtection;
	VirtualProtect( Dest, Size, PAGE_EXECUTE_READWRITE, &NewProtection );
	memset( Dest, Val, Size );
	VirtualProtect( Dest, Size, NewProtection, &NewProtection );
}

void HydraPath( bool enable = true ) {
	if ( enable ) {
		WriteMemory( (void*)7187944, 1, 235 );
		WriteMemory( (void*)6812264, 1, 235 );
		WriteMemory( (void*)7153181, 1, 235 );	
	} else {
		WriteMemory( (void*)7187944, 1, 117 );
		WriteMemory( (void*)6812264, 1, 122 );
		WriteMemory( (void*)7153181, 1, 123 );
	}
}

void AsiPlugin::GameloopHooked( const decltype( gameloopHook )& hook ) {
	static bool isInitialize = false;
	if ( !isInitialize && SAMP::IsSampInitialized() ) {
		HydraPath();
		directHook = std::make_unique<Direct3D9Hook>();
		rakHook = std::make_unique<RakHook>();
		isInitialize = true;
	}
	return hook.get_trampoline()();
}

AsiPlugin::AsiPlugin() {
	gameloopHook.set_cb([this]( auto&&... args ) { return AsiPlugin::GameloopHooked( args... ); });
	gameloopHook.install();
}

AsiPlugin::~AsiPlugin() {
	HydraPath( false );
	gameloopHook.remove();
}