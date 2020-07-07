/*******************************************************************************
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Jean-David Gadina - www.xs-labs.com
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/

/*!
 * @header      BinaryStream.hpp
 * @copyright   (c) 2020, Jean-David Gadina - www.xs-labs.com
 */

#ifndef XS_IO_BINARY_STREAM_HPP
#define XS_IO_BINARY_STREAM_HPP

#include <string>
#include <cstdint>
#include <vector>
#include <type_traits>
#include <XS/Casts.hpp>

namespace XS
{
    namespace IO
    {
        class BinaryStream
        {
            public:
                
                enum class SeekDirection
                {
                    Current,
                    Begin,
                    End
                };
                
                enum class Endianness
                {
                    Default,
                    LittleEndian,
                    BigEndian
                };
                
                virtual ~BinaryStream() = default;
                
                virtual Endianness preferredEndianness()                const = 0;
                virtual void       setPreferredEndianness( Endianness value ) = 0;
                
                virtual void   read( uint8_t * buf, size_t size )        = 0;
                virtual size_t tell()                              const = 0;
                virtual void   seek( ssize_t offset, SeekDirection dir ) = 0;
                
                bool   hasBytesAvailable();
                size_t availableBytes();
                
                void seek( ssize_t offset );
                
                template< typename T, typename std::enable_if< std::is_integral< T >::value && std::is_unsigned< T >::value >::type * = nullptr >
                void seek( T offset )
                {
                    this->seek( numeric_cast< ssize_t >( offset ) );
                }
                
                template< typename T, typename std::enable_if< std::is_integral< T >::value && std::is_unsigned< T >::value >::type * = nullptr >
                void seek( T offset, SeekDirection dir )
                {
                    this->seek( numeric_cast< ssize_t >( offset ), dir );
                }
                
                std::vector< uint8_t > read( size_t size );
                std::vector< uint8_t > readAll();
                
                uint8_t readUInt8();
                int8_t  readInt8();
                
                uint16_t readUInt16();
                uint16_t readBigEndianUInt16();
                uint16_t readLittleEndianUInt16();
                
                uint32_t readUInt32();
                uint32_t readBigEndianUInt32();
                uint32_t readLittleEndianUInt32();
                
                uint64_t readUInt64();
                uint64_t readBigEndianUInt64();
                uint64_t readLittleEndianUInt64();
                
                float readBigEndianFixedPoint( unsigned int integerLength, unsigned int fractionalLength );
                float readLittleEndianFixedPoint( unsigned int integerLength, unsigned int fractionalLength );
                
                std::string readNULLTerminatedString();
                std::string readPascalString();
                std::string readString( size_t length );
        };
    }
}

#endif /* XS_IO_BINARY_STREAM_HPP */
