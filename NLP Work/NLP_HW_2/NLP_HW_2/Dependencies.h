#pragma once

#include <regex>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <random>

/*

Written by Dom Amato 4/18/17

*/

using namespace std;

static double float2log(double val) {
	return log(val);
}

static double log2float(double val) {
	return exp(val);
}