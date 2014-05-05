# ThreeFry4x64 Random Engine #

The threefry4x64 random engine is a counter based random engine that uses a stripped-down 
Threefish cryptographic function that is optimised for speed. It's main freatures are:

* speed: the engine is as fast as mersenne twister
* quality: the random quality acording to the BigCrush test is highest if you compared is with current engines
*  parallel guarantees: engines with different seeds have a guarantee to have non- overlapping streams. 
* O(1) discard. Besides seeding, discarding is also a common strategy for generate non-
overlapping streams for parallel processing.
* small footprint, the engines uses 5..13x a 64 bit integers for its state and internal cache. 

## Overview ##

|engine  | return type|random bits | rounds | key (bytes) | counter (bytes) | memory (bytes) | Cycle length | Seeds |
|-----|---|--------|---|----------|-----------------|----------------|--------------|---:|
|threefry4x64_13      |uint32_t|32|  13| 8| 8|48 | 2^67 | 2^64 
|threefry4x64_13_64   |uint64_t|64|  13| 8| 8|48 | 2^66 | 2^64 
|threefry4x64_20      |uint32_t|32|  20| 8| 8|48 | 2^67 | 2^64 
|threefry4x64_20_64   |uint64_t|64|  20| 8| 8|48 | 2^66 | 2^64 
|threefry4x64_engine<T,B,R,K,C> |T|B={8,16,32,64}|R>=1 |8\*K, K={0..4} |C\*8, C={1..4}| 32+(K+C)\*8 | 2^{64* C+2} | 2^{K* 64} 


Threefish is a symmetric-key block cipher that can be used to encrypt and  decrypt messages 
using a secret key. Counter based random engines (like this threefry engine) use this 
encryption to encrypt a simple counter -in this case a C\*64 bit integer-. The encrypted 
output message in then used as a pool of pseudo-random bits. The main idea is that the 
encrypted message looks like pure random bits, and that that will be the case for anything 
you encrypt -either a text message or more convenient a large number-.

The threefry random engine uses a C\*64 bit integer counter and a K\*64 bit key to generate a 256 bit encrypted message.

Encrypting the counter with the key gives a 256 bit encrypted message which is uses to 
construct 8,16,32 of 64 bit random integers. After all 256 bits are consumed the internal 
counter is incremented and  encrypted again to generate a new encrypted message.  Small 
changes in the counter -or the key- result in big changes the encrypted output messages.

The fact that it's very difficult to decrypt the message without knowing the key makes it 
sound to use a simple counter as input. To be able to predict the random output of the 
engine one would need to solve a task that is similar to decrypt the message and recover 
the counter (once you can decrypt and have recovered the counter, the random output will 
be easy to predict). The Threefish cipher has been studies extensively and it has so far 
withstood decryption attacks. This give us extra trust that it's not easy to predict the 
output of the random engine, and that the random qualities are good.

Using an encrypted counter makes discard() very efficient. If we want to advance a billion 
numbers, we simply advance the internal counter will a billion and encrypt the new counter 
state. The discard operator has O(1) complexity, the whole random stream the random engine 
can produce has random access. 

Another useful property is that the encryption is invertible (you can decrypt the message 
and recover the counter if you know the key). This property ensures a 1-1 mapping between 
the counters and output messages, there are exactly as many different output messages are 
there are counter states.

It's undoable to decrypt the message without knowing the key, and using the wrong key 
gives a wrong decrypted counter. Small changes in the key result in big changes in the 
encryption/decryption process, if it weren't then recovering the key would be easy. The 
big unpredictable effect of changes in the key makes it plausible that different keys give 
independent random streams. If there was some dependency than that would allow security 
researches to attach the Threefish encryption scheme. 

The Threefry random engines uses a stripped down version of the Threefish encryption cipher. 
Threefish uses 72 encryption round, and Threefry uses 13 or 20 round. This reduction in 
rounds was done to speed up the random engine. 13 rounds were picked because that's the 
minimal number of rounds that still allow the engine to pass the  BigCrush test (159 of 
the 160 tests). The 20 rounds version is slower but has 7 extra rounds as a safety margin 
(and passes alls tests). 

## Random Quality Validation ##

The quality of the random number generates has been tested by running the  BigCrush test 
from the TestU01 framework. The BigCrush test consist of a suite of 160 statistical test 
and takes 3 hours to complete. I've tested a couple of engines, (it would be nice to test 
them all!).

|engine         | failed tests |
|---------------|-------------:|
|minstd_rand    |           107|
|mt11213b       |             3|
|mt19937        |             2|
|threefry13     |             1|
|threefry20     |             0|

More infor about the BigCrush test can be found here:

[TestU01 website](http://www.iro.umontreal.ca/~simardr/testu01/tu01.html)
[TestU01 documentation (pdf)](http://www.iro.umontreal.ca/~lecuyer/myftp/papers/testu01.pdf)

## Algorithmic Validation ##

The boost implementation passes all the test cases that are part of 
[Salmon et al.'s Random123 library](https://github.com/girving/random123/blob/master/examples/kat_vectors). 
The test script can be found in test-cat_vectors directory.
The implemenation also passes the random engine concept check.


## Speed ##

The speed was tested using the boost random/performance/random_speed.cpp program:

|engine        | speed                                |
|--------------|--------------------------------------|
|minstd_rand   | 4.8031 nsec/loop = 8.9818 CPU cycles
|mt11213b      | 4.1501 nsec/loop = 7.76069 CPU cycles
|mt19937       | 5.7962 nsec/loop = 10.8389 CPU cycles
|mt19937_64    | 4.2335 nsec/loop = 7.91664 CPU cycles
|threefry13    | 3.6672 nsec/loop = 6.85766 CPU cycles
|threefry20    | 5.2351 nsec/loop = 9.78964 CPU cycles
|threefry13_64 | 5.1499 nsec/loop = 9.63031 CPU cycles
|threefry20_64 | 8.3034 nsec/loop = 15.5274 CPU cycles
## References ##

The algorithm is described in "Parallel random numbers: as easy as 1, 2, 3"

**Parallel random numbers: as easy as 1, 2, 3** *Salmon, John K. and Moraes, Mark A. and 
Dror, Ron O. and Shaw, David E.* Proceedings of 2011 International Conference for High 
Performance Computing, Networking, Storage and Analysis. ACM 2011, isbn 978-1-4503-0771-0
