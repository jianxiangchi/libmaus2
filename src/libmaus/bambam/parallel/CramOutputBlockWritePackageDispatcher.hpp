/*
    libmaus
    Copyright (C) 2009-2015 German Tischler
    Copyright (C) 2011-2015 Genome Research Limited

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
#if ! defined(LIBMAUS_BAMBAM_PARALLEL_CRAMOUTPUTBLOCKWRITEPACKAGEDISPATCHER_HPP)
#define LIBMAUS_BAMBAM_PARALLEL_CRAMOUTPUTBLOCKWRITEPACKAGEDISPATCHER_HPP

#include <libmaus/bambam/parallel/CramOutputBlockWritePackageFinishedInterface.hpp>
#include <libmaus/bambam/parallel/CramOutputBlockWritePackageReturnInterface.hpp>
#include <libmaus/parallel/SimpleThreadWorkPackageDispatcher.hpp>

namespace libmaus
{
	namespace bambam
	{
		namespace parallel
		{

			struct CramOutputBlockWritePackageDispatcher : public libmaus::parallel::SimpleThreadWorkPackageDispatcher
			{
				typedef CramOutputBlockWritePackageDispatcher this_type;
				typedef libmaus::util::unique_ptr<this_type>::type unique_ptr_type;
				typedef libmaus::util::shared_ptr<this_type>::type shared_ptr_type;
				
				CramOutputBlockWritePackageReturnInterface & packageReturnInterface;
				CramOutputBlockWritePackageFinishedInterface & finishedInterface;
						
				CramOutputBlockWritePackageDispatcher(
					CramOutputBlockWritePackageReturnInterface & rpackageReturnInterface,
					CramOutputBlockWritePackageFinishedInterface & rfinishedInterface
				) : packageReturnInterface(rpackageReturnInterface), finishedInterface(rfinishedInterface) {}
				~CramOutputBlockWritePackageDispatcher() {}
				void dispatch(libmaus::parallel::SimpleThreadWorkPackage * P, libmaus::parallel::SimpleThreadPoolInterfaceEnqueTermInterface & /* tpi */)
				{
					CramOutputBlockWritePackage * BP = dynamic_cast<CramOutputBlockWritePackage *>(P);

					CramOutputBlock::shared_ptr_type block = BP->block;
					char const * data = block->A->begin();
					size_t const n = block->fill;
					std::ostream * out = BP->out;
					
					out->write(data,n);
					if ( ! out )
					{
						libmaus::exception::LibMausException lme;
						lme.getStream() << "CramOutputBlockWritePackageDispatcher::dispatch: failed to write " << n << " bytes of data." << std::endl;
						lme.finish();
						throw lme;
					}

					finishedInterface.cramOutputBlockWritePackageFinished(block);
					packageReturnInterface.cramOutputBlockWritePackageReturn(BP);
				}
			};
		}
	}
}
#endif
