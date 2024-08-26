#include "Window.h"

#include "imgui.h"

#include <cstdlib>
#include <iostream>
#include <algorithm>

#include <set>
#include <cmath>
#include <string_view>
#include <array>
#include <format>

#include <implot.h>

void Window::Draw(std::string label)
{
    {
        constexpr static auto windowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
        constexpr static auto windowSize = ImVec2(1280.f, 720.f);
        constexpr static auto windowPos = ImVec2(0.f, 0.f);

        ImGui::SetNextWindowSize(windowSize);
        ImGui::SetNextWindowPos(windowPos);

        ImGui::Begin("Function Plotter", nullptr, windowFlags);

        drawSelection();
        drawPlot();

        ImGui::End();
    }
}

void Window::drawSelection()
{
    for (const auto funcName : functionNames)
    {
        const auto currFunction = functionNameMapping(funcName);
        //bool selected = selectedFunctions.find(currFunction) != selectedFunctions.end();
        bool selected = selectedFunctions.contains(currFunction);

        if (ImGui::Checkbox(funcName.data(), &selected))
        {
            if (selected)
                selectedFunctions.insert(currFunction);
            else selectedFunctions.erase(currFunction);
        }
    }
}

void Window::drawPlot()
{
    static constexpr auto numPoints = 10'000;
    static constexpr auto x_Min = -100.f;
    static constexpr auto x_Max = 100.f;
    static const auto x_Step = (std::abs(x_Max) + std::abs(x_Min)) / numPoints;

    static auto xs = std::array<double, numPoints>{};
    static auto ys = std::array<double, numPoints>{};

    if (selectedFunctions.size() == 0 ||
        selectedFunctions.size() == 1 && *selectedFunctions.begin() == Function::NONE)
    {
        ImPlot::CreateContext();
        ImPlot::BeginPlot("###plot", ImVec2(-1.f, -1.f), ImPlotFlags_NoTitle);
        ImPlot::EndPlot();
        return;
    }
    else {
        ImPlot::BeginPlot("###plot", ImVec2(-1.f, -1.f), ImPlotFlags_NoTitle);

        for (const auto function : selectedFunctions)
        {
            auto x = x_Min;
            for (std::size_t i = 0; i < numPoints; i++)
            {
                xs[i] = x;
                ys[i] = evaluateFunction(function, x);
                x += x_Step;
            }

            const auto plotLabel = "function" + static_cast<int>(function);
            ImPlot::PlotLine(plotLabel, xs.data(), ys.data(), numPoints);
        }

        ImPlot::EndPlot();
    }
}

Window::Function Window::functionNameMapping(std::string_view funtionName)
{
    if (funtionName == std::string_view{ "sin(x)" })
        return Window::Function::SIN;
    else if (funtionName == std::string_view{ "cos(x)" })
        return Window::Function::COS;
    else return Window::Function::NONE;
}

double Window::evaluateFunction(const Function function, const double x)
{
    switch (function)
    {
    case Function::SIN:
        return std::sin(x);
    case Function::COS:
        return std::cos(x);
    default:
        break;
    }
    return 0.f;
}
