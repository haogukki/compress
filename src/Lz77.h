/**********************************************************
 *   Author          : Apriluestc
 *   Email           : 13669186256@163.com
 *   Last modified   : 2019-07-07 11:28
 *   Filename        : Lz77.h
 *   Description     : 
 * *******************************************************/

#define _CRT_SECURE_NO_WARNINGS 1

#ifndef LZ77_H
#define LZ77_H

#include <string>
#include <cstdio>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <algorithm>

const size_t N = 2 * 32 * 1024;
enum {
    SlipBlock = 32 * 1024
}; // cache

typedef long long LongType;

class LzCompress {
public:
	LzCompress() {
		windows_.reserve(N);
		first_ = 0;
		last_ = 0;
	}

	std::string Compress(const std::string& filename) {
		return _ZIP_FileCompress(filename);
	}

	std::string UnCompress(const std::string& filename) {
		return _ZIP_UnCompress(filename);
	}

protected:
	std::string _ZIP_FileCompress(const std::string& filename) {
		assert(filename.c_str());
		std::string FirstCompressFileName = filename;
		FirstCompressFileName += ".fzip";

		FILE *fInput = fopen(filename.c_str(), "rb+");
		assert(fInput);
		FILE *fOut = fopen(FirstCompressFileName.c_str(), "wb+");
		assert(fOut);

		FILE *pWindows = fopen(filename.c_str(), "rb+");
		assert(pWindows);

		int ch = 0;
		ch = fgetc(fInput);

		LongType count = 0;
		unsigned char buf = 0;
		int flag = 7;
		while (ch != EOF) {
			long distance = 0;
			int length = 0;
			long OFFSET = ftell(fInput);
			if (OFFSET > SlipBlock) {
				fseek(pWindows, OFFSET - 1 - SlipBlock, SEEK_SET);
				distance = SlipBlock;
			} else {
				fseek(pWindows, 0, SEEK_SET);
				distance = OFFSET - 1;
			}

			if (distance > 1) {
				fseek(fInput, OFFSET - 1, SEEK_SET);
				length = FindRepetition(pWindows, fInput, distance);
				fseek(fInput, OFFSET, SEEK_SET);
			}

			if (length > 0) {
				fseek(fInput, length - 1, SEEK_CUR);

				buf |= (1 << flag);
				flag--;
				if (flag < 0) {
					fputc(buf, fOut);
					flag = 7;
					buf = 0;
				}

				int pos = 15;
				while (pos >= 0) {
					if (distance&(1 << pos))
						buf |= (1 << flag);
					else
						buf &= (~(1 << flag));
					flag--;
					if (flag < 0) {
						fputc(buf, fOut);
						flag = 7;
						buf = 0;
					}
					pos--;
				}

				pos = 7;
				while (pos >= 0) {
					if (length&(1 << pos))
						buf |= (1 << flag);
					else
						buf &= (~(1 << flag));
					flag--;
					if (flag < 0) {
						fputc(buf, fOut);
						flag = 7;
						buf = 0;
					}
					pos--;
				}
				count += 3;
			} else {
				buf &= (~(1 << flag));
				flag--;
				if (flag < 0) {
					fputc(buf, fOut);
					flag = 7;
					buf = 0;
				}

				int pos = 7;
				while (pos >= 0) {
					if (ch&(1 << pos))
						buf |= (1 << flag);
					else
						buf &= (~(1 << flag));
					flag--;
					if (flag < 0) {
						fputc(buf, fOut);
						flag = 7;
						buf = 0;
					}
					pos--;
				}
				count++;
			}
			ch = fgetc(fInput);
		}

		if (flag != 7) {
			fputc(buf, fOut);
		}
		fwrite(&count, 1, 8, fOut);
		fclose(fInput);
		fclose(fOut);
		fclose(pWindows);
		return FirstCompressFileName;
	}

	std::string _ZIP_UnCompress(const std::string& CompressFileName) {
		std::string UnCompressFileName = CompressFileName;
		UnCompressFileName += ".ufzip";

		FILE* fInput = fopen(CompressFileName.c_str(), "rb+");
		assert(fInput);

		FILE* fOut = fopen(UnCompressFileName.c_str(), "wb+");
		assert(fOut);

		FILE* pWindows = fopen(UnCompressFileName.c_str(), "rb+");
		assert(pWindows);

		LongType count;
		fseek(fInput, -8, SEEK_END);
		fread(&count, 1, 8, fInput);

		fseek(fInput, 0, SEEK_SET);
		int ch = 0;
		ch = fgetc(fInput);

		unsigned char buf = 0;
		int status = 0;
		int flag = 7;
		while (count > 0) {
			int distance = 0;
			int length = 0;
			status = ch & (1 << flag);
			flag--;
			if (flag < 0) {
				ch = fgetc(fInput);
				flag = 7;
			}

			if (status != 0) {
				int pos = 15;
				while (pos >= 0) {
					if (ch&(1 << flag))
						distance |= (1 << pos);
					else
						distance &= (~(1 << pos));
					flag--;
					if (flag < 0) {
						ch = fgetc(fInput);
						flag = 7;
					}
					pos--;
				}
				pos = 7;
				while (pos >= 0) {
					if (ch&(1 << flag))
						length |= (1 << pos);
					else
						length &= (~(1 << pos));
					flag--;
					if (flag < 0) {
						ch = fgetc(fInput);
						flag = 7;
					}
					pos--;
				}

				fflush(fOut);
				int OFFSET = ftell(fOut);
				fseek(pWindows, OFFSET - distance, SEEK_SET);
				while (length--) {
					int c = fgetc(pWindows);
					fputc(c, fOut);
				}
				count -= 3;
			}
			else {
				int pos = 7;
				while (pos >= 0) {
					if (ch&(1 << flag))
						buf |= (1 << pos);
					else
						buf &= (~(1 << pos));
					flag--;
					if (flag < 0) {
						ch = fgetc(fInput);
						flag = 7;
					}
					pos--;
				}
				fputc(buf, fOut);
				count--;
				buf = 0;
			}
		}
		fclose(fInput);
		fclose(fOut);
		fclose(pWindows);
		return UnCompressFileName;
	}
protected:
	int FindRepetition(FILE* pWindows, FILE *fInput, long& distance) {
		long OFFSET1 = ftell(pWindows);
		long OFFSET2 = ftell(fInput);
		int ch = 0;

		if ((size_t)OFFSET2 > last_) {
			windows_.clear();
			for (size_t i = 0; i < N; i++) {
				ch = fgetc(pWindows);
				if (ch == EOF)
					break;
				windows_.push_back(ch);
			}
			first_ = OFFSET1;
			last_ = windows_.size() + OFFSET1;
		}

		int length = GetRepetionlength(fInput, distance, OFFSET1);
		return length;
	}

	int  GetRepetionlength(FILE *fInput, long& distance, long pWindowsPos) {
		long OFFSET = ftell(fInput);

        std::vector<unsigned char> v;
		if (Getch(fInput, v) == false)
			return 0;

		size_t size = OFFSET - pWindowsPos;
		size_t index = pWindowsPos - first_;
		int length = 0;

		for (; index < size; ++index) {
			if (windows_[index] == v[0]) {
				size_t flag = index;
				size_t i = 0;
				while ((flag < size) && (length < 255)) {
					if (i == v.size() - 1) {
						if (Getch(fInput, v) == false)
							break;
					}
					if (windows_[flag] == v[i]) {
						length++;
						flag++;
						i++;
					} else
						break;
				}
				if (length >= 3) {
					distance = OFFSET - (index + first_);
					return length;
				}
				length = 0;
			}
		}
		return 0;
	}

	bool Getch(FILE *fInput, std::vector<unsigned char>& v) {
		int ch = 0;

		ch = fgetc(fInput);
		v.push_back(ch);

		if (ch == EOF)
			return false;
		else
			return true;
	}

private:
    std::vector<unsigned char> windows_;
	size_t first_;
	size_t last_;
};

#endif
