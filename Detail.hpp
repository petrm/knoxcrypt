#ifndef BFS_BFS_DETAIL_HPP__
#define BFS_BFS_DETAIL_HPP__


#include <fstream>
#include <iostream>
#include <stdint.h>

namespace bfs { namespace detail {

    uint64_t const METABLOCKS_BEGIN = 24;
    uint64_t const METABLOCK_SIZE = 32;
    uint64_t const MAX_FILENAME_LENGTH = 50;

    inline void convertInt64ToInt8Array(uint64_t const bigNum, uint8_t array[8])
    {
        array[0] = static_cast<uint8_t>((bigNum >> 56) & 0xFF);
        array[1] = static_cast<uint8_t>((bigNum >> 48) & 0xFF);
        array[2] = static_cast<uint8_t>((bigNum >> 40) & 0xFF);
        array[3] = static_cast<uint8_t>((bigNum >> 32) & 0xFF);
        array[4] = static_cast<uint8_t>((bigNum >> 24) & 0xFF);
        array[5] = static_cast<uint8_t>((bigNum >> 16) & 0xFF);
        array[6] = static_cast<uint8_t>((bigNum >> 8) & 0xFF);
        array[7] = static_cast<uint8_t>((bigNum) & 0xFF);
    }

    inline uint64_t convertInt8ArrayToInt64(uint8_t array[8])
    {
        return ((uint64_t)array[0] << 56) | ((uint64_t)array[1] << 48)  |
               ((uint64_t)array[2] << 40) | ((uint64_t)array[3] << 32) |
               ((uint64_t)array[4] << 24) | ((uint64_t)array[5] << 16)  |
               ((uint64_t)array[6] << 8) | ((uint64_t)array[7]);
    }

    /**
     * @brief gets the size of the BFS image
     * @param in the image stream
     * @return image size
     */
    inline uint64_t getImageSize(std::fstream &in)
    {
        in.seekg(0, in.end);
        uint64_t const bytes(in.tellg());
        return bytes;
    }

    /**
     * @brief get amount of space reserved for file data
     * @param in the image stream
     * @return amount of file space
     */
    inline uint64_t getBlockCount(std::fstream &in)
    {
        in.seekg(0, in.beg);
        uint8_t dat[8];
        (void)in.read((char*)dat, 8);
        return convertInt8ArrayToInt64(dat);
    }

    /**
     * @brief gets the number of files stored in the BFS
     * @param in the BFS image stream
     * @return the number of files
     */
    inline uint64_t getFileCount(std::fstream &in)
    {
        uint8_t blockCountBytes[8];
        in.seekg(0, in.beg);
        (void)in.read((char*)blockCountBytes, 8);
        uint64_t blockCount = convertInt8ArrayToInt64(blockCountBytes);
        in.seekg(blockCount + 8);
        uint8_t dat[8];
        (void)in.read((char*)dat, 8);
        return convertInt8ArrayToInt64(dat);
    }

    /**
     * @brief increments the file count
     * @param imagePath the path of the container
     */
    inline void incrementFileCount(std::fstream &in)
    {
        uint64_t orig = getFileCount(in);                    // convert to a 64 bit int
        ++orig;                                              // increment the count
        uint8_t countBytes[8];
        convertInt64ToInt8Array(orig, countBytes);           // convert back to bytes
        in.seekp((std::streampos)8);                         // seek to position 8
        in.write((char*)countBytes, 8);                      // write the bytes
    }

    /**
     * @brief gets the accumulated file size from the third block
     * @param in in the stream of the image
     * @return the accumulated size
     */
    inline uint64_t getAccumulatedFileSize(std::fstream &in)
    {
        in.seekg((std::streampos)16);                        // first 16 bytes are fs size + fileCount
        uint8_t sizeBytes[8];                                // buffer to store the current file accum size
        in.read((char*)sizeBytes, 8);                        // read in the file count bytes
        return convertInt8ArrayToInt64(sizeBytes);           // convert to a 64 bit int
    }

    /**
     * @brief updates the total space occupied by all files
     * @param imagePath the path of the container
     * @param size the size of the file that was last added
     */
    inline void updateFileSpaceAccumulator(std::fstream &in, uint64_t const size)
    {
        uint64_t orig = getAccumulatedFileSize(in);          // convert to a 64 bit int
        orig += size;                                        // update the accum size
        uint8_t sizeBytes[8];
        convertInt64ToInt8Array(orig, sizeBytes);            // convert back to bytes
        in.seekp(0, in.beg);                                 // seek to beginning
        in.seekp((std::streampos)16);                        // seek to position 8
        in.write((char*)sizeBytes, 8);                       // write the bytes
    }

    /**
     * @brief gets the offset of next free (unoccupied) metablock
     * @param in the bfs image
     * @return the offset
     */
    inline uint64_t getOffsetOfNextFreeMetaSpaceBlock(std::fstream &in)
    {
        uint64_t fileCount = getFileCount(in);
        return (fileCount * METABLOCK_SIZE) + METABLOCKS_BEGIN;
    }

    /**
     * @brief gets the offset of a specific meta entry for file N
     * @param in the bfs image
     * @param n the file entry
     * @return the offset
     */
    inline uint64_t getOffsetOfMetaBlockForFileN(std::fstream &in, uint64_t const n)
    {
        return ((n - 1) * METABLOCK_SIZE) + METABLOCKS_BEGIN;
    }

    /**
     * @brief gets the size of file n from its metadata
     * @param in the bfs image stream
     * @param n the file to lookup about
     * @return the file's size
     */
    inline uint64_t getSizeOfFileN(std::fstream &in, uint64_t const n)
    {
        uint64_t metaOffset = getOffsetOfMetaBlockForFileN(in, n);
        (void)in.seekg((std::streampos)metaOffset);
        uint8_t dat[8];
        (void)in.read((char*)dat, 8);
        return convertInt8ArrayToInt64(dat);
    }

    /**
     * @brief gets the offset of file n in the filespace
     * @param in the bfs image stream
     * @param n the file to lookup about
     * @return the file's offset
     */
    inline uint64_t getOffsetOfFileN(std::fstream &in, uint64_t const n)
    {
        uint64_t metaOffset = getOffsetOfMetaBlockForFileN(in, n);
        (void)in.seekg(0, in.beg);
        in.seekg((std::streampos)(metaOffset + 8)); // second 8 bytes is the position
        uint8_t dat[8];
        (void)in.read((char*)dat, 8);
        return convertInt8ArrayToInt64(dat);
    }

    /**
     * @brief gets the total amount of space allocated to metadata
     * @param bytes the size of the bfs image
     * @return space allocated to metadata
     */
    inline uint64_t getMetaDataSize(uint64_t const bytes)
    {
        return (static_cast<uint64_t>(bytes * 0.001) * METABLOCK_SIZE);
    }

    /**
     * @brief gets the offset of where file space begins
     * @param bytes the bfs image size
     * @return the offset
     */
    inline uint64_t getFileSpaceBeginOffset(uint64_t const bytes)
    {
        return getMetaDataSize(bytes) + METABLOCKS_BEGIN;
    }

    /**
     * @brief gets the offset of where we can store the next file
     * @param in the image stream
     * @return the offset
     */
    inline uint64_t getOffsetOfNextFreeFileSpaceBlock(std::fstream &in)
    {
        uint64_t const fileCount = getFileCount(in);

        if(fileCount == 0) {
            in.seekg(0, in.end);
            return METABLOCKS_BEGIN + getMetaDataSize((uint64_t)in.tellg());
        }

        uint64_t const offsetOfLastFile = getOffsetOfFileN(in, fileCount);
        uint64_t const sizeOfLastFile = getSizeOfFileN(in, fileCount);
        return (offsetOfLastFile + sizeOfLastFile);
    }

    /**
     * @brief gets the file name of a particular entry
     * @param in the image stream
     * @param n the file index
     * @return the file name
     */
    inline std::string getFileNameForFileN(std::fstream &in, uint64_t const n)
    {
        uint64_t fileOffset = getOffsetOfFileN(in, n);
        (void)in.seekg((std::streampos)fileOffset);
        uint8_t dat[MAX_FILENAME_LENGTH];
        (void)in.read((char*)dat, MAX_FILENAME_LENGTH);
        std::string name("");
        for(int i = 0; i < MAX_FILENAME_LENGTH; ++i) {
            if((char)dat[i] != '\0') {
                name.push_back((char)dat[i]);
            } else {
                break;
            }
        }
        return name;
    }
}
}

#endif // BFS_BFS_DETAIL_HPP__
