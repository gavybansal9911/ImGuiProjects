#include "Window.h"
#include "Window.h"

#include "imgui.h"

#include <cstdlib>
#include <iostream>

void Window::Draw(std::string label)
{
    {
        constexpr static auto windowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
        constexpr static auto windowSize = ImVec2(1280.f, 720.f);
        constexpr static auto windowPos = ImVec2(0.f, 0.f);

        ImGui::SetNextWindowSize(windowSize);
        ImGui::SetNextWindowPos(windowPos);

        ImGui::Begin("File Explorer", nullptr, windowFlags);

        DrawMenu();
        ImGui::Separator();
        DrawContent();
        ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 100.f);
        ImGui::Separator();
        DrawActions();
        ImGui::Separator();
        DrawFilter();

        ImGui::End();
    }
}

void Window::DrawMenu()
{
    if (ImGui::Button("Go up"))
    {
        if (m_CurrentPath.has_parent_path())
            m_CurrentPath = m_CurrentPath.parent_path();
    }
    ImGui::SameLine();

    ImGui::Text("Current directory: %ls", m_CurrentPath.c_str());
}

void Window::DrawContent()
{
    for (const auto& entry : fs::directory_iterator(m_CurrentPath))
    {
        const bool is_Selected = entry.path() == m_SelectedEntry;
        const bool is_Directory = entry.is_directory();
        const bool is_File = entry.is_regular_file();
        std::string entryName = entry.path().filename().string();

        if (is_Directory)
            entryName = "[D]" + entryName;
        if (is_File)
            entryName = "[F]" + entryName;

        if (ImGui::Selectable(entryName.c_str(), is_Selected))
        {
	        if (is_Directory && m_SelectedEntry == entry.path())
                m_CurrentPath /= entry.path().filename();

        	m_SelectedEntry = entry.path();
        }
    }
}

void Window::DrawActions()
{
    if (fs::is_directory(m_SelectedEntry))
        ImGui::Text("Selected dir: %s", m_SelectedEntry.filename().string().c_str());
    else if (fs::is_regular_file(m_SelectedEntry))
        ImGui::Text("Selected file: %s", m_SelectedEntry.filename().string().c_str());
    else ImGui::Text("Nothing selected!");

    if (fs::is_regular_file(m_SelectedEntry) && ImGui::Button("Open"))
        OpenFileWithDefaultEditor();

    ImGui::SameLine();

    if (ImGui::Button("Rename"))
    {
        m_RenameDialog_Opened = true;
        ImGui::OpenPopup("Rename File");
    }

    ImGui::SameLine();

    if (ImGui::Button("Delete"))
    {
        m_DeleteDialog_Opened = true;
        ImGui::OpenPopup("Delete File");
    }
 
    ImGui::SameLine();

    RenameFilePopup();
    DeleteFilePopup();
}

void Window::DrawFilter()
{
    ImGui::Text("Filter by extension");
    ImGui::SameLine();

    static char extension_Filter[16] = {"\0"};
    ImGui::InputText("###inFilter", extension_Filter, sizeof(extension_Filter));

    if (std::strlen(extension_Filter) == 0)
	    return;

    auto filteredFileCount = std::size_t{0};
    for (const auto& entry : fs::directory_iterator(m_CurrentPath))
    {
	    if (!fs::is_regular_file(entry))
            continue;

        if (extension_Filter == entry.path().extension().string())
        {
            filteredFileCount++;
        }
    }
	ImGui::Text("Number of files: %u", filteredFileCount);
}

void Window::OpenFileWithDefaultEditor()
{
#ifdef _WIN32
    const auto command = "start \"\" \"" + m_SelectedEntry.string() + "\"";
#elif 
    const auto command = "open \"" + m_SelectedEntry.string() + "\"";
#else
    const auto command = "xdg-open \"" + m_SelectedEntry.string() + "\"";
#endif

    std::system(command.c_str());
}

void Window::RenameFilePopup()
{
    if (ImGui::BeginPopupModal("Rename File", &m_RenameDialog_Opened))
    {
        static char buffer_Name[512] = { "\0" };
        
        ImGui::Text("New name: ");
        ImGui::InputText("###newName", buffer_Name, sizeof(buffer_Name));

        if (ImGui::Button("Rename"))
        {
            auto new_Path = m_SelectedEntry.parent_path() / buffer_Name;
            if (RenameFile(m_SelectedEntry, new_Path))
            {
                m_RenameDialog_Opened = false;
                m_SelectedEntry = new_Path;
                std::memset(buffer_Name, 0, sizeof(buffer_Name));
            }
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
            m_RenameDialog_Opened = false;

        ImGui::EndPopup();
    }
}

void Window::DeleteFilePopup()
{
    if (ImGui::BeginPopupModal("Delete File", &m_DeleteDialog_Opened))
    {
        ImGui::Text("Are you sure you want to delete %s?", m_SelectedEntry.filename().string().c_str());

        if (ImGui::Button("Yes"))
        {
            if (DeleteFile(m_SelectedEntry))
            {
                m_SelectedEntry.clear();
                m_DeleteDialog_Opened = false;
            }
        }

        ImGui::SameLine();

        if (ImGui::Button("No"))
        {
            m_DeleteDialog_Opened = false;
        }

        ImGui::EndPopup();
    }
}

bool Window::RenameFile(const fs::path& old_Path, const fs::path& new_Path)
{
    try
    {
        fs::rename(old_Path, new_Path);
        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
}

bool Window::DeleteFile(const fs::path& path)
{
    try
    {
        fs::remove(path);
        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
}
