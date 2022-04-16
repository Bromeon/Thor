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

#ifndef THOR_RESOURCELOADERHELPERS_HPP
#define THOR_RESOURCELOADERHELPERS_HPP

#include <Thor/Resources/ResourceLoader.hpp>
#include <Thor/Graphics/ToString.hpp>
#include <Thor/Config.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <functional>
#include <memory>
#include <sstream>
#include <cstring>
#include <type_traits>


namespace sf
{

	class InputStream;
	class Image;

} // namespace sf


namespace thor
{
namespace detail
{

	// Helper class to build unique string tags from several properties
	struct Tagger
	{
		Tagger(const char* text = "")
		: stream()
		, begin(true)
		{
			if (std::strlen(text) != 0)
				stream << "[From" << text << "] ";
		}

		// Enable If to exclude sf::InputStream derivates
		template <typename T>
		Tagger& operator<< (const T& value)
		{
			// Insert separator before every value except the first
			if (!begin)
				stream << "; ";
			begin = false;

			// Push actual value
			stream << value;
			return *this;
		}

		Tagger& operator<< (const sf::Color& color)
		{
			return operator<< (toString(color));
		}

		Tagger& operator<< (const sf::IntRect& rect)
		{
			return operator<< (toString(rect));
		}

		operator std::string() const
		{
			// Remove last separator
			std::string s = stream.str();
			if (!s.empty() && s.rfind("; ") == s.size() - 2)
				s.erase(s.size() - 2);
			return s;
		}

		std::ostringstream	stream;
		bool				begin;
	};

	// Creates a resource loader by transforming the function
	//      bool               boolLoader(R&)
	// to:  std::unique_ptr<R> loader()
	template <class R, typename Fn>
	ResourceLoader<R> makeResourceLoader(Fn boolLoader, std::string key)
	{
		auto loader = [=] () -> std::unique_ptr<R>
		{
			std::unique_ptr<R> resource(new R());
			if (boolLoader(*resource))
				return resource;
			else
				return nullptr;
		};

		return ResourceLoader<R>(loader, key);
	}

} // namespace detail
} // namespace thor

#endif // THOR_RESOURCELOADERHELPERS_HPP
