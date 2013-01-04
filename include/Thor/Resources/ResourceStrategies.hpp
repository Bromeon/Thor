/////////////////////////////////////////////////////////////////////////////////
//
// Thor C++ Library
// Copyright (c) 2011-2013 Jan Haller
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
/// @brief Resource management strategies (enumerations)

#ifndef THOR_RESOURCESTRATEGIES_HPP
#define THOR_RESOURCESTRATEGIES_HPP


namespace thor
{
namespace Resources
{

/// @brief Specifies the behavior when resources can't be loaded.
/// @details This strategy determines what happens when a call to ResourceCache::acquire() fails (e.g. because
///  of a wrong filename).
/// @see ResourceCache::setLoadingFailureStrategy()
enum LoadingFailureStrategy
{
	ThrowException,		///< Throws an exception of type ResourceLoadingException.
	ReturnNullPointer,	///< Returns an empty std::shared_ptr.
};


/// @brief Specifies the release behavior of unused resources.
/// @details A resource is unused when no more std::shared_ptr references it. With this type, you can determine
///  if the resource cache should try to cache the resources or to keep the allocation as low as possible.
/// @see ResourceCache::setReleaseStrategy()
enum ReleaseStrategy
{
	AutoRelease,		///< %Resources are immediately released as soon as they are unused.
	ExplicitRelease,	///< Unused resources are kept in memory until they are explicitly released or the ResourceCache is destroyed.
};

} // namespace Resources
} // namespace thor

#endif // THOR_RESOURCESTRATEGIES_HPP
