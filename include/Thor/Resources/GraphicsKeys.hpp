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
/// @brief Resource keys for SFML Graphics module

#ifndef THOR_GRAPHICSKEYS_HPP
#define THOR_GRAPHICSKEYS_HPP

#include <Thor/Resources/ResourceKeyTraits.hpp>
#include <Thor/Config.hpp>

#include <Aurora/Tools/Swap.hpp>
#include <Aurora/SmartPtr/CopiedPtr.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Config.hpp>

#include AURORA_TR1_HEADER(functional)
#include <string>


namespace thor
{

/// @addtogroup Resources
/// @{

/// @brief Namespace for resource-specific functionality
/// @details Contains resource-related functionality that is a bit more specific than ResourceManager and ResourcePtr.
namespace Resources
{

	/// @brief Predefined key class for sf::Image resources
	/// @details You can use this class to specifiy the access and loading of sf::Image resources in a ResourceManager.
	class THOR_API ImageKey
	{
		// ---------------------------------------------------------------------------------------------------------------------------
		// Public member functions
		public:
			/// @brief Load sf::Image from size and fill color
			/// @param width,height The size of the image.
			/// @param fillColor The color used to fill the image.
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Image::create()
			static ImageKey				fromSize(unsigned int width, unsigned int height, const sf::Color& fillColor = sf::Color(0, 0, 0),
											const std::string& tag = "");

			/// @brief Load sf::Image from file name
			/// @param fileName The name of the file from which you want to load the sf::Image.
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Image::loadFromFile()
			static ImageKey				fromFile(const std::string& fileName, const std::string& tag = "");

			/// @brief Load sf::Image from file in memory
			/// @param data Pointer to the file data in memory.
			/// @param size Size of the data to load, in bytes. 
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Image::loadFromMemory()
			static ImageKey				fromMemory(const void* data, std::size_t size, const std::string& tag = "");

			/// @brief Load sf::Image from array of pixels
			/// @param width,height The size of the image.
			/// @param pixels Pointer to the pixels in memory.
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Image::create()
			static ImageKey				fromPixels(unsigned int width, unsigned int height, const sf::Uint8* pixels,
											const std::string& tag = "");

			/// @brief Load sf::Image from input stream
			/// @param stream Source stream to read from.
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Image::loadFromStream()
			static ImageKey				fromStream(sf::InputStream& stream, const std::string& tag = "");

			/// @brief Loads the image as specified by the static functions.
			/// @return Movable smart pointer to the allocated sf::Image, or null pointer at loading failure.
			aur::CopiedPtr<sf::Image>	load() const;

			/// @brief Exchanges the contents of @a *this with @a other.
			///
			void						swap(ImageKey& other);


		// ---------------------------------------------------------------------------------------------------------------------------
		// Private member functions
		private:
			// Make default constructor private
										ImageKey();

		
		// ---------------------------------------------------------------------------------------------------------------------------
		// Private variables
		private:
			std::tr1::function<bool(sf::Image&)>	mLoader;
			std::string								mKey;

	
		// ---------------------------------------------------------------------------------------------------------------------------
		// Friends
		friend struct detail::KeyInfoAccessor;
	};


	/// @brief Predefined key class for sf::Texture resources
	/// @details You can use this class to specifiy the access and loading of sf::Texture resources in a ResourceManager.
	class THOR_API TextureKey
	{
		// ---------------------------------------------------------------------------------------------------------------------------
		// Public member functions
		public:
			/// @brief Load sf::Texture from size
			/// @param width,height The size of the texture.
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Texture::create()
			static TextureKey			fromSize(unsigned int width, unsigned int height, const std::string& tag = "");

			/// @brief Load sf::Texture from file name
			/// @param fileName The name of the file from which you want to load the sf::Texture.
			/// @param area Sub-rectangle of the image to load.
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Texture::loadFromFile()
			static TextureKey			fromFile(const std::string& fileName, const sf::IntRect& area = sf::IntRect(),
											const std::string& tag = "");

			/// @brief Load sf::Texture from file in memory
			/// @param data Pointer to the file data in memory.
			/// @param size Size of the data to load, in bytes. 
			/// @param area Sub-rectangle of the image to load.
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Texture::loadFromMemory()
			static TextureKey			fromMemory(const void* data, std::size_t size, const sf::IntRect& area = sf::IntRect(),
											const std::string& tag = "");

			/// @brief Load sf::Texture from an existing sf::Image
			/// @param image Image containing the pixels that are copied to the texture.
			/// @param area Sub-rectangle of the image to load.
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Texture::loadFromImage()
			static TextureKey			fromImage(const sf::Image& image, const sf::IntRect& area = sf::IntRect(),
											const std::string& tag = "");

			/// @brief Load sf::Texture from input stream
			/// @param stream Source stream to read from.
			/// @param area Sub-rectangle of the image to load.
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Texture::loadFromStream()
			static TextureKey			fromStream(sf::InputStream& stream, const sf::IntRect& area = sf::IntRect(), 
											const std::string& tag = "");

			/// @brief Loads the image as specified by the static functions.
			/// @return Movable smart pointer to the allocated sf::Texture, or null pointer at loading failure.
			aur::CopiedPtr<sf::Texture>	load() const;

			/// @brief Exchanges the contents of @a *this with @a other.
			///
			void						swap(TextureKey& other);


		// ---------------------------------------------------------------------------------------------------------------------------
		// Private member functions
		private:
			// Make default constructor private
										TextureKey();

		
		// ---------------------------------------------------------------------------------------------------------------------------
		// Private variables
		private:
			std::tr1::function<bool(sf::Texture&)>	mLoader;
			std::string								mKey;

	
		// ---------------------------------------------------------------------------------------------------------------------------
		// Friends
		friend struct detail::KeyInfoAccessor;
	};


	/// @brief Predefined key class for sf::Font resources
	/// @details You can use this class to specifiy the access and loading of sf::Font resources in a ResourceManager.
	class THOR_API FontKey
	{
		// ---------------------------------------------------------------------------------------------------------------------------
		// Public member functions
		public:
			/// @brief Load sf::Font from file name
			/// @param fileName The name of the file from which you want to load the sf::Font.
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Font::loadFromFile()
			static FontKey				fromFile(const std::string& fileName, const std::string& tag = "");

			/// @brief Load sf::Font from file in memory
			/// @param data Pointer to the file data in memory.
			/// @param size Size of the data to load, in bytes. 
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Font::loadFromMemory()
			static FontKey				fromMemory(const void* data, std::size_t size, const std::string& tag = "");

			/// @brief Load sf::Font from input stream
			/// @param stream Source stream to read from.
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Font::loadFromStream()
			static FontKey				fromStream(sf::InputStream& stream, const std::string& tag = "");

			/// @brief Loads the font as specified by the static functions.
			/// @details This function is mainly provided for ResourceManager, you don't need to call it directly.
			/// @return Movable smart pointer to the allocated sf::Font, or null pointer at loading failure.
			aur::CopiedPtr<sf::Font>	load() const;

			/// @brief Exchanges the contents of @a *this with @a other.
			///
			void						swap(FontKey& other);


		// ---------------------------------------------------------------------------------------------------------------------------
		// Private member functions
		private:
			// Make default constructor private
										FontKey();

		
		// ---------------------------------------------------------------------------------------------------------------------------
		// Private variables
		private:
			std::tr1::function<bool(sf::Font&)>		mLoader;
			std::string								mKey;

	
		// ---------------------------------------------------------------------------------------------------------------------------
		// Friends
		friend struct detail::KeyInfoAccessor;
	};


	/// @brief Predefined key class for sf::Shader resources
	/// @details You can use this class to specifiy the access and loading of sf::Shader resources in a ResourceManager.
	class THOR_API ShaderKey
	{
		// ---------------------------------------------------------------------------------------------------------------------------
		// Public member functions
		public:
			/// @brief Load sf::Shader from file name
			/// @param fileName The name of the file from which you want to load the sf::Shader.
			/// @param shaderType Type of shader (vertex or fragment)
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Shader::loadFromFile()
			static ShaderKey			fromFile(const std::string& fileName, AURORA_FAKE_DOC(unsigned int, sf::Shader::Type) shaderType, const std::string& tag = "");

			/// @brief Load sf::Shader from two file names
			/// @param vertexShaderFilename The name of the file from which you want to load the vertex shader.
			/// @param fragmentShaderFilename The name of the file from which you want to load the fragment shader.
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Shader::loadFromFile()
			static ShaderKey			fromFile(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename, const std::string& tag = "");

			/// @brief Load sf::Shader from source code in memory
			/// @param shaderCode String containing the source code of the shader.
			/// @param shaderType Type of shader (vertex or fragment)
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Shader::loadFromMemory()
			static ShaderKey			fromMemory(const std::string& shaderCode, AURORA_FAKE_DOC(unsigned int, sf::Shader::Type) shaderType, const std::string& tag = "");

			/// @brief Load sf::Shader from two source codes in memory
			/// @param vertexShaderCode String containing the source code of the vertex shader.
			/// @param fragmentShaderCode String containing the source code of the fragment shader.
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Shader::loadFromMemory()
			static ShaderKey			fromMemory(const std::string& vertexShaderCode, const std::string& fragmentShaderCode, const std::string& tag = "");
			
			/// @brief Load sf::Shader from input stream
			/// @param stream Source stream to read from.
			/// @param shaderType Type of shader (vertex or fragment)
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Shader::loadFromStream()
			static ShaderKey			fromStream(sf::InputStream& stream, AURORA_FAKE_DOC(unsigned int, sf::Shader::Type) shaderType, const std::string& tag = "");

			/// @brief Load sf::Shader from two input streams
			/// @param vertexShaderStream Source stream to read the vertex shader from.
			/// @param fragmentShaderStream Source stream to read the fragment shader from.
			/// @param tag If an existing key has the same loading parameters, you can use this ID to create a distinct key.
			/// @see sf::Shader::loadFromStream()
			static ShaderKey			fromStream(sf::InputStream& vertexShaderStream, sf::InputStream& fragmentShaderStream, const std::string& tag = "");

			/// @brief Loads the shader as specified by the static functions.
			/// @return Movable smart pointer to the allocated sf::Shader, or null pointer at loading failure.
			aur::CopiedPtr<sf::Shader> load() const;

			/// @brief Exchanges the contents of @a *this with @a other.
			///
			void						swap(ShaderKey& other);


		// ---------------------------------------------------------------------------------------------------------------------------
		// Private member functions
		private:
			// Make default constructor private
										ShaderKey();

		
		// ---------------------------------------------------------------------------------------------------------------------------
		// Private variables
		private:
			std::tr1::function<bool(sf::Shader&)>	mLoader;
			std::string								mKey;

	
		// ---------------------------------------------------------------------------------------------------------------------------
		// Friends
		friend struct detail::KeyInfoAccessor;
	};


	/// @relates ImageKey 
	/// @brief Comparison operator for strict weak ordering
	bool THOR_API				operator< (const ImageKey& lhs, const ImageKey& rhs);

	/// @relates TextureKey 
	/// @brief Comparison operator for strict weak ordering
	bool THOR_API				operator< (const TextureKey& lhs, const TextureKey& rhs);

	/// @relates FontKey 
	/// @brief Comparison operator for strict weak ordering
	bool THOR_API				operator< (const FontKey& lhs, const FontKey& rhs);

	/// @relates ShaderKey 
	/// @brief Comparison operator for strict weak ordering
	bool THOR_API				operator< (const ShaderKey& lhs, const ShaderKey& rhs);


	/// @relates ImageKey 
	/// @brief Swaps two sf::Image resource keys.
	AURORA_GLOBAL_SWAP(ImageKey)

	/// @relates TextureKey 
	/// @brief Swaps two sf::Texture resource keys.
	AURORA_GLOBAL_SWAP(TextureKey)

	/// @relates FontKey
	/// @brief Swaps two sf::Font resource keys.
	AURORA_GLOBAL_SWAP(FontKey)

	/// @relates ShaderKey 
	/// @brief Swaps two sf::Shader resource keys.
	AURORA_GLOBAL_SWAP(ShaderKey)

} // namespace Resources

/// @}

} // namespace thor

#endif // THOR_GRAPHICSKEYS_HPP
