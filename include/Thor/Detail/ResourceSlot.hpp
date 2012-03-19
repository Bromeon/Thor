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

#ifndef THOR_RESOURCESLOT_HPP
#define THOR_RESOURCESLOT_HPP

#include <Aurora/Tools/ForEach.hpp>
#include <Thor/Config.hpp>

#include AURORA_TR1_HEADER(memory)
#include <cassert>


namespace thor
{

template <class Resource, class ResourceKey>
class ResourceManager;


namespace detail
{

	// Deleter functor for std::tr1::shared_ptr
	// Is called when the last shared_ptr to a resource goes away, releases the resource.
	// Note that the last shared_ptr can be inside a ResourceManager (explicit release policy)
	// or an external shared_ptr to a resource (auto release policy).
	template <class Resource, class ResourceKey>
	class ResourceDeleter
	{
		private:
			typedef ResourceManager<Resource, ResourceKey>	Manager;
			typedef typename Manager::ResourceMap			ResourceMap;
			typedef typename Manager::SlotIterator			SlotIterator;

		public:
			ResourceDeleter(std::tr1::weak_ptr<ResourceMap> map, SlotIterator iterator)
			: mMap(map)
			, mIterator(iterator)
			{
			}

			// Actual resource deletion - This operator is only invoked when the last shared_ptr to
			// the resource (including the shared_ptr inside the manager) is destroyed.
			void operator() (Resource* pointer)
			{
				delete pointer;

				// Unregister resource slot, unless manager has been destroyed meanwhile
				std::tr1::shared_ptr<ResourceMap> map = mMap.lock();
				if (map)
					map->erase(mIterator);
			}

		private:
			std::tr1::weak_ptr<ResourceMap>		mMap;
			SlotIterator						mIterator;
	};
	
	template <class Resource>
	class ResourceSlot
	{
		public:
			// Constructor - can't initialize object completely because iterator to slot is not known yet
			ResourceSlot()
			: mStrongRef()
			, mWeakRef()
			{
			}

			template <typename DeleterFn>
			std::tr1::shared_ptr<Resource> initialize(Resource* newResource, DeleterFn deleter, bool explicitRelease)
			{
				std::tr1::shared_ptr<Resource> result(newResource, deleter);
				
				// For explicit release policy, keep strong reference to resource and prevent automatic deletion
				mWeakRef = result;
				if (explicitRelease)
					mStrongRef = result;

				return result;
			}

			// Gives back a shared pointer to the resource
			std::tr1::shared_ptr<Resource> share() const
			{
				return mWeakRef.lock();
			}

			// Sets whether the slot keeps a strong reference to the resource or not.
			void keepStrongReference(bool keepStrongRef)
			{
				// If mStrongRef is non-empty, resources cannot be released from outside - explicit ResourceManager::Release() calls
				// are required. In contrast, for an empty mStrongRef, the last external shared_ptr releases the resource.
				if (keepStrongRef)
					mStrongRef = mWeakRef.lock();
				else
					mStrongRef.reset();
			}

			// Returns true when no external shared_ptr points to the resource
			bool isUnused() const
			{
				// Note: unique() is undefined for empty shared_ptr objects
				return mStrongRef && mStrongRef.unique();
			}

		private:
			std::tr1::shared_ptr<Resource>	mStrongRef;
			std::tr1::weak_ptr<Resource>	mWeakRef;
	};

} // namespace detail
} // namespace thor

#endif // THOR_RESOURCESLOT_HPP
