/*
  Copyright (c) <2013-2014>, <BenHJ>
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

#include "knoxcrypt/FileBlockIterator.hpp"

namespace knoxcrypt
{

    FileBlockIterator::FileBlockIterator(SharedCoreIO const &io,
                                         uint64_t rootBlock,
                                         OpenDisposition const &openDisposition,
                                         SharedImageStream const &stream)
        : m_io(io)
        , m_openDisposition(openDisposition)
        , m_stream(stream)
        , m_workingFileBlock(FileBlock(io, rootBlock, openDisposition, stream))
    {
        // optimization, make sure global stream ptr is updated
        if(m_stream) {
            m_stream = m_workingFileBlock->getStream();
        }

    }

    FileBlockIterator::FileBlockIterator()
        : m_io()
        , m_openDisposition(OpenDisposition::buildAppendDisposition())
        , m_stream()
        , m_workingFileBlock()
    {
    }

    void
    FileBlockIterator::increment()
    {
        auto nextBlock = m_workingFileBlock->getNextIndex();
        if (nextBlock != m_workingFileBlock->getIndex()) {
            m_workingFileBlock = FileBlock(m_io, nextBlock, m_openDisposition, m_stream);
        } else {
            m_workingFileBlock = WorkingFileBlock();
        }
    }

    bool
    FileBlockIterator::equal(FileBlockIterator const &other) const
    {
        return this->m_workingFileBlock == other.m_workingFileBlock;
    }

    FileBlock
    FileBlockIterator::dereference() const
    {
        return *m_workingFileBlock;
    }

}
