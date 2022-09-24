/*
* Copyright © 2022 s1zeoftr1ckster. Contacts: <sizeoftrickster@gmail.com>
* License: http://opensource.org/licenses/MIT
*/

#ifndef _G_FORCE_H_
#define _G_FORCE_H_

#include <Windows.h>

class GForce
{
	ULONGLONG DeltaVelocityTime = GetTickCount64();
	ULONGLONG AlphaTime = GetTickCount64();

	float mps_0 = 0.0f;
public:
	GForce();
	~GForce();

	void OnDraw();
	void OnReset();

	static float GetAlpha();
private:
	void CreateImGuiContext();
}; // class GForce

#endif // !_G_FORCE_H_