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
#if ! defined(LIBMAUS2_GAMMA_GAMMAENCODER_HPP)
#define LIBMAUS2_GAMMA_GAMMAENCODER_HPP

#include <libmaus2/gamma/GammaEncoderBase.hpp>
#include <libmaus2/bitio/Clz.hpp>
#include <libmaus2/util/unique_ptr.hpp>
#include <libmaus2/math/lowbits.hpp>
#include <algorithm>
#include <libmaus2/uint/uint.hpp>
#include <libmaus2/math/UnsignedInteger.hpp>
#include <libmaus2/util/Demangle.hpp>
#include <libmaus2/util/NumberToString.hpp>
#include <libmaus2/aio/SynchronousGenericOutput.hpp>

namespace libmaus2
{
	namespace gamma
	{
		template<typename _stream_type>
		struct GammaEncoder : public GammaEncoderBase<typename _stream_type::data_type>
		{
			typedef _stream_type stream_type;
			typedef typename stream_type::data_type stream_data_type;
			typedef GammaEncoder<stream_type> this_type;
			typedef typename ::libmaus2::util::unique_ptr<this_type>::type unique_ptr_type;

			stream_type & stream;
			stream_data_type v;
			unsigned int bav;

			GammaEncoder(stream_type & rstream)
			: stream(rstream), v(0), bav((CHAR_BIT*sizeof(stream_data_type)))
			{

			}

			inline void encodeWord(stream_data_type const code, unsigned int const codelen)
			{
				if ( bav >= codelen )
				{
					v <<= codelen;
					v |= code;
					bav -= codelen;
				}
				else
				{
					unsigned int const overflow = (codelen-bav);
					stream.put((v << bav) | (code >> overflow));
					v = code & libmaus2::math::LowBits<stream_data_type>::lowbits(overflow);
						// ((1ull << overflow)-1);
					bav = (CHAR_BIT*sizeof(stream_data_type))-overflow;
				}
			}

			std::string printNumber(stream_data_type q)
			{
				std::vector<char> digits;
				stream_data_type const base = 10ull;
				if ( q )
					while ( q )
					{
						digits.push_back(static_cast<uint64_t>(q % base));
						q /= base;
					}
				else
					digits.push_back(0);
				std::reverse(digits.begin(),digits.end());
				for ( uint64_t i = 0; i < digits.size(); ++i )
					digits[i] += '0';
				return std::string(digits.begin(),digits.end());
			}

			void encode(stream_data_type const q)
			{
				stream_data_type const code = q+static_cast<stream_data_type>(1);
				unsigned int codelen = GammaEncoderBase<stream_data_type>::getCodeLen(code);
				encodeWord(code,codelen);
			}

			void encodeSlow(stream_data_type const q)
			{
				stream_data_type const code = q+static_cast<stream_data_type>(1);
				assert ( code != stream_data_type() );
				unsigned int const nd = GammaEncoderBase<stream_data_type>::getLengthCode(code);
				encodeWord(0,nd);

				if ( expect_false(nd+1 == CHAR_BIT*sizeof(stream_data_type)) )
				{
					unsigned int const wordbits = CHAR_BIT*sizeof(stream_data_type);
					unsigned int const topbits = wordbits / 2;
					unsigned int const bottombits = wordbits - topbits;

					encodeWord(code >> bottombits, topbits);
					encodeWord(code & ((static_cast<stream_data_type>(1) << bottombits)-1), bottombits);
				}
				else
				{
					encodeWord(code,nd+1);
				}
			}

			void flush()
			{
				if ( bav != (CHAR_BIT*sizeof(stream_data_type)) )
				{
					v <<= bav;
					stream.put(v);
					v = 0;
					bav = (CHAR_BIT*sizeof(stream_data_type));
				}
			}

			uint64_t getOffset() const
			{
				return (CHAR_BIT*sizeof(stream_data_type)) * stream.getWrittenWords() + ((CHAR_BIT*sizeof(stream_data_type))-bav);
			}
		};
	}
}
#endif
