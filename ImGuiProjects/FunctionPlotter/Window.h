#pragma once

#include <array>
#include <string_view>
#include <set>

class Window
{
public:
	Window() : selectedFunctions({}) {}

	void Draw(std::string label);

public:
	constexpr static auto functionNames = std::array<std::string_view, 3>{"unk", "sin(x)", "cos(x)"};

	enum class Function
	{
		NONE, SIN, COS
	};

private:
	void drawSelection();
	void drawPlot();

	Function functionNameMapping(std::string_view funtionName);
	double evaluateFunction(const Function function, const double x);

public:
	std::set<Function> selectedFunctions;
};
