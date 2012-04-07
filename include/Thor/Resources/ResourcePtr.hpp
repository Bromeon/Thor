/////////////////////////////////////////////////////////////////////////////////
//
// Thor C++ Library
// Copyright (c) 2011-2012 Jan Haller
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
/// @brief Class template thor::ResourcePtr

#ifndef THOR_RESOURCEPTR_HPP
#define THOR_RESOURCEPTR_HPP

#include <Thor/Config.hpp>

#include <Aurora/Tools/SafeBool.hpp>

#include <memory>
#include <cassert>


namespace thor
{

/// @addtogroup Resources
/// @{

/// @brief Reference-counted smart pointer allowing safe access to resources.
/// @details Objects of this class are returned from ResourceManager, when you acquire new or search for existing resources.
///  You should keep a %ResourcePtr instance everywhere you need access to a resource.
/// @n@n The %ResourcePtr has the semantics of a <i>strong reference</i>. That is, as long as it points to a resource,
///  the referenced resource cannot be released. This ensures that program parts that currently use the resource do not suddenly
///  lose access to it.
/// @tparam Resource The resource class you want to access. If @a Resource is const-qualified, you cannot modify
///  the resource object through this smart pointer. The type @a Resource can be incomplete as long as you don't 
///  dereference the pointer.
template <class Resource>
class ResourcePtr
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Default constructor
		/// @details Initializes the object with a null pointer.
								ResourcePtr();

		/// @brief Construct from new resource
		/// @details Increments the reference counter for the new resource.
		/// @param rawPointer Raw pointer to resource, allocated with the new operator.
		explicit				ResourcePtr(Resource* rawPointer);

		/// @brief Construct from new resource with custom deleter
		/// @details Increments the reference counter for the new resource.
		/// @param rawPointer Raw pointer to resource.
		/// @param deleter Callable with the signature <i>void(Resource*)</i> that is invoked when the last %ResourcePtr loses ownership.
		///  To allow %ResourcePtr\<Resource> instances where @a Resource is const-qualified, the deleter shall take a pointer to const.
		template <typename D>
								ResourcePtr(Resource* rawPointer, D deleter);

		/// @brief Copy constructor
		/// @details Increments the reference counter (if the resource is not @a NULL).
								ResourcePtr(const ResourcePtr& origin);
								
		/// @brief Template constructor from less const-qualified %ResourcePtr
		/// @details Allows conversion from <i>ResourcePtr<Resource></i> to <i>ResourcePtr<const Resource></i>.
		template <class Resource2>
								ResourcePtr(const ResourcePtr<Resource2>& origin);

		/// @brief Copy assignment operator
		/// @details Increments the reference counter for the new resource, and decrements the one for the old resource.
		ResourcePtr&			operator= (const ResourcePtr& origin);

		/// @brief Template assignment operator from less const-qualified %ResourcePtr
		/// @details Allows conversion from <i>ResourcePtr<Resource></i> to <i>ResourcePtr<const Resource></i>.
		template <class Resource2>
		ResourcePtr&			operator= (const ResourcePtr<Resource2>& origin);

		/// @brief Destructor
		/// @details Decrements the reference counter. If it reaches zero (i.e. this is the last %ResourcePtr pointing to the
		///  resource) and if the corresponding ResourceManager uses the @a AutoRelease strategy, then the resource is released.
								~ResourcePtr();

		/// @brief Exchanges the contents of two instances.
		void					swap(ResourcePtr& other);

		/// @brief Reset the pointer to @a NULL
		/// @details If this instance is a null pointer, the call has no effects. Otherwise, the reference counter is decremented.
		void					reset();

		/// @brief Reset pointer to new resource
		/// @details Increments the reference counter for the new resource, and decrements the one for the old resource.
		/// @param rawPointer Raw pointer to resource.
		void					reset(Resource* rawPointer);

		/// @brief Reset pointer to new resource with custom deleter
		/// @details Increments the reference counter for the new resource, and decrements the one for the old resource.
		/// @param rawPointer Raw pointer to resource.
		/// @param deleter Callable with the signature <i>void(Resource*)</i> that is invoked when the last %ResourcePtr loses ownership.
		///  To allow %ResourcePtr\<Resource> instances where @a Resource is const-qualified, the deleter shall take a pointer to const.
		template <typename D>
		void					reset(Resource* rawPointer, D deleter);

		/// @brief Checks whether the %ResourcePtr references a valid resource or points to @a NULL.
		/// @details This allows you to write statements of the form <i>if(ptr)</i> or <i>if(!ptr)</i> in a type-safe way
		///  (@a ptr is a %ResourcePtr instance). The actual return type is implementation-defined.
		/// @return bool-like value that behaves in conditional expressions like true, if the pointer is valid; 
		///  and like false, if it points to @a NULL.
								operator aur::SafeBool() const;

		/// @brief Dereferences the pointer.
		/// @pre The %ResourcePtr must be non-empty.
		Resource&				operator* () const;
		
		/// @brief Dereferences the pointer for member access.
		/// @pre The %ResourcePtr must be non-empty.
		Resource*				operator-> () const;
	

	// ---------------------------------------------------------------------------------------------------------------------------
	// Implementation details
	public:
		// Construct from shared_ptr
		explicit				ResourcePtr(std::shared_ptr<Resource> sharedPtr);


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		std::shared_ptr<Resource>	mPointer;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Friends
	/// @cond FriendsAreAnImplementationDetail
	template <class Resource1, class Resource2>
	friend bool operator== (const ResourcePtr<Resource1>& lhs, const ResourcePtr<Resource2>& rhs);
	
	template <class Resource2>
	friend class ResourcePtr;
	/// @endcond
};

/// @relates ResourcePtr
/// @brief Swaps both %ResourcePtr objects.
template <class Resource>
void swap(ResourcePtr<Resource>& lhs, ResourcePtr<Resource>& rhs);

/// @relates ResourcePtr
/// @brief Equality comparison operator
/// @details Returns true when @a lhs and @a rhs share the same resource or are both @a NULL, and false otherwise.
///  @a Resource1 and @a Resource2 may only differ in const-qualification, apart from that the types must be the same.
template <class Resource1, class Resource2>
bool operator== (const ResourcePtr<Resource1>& lhs, const ResourcePtr<Resource2>& rhs);

/// @relates ResourcePtr
/// @brief Inequality comparison operator
/// @details Returns false when @a lhs and @a rhs share the same resource or are both @a NULL, and true otherwise.
///  @a Resource1 and @a Resource2 may only differ in const-qualification, apart from that the types must be the same.
template <class Resource1, class Resource2>
bool operator!= (const ResourcePtr<Resource1>& lhs, const ResourcePtr<Resource2>& rhs);

/// @relates ResourcePtr
/// @brief Returns a %ResourcePtr that doesn't own the resource.
/// @details The returned smart pointer will not call the delete operator, so do not pass objects allocated with new.
///  The caller must make sure that the passed pointer is valid as long as it is referenced in the %ResourcePtr.
///  This allows to store local and static objects in %ResourcePtr instances.
/// @return This function returns <b>ResourcePtr \<Resource>(pointer, nullDeleter)</b> where @a nullDeleter is a functor without any effect.
template <class Resource>
ResourcePtr<Resource> noDeletePtr(Resource* pointer);

/// @}

} // namespace thor

#include <Thor/Detail/ResourcePtr.inl>
#endif // THOR_RESOURCEPTR_HPP
