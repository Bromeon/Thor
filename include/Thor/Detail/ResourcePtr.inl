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

namespace thor
{

template <class Resource>
ResourcePtr<Resource>::ResourcePtr()
: mPointer()
{
}

template <class Resource>
ResourcePtr<Resource>::ResourcePtr(Resource* rawPointer)
: mPointer(rawPointer)
{
}

template <class Resource>
template <typename D>
ResourcePtr<Resource>::ResourcePtr(Resource* rawPointer, D deleter)
: mPointer(rawPointer, deleter)
{
}

template <class Resource>
ResourcePtr<Resource>::ResourcePtr(const ResourcePtr& origin)
: mPointer(origin.mPointer)
{
}

template <class Resource>
template <class Resource2>
ResourcePtr<Resource>::ResourcePtr(const ResourcePtr<Resource2>& origin)
: mPointer(origin.mPointer)
{
}

template <class Resource>
ResourcePtr<Resource>& ResourcePtr<Resource>::operator= (const ResourcePtr& origin)
{
	mPointer = origin.mPointer;
	return *this;
}

template <class Resource>
template <class Resource2>
ResourcePtr<Resource>& ResourcePtr<Resource>::operator= (const ResourcePtr<Resource2>& origin)
{
	mPointer = origin.mPointer;
	return *this;
}

template <class Resource>
ResourcePtr<Resource>::~ResourcePtr()
{
}

template <class Resource>
void ResourcePtr<Resource>::swap(ResourcePtr& other)
{
	std::swap(mPointer, other.mPointer);
}

template <class Resource>
void ResourcePtr<Resource>::reset()
{
	mPointer.reset();
}

template <class Resource>
void ResourcePtr<Resource>::reset(Resource* rawPointer)
{
	mPointer.reset(rawPointer);
}

template <class Resource>
template <typename D>
void ResourcePtr<Resource>::reset(Resource* rawPointer, D deleter)
{
	mPointer.reset(rawPointer, deleter);
}

template <class Resource>
ResourcePtr<Resource>::operator aur::SafeBool() const
{
	return aur::toSafeBool(static_cast<bool>(mPointer));
}

template <class Resource>
Resource& ResourcePtr<Resource>::operator* () const
{
	assert(mPointer);
	return mPointer.operator*();
}

template <class Resource>
Resource* ResourcePtr<Resource>::operator-> () const
{
	assert(mPointer);
	return mPointer.operator->();
}

template <class Resource>
ResourcePtr<Resource>::ResourcePtr(std::shared_ptr<Resource> sharedPtr)
: mPointer()
{
	mPointer.swap(sharedPtr);
}

// ---------------------------------------------------------------------------------------------------------------------------

namespace detail
{

	// Deleter that does nothing
	struct NullDeleter
	{
		void operator() (const void*)
		{
		}
	};

} // namespace detail


template <class Resource>
void swap(ResourcePtr<Resource>& lhs, ResourcePtr<Resource>& rhs)
{
	lhs.swap(rhs);
}

template <class Resource1, class Resource2>
bool operator== (const ResourcePtr<Resource1>& lhs, const ResourcePtr<Resource2>& rhs)
{
	// Note: Since C++ doesn't allow comparisons of pointers to different (unrelated) types, we automatically
	// get compiler errors if Resource1 and Resource2 differ in more than CV-qualification.
	return lhs.mPointer == rhs.mPointer;
}

template <class Resource1, class Resource2>
bool operator!= (const ResourcePtr<Resource1>& lhs, const ResourcePtr<Resource2>& rhs)
{
	return !(lhs == rhs);
}

template <class Resource>
ResourcePtr<Resource> noDeletePtr(Resource* pointer)
{
	return ResourcePtr<Resource>(pointer, detail::NullDeleter());
}

} // namespace thor
