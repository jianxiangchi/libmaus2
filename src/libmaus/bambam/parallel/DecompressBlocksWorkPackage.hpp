/*
    libmaus
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
#if ! defined(LIBMAUS_BAMBAM_PARALLEL_DECOMPRESSBLOCKSWORKPACKAGE_HPP)
#define LIBMAUS_BAMBAM_PARALLEL_DECOMPRESSBLOCKSWORKPACKAGE_HPP

#include <libmaus/bambam/parallel/ControlInputInfo.hpp>
#include <libmaus/bambam/parallel/DecompressedBlock.hpp>
#include <libmaus/parallel/SimpleThreadWorkPackage.hpp>

namespace libmaus
{
	namespace bambam
	{
		namespace parallel
		{
			struct DecompressBlocksWorkPackage : public libmaus::parallel::SimpleThreadWorkPackage
			{
				typedef DecompressBlocksWorkPackage this_type;
				typedef libmaus::util::unique_ptr<this_type>::type unique_ptr_type;
				typedef libmaus::util::shared_ptr<this_type>::type shared_ptr_type;

				std::vector<ControlInputInfo::input_block_type *> inputblocks;
				std::vector<DecompressedBlock *> outputblocks;
	
				DecompressBlocksWorkPackage() : libmaus::parallel::SimpleThreadWorkPackage() {}
				
				void setData(
					uint64_t const rpriority, 
					std::vector<ControlInputInfo::input_block_type *> & rinputblocks,
					std::vector<DecompressedBlock *> & routputblocks,
					uint64_t const rdecompressDispatcherId
				)
				{
					libmaus::parallel::SimpleThreadWorkPackage::priority = rpriority;
					
					if ( inputblocks.size() != rinputblocks.size() )
						inputblocks.resize(rinputblocks.size());
					if ( outputblocks.size() != routputblocks.size() )
						outputblocks.resize(routputblocks.size());
						
					std::copy(rinputblocks.begin(),rinputblocks.end(),inputblocks.begin());
					std::copy(routputblocks.begin(),routputblocks.end(),outputblocks.begin());
					
					libmaus::parallel::SimpleThreadWorkPackage::dispatcherid = rdecompressDispatcherId;
				}
			
				char const * getPackageName() const
				{
					return "DecompressBlocksWorkPackage";
				}
			};
		}
	}
}
#endif
