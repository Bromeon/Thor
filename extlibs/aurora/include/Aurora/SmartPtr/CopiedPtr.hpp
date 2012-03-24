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

/// @file
/// @brief Class template aur::CopiedPtr

#ifndef AURORA_COPIEDPTR_HPP
#define AURORA_COPIEDPTR_HPP

#include <Aurora/SmartPtr/PtrFunctors.hpp>
#include <Aurora/SmartPtr/Detail/PtrOwner.hpp>
#include <Aurora/Tools/SafeBool.hpp>
#include <Aurora/Config.hpp>

#include <cstddef>
#include <algorithm>
#include AURORA_TR1_HEADER(type_traits)

namespace aur
{

/// @addtogroup SmartPtr
/// @{

/// @brief Copyable smart pointer template
/// @tparam T Type of the stored object. Can be void.
/// @details This class supports automatic destruction and copying. It has full value semantics, i.e. the stored object is
///  destroyed in the smart pointer's destructor and copied in the smart pointer's copy constructor. The concept of custom
///  cloners and deleters allows you to specify the way how destructions and copies are performed.
/// @n@n This smart pointer is always unique, no two CopiedPtr instances share the same object.
template <typename T>
class CopiedPtr
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Default constructor
		/// @details Initializes the smart pointer with a null pointer.
		CopiedPtr()
		: mOwner(NULL)
		, mPointer(NULL)
		{
		}

		/// @brief Construct from raw pointer
		/// @param pointer Initial pointer value, can be NULL. Must be convertible to T*.
		template <typename U>
		explicit CopiedPtr(U* pointer)
		: mOwner( detail::newPtrOwner<T>(pointer, OperatorNewCopy<U>(), OperatorDelete<U>()) )
		, mPointer(pointer)
		{
		}

		/// @brief Construct from raw pointer with cloner and deleter
		/// @param pointer Initial pointer value, can be NULL. Must be convertible to T*.
		/// @param cloner Callable with signature <b>T*(const T*)</b> that is invoked during CopiedPtr copies.
		///  Must return a pointer to a copy of the argument. 
		/// @param deleter Callable with signature <b>void(T*)</b> that is invoked during CopiedPtr destruction.
		template <typename U, typename C, typename D>
		CopiedPtr(U* pointer, C cloner, D deleter)
		: mOwner( detail::newPtrOwner<T>(pointer, cloner, deleter) )
		, mPointer(pointer)
		{
		}

		/// @brief Copy constructor
		/// @param origin Original smart pointer
		/// @details If the origin's pointer is NULL, this pointer will also be NULL.
		///  Otherwise, this instance will hold the pointer returned by the cloner.
		CopiedPtr(const CopiedPtr& origin)
		: mOwner(origin.mOwner ? origin.mOwner->clone() : NULL)
		, mPointer(origin.mOwner ? mOwner->getPointer() : NULL)
		{
		}

		/// @brief Construct from different CopiedPtr
		/// @param origin Original smart pointer, where U* convertible to T*. Can refer to a derived object.
		/// @details If the origin's pointer is NULL, this pointer will also be NULL.
		///  Otherwise, this instance will hold the pointer returned by the cloner.
		template <typename U>
		CopiedPtr(const CopiedPtr<U>& origin)
		: mOwner(new detail::PtrIndirection<T, U>(origin.mOwner))
		, mPointer(mOwner->getPointer())
		{
		}

#ifdef AURORA_HAS_CPP11
		/// @brief Move constructor
		/// @param source RValue reference to object of which the ownership is taken.
		template <typename U>
		CopiedPtr(CopiedPtr<U>&& source)
		: mOwner(source.mOwner)
		, mPointer(source.mPointer)
		{
			source.mOwner = NULL;
			source.mPointer = NULL;
		}
#endif // AURORA_HAS_CPP11

		/// @brief Copy assignment operator
		/// @param origin Original smart pointer
		/// @details Can imply a copy and a destruction, invoking origin's cloner and this deleter.
		CopiedPtr& operator= (const CopiedPtr& origin)
		{
			CopiedPtr(origin).swap(*this);
			return *this;
		}

		/// @brief Assignment operator from different CopiedPtr
		/// @param origin Original smart pointer, where U* convertible to T*. Can refer to a derived object.
		/// @details Can imply a copy and a destruction, invoking origin's cloner and this deleter.
		template <typename U>
		CopiedPtr& operator= (const CopiedPtr<U>& origin)
		{
			CopiedPtr(origin).swap(*this);
			return *this;
		}

#ifdef AURORA_HAS_CPP11
		/// @brief Move assignment operator
		/// @param source RValue reference to object of which the ownership is taken.
		template <typename U>
		CopiedPtr& operator= (CopiedPtr<U>&& source)
		{
			CopiedPtr(std::move(source)).swap(*this);
			return *this;
		}
#endif // AURORA_HAS_CPP11

		/// @brief Destructor
		/// @details Invokes the deleter with the stored pointer as argument.
		~CopiedPtr()
		{
			// Destroy owner, whose destructor also deletes the pointer
			delete mOwner;
		}

		/// @brief Exchanges the values of *this and @a other.
		/// 
		void swap(CopiedPtr& other)
		{
			std::swap(mOwner, other.mOwner);
			std::swap(mPointer, other.mPointer);
		}
		
		/// @brief Dereferences the pointer.
		/// 
		AURORA_FAKE_DOC(typename std::tr1::add_reference<T>::type, T&) operator* () const
		{
			assert(mPointer);
			return *mPointer;
		}

		/// @brief Dereferences the pointer for member access.
		/// 
		T* operator-> () const
		{
			assert(mPointer);
			return mPointer;
		}

		/// @brief Checks if the smart pointer is not NULL.
		/// @details Allows expressions of the form <i>if (ptr)</i> or <i>if (!ptr)</i>.
		/// @return Value convertible to true, if CopiedPtr is not empty; value convertible to false otherwise
		operator SafeBool() const
		{
			return toSafeBool(mPointer != NULL);
		}

		/// @brief Permits access to the internal pointer. Designed for rare use.
		/// @return Internally used pointer, use it wisely not to upset the CopiedPtr's memory management.
		T* get() const
		{
			return mPointer;
		}

		T* release()
		{
			mOwner->dismiss();

			T* copy = mPointer;

			reset();
			return copy;
		}

		/// @brief Reset to null pointer
		/// @details If this instance currently holds a pointer, the old deleter is invoked.
		void reset()
		{
			CopiedPtr().swap(*this);
		}

		/// @brief Reset to raw pointer
		/// @param pointer Initial pointer value, can be NULL. Must be convertible to T*.
		/// @details If this instance currently holds a pointer, the old deleter is invoked.
		template <typename U>
		void reset(U* pointer)
		{
			CopiedPtr(pointer).swap(*this);
		}

		/// @brief Reset to raw pointer with cloner and deleter
		/// @param pointer Initial pointer value, can be NULL. Must be convertible to T*.
		/// @param cloner Callable with signature <b>T*(const T*)</b> that is invoked during CopiedPtr copies.
		///  Must return a pointer to a copy of the argument. 
		/// @param deleter Callable with signature <b>void(T*)</b> that is invoked during CopiedPtr destruction.
		/// @details If this instance currently holds a pointer, the old deleter is invoked.
		template <typename U, typename C, typename D>
		void reset(U* pointer, C cloner, D deleter)
		{
			CopiedPtr(pointer, cloner, deleter).swap(*this);
		}


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		detail::PtrOwnerBase<T>*	mOwner;
		T*							mPointer;

	template <typename T2>
	friend class CopiedPtr;
};

/// @relates CopiedPtr
/// @brief Swaps the contents of two CopiedPtr instances.
template <typename T>
void swap(CopiedPtr<T>& lhs, CopiedPtr<T>& rhs)
{
	return lhs.swap(rhs);
}

/// @}

} // namespace aur

#endif // AURORA_COPIEDPTR_HPP
