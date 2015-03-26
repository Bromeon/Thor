/////////////////////////////////////////////////////////////////////////////////
//
// Thor C++ Library
// Copyright (c) 2011-2015 Jan Haller
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
/// @brief Classes thor::ResourceLoadingException, thor::ResourceAccessException

#ifndef THOR_RESOURCEEXCEPTIONS_HPP
#define THOR_RESOURCEEXCEPTIONS_HPP

#include <Aurora/Tools/Exceptions.hpp>


namespace thor
{

/// @addtogroup Resources
/// @{

/// @brief %Exception class for failed resource allocation.
/// @details Is thrown when an error occurs while loading a resource.
class ResourceLoadingException : public aurora::Exception
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Constructor
		/// @param message The exception message (how the error occurred).
		explicit ResourceLoadingException(const std::string& message)
		: aurora::Exception(message)
		{
		}
};

/// @brief %Exception class for invalid resource access.
/// @details Is thrown when the user tries to access an invalid resource. Usually, this means that a requested resource has been
///  already released or not yet loaded.
class ResourceAccessException : public aurora::Exception
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Constructor
		/// @param message The exception message (how the error occurred).
		explicit ResourceAccessException(const std::string& message)
		: aurora::Exception(message)
		{
		}
};

/// @}

} // namespace thor

#endif // THOR_RESOURCEEXCEPTIONS_HPP
