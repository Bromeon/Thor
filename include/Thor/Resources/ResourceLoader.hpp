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

/// @file
/// @brief Class template thor::ResourceLoader

#ifndef THOR_RESOURCELOADER_HPP
#define THOR_RESOURCELOADER_HPP

#include <Thor/Config.hpp>

#include <functional>
#include <memory>
#include <string>


namespace thor
{

/// @addtogroup Resources
/// @{

/// @brief Class storing loading information for resources
///
template <class R>
class ResourceLoader
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public types
	public:
		/// @brief Function type to load a resource.
		///
		typedef std::function< std::unique_ptr<R>() > Loader;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Constructor
		/// @param loader Function returning a unique_ptr<R>. Shall return a unique pointer to the resource if it has been loaded,
		///  and nullptr in case of loading failure. The function shall not throw any exceptions.
		/// @param id Identifier which is equal to another identifier if and only if the key refers to the same resource. Can also
		///  contain debug information in case of loading failures.
									ResourceLoader(std::function< std::unique_ptr<R>() > loader, std::string id)
		: mLoader(std::move(loader))
		, mId(std::move(id))
		{
		}

		/// @brief Loads a resource.
		/// @return Unique pointer to resource if loaded, nullptr in case of loading failure.
		std::unique_ptr<R>			load() const
		{
			return mLoader();
		}

		/// @brief Returns a string describing the resource loader.
		///
		std::string					getInfo() const
		{
			return mId;
		}


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		Loader						mLoader;
		std::string					mId;
};

/// @}

} // namespace thor

#endif // THOR_RESOURCELOADER_HPP
