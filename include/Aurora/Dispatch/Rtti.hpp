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

#ifndef AURORA_RTTI_HPP
#define AURORA_RTTI_HPP

/// @file
/// @brief Manual enhancements of Runtime Type Information

#include <Aurora/Dispatch/Detail/RttiImpl.hpp>
#include <Aurora/Config.hpp>


namespace aurora
{

/// @addtogroup Dispatch
/// @{

/// @brief Register runtime type information for a base class
/// @details You need this for the dynamic dispatchers to cope with derived-to-base conversions.
/// Since C++ offers no possibility to recognize the base classes of a class at runtime, this must be done manually.
/// Note that multiple inheritance is not supported.
/// @n@n Example class hierarchy:
/// @code
/// class Vehicle { public: virtual ~Vehicle(); }
/// class Aircraft : public Vehicle {};
/// class Watercraft : public Vehicle {};
/// class Ship : public Watercraft {};
/// class Submarine : public Watercraft {};
/// @endcode
/// Register classes as follows (once inside a function):
/// @code
/// AURORA_RTTI_BASE(Vehicle)
///   AURORA_RTTI_DERIVED(Aircraft)
///   AURORA_RTTI_DERIVED(Watercraft);
/// AURORA_RTTI_BASE(Watercraft)
///   AURORA_RTTI_DERIVED(Ship)
///   AURORA_RTTI_DERIVED(Submarine);
/// @endcode
/// @hideinitializer
#define AURORA_RTTI_BASE(BaseClass)			::aurora::detail::RttiClass(typeid(BaseClass))

/// @brief Register runtime type information for a derived class
/// @see AURORA_RTTI_BASE
/// @hideinitializer
#define AURORA_RTTI_DERIVED(DerivedClass)	.derived(typeid(DerivedClass))

/// @}

} // namespace aurora

#endif // AURORA_RTTI_HPP
