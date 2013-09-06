/*!
 \page page1 About cvlibbase-Computer Vision Basis Library 1.0
	Copyright 2007-2010 by ChongBong Information Center, KCC \n
	All rights reserved.\n
	\n

 \section sec ÁäÂÜÂ×²÷ ·º´Â´É
	CVLibÌ© cvlibbase²÷ Ãùº¬½å¶® ·ñ Â²ÀÀËÍ»¥Ëæº· ¶®Ë¦´ó²÷ ±¨½ó»ôµê°é¼¿´É±á ºãÂØº·°ÖÂÜºã´É, 
	ÊÞµÍ°¡¼è Ãùº¬Ì© ¿¸´è¼èÌº·º´Â ´ÐË¾ ÁäÂÜÂ×°ÖËØ»¡³£³Þ.
*/

/*!
 * \file	cvlibbase.h
 * \ingroup cvlibbase
 * \brief   Computer Vision Base LibraryÌ© ±¨¸õ·£¶®¹¢ÁÄËÎ
 * \author  ±®½èË§
 */

#ifndef _internal_CVLIBBASE_INC_H_
#define _internal_CVLIBBASE_INC_H_

// macros
#include "cvlibbase/inc/cvlibbaseVersion.h"
#include "cvlibbase/inc/cvlibbaseDef.h"
#include "cvlibbase/inc/cvlibMalloc.h"
#include "cvlibbase/inc/News.h"
#include "cvlibbase/inc/MintErrcode.h"

// basis classes
#include "cvlibbase/inc/Object.h"
#include "cvlibbase/inc/Exception.h"
#include "cvlibbase/inc/coImage.h"
#include "cvlibbase/inc/coImageDef.h"
#include "cvlibbase/inc/coImageCFG.h"
#include "cvlibbase/inc/coImageBMP.h"
#ifndef CVLIB_IMG_NOCODEC
#include "cvlibbase/inc/coImageJPG.h"
#include "cvlibbase/inc/coImageJAS.h"
#include "cvlibbase/inc/coImagePNG.h"
#include "cvlibbase/inc/coImageTIF.h"
#endif //CVLIB_IMG_NOCODEC
#include "cvlibbase/inc/ImageIter.h"
#include "cvlibbase/inc/Algorithm.h"
#include "cvlibbase/inc/BitOperation.h"
#include "cvlibbase/inc/ParamABC.h"
#include "cvlibbase/inc/PtrArray.h"
#include "cvlibbase/inc/PtrList.h"
#include "cvlibbase/inc/PtrQueue.h"
#include "cvlibbase/inc/PtrHash.h"
#include "cvlibbase/inc/Queue.h"
#include "cvlibbase/inc/Random.h"
#include "cvlibbase/inc/Timer.h"
#include "cvlibbase/inc/SString.h"
#include "cvlibbase/inc/StringArray.h"
#include "cvlibbase/inc/cvlibstructs.h"
#include "cvlibbase/inc/Tree.h"
#include "cvlibbase/inc/Graph.h"
#include "cvlibbase/inc/Plex.h"
#include "cvlibbase/inc/AutoBuffer.h"

// matrix and vector modules
#include "cvlibbase/inc/Mat.h"
#include "cvlibbase/inc/Vec.h"
#include "cvlibbase/inc/MatOperation.h"
#include "cvlibbase/inc/MatLib.h"

// mathematics classes and functions
#include "cvlibbase/inc/MathLib.h"
#include "cvlibbase/inc/CholeskyDecomposition.h"
#include "cvlibbase/inc/EigenvalueDecomposition.h"
#include "cvlibbase/inc/LUDecomposition.h"
#include "cvlibbase/inc/QRDecomposition.h"
#include "cvlibbase/inc/SingularValueDecomposition.h"
#include "cvlibbase/inc/Statistics.h"

// Utilitiy classes
#include "cvlibbase/inc/cvlibutil.h"

// File Operation classes
#include "cvlibbase/inc/IniFile.h"
#include "cvlibbase/inc/XFile.h"
#include "cvlibbase/inc/XFileDisk.h"
#include "cvlibbase/inc/XFileMem.h"

// template classes
#include "cvlibbase/inc/Template.h"
#include "cvlibbase/inc/TemplateFactory.h"

// pipe modules
#include "cvlibbase/inc/PumpABC.h"

#include "cvlibbase/inc/CommandLineParameters.h"

// #ifdef CVLIB_MEMCHECK
// #define new new(__FILE__, __LINE__)
// #endif

#endif //_internal_CVLIBBASE_INC_H_