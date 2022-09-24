/*
* Copyright © 2022 s1zeoftr1ckster. Contacts: <sizeoftrickster@gmail.com>
* License: http://opensource.org/licenses/MIT
*/

#include "Config.h"
#include "Utility.hpp"

struct Config::Data data;

struct Config::Data* Config::Get() {
	return &data;
}

void Config::Save() {
	nlohmann::ordered_json json;
	json["EnabledGForce"] = data.EnabledGForce;
	std::ofstream( GetConfig().string() ) << json.dump( 4 );
}

void Config::Load() {
	std::ifstream config( GetConfig().string() );
	if ( !config.is_open() )
		return Save();
	try {
		std::string content = std::string( ( std::istreambuf_iterator<char>( config ) ), std::istreambuf_iterator<char>() );
		auto json = nlohmann::json::parse( content );
		json["EnabledGForce"].get_to<bool>( data.EnabledGForce );
	} catch ( ... ) {
		Save();
	}

	config.close();
}

std::filesystem::path Config::GetConfig() {
	std::filesystem::path modulePath = Utility::GetCurrentModulePath();
	std::filesystem::path path = std::filesystem::current_path();
	std::filesystem::path name = "GForce";
	if ( modulePath.has_parent_path() )
		path = modulePath.parent_path();
	if ( modulePath.has_stem() )
		name = modulePath.stem();
	return ( path / ( name.string() + ".json" ) );
}