#include <iostream>
#include "HuffmanCompress.h"

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "usage : [target] [object]";
    }
    HuffFileCompress hs;
    hs.CompressFile(argv[1]);
    std::string filename = argv[1];
    filename.append(".huf");
    hs.UnCompressFile(filename);
    return 0;
}
