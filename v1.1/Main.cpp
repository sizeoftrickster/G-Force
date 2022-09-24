/*
* Copyright � 2022 s1zeoftr1ckster. Contacts: <sizeoftrickster@gmail.com>
* License: http://opensource.org/licenses/MIT
*/

#include "Main.h"
#include "Direct3D9Hook.h"
#include "WndProcHook.h"
#include "Utility.hpp"
#include "Config.h"

void HydraPath( bool enable = true ) {
	if ( enable ) {
		Utility::WriteMemory( (void*)0x6DADE8, 1, 235 );
		Utility::WriteMemory( (void*)0x67F268, 1, 235 );
		Utility::WriteMemory( (void*)0x6D261D, 1, 235 );
	} else {
		Utility::WriteMemory( (void*)0x6DADE8, 1, 117 );
		Utility::WriteMemory( (void*)0x67F268, 1, 122 );
		Utility::WriteMemory( (void*)0x6D261D, 1, 123 );
	}
}

void AsiPlugin::GameloopHooked( const decltype( gameloopHook )& hook ) {
	static bool isInitialize = false;
	if ( !isInitialize ) {
		// ��������� ������
		Config::Load();
		// ������������� ������ ������������������ ������� "rand()"
		srand( (size_t)time( NULL ) );
		// ������������� ����, ������� ������� ������ � ����������� Hydra
		HydraPath();
		// ����� ���������
		directHook = std::make_unique<Direct3D9Hook>();
		wndprocHook = std::make_unique<WndProcHook>();
		isInitialize = true;
	}
	return hook.get_trampoline()();
}

AsiPlugin::AsiPlugin() {
	// ������������� ��� gameloop
	gameloopHook.set_cb( [this]( auto&&... args ) { return AsiPlugin::GameloopHooked( args... ); } );
	gameloopHook.install();
}

AsiPlugin::~AsiPlugin() {
	// ������������� ����������� ������ ����������� Hydra
	HydraPath( false );
	// ������� ��� gameloop
	gameloopHook.remove();
}