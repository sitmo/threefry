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
#include <algorithm>

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

namespace detail {

    static const uint_least64_t threefry4x64_tweak = 0x1BD11BDAA9FC1A22;
    
    // primary template
    template< typename UIntType, std::size_t w>
    struct extract4x64_impl {
        inline static UIntType zth(const boost::uint_least64_t (&_output)[4]);
        inline static UIntType nth(const boost::uint_least64_t (&_output)[4], std::size_t n);
        inline static UIntType w_max();
    };
    
    // specialisation
    template< typename UIntType>
    struct extract4x64_impl<UIntType,64> {
        inline static UIntType zth(const boost::uint_least64_t (&_output)[4])
            { return _output[0]; }
        inline static UIntType nth(const boost::uint_least64_t (&_output)[4], std::size_t n)
            { return _output[n]; }
        inline static UIntType w_max()
            { return 0xFFFFFFFFFFFFFFFF; }
    };
    template< typename UIntType>
    struct extract4x64_impl<UIntType,32> {
        inline static UIntType zth(const boost::uint_least64_t (&_output)[4])
            { return _output[0] & 0xFFFFFFFF; }
        inline static UIntType nth(const boost::uint_least64_t (&_output)[4], std::size_t n)
            { return (_output[n>>1] >> ((n&1)<<5)) & 0xFFFFFFFF; }
        inline static UIntType w_max()
            { return 0xFFFFFFFF; }
    };
    template< typename UIntType>
    struct extract4x64_impl<UIntType,16> {
        inline static UIntType zth(const boost::uint_least64_t (&_output)[4])
            { return _output[0] & 0xFFFF; }
        inline static UIntType nth(const boost::uint_least64_t (&_output)[4], std::size_t n)
            { return (_output[n>>2] >> ((n&11)<<4)) & 0xFFFF; }
        inline static UIntType w_max()
            { return 0xFFFF; }
    };
    template< typename UIntType>
    struct extract4x64_impl<UIntType,8> {
        inline static UIntType zth(const boost::uint_least64_t (&_output)[4])
            { return _output[0] & 0xFF; }
        inline static UIntType nth(const boost::uint_least64_t (&_output)[4], std::size_t n)
            { return (_output[n>>3] >> ((n&111)<<3)) & 0xFF; }
        inline static UIntType w_max()
            { return 0xFF; }
    };
} // detail

/**
* @brief The threefry random engine is a counter based random engine that uses a stripped-down Threefish cryptographic function that is optimised for speed.
*
* parameter @p UIntType the return type of the random engine, must be an unsigned integral type, e.g. uint_least32_t
* parameter @p ReturnBits is the width of the return type, the number of bits. Valid values are 8,16,32,64.
* parameter @p Rounds the number of encryption rounds. Must be 1 or higher.
* parameter @p KeySize the number of 64bit integers in the key. The key value is set via a seed, and different keys give different random sequences. The default value is 4, valida values are 0,1,2,3,4. The number of independent seed valuedsis 2^( 64*KeySize ).
* parameter @p CounterSize the number of 64bit integers in the internal counter. The default value is 4, valid values are 1,2,3,4. The cycle of the engine is  2^( 64*CounterSize + S ) with S=3 for a 32 bit random engine, and S=2 for a 64 bit random engine.
*
* @blockquote
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

template <  typename UIntType,
            std::size_t ReturnBits,
            std::size_t Rounds = 20,
            std::size_t KeySize = 4,
            std::size_t CounterSize = 4
        >
class threefry4x64_engine
{
public:
    BOOST_STATIC_ASSERT( ReturnBits==8 || ReturnBits==16 || ReturnBits==32 || ReturnBits==64 );
    BOOST_STATIC_ASSERT( KeySize>=0 && KeySize<=4 );
    BOOST_STATIC_ASSERT( CounterSize>=1 && CounterSize<=4 );
 
    /** The type of the generated random value. */
    typedef UIntType result_type;
    BOOST_STATIC_CONSTANT(std::size_t, word_size = ReturnBits);
    BOOST_STATIC_CONSTANT(std::size_t, rounds = Rounds);
    BOOST_STATIC_CONSTANT(bool, has_fixed_range = false);

    BOOST_STATIC_CONSTANT(std::size_t, samples_per_block = 256/ReturnBits);
    
    /**
     * @brief Constructs the defafult %threefry4x64_engine.
     */
    threefry4x64_engine()
    {
        seed(0);
    }
    
    /**
     * @brief Constructs a %threefry4x64_engine random number
     *        generator engine with seed @p value. The default seed value is 0.
     *
     * @param value The initial seed value.
     */
    BOOST_RANDOM_DETAIL_ARITHMETIC_CONSTRUCTOR(threefry4x64_engine, UIntType, value)
    { seed(value); }
    
    /**
     * @brief Constructs a %threefry4x64_engine and seed it
     * with values taken from the iterator range [@p first, @p last)
     * and adjusts first iterator to point to the element after the last one
     * used.  If there are not enough elements, throws @c std::invalid_argument.
     *
     * @p first and @p last must be input iterators.
     */
    template<class It> threefry4x64_engine(It& first, It last)
    { seed(first,last); }
    
    /**
     * @brief Constructs a %threefry4x64_engine random number
     *        generator engine seeded from the seed sequence @p seq.
     *
     * @param seq the seed sequence.
     */
    BOOST_RANDOM_DETAIL_SEED_SEQ_CONSTRUCTOR(threefry4x64_engine, SeedSeq, seq)
    { seed(seq); }

    /**
     * @brief Re-seed the %threefry4x64_engine to it's default seed.
     */
    void seed()
    { seed(0); }
    
    /**
     * @brief Re-seed the  %threefry4x64_engine random number
     *        generator engine with the seed @p value.
     *        The default seed value is 0.
     *
     * @param value the new seed.
     */
    BOOST_RANDOM_DETAIL_ARITHMETIC_SEED(threefry4x64_engine, UIntType, value)
    {
        if (KeySize>=1) _key[1] = value; // UIntType is max 64 bits, key[0] has the xor-tweak
        if (KeySize>=2) _key[2] = 0;
        if (KeySize>=3) _key[3] = 0;
        if (KeySize>=4) _key[4] = 0;
        reset_after_key_change();
    }

    /**
     * @brief Seeds the %threefry4x64_engine random number
     *        generator engine with the seed sequence @p seq.
     *
     * @param seq the seed sequence.
     */
    BOOST_RANDOM_DETAIL_SEED_SEQ_SEED(threefry4x64_engine, SeeqSeq, seq)
    {
        boost::uint64_t tmp[KeySize];
        detail::seed_array_int<64>(seq, tmp);
        for (std::size_t i=0; i<KeySize; ++i) 
            _key[i+1] = tmp[i];
        reset_after_key_change();
    }

    /**
     * @brief Seed the %threefry4x64_engine with values taken
     * from the iterator range [@p first, @p last) and adjusts @p first to
     * point to the element after the last one used.  If there are
     * not enough elements, throws @c std::invalid_argument.
     *
     * @p first and @p last must be input iterators.
     */
    template<class It> void seed(It& first, It last)
    { 
        boost::uint64_t tmp[KeySize];
        detail::fill_array_int<64>(first, last, tmp);
        for (std::size_t i=0; i<KeySize; ++i) 
            _key[i+1] = tmp[i];
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
    { return detail::extract4x64_impl<UIntType,ReturnBits>::w_max(); }


    /**
     * @brief Generate a random sample.
     */
    result_type operator()()
    {
        // can we return a value from the current block?
        if (_o_counter < samples_per_block)
            return detail::extract4x64_impl<UIntType,ReturnBits>::nth(_output, _o_counter++);
        
        // generate a new block and return the first result_type 
        inc_counter();
        encrypt_counter();
        _o_counter = 1; // the next call
        return detail::extract4x64_impl<UIntType,ReturnBits>::zth(_output);
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
        if (z <= samples_per_block - _o_counter) {
            _o_counter += static_cast<unsigned short>(z);
            return;
        }

        _o_counter += (z % samples_per_block);
         z /= samples_per_block;
         
         if (_o_counter > samples_per_block) {
            _o_counter -= samples_per_block;
            ++z;
         }
         
        inc_counter(z);
        
        if (_o_counter != samples_per_block)
            encrypt_counter();
    }

#ifndef BOOST_RANDOM_NO_STREAM_OPERATORS
    /**
     * @brief Writes the textual representation of the state x(i) of x to
     *        @p _os.
     *
     * @param os  The output stream.
     * @param eng A %threefry4x64_engine random number generator.
     * @returns os.
     */
    template<class CharT, class Traits>
    friend std::basic_ostream<CharT, Traits>&
    operator << (std::basic_ostream<CharT, Traits>& os, const threefry4x64_engine& eng)
    {
        for (unsigned short i=0; i<KeySize; ++i)
            os << eng._key[i+1] << ' ';
            
        for (unsigned short i=0; i<CounterSize; ++i)
            os << eng._counter[i] << ' ';
            
        os << eng._o_counter;
       return os;
    }
   
    /**
     * @brief reads the textual representation of the state x(i) from
     *        @p is.
     *
     * @param is  The input stream.
     * @param eng A %threefry4x64_engine random number generator.
     * @returns is.
     */
    template<class CharT, class Traits>
    friend std::basic_istream<CharT, Traits>&
    operator >> (std::basic_istream<CharT, Traits>& is, threefry4x64_engine& eng)
    {
        for (unsigned short i=0; i<KeySize; ++i) 
            is >> eng._key[i+1] >> std::ws;
            
        for (unsigned short i=0; i<CounterSize; ++i) 
            is >> eng._counter[i] >> std::ws;
            
        is >> eng._o_counter;
        eng.init_key();
        
        eng.encrypt_counter();
        return is;
    } 
#endif

    /**
     * @brief Compares two %threefry4x64_engine
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
    operator==(const threefry4x64_engine& _lhs, const threefry4x64_engine& _rhs) 
    {
        if (_lhs._o_counter != _rhs._o_counter) return false;
        
        for (unsigned short i=0; i<KeySize; ++i)
            if (_lhs._key[i] != _rhs._key[i]) return false;

        for (unsigned short i=0; i<CounterSize; ++i)
            if (_lhs._counter[i] != _rhs._counter[i]) return false;

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
    operator!=(const threefry4x64_engine& _lhs, const threefry4x64_engine& _rhs) 
    { return !(_lhs == _rhs); }


private:
    inline void rotl64(boost::uint_least64_t& v, const boost::uint8_t bits) const
    { 
        v = (v << bits) | (v >> (64 - bits));
    }
    
    inline void mix64(boost::uint_least64_t& x0, boost::uint64_t& x1, const boost::uint8_t bits) const
    {
        x0 += x1;
        rotl64(x1, bits);
        x1 ^= x0;
    }

    inline void double_mix64(   boost::uint_least64_t& x0, boost::uint_least64_t& x1, const boost::uint8_t rx,
                                boost::uint_least64_t& z0, boost::uint_least64_t& z1, const boost::uint8_t rz) const
    {
        mix64(x0,x1,rx);
        mix64(z0,z1,rz);
    }

    template <std::size_t offset>
    inline void add_key64_t( boost::uint_least64_t (&output)[4], boost::uint_least64_t (&key)[KeySize+1], const std::size_t c) const
    {
        if (((offset+1)%5) <= KeySize) output[0] += key[(offset+1)%5];
        if (((offset+2)%5) <= KeySize) output[1] += key[(offset+2)%5];
        if (((offset+3)%5) <= KeySize) output[2] += key[(offset+3)%5];
        if (((offset+4)%5) <= KeySize) output[3] += key[(offset+4)%5];
        output[3] += c;
    }

    template <std::size_t R>
    inline void encrypt_counter_t(std::size_t& four_cycles)
    {
        double_mix64( _output[0], _output[1], 14, _output[2], _output[3], 16); if (R<2) return;
        double_mix64( _output[0], _output[3], 52, _output[2], _output[1], 57); if (R<3) return;
        double_mix64( _output[0], _output[1], 23, _output[2], _output[3], 40); if (R<4) return;
        double_mix64( _output[0], _output[3],  5, _output[2], _output[1], 37);
        add_key64_t<1>(_output, _key, ++four_cycles); if (R<5) return;
     
        double_mix64( _output[0], _output[1], 25, _output[2], _output[3], 33); if (R<6) return;
        double_mix64( _output[0], _output[3], 46, _output[2], _output[1], 12); if (R<7) return;
        double_mix64( _output[0], _output[1], 58, _output[2], _output[3], 22); if (R<8) return;
        double_mix64( _output[0], _output[3], 32, _output[2], _output[1], 32);
        add_key64_t<2>(_output, _key, ++four_cycles); if (R<9) return;
        
        double_mix64( _output[0], _output[1], 14, _output[2], _output[3], 16); if (R<10) return;
        double_mix64( _output[0], _output[3], 52, _output[2], _output[1], 57); if (R<11) return;
        double_mix64( _output[0], _output[1], 23, _output[2], _output[3], 40); if (R<12) return;
        double_mix64( _output[0], _output[3],  5, _output[2], _output[1], 37);
        add_key64_t<3>(_output, _key, ++four_cycles); if (R<13) return;

        double_mix64( _output[0], _output[1], 25, _output[2], _output[3], 33); if (R<14) return;
        double_mix64( _output[0], _output[3], 46, _output[2], _output[1], 12); if (R<15) return;
        double_mix64( _output[0], _output[1], 58, _output[2], _output[3], 22); if (R<16) return;
        double_mix64( _output[0], _output[3], 32, _output[2], _output[1], 32); 
        add_key64_t<4>(_output, _key, ++four_cycles); if (R<17) return;
        
        double_mix64( _output[0], _output[1], 14, _output[2], _output[3], 16); if (R<18) return;
        double_mix64( _output[0], _output[3], 52, _output[2], _output[1], 57); if (R<19) return;
        double_mix64( _output[0], _output[1], 23, _output[2], _output[3], 40); if (R<20) return;
        double_mix64( _output[0], _output[3],  5, _output[2], _output[1], 37);
        add_key64_t<0>(_output, _key, ++four_cycles); if (R<21) return;

        double_mix64( _output[0], _output[1], 25, _output[2], _output[3], 33); if (R<22) return;
        double_mix64( _output[0], _output[3], 46, _output[2], _output[1], 12); if (R<23) return;
        double_mix64( _output[0], _output[1], 58, _output[2], _output[3], 22); if (R<24) return;
        double_mix64( _output[0], _output[3], 32, _output[2], _output[1], 32);
        add_key64_t<1>(_output, _key, ++four_cycles); if (R<25) return;

        double_mix64( _output[0], _output[1], 14, _output[2], _output[3], 16); if (R<26) return;
        double_mix64( _output[0], _output[3], 52, _output[2], _output[1], 57); if (R<27) return;
        double_mix64( _output[0], _output[1], 23, _output[2], _output[3], 40); if (R<28) return;
        double_mix64( _output[0], _output[3],  5, _output[2], _output[1], 37); 
        add_key64_t<2>(_output, _key, ++four_cycles); if (R<29) return;

        double_mix64( _output[0], _output[1], 25, _output[2], _output[3], 33); if (R<30) return;
        double_mix64( _output[0], _output[3], 46, _output[2], _output[1], 12); if (R<31) return;
        double_mix64( _output[0], _output[1], 58, _output[2], _output[3], 22); if (R<32) return;
        double_mix64( _output[0], _output[3], 32, _output[2], _output[1], 32); 
        add_key64_t<3>(_output, _key, ++four_cycles); if (R<33) return;

        double_mix64( _output[0], _output[1], 14, _output[2], _output[3], 16); if (R<34) return;
        double_mix64( _output[0], _output[3], 52, _output[2], _output[1], 57); if (R<35) return;
        double_mix64( _output[0], _output[1], 23, _output[2], _output[3], 40); if (R<36) return;

        double_mix64( _output[0], _output[3],  5, _output[2], _output[1], 37);
        add_key64_t<4>(_output, _key, ++four_cycles); if (R<37) return;

        double_mix64( _output[0], _output[1], 25, _output[2], _output[3], 33); if (R<38) return;
        double_mix64( _output[0], _output[3], 46, _output[2], _output[1], 12); if (R<39) return;
        double_mix64( _output[0], _output[1], 58, _output[2], _output[3], 22); if (R<40) return;

        double_mix64( _output[0], _output[3], 32, _output[2], _output[1], 32);
        add_key64_t<0>(_output, _key, ++four_cycles);
    }
    
    void encrypt_counter()
    {
        for (std::size_t i=0; i<CounterSize; ++i)
            _output[i] = _counter[i];
        
        for (std::size_t i=CounterSize; i<4; ++i)
            _output[i] = 0;
        
        for (std::size_t i=0; i<KeySize; ++i)
            _output[i] +=_key[(i+1)%5];
        
        std::size_t four_cycles = 0;

        // do chunks of 40 rounds
        for (std::size_t big_rounds=0; big_rounds < Rounds/40; ++big_rounds)
            encrypt_counter_t<40>(four_cycles);
        
        // the remaining rounds
        encrypt_counter_t<Rounds - 40*(Rounds/40)>(four_cycles);
    }
    
    // increment the counter with 1
    void inc_counter()
    {
        ++_counter[0]; 
        
        if (CounterSize > 1) {
            if (_counter[0] != 0) return; // test for overflow, exit when not
            ++_counter[1];

            if (CounterSize > 2) {
                if (_counter[1] != 0) return;
                ++_counter[2];

                if (CounterSize > 3) {
                    if (_counter[2] != 0) return;
                    ++_counter[3];
                }
            }
        }
    }
    
    // increment the counter with z
    void inc_counter(boost::uintmax_t z)
    {
        if (CounterSize == 1) {
            _counter[0] += z;
            return;
        }
        
        bool overflow = (z > 0xFFFFFFFFFFFFFFFF - _counter[0]);
        _counter[0] += z;
        if (!overflow) return;
        
        ++_counter[1];
        
        if(CounterSize <= 2) return;
        if (_counter[1]!=0) return;

        ++_counter[2];
            
        if(CounterSize <= 3) return;
        if (_counter[2]!=0) return;

        ++_counter[3];
    }

    void init_key()
    {
        _key[0] = detail::threefry4x64_tweak;
        if (KeySize>0)  _key[0] ^= _key[1];
        if (KeySize>1)  _key[0] ^= _key[2];
        if (KeySize>2)  _key[0] ^= _key[3];
        if (KeySize>3)  _key[0] ^= _key[4];
    }
    
    void reset_counter()
    {
        _counter[0] = 0xFFFFFFFFFFFFFFFF;
        if (CounterSize>=2) _counter[1] = 0xFFFFFFFFFFFFFFFF;
        if (CounterSize>=3) _counter[2] = 0xFFFFFFFFFFFFFFFF;
        if (CounterSize>=4) _counter[3] = 0xFFFFFFFFFFFFFFFF;
        _o_counter  = samples_per_block;
    }

    // reset the counter to zero, and reset the key
    void reset_after_key_change()
    {
        init_key();
        reset_counter();
    }

    
    boost::uint_least64_t _counter[CounterSize];    // the 256 bit counter (message) that gets encrypted
    boost::uint_least64_t _output[4];               // the 256 bit cipher output 4 * 64 bit = 256 bit output
    boost::uint_least64_t _key[KeySize+1];            // the 256 bit encryption key
    boost::uint_least16_t _o_counter;               // output chunk counter, e.g. for a 64 bit random engine
                                                    // the 256 bit output buffer gets split in 4x64bit chunks or 8x32bit chunks chunks.
    /// \endcond
};

/**
 * 32 bit version of the 13 rounds threefry engine, 64 bit seed, 2^67 cycle length
 */
typedef threefry4x64_engine<boost::uint32_t, 32, 13, 1, 1> threefry4x64_13;
typedef threefry4x64_engine<boost::uint32_t, 32, 13, 4, 4> threefry4x64_13_slow;

/**
 * 64 bit version of the 13 rounds threefry engine, 64 bit seed, 2^66 cycle length
 */
typedef threefry4x64_engine<boost::uint64_t, 64, 13, 1, 1> threefry4x64_13_64;


/**
 * 32 bit version of the 20 rounds threefry engine, 64 bit seed, 2^67 cycle length
 */
typedef threefry4x64_engine<boost::uint32_t, 32, 20, 1, 1> threefry4x64_20;

/**
 * 64 bit version of the 20 rounds threefry engine, 64 bit seed, 2^66 cycle length
 */
typedef threefry4x64_engine<boost::uint64_t, 64, 20, 1, 1> threefry4x64_20_64;

} // namespace random


} // namespace boost

#include <boost/random/detail/enable_warnings.hpp>

#endif // BOOST_RANDOM_THREEFRY_HPP