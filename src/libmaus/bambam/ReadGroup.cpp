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
#include <libmaus/bambam/ReadGroup.hpp>

std::ostream & operator<<(std::ostream & out, libmaus::bambam::ReadGroup const & RG)
{
	out << "ReadGroup(ID=" << RG.ID;
	
	for ( ::libmaus::util::unordered_map<std::string,std::string>::type::const_iterator ita = RG.M.begin();
		ita != RG.M.end(); ++ita )
		out << "," << ita->first << "=" << ita->second;
	
	out << ")";

	return out;
}
