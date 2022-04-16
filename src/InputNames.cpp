/////////////////////////////////////////////////////////////////////////////////
//
// Thor C++ Library
// Copyright (c) 2011-2022 Jan Haller
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////

#include <Thor/Input/InputNames.hpp>
#include <Thor/Graphics/ToString.hpp> // thor::StringConversionException

#include <Aurora/Meta/Templates.hpp>

#include <map>
#include <vector>


namespace thor
{
namespace
{
	// Bidirectional map: std::string <-> T
	template <typename T>
	class StringBimap
	{
		static_assert(!std::is_same<T, std::string>::value, "Type different from std::string expected");

		public:
			explicit StringBimap(std::size_t valueCount, std::size_t specialValueCount = 0)
			: mStringVector(valueCount + specialValueCount)
			, mTMap()
			, mSpecialValueCount(specialValueCount)
			{
			}

			void insert(T t, const std::string& string)
			{
				mStringVector[t + mSpecialValueCount] = string;
				mTMap[string] = t;
			}

			const std::string& at(T t) const
			{
				std::size_t index = static_cast<std::size_t>(t + mSpecialValueCount);
				if (index >= mStringVector.size() + mSpecialValueCount)
					throw StringConversionException("thor::toString() - No match for key / mouse button found");
				else
					return mStringVector[index];
			}

			T at(const std::string& string) const
			{
				auto itr = mTMap.find(string);
				if (itr == mTMap.end())
					throw StringConversionException(
						"thor::toKeyboardKey() / thor::toMouseButton() / thor::toJoystickAxis() - No match for string found");
				else
					return itr->second;
			}

		private:
			std::vector<std::string>	mStringVector;
			std::map<std::string, T>	mTMap;
			std::size_t					mSpecialValueCount;
	};

	StringBimap<sf::Keyboard::Key> initMap(aurora::Type<sf::Keyboard::Key>)
	{
		StringBimap<sf::Keyboard::Key> bimap(sf::Keyboard::KeyCount, 1);

		#define THOR_INSERT_MAPPING(identifier) bimap.insert(sf::Keyboard::identifier, #identifier)

		THOR_INSERT_MAPPING(Unknown);
		THOR_INSERT_MAPPING(A);
		THOR_INSERT_MAPPING(B);
		THOR_INSERT_MAPPING(C);
		THOR_INSERT_MAPPING(D);
		THOR_INSERT_MAPPING(E);
		THOR_INSERT_MAPPING(F);
		THOR_INSERT_MAPPING(G);
		THOR_INSERT_MAPPING(H);
		THOR_INSERT_MAPPING(I);
		THOR_INSERT_MAPPING(J);
		THOR_INSERT_MAPPING(K);
		THOR_INSERT_MAPPING(L);
		THOR_INSERT_MAPPING(M);
		THOR_INSERT_MAPPING(N);
		THOR_INSERT_MAPPING(O);
		THOR_INSERT_MAPPING(P);
		THOR_INSERT_MAPPING(Q);
		THOR_INSERT_MAPPING(R);
		THOR_INSERT_MAPPING(S);
		THOR_INSERT_MAPPING(T);
		THOR_INSERT_MAPPING(U);
		THOR_INSERT_MAPPING(V);
		THOR_INSERT_MAPPING(W);
		THOR_INSERT_MAPPING(X);
		THOR_INSERT_MAPPING(Y);
		THOR_INSERT_MAPPING(Z);
		THOR_INSERT_MAPPING(Num0);
		THOR_INSERT_MAPPING(Num1);
		THOR_INSERT_MAPPING(Num2);
		THOR_INSERT_MAPPING(Num3);
		THOR_INSERT_MAPPING(Num4);
		THOR_INSERT_MAPPING(Num5);
		THOR_INSERT_MAPPING(Num6);
		THOR_INSERT_MAPPING(Num7);
		THOR_INSERT_MAPPING(Num8);
		THOR_INSERT_MAPPING(Num9);
		THOR_INSERT_MAPPING(Escape);
		THOR_INSERT_MAPPING(LControl);
		THOR_INSERT_MAPPING(LShift);
		THOR_INSERT_MAPPING(LAlt);
		THOR_INSERT_MAPPING(LSystem);
		THOR_INSERT_MAPPING(RControl);
		THOR_INSERT_MAPPING(RShift);
		THOR_INSERT_MAPPING(RAlt);
		THOR_INSERT_MAPPING(RSystem);
		THOR_INSERT_MAPPING(Menu);
		THOR_INSERT_MAPPING(LBracket);
		THOR_INSERT_MAPPING(RBracket);
		THOR_INSERT_MAPPING(SemiColon);
		THOR_INSERT_MAPPING(Comma);
		THOR_INSERT_MAPPING(Period);
		THOR_INSERT_MAPPING(Quote);
		THOR_INSERT_MAPPING(Slash);
		THOR_INSERT_MAPPING(BackSlash);
		THOR_INSERT_MAPPING(Tilde);
		THOR_INSERT_MAPPING(Equal);
		THOR_INSERT_MAPPING(Dash);
		THOR_INSERT_MAPPING(Space);
		THOR_INSERT_MAPPING(Return);
		THOR_INSERT_MAPPING(BackSpace);
		THOR_INSERT_MAPPING(Tab);
		THOR_INSERT_MAPPING(PageUp);
		THOR_INSERT_MAPPING(PageDown);
		THOR_INSERT_MAPPING(End);
		THOR_INSERT_MAPPING(Home);
		THOR_INSERT_MAPPING(Insert);
		THOR_INSERT_MAPPING(Delete);
		THOR_INSERT_MAPPING(Add);
		THOR_INSERT_MAPPING(Subtract);
		THOR_INSERT_MAPPING(Multiply);
		THOR_INSERT_MAPPING(Divide);
		THOR_INSERT_MAPPING(Left);
		THOR_INSERT_MAPPING(Right);
		THOR_INSERT_MAPPING(Up);
		THOR_INSERT_MAPPING(Down);
		THOR_INSERT_MAPPING(Numpad0);
		THOR_INSERT_MAPPING(Numpad1);
		THOR_INSERT_MAPPING(Numpad2);
		THOR_INSERT_MAPPING(Numpad3);
		THOR_INSERT_MAPPING(Numpad4);
		THOR_INSERT_MAPPING(Numpad5);
		THOR_INSERT_MAPPING(Numpad6);
		THOR_INSERT_MAPPING(Numpad7);
		THOR_INSERT_MAPPING(Numpad8);
		THOR_INSERT_MAPPING(Numpad9);
		THOR_INSERT_MAPPING(F1);
		THOR_INSERT_MAPPING(F2);
		THOR_INSERT_MAPPING(F3);
		THOR_INSERT_MAPPING(F4);
		THOR_INSERT_MAPPING(F5);
		THOR_INSERT_MAPPING(F6);
		THOR_INSERT_MAPPING(F7);
		THOR_INSERT_MAPPING(F8);
		THOR_INSERT_MAPPING(F9);
		THOR_INSERT_MAPPING(F10);
		THOR_INSERT_MAPPING(F11);
		THOR_INSERT_MAPPING(F12);
		THOR_INSERT_MAPPING(F13);
		THOR_INSERT_MAPPING(F14);
		THOR_INSERT_MAPPING(F15);
		THOR_INSERT_MAPPING(Pause);

		static_assert(sf::Keyboard::KeyCount == 101, "Number of SFML keys has changed");
		#undef THOR_INSERT_MAPPING

		return bimap;
	}

	StringBimap<sf::Mouse::Button> initMap(aurora::Type<sf::Mouse::Button>)
	{
		StringBimap<sf::Mouse::Button> bimap(sf::Mouse::ButtonCount);

		#define THOR_INSERT_MAPPING(identifier) bimap.insert(sf::Mouse::identifier, #identifier)

		THOR_INSERT_MAPPING(Left);
		THOR_INSERT_MAPPING(Right);
		THOR_INSERT_MAPPING(Middle);
		THOR_INSERT_MAPPING(XButton1);
		THOR_INSERT_MAPPING(XButton2);

		static_assert(sf::Mouse::ButtonCount == 5, "Number of SFML mouse buttons has changed");
		#undef THOR_INSERT_MAPPING

		return bimap;
	}

	StringBimap<sf::Joystick::Axis> initMap(aurora::Type<sf::Joystick::Axis>)
	{
		StringBimap<sf::Joystick::Axis> bimap(sf::Joystick::AxisCount);

		#define THOR_INSERT_MAPPING(identifier) bimap.insert(sf::Joystick::identifier, #identifier)

		THOR_INSERT_MAPPING(X);
		THOR_INSERT_MAPPING(Y);
		THOR_INSERT_MAPPING(Z);
		THOR_INSERT_MAPPING(R);
		THOR_INSERT_MAPPING(U);
		THOR_INSERT_MAPPING(V);
		THOR_INSERT_MAPPING(PovX);
		THOR_INSERT_MAPPING(PovY);

		static_assert(sf::Joystick::AxisCount == 8, "Number of SFML joystick axes has changed");
		#undef THOR_INSERT_MAPPING

		return bimap;
	}

	template <typename T>
	StringBimap<T>& mapInstance()
	{
		static StringBimap<T> instance = initMap(aurora::Type<T>());
		return instance;
	}

} // namespace

// ---------------------------------------------------------------------------------------------------------------------------


std::string toString(sf::Keyboard::Key key)
{
	return mapInstance<sf::Keyboard::Key>().at(key);
}

std::string toString(sf::Mouse::Button button)
{
	return mapInstance<sf::Mouse::Button>().at(button);
}

std::string toString(sf::Joystick::Axis axis)
{
	return mapInstance<sf::Joystick::Axis>().at(axis);
}

sf::Keyboard::Key toKeyboardKey(const std::string& string)
{
	return mapInstance<sf::Keyboard::Key>().at(string);
}

sf::Mouse::Button toMouseButton(const std::string& string)
{
	return mapInstance<sf::Mouse::Button>().at(string);
}

sf::Joystick::Axis toJoystickAxis(const std::string& string)
{
	return mapInstance<sf::Joystick::Axis>().at(string);
}

} // namespace thor
