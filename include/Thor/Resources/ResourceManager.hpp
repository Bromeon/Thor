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

/// @file
/// @brief Class template thor::ResourceManager

#ifndef THOR_RESOURCEMANAGER_HPP
#define THOR_RESOURCEMANAGER_HPP

#include <Thor/Resources/ResourcePtr.hpp>
#include <Thor/Resources/ResourceStrategies.hpp>
#include <Thor/Resources/ResourceKeyTraits.hpp>
#include <Thor/Resources/ResourceLoadingException.hpp>
#include <Thor/Detail/ResourceSlot.hpp>
#include <Thor/Config.hpp>

#include <Aurora/Tools/NonCopyable.hpp>
#include <Aurora/Tools/ForEach.hpp>
#include <Aurora/SmartPtr/CopiedPtr.hpp>

#include AURORA_TR1_HEADER(functional)
#include AURORA_TR1_HEADER(type_traits)
#include <map>


namespace thor
{

/// @addtogroup Resources
/// @{

/// @brief Class that is responsible for the management of resources like images, fonts or music.
/// @details You can acquire and release resources. Access is granted through the key (an ID class).
/// @tparam Resource The resource type to manage (for example sf::Image). Const-correctness is properly forwarded: If
///  @a Resource is const-qualified, you cannot modify the resources after initialization (via ResourcePtr).
///  @n @a Resource isn't required to be default-constructible or copy-constructible.
/// @tparam ResourceKey The key class that is used to distinguish between resources (for example thor::Resources::ImageKey).
///  By default, a library implementation supporting the SFML resource classes is used. If you want to specify your own
///  key classes, they have to fulfill the following requirements:
///   @arg <b>Value semantics.</b> Your key class must be copyable and assignable.
///   @arg <b>thor::MovedPtr<Resource, thor::NoCopy> ResourceKey::load() const</b> loads the resource and returns a smart pointer to it
///    (pointing to memory allocated with new). In case of loading failure, a null pointer shall be returned.
///   @arg <b>bool operator< (const ResourceKey&, const ResourceKey&)</b> which supports strict weak ordering.
template <class Resource, class ResourceKey = typename Resources::KeyTraits<Resource>::Type >
class ResourceManager : private aur::NonCopyable
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Default constructor
		/// @details Initially, the object owns no allocated resources. By default, the loading failure strategy
		///  Resources::ThrowException and the release strategy Resources::ExplicitRelease are used.
									ResourceManager();
			
		/// @brief Searches for an occurrence of @a key and returns the mapped resource, if possible.
		/// @details This function does never load a new resource.
		/// @param key The resource identifier to search for.
		/// @return The corresponding ResourcePtr, if found, or @a NULL otherwise.
		ResourcePtr<Resource>		search(const ResourceKey& key);
		
		/// @brief Searches for an occurrence of @a key and returns the mapped resource, if possible (const overload).
		/// @details This function does never load a new resource. 
		/// @param key The resource identifier to search for.
		/// @return The corresponding read-only ResourcePtr, if found, or @a NULL otherwise.
		ResourcePtr<const Resource>	search(const ResourceKey& key) const;

		/// @brief Returns the resource mapped to @a key, loading the resource if required.
		/// @details If the key is already stored, the corresponding resource is returned. Otherwise, the resource is loaded. The
		///  release time of the resource is determined by the release strategy.
		/// @param key The resource identifier to search for, which also contains loading information.
		/// @return ResourcePtr that allows access to the resource. If the resource allocation fails and the @a ReturnNullPointer
		///  strategy is active, a null pointer is returned.
		/// @throw ResourceLoadingException if the loading of the resource fails (and the @a ThrowException strategy is active).
		/// @see setLoadingFailureStrategy(), SetReleaseStrategy()
		ResourcePtr<Resource>		acquire(const ResourceKey& key);

		/// @brief Releases the resource as soon as possible.
		/// @details If the resource is not in use (i.e. no ResourcePtr references it), it is immediately released.
		///  Otherwise, it is released as soon as the last ResourcePtr loses ownership. Calling this function
		///  has no effect when the @a AutoRelease strategy is active, since the resource is released ASAP anyway.
		/// @pre @a key must be a valid resource key (the corresponding resource must be stored inside this manager).
		///  To check for available resources, use Search().
		/// @param key The resource identifier.
		/// @return true, if the resource was immediately released; false, if it is in use.
		bool						release(const ResourceKey& key);

		/// @brief Determines how to react to resources that cannot be loaded.
		/// @details The strategy concerns all failing Acquire() calls.
		void						setLoadingFailureStrategy(Resources::LoadingFailureStrategy strategy);

		/// @brief Determines whether resources are automatically released or not.
		/// @details This function sets the strategy for each resource that will be acquired in the future.
		///  %Resources that have already been allocated are not concerned! This allows you to set the release
		///  behaviour specificially for each resource before you call Acquire().
		void						setReleaseStrategy(Resources::ReleaseStrategy strategy);


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private types
	private:
		typedef typename std::tr1::remove_const<Resource>::type		MutableResource;
		typedef detail::ResourceSlot<MutableResource>				ResourceSlot;
		typedef std::map<ResourceKey, ResourceSlot>					ResourceMap;
		typedef typename ResourceMap::iterator						SlotIterator;
		typedef typename ResourceMap::const_iterator				SlotConstIterator;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private member functions
	private:
		// Loads and inserts a resource.
		ResourcePtr<Resource>					addResource(const ResourceKey& key);
	
		// Unloads and erases a resource.
		void									removeResource(SlotIterator itr);
		

	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		std::tr1::shared_ptr<ResourceMap>		mMap;
		Resources::ReleaseStrategy				mReleaseStrategy;
		Resources::LoadingFailureStrategy		mLoadingFailureStrategy;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Friends
	friend class detail::ResourceDeleter<MutableResource, ResourceKey>;
};

/// @}

} // namespace thor

#include <Thor/Detail/ResourceManager.inl>
#endif // THOR_RESOURCEMANAGER_HPP
