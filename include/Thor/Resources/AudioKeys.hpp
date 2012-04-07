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
/// @brief Resource keys for SFML Audio module

#ifndef THOR_AUDIOKEYS_HPP
#define THOR_AUDIOKEYS_HPP

#include <Thor/Resources/ResourceKeyTraits.hpp>
#include <Thor/Config.hpp>

#include <Aurora/Tools/Swap.hpp>
#include <Aurora/SmartPtr/CopiedPtr.hpp>

#include <SFML/Config.hpp>

#include <functional>
#include <string>


namespace thor
{

/// @addtogroup Resources
/// @{

/// @brief Namespace for resource-specific functionality
/// @details Contains resource-related functionality that is a bit more specific than ResourceManager and std::shared_ptr.
namespace Resources
{

	/// @brief Predefined key class for sf::SoundBuffer resources
	/// @details You can use this class to specifiy the access and loading of sf::SoundBuffer resources in a ResourceManager.
	class THOR_API SoundBufferKey
	{
		// ---------------------------------------------------------------------------------------------------------------------------
		// Public member functions
		public:
			/// @brief Load sf::SoundBuffer from file name
			/// @param fileName The name of the file from which you want to load the sf::SoundBuffer.
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::SoundBuffer::loadFromFile()
			static SoundBufferKey		fromFile(const std::string& fileName, const std::string& tag = "");

			/// @brief Load sf::SoundBuffer from file in memory
			/// @param data Pointer to the file data in memory.
			/// @param size Size of the data to load, in bytes. 
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::SoundBuffer::loadFromMemory()
			static SoundBufferKey		fromMemory(const void* data, std::size_t size, const std::string& tag = "");

			/// @brief Load sf::SoundBuffer from array of audio samples in memory
			/// @param samples Pointer to the samples array
			/// @param samplesCount Number of samples in the array
			/// @param channelsCount Number of channels (1 = mono, 2 = stereo, ...)  
			/// @param sampleRate Sample rate (number of samples to play per second)
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Soundbuffer::loadFromSamples()
			static SoundBufferKey		fromSamples(const sf::Int16* samples, std::size_t samplesCount,
											unsigned int channelsCount, unsigned int sampleRate, const std::string& tag = "");
			
			/// @brief Load sf::SoundBuffer from input stream
			/// @param stream Source stream to read from.
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::SoundBuffer::loadFromStream()
			static SoundBufferKey		fromStream(sf::InputStream& stream, const std::string& tag = "");

			/// @brief Loads the sound buffer as specified by the static functions.
			/// @details This function is mainly provided for ResourceManager, you don't need to call it directly.
			/// @return Movable smart pointer to the allocated sf::SoundBuffer, or null pointer at loading failure.
			aur::CopiedPtr<sf::SoundBuffer> load() const;

			/// @brief Exchanges the contents of @a *this with @a other.
			///
			void						swap(SoundBufferKey& other);


		// ---------------------------------------------------------------------------------------------------------------------------
		// Private member functions
		private:
			// Make default constructor private
										SoundBufferKey();

		
		// ---------------------------------------------------------------------------------------------------------------------------
		// Private variables
		private:
			std::function<bool(sf::SoundBuffer&)>	mLoader;
			std::string								mKey;

	
		// ---------------------------------------------------------------------------------------------------------------------------
		// Friends
		friend struct detail::KeyInfoAccessor;
	};


	/// @brief Predefined key class for sf::Music resources
	/// @details You can use this class to specifiy the access and loading of sf::Music resources in a ResourceManager.
	///  Note that unlike the other SFML resources, sf::Music is not copied completely into memory, rather streamed continuously.
	///  Therefore, it is only suited to a limited extend as a resource in a resource manager. While ResourceManager allows easy
	///  loading and error-checking, a single sf::Music instance cannot be meaningfully shared, therefore the access through multiple
	///  std::shared_ptr objects isn't recommended. Also note that const pointers to sf::Music aren't useful at all.
	class THOR_API MusicKey
	{
		// ---------------------------------------------------------------------------------------------------------------------------
		// Public member functions
		public:
			/// @brief Open sf::Music from file name
			/// @param fileName The name of the file from which you want to load the sf::Music.
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Music::OpenFromFile()
			static MusicKey				fromFile(const std::string& fileName, const std::string& tag = "");

			/// @brief Open sf::Music from file in memory
			/// @param data Pointer to the file data in memory.
			/// @param size Size of the data to load, in bytes. 
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Music::OpenFromMemory()
			static MusicKey				fromMemory(const void* data, std::size_t size, const std::string& tag = "");

			/// @brief Open sf::Music from input stream
			/// @param stream Source stream to read from.
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Music::OpenFromStream()
			static MusicKey				fromStream(sf::InputStream& stream, const std::string& tag = "");

			/// @brief Loads the music as specified by the static functions.
			/// @details This function is mainly provided for ResourceManager, you don't need to call it directly.
			/// @return Movable smart pointer to the allocated sf::Music, or null pointer at loading failure.
			aur::CopiedPtr<sf::Music>	load() const;

			/// @brief Exchanges the contents of @a *this with @a other.
			///
			void						swap(MusicKey& other);


		// ---------------------------------------------------------------------------------------------------------------------------
		// Private member functions
		private:
			// Make default constructor private
										MusicKey();

		
		// ---------------------------------------------------------------------------------------------------------------------------
		// Private variables
		private:
			std::function<bool(sf::Music&)>	mLoader;
			std::string								mKey;

	
		// ---------------------------------------------------------------------------------------------------------------------------
		// Friends
		friend struct detail::KeyInfoAccessor;
	};


	/// @relates SoundBufferKey 
	/// @brief Comparison operator for strict weak ordering
	bool THOR_API				operator< (const SoundBufferKey& lhs, const SoundBufferKey& rhs);
	
	/// @relates MusicKey 
	/// @brief Comparison operator for strict weak ordering
	bool THOR_API				operator< (const MusicKey& lhs, const MusicKey& rhs);


	/// @relates SoundBufferKey
	/// @brief Swaps two sf::SoundBuffer resource keys.
	AURORA_GLOBAL_SWAP(SoundBufferKey)

	/// @relates MusicKey
	/// @brief Swaps two sf::Music resource keys.
	AURORA_GLOBAL_SWAP(MusicKey)

} // namespace Resources

/// @}

} // namespace thor

#endif // THOR_AUDIOKEYS_HPP
