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

#ifndef THOR_RESOURCESLOT_HPP
#define THOR_RESOURCESLOT_HPP

#include <Aurora/Tools/ForEach.hpp>
#include <Aurora/Tools/Metaprogramming.hpp>
#include <Thor/Config.hpp>

#include <memory>
#include <cassert>


namespace thor
{

template <class R>
class ResourceCache;


namespace detail
{

	// Deleter functor for std::shared_ptr
	// Is called when the last shared_ptr to a resource goes away, releases the resource.
	// Note that the last shared_ptr can be inside a ResourceCache (explicit release policy)
	// or an external shared_ptr to a resource (auto release policy).
	template <class R>
	class ResourceDeleter
	{
		private:
			typedef ResourceCache<R>				Cache;
			typedef typename Cache::ResourceMap		ResourceMap;
			typedef typename Cache::SlotIterator	SlotIterator;

		public:
			ResourceDeleter(std::weak_ptr<ResourceMap> map, SlotIterator iterator)
			: mMap()
			, mIterator(iterator)
			{
				mMap.swap(map);
			}

			// Actual resource deletion - This operator is only invoked when the last shared_ptr to
			// the resource (including the shared_ptr inside the cache) is destroyed.
			void operator() (R* pointer)
			{
				AURORA_REQUIRE_COMPLETE_TYPE(R);
				delete pointer;

				// Unregister resource slot, unless cache has been destroyed meanwhile
				std::shared_ptr<ResourceMap> map = mMap.lock();
				if (map)
					map->erase(mIterator);
			}

		private:
			std::weak_ptr<ResourceMap>		mMap;
			SlotIterator					mIterator;
	};
	
	template <class R>
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
			std::shared_ptr<R> initialize(std::unique_ptr<R> newResource, DeleterFn deleter, bool explicitRelease)
			{
				std::shared_ptr<R> result(newResource.release(), deleter);
				
				// For explicit release policy, keep strong reference to resource and prevent automatic deletion
				mWeakRef = result;
				if (explicitRelease)
					mStrongRef = result;

				return result;
			}

			// Gives back a shared pointer to the resource
			std::shared_ptr<R> share() const
			{
				return mWeakRef.lock();
			}

			// Sets whether the slot keeps a strong reference to the resource or not.
			void keepStrongReference(bool keepStrongRef)
			{
				// If mStrongRef is non-empty, resources cannot be released from outside - explicit ResourceCache::release() calls
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
			std::shared_ptr<R>		mStrongRef;
			std::weak_ptr<R>		mWeakRef;
	};

} // namespace detail
} // namespace thor

#endif // THOR_RESOURCESLOT_HPP
