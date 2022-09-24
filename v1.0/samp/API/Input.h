#ifndef _SAMP_INPUT_H_
#define _SAMP_INPUT_H_

#include <tuple>

namespace SAMP
{
	class Input
	{
		static Input* self;

		void* pInput = nullptr;
		int ofInputBox = 0;
		int ofPosX = 0;
		int ofPosY = 0;

		Input();
		~Input(){};
	public:
		static Input* CreateInstance();

		std::tuple<float, float> GetChatboxPos();

		static void DeleteInstance();
	}; // class Input
}; // namespace SAMP

#endif // !_SAMP_INPUT_H_