#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include "dynamicdimentions.h"

class IDXio
{
public:
dynamicdimentions recallIDX(std::string filename);

template <typename matrix>
matrix saveIDX(std::string filename, matrix tosave);

int reverseInt(int i)
{
	unsigned char c1, c2, c3, c4;

	c1 = i & 255;
	c2 = (i >> 8) & 255;
	c3 = (i >> 16) & 255;
	c4 = (i >> 24) & 255;

	return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
}
int toInt(char a, char b, char c, char d)
{
	unsigned char c1, c2, c3, c4;

	c1 = a;
	c2 = b;
	c3 = c;
	c4 = d;

	return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
}
};

inline dynamicdimentions IDXio::recallIDX(std::string filename)
{
	try {
		char prelim[5];
		std::ifstream trashexplorer(filename.c_str(), std::ifstream::binary);
		if (trashexplorer.is_open() != true) throw std::runtime_error("file was closed and could not be opened again");
		int magic_number = 0;
		trashexplorer.getline(prelim, 5);

		char type = prelim[2];
		char dimentions = prelim[3];

		magic_number = IDXio::toInt(prelim[0], prelim[1], prelim[2], prelim[3]);
		if (type == 59) {//double

		}
		else if (type == 43) {//float

		}
		else if (type == 27) {//int

		}
		else if (type == 11) {//short

		}
		else if (type == 8) {//unsigned byte
			std::vector<int> sizes;
			char* tempsizes = new char[(dimentions * 8)];

			trashexplorer.close();
			trashexplorer.open(filename.c_str(), std::ifstream::binary);

			if (trashexplorer.is_open() != true) throw std::runtime_error("file was closed and could not be opened again");

			trashexplorer.read(tempsizes, (long long int)(dimentions * 8));
			sizes.resize(dimentions);

			unsigned char c1 = 0, c2 = 0, c3 = 0, c4 = 0;
			long long int totalread = 0;
			for (int i = 0; i < dimentions; i = i + 1) {
				totalread = totalread + 4;
				c1 = 0, c2 = 0, c3 = 0, c4 = 0;
				c1 = tempsizes[totalread];
				c2 = tempsizes[totalread + 1];
				c3 = tempsizes[totalread + 2];
				c4 = tempsizes[totalread + 3];
				sizes[i] = IDXio::toInt(c1, c2, c3, c4);
			}
				totalread = totalread + 4;

			dynamicdimentions fullfile;
			fullfile.create(sizes);

			long long int filesize = 1;
			filesize = fullfile.size();

			char* tempfile = new char[filesize + totalread + 4];
			trashexplorer.seekg(0);
			trashexplorer.read(tempfile, filesize);
			
				for (long long int x = 0; x < filesize; x = x + 1) {
					try {

						if (x + totalread > filesize + totalread) throw std::length_error("continued reading from the file even though the file length was reached ??bad file??");
						if (x + totalread < 0) throw std::overflow_error("\"totalread\" overflowed ==file is too large==");
						fullfile.settruelocation(x, (unsigned char)tempfile[x + totalread]);

					}
					catch (std::length_error& e) {
						std::cerr << "\n=====\n=======\nexception: " << e.what() << "\n=======\n=====\n";
					}
					catch (std::overflow_error& e) {
						std::cerr << "\n=====\n=======\nexception: " << e.what() << "\n=======\n=====\n";
					}
					catch (...) {
						std::cerr << "\nfunction \"recallIDX\" has hit an unknown error\n";
					}
				}

				try {
					delete[] tempsizes;
					delete[] tempfile;
				}
				catch (std::exception& e) {
					std::cerr << "\n!=!=!=!\n!=!=!=!=!=!\nexception in IDXIO: memory deallocation error; a memory leak has likely occured\n\nfunction: recallIDX\nvariable: tempsizes or tempfile\n\nmore info:" << e.what() << "\n!=!=!=!=!=!\n!=!=!=!";
				}

			trashexplorer.close();
			return fullfile;
		}
		else if (type == 9) {//signed byte

		}
	}
	catch (std::exception& e) {
		std::cerr << "\n=====\n=======\nexception: " << e.what() << "\n=======\n=====\n";
	}
}

template<typename matrix>
inline matrix IDXio::saveIDX(std::string filename, matrix tosave)
{
	return matrix();
}

//a = (value & 0xFF); //extract first byte
//b = ((value >> 8) & 0xFF); //extract second byte
//c = ((value >> 16) & 0xFF); //extract third byte
//d = ((value >> 24) & 0xFF); //extract fourth byte