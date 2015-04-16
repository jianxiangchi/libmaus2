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
#if ! defined(LIBMAUS_BAMBAM_PARALLEL_SAMENCODINGWORKPACKAGEDISPATCHER_HPP)
#define LIBMAUS_BAMBAM_PARALLEL_SAMENCODINGWORKPACKAGEDISPATCHER_HPP

#include <libmaus/parallel/SimpleThreadWorkPackageDispatcher.hpp>
#include <libmaus/bambam/parallel/SamEncodingWorkPackageWrapperReturnInterface.hpp>

namespace libmaus
{
	namespace bambam
	{
		namespace parallel
		{
			struct SamEncodingWorkPackageDispatcher : public libmaus::parallel::SimpleThreadWorkPackageDispatcher
			{
				typedef SamEncodingWorkPackageDispatcher this_type;
				typedef libmaus::util::unique_ptr<this_type>::type unique_ptr_type;
				typedef libmaus::util::shared_ptr<this_type>::type shared_ptr_type;
				
				SamEncodingWorkPackageWrapperReturnInterface & packageReturnInterface;
						
				SamEncodingWorkPackageDispatcher(
					SamEncodingWorkPackageWrapperReturnInterface & rpackageReturnInterface
				) : packageReturnInterface(rpackageReturnInterface) {}
				~SamEncodingWorkPackageDispatcher() {}
				void dispatch(libmaus::parallel::SimpleThreadWorkPackage * P, libmaus::parallel::SimpleThreadPoolInterfaceEnqueTermInterface & /* tpi */)
				{
					SamEncodingWorkPackageWrapper * BP = dynamic_cast<SamEncodingWorkPackageWrapper *>(P);
					BP->dispatch();
					packageReturnInterface.samEncodingWorkPackageWrapperReturn(BP);
				}
			};
		}
	}
}
#endif
