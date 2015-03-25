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
typename ResourceHolder<R, I, O>::Resource ResourceHolder<R, I, O>::acquire(const I& id, const ResourceLoader<R>& how)
{
	auto found = mMap.find(id);
	if (found == mMap.end())
		return load(id, how);
	else
		return Om::MakeReturned(found->second);
}

template <typename R, typename I, class O>
void ResourceHolder<R, I, O>::release(const I& id)
{
	assert(mMap.find(id) != mMap.end());
	mMap.erase(id);
}

template <typename R, typename I, class O>
typename ResourceHolder<R, I, O>::Resource ResourceHolder<R, I, O>::operator[] (const I& id)
{
	auto found = mMap.find(id);
	assert(found != mMap.end());

	return Om::MakeReturned(found->second);
}

template <typename R, typename I, class O>
typename ResourceHolder<R, I, O>::ConstResource ResourceHolder<R, I, O>::operator[] (const I& id) const
{
	auto found = mMap.find(id);
	assert(found != mMap.end());

	return Om::MakeReturned(found->second);
}

template <typename R, typename I, class O>
typename ResourceHolder<R, I, O>::Resource ResourceHolder<R, I, O>::load(const I& id, const ResourceLoader<R>& how)
{
	assert(mMap.find(id) == mMap.end());

	// Null pointer: Failed to load
	std::unique_ptr<R> loaded = how.load();
	if (!loaded)
		throw ResourceLoadingException("Failed to load resource \"" + how.getInfo() + "\"");

	// This order because in case of shared ownership, shared_ptr must be kept alive across weak_ptr
	typename Om::Loaded resource(std::move(loaded));
	typename Om::Returned returned = Om::MakeReturned(resource);
	mMap.emplace(id, Om::MakeStored(std::move(resource)));

	return returned;
}

} // namespace thor
