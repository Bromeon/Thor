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
/// @brief Namespace thor::Playback

#ifndef THOR_PLAYBACK_HPP
#define THOR_PLAYBACK_HPP

#include <Thor/Config.hpp>
#include <Thor/Animations/Detail/PlaybackSchemes.hpp>

#include <cassert>


namespace thor
{

/// @addtogroup Animations
/// @{

/// @brief Namespace for animation playback schemes
/// @details Provides functions that define the way in which animations can be queued and repeated.
/// @see thor::Animator
namespace Playback
{

	/// @brief Repeat animation a finite number of times
	/// @param id Identifier referring to the animation
	/// @param times Number of times > 0
	/// @return Queue object with @c operator<< to chain multiple playback schemes
	template <typename Id>
	AURORA_IMPL_DEF(typename detail::DecayedScheme<detail::RepeatScheme, Id>::Type) repeat(Id id, std::size_t times)
	{
		assert(times > 0u);
		return typename detail::DecayedScheme<detail::RepeatScheme, Id>::Type(std::move(id), times);
	}

	/// @brief Repeat animation an infinite number of times
	/// @param id Identifier referring to the animation
	/// @return Queue object with @c operator<< to chain multiple playback schemes
	template <typename Id>
	AURORA_IMPL_DEF(typename detail::DecayedScheme<detail::LoopScheme, Id>::Type) loop(Id id)
	{
		return typename detail::DecayedScheme<detail::LoopScheme, Id>::Type(std::move(id));
	}

	/// @brief Register a callback function in the queue
	/// @param callback Function to be called.
	/// @return Queue object with @c operator<< to chain multiple playback schemes
	inline AURORA_IMPL_DEF(detail::NotifyScheme) notify(std::function<void()> callback)
	{
		return detail::NotifyScheme(std::move(callback));
	}

} // namespace Playback

/// @}

} // namespace thor

#endif // THOR_PLAYBACK_HPP
