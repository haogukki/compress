/**********************************************************
 *   Author          : Apriluestc
 *   Email           : 13669186256@163.com
 *   Last modified   : 2019-07-07 12:03
 *   Filename        : HuffFileCompress.h
 *   Description     : 
 * *******************************************************/

#ifndef COMPRESS_H
#define COMPRESS_H

#include <string>
#include <cstdio>
#include <cassert>
#include <stdlib.h>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <cstring>
#include <io.h>
#include <direct.h>

/*
 * #include <stdlib.h>
 * #include <sys/stat.h>
 * */

#include "HuffmanTree.h"

typedef long long LongType;

struct CharInfo {
	unsigned char _data;
	LongType _count;
    std::string _Code;
	CharInfo(LongType count = 0)
		:_count(count)
	{}

	CharInfo operator+(CharInfo& ch) {
		return CharInfo(_count + ch._count);
	}

	bool operator<(CharInfo &ch) {
		return _count < ch._count;
	}
};


class HuffFileCompress {
public:
	HuffFileCompress() {
		for (int i = 0; i < 256; i++) {
			_Infos[i]._data = i;
		}
	}

	const std::string CompressFile(std::string filename) {
        std::vector<std::string> file;
        std::string path = filename.c_str();
		getFiles(path, file);

		if (file.empty()) {
			return _CompressFile(filename);
		} else {
            std::string newpath = path;
			newpath += ".huf";
			// mkdir(newpath.c_str(), 0775);
			_mkdir(newpath.c_str());

			for (int i = 0; i < (int)file.size(); i++) {
				_CompressFile(file[i], newpath);
			}
			return newpath;
		}
	}

	const std::string UnCompressFile(std::string filename) {
        std::vector<std::string> file;
        std::string path = filename.c_str();
		getFiles(path, file);

		if (file.empty()) {
			return _UnCompressFile(filename);
		} else {
            std::string newpath = filename;
			for (int i = (int)filename.size() - 1; i >= 0; i--) {
				if (filename[i] == '.') {
					newpath.resize(i);
					break;
				}
			}
			newpath += ".uhuf";
			// mkdir(newpath.c_str(), 0775);
			_mkdir(newpath.c_str());

			for (int i = 0; i < (int)file.size(); i++) {
				_UnCompressFile(file[i], newpath);
			}
			return newpath;
		}
	}
protected:
	const std::string _CompressFile(const std::string filename, const std::string path = std::string()) {
		CreatHuffmanTree(filename.c_str());
        std::string CompressFileName = filename;
		CompressFileName += ".huf";

		FILE *fInput = fopen(filename.c_str(), "rb");
		assert(fInput);
		FILE *fOut = NULL;

        std::string configFileName = filename;
		configFileName += ".config";
		FILE *configOut = NULL;

		if (path.empty()) {
			fOut = fopen(CompressFileName.c_str(), "wb");
			if (fOut == NULL) {
				fclose(fOut);
				exit(EXIT_FAILURE);
			}
			configOut = fopen(configFileName.c_str(), "wb");
			assert(configOut);
		} else {
            std::string FileName;
			int i = filename.size() - 1;
			for (; i >= 0; i--) {
				if (filename[i] == '\\')
					break;
				FileName += filename[i];
			}
			reverse(FileName.begin(), FileName.end());

            std::string newpath = path;
			newpath += '\\';
			newpath += FileName;
			newpath += ".huf";
			fOut = fopen(newpath.c_str(), "wb");
			if (fOut == NULL) {
				fclose(fOut);
				exit(EXIT_FAILURE);
			}

			newpath = path;
			newpath += '\\';
			newpath += FileName;
			newpath += ".config";
			configOut = fopen(newpath.c_str(), "wb");
			assert(configOut);
		}

        std::string line;
		for (int i = 0; i < 256; i++) {
			if (_Infos[i]._count != 0) {
				int c = _Infos[i]._data;
				fputc(c, configOut);
				line += ",";
				char buffer[25] = { 0 };
				line += _itoa((int)_Infos[i]._count, buffer, 10);
				line += '\n';
				fwrite(line.c_str(), 1, line.size(), configOut);
				line.clear();
			}
		}
		fclose(configOut);

		int c = 0;
		int pos = 7;
		int ch = fgetc(fInput);
		while (ch != EOF) {
            std::string &code = _Infos[ch]._Code;
			for (size_t i = 0; i < code.size(); i++) {
				c |= ((code[i] - '0') << pos);
				pos--;
				if (pos < 0) {
					fputc(c, fOut);
					pos = 7;
					c = 0;
				}
			}
			ch = fgetc(fInput);
		}

		if (pos < 7) {
			fputc(c, fOut);
		}
		fclose(fOut);
		fclose(fInput);
		memset(_Infos, 0, 256 * sizeof(CharInfo));
		return CompressFileName;
	}

	const std::string _UnCompressFile(std::string filename, const std::string path = std::string()) {
		assert(!filename.empty());

        std::string name;
		name = filename;
		int i = 0;
        std::string posfix;
		for (i = (int)filename.size() - 1; i >= 0; --i) {
			posfix.push_back(filename[i]);
			if (filename[i] == '.')
				break;
		}
		reverse(posfix.begin(), posfix.end());

		if (posfix != ".huf") {
			return std::string();
		}
		name.resize(i);

        std::string UnCompressFileName = name;
		UnCompressFileName += ".uhuf";

        std::string configName = name;
		configName += ".config";
		FILE *fInput = fopen(filename.c_str(), "rb");
		assert(fInput);

		FILE *configInput = fopen(configName.c_str(), "rb");
		assert(configInput);

		FILE *fOut = NULL;
		if (path.empty()) {
			fOut = fopen(UnCompressFileName.c_str(), "wb");
			if (fOut == NULL) {
				fclose(fInput);
				exit(EXIT_FAILURE);
			}
		} else {
            std::string FileName;
			for (int i = (int)name.size() - 1; i >= 0; i--) {
				if (name[i] == '\\') {
					break;
				}
				FileName += name[i];
			}
			reverse(FileName.begin(), FileName.end());

            std::string newpath = path;
			newpath += "\\";
			newpath += FileName;
			newpath += ".uhuf";

			fOut = fopen(newpath.c_str(), "wb");
			if (fOut == NULL) {
				fclose(fInput);
				exit(EXIT_FAILURE);
			}
		}

        std::string line;
		int c = 0;
		while ((c = fgetc(configInput)) != EOF) {
			GetLine(configInput, line);
			_Infos[c]._count = atoi((&line[1]));
			line.clear();
		}
		fclose(configInput);

		int ch = 0;
		CharInfo invalid;
		for (int i = 0; i < 256; i++) {
			_Infos[i]._data = i;
		}
		HuffmanTree<CharInfo> ht(_Infos, 256, invalid);
		LongType count = ht.GetRoot()->_data._count;

		int pos = 7;
		c = 1;
		HuffmanNode<CharInfo> *root = ht.GetRoot();
		HuffmanNode<CharInfo> *cur = root;
		while (count > 0) {
			ch = fgetc(fInput);
			while (pos >= 0 && count > 0) {
				if (ch&(c << pos)) {
					cur = cur->right_;
				} else {
					cur = cur->left_;
				}
				if (cur->left_ == NULL && cur->right_ == NULL) {
					fputc(cur->_data._data, fOut);
					cur = root;
					count--;
				}
				pos--;
			}
			pos = 7;
		}
		fclose(fInput);
		fclose(fOut);
		memset(_Infos, 0, 256 * sizeof(CharInfo));
		return UnCompressFileName;
	}

protected:
	void CreatHuffmanTree(const char * filename) {
		assert(filename != NULL);
		for (int i = 0; i < 256; i++) {
			_Infos[i]._data = i;
		}

		FILE *fInput = fopen(filename, "rb");
		assert(fInput);
		int ch = 0;

		while ((ch = fgetc(fInput)) != EOF) {
			_Infos[ch]._count++;
		}

		CharInfo invalid;
		HuffmanTree<CharInfo> ht(_Infos, 256, invalid);

        std::string str;
		GetHufCode(ht.GetRoot(), str);
		fclose(fInput);
	}


	void GetHufCode(HuffmanNode<CharInfo>* root, std::string str) {
		if (root == NULL)
			return;
		if (root->left_ == NULL && root->right_ == NULL) {
			_Infos[root->_data._data]._Code = str;
			return;
		}
		GetHufCode(root->left_, str + '0');
		GetHufCode(root->right_, str + '1');
	}

	bool GetLine(FILE*& Input, std::string &line) {
		assert(Input);
		int ch = 0;
		ch = fgetc(Input);
		if (ch == EOF)
			return false;
		while (ch != EOF && ch != '\n') {
			line += ch;
			ch = fgetc(Input);
		}
		return true;
	}

	void getFiles(std::string path, std::vector<std::string>& files) {
		long   hFile = 0;
		struct _finddata_t fileinfo;
        std::string p;
		if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) {
			do {
				if ((fileinfo.attrib &  _A_SUBDIR)) {
					if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
						getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
				} else {
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
				}
			} while (_findnext(hFile, &fileinfo) == 0);
			_findclose(hFile);
		}
	}
private:
	CharInfo _Infos[256];
};

#endif
