#include "Window.h"

#include "imgui.h"

#include <cstdlib>
#include <iostream>

#include <algorithm>
#include <cstring>
#include <sstream>

#include <fstream>

void Window::Draw(std::string label)
{
    {
        constexpr static auto windowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
        constexpr static auto windowSize = ImVec2(1280.f, 720.f);
        constexpr static auto windowPos = ImVec2(0.f, 0.f);

        ImGui::SetNextWindowSize(windowSize);
        ImGui::SetNextWindowPos(windowPos);

        ImGui::Begin("Text Editor", nullptr, windowFlags);

        DrawMenu();
        DrawContent();
        DrawInfo();

        ImGui::End();
    }
}

void Window::DrawMenu()
{
    const auto ctrlPressed = ImGui::GetIO().KeyCtrl;
    const auto escPressed = ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape));
    const auto sPressed = ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_S));
    const auto lPressed = ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_L));

    if (ImGui::Button("Save") || (ctrlPressed && sPressed))
    {
        ImGui::OpenPopup("Save File");
    }

    ImGui::SameLine();

    if (ImGui::Button("Load") || (ctrlPressed && lPressed))
    {
        ImGui::OpenPopup("Load File");
    }

    ImGui::SameLine();

    if (ImGui::Button("Clear"))
    {
        std::memset(textBuffer, 0, bufferSize);
    }

    DrawSavePopup();
    DrawLoadPopup();
}

void Window::DrawSavePopup()
{
    const auto escPressed = ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape));
    static char saveFilenameBuffer[256] = "text.txt";

    ImGui::SetNextWindowSize(popupSize);
    ImGui::SetNextWindowPos(ImVec2(
        ImGui::GetIO().DisplaySize.x / 2.f - popupSize.x / 2.f,
        ImGui::GetIO().DisplaySize.y / 2.f - popupSize.y / 2.f));

    if (ImGui::BeginPopupModal("Save File", nullptr, popupFlags))
    {
        ImGui::InputText("Filename", saveFilenameBuffer, sizeof(saveFilenameBuffer));

        if (ImGui::Button("Save", popupButtonSize))
        {
            SaveToFile(saveFilenameBuffer);
            currentFilename = saveFilenameBuffer;
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel", popupButtonSize) || escPressed)
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void Window::DrawLoadPopup()
{
    const auto escPressed = ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape));
    static char saveFilenameBuffer[256] = "text.txt";

    ImGui::SetNextWindowSize(popupSize);
    ImGui::SetNextWindowPos(ImVec2(
        ImGui::GetIO().DisplaySize.x / 2.f - popupSize.x / 2.f,
        ImGui::GetIO().DisplaySize.y / 2.f - popupSize.y / 2.f));

    if (ImGui::BeginPopupModal("Load File"))
    {
        ImGui::InputText("Filename", saveFilenameBuffer, sizeof(saveFilenameBuffer));

        if (ImGui::Button("Load", popupButtonSize))
        {
            LoadFromFile(saveFilenameBuffer);
            currentFilename = saveFilenameBuffer;
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel", popupButtonSize) || escPressed)
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void Window::DrawContent()
{
    static constexpr auto inputTextSize = ImVec2(1200.f, 625.f);
    static constexpr auto inputTextFlags = ImGuiInputTextFlags_AllowTabInput |
        ImGuiInputTextFlags_NoHorizontalScroll;
    static constexpr auto lineNumberSize = ImVec2(30.f, inputTextSize.y);

    ImGui::BeginChild("LineNumbers", lineNumberSize);

    const auto lineCount = std::count(textBuffer, textBuffer + bufferSize, '\n') + 1;
    for (size_t i = 1; i <= lineCount; i++)
    {
        ImGui::Text("%d", i);
    }

    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::InputTextMultiline("###inputField", textBuffer, bufferSize, inputTextSize, inputTextFlags);
}

void Window::DrawInfo()
{
    if (currentFilename.size() == 0)
    {
        ImGui::Text("No File Opened!");
        return;
    }

    const auto fileExtension = GetFileExtension(currentFilename);
    ImGui::Text("Opened file %s | File extension %s", currentFilename, fileExtension);
}

void Window::SaveToFile(std::string filename)
{
    auto out = std::ofstream{ filename };

    if (out.is_open())
    {
        out << textBuffer;
        out.close();
    }
}

void Window::LoadFromFile(std::string filename)
{
    auto in = std::ifstream{ filename };

    if (in.is_open())
    {
        auto buffer = std::stringstream{};
        buffer << in.rdbuf();
        std::memcpy(textBuffer, buffer.str().data(), bufferSize);
        in.close();
    }
}

std::string Window::GetFileExtension(std::string filename)
{
    const auto filePath = fs::path{filename};
    return filePath.extension().string();
}
