/**********************************************************
 *   Author          : Apriluestc
 *   Email           : 13669186256@163.com
 *   Last modified   : 2019-07-07 13:31
 *   Filename        : Main.cpp
 *   Description     : Lz77 算法测试代码
 *   主要包括类型文件的压缩与解压缩所花费的时间
 * *******************************************************/

#include <iostream>
#include <time.h>
#include <exception>

#include "Lz77.h"

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "usgae:[target] [object]" << std::endl;
        return 1;
    }
    LzCompress ls;
    clock_t start, finish;
    double compress = 0;
    double Uncompress = 0;
    start = clock();
    ls.Compress(argv[1]);
    finish = clock();
    compress = (double)(finish - start) / CLOCKS_PER_SEC;
    std::cout << "lz77 压缩程序的运行时间为：" << compress << "秒！" << std::endl;
    std::string filename = argv[1];
    filename.append(".fzip");
    start = clock();
    ls.UnCompress(filename);
    finish = clock();
    Uncompress = (double)(finish - start) / CLOCKS_PER_SEC;
    std::cout << "lz77 解压缩程序的运行时间为：" << Uncompress << "秒！" << std::endl;
	return 0;
}
