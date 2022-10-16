#pragma once

// I have tried use GPU to speed the inverting progress up with c++ amp but unfortunately failed.
//#define _SILENCE_AMP_DEPRECATION_WARNINGS true

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include <windows.h>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)
#define op(str) cout<<str<<"\n"
