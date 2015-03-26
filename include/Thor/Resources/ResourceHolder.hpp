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
/// @brief Class template thor::ResourceHolder

#ifndef THOR_RESOURCEHOLDER_HPP
#define THOR_RESOURCEHOLDER_HPP

#include <Thor/Resources/KnownIdStrategy.hpp>
#include <Thor/Resources/OwnershipModels.hpp>
#include <Thor/Resources/ResourceExceptions.hpp>
#include <Thor/Resources/ResourceLoader.hpp>

#include <Aurora/Tools/NonCopyable.hpp>

#include <memory>
#include <map>


namespace thor
{

/// @addtogroup Resources
/// @{

/// @brief Class that is responsible of the management of resources like images, fonts or music.
/// @details A resource, sometimes called asset, is a heavyweight object that is used from multiple places in your application. This
///  class acts as a central access point for resources. ResourceHolder provides methods to load (acquire), release and access resources.
///  Every resource is associated with an ID, a user-defined identifier, which acts as a name or address referring to the resource.
/// @tparam R The resource type to manage (for example sf::Texture). @a R must not be const-qualified.
///  @a R isn't required to be default-constructible or copy-constructible.
/// @tparam I The type identifying the resource. IDs are usually enums or strings. If you don't want to choose IDs yourself,
///   let ResourceLoader::getInfo() generate them automatically -- in this case @a I would be std::string.
/// @tparam O Ownership model. Determines who owns the resources and is responsible of their lifetime. Possible types:
///  Resources::CentralOwner (default), Resources::RefCounted.
template <typename R, typename I, class O = Resources::CentralOwner>
class ResourceHolder : private aurora::NonCopyable
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Private types
	private:
		// Abbreviate class containing ownership policy types and functions
		typedef typename detail::OwnershipModel<O, R>	Om;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Public types
	public:
		/// @brief Resource handle for mutable access
		/// @details The type depends on the ownership policy:
		///  @li @ref thor::Resources::CentralOwner "CentralOwner": <b>R&</b>
		///  @li @ref thor::Resources::RefCounted "RefCounted": <b>shared_ptr<R></b>
		/// @hideinitializer
		typedef typename Om::Returned			Resource;

		/// @brief Resource handle for constant access
		/// @details The type depends on the ownership policy:
		///  @li @ref thor::Resources::CentralOwner "CentralOwner": <b>const R&</b>
		///  @li @ref thor::Resources::RefCounted "RefCounted": <b>shared_ptr<const R></b>
		/// @hideinitializer
		typedef typename Om::ConstReturned		ConstResource;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Default constructor
		///
									ResourceHolder();

		/// @brief Move constructor
		///
									ResourceHolder(ResourceHolder&& source);

		/// @brief Move assignment operator
		///
		ResourceHolder&				operator= (ResourceHolder&& source);

		/// @brief Loads a new resource, identified as @a id.
		/// @param id Value identifying the resource.
		/// @param how Resource loader containing loading information. Determines how the resource is loaded.
		/// @param known Determines what happens if @a id is already known. By default, an exception is thrown.
		/// @return Resource associated with @a id; either just loaded or already stored in the holder.
		/// @throw ResourceLoadingException if the loading of the resource fails.
		/// @throw ResourceAccessException if a resource associated with @a id is already known and @a known is AssumeNew.
		Resource					acquire(const I& id, const ResourceLoader<R>& how, Resources::KnownIdStrategy known = Resources::AssumeNew);

		/// @brief Unloads the resource currently identified as @a id.
		/// @details The resource is removed from this resource holder. Depending on the ownership policy,
		///  it may either be released immediately or live until it is no longer referenced. In any case,
		///  you cannot access it any longer through this instance.
		/// @param id Value identifying the resource.
		/// @throw ResourceAccessException If @a id doesn't refer to a currently stored resource.
		void						release(const I& id);

		/// @brief Accesses a resource using the identifier @a id.
		/// @details Requires that a resource with the identifier @a id is currently stored in this holder.
		/// @param id Value identifying the resource.
		/// @return Handle to that resource.
		/// @throw ResourceAccessException If @a id doesn't refer to a currently stored resource.
		Resource					operator[] (const I& id);

		/// @brief Accesses a resource using the identifier @a id (const overload).
		/// @details Requires that a resource with the identifier @a id is currently stored in this holder.
		/// @param id Value identifying the resource.
		/// @return Handle to that resource, which does not allow modification of the resource.
		/// @throw ResourceAccessException If @a id doesn't refer to a currently stored resource.
		ConstResource				operator[] (const I& id) const;

	
	// ---------------------------------------------------------------------------------------------------------------------------
	// Private member functions
	private:
		// Load resource (must be new)
		Resource					load(const I& id, const ResourceLoader<R>& how);
	

	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		std::map<I, typename Om::Stored>	mMap;
};

/// @}

} // namespace thor

#include <Thor/Resources/Detail/ResourceHolder.inl>
#endif // THOR_RESOURCEHOLDER_HPP
