/*
    libmaus
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
#if ! defined(LIBMAUS_PARALLEL_LOCKEDFREELIST_HPP)
#define LIBMAUS_PARALLEL_LOCKEDFREELIST_HPP

#include <libmaus/util/FreeList.hpp>

namespace libmaus
{
	namespace parallel
	{
		template<typename _element_type, typename _allocator_type = libmaus::util::FreeListDefaultAllocator<_element_type> >
		struct LockedFreeList : private libmaus::util::FreeList<_element_type,_allocator_type>
		{
			typedef _element_type element_type;
			typedef _allocator_type allocator_type;
			typedef libmaus::util::FreeList<element_type,allocator_type> base_type;
			typedef LockedFreeList<element_type,allocator_type> this_type;
			typedef typename libmaus::util::unique_ptr<this_type>::type unique_ptr_type;
			typedef typename libmaus::util::shared_ptr<this_type>::type shared_ptr_type;

			libmaus::parallel::PosixSpinLock lock;

			LockedFreeList(uint64_t const numel, allocator_type allocator = allocator_type()) : base_type(numel,allocator)
			{
			}
			
			virtual ~LockedFreeList()
			{
			}
			
			bool empty()
			{
				libmaus::parallel::ScopePosixSpinLock slock(lock);
				return base_type::empty();
			}
			
			element_type * get()
			{
				libmaus::parallel::ScopePosixSpinLock slock(lock);
				return base_type::get();
			}

			element_type * getIf()
			{
				libmaus::parallel::ScopePosixSpinLock slock(lock);
				
				if ( base_type::empty() )
					return 0;
				else
					return base_type::get();
			}
			
			void put(element_type * ptr)
			{
				libmaus::parallel::ScopePosixSpinLock slock(lock);
				base_type::put(ptr);
			}
		};
	}
}
#endif
