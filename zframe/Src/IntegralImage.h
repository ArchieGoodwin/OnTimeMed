/*!
 * \file
 * \brief ��������˾ ���ױ� ̡�� �е����� ��̩�ٳ�.
 * \author CholU Kim
 */
#ifndef _INTEGRALIMAGE_H_
#define _INTEGRALIMAGE_H_

#include "cvlibbase/inc/Mat.h"

namespace CVLib 
{

/**
 @brief  ������������ �е���
 */
class CVLIB_DECLSPEC IntegralImage : public Object
{
public:
	/**
	 @brief  �龢��
	 */
	IntegralImage (){};

	/**
	 @brief  �ıл�
	 */
    virtual ~IntegralImage(){};

	/**
	 @brief  ��������˾ ���ٳ�.
	 @param		pmImg		[in]	�ӵ�����
	 @param		pmSum		[out]	�˺��� ��������.
	 @param		pmSquare	[out]	�˺��� �����߼�������.
			�� �ĵ������� �ӵٻ��� NULL�˷� �����߼�������˾ ���׼� ʱ����.
	 */
	void	Process (Mat* pmImg, Mat* pmSum, Mat* pmSquare = NULL);

	/**
	 @brief  ��������˾ ���ٳ�.
	 @param		pmImg		[in]	�ӵ�����
	 @param		pmSum		[out]	�˺��� ��������.
	 @param		pmTilted	[out]	�˺��� �Һ��� ��������.
	 @param		pmSquare	[out]	�˺��� �����߼�������.
			�� �ĵ������� �ӵٻ��� NULL�˷� �����߼�������˾ ���׼� ʱ����.
	 */
	void	ProcessEx (Mat* pmImg, Mat* pmSum, Mat* pmTilted, Mat* pmSquare = NULL);

protected:
	void	iProcess_Byte (Mat* pmImg, Mat* pmSum, Mat* pmSquare = NULL);
	void	iProcess_int (Mat* pmImg, Mat* pmSum, Mat* pmSquare = NULL);
	void	iProcess_float (Mat* pmImg, Mat* pmSum, Mat* pmSquare = NULL);
};

}

#endif //_INTEGRALIMAGE_H_