/*
    libmaus2
    Copyright (C) 2009-2013 German Tischler
    Copyright (C) 2011-2013 Genome Research Limited

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#if ! defined(LIBMAUS2_FASTX_STREAMFASTAREADERWRAPPER_HPP)
#define LIBMAUS2_FASTX_STREAMFASTAREADERWRAPPER_HPP

#include <libmaus2/fastx/FastAReader.hpp>
#include <libmaus2/fastx/StreamFastReaderBase.hpp>

namespace libmaus2
{
	namespace fastx
	{
                typedef FastAReaderTemplate< ::libmaus2::fastx::StreamFastReaderBase > StreamFastAReader;

                struct StreamFastAReaderWrapper : public StreamFastAReader
                {
                	StreamFastAReaderWrapper(std::istream & in) : StreamFastAReader(&in) {}
                	StreamFastAReaderWrapper(std::istream & in, libmaus2::fastx::FastInterval const & FI) : StreamFastAReader(&in,FI) {}
                };
	}
}
#endif
