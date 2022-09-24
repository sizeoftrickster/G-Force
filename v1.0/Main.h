#ifndef _MAIN_H_
#define _MAIN_H_

#include <kthook/kthook.hpp>
#include <memory>

using GameloopPrototype = void(__cdecl*)();

class AsiPlugin
{
	kthook::kthook_simple<GameloopPrototype> gameloopHook{ 0x748DA3 };

	std::unique_ptr<class Direct3D9Hook> directHook;
	std::unique_ptr<class RakHook> rakHook;
public:
	explicit AsiPlugin();
	virtual ~AsiPlugin();
private:
	void GameloopHooked( const decltype( gameloopHook )& hook );
} AsiPlugin; // class AsiPlugin

#endif // !_MAIN_H_