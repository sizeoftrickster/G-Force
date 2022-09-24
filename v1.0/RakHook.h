#ifndef _RAK_HOOK_H_
#define _RAK_HOOK_H_

#include "raknet/BitStream.h"
#include "raknet/PacketEnumerations.h"
#include "raknet/PacketPriority.h"
#include <kthook/kthook.hpp>

using OutcomingPacketsFunctionPrototype = char( __thiscall* )( void*, RakNet::BitStream*, PacketPriority, PacketReliability, char );

class RakHook
{
	void* pOutcomingPackets = nullptr;

	kthook::kthook_simple<OutcomingPacketsFunctionPrototype> outcomingPacketsHook{ 0x01 };
public:
	RakHook();
	~RakHook();
	struct VehicleSync
	{
		float Quaternion[4];
		float Position[3];
		float VecMoveSpeed[3];
		float Health;
	};
	static VehicleSync GetVehicleSync();
private:
	char OutcomingPacketsHooked( const decltype( outcomingPacketsHook )& hook, void*, RakNet::BitStream*, PacketPriority, PacketReliability, char );
}; // class RakHook

#endif // !_RAK_HOOK_H_