/////////////////////////////////////////////////////////////////////////////////
//
// Thor C++ Library
// Copyright (c) 2011-2015 Jan Haller
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
/// @brief Functions to create resource loaders for SFML

#ifndef THOR_SFMLLOADERS_HPP
#define THOR_SFMLLOADERS_HPP

#include <Thor/Resources/ResourceLoader.hpp>
#include <Thor/Resources/Detail/ResourceLoaderHelpers.hpp>

#include <Aurora/Meta/Templates.hpp>


namespace thor
{

/// @addtogroup Resources
/// @{

/// @brief Namespace for resource-related functionality
/// 
namespace Resources
{
	// Make _1, _2 etc. known within this namespace
	using namespace std::placeholders;

	/// @brief Load the resource from a file on hard disk.
	/// @param filename The name of the file from which you want to load the resource.
	/// @return Resource loader which is going to invoke <i>loadFromFile(filename)</i>.
	template <class R>
	ResourceLoader<R> fromFile(const std::string& filename)
	{
		return detail::makeResourceLoader<R>(
			[=] (R& resource) { return resource.loadFromFile(filename); },
			detail::Tagger("File") << filename);
	}

	/// @brief Load the resource from a file on hard disk.
	/// @param filename The name of the file from which you want to load the resource.
	/// @param arg1 An additional argument (for example sf::IntRect at sf::Texture or std::string / sf::Shader::Type at sf::Shader).
	/// @return Resource loader which is going to invoke <i>loadFromFile(filename, arg1)</i>.
	template <class R, typename T>
	ResourceLoader<R> fromFile(const std::string& filename, T arg1)
	{
		return detail::makeResourceLoader<R>(
			[=] (R& resource) { return resource.loadFromFile(filename, arg1); },
			detail::Tagger("File") << filename << arg1);
	}

	/// @brief Load the resource from a file in memory.
	/// @param arg1 Usually <i>const void*</i> for a pointer in memory; alternatively <i>std::string</i> for shader.
	/// @param arg2 Usually <i>std::size_t</i> for the data length in bytes; alternatively <i>std::string</i> or <i>sf::Shader::Type</i> for shader.
	/// @return Resource loader which is going to invoke <i>loadFromMemory(arg1, arg2)</i>.
	template <class R, typename T, typename U>
	ResourceLoader<R> fromMemory(T arg1, U arg2)
	{
		return detail::makeResourceLoader<R>(
			[=] (R& resource) { return resource.loadFromMemory(arg1, arg2); },
			detail::Tagger("Memory") << arg1 << arg2);
	}

	/// @brief Load the resource from a file in memory.
	/// @param arg1 Usually <i>const void*</i> for a pointer in memory; alternatively <i>std::string</i> for shader.
	/// @param arg2 Usually <i>std::size_t</i> for the data length in bytes; alternatively <i>std::string</i> or <i>sf::Shader::Type</i> for shader.
	/// @param arg3 An additional argument (for example sf::IntRect at sf::Texture).
	/// @return Resource loader which is going to invoke <i>loadFromMemory(arg1, arg2, arg3)</i>.
	template <class R, typename T, typename U, typename V>
	ResourceLoader<R> fromMemory(T arg1, U arg2, V arg3)
	{
		return detail::makeResourceLoader<R>(
			[=] (R& resource) { return resource.loadFromMemory(arg1, arg2, arg3); },
			detail::Tagger("Memory") << arg1 << arg2 << arg3);
	}

	/// @brief Load the resource from an input stream.
	/// @param stream Source stream to read from.
	/// @return Resource loader which is going to invoke <i>loadFromStream(stream)</i>.
	template <class R>
	ResourceLoader<R> fromStream(sf::InputStream& stream)
	{
		return detail::makeResourceLoader<R>(
			[&] (R& resource) { return resource.loadFromStream(stream); },
			detail::Tagger("Stream") << &stream);
	}

	/// @brief Load the resource (usually sf::Shader) from two streams.
	/// @param vertexShaderStream Source stream to read the vertex shader from.
	/// @param fragmentShaderStream Source stream to read the fragment shader from.
	/// @return Resource loader which is going to invoke <i>loadFromStream(vertexShaderStream, fragmentShaderStream)</i>.
	template <class R>
	ResourceLoader<R> fromStream(sf::InputStream& vertexShaderStream, sf::InputStream& fragmentShaderStream)
	{
		return detail::makeResourceLoader<R>(
			[&] (R& resource) { return resource.loadFromStream(vertexShaderStream, fragmentShaderStream); },
			detail::Tagger("Stream") << &vertexShaderStream << &fragmentShaderStream);
	}

	/// @brief Load the resource from a stream with additional information.
	/// @param stream Source stream to read from.
	/// @param arg1 An additional argument (for example sf::IntRect at sf::Texture or std::string / sf::Shader::Type at sf::Shader).
	/// @return Resource loader which is going to invoke <i>loadFromStream(stream, arg1)</i>.
	template <class R, typename T>
	ResourceLoader<R> fromStream(sf::InputStream& stream, T arg1
		AURORA_ENABLE_IF(!std::is_base_of<sf::InputStream, T>::value))
	{
		return detail::makeResourceLoader<R>(
			[&stream, arg1] (R& resource) { return resource.loadFromStream(stream, arg1); },
			detail::Tagger("Stream") << &stream << arg1);
	}

	/// @brief Load sf::SoundBuffer from array of audio samples in memory
	/// @param samples Pointer to the samples array.
	/// @param sampleCount Number of samples in the array.
	/// @param channelCount Number of channels (1 = mono, 2 = stereo, ...).
	/// @param sampleRate Sample rate (number of samples to play per second).
	/// @return Resource loader which is going to invoke <i>loadFromSamples(width, height, color)</i>.
	template <class R>
	ResourceLoader<R> fromSamples(const sf::Int16* samples, std::size_t sampleCount, unsigned int channelCount, unsigned int sampleRate)
	{
		return detail::makeResourceLoader<R>(
			[=] (R& resource) { return resource.loadFromSamples(samples, sampleCount, channelCount, sampleRate); },
			detail::Tagger("Samples") << samples << sampleCount << channelCount << sampleRate);
	}

	/// @brief Load resource (usually sf::Image) from array of pixels
	/// @param width,height The size of the image.
	/// @param pixels Pointer to the pixels in memory.
	/// @return Resource loader which is going to invoke <i>create(width, height, pixels)</i>.
	template <class R>
	ResourceLoader<R> fromPixels(unsigned int width, unsigned int height, const sf::Uint8* pixels)
	{
		return detail::makeResourceLoader<R>(
			[=] (R& resource) -> bool
			{
				resource.create(width, height, pixels);
				return true;
			},
			detail::Tagger("Pixels") << width << height << pixels);
	}

	/// @brief Load resource (usually sf::Image) from size and fill color
	/// @param width,height The size of the image.
	/// @param color The color used to fill the image.
	/// @return Resource loader which is going to invoke <i>create(width, height, color)</i>.
	template <class R>
	ResourceLoader<R> fromColor(unsigned int width, unsigned int height, const sf::Color& color)
	{
		return detail::makeResourceLoader<R>(
			[=] (R& resource) -> bool
			{
				resource.create(width, height, color);
				return true;
			},
			detail::Tagger("Color") << width << height << color);
	}

	/// @brief Load the resource (usually sf::Texture) from a sf::Image.
	/// @param image Image to load into the texture.
	/// @param area Area of the image to load.
	/// @return Resource loader which is going to invoke <i>loadFromImage(image, area)</i>.
	template <class R>
	ResourceLoader<R> fromImage(const sf::Image& image, const sf::IntRect area = sf::IntRect())
	{
		return detail::makeResourceLoader<R>(
			[&image, area] (R& resource) { return resource.loadFromImage(image, area); },
			detail::Tagger("Image") << &image << area);
	}

} // namespace Resources

/// @}

} // namespace thor

#endif // THOR_SFMLLOADERS_HPP
