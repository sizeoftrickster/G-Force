/*
* Copyright © 2022 s1zeoftr1ckster. Contacts: <sizeoftrickster@gmail.com>
* License: http://opensource.org/licenses/MIT
*/

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <filesystem>
#include "json/json.hpp"

class Config
{
public:
	struct Data
	{
		bool EnabledGForce = true;
	}; // struct Data

	static void Save();
	static void Load();

	static struct Data* Get();
private:
	static std::filesystem::path GetConfig();
}; // class Config

#endif // !_CONFIG_H_