#pragma once

#include "cvlibbase/Inc/_cvlibbase.h"

namespace CVLib
{

CVLIB_DECLSPEC void thinning(Mat& im);
CVLIB_DECLSPEC void findRects(const Mat& mask, Vector<Rect>& rects);


}