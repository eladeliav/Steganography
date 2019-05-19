#ifndef STEG_H
#define STEG_H

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#define FINAL_STRING "END_OF_STEG"
#define LEN_OF_FINAL_STRING 11
#define MAX_BUF 1024

#define LOG(x) std::cout << x << std::endl

bool isBitSet(char ch, int pos);

#endif /* STEG_H */