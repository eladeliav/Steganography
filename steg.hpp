#ifndef STEG_H
#define STEG_H

#pragma once

#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>

#define LOG(x) std::cout << x << std::endl

bool isBitSet(char ch, int pos);

#include "encode.hpp"
#include "decode.hpp"

#endif /* STEG_H */