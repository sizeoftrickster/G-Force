#pragma execution_character_set("utf-8")
#include "OverloadingSystem.h"
#include "gtasa/gtasa.hpp"
#include "samp/samp.hpp"
#include "RakHook.h"
#include "UbuntuFont.hpp"
#include "Fontello.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx9.h"
#include "Direct3D9Wrapper.hpp"

static bool& isMenuOpened = *reinterpret_cast<bool*>( 0xBA67A4 );

OverloadingSystem::OverloadingSystem() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
	
	ImFontConfig font_config;
	font_config.GlyphExtraSpacing.x = 1.f;
	ImGui::GetIO().Fonts->AddFontFromMemoryCompressedBase85TTF( ubuntu_compressed_data_base85, 20.0f, &font_config, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic() );

	static const ImWchar icons_ranges[] = { FONTELLO_ICON_MIN,  FONTELLO_ICON_MAX, 0 };
	ImFontConfig config_;
	config_.MergeMode = true;
	ImGui::GetIO().Fonts->AddFontFromMemoryCompressedBase85TTF( fontello_compressed_data_base85, 19.0f, &config_, icons_ranges );

	ImGui_ImplWin32_Init( Direct3D9Wrapper::GetGameHWND() );
	ImGui_ImplDX9_Init( Direct3D9Wrapper::GetGameDevice() );
}

OverloadingSystem::~OverloadingSystem() {
	ImGui::DestroyContext();
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
}

void OverloadingSystem::OnDraw() {
	if ( !isMenuOpened && !CPed::IsPlane() && alpha != 0.0f ) alpha = 0.0f;
	if ( !isMenuOpened && CPed::IsPlane() ) {
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		
		auto [posX_, posY_] = CPed::GetScreenResolution();
		auto [posX, posY] = SAMP::Input::CreateInstance()->GetChatboxPos();
		auto speed = Magnitude( RakHook::GetVehicleSync().VecMoveSpeed[0], 
								RakHook::GetVehicleSync().VecMoveSpeed[1], 
								RakHook::GetVehicleSync().VecMoveSpeed[2] ) * 100;

		auto DrawList = ImGui::GetBackgroundDrawList();

		auto g = ( ( speed * speed ) / ( 9.8f * 1000 ) ) + 1;

		if ( GetTickCount64() - Time > 100 ) {
			if ( g > 9.0 && alpha < 0.95f ) {
				alpha += 0.05f;
			} else if ( g < 9.0 && alpha > 0.1f ) {
				alpha -= 0.100f;
			}
			Time = GetTickCount64();
		}
		DrawList->AddRectFilled( ImVec2( 0.0f, 0.0f ), ImVec2( posX_, posY_ ), ImGui::GetColorU32( ImVec4( 0.0f, 0.0f, 0.0f, alpha ) ) );

		char indicatorsBuffer[256];
		sprintf( indicatorsBuffer, "IAS       %.0f км/ч\nВЫС     %.0f м\nHP        %.0f ед.",
			speed, RakHook::GetVehicleSync().Position[2], RakHook::GetVehicleSync().Health  );
		DrawList->AddText( ImVec2( posX, posY + 45 ), IM_COL32_WHITE, indicatorsBuffer );
		
		if ( g > 9.0 ) {
			char olBuffer[128];
			sprintf( olBuffer, "%s Критическая перегрузка %.0fG!", ICON_ATTENTION, g );
			auto textsize = ImGui::CalcTextSize( olBuffer );
			DrawList->AddText( ImVec2( posX_ / 2 - textsize.x / 2, ( posY_ / 2 - textsize.y / 2 ) + 200.0f ), IM_COL32( 255, 0, 0, 255 ), olBuffer );
		}

		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData() );
	}
}

void OverloadingSystem::OnReset() {
	ImGui_ImplDX9_InvalidateDeviceObjects();
}

float OverloadingSystem::Magnitude( float x, float y, float z ) {
	return sqrtf( x * x + y * y + z * z );
}