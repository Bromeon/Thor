/////////////////////////////////////////////////////////////////////////////////
//
// Thor C++ Library
// Copyright (c) 2011-2021 Jan Haller
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

// This header is for documentation purposes only.


/// @mainpage
///
/// @section welcome Welcome
/// Welcome to the documentation of the Thor Library. Here you'll find a reference for all types, functions, variables and macros
/// of the public API. 
///
/// If you are new to the library, you might want to go through the tutorials and the code examples first. The documentation is
/// the best choice to lookup specific functionality, however you can also use it to get an overview. In this case, 
/// I suggest to take especially a look at the Modules section, there you'll see an summary of each module. The other sections
/// are helpful to inform yourself about a specific class, function or namespace, but you're not going to see much of the context
/// in which it is used.
///
/// All functions, constants, namespaces, classes and other types which are part of the public API are located inside the namespace
/// @b thor (or one of the few nested namespaces). Everything inside namespace @b detail and all files in the Thor/Detail include directory
/// exist only for internal implementation. Don't use them. In case you come across something useful there, please let me know, so that
/// I can think about an integration to the API. Also don't hesitate to inform me about semantic or formal mistakes you spot in the
/// documentation, the tutorials or the examples.

/// @defgroup Animations Animations
/// @brief Classes able to animate graphical objects in different ways.

/// @defgroup Graphics Graphics
/// @brief Miscellaneous graphics-related functionality, such as specialized sprites or color helpers.

/// @defgroup Input Input
/// @brief Features related to user input, such as object-oriented event callbacks.

/// @defgroup Math Math
/// @brief Mathematical functionality, such as random number generator or trigonometric wrappers.

/// @defgroup Particles Particles
/// @brief Contains a particle system based on textures. The module also provides several affector and emitter classes.

/// @defgroup Resources Resources
/// @brief Provides a resource manager for various resource types (textures, fonts, sound buffers, ...).

/// @defgroup Shapes Shapes
/// @brief Extensions for sf::Shape, like concave shapes or predefined figures.

/// @defgroup Time Time
/// @brief Supplies classes to measure time. These are convenient wrappers around sf::Clock.

/// @defgroup Vectors Vectors
/// @brief A lot of operations based on sf::Vector2 and sf::Vector3. For specific overview, see VectorAlgebra2D.hpp and VectorAlgebra3D.hpp.
