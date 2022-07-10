#include "Test.h"
#include "imgui/imgui.h"

namespace test {
	TestMenu::TestMenu(Test*& currentTest)
		: m_currentTest(currentTest)
	{
	}

	void TestMenu::onImGuiRender()
	{
		for (auto& test : m_Test) {
			if (ImGui::Button(test.first.c_str())) {
				m_currentTest = test.second();
			}
		}
	}
}