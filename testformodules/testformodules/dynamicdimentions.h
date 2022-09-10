#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <iostream>

class dynamicdimentions {
private:

	double* STORAGE = nullptr;

	int dimentions = 0;

	std::vector<int> dimsizes;

	std::vector<int> transfur;

	long long int Size = 1;

public:

	double get(std::vector<int> i);

	void set(std::vector<int> i, double x);

	void create(std::vector<int> i);

	void settruelocation(long long int i, double x);

	double gettruelocation(long long int i);

	void flip();

	long long int size(int x = 0);

	int to_true_value(int x, int dimention);
};
namespace xtra {
	template <typename datavalue>
	void fill(std::vector<datavalue>* v, double i);

	template<typename datavalue>
	void fill(std::vector<datavalue>* v, double i)
	{
		for (long long int x = ((long long int)v->size() - 1); x >= 0; x = x - 1) {
			v->at(x) = i;
		}
	}

template<typename datavalue>
	datavalue max(std::vector<datavalue> v);

	template<typename datavalue>
	datavalue max(std::vector<datavalue> v)
	{
		datavalue output{};
		for (int y = (int)v.size() - 1; y >= 0; y = y - 1) {
			output = std::max(v[y], output);
		}
		return output;
	}

	template<typename datavalue>
	datavalue errorcorrection(datavalue risk);

	template<typename datavalue>
	datavalue errorcorrection(datavalue risk)
	{
		if (risk == risk) {
			if (std::isinf(risk) == false) {
				if (std::isnan(risk) == false) {
					if (risk != 0 && fabs(risk) <= std::numeric_limits<double>::denorm_min())
					{
					}
					else {
						return risk;
					}
				}
			}
		}
		return 0;
	}

	template<typename datavalue>
	void vout(std::vector<datavalue> toprint, int newlines = 0);

	template<typename datavalue>
	void vout(std::vector<datavalue> toprint, int newlines)
	{
		for (int y = 0; y < (int)toprint.size(); y = y + 1) {
			std::cout << toprint[y];
			for (int x = 0; x < newlines; x = x + 1) {
				std::cout << "\n";
			}
		}
	}

	template<typename datavalue>
	void voutlable(std::vector<datavalue> toprint, int newlines = 0);

	template<typename datavalue>
	void voutlable(std::vector<datavalue> toprint, int newlines)
	{
		for (int y = 0; y < (int)toprint.size(); y = y + 1) {
			std::cout << y << " = " << toprint[y];
			for (int x = 0; x < newlines; x = x + 1) {
				std::cout << "\n";
			}
		}
	}



	template<typename datavalue>
	void mnistout(std::vector<datavalue> toprint, float threshold = 122.5);

	template<typename datavalue>
	void mnistout(std::vector<datavalue> toprint, float threshold)
	{
		for (int i = 0; i < 28; i = i + 1) {
			for (int x = 0; x < 28; x = x + 1) {
				if (toprint[(long long)x + (long long)(i*28)] > threshold) {
					std::cout << " X";
				}
				else {
					std::cout << "  ";
				}
			}
			std::cout << "\n";
		}
	}

	template<typename datavalue>
	bool contains(std::vector<datavalue> v, datavalue x);

	template<typename datavalue>
	bool contains(std::vector<datavalue> v, datavalue x)
	{
		for (int count = 0; count < v.size(); count = count + 1) {
			if (v[count] == x) {
				return 1;
			}
	}
		return 0;
	}

	template<typename datavalue>
	datavalue vmax(std::vector<datavalue> v);

	template<typename datavalue>
	datavalue vmax(std::vector<datavalue> v)
	{
		std::vector<datavalue> output;
		output.resize(v.size());
		for (int x = 0; x < (int)v.size(); x = x + 1) {
			for (int y = (int)v.size() - 1; y >= 0; y = y - 1) {
				if (contains(output, v[y]) == false) {
					output[x] = std::max(v[y], output[x]);
				}
			}
		}
		return output;
	}
}
