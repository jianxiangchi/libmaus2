/*
    libmaus2
    Copyright (C) 2009-2014 German Tischler
    Copyright (C) 2011-2014 Genome Research Limited

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
#if ! defined(LIBMAUS2_BAMBAM_PARALLEL_COMPRESSIONPENDINGELEMENT_HPP)
#define LIBMAUS2_BAMBAM_PARALLEL_COMPRESSIONPENDINGELEMENT_HPP

#include <libmaus2/bambam/parallel/AlignmentRewriteBuffer.hpp>

namespace libmaus2
{
	namespace bambam
	{
		namespace parallel
		{
			struct CompressionPendingElement
			{
				AlignmentRewriteBuffer * buffer;
				uint64_t blockid;
				uint64_t subid;
				uint64_t totalsubids;

				CompressionPendingElement() : buffer(0), blockid(0), subid(0), totalsubids(0) {}
				CompressionPendingElement(
					AlignmentRewriteBuffer * rbuffer,
					uint64_t const rblockid,
					uint64_t const rsubid,
					uint64_t const rtotalsubids
				) : buffer(rbuffer), blockid(rblockid), subid(rsubid), totalsubids(rtotalsubids)
				{

				}
			};
		}
	}
}
#endif
