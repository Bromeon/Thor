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

namespace thor
{

template <typename R, typename I, class O>
ResourceHolder<R, I, O>::ResourceHolder()
: mMap()
{
}

template <typename R, typename I, class O>
ResourceHolder<R, I, O>::ResourceHolder(ResourceHolder&& source)
: mMap(std::move(source.mMap))
{
}

template <typename R, typename I, class O>
ResourceHolder<R, I, O>& ResourceHolder<R, I, O>::operator= (ResourceHolder&& source)
{
	mMap = std::move(source.mMap);

	return *this;
}

template <typename R, typename I, class O>
typename ResourceHolder<R, I, O>::Resource ResourceHolder<R, I, O>::acquire(const I& id, const ResourceLoader<R>& how, Resources::KnownIdStrategy known)
{
	// ID is new: we always load the resource
	auto found = mMap.find(id);
	if (found == mMap.end())
		return load(id, how);

	// ID is known: behavior depends on strategy
	switch (known)
	{
		default: // TODO: Assume 'default' unreachable
		case Resources::AssumeNew:
			throw ResourceAccessException("Failed to load resource, ID already stored in ResourceHolder");

		case Resources::Reload:
			release(id);
			return load(id, how);

		case Resources::Reuse:
			return Om::makeReturned(found->second);
	}
}

template <typename R, typename I, class O>
void ResourceHolder<R, I, O>::release(const I& id)
{
	auto found = mMap.find(id);
	if (found == mMap.end())
		throw ResourceAccessException("Failed to release resource, ID not currently stored in ResourceHolder");

	mMap.erase(found);
}

template <typename R, typename I, class O>
typename ResourceHolder<R, I, O>::Resource ResourceHolder<R, I, O>::operator[] (const I& id)
{
	auto found = mMap.find(id);
	if (found == mMap.end())
		throw ResourceAccessException("Failed to access resource, ID not currently stored in ResourceHolder");

	return Om::makeReturned(found->second);
}

template <typename R, typename I, class O>
typename ResourceHolder<R, I, O>::ConstResource ResourceHolder<R, I, O>::operator[] (const I& id) const
{
	auto found = mMap.find(id);
	if (found == mMap.end())
		throw ResourceAccessException("Failed to access resource, ID not currently stored in ResourceHolder");

	return Om::makeReturned(found->second);
}

template <typename R, typename I, class O>
typename ResourceHolder<R, I, O>::Resource ResourceHolder<R, I, O>::load(const I& id, const ResourceLoader<R>& what)
{
	assert(mMap.find(id) == mMap.end());

	// Loading process is rather complicated because it has to respect different ownership semantics.
	// That's why the resource is moved several times. The data flow is as follows:
	// original (temporary) ----> loaded (temporary) .---> returned (handed out to user)
	//                                                `--> stored (stored in resource holder's map)
	std::unique_ptr<R> original = what.load();
	if (!original)
		throw ResourceLoadingException("Failed to load resource \"" + what.getInfo() + "\"");

	// Insert initially empty element, to learn about its iterator
	auto inserted = mMap.insert(std::make_pair(id, typename Om::Stored())).first;

	// For ownership policies that try to be smart and remove resources from the holder when unused, 
	// we need to pass them information about the container and the iterator referring to the element
	auto elementRef = detail::makeElementRef(mMap, inserted);

	// Create temporary 'loaded' object and from it, 'returned' object given to user
	typename Om::Loaded loaded = Om::makeLoaded(std::move(original), std::move(elementRef));
	typename Om::Returned returned = Om::makeReturned(loaded);

	// Actually store resource (together with tracking element) in map
	inserted->second = Om::makeStored(std::move(loaded));

	return returned;
}

} // namespace thor
