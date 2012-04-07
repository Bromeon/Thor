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
/// @brief Class template thor::Resources::KeyTraits

#ifndef THOR_RESOURCEKEYTRAITS_HPP
#define THOR_RESOURCEKEYTRAITS_HPP

#include <Thor/Config.hpp>

#include <Aurora/Tools/Detail/Metaprogramming.hpp>

#include <string>


namespace sf
{

	class Image;
	class Texture;
	class Font;
	class Shader;
	class Music;
	class SoundBuffer;

	class InputStream;

} // namespace sf


namespace thor
{	
namespace Resources
{

	class ImageKey;
	class TextureKey;
	class FontKey;
	class ShaderKey;
	class SoundBufferKey;
	class MusicKey;

} // namespace Resources
} // namespace thor

// ---------------------------------------------------------------------------------------------------------------------------


namespace thor
{

/// @addtogroup Resources
/// @{

namespace Resources
{

	/// @struct thor::Resources::KeyTraits
	/// @brief Maps common SFML resources to ResourceManager keys.
	/// @details Used by ResourceManager. If its @a ResourceKey template argument isn't specified,
	///  it looks up the default key corresponding to the first argument @a Resource.
	/// @n@n You can use the macro @ref THOR_MAP_RESOURCEKEY to create a specialization.
	/// Five specialization of the traits template are already provided:
	/// <table>
	///  <tr><th>SFML resource</th>		<th>Thor resource key</th>				</tr>
	///	 <tr><td>sf::Image</td>			<td>thor::Resources::ImageKey</td>		</tr>
	///	 <tr><td>sf::Texture</td>		<td>thor::Resources::TextureKey</td>	</tr>
	///  <tr><td>sf::Font</td>			<td>thor::Resources::FontKey</td>		</tr>
	///  <tr><td>sf::Shader</td>		<td>thor::Resources::ShaderKey</td>		</tr>
	///  <tr><td>sf::SoundBuffer</td>	<td>thor::Resources::SoundBufferKey</td></tr>
	///  <tr><td>sf::Music</td>			<td>thor::Resources::MusicKey</td>		</tr>
	template <class Resource>
	struct KeyTraits;

} // namespace Resources


/// @brief Macro that maps resources to their default resource key
/// @details This macro creates a specialization of thor::Resources::KeyTraits.
///  You can use the macro in the global namespace to define a default ResourceKey for a given Resource
///  class. Then, you don't have to specify the thor::ResourceManager's second template parameter anymore.
/// @hideinitializer
#define THOR_MAP_RESOURCEKEY(Resource, ResourceKey)		\
namespace thor											\
{														\
namespace Resources										\
{														\
	template <>											\
	struct KeyTraits< Resource >						\
	{													\
		typedef ResourceKey Type;						\
	};													\
} 														\
} 

/// @}

} // namespace thor

// Map SFML resources to built-in keys
THOR_MAP_RESOURCEKEY(sf::Image,			ImageKey)
THOR_MAP_RESOURCEKEY(sf::Texture,		TextureKey)
THOR_MAP_RESOURCEKEY(sf::Font,			FontKey)
THOR_MAP_RESOURCEKEY(sf::Shader,		ShaderKey)
THOR_MAP_RESOURCEKEY(sf::SoundBuffer,	SoundBufferKey)
THOR_MAP_RESOURCEKEY(sf::Music,			MusicKey)

// ---------------------------------------------------------------------------------------------------------------------------


// Helper functions to access key strings
namespace thor
{
namespace detail
{
	// Class to access the strings of resource keys
	struct KeyInfoAccessor
	{
		// Resource key string accessors for built-in keys
		template <class Key>
		static const std::string& get(const Key& key, aur::detail::Int2Type<true>)
		{
			return key.mKey;
		}

		// Resource key string accessors for other keys
		template <class Key>
		static const std::string& get(const Key&, aur::detail::Int2Type<false>)
		{
			static const std::string emptyString = "";
			return emptyString;
		}
	};

	// Access the resource key string
	template <class Key>
	const std::string& getKeyInfo(const Key& key)
	{
		using std::is_same;

		const bool builtin =
			is_same<Key, Resources::ImageKey>::value
		 || is_same<Key, Resources::TextureKey>::value
		 || is_same<Key, Resources::FontKey>::value
		 || is_same<Key, Resources::ShaderKey>::value
		 || is_same<Key, Resources::SoundBufferKey>::value
		 || is_same<Key, Resources::MusicKey>::value;

		return KeyInfoAccessor::get(key, aur::detail::Int2Type<builtin>());
	}

} // namespace detail
} // namespace thor

#endif // THOR_RESOURCEKEYTRAITS_HPP
