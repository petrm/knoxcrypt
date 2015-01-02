/*
  Copyright (c) <2014>, <BenHJ>
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.
  3. Neither the name of the copyright holder nor the names of its contributors
  may be used to endorse or promote products derived from this software without
  specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/// builds a specifc type of cipher that will be used by the main image stream

#ifndef TEASAFE_CIPHER_BUILDER_HPP__
#define TEASAFE_CIPHER_BUILDER_HPP__

#include "cipher/CryptoByteTransformer.hpp"
#include "cipher/NullByteTransformer.hpp"
#include "teasafe/CoreTeaSafeIO.hpp"

#include "cryptopp/aes.h"
#include "cryptopp/camellia.h"
#include "cryptopp/mars.h"
#include "cryptopp/rc5.h"
#include "cryptopp/rc6.h"
#include "cryptopp/serpent.h"
#include "cryptopp/shacal2.h"
#include "cryptopp/twofish.h"
#include "cryptopp/cast.h"

#include <memory>

#define BUILD_CIPHER(X)                                                              \
  return std::make_shared<cipher::CryptoByteTransformer<CryptoPP::X> >(io->password, \
                                                                       io->iv,       \ 
                                                                       io->iv2,      \
                                                                       io->iv3,      \
                                                                       io->iv3);     
namespace teasafe
{

    std::shared_ptr<cipher::IByteTransformer> buildCipherType(SharedCoreIO const &io)
    {
        if(io->cipher == 2) {
            BUILD_CIPHER(Twofish);
        } else if(io->cipher == 3) {
            BUILD_CIPHER(Serpent);
        } else if(io->cipher == 4) {
            BUILD_CIPHER(RC6);
        } else if(io->cipher == 5) {
            BUILD_CIPHER(MARS);
        } else if(io->cipher == 6) {
            BUILD_CIPHER(CAST256);
        } else if(io->cipher == 7) {
            BUILD_CIPHER(Camellia);
        } else if(io->cipher == 8) {
            BUILD_CIPHER(RC5);
        } else if(io->cipher == 9) {
            BUILD_CIPHER(SHACAL2);
        } else if(io->cipher == 0) {
            return std::make_shared<cipher::NullByteTransformer>(io->password,
                                                                 io->iv,
                                                                 io->iv2,
                                                                 io->iv3,
                                                                 io->iv3);
        } else {
            BUILD_CIPHER(AES);
        }
    }
}

#endif // TEASAFE_CIPHER_BUILDER_HPP__