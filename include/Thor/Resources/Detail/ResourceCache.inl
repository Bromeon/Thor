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

namespace thor
{

template <class R>
ResourceCache<R>::ResourceCache()
: mMap(new ResourceMap())
, mReleaseStrategy(Resources::ExplicitRelease)
, mLoadingFailureStrategy(Resources::ThrowException)
{
}

template <class R>
std::shared_ptr<R> ResourceCache<R>::search(const ResourceKey<R>& key)
{
	SlotConstIterator itr = mMap->find(key);
	
	// Return found resource or nullptr if not found
	if (itr == mMap->end())
		return nullptr;
	else
		return std::shared_ptr<R>(itr->second.share());
}

template <class R>
std::shared_ptr<const R> ResourceCache<R>::search(const ResourceKey<R>& key) const
{
	return const_cast<ResourceCache*>(this)->search(key);
}

template <class R>
std::shared_ptr<R> ResourceCache<R>::acquire(const ResourceKey<R>& key)
{
	SlotIterator itr = mMap->find(key);

	// Load and insert resource, if not stored yet
	if (itr == mMap->end())
		return addResource(key);

	// If resource is already stored, return pointer to it
	return itr->second.share();
}

template <class R>
bool ResourceCache<R>::release(const ResourceKey<R>& key)
{
	// Find corresponding map entry
	SlotIterator itr = mMap->find(key);
	assert(itr != mMap->end());
	
	// Unload and remove resource (don't call removeResource() because the last shared_ptr's deleter erases the iterator)
	ResourceSlot& slot = itr->second;
	const bool unused = slot.isUnused();

	// AFTER checking for unused, release ownership of strong-ref shared_ptr
	// This either directly releases the resource, or at the latest when external strong refs lose ownership.
	slot.keepStrongReference(false);

	// Release() is successful if the resource is not in use
	return unused;
}

template <class R>
void ResourceCache<R>::setLoadingFailureStrategy(Resources::LoadingFailureStrategy strategy)
{
	mLoadingFailureStrategy = strategy;
}

template <class R>
void ResourceCache<R>::setReleaseStrategy(Resources::ReleaseStrategy strategy)
{
	mReleaseStrategy = strategy;
}

template <class R>
std::shared_ptr<R> ResourceCache<R>::addResource(const ResourceKey<R>& key)
{
	// Try to load resource, react with strategy at failure
	std::unique_ptr<R> resource = key.load();
	if (!resource)
	{
		switch (mLoadingFailureStrategy)
		{
			case Resources::ThrowException:
				throw ResourceLoadingException("Failed to load resource \"" + key.getInfo() + "\"");

			case Resources::ReturnNullPointer:
				return nullptr;
		}
	}

	// When succeeded, insert empty resource slot into the map
	SlotIterator itr = mMap->insert( std::make_pair(key, ResourceSlot()) ).first;

	// Create deleter able to call std::map::erase() and to release the resource
	detail::ResourceDeleter<R> deleter(mMap, itr);

	// Post-initialize inserted slot, return shared pointer to resource
	return itr->second.initialize(std::move(resource), deleter, mReleaseStrategy == Resources::ExplicitRelease);
}

template <class R>
void ResourceCache<R>::removeResource(SlotIterator itr)
{
	// Deallocate and remove resource from map
	mMap->erase(itr);
}

} // namespace thor
