/////////////////////////////////////////////////////////////////////////////////
//
// Thor C++ Library
// Copyright (c) 2011-2022 Jan Haller
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
/// @brief Enum KnownIdStrategy, used by thor::ResourceHolder

#ifndef THOR_KNOWNIDSTRATEGY_HPP
#define THOR_KNOWNIDSTRATEGY_HPP


namespace thor
{

/// @addtogroup Resources
/// @{

namespace Resources
{

	/// @brief Strategy to deal with already known resource IDs
	/// @details Determines what happens if the user calls ResourceHolder::acquire() with an ID that is already
	///  associated with a different resource.
	enum KnownIdStrategy
	{
		AssumeNew,	///< Loads a new resource if the ID is unknown. Otherwise, a ResourceAccessException is thrown.
		Reuse,		///< Loads a new resource if the ID is unknown. Otherwise, the resource associated with the ID is reused.
		Reload,		///< Always loads a new resource. If the ID is already known, the corresponding resource is released.
	};

} // namespace Resources

/// @}

} // namespace thor

#endif // THOR_KNOWNIDSTRATEGY_HPP
