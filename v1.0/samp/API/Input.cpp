#include "Input.h"
#include "Library.h"

SAMP::Input* SAMP::Input::self{ nullptr };

SAMP::Input::Input() {
	switch ( GetSAMPVersion() ) {
		case ( sampVersion::R1 ): {
			pInput = reinterpret_cast<void*>( GetSAMPHandle() + 0x21A0E8 );
			break;
		}
		case ( sampVersion::R2 ): {
			pInput = reinterpret_cast<void*>( GetSAMPHandle() + 0x21A0F0 );
			break;
		}
		case ( sampVersion::R3 ): {
			pInput = reinterpret_cast<void*>( GetSAMPHandle() + 0x26E8CC );
			break;
		}
		case ( sampVersion::R4 ): {
			pInput = reinterpret_cast<void*>( GetSAMPHandle() + 0x26E9FC );
			break;
		}
		default: {
			break;
		}
	}
	ofInputBox = 0x8;
	ofPosX = 0x8;
	ofPosY = 0xC;
}

SAMP::Input* SAMP::Input::CreateInstance() {
	if ( !self )
		self = new Input();
	return self;
}

std::tuple<float, float> SAMP::Input::GetChatboxPos() {
	if ( !pInput )
		return std::make_tuple( -1.0f, -1.0f );
	auto struct_ = *reinterpret_cast<int*>( *(size_t*)pInput + ofInputBox );

	auto posX = *reinterpret_cast<int*>( struct_ + ofPosX );
	auto posY = *reinterpret_cast<int*>( struct_ + ofPosY );
	return std::make_tuple( (float)posX, (float)posY );
}

void SAMP::Input::DeleteInstance() {
	if ( self ) {
		delete self;
		self = nullptr;
	}
}