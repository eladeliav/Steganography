#ifndef STEG_H
#define STEG_H

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#define LOG(x) std::cout << x << std::endl

bool isBitSet(char ch, int pos);

#include "encode.hpp"
#include "decode.hpp"

#endif /* STEG_H */