#pragma once

#include <filesystem>

namespace fs = std::filesystem;

class Window
{
public:
	Window() : m_CurrentPath(fs::current_path()), m_SelectedEntry(fs::path()) {}

	void Draw(std::string label);

private:
	void DrawMenu();
	void DrawContent();
	void DrawActions();
	void DrawFilter();

	void OpenFileWithDefaultEditor();
	
	void RenameFilePopup();
	void DeleteFilePopup();

	bool RenameFile(const fs::path& old_Path, const fs::path& new_Path);
	bool DeleteFile(const fs::path& path);

private:
	fs::path m_CurrentPath;
	fs::path m_SelectedEntry;

	bool m_RenameDialog_Opened = false;
	bool m_DeleteDialog_Opened = false;
};
