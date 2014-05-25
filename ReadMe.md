# Aurora C++ Library

Aurora is an open-source C++ library providing various rather uncommon C++ utilities. Features include:

* Named tuples
* Dynamic dispatch
* Ranges as iterator abstraction
* Smart pointers with deep-copy semantics
* Basic template and preprocessor metaprogramming
* ...

For a full list of features as well as tutorials and API documentation, visit the [project homepage](http://www.bromeon.ch/libraries/aurora).

Aurora is a header-only library that has no external dependencies, therefore it can be directly used in any project, without the need for a build or installation step. It requires partial C++11 support.

The Aurora library is used in my other project [Thor](https://github.com/Bromeon/Thor), an extension of the multimedia library SFML.



## License

Aurora is licensed under [zlib/libpng](http://opensource.org/licenses/zlib-license.php), which is very permissive. You can use the code in free and commercial products, open- or closed-source.
I would appreciate if you left a short note that you used one my libraries, but it's not required.



## Author and contact

Aurora has been developed by Jan Haller since 2012.

* E-mail: bromeon@gmail.com
* Project homepage: http://www.bromeon.ch/libraries/aurora
* GitHub repository: https://github.com/Bromeon/Aurora



## Contributions

Aurora is not meant to be a community project in the classical sense, where dozens of people contribute with code patches. When I implement something new in my libraries, I spend most of the time thinking about the design -- writing code is just a small part, and usually the easy one. I consider it crucial that functionality is provided in the most simple, yet flexible way, and finding appropriate APIs takes time.

That is the main reason why pull requests, especially the way GitHub provides them, are of limited use to me. Another problem is that they are often not directly applicable, and even for simple things, it takes a lot of time to review them, test them locally and suggest corrections, possibly during multiple iterations. In many cases, the overhead is several times higher than if I'd write the code directly, which somehow defeats the point of helping me out. Since I'm also involved in the development of SFML, I have only few time left for my other open-source projects, and I would really like to spend it on the development of new features. Furthermore, people who submit a pull request often have a specific use case in mind and don't consider how it fits into the general library structure. I agree that this is difficult to know when you have not followed development for a longer time, which is another reason why I'd like to integrate modifications myself. Thus, please do not submit pull requests and expect me to merge them 1:1. Even for simple issues like space inconsistences, typos or other mistakes in the code, it's usually much faster to simply inform me.

I am always open for discussions about Aurora, and I highly appreciate suggestions concerning its API or implementation. If there is something that you would like to see implemented, just ask me -- I have already been able to improve many parts based on user ideas. You can contact me by e-mail or alternatively in the corresponding Aurora thread on the SFML forum.
