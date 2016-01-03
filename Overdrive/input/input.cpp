#include "stdafx.h"
#include "input.h"
#include <iostream>

namespace overdrive {
	namespace input {
		Input::Input():
			System("Input")
		{
			addDependency("Video");
		}

		void Input::initialize() {
			System::initialize();
			
			detail::loadXInput();

			for (unsigned int i = 0; i < Gamepad::getMaxGamepads(); ++i)
				mGamepadList.push_back(Gamepad(i));
		}

		void Input::update() {
			for (auto& gamepad: mGamepadList)
				gamepad.update();
		}

		void Input::shutdown() {
			System::shutdown();
			
			detail::unloadXInput();
		}

		const std::vector<Keyboard*>& Input::getKeyboardList() const {
			return mKeyboardList;
		}

		const std::vector<Mouse*>& Input::getMouseList() const {
			return mMouseList;
		}

		const std::vector<Gamepad>& Input::getGamepadList() const {
			return mGamepadList;
		}

		void Input::operator()(const video::Window::OnCreated& created) {
			mKeyboardList.push_back(created.mWindow->getKeyboard());
			mMouseList.push_back(created.mWindow->getMouse());
		}

		void Input::operator()(const video::Window::OnClosed& closed) {
			auto keyboard = closed.mWindow->getKeyboard();
			auto mouse = closed.mWindow->getMouse();

			{
				auto it = std::find(mKeyboardList.begin(), mKeyboardList.end(), keyboard);
				assert(it != mKeyboardList.end());
				mKeyboardList.erase(it);
			}

			{
				auto it = std::find(mMouseList.begin(), mMouseList.end(), mouse);
				assert(it != mMouseList.end());
				mMouseList.erase(it);
			}
		}
	}
}