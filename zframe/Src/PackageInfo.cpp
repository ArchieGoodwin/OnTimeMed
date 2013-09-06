//
//  PackageInfo.h
//  ExtractPack
//
//  Created by lion on 5/23/13.
//  Copyright (c) 2013 ChangShiYuan. All rights reserved.
//

#include "PackageInfo.h"

namespace CVLib
{

PackageInfo::PackageInfo()
{
    memset (this, 0, sizeof(PackageInfo));
    rows=cols=1;
}
PackageInfo::PackageInfo(const PackageInfo& t)
{
    width=t.width;
    height=t.height;
    strcpy(szID, t.szID);
//    refColor=t.refColor;
    backgroundColor=t.backgroundColor;
	dotColor=t.dotColor;
    strcpy (szItemCode, t.szItemCode);
    cols=t.cols;
	rows=t.rows;
	corners[0]=t.corners[0];
	corners[1]=t.corners[1];
	corners[2]=t.corners[2];
	corners[3]=t.corners[3];
}

PackageInfo::~PackageInfo()
{
}

bool PackageInfo::isValid()
{
    if (width==0 || height==0)
        return false;
    return true;
}

bool PackageInfo::decode (const char* szstr)
{
    int len=strlen(szstr);
    Vector<int> startIdx;
    Vector<int> endIdx;
    for (int i=0; i<len; i++)
    {
        if (szstr[i] == '<')
            startIdx.Add (i);
        if (szstr[i]=='>')
            endIdx.Add (i);
    }
    if (startIdx.GetSize() < 7)
        return false;
    char sztemp[512];
    strcpy (sztemp, szstr);
    for (int k=0; k<startIdx.GetSize(); k++)
    {
        int n1=startIdx[k];
        int n2=endIdx[k];
        if (n2<=n1+1)
            return false;
        const char* str=&sztemp[n1+1];
        sztemp[n2]=0;
        if (k==0)
        {
            Vector<int> values;
            if (!splitByComma (str, values))
                return false;
            if (values.GetSize()!=2)
                return false;
            width=values[0];
            height=values[1];
        }
        else if (k==1)
        {
            strcpy(szID, str);
        }
        else if (k==2)
        {
            Vector<int> values;
            if (!splitByComma (str, values))
                return false;
            if (values.GetSize()!=3)
                return false;
            backgroundColor = RGB(values[0], values[1], values[2]);
        }
        else if (k==3)
        {
            Vector<int> values;
            if (!splitByComma (str, values))
                return false;
            if (values.GetSize()!=3)
                return false;
            dotColor = RGB(values[0], values[1], values[2]);
        }
       /* else if (k==4)
        {
            Vector<int> values;
            if (!splitByComma (str, values))
                return false;
            if (values.GetSize()!=3)
                return false;
            dotColor = RGB(values[0], values[1], values[2]);
        }*/
        else if (k==4)
        {
            strcpy(szItemCode, str);
        }
        else if (k==5)
        {
            Vector<int> values;
            if (!splitByComma (str, values))
                return false;
            if (values.GetSize()!=2)
                return false;
            cols=values[0];
            rows=values[1];
        }
		else if (k>=6 && k<10)
		{
            Vector<int> values;
            if (!splitByComma (str, values))
                return false;
            if (values.GetSize()!=2)
                return false;
            corners[k-6].x=values[0];
            corners[k-6].y=values[1];
		}
	}
    return true;
}

bool    PackageInfo::splitByComma (const char* szstr, Vector<int>& values)
{
    char sztemp[512];
    strcpy (sztemp, szstr);
    int len=strlen(sztemp);
    int first=0;
    for (int i=0;i <len; i++)
    {
        if (sztemp[i]==',')
        {
            sztemp[i]=0;
            int n=atoi (&sztemp[first]);
            values.Add (n);
            first=i+1;
        }
    }
    if (first < len)
    {
        int n=atoi (&sztemp[first]);
        values.Add (n);
    }
    return true;
}

}