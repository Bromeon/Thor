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

#ifndef THOR_OWNERSHIPMODELS_HPP
#define THOR_OWNERSHIPMODELS_HPP

#include <Thor/Config.hpp>

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

		static Returned MakeReturned(const std::unique_ptr<R>& initialOrStorage)
		{
			return *initialOrStorage;
		}

		static std::unique_ptr<R> MakeStored(std::unique_ptr<R>&& loaded)
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
		typedef std::shared_ptr<R>			Loaded;
		typedef std::weak_ptr<R>			Stored;

		static std::shared_ptr<R> MakeReturned(const std::shared_ptr<R>& loaded)
		{
			return loaded;
		}

		static std::shared_ptr<R> MakeReturned(const std::weak_ptr<R>& stored)
		{
			assert(!stored.expired());
			return std::shared_ptr<R>(stored); // shouldn't throw after assert
		}

		static std::weak_ptr<R> MakeStored(std::shared_ptr<R>&& loaded)
		{
			return std::weak_ptr<R>(loaded);
		}
	};

} // namespace detail
} // namespace thor

#endif // THOR_OWNERSHIPMODELS_HPP
