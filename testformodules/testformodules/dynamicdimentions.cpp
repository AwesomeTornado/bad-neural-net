#include "dynamicdimentions.h"
#include <iostream>

template<typename T>
void pop_front(std::vector<T>& v)
{
	if (v.Size() > 0)
	{
		v.front() = std::move(v.back());
		v.pop_back();
	}
}

double dynamicdimentions::get(std::vector<int> i)
{
	int truelocation = 0;
	try {
	for (int x = 0; x < dimentions; x = x + 1) {
		if (i[x] >= dimsizes[x]) throw std::length_error("specified array location is out of bounds ==too large==\nerror triggered in function GET");
		if (i[x] < 0) throw std::length_error("specified array location is out of bounds ==too small==\nerror triggered in function GET");
		truelocation = truelocation + (i[x] * transfur[x]);
	}

		if (truelocation >= Size) throw std::length_error("specified array location is out of bounds ==too large==\nerror triggered in function GET");
		if (truelocation < 0) throw std::length_error("specified array location is out of bounds ==too small==\nerror triggered in function GET");
		return STORAGE[truelocation];
	}
	catch(std::length_error& e){
		std::cerr << "\n=====\n=======\nexception: " << e.what() << "\n=======\n=====\n";
	}
	return NULL;
}

void dynamicdimentions::set(std::vector<int> i, double x)
{
	int truelocation = 0;
	for (int x = 0; x < dimentions; x = x + 1) {
		truelocation = truelocation + (i[x] * transfur[x]);
	}
	try {
		if (truelocation >= Size) throw std::length_error("specified array location is out of bounds ==too large==\nerror triggered in function SET");
		if (truelocation < 0) throw std::length_error("specified array location is out of bounds ==too small==\nerror triggered in function SET");
		STORAGE[truelocation] = x;
	}
	catch (std::length_error& e) {
		std::cerr << "\n=====\n=======\nexception: " << e.what() << "\n=======\n=====\n";
	}
}

void dynamicdimentions::create(std::vector<int> i)
{
	Size = 1;
	dimentions = (int)i.size();
	dimsizes.resize(dimentions);
	transfur.resize(dimentions);
	for (int y = 0; y < dimentions; y = y + 1) {
		Size = Size * i[y];
		dimsizes[y] = i[y];
	}
	STORAGE = new double[Size];
	for (int a = 0; a < Size; a = a + 1) {
		STORAGE[a] = NULL;
	}
	int transfurtemp = 1;
	for (int p = 0; p < dimentions; p = p + 1) {
		transfurtemp = 1;
		for (int q = 0; q < p; q = q + 1) {
			transfurtemp = transfurtemp * dimsizes[long long(dimentions - (q + 1))];
		}
		transfur[long long(dimentions - (p + 1))] = transfurtemp;
	}
}

void dynamicdimentions::settruelocation(long long int i, double x)
{
	try {
		if (i >= Size) throw std::length_error("specified array location is out of bounds ==too large==\nerror triggered in function SETTRUELOCATION");
		if (i < 0) throw std::length_error("specified array location is out of bounds ==too small==\nerror triggered in function SETTRUELOCATION");
		STORAGE[i] = x;
	}
	catch (std::length_error& e) {
		std::cerr << "\n=====\n=======\nexception: " << e.what() << "\n=======\n=====\n";
	}
}

double dynamicdimentions::gettruelocation(long long int i)
{
	try {
		if (i >= Size) throw std::length_error("specified array location is out of bounds ==too large==\nerror triggered in function GETTRUELOCATION");
		if (i < 0) throw std::length_error("specified array location is out of bounds ==too small==\nerror triggered in function GETTRUELOCATION");
		return STORAGE[i];
	}
	catch (std::length_error& e) {
		std::cerr << "\n=====\n=======\nexception: " << e.what() << "\n=======\n=====\n";
	}
	return NULL;
}

void dynamicdimentions::flip()
{
	double* temparrayforflip = new double[Size];
	for (long long int x = Size-1; x >= 0; x = x - 1) {
		temparrayforflip[x] = STORAGE[Size - (static_cast<long long>(x) + 1)];
	}
	delete[] STORAGE;
	STORAGE = temparrayforflip;
}

long long int dynamicdimentions::size(int x)
{
	if (x == 0) {
	return Size;
	}
	else {
		try {
			if (x >= dimentions) throw std::length_error("specified array dimention is out of bounds ==too large==\nerror triggered in function SIZE");
			if (x < 0) throw std::length_error("specified array dimention is out of bounds ==too small==\nerror triggered in function SIZE");
			return dimsizes[x];
		}
		catch (std::length_error& e) {
			std::cerr << "\n=====\n=======\nexception: " << e.what() << "\n=======\n=====\n";
		}
	}

}

int dynamicdimentions::to_true_value(int x, int dimention)
{
	return x * transfur[dimention];
}
