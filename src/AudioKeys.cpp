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

#include <Thor/Resources/AudioKeys.hpp>
#include <Thor/Detail/ResourceKeyHelpers.hpp>

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Music.hpp>


namespace thor
{
namespace Resources
{

	SoundBufferKey::SoundBufferKey()
	{
	}

	SoundBufferKey SoundBufferKey::fromFile(const std::string& fileName, const std::string& tag)
	{
		SoundBufferKey tmp;
		tmp.mLoader =	tr1::bind(&sf::SoundBuffer::loadFromFile, _1, fileName);
		tmp.mKey =		detail::Tagger("File") << fileName << tag;
		return tmp;
	}

	SoundBufferKey SoundBufferKey::fromMemory(const void* data, std::size_t size, const std::string& tag)
	{
		SoundBufferKey tmp;
		tmp.mLoader =	tr1::bind(&sf::SoundBuffer::loadFromMemory, _1, data, size);
		tmp.mKey =		detail::Tagger("Memory") << data << size << tag;
		return tmp;
	}

	SoundBufferKey SoundBufferKey::fromSamples(const sf::Int16* samples, std::size_t samplesCount, unsigned int channelsCount,
		unsigned int sampleRate, const std::string& tag)
	{
		SoundBufferKey tmp;
		tmp.mLoader =	tr1::bind(&sf::SoundBuffer::loadFromSamples, _1, samples, samplesCount, channelsCount, sampleRate);
		tmp.mKey =		detail::Tagger("Samples") << samples << samplesCount << channelsCount << sampleRate << tag;
		return tmp;
	}

	SoundBufferKey SoundBufferKey::fromStream(sf::InputStream& stream, const std::string& tag)
	{
		SoundBufferKey tmp;
		tmp.mLoader =	tr1::bind(&sf::SoundBuffer::loadFromStream, _1, tr1::ref(stream));
		tmp.mKey =		detail::Tagger("Stream") << &stream << tag;
		return tmp;
	}

	aur::CopiedPtr<sf::SoundBuffer> SoundBufferKey::load() const
	{
		return detail::LoadResource(mLoader);
	}

	void SoundBufferKey::swap(SoundBufferKey& other)
	{
		std::tr1::swap(mLoader, other.mLoader);
		std::swap(mKey, other.mKey);
	}

	bool operator< (const SoundBufferKey& lhs, const SoundBufferKey& rhs)
	{
		return detail::getKeyInfo(lhs) < detail::getKeyInfo(rhs);
	}

	// ---------------------------------------------------------------------------------------------------------------------------


	MusicKey::MusicKey()
	{
	}

	MusicKey MusicKey::fromFile(const std::string& fileName, const std::string& tag)
	{
		MusicKey tmp;
		tmp.mLoader =	tr1::bind(&sf::Music::openFromFile, _1, fileName);
		tmp.mKey =		detail::Tagger("File") << fileName << tag;
		return tmp;
	}

	MusicKey MusicKey::fromMemory(const void* data, std::size_t size, const std::string& tag)
	{
		MusicKey tmp;
		tmp.mLoader =	tr1::bind(&sf::Music::openFromMemory, _1, data, size);
		tmp.mKey =		detail::Tagger("Memory") << data << size << tag;
		return tmp;
	}

	MusicKey MusicKey::fromStream(sf::InputStream& stream, const std::string& tag)
	{
		MusicKey tmp;
		tmp.mLoader =	tr1::bind(&sf::Music::openFromStream, _1, tr1::ref(stream));
		tmp.mKey =		detail::Tagger("Stream") << &stream << tag;
		return tmp;
	}

	aur::CopiedPtr<sf::Music> MusicKey::load() const
	{
		return detail::LoadResource(mLoader);
	}

	void MusicKey::swap(MusicKey& other)
	{
		std::tr1::swap(mLoader, other.mLoader);
		std::swap(mKey, other.mKey);
	}

	bool operator< (const MusicKey& lhs, const MusicKey& rhs)
	{
		return detail::getKeyInfo(lhs) < detail::getKeyInfo(rhs);
	}

} // namespace Resources
} // namespace thor
