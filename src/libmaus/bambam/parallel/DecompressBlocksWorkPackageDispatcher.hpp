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
#if ! defined(LIBMAUS_BAMBAM_PARALLEL_DECOMPRESSBLOCKSWORKPACKAGEDISPATCHER_HPP)
#define LIBMAUS_BAMBAM_PARALLEL_DECOMPRESSBLOCKSWORKPACKAGEDISPATCHER_HPP

#include <libmaus/parallel/SimpleThreadWorkPackageDispatcher.hpp>
#include <libmaus/bambam/parallel/DecompressBlocksWorkPackageReturnInterface.hpp>
#include <libmaus/bambam/parallel/InputBlockReturnInterface.hpp>
#include <libmaus/bambam/parallel/DecompressedBlockAddPendingInterface.hpp>
#include <libmaus/bambam/parallel/BgzfInflateZStreamBaseReturnInterface.hpp>
#include <libmaus/bambam/parallel/BgzfInflateZStreamBaseGetInterface.hpp>

namespace libmaus
{
	namespace bambam
	{
		namespace parallel
		{
			// dispatcher for block decompression
			struct DecompressBlocksWorkPackageDispatcher : public libmaus::parallel::SimpleThreadWorkPackageDispatcher
			{
				DecompressBlocksWorkPackageReturnInterface & packageReturnInterface;
				InputBlockReturnInterface & inputBlockReturnInterface;
				DecompressedBlockAddPendingInterface & decompressedBlockPendingInterface;
				BgzfInflateZStreamBaseReturnInterface & decoderReturnInterface;
				BgzfInflateZStreamBaseGetInterface & decoderGetInterface;
	
				DecompressBlocksWorkPackageDispatcher(
					DecompressBlocksWorkPackageReturnInterface & rpackageReturnInterface,
					InputBlockReturnInterface & rinputBlockReturnInterface,
					DecompressedBlockAddPendingInterface & rdecompressedBlockPendingInterface,
					BgzfInflateZStreamBaseReturnInterface & rdecoderReturnInterface,
					BgzfInflateZStreamBaseGetInterface & rdecoderGetInterface
	
				) : packageReturnInterface(rpackageReturnInterface), inputBlockReturnInterface(rinputBlockReturnInterface),
				    decompressedBlockPendingInterface(rdecompressedBlockPendingInterface), decoderReturnInterface(rdecoderReturnInterface),
				    decoderGetInterface(rdecoderGetInterface)
				{
				
				}
			
				virtual void dispatch(
					libmaus::parallel::SimpleThreadWorkPackage * P, 
					libmaus::parallel::SimpleThreadPoolInterfaceEnqueTermInterface & tpi
				)
				{
					DecompressBlocksWorkPackage * BP = dynamic_cast<DecompressBlocksWorkPackage *>(P);
					assert ( BP );
					
					assert ( BP->inputblocks.size() == BP->outputblocks.size() );
					
					libmaus::lz::BgzfInflateZStreamBase * zdecoder = decoderGetInterface.getBgzfInflateZStreamBase();
					
					for ( uint64_t z = 0; z < BP->inputblocks.size(); ++z )
					{
						// decompress the block
						BP->outputblocks[z]->decompressBlock(zdecoder,BP->inputblocks[z]);
					
						// compute crc of uncompressed data
						uint32_t const crc = BP->outputblocks[z]->computeCrc();

						// check crc
						if ( crc != BP->inputblocks[z]->crc )
						{
							tpi.getGlobalLock().lock();
							std::cerr << "crc failed for block " << BP->inputblocks[z]->blockid 
								<< " expecting " << std::hex << BP->inputblocks[z]->crc << std::dec
								<< " got " << std::hex << crc << std::dec << std::endl;
							tpi.getGlobalLock().unlock();
						
							libmaus::exception::LibMausException lme;
							lme.getStream() << "DecompressBlocksWorkPackageDispatcher: corrupt input data (crc mismatch)\n";
							lme.finish();
							throw lme;
						}

						// set stream and block id
						BP->outputblocks[z]->streamid = BP->inputblocks[z]->streamid;
						BP->outputblocks[z]->blockid  = BP->inputblocks[z]->blockid;

						// return input block
						inputBlockReturnInterface.putInputBlockReturn(BP->inputblocks[z]);
						// mark output block as pending
						decompressedBlockPendingInterface.putDecompressedBlockAddPending(BP->outputblocks[z]);
					}

					// return zstream base (decompressor)
					decoderReturnInterface.putBgzfInflateZStreamBaseReturn(zdecoder);
					// return work meta package
					packageReturnInterface.putDecompressBlocksWorkPackage(BP);
				}		
			};
		}
	}
}
#endif
