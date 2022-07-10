#pragma once

#include "Test.h"
#include <GL/glew.h>
#include "Renderer.h"
#include "imgui/imgui.h"

namespace test {
	class TestClearColor : public Test {
	public: 
		TestClearColor();
		~TestClearColor();

		void onUpdate(float deltaTime) override;
		void onRender() override;
		void onImGuiRender() override;

	private:
		float m_ClearColor[4];

	};
}