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
/// @brief Class thor::MultiResourceCache

#ifndef THOR_MULTIRESOURCECACHE_HPP
#define THOR_MULTIRESOURCECACHE_HPP

#include <Thor/Resources/ResourceCache.hpp>
#include <Thor/Config.hpp>

#include <map>
#include <memory>
#include <typeindex>


namespace thor
{
namespace detail
{

	struct CacheHolderBase;

} // namespace detail

// ---------------------------------------------------------------------------------------------------------------------------


/// @addtogroup Resources
/// @{

/// @brief Class to manage resources of different types uniformly.
/// 
class MultiResourceCache
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @copydoc ResourceCache::ResourceCache()
		///
									MultiResourceCache() {}
			
		/// @copydoc ResourceCache::search(const ResourceKey<R>&)
		///
		template <class R>
		std::shared_ptr<R>			search(const ResourceKey<R>& key);

		/// @copydoc ResourceCache::search(const ResourceKey<R>&) const
		///
		template <class R>
		std::shared_ptr<const R>	search(const ResourceKey<R>& key) const;

		/// @copybrief ResourceCache::acquire
		///
		template <class R>
		std::shared_ptr<R>			acquire(const ResourceKey<R>& key);

		/// @copydoc ResourceCache::release
		///
		template <class R>
		bool						release(const ResourceKey<R>& key);

		/// @copydoc ResourceCache::setLoadingFailureStrategy
		///
		void						setLoadingFailureStrategy(Resources::LoadingFailureStrategy strategy);

		/// @copydoc ResourceCache::setReleaseStrategy
		///
		void						setReleaseStrategy(Resources::ReleaseStrategy strategy);


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private types
	private:
		typedef std::map<std::type_index, std::unique_ptr<detail::CacheHolderBase>> ResourceCacheMap;
		typedef ResourceCacheMap::iterator											ResourceCacheIterator;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private member functions
	private:
		template <class R>
		ResourceCache<R>*			findCache();

		template <class R>
		ResourceCache<R>&			dereference(ResourceCacheIterator itr);


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		ResourceCacheMap						mSpecificCaches;
		Resources::ReleaseStrategy				mReleaseStrategy;
		Resources::LoadingFailureStrategy		mLoadingFailureStrategy;
};

/// @}

} // namespace thor

#include <Thor/Resources/Detail/MultiResourceCache.inl>
#endif // THOR_MULTIRESOURCECACHE_HPP
