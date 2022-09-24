/*
* Copyright © 2022 s1zeoftr1ckster. Contacts: <sizeoftrickster@gmail.com>
* License: http://opensource.org/licenses/MIT
*/

#ifndef _UTILITY_HPP_
#define _UTILITY_HPP_

#include <utility> // std::pair, std::make_pair
#include <random> // rand
#include <filesystem>
#include <plugin_sa.h>

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

class Utility
{
public:
	static std::pair<float, float> GetScreenResolution() {
		auto posX = *reinterpret_cast<int*>( 0xC9C040 );
		auto posY = *reinterpret_cast<int*>( 0xC9C044 );
		return std::make_pair( (float)posX, (float)posY );
	}
	static bool PedIsInPlane() {
		CPed* ped = FindPlayerPed( -1 );
		CVehicle* veh = FindPlayerVehicle( -1, true );
		if ( !veh || !ped )
			return false;
		return veh->m_nVehicleSubClass == VEHICLE_PLANE && veh->IsDriver( ped );
	}
	static bool PedInInHydra() {
		CPed* ped = FindPlayerPed( -1 );
		CVehicle* veh = FindPlayerVehicle( -1, true );
		if ( !veh || !ped )
			return false;
		return veh->m_nModelIndex == 520 && veh->IsDriver( ped );
	}
	static void LeaveFromPlainAndGiveParachute() {
		CPed* ped = FindPlayerPed();
		if ( !ped )
			return;
		// Clearing Tasks
		ped->m_pIntelligence->FlushImmediately( false );
		ped->m_pIntelligence->FlushImmediately( true );
		// Give And Set Parachute
		ped->GiveWeapon( eWeaponType::WEAPON_PARACHUTE, 1, true );
		ped->SetCurrentWeapon( eWeaponType::WEAPON_PARACHUTE );
	}
	static int GenerateRandomNumberFromXToY( int X, int Y ) {
		return X + ( rand() / ( RAND_MAX / ( Y - X ) ) );
	}
	static std::filesystem::path GetCurrentModulePath() {
		wchar_t lpFilename[MAX_PATH];
		GetModuleFileNameW( reinterpret_cast<HMODULE>( &__ImageBase ), lpFilename, MAX_PATH );
		return lpFilename;
	}
	static void WriteMemory( LPVOID Dest, size_t Size, int Val ) {
		DWORD NewProtection;
		VirtualProtect( Dest, Size, PAGE_EXECUTE_READWRITE, &NewProtection );
		memset( Dest, Val, Size );
		VirtualProtect( Dest, Size, NewProtection, &NewProtection );
	}
}; // class Utility

#endif // !_UTILITY_HPP_