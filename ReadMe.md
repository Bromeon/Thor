![Thor C++ Library](http://www.bromeon.ch/libraries/thor/thor.png)

# Thor C++ Library

Thor is an open-source and cross-platform C++ library. It extends the multimedia library SFML with higher-level features such as:

* Animations
* Particle systems
* Resource management
* Time measurement utilities
* Event handling and callbacks
* Delaunay triangulation
* Color gradients
* Vector algebra
* ...

For a full list of features as well as tutorials and API documentation, visit the [project homepage](http://www.bromeon.ch/libraries/thor).

Thor uses the build system CMake and can be compiled for Visual C++, g++ and Clang compilers, as long as they provide at least partial C++11 support. Besides SFML, the library depends on my other project Aurora, which is shipped with Thor.



## License

Both Thor and Aurora are licensed under [zlib/libpng](http://opensource.org/licenses/zlib-license.php), which is very permissive. You can use the code in free and commercial products, open- or closed-source.
I would appreciate if you left a short note that you used one my libraries, but it's not required.



## Author and contact

Thor has been developed by Jan Haller since 2011.

* E-mail: bromeon@gmail.com
* Project homepage: http://www.bromeon.ch/libraries/thor
* GitHub repository: https://github.com/Bromeon/Thor



## Contributions

Thor and Aurora are not meant to be community projects in the classical sense, where dozens of people contribute with code patches. When I implement something new in my libraries, I spend most of the time thinking about the design -- writing code is just a small part, and usually the easy one. I consider it crucial that functionality is provided in the most simple, yet flexible way, and finding appropriate APIs takes time.

That is the main reason why pull requests, especially the way GitHub provides them, are of limited use to me. Another problem is that they are often not directly applicable, and even for simple things, it takes a lot of time to review them, test them locally and suggest corrections, possibly during multiple iterations. In many cases, the overhead is several times higher than if I'd write the code directly, which somehow defeats the point of helping me out. Since I'm also involved in the development of SFML, I have only few time left for my other open-source projects, and I would really like to spend it on the development of new features. Furthermore, people who submit a pull request often have a specific use case in mind and don't consider how it fits into the general library structure. I agree that this is difficult to know when you have not followed development for a longer time, which is another reason why I'd like to integrate modifications myself. Thus, please do not submit pull requests and expect me to merge them 1:1. Even for simple issues like space inconsistences, typos or other mistakes in the code, it's usually much faster to simply inform me.

I am always open for discussions about Thor and Aurora, and I highly appreciate suggestions concerning their API or implementation. If there is something that you would like to see implemented, just ask me -- I have already been able to improve many parts based on user ideas. You can contact me by e-mail or alternatively in the corresponding Thor/Aurora threads on the SFML forum.
