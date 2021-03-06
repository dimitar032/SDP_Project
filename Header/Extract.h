#ifndef ExtractDec
#define ExtractDec

#include <string> // For strcmp() and strlen()
#include <fstream>
#include "BinaryTree.h"
#include "BitVector.h"
#include "PriorityQueue.h"
#include "Vector.h"
#include "FileStates.h"

namespace Extract
{
    enum EXTRACTION_ERRORS
    {
        CORRUPTED_ARCHIVE
    };

    bool ExtractFolder(std::fstream& archive, const char* WhereToExtract, size_t WhereToExtractLen);

    BinaryTree ExtractHuffmanTree(std::fstream& archive);
    void ExtractFile(std::fstream& archive, std::fstream& file, const BinaryTree& HuffmanTree, const char* fileName);

    size_t ReadFileName(char*& file_path, std::fstream& archive, const char* dir_path, size_t dir_path_len);
}

#endif // ExtractDec
