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
namespace detail
{

	// Base class for type-erasure wrapper around thor::ResourceCache
	struct CacheHolderBase
	{
		virtual ~CacheHolderBase() {}

		virtual void setLoadingFailureStrategy(Resources::LoadingFailureStrategy strategy) = 0;
		virtual void setReleaseStrategy(Resources::ReleaseStrategy strategy) = 0;
	};

	// Concrete wrapper around thor::ResourceCache
	template <class R>
	struct CacheHolder : CacheHolderBase
	{
		virtual void setLoadingFailureStrategy(Resources::LoadingFailureStrategy strategy)
		{
			cache.setLoadingFailureStrategy(strategy);
		}

		virtual void setReleaseStrategy(Resources::ReleaseStrategy strategy)
		{
			cache.setReleaseStrategy(strategy);
		}

		ResourceCache<R> cache;
	};

} // namespace detail

// ---------------------------------------------------------------------------------------------------------------------------


template <class R>
std::shared_ptr<R> MultiResourceCache::search(const ResourceKey<R>& key)
{
	if (ResourceCache<R>* cache = findCache<R>())
		return cache->search(key);
	else
		return nullptr;
}

template <class R>
std::shared_ptr<const R> MultiResourceCache::search(const ResourceKey<R>& key) const
{
	return const_cast<MultiResourceCache*>(this)->search(key);
}

template <class R>
std::shared_ptr<R> MultiResourceCache::acquire(const ResourceKey<R>& key)
{
	ResourceCache<R>* cache = findCache<R>();
	if (!cache)
	{
		// Create thor::ResourceCache and initialize it with strategies
		std::unique_ptr<detail::CacheHolderBase> holder(new detail::CacheHolder<R>());
		holder->setLoadingFailureStrategy(mLoadingFailureStrategy);
		holder->setReleaseStrategy(mReleaseStrategy);

		// Insert cache to the map
		auto pair = std::make_pair(std::type_index(typeid(R)), std::move(holder));
		cache = &dereference<R>(mSpecificCaches.insert(std::move(pair)).first);
	}

	return cache->acquire(key);
}

template <class R>
bool MultiResourceCache::release(const ResourceKey<R>& key)
{
	ResourceCache<R>* cache = findCache<R>();
	assert(cache);

	return cache->release(key);
}

template <class R>
ResourceCache<R>* MultiResourceCache::findCache()
{
	ResourceCacheIterator itr = mSpecificCaches.find(typeid(R));
	if (itr == mSpecificCaches.end())
		return nullptr;
	else
		return &dereference<R>(itr);
}

template <class R>
ResourceCache<R>& MultiResourceCache::dereference(ResourceCacheIterator itr)
{
	assert( dynamic_cast<detail::CacheHolder<R>*>(itr->second.get()) );
	return static_cast<detail::CacheHolder<R>&>(*itr->second).cache;
}

inline void MultiResourceCache::setLoadingFailureStrategy(Resources::LoadingFailureStrategy strategy)
{
	mLoadingFailureStrategy = strategy;

	// Adapt already existing caches
	AURORA_FOREACH(auto& pair, mSpecificCaches)
		pair.second->setLoadingFailureStrategy(strategy);
}

inline void MultiResourceCache::setReleaseStrategy(Resources::ReleaseStrategy strategy)
{
	mReleaseStrategy = strategy;

	// Adapt already existing caches
	AURORA_FOREACH(auto& pair, mSpecificCaches)
		pair.second->setReleaseStrategy(strategy);
}

} // namespace thor
