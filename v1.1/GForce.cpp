/*
* Copyright © 2022 s1zeoftr1ckster. Contacts: <sizeoftrickster@gmail.com>
* License: http://opensource.org/licenses/MIT
*/

#pragma execution_character_set("utf-8")
#include "GForce.h"
// ImGui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx9.h"
// Fonts For ImGui
#include "UbuntuFont.hpp"
#include "IconsFont.hpp"
// Direct3DWrapper For Getting GameHWND and GameDevice
#include "Direct3D9Wrapper.hpp"
// Config
#include "Config.h"
// Other
#include "Utility.hpp"

static bool& isMenuOpened = *reinterpret_cast<bool*>( 0xBA67A4 );
float alpha = 0.0f;

GForce::GForce() {
	IMGUI_CHECKVERSION();
	CreateImGuiContext();
	ImGui_ImplWin32_Init( Direct3D9Wrapper::GetGameHWND() );
	ImGui_ImplDX9_Init( Direct3D9Wrapper::GetGameDevice() );
}

GForce::~GForce() {
	ImGui::DestroyContext();
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
}

void GForce::OnDraw() {
	if ( !Utility::PedIsInPlane() && alpha != 0.0f ) alpha = 0.0f;
	if ( !isMenuOpened && Utility::PedIsInPlane() ) {
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// DrawList
		auto drawList = ImGui::GetBackgroundDrawList();

		// Getting Screen Resolution => X, Y
		auto [posX, posY] = Utility::GetScreenResolution();
		// Find Local Ped Vehicle
		CVehicle* vehicle = FindPlayerVehicle( -1, true );
		// Hydra Path For No Delay Firing
		if ( vehicle->m_nModelIndex == 520 )
			vehicle->SetFiringRateMultiplier( 0.0f );
		// Getting Vehicle Speed
		auto speed = vehicle->m_vecMoveSpeed.Magnitude() * 100.0f;

		if ( Config::Get()->EnabledGForce ) {
			// Required: delta speed in meters per second
			auto mps_1 = speed / 3.6f;
			// 1/10 sec
			if ( GetTickCount64() - DeltaVelocityTime < 100 ) {
				mps_0 = speed / 3.6f;
				DeltaVelocityTime = GetTickCount64();
			}
			auto speed_delta = mps_1 - mps_0;

			auto g = ( ( speed_delta ) / ( 1000.f * 9.81f ) ) + 0.001f;

			if ( GetTickCount64() - AlphaTime > 100 ) {
				if ( g >= 0.009f && alpha < 0.95f ) {
					alpha += 0.05f;
				} else if ( g < 0.009f && alpha > 0.1f ) {
					alpha -= 0.100f;
				}
				AlphaTime = GetTickCount64();
			}

			drawList->AddRectFilled( ImVec2( 0.0f, 0.0f ), ImVec2( posX, posY ), ImGui::GetColorU32( ImVec4( 0.f, 0.f, 0.f, alpha ) ) );

			if ( g >= 0.009f ) {
				char gfBuf[128];
				auto _g = g * 1000.f;
				sprintf( gfBuf, "%s Критическая перегрузка %.0fG!", ICON_ATTENTION, _g );
				drawList->AddText(
					ImVec2( posX / 2 - ImGui::CalcTextSize( gfBuf ).x / 2, ( posY / 2 - ImGui::CalcTextSize( gfBuf ).y / 2 ) + 200.0f ), 
					IM_COL32( 255, 0, 0, 255 ), gfBuf
				);
			}
		}

		// Draw First Indicator
		auto speedIndicatorVec = ImVec2( 10.0f, posY / 2 );
		std::string speedIndicatorText = "СКОР    " + std::to_string( (int)speed );
		auto speedIndicatorTextSize = ImGui::CalcTextSize( speedIndicatorText.c_str() );
		drawList->AddText( speedIndicatorVec, IM_COL32_WHITE, speedIndicatorText.c_str() );
		drawList->AddText( ImVec2( speedIndicatorVec.x + speedIndicatorTextSize.x, speedIndicatorVec.y ), IM_COL32_BLACK, "км/ч" );

		// Draw Second Indicator
		auto healthIndicatorVec = ImVec2( speedIndicatorVec.x, speedIndicatorVec.y + 15 );
		std::string heightIndicatorText = "ВЫС      " + std::to_string( (int)vehicle->m_matrix->pos.z );
		auto heightIndicatorTextSize = ImGui::CalcTextSize( heightIndicatorText.c_str() );
		drawList->AddText( healthIndicatorVec, IM_COL32_WHITE, heightIndicatorText.c_str() );
		drawList->AddText( ImVec2( healthIndicatorVec.x + heightIndicatorTextSize.x, speedIndicatorVec.y + 15 ), IM_COL32_BLACK, "м" );

		ImGui::EndFrame(); 
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData() );
	}
}

void GForce::OnReset() {
	ImGui_ImplDX9_InvalidateDeviceObjects();
}

void GForce::CreateImGuiContext() {
	ImGui::CreateContext();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
	
	ImFontConfig ubuntuFontConfig;
	ubuntuFontConfig.GlyphExtraSpacing.x = 1.f;
	ImGui::GetIO().Fonts->AddFontFromMemoryCompressedBase85TTF( ubuntu_font_compressed_data_base85, 20.0f, &ubuntuFontConfig, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic() );

	static const ImWchar icons_ranges[] = { ICON_MIN,  ICON_MAX, 0 };
	ImFontConfig config_;
	config_.MergeMode = true;
	ImGui::GetIO().Fonts->AddFontFromMemoryCompressedBase85TTF( icons_compressed_data_base85, 19.0f, &config_, icons_ranges );
}

float GForce::GetAlpha() {
	return alpha;
}