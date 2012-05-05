/////////////////////////////////////////////////////////////////////////////////
//
// Aurora C++ Library
// Copyright (c) 2012 Jan Haller
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

namespace aurora
{

template <class B, typename R>
SingleDispatcher<B, R>::SingleDispatcher(bool supportDerivedToBase)
: mMap()
, mCachedMap()
, mNeedsCacheUpdate(false)
, mDerivedToBase(supportDerivedToBase)
{
}

template <class B, typename R>
template <class D>
void SingleDispatcher<B, R>::add(R (*globalFunction)( AURORA_REPLICATE(B,D) ))
{
	// Without derived-to-base conversions, we can directly edit the cached map and save memory
	registerFunction(mDerivedToBase ? mMap : mCachedMap, typeid(D), 
		Value(new detail::UnaryGlobalFunction<B, AURORA_REPLICATE(B,D), R>(globalFunction)));
}

template <class B, typename R>
template <class D, class C>
void SingleDispatcher<B, R>::add(R (C::*memberFunction)( AURORA_REPLICATE(B,D) ), C& object)
{
	registerFunction(mDerivedToBase ? mMap : mCachedMap, typeid(D),
		Value(new detail::UnaryMemberFunction<B, AURORA_REPLICATE(B,D), R, C>(memberFunction, object)));
}

template <class B, typename R>
template <class D, typename Fn>
void SingleDispatcher<B, R>::add(const Fn& functionObject)
{
	registerFunction(mDerivedToBase ? mMap : mCachedMap, typeid(D),
		Value(new detail::UnaryGlobalFunction<B, AURORA_REPLICATE(B,D), R, Fn>(functionObject)));
}

template <class B, typename R>
R SingleDispatcher<B, R>::call(B arg) const
{
	ensureCacheUpdate();

	std::type_index key = detail::derefTypeid(arg);
	typename FnMap::const_iterator directDispatchItr = findFunction(key, true);

	// If function is directly found, call it
	if (directDispatchItr != mCachedMap.end())
		return directDispatchItr->value->call(arg);

	// If derived-to-base conversions are enabled, look for base class functions to dispatch
	if (mDerivedToBase)
	{
		std::vector<std::type_index> bases;
		detail::getRttiBaseClasses(key, bases);

		// Iterate through all direct and indirect base classes
		AURORA_CITR_FOREACH(std::vector<std::type_index>, bases, baseItr)
		{
			typename FnMap::const_iterator dispatchItr = findFunction(*baseItr);

			// Found match: Add entry to cache, call function
			if (dispatchItr != mMap.end())
			{
				// We are sure the key isn't stored yet, otherwise the direct lookup would have found it
				registerFunction(mCachedMap, key, dispatchItr->value);
				return dispatchItr->value->call(arg);
			}
		}
	}

	// If no corresponding class (or base class) has been found, throw exception
	throw FunctionCallException(std::string("SingleDispatcher::Call() - function with parameter \"") + key.name() +  "\" not registered");
}

template <class B, typename R>
void SingleDispatcher<B, R>::registerFunction(FnMap& fnMap, std::type_index key, Value value) const
{
	// If we update the normal map, the cache becomes invalid
	if (&fnMap == &mMap)
		mNeedsCacheUpdate = true;

	// Create pair of typeids and function
	Pair pair(key, value);

	// If this assertion fails, you are trying to register an already registered function.
	assert(!std::binary_search(fnMap.begin(), fnMap.end(), pair));

	fnMap.push_back(pair);
	std::sort(fnMap.begin(), fnMap.end());
}

template <class B, typename R>
typename SingleDispatcher<B, R>::FnMap::const_iterator SingleDispatcher<B, R>::findFunction(std::type_index key, bool useCache) const
{
	const FnMap& fnMap = useCache ? mCachedMap : mMap;

	// Note: We only compare keys anyway, that's why we can pass a null pointer as value	
	return detail::binarySearch(fnMap.begin(), fnMap.end(), Pair(key, Value()));
}

template <class B, typename R>
void SingleDispatcher<B, R>::ensureCacheUpdate() const
{
	// If map has been changed, apply changes to cache
	if (mNeedsCacheUpdate)
	{
		mCachedMap = mMap;
		mNeedsCacheUpdate = false;
	}
}

} // namespace aurora
