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

/// @file
/// @brief Ownership models for thor::ResourceHolder

#ifndef THOR_OWNERSHIPMODELS_HPP
#define THOR_OWNERSHIPMODELS_HPP

#include <Thor/Config.hpp>

#include <Aurora/Meta/Templates.hpp>

#include <memory>
#include <cassert>


namespace thor
{

/// @addtogroup Resources
/// @{

namespace Resources
{

	/// @brief Centralized ownership policy
	/// @details The ResourceHolder object is in charge of ownership: it stores all resources and hands out references to them.
	///  Upon destruction, ResourceHolder releases all resources. It's the users responsibility to keep resources alive as long
	///  as they are in use.
	/// @n@n This is the default ownership policy. It is the simplest and most efficient variant and will suffice for most cases.
	///  Only deviate from it when you have specific requirements.
	/// @n@n With this policy, the user accesses resources through <b>R&</b> or <b>const R&</b>.
	struct CentralOwner {};

	/// @brief Reference-counted ownership policy
	/// @details This ownership model is helpful when resources should remain allocated exactly as long as they are in use.
	///  Access to resources is granted through reference-counted shared pointers. One resource can be used by multiple shared_ptr
	///  objects; the last one being destroyed releases the resource. You @b must store a shared_ptr upon loading the resource, or
	///  it will be released immediately.
	/// @n@n With this policy, the user accesses resources through <b>std::shared_ptr<R></b> or <b>std::shared_ptr<const R></b>.
	struct RefCounted {};

} // namespace Resources

/// @}

// ---------------------------------------------------------------------------------------------------------------------------


namespace detail
{

	template <typename Map>
	struct ElementRef
	{
		Map* map;
		typename Map::iterator itr;
	};

	template <typename Map>
	ElementRef<Map> makeElementRef(Map& map, typename Map::iterator itr)
	{
		// Aggregate syntax instead of return {...}; to support older compilers and work around compiler bug in VS 2015 RC
		ElementRef<Map> ref = {&map, itr};
		return ref;
	}

	template <typename R, typename Map>
	struct TrackingDeleter
	{
		void operator() (R* pointer)
		{
			// If map element exists, erase it
			if (!tracker.expired())
			{
				// Verify resource ID is still there
				assert(element.map->find(element.itr->first) != element.map->end());
				element.map->erase(element.itr);
			}

			// Perform actual deallocation
			AURORA_REQUIRE_COMPLETE_TYPE(R);
			delete pointer;
		}

		ElementRef<Map> element;
		std::weak_ptr<char> tracker;
	};

	// Class to dispatch between ownership model, using partial template specialization
	template <typename Model, typename R>
	struct OwnershipModel;

	// Specialization for centralized ownership (user only holds references)
	template <typename R>
	struct OwnershipModel<Resources::CentralOwner, R>
	{
		typedef R&					Returned;
		typedef const R&			ConstReturned;
		typedef std::unique_ptr<R>	Loaded;
		typedef std::unique_ptr<R>	Stored;

		static Returned makeReturned(const std::unique_ptr<R>& initialOrStorage)
		{
			return *initialOrStorage;
		}

		template <typename Map>
		static std::unique_ptr<R> makeLoaded(std::unique_ptr<R>&& resource, ElementRef<Map>&&)
		{
			return std::move(resource);
		}

		static std::unique_ptr<R> makeStored(std::unique_ptr<R>&& loaded)
		{
			return std::move(loaded);
		}
	};

	// Specialization for reference-counted ownership
	template <typename R>
	struct OwnershipModel<Resources::RefCounted, R>
	{
		typedef std::shared_ptr<R>			Returned;
		typedef std::shared_ptr<const R>	ConstReturned;

		// First object being constructed directly from resource.
		// Prepares tracking semantics
		struct Loaded
		{
			std::shared_ptr<char> tracked;
			std::shared_ptr<R> resource;
		};

		// Map's value type, stores weak pointer to resource and eraser to remove itself from map
		// Can potentially be optimized by avoiding indirect call through std::function
		struct Stored
		{
			std::shared_ptr<char> tracked;
			std::weak_ptr<R> resource;
		};

		static std::shared_ptr<R> makeReturned(const Loaded& loaded)
		{
			return loaded.resource;
		}

		static std::shared_ptr<R> makeReturned(const Stored& stored)
		{
			assert(!stored.resource.expired());
			return std::shared_ptr<R>(stored.resource); // shouldn't throw after assert
		}

		template <typename Map>
		static Loaded makeLoaded(std::unique_ptr<R>&& resource, ElementRef<Map>&& element)
		{
			// Tracked object: shared pointer referenced by multiple weak pointers.
			// If object holding tracked dies, all weak_ptr objects become expired.
			auto tracked = std::make_shared<char>();

			// Deleter for shared_ptr. Users will hold shared_ptrs with this deleter, so
			// the last one will erase the element from the map.
			TrackingDeleter<R, Map> deleter;
			deleter.tracker = tracked;
			deleter.element = element;

			// Create initial object that holds the resource as well as a strong tracked shared_ptr
			Loaded loaded;
			loaded.tracked = std::move(tracked);
			loaded.resource = std::shared_ptr<R>(resource.release(), deleter);
			return loaded;
		}

		static Stored makeStored(Loaded&& loaded)
		{
			Stored stored;
			stored.tracked = std::move(loaded.tracked);
			stored.resource = std::weak_ptr<R>(loaded.resource);
			return stored;
		}
	};

} // namespace detail
} // namespace thor

#endif // THOR_OWNERSHIPMODELS_HPP
