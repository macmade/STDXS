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
 * @file        BinaryDataStream.cpp
 * @copyright   (c) 2020, Jean-David Gadina - www.xs-labs.com
 */

#include <fstream>
#include <cmath>
#include <vector>
#include <XS/IO/BinaryDataStream.hpp>
#include <XS/Casts.hpp>

namespace XS
{
    namespace IO
    {
        class BinaryDataStream::IMPL
        {
            public:
                
                IMPL();
                IMPL( const std::vector< uint8_t > & data );
                IMPL( const IMPL & o );
                ~IMPL();
                
                std::vector< uint8_t > _data;
                size_t                 _pos;
                Endianness             _endianness;
        };
        
        BinaryDataStream::BinaryDataStream():
            impl( std::make_unique< IMPL >() )
        {}
        
        BinaryDataStream::BinaryDataStream( const std::vector< uint8_t > & data ):
            impl( std::make_unique< IMPL >( data ) )
        {}
        
        BinaryDataStream::BinaryDataStream( const BinaryDataStream & o ):
            impl( std::make_unique< IMPL >( *( o.impl ) ) )
        {}
        
        BinaryDataStream::BinaryDataStream( BinaryDataStream && o ) noexcept:
            impl( std::move( o.impl ) )
        {}
        
        BinaryDataStream::~BinaryDataStream()
        {}
        
        BinaryDataStream & BinaryDataStream::operator =( BinaryDataStream o )
        {
            swap( *( this ), o );
            
            return *( this );
        }
        
        BinaryStream::Endianness BinaryDataStream::preferredEndianness() const
        {
            return this->impl->_endianness;
        }
        
        void BinaryDataStream::setPreferredEndianness( Endianness value )
        {
            this->impl->_endianness = value;
        }
        
        void BinaryDataStream::read( uint8_t * buf, size_t size )
        {
            if( size == 0 )
            {
                return;
            }
            
            if( size > this->impl->_data.size() - this->impl->_pos )
            {
                throw std::runtime_error( "Invalid read - Not enough data available" );
            }
            
            memcpy( buf, &( this->impl->_data[ 0 ] ) + this->impl->_pos, size );
            
            this->impl->_pos += size;
        }
        
        void BinaryDataStream::seek( ssize_t offset, SeekDirection dir )
        {
            size_t pos;
            
            if( dir == SeekDirection::Begin )
            {
                if( offset < 0 )
                {
                    throw std::runtime_error( "Invalid seek offset" );
                }
                
                pos = numeric_cast< size_t >( offset );
            }
            else if( dir == SeekDirection::End )
            {
                if( offset > 0 )
                {
                    throw std::runtime_error( "Invalid seek offset" );
                }
                
                pos = this->impl->_data.size() - numeric_cast< size_t >( abs( offset ) );
            }
            else if( offset < 0 )
            {
                pos = this->impl->_pos - numeric_cast< size_t >( abs( offset ) );
            }
            else
            {
                pos = this->impl->_pos + numeric_cast< size_t >( offset );
            }
            
            if( pos > this->impl->_data.size() )
            {
                throw std::runtime_error( "Invalid seek offset" );
            }
            
            this->impl->_pos = pos;
        }
        
        size_t BinaryDataStream::tell() const
        {
            return this->impl->_pos;
        }
        
        BinaryDataStream & BinaryDataStream::operator +=( const BinaryDataStream & stream )
        {
            this->append( stream );
            
            return *( this );
        }
        
        BinaryDataStream & BinaryDataStream::operator +=( const std::vector< uint8_t > & data )
        {
            this->append( data );
            
            return *( this );
        }
        
        void BinaryDataStream::append( const BinaryDataStream & stream )
        {
            this->impl->_data.insert
            (
                this->impl->_data.end(),
                stream.impl->_data.begin() + numeric_cast< ssize_t >( stream.impl->_pos ),
                stream.impl->_data.end()
            );
            
            stream.impl->_pos = stream.impl->_data.size();
        }
        
        void BinaryDataStream::append( const std::vector< uint8_t > & data )
        {
            this->impl->_data.insert
            (
                this->impl->_data.end(),
                data.begin(),
                data.end()
            );
        }
        
        void swap( BinaryDataStream & o1, BinaryDataStream & o2 )
        {
            using std::swap;
            
            swap( o1.impl, o2.impl );
        }
        
        BinaryDataStream::IMPL::IMPL():
            _pos(        0 ),
            _endianness( Endianness::Default )
        {}
        
        BinaryDataStream::IMPL::IMPL( const std::vector< uint8_t > & data ):
            _data(       data ),
            _pos(        0 ),
            _endianness( Endianness::Default )
        {}
        
        BinaryDataStream::IMPL::IMPL( const IMPL & o ):
            _data(       o._data ),
            _pos(        o._pos ),
            _endianness( o._endianness )
        {}
        
        BinaryDataStream::IMPL::~IMPL()
        {}
    }
}
