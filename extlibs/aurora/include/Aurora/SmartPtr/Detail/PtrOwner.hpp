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

#pragma once

#include <Aurora/Tools/NonCopyable.hpp>

#include <cassert>


namespace aurora
{
namespace detail
{
	
	// Types to differ between move and copy semantics
	struct CopyTag {};
	struct MoveTag {};

	// Abstract base class for pointer owners
	template <typename T>
	struct PtrOwnerBase
	{
		virtual ~PtrOwnerBase() {}

		// Returns an independent polymorphic copy
		virtual PtrOwnerBase<T>*	clone() const = 0;

		// Returns the stored pointer
		virtual T*					getPointer() const = 0;
	};


	// Default pointer owner
	template <typename T, typename U, typename C, typename D>
	struct PtrOwner : PtrOwnerBase<T>
	{
		PtrOwner(U* pointer, C cloner, D deleter)
		: pointer(pointer)
		, cloner(cloner)
		, deleter(deleter)
		{
		}

		virtual ~PtrOwner()
		{
			if (pointer)
				deleter(pointer);
		}

		virtual PtrOwner* clone() const
		{
			assert(pointer);
			return new PtrOwner(cloner(pointer), cloner, deleter);
		}

		virtual T* getPointer() const
		{
			return pointer;
		}

		U* pointer;
		C cloner;
		D deleter;
	};
	
	// Used for U* -> T* conversion
	template <typename T, typename U>
	struct PtrIndirection : PtrOwnerBase<T>
	{
		explicit PtrIndirection(const PtrOwnerBase<U>* originBase, CopyTag)
		: base(originBase->clone())
		{
		}

		explicit PtrIndirection(PtrOwnerBase<U>* sourceBase, MoveTag)
		: base(sourceBase)
		{
		}

		virtual ~PtrIndirection()
		{
			delete base;
		}

		virtual PtrIndirection<T, U>* clone() const
		{
			return new PtrIndirection<T, U>(base, CopyTag());
		}

		virtual T* getPointer() const
		{
			return base->getPointer();
		}

		PtrOwnerBase<U>* base;
	};
	
	template <typename T, typename U, typename C, typename D>
	PtrOwnerBase<T>* newPtrOwner(U* pointer, C cloner, D deleter)
	{
		return new PtrOwner<T, U, C, D>(pointer, cloner, deleter);
	}

} // namespace detail
} // namespace aurora
