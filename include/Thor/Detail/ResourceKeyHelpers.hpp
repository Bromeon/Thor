/////////////////////////////////////////////////////////////////////////////////
//
// Thor C++ Library
// Copyright (c) 2011-2012 Jan Haller
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

#ifndef THOR_RESOURCEKEYHELPERS_HPP
#define THOR_RESOURCEKEYHELPERS_HPP

#include <Thor/Multimedia/ToString.hpp>
#include <Thor/Config.hpp>

#include AURORA_TR1_HEADER(functional)
#include <sstream>
#include <cstring>

// Using and namespace declarations
// (this is okay here because this header is only included inside internal .cpp files)
using namespace std::tr1::placeholders;
namespace tr1 = std::tr1;


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

		operator std::string() const
		{
			// Remove last separator
			std::string s = stream.str();
			if (!s.empty() && s.rfind("; ") == s.size() - 2)
				s.erase(s.size() - 2);
			return s;
		}

		std::ostringstream stream;
		bool begin;
	};


	// Forbids copies
	struct NoCopy
	{
		template <typename T>
		T* operator() (const T*) const
		{
			throw std::logic_error("No copy allowed");
		}
	};

	// Helper function to load resource
	template <class Resource>
	aur::CopiedPtr<Resource> LoadResource(const tr1::function<bool(Resource&)>& loader)
	{
		aur::CopiedPtr<Resource> resource(new Resource, NoCopy(), aur::OperatorDelete<Resource>());

		if (loader(*resource))
			return resource;
		else
			return aur::CopiedPtr<Resource>();
	}

} // namespace detail
} // namespace thor

#endif // THOR_RESOURCEKEYHELPERS_HPP
