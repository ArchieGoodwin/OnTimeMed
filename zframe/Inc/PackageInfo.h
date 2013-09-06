//
//  PackageInfo.h
//  ExtractPack
//
//  Created by lion on 5/23/13.
//  Copyright (c) 2013 ChangShiYuan. All rights reserved.
//

#ifndef ExtractPack_PackageInfo_h
#define ExtractPack_PackageInfo_h

#include "cvlibbase/inc/_cvlibbase.h"

namespace CVLib
{

class CVLIB_DECLSPEC PackageInfo
{
public:
    PackageInfo();
    PackageInfo(const PackageInfo& t);
    ~PackageInfo();
    
    bool    decode (const char* szstr);
    bool    isValid();
    
    int				width;                          // 0
    int				height;
    char			szID[256];                      // 1
//    COLORREF		refColor;                // 
    COLORREF		backgroundColor;         // 2
    COLORREF		dotColor;                // 3
    char			szItemCode[256];                // 4
    int				cols;							
    int				rows;                           // 5
	Point2i			corners[4];				// 6
    
private:
    bool    splitByComma (const char* szstr, CVLib::Array<int>& values);
    
};
    
}

#endif
