#include "RakHook.h"
#include "samp/samp.hpp"

RakHook::VehicleSync VehSync;

RakHook::RakHook() {
	switch ( SAMP::GetSAMPVersion() ) {
		case ( SAMP::sampVersion::R1 ): {
			pOutcomingPackets = reinterpret_cast<void*>( SAMP::GetSAMPHandle() + 0x307F0 );
			break;
		}
		case ( SAMP::sampVersion::R2 ): {
			pOutcomingPackets = reinterpret_cast<void*>( SAMP::GetSAMPHandle() + 0x308D0 );
			break;
		}
		case ( SAMP::sampVersion::R3 ): {
			pOutcomingPackets = reinterpret_cast<void*>( SAMP::GetSAMPHandle() + 0x33BA0 );
			break;
		}
		case ( SAMP::sampVersion::R4 ): {
			pOutcomingPackets = reinterpret_cast<void*>( SAMP::GetSAMPHandle() + 0x34290 );
			break;
		}
		default: {
			break;
		}
	}
	if ( !pOutcomingPackets )
		return;
	outcomingPacketsHook.set_dest( pOutcomingPackets );
	outcomingPacketsHook.set_cb([this]( auto&&... args ) { return RakHook::OutcomingPacketsHooked( args... ); });
	outcomingPacketsHook.install();
}

RakHook::~RakHook() {
	outcomingPacketsHook.remove();
}

RakHook::VehicleSync RakHook::GetVehicleSync() {
	return VehSync;
}

char RakHook::OutcomingPacketsHooked( const decltype( outcomingPacketsHook )& hook, void* _this, RakNet::BitStream* BitStream, PacketPriority PacketPriority, PacketReliability PacketReliability, char OrderingChannel ) {
	uint8_t PacketId;
	BitStream->Read( PacketId );

	if ( PacketId == PacketEnumeration::ID_VEHICLE_SYNC ) {
		// Clearing Struct
		memset( &VehSync, NULL, sizeof( VehSync ) );

		// VehicleID, LeftRightKeys, UpDownKeys, sKeys
		BitStream->IgnoreBits( 16 + 16 + 16 + 16 );
		
		// Read Quaternion[4]
		BitStream->Read( VehSync.Quaternion[0] );
		BitStream->Read( VehSync.Quaternion[1] );
		BitStream->Read( VehSync.Quaternion[2] );
		BitStream->Read( VehSync.Quaternion[3] );
		
		// Read Position[3]
		BitStream->Read( VehSync.Position[0] );
		BitStream->Read( VehSync.Position[1] );
		BitStream->Read( VehSync.Position[2] );

		// Read VecMoveSpeed[3]
		BitStream->Read( VehSync.VecMoveSpeed[0] );
		BitStream->Read( VehSync.VecMoveSpeed[1] );
		BitStream->Read( VehSync.VecMoveSpeed[2] );
		
		// Read Health
		BitStream->Read( VehSync.Health );
	}

	return hook.get_trampoline()( _this, BitStream, PacketPriority, PacketReliability, OrderingChannel );
}