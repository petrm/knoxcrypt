/*
  The MIT License (MIT)

  Copyright (c) 2013 Ben H.D. Jones

  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in
  the Software without restriction, including without limitation the rights to
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
  the Software, and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef BFS_ENTRY_INFO_HPP__
#define BFS_ENTRY_INFO_HPP__

#include "bfs/EntryType.hpp"

#include <string>

namespace bfs
{

    class EntryInfo
    {
      public:
        EntryInfo(std::string const &fileName,
                  uint64_t const &fileSize,
                  EntryType const &entryType,
                  bool const writable,
                  uint64_t const firstFileBlock,
                  uint64_t const folderIndex);

        std::string filename() const;

        uint64_t size() const;

        EntryType type() const;

        bool writable() const;

        uint64_t firstFileBlock() const;

        uint64_t folderIndex() const;

      private:
        std::string m_fileName;
        uint64_t m_fileSize;
        EntryType m_entryType;
        bool m_writable;
        uint64_t m_firstFileBlock;
        uint64_t m_folderIndex;
    };

}

#endif // BFS_ENTRY_INFO_HPP__
