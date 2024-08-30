#pragma once

#include <string>

#include <imgui.h>
#include <filesystem>

namespace fs = std::filesystem;

class Window
{
public:
	Window() : currentFilename({}) {
		std::memset(textBuffer, 0, bufferSize);
	}

	void Draw(std::string label);

	static constexpr auto bufferSize = std::size_t{ 1024 };
	static constexpr auto popupFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar;
	static constexpr auto popupSize = ImVec2(300.f, 100.f);
	static constexpr auto popupButtonSize = ImVec2(120.f, 0.f);

private:
	void DrawMenu();
	void DrawSavePopup();
	void DrawLoadPopup();
	void DrawContent();
	void DrawInfo();

	void SaveToFile(std::string filename);
	void LoadFromFile(std::string filename);
	std::string GetFileExtension(std::string filename);

private:
	char textBuffer[bufferSize];
	std::string currentFilename;
};
