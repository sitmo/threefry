/* boost random/threefry.hpp header file
 *
 * Copyright (c) 2014 M.A. (Thijs) van den Berg
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org for most recent version including documentation.
 *
 * $Id$
 */

#ifndef BOOST_RANDOM_THREEFRY_HPP
#define BOOST_RANDOM_THREEFRY_HPP

#include <istream>

#include <boost/cstdint.hpp>
#include <boost/config.hpp>
#include <boost/limits.hpp>

#include <boost/static_assert.hpp>

#include <boost/random/detail/config.hpp>
#include <boost/random/detail/const_mod.hpp>
#include <boost/random/detail/seed.hpp>
#include <boost/random/detail/seed_impl.hpp>

namespace boost {
namespace random {

/**
* @brief The threefry random engine is a counter based random engine that uses a stripped-down Threefish cryptographic function that is optimised for speed.
*
* The template parameter @p UIntType must be an unsigned integral type. 
* The template parameter @p rounds should be 13 or 20
*
* @blockquote
* engines:   Threefry-4×64, 13 and 20 rounds
* title:     Parallel random numbers: as easy as 1, 2, 3
* authors:   Salmon, John K. and Moraes, Mark A. and Dror, Ron O. and Shaw, David E.
* booktitle: Proceedings of 2011 International Conference for High Performance Computing, Networking, Storage and Analysis
* publicher: ACM
* year:      2011
* isbn:      978-1-4503-0771-0
* @endblockquote
*
* @xmlnote
* The boost variant has been implemented from scratch and does not
* derive from or use the Random123 library provided at http://www.thesalmons.org/john/random123/
* However, it  was verified that both produce identical output.
* Output was verified against the threefry4x64 unit test cases from https://github.com/girving/random123/blob/master/examples/kat_vectors
* @endxmlnote
*/
template <typename UIntType,  std::size_t rounds=20>
class threefry_engine
{
public:
    BOOST_STATIC_ASSERT( rounds==13 || rounds==20 );
 
    /** The type of the generated random value. */
    typedef UIntType result_type;

    BOOST_STATIC_CONSTANT(bool, has_fixed_range = false);
    BOOST_STATIC_CONSTANT(std::size_t, samples_per_block = 256/std::numeric_limits<UIntType>::digits);


    /**
     * @brief Constructs the defafult %threefry_engine.
     */
    threefry_engine() 
    { seed(0); }
    
    /**
     * @brief Constructs a %threefry_engine random number
     *        generator engine with seed @p value.  The default seed value
     *        is 0.
     *
     * @param value The initial seed value.
     */
    BOOST_RANDOM_DETAIL_ARITHMETIC_CONSTRUCTOR(threefry_engine, UIntType, value)
    { seed(value); }
    
    /**
     * @brief Constructs a %threefry_engine and seed it
     * with values taken from the iterator range [@p first, @p last)
     * and adjusts first iterator to point to the element after the last one
     * used.  If there are not enough elements, throws @c std::invalid_argument.
     *
     * @p first and @p last must be input iterators.
     */
    template<class It> threefry_engine(It& first, It last)
    { seed(first,last); }
    
    /**
     * @brief Constructs a %threefry_engine random number
     *        generator engine seeded from the seed sequence @p seq.
     *
     * @param seq the seed sequence.
     */
    BOOST_RANDOM_DETAIL_SEED_SEQ_CONSTRUCTOR(threefry_engine, SeedSeq, seq)
    { seed(seq); }

    /**
     * @brief Re-seed the %threefry_engine to it's default seed.
     */
    void seed()
    { seed(0); }
    
    /**
     * @brief Re-seed the  %threefry_engine random number
     *        generator engine with the seed @p value.
     *        The default seed value is 0.
     *
     * @param value the new seed.
     */
    BOOST_RANDOM_DETAIL_ARITHMETIC_SEED(threefry_engine, UIntType, value)
    {
        UIntType v = value;
        for (unsigned int i=0; i<4; ++i) {
            _key[i] = v & 0xFFFFFFFFFFFFFFFF;
            
            // v >>= 64 causes undefined behaviour when the type of b <= 64 bits, but this works
            for (unsigned int b=0; b<8; ++b)
                v >>= 8;
        }
        reset_after_key_change();
    }

    /**
     * @brief Seeds the %threefry_engine random number
     *        generator engine with the seed sequence @p seq.
     *
     * @param seq the seed sequence.
     */
    BOOST_RANDOM_DETAIL_SEED_SEQ_SEED(threefry_engine, SeeqSeq, seq)
    {
        detail::seed_array_int<64>(seq, _key);
        reset_after_key_change();
    }

    /**
     * @brief Seed the %threefry_engine with values taken
     * from the iterator range [@p first, @p last) and adjusts @p first to
     * point to the element after the last one used.  If there are
     * not enough elements, throws @c std::invalid_argument.
     *
     * @p first and @p last must be input iterators.
     */
    template<class It> void seed(It& first, It last)
    { 
        detail::fill_array_int<64>(first, last, _key);
        reset_after_key_change();
    }
    
    /**
     * @brief Gets the smallest possible value in the output range.
     */
    static result_type min BOOST_PREVENT_MACRO_SUBSTITUTION ()
    { return 0; }

    /**
     * @brief Gets the largest possible value in the output range.
     */
    static result_type max BOOST_PREVENT_MACRO_SUBSTITUTION ()
    { return std::numeric_limits<UIntType>::max(); }


    /**
     * @brief Generate a random sample.
     */
    result_type operator()()
    {
        // can we return a value from the current block?
        if (_o_counter < samples_per_block)
            return reinterpret_cast<result_type*>(_output)[_o_counter++];

        
        // generate a new block and return the first result_type 
        inc_counter();
        encrypt_counter();
        _o_counter = 1; // the next call
        return *reinterpret_cast<result_type*>(_output);  // this call
    }

    /**
     * @brief Fills a range with random values.
     */
    template<class Iter>
    void generate(Iter first, Iter last)
    { detail::generate_from_int(*this, first, last); }
    
    /**
     * @brief Discard a number of elements from the random numbers sequence.
     *
     * @param z the number of elements to discard.
     */
    void discard(boost::uintmax_t z)
    {
        // check if we stay in the current block
        if (z < samples_per_block - _o_counter) {
            _o_counter += static_cast<unsigned short>(z);
            return;
        }

        // we will have to generate a new block...
        z -= (samples_per_block - _o_counter);  // discard the remainder of the current blok
        _o_counter = z % samples_per_block;     // set the pointer in the correct element in the new block
        z -= _o_counter;                        // update z
        z /= samples_per_block;                 // the number of bocks to skip is z/samples_per_block
        ++z;                                    // and one more because we crossed the buffer line
        inc_counter(z);
        encrypt_counter();
    }

#ifndef BOOST_RANDOM_NO_STREAM_OPERATORS
    /**
     * @brief Writes the textual representation of the state x(i) of x to
     *        @p _os.
     *
     * @param os  The output stream.
     * @param eng A %threefry_engine random number generator.
     * @returns os.
     */
   template<class CharT, class Traits>
   friend std::basic_ostream<CharT, Traits>&
   operator<<(std::basic_ostream<CharT, Traits>& os, const threefry_engine& eng) {
       for (unsigned short i=0; i<4; ++i)
           os << eng._key[i] << ' ' << eng._counter[i] << ' ' << eng._output[i] << ' ';
       os << eng._o_counter;

       return os;
   }
   
    /**
     * @brief reads the textual representation of the state x(i) from
     *        @p is.
     *
     * @param is  The input stream.
     * @param eng A %threefry_engine random number generator.
     * @returns is.
     */
   template<class CharT, class Traits>
   friend std::basic_istream<CharT, Traits>&
    operator >> (std::basic_istream<CharT, Traits>& is, threefry_engine& eng) {
        for (unsigned short i=0; i<4; ++i) 
           is >> eng._key[i] >> std::ws >> eng._counter[i] >> std::ws >> eng._output[i] >> std::ws;
        is >> eng._o_counter;
        eng._keyx = 0x1BD11BDAA9FC1A22 ^ eng._key[0] ^ eng._key[1] ^ eng._key[2] ^ eng._key[3];
        return is;
    } 
#endif

    /**
     * @brief Compares two %threefry_engine
     * objects of the same type for equality.
     *
     *
     * @param _lhs A threefry engine.
     * @param _rhs Another threefry engine.
     *
     * @returns true if the infinite sequences of generated values
     *          would be equal, false otherwise.
     */
    friend bool 
    operator==(const threefry_engine& _lhs, const threefry_engine& _rhs) 
    {
        if (_lhs._o_counter != _rhs._o_counter) return false;
        
        for (unsigned short i=0; i<4; ++i) {
            if (_lhs._counter[i] != _rhs._counter[i]) return false;
            if (_lhs._key[i]     != _rhs._key[i])     return false;
            if (_lhs._output[i]  != _rhs._output[i])  return false;
        }
        return true;
    }
    
    /**
     * @brief Compares two threefry engines for inequality.
     *
     *
     * @param _lhs A threefry engine.
     * @param _rhs Another threefry engine.
     *
     * @returns true if the infinite sequences of generated values
     *          would not be equal, false otherwise.
     */
    friend bool 
    operator!=(const threefry_engine& _lhs, const threefry_engine& _rhs) 
    { return !(_lhs == _rhs); }
    
    /**
     * @brief Set the internal 256 bit key of the threefry engine, don't reset the counter.
     *
     * @param _k0 the first 64 bits of the key.
     * @param _k1 the 2nd set of 64 bits of the key.
     * @param _k2 the 3rd set of 64 bits of the key.
     * @param _k3 the last 64 bits of the key.
     */
    void set_key(boost::uint64_t _k0=0, boost::uint64_t _k1=0, boost::uint64_t _k2=0, boost::uint64_t _k3=0)
    {
        _key[0] = _k0;
        _key[1] = _k1;
        _key[2] = _k2;
        _key[3] = _k3;
        _keyx = 0x1BD11BDAA9FC1A22 ^ _key[0] ^ _key[1] ^ _key[2] ^ _key[3];
        encrypt_counter();
    }
    
    /**
     * @brief Set the internal 256 bit counter of the threefry engine.
     *
     * @param _s0 the first 64 bits of the counter.
     * @param _s1 the 2nd set of 64 bits of the counter.
     * @param _s2 the 3rd set of 64 bits of the counter.
     * @param _s3 the last 64 bits of the counter.
     * @param _o_counter the output counter  which specifies the current position in
     *       the 256 bit block
     */
    void set_counter(boost::uint64_t _s0=0, boost::uint64_t _s1=0, boost::uint64_t _s2=0, boost::uint64_t _s3=0, unsigned short _o_counter=0)
    {
        _counter[0] = _s0; 
        _counter[1] = _s1; 
        _counter[2] = _s2; 
        _counter[3] = _s3;

        _o_counter = _o_counter % samples_per_block;

        encrypt_counter();
    }

    /**
     * @brief Set the internal 256 bit key and 256 bit counter of the threefry engine.
     *
     * @param _k0 the first 64 bits of the key.
     * @param _k1 the 2nd set of 64 bits of the key.
     * @param _k2 the 3rd set of 64 bits of the key.
     * @param _k3 the last 64 bits of the key.
     * @param _s0 the first 64 bits of the counter.
     * @param _s1 the 2nd set of 64 bits of the counter.
     * @param _s2 the 3rd set of 64 bits of the counter.
     * @param _s3 the last 64 bits of the counter.
     * @param _o_counter the output counter  which specifies the current position in
     *       the 256 bit block
     */
    void set_key_and_counter(boost::uint64_t _k0=0, boost::uint64_t _k1=0, boost::uint64_t _k2=0, boost::uint64_t _k3=0,
                             boost::uint64_t _s0=0, boost::uint64_t _s1=0, boost::uint64_t _s2=0, boost::uint64_t _s3=0, 
                             unsigned short _o_counter=0)
    {
        _key[0] = _k0;
        _key[1] = _k1;
        _key[2] = _k2;
        _key[3] = _k3;
        _keyx = 0x1BD11BDAA9FC1A22 ^ _key[0] ^ _key[1] ^ _key[2] ^ _key[3];
        _counter[0] = _s0; 
        _counter[1] = _s1; 
        _counter[2] = _s2; 
        _counter[3] = _s3;
        _o_counter = _o_counter % samples_per_block;

        encrypt_counter();
    }

    
private:
    /// \cond show_private

    
    // double mixing function
    void dmf(   boost::uint64_t& x0, boost::uint64_t& x1, boost::uint8_t rx,
                boost::uint64_t& z0, boost::uint64_t& z1, boost::uint8_t rz)
    {
        x0 += x1;
        z0 += z1;
        x1 = (x1 << rx) | (x1 >> (64-rx));
        z1 = (z1 << rz) | (z1 >> (64-rz));
        x1 ^= x0;
        z1 ^= z0;
    }

    // Double mixing function with key adition
    void dmfk(  boost::uint64_t& x0, boost::uint64_t& x1, boost::uint8_t rx,
                boost::uint64_t& z0, boost::uint64_t& z1, boost::uint8_t rz,
                boost::uint64_t  k0, boost::uint64_t  k1, boost::uint64_t l0, boost::uint64_t l1)
    {
        x1 += k1;
        z1 += l1;
        x0 += x1 + k0;
        z0 += z1 + l0;
        x1 = (x1 << rx) | (x1 >> (64-rx));
        z1 = (z1 << rz) | (z1 >> (64-rz));
        x1 ^= x0;
        z1 ^= z0;
    }
 
    // encrypt the couter with the key and store the result in the output buffer
    void encrypt_counter()
    {

        for (unsigned short i=0; i<4; ++i) 
            _output[i] = _counter[i];

        dmfk(_output[0], _output[1], 14,   _output[2], _output[3], 16,   _key[0], _key[1], _key[2], _key[3]);
        dmf( _output[0], _output[3], 52,   _output[2], _output[1], 57);
        dmf( _output[0], _output[1], 23,   _output[2], _output[3], 40);
        dmf( _output[0], _output[3],  5,   _output[2], _output[1], 37);

        dmfk(_output[0], _output[1], 25,   _output[2], _output[3], 33,   _key[1], _key[2], _key[3], _keyx + 1);
        dmf( _output[0], _output[3], 46,   _output[2], _output[1], 12);
        dmf( _output[0], _output[1], 58,   _output[2], _output[3], 22);
        dmf( _output[0], _output[3], 32,   _output[2], _output[1], 32);

        dmfk(_output[0], _output[1], 14,   _output[2], _output[3], 16,   _key[2], _key[3], _keyx, _key[0] + 2);
        dmf( _output[0], _output[3], 52,   _output[2], _output[1], 57);
        dmf( _output[0], _output[1], 23,   _output[2], _output[3], 40);
        dmf( _output[0], _output[3],  5,   _output[2], _output[1], 37);

        dmfk(_output[0], _output[1], 25,   _output[2], _output[3], 33,   _key[3], _keyx, _key[0], _key[1] + 3);        
        
        if (rounds>13)
        {
            dmf( _output[0], _output[3], 46,   _output[2], _output[1], 12);
            dmf( _output[0], _output[1], 58,   _output[2], _output[3], 22);
            dmf( _output[0], _output[3], 32,   _output[2], _output[1], 32);

            dmfk(_output[0], _output[1], 14,   _output[2], _output[3], 16,   _keyx, _key[0], _key[1], _key[2] + 4);
            dmf( _output[0], _output[3], 52,   _output[2], _output[1], 57);
            dmf( _output[0], _output[1], 23,   _output[2], _output[3], 40);
            dmf( _output[0], _output[3],  5,   _output[2], _output[1], 37);

            for (unsigned int i=0; i<4; ++i)
                _output[i] += _key[i];
                
            _output[3] += 5;
        }
    }
    
    // increment the counter with 1
    void inc_counter()
    {
        ++_counter[0]; 
        if (_counter[0] != 0) return;
        
        ++_counter[1]; 
        if (_counter[1] != 0) return;
        
        ++_counter[2]; 
        if (_counter[2] != 0) return;
        
        ++_counter[3];
    }
    
    // increment the counter with z
    void inc_counter(boost::uintmax_t z)
    {
        if (z > 0xFFFFFFFFFFFFFFFF - _counter[0]) {   // check if we will overflow the first 64 bit int
            ++_counter[1];
            if (_counter[1] == 0) {
                ++_counter[2];
                if (_counter[2] == 0) {
                    ++_counter[3];
                }
            }
        }
        _counter[0] += z;
    }

    // reset the counter to zero, and reset the keyx
    void reset_after_key_change()
    {
        _keyx = 0x1BD11BDAA9FC1A22 ^ _key[0] ^ _key[1] ^ _key[2] ^ _key[3];
        _counter[0] = 0;
        _counter[1] = 0;
        _counter[2] = 0;
        _counter[3] = 0;
        _o_counter = 0;
        encrypt_counter();
    }
    
private:
    boost::uint64_t _key[4];       // the 256 bit encryption key
    boost::uint64_t _keyx;
    boost::uint64_t _counter[4];   // the 256 bit counter (message) that gets encrypted
    boost::uint64_t _output[4];    // the 256 bit cipher output 4 * 64 bit = 256 bit output
    boost::uint16_t _o_counter;     // output chunk counter, e.g. for a 64 bit random engine
                            // the 256 bit output buffer gets split in 4 chunks.
    /// \endcond

};

/**
 * 32 bit version of the 13 rounds threefry engine
 */
typedef threefry_engine<boost::uint32_t, 13> threefry13;

/**
 * 32 bit version of the 20 rounds threefry engine
 */
 
typedef threefry_engine<boost::uint32_t, 20> threefry20;

/**
 * 64 bit version of the 13 rounds threefry engine
 */
typedef threefry_engine<boost::uint64_t, 13> threefry13_64;

/**
 * 64 bit version of the 20 rounds threefry engine
 */
typedef threefry_engine<boost::uint64_t, 20> threefry20_64;

} // namespace random

using random::threefry13;
using random::threefry20;
using random::threefry13_64;
using random::threefry20_64;

} // namespace boost

#include <boost/random/detail/enable_warnings.hpp>

#endif // BOOST_RANDOM_THREEFRY_HPP
