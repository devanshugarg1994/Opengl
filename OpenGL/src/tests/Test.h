#pragma once

#include<vector>
#include <functional>
#include <string>

namespace test {

	class Test
	{
	public:
		Test() {};
		virtual ~Test() {};

		virtual void onUpdate(float deltaTIme) {};
		virtual void onRender() {};
		virtual void onImGuiRender() {};

	private:

	};


	class TestMenu : public Test {
	public:
		TestMenu(Test *&currentTest);

		void onImGuiRender() override;

		template <typename T>
		void registerTest(const std::string& name) {
			m_Test.push_back(std::make_pair(name, []() {return new T(); }));
		}
	private:
		Test*& m_currentTest;
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_Test;
	};

}