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

#include <Thor/Input/Connection.hpp>
#include <Thor/Input/Detail/ConnectionImpl.hpp>


namespace thor
{

Connection::Connection()
: mWeakRef()
{
}

Connection::Connection(std::weak_ptr<detail::AbstractConnectionImpl> tracker)
: mWeakRef(std::move(tracker))
{
}

bool Connection::isConnected() const
{
	return !mWeakRef.expired();
}

void Connection::invalidate()
{
	mWeakRef.reset();
}

void Connection::disconnect()
{
	if (auto shared = mWeakRef.lock())
	{
		shared->disconnect();
		invalidate();
	}
}

// ---------------------------------------------------------------------------------------------------------------------------


ScopedConnection::ScopedConnection()
: mConnection()
{
}

ScopedConnection::ScopedConnection(const Connection& connection)
: mConnection(connection)
{
}

ScopedConnection::ScopedConnection(ScopedConnection&& source)
: mConnection(source.mConnection)
{
	source.mConnection.invalidate();
}

ScopedConnection& ScopedConnection::operator= (ScopedConnection&& source)
{
	mConnection = source.mConnection;
	source.mConnection.invalidate();

	return *this;
}

ScopedConnection::~ScopedConnection()
{
	mConnection.disconnect();
}

bool ScopedConnection::isConnected() const
{
	return mConnection.isConnected();
}

} // namespace thor
