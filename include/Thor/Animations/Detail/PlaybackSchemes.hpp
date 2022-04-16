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

#ifndef THOR_PLAYBACKSCHEMES_HPP
#define THOR_PLAYBACKSCHEMES_HPP

#include <Thor/Config.hpp>

#include <Aurora/SmartPtr/CopiedPtr.hpp>

#include <queue>
#include <functional>


namespace thor
{
namespace detail
{

	// Type-erased base class for playback scheme implementations, used by the PlaybackScheme frontend.
	// This class can be used as a base class by typeless playback schemes.
	struct PlaybackSchemeImpl
	{
		virtual					~PlaybackSchemeImpl() {}

		virtual bool			consume() = 0;
		virtual const void*		checkId() const
		{
			return nullptr;
		}
	};

	// Base class for playback scheme backends.
	// General case for implementations that use an ID.
	template <typename Id>
	struct TypedPlaybackSchemeImpl : PlaybackSchemeImpl
	{
		// Introduce type-safety and non-nullness for derived classes
		virtual const Id&		getId() const = 0;

		virtual const void*		checkId() const
		{
			// Implicit Id* -> void*
			return &getId();
		}
	};

	// ---------------------------------------------------------------------------------------------------------------------------


	// Front-end class for playback schemes
	// A playback scheme is an element in the playing queue. It can refer to an animation ID that is played next (possibly coming
	// with playback behavior like repetition or looping) or to side effects (like callbacks).
	template <typename Id>
	class PlaybackScheme
	{
		public:
			// Implicit constructor from derived backend instance
			// Moves an automatic object into a smart pointer for dynamic polymorphism.
			template <typename ConcreteBackend>
			PlaybackScheme(ConcreteBackend&& backend)
			: mBackend(aurora::makeCopied<typename std::remove_reference<ConcreteBackend>::type>(std::move(backend)))
			{
			}

			// Returns whether this scheme should be removed from the queue.
			// This call may change the scheme (e.g. reduce a repeat counter) or have side effects (e.g. invoke a callback)
			bool consume()
			{
				return mBackend->consume();
			}

			// Returns the ID to be looked up next by the player, or nullptr if this is a pure side-effect scheme
			const Id* checkId() const
			{
				return static_cast<const Id*>(mBackend->checkId());
			}

		private:
			aurora::CopiedPtr<PlaybackSchemeImpl> mBackend;
	};

	// ---------------------------------------------------------------------------------------------------------------------------


	// Playback scheme for the repeat() function, or simple IDs (which are treated as 1-time repetition)
	template <typename Id>
	class RepeatScheme : public TypedPlaybackSchemeImpl<Id>
	{
		public:
			RepeatScheme(Id id, std::size_t repetitions)
			: mId(std::move(id))
			, mRepetitionCount(repetitions)
			{
			}

			virtual bool consume()
			{
				return mRepetitionCount-- == 1u;
			}

			virtual const Id& getId() const
			{
				return mId;
			}

			template <typename Id2>
			RepeatScheme<Id2> rebind()
			{
				return RepeatScheme<Id2>(std::move(mId), mRepetitionCount);
			}

		private:
			Id			mId;
			std::size_t	mRepetitionCount;
	};

	// Playback scheme for the loop() function
	template <typename Id>
	class LoopScheme : public TypedPlaybackSchemeImpl<Id>
	{
		public:
			explicit LoopScheme(Id id)
			: mId(std::move(id))
			{
			}

			virtual bool consume()
			{
				return false;
			}

			virtual const Id& getId() const
			{
				return mId;
			}

			template <typename Id2>
			LoopScheme<Id2> rebind()
			{
				return LoopScheme<Id2>(std::move(mId));
			}

		private:
			Id mId;
	};

	// Playback scheme for the callback() function
	class NotifyScheme : public PlaybackSchemeImpl
	{
		public:
			explicit NotifyScheme(std::function<void()> callback)
			: mCallback(std::move(callback))
			{
			}

			virtual bool consume()
			{
				mCallback();
				return true;
			}

			template <typename Unused>
			NotifyScheme& rebind()
			{
				return *this;
			}

		private:
			std::function<void()> mCallback;
	};

	// ---------------------------------------------------------------------------------------------------------------------------


	// Metafunction to infer the right type for the PlaybackScheme template argument
	// Performs an array-to-pointer conversion: "myId" is char[5] -> const char*
	template <template <typename> class Scheme, typename Id>
	struct DecayedScheme
	{
		// Array-to-pointer conversion
		typedef typename std::conditional<std::is_array<Id>::value,
			const typename std::remove_extent<Id>::type*,
			Id
		>::type DecayedId;

		typedef Scheme<DecayedId> Type;
	};

	// ---------------------------------------------------------------------------------------------------------------------------


	// Class acting as a playlist of IDs and information about their repetition schemes
	// The class does not actually store the data, but refer to an external queue to fill
	template <typename Id>
	class PlayingQueue
	{
		public:
			explicit PlayingQueue(std::queue<PlaybackScheme<Id>>& referenced)
			: mQueue(referenced)
			{
			}

			template <typename T>
			PlayingQueue<Id>& operator<< (T&& arg)
			{
				// Template parameter T can be one of:
				// * type convertible to Id
				// * class derived from PlaybackScheme<Id>::Backend
				// * class derived from PlaybackScheme<X>::Backend with X != Id
				// Find out which case it is through tag-dispatching

				typedef typename std::remove_reference<T>::type									ObjectT;
				typedef typename std::is_base_of<PlaybackSchemeImpl, ObjectT>::type				IsPlaybackScheme;
				typedef typename std::is_base_of<TypedPlaybackSchemeImpl<Id>, ObjectT>::type	HasCorrectIdType;

				mQueue.push(createFrontend(std::forward<T>(arg), IsPlaybackScheme(), HasCorrectIdType()));
				return *this;
			}

		private:
			template <typename FwdId>
			PlaybackScheme<Id> createFrontend(FwdId&& id, std::false_type, std::false_type)
			{
				// Argument is not a playback scheme: Use single repetition
				return RepeatScheme<Id>(std::forward<FwdId>(id), 1u);
			}

			template <typename Scheme>
			PlaybackScheme<Id> createFrontend(Scheme&& scheme, std::true_type, std::true_type)
			{
				// Playback scheme with same ID type: forward as-is
				return std::move(scheme);
			}

			template <typename Scheme>
			PlaybackScheme<Id> createFrontend(Scheme&& scheme, std::true_type, std::false_type)
			{
				// Playback scheme with different ID type: call rebind()
				return scheme.template rebind<Id>();
			}

		private:
			std::queue<PlaybackScheme<Id>>& mQueue;
	};

} // namespace detail
} // namespace thor

#endif // THOR_PLAYBACKSCHEMES_HPP
