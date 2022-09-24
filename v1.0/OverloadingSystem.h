#ifndef _OVERLOADING_SYSTEM_H_
#define _OVERLOADING_SYSTEM_H_

#include <Windows.h>

class OverloadingSystem
{
	ULONGLONG Time = GetTickCount64();
	float alpha = 0.0f;
public:
	OverloadingSystem();
	~OverloadingSystem();

	void OnDraw();
	void OnReset();
private:
	float Magnitude( float x, float y, float z );
}; // class OverloadingSystem

#endif // !_OVERLOADING_SYSTEM_H_