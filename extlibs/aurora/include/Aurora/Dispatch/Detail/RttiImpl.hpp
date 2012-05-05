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

#ifndef AURORA_RTTIIMPL_HPP
#define AURORA_RTTIIMPL_HPP

#include <vector>
#include <set>
#include <algorithm>
#include <cassert>
#include <typeindex>


namespace aurora
{
namespace detail
{

	// Stores a RTTI class type and links to base classes
	struct RttiClassNode
	{	
		explicit RttiClassNode(std::type_index type)
		: type(type)
		, base(nullptr)
		{
		}

		std::type_index					type;
		mutable const RttiClassNode*	base;
	};

	// Comparison operator for std::set
	inline bool operator< (const RttiClassNode& lhs, const RttiClassNode& rhs)
	{
		return lhs.type < rhs.type;
	}

	// ---------------------------------------------------------------------------------------------------------------------------


	// Manager for class hierarchies whose RTTI is registered by the user
	struct RttiClassManager
	{
		// Return global instance
		static RttiClassManager& instance()
		{
			static RttiClassManager instance;
			return instance;
		}

		// Add type information for a base class
		const RttiClassNode& insertBase(std::type_index base)
		{
			return *types.insert(RttiClassNode(base)).first;
		}

		// Add type information for a derived class (and its base class)
		void insertDerived(std::type_index derived, const RttiClassNode& base)
		{
			std::set<RttiClassNode>::iterator itr = types.insert(RttiClassNode(derived)).first;
			assert(!itr->base);
			itr->base = &base;
		}

		// Fill container with direct and indirect base classes, if available.
		// The container front contains the direct base, the back the indirect ones.
		void getBases(std::type_index derived, std::vector<std::type_index>& out)
		{
			// Add class itself
			out.push_back(derived);

			// Check if the class has a base; if not, abort
			std::set<RttiClassNode>::iterator derivedItr = types.find(RttiClassNode(derived));
			if (derivedItr == types.end())
				return;

			// Go to next base class and add its type info
			const RttiClassNode* next = derivedItr->base;
			while (next)
			{
				out.push_back(next->type);
				next = next->base;
			}
		};

		// Type informations for the stored classes
		std::set<RttiClassNode> types;
	};

	// ---------------------------------------------------------------------------------------------------------------------------


	// Class used to build a class hierarchy based on runtime type informations
	struct RttiClass
	{
		explicit RttiClass(std::type_index type)
		: base(RttiClassManager::instance().insertBase(type))
		{
		}

		RttiClass& derived(std::type_index derived)
		{
			RttiClassManager::instance().insertDerived(derived, base);
			return *this;
		}

		const RttiClassNode& base;
	};

	
	// ---------------------------------------------------------------------------------------------------------------------------
	
	// Type to represent pairs of "indirections" (distances to base classes)
	typedef std::pair<int, int> IndirectionPair;


	// ---------------------------------------------------------------------------------------------------------------------------

	// Collects base classes of a class, registered with the AURORA_RTTI_... macros
	inline void getRttiBaseClasses(std::type_index derived, std::vector<std::type_index>& out)
	{
		RttiClassManager::instance().getBases(derived, out);
	}

	// Computes "indirection pairs" (x, y) which contain the distances from derived to base classes (x for lhs, y for rhs).
	// indirectionLevel specifies the sum x+y for which pairs are generated.
	// (0, 0) is the direct match; i.e. argument and parameter types are the same. (1, 0) requires a derived-to-base conversion
	// of the first argument, (0, 1) one of the second argument, and so on.
	inline void computeRttiBaseIndirections(int nbFirst, int nbSecond, int indirectionLevel, std::vector<IndirectionPair>& out)
	{
		const int end = std::min(nbFirst-1, indirectionLevel);
		for (int i = std::max(indirectionLevel-nbSecond+1, 0); i <= end; ++i)
			out.push_back( IndirectionPair(i, indirectionLevel-i) );
	}
	
	// ---------------------------------------------------------------------------------------------------------------------------
	
	
	// Type-information for dereferenced type
	// Important: Do not const-qualify parameters, or the first overload will accept const U*
	template <typename T>
	std::type_index derefTypeid(T& reference)
	{
		return typeid(reference);
	}

	template <typename T>
	std::type_index derefTypeid(T* pointer)
	{
		return typeid(*pointer);
	}

} // namespace detail
} // namespace aurora

#endif // AURORA_RTTIIMPL_HPP
