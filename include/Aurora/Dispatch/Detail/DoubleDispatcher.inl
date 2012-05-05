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
DoubleDispatcher<B, R>::DoubleDispatcher(bool symmetric, bool supportDerivedToBase)
: mMap()
, mCachedMap()
, mNeedsCacheUpdate(false)
, mSymmetric(symmetric)
, mDerivedToBase(supportDerivedToBase)
{
}

template <class B, typename R>
template <class D1, class D2>
void DoubleDispatcher<B, R>::add(R (*globalFunction)( AURORA_REPLICATE(B,D1), AURORA_REPLICATE(B,D2) ))
{
	// Without derived-to-base conversions, we can directly edit the cached map and save memory
	registerFunction(mDerivedToBase ? mMap : mCachedMap, typeid(D1), typeid(D2),
		Value(new detail::BinaryGlobalFunction<B, AURORA_REPLICATE(B,D1), AURORA_REPLICATE(B,D2), R>(globalFunction)));
}

template <class B, typename R>
template <class D1, class D2, class C>
void DoubleDispatcher<B, R>::add(R (C::*memberFunction)( AURORA_REPLICATE(B,D1), AURORA_REPLICATE(B,D2) ), C& object)
{
	registerFunction(mDerivedToBase ? mMap : mCachedMap, typeid(D1), typeid(D2),
		Value(new detail::BinaryMemberFunction<B, AURORA_REPLICATE(B,D1), AURORA_REPLICATE(B,D2), R, C>(memberFunction, object)));
}

template <class B, typename R>
template <class D1, class D2, typename Fn>
void DoubleDispatcher<B, R>::add(const Fn& functionObject)
{
	registerFunction(mDerivedToBase ? mMap : mCachedMap, typeid(D1), typeid(D2),
		Value(new detail::BinaryGlobalFunction<B, AURORA_REPLICATE(B,D1), AURORA_REPLICATE(B,D2), R, Fn>(functionObject)));
}

template <class B, typename R>
R DoubleDispatcher<B, R>::call(B arg1, B arg2) const
{
	ensureCacheUpdate();

	std::type_index key1 = detail::derefTypeid(arg1);
	std::type_index key2 = detail::derefTypeid(arg2);
	typename FnMap::const_iterator directDispatchItr = findFunction(key1, key2, true);

	// If function is directly found, call it
	if (directDispatchItr != mCachedMap.end())
		return directDispatchItr->value->call(arg1, arg2);
	
	// If derived-to-base conversions are enabled, look for base class functions to dispatch
	if (mDerivedToBase)
	{
		std::vector<std::type_index> bases1, bases2;
		detail::getRttiBaseClasses(key1, bases1);
		detail::getRttiBaseClasses(key2, bases2);

		// Check different indirection levels (distances of derived-to-base conversions), where a smaller indirection has
		// higher priority for overload resolution. Begin at 1, because indirection 0 (exact types) has already been checked.
		const unsigned int indirectionMaxLevel = std::max(bases1.size(), bases2.size());
		for (unsigned int indirectionLevel = 1; indirectionLevel <= indirectionMaxLevel; ++indirectionLevel)
		{
			std::vector<detail::IndirectionPair> indirections;
			detail::computeRttiBaseIndirections(bases1.size(), bases2.size(), indirectionLevel, indirections);

			// Iterate through all possible matches of the current indirection level (for success, there must be exactly 1 match)
			typename FnMap::const_iterator dispatchItr = mMap.end();
			AURORA_CITR_FOREACH(std::vector<detail::IndirectionPair>, indirections, itr)
			{
				// Check if types corresponding to current indirection are registered
				typename FnMap::const_iterator currentItr = findFunction(bases1[itr->first], bases2[itr->second]);

				if (currentItr == mMap.end())
					continue;

				// If more than one match is found, the call is ambiguous
				if (dispatchItr != mMap.end())
				{
					throw FunctionCallException(std::string("DoubleDispatcher::Call() - overload resolution of parameters \"")
						+ key1.name() +  "\" and \"" + key2.name() + "\" is ambiguous");
				}

				dispatchItr = currentItr;
			}

			// Found unique match: Add entry to cache, call function
			if (dispatchItr != mMap.end())
			{
				// We are sure the key isn't stored yet, otherwise the direct lookup would have found it
				registerFunction(mCachedMap, key1, key2, dispatchItr->value);
				return dispatchItr->value->call(arg1, arg2);
			}	
		}
	}

	throw FunctionCallException(std::string("DoubleDispatcher::Call() - function with parameters \"")
		+ key1.name() +  "\" and \"" + key2.name() + "\" not registered");
}

template <class B, typename R>
void DoubleDispatcher<B, R>::registerFunction(FnMap& fnMap, std::type_index key1, std::type_index key2, Value value) const
{
	// If we update the normal map, the cache becomes invalid
	if (&fnMap == &mMap)
		mNeedsCacheUpdate = true;

	// Create pair of typeids and function
	Pair pair(makeArgumentPair(key1, key2), value);

	// If this assertion fails, you are trying to register an already registered function.
	assert(!std::binary_search(fnMap.begin(), fnMap.end(), pair));

	fnMap.push_back(pair);
	std::sort(fnMap.begin(), fnMap.end());
}

template <class B, typename R>
typename DoubleDispatcher<B, R>::FnMap::const_iterator DoubleDispatcher<B, R>::findFunction(
	std::type_index key1, std::type_index key2, bool useCache) const
{
	const FnMap& fnMap = useCache ? mCachedMap : mMap;

	// Note: We only compare keys anyway, that's why we can pass a null pointer as value
	Key key = makeArgumentPair(key1, key2);
	return detail::binarySearch(fnMap.begin(), fnMap.end(), Pair(key, Value()));
}

template <class B, typename R>
void DoubleDispatcher<B, R>::ensureCacheUpdate() const
{
	// If map has been changed, apply changes to cache
	if (mNeedsCacheUpdate)
	{
		mCachedMap = mMap;
		mNeedsCacheUpdate = false;
	}
}

template <class B, typename R>
typename DoubleDispatcher<B, R>::Key DoubleDispatcher<B, R>::makeArgumentPair(std::type_index key1, std::type_index key2) const
{
	// When symmetric, (key1,key2) and (key2,key1) are the same -> sort so that we always have (key1,key2)
	if (mSymmetric && key2 < key1)
		std::swap(key1, key2);

	return Key(key1, key2);
}

} // namespace aurora
