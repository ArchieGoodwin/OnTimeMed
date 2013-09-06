/** 
 * @file
 * @brief		��˱���� ��������
 */

#ifndef __MFERROR_H__
#define __MFERROR_H__

#define M_E_SUCCESS						1
#define M_E_ERROR						0

// General errors
#define M_E_BAD_COMMAND					-1002
#define M_E_CRC							-1003
#define M_E_INVALID_PASSWORD			-1004
#define M_E_INVALID_PARAMETER			-1005
#define M_E_INVALID_POINTER				-1006
#define M_E_BUSY						-1007

// Memory errors
#define M_E_OUT_OF_MEMORY				-1100	
#define M_E_BUFFER_OVERFLOW				-1101
#define M_E_MIS_USE						-1102
#define M_E_OUT_OF_RANGE				-1103
	
// File errors	
#define M_E_FILE_NOT_FOUND				-1201
#define M_E_PATH_NOT_FOUND				-1202
#define M_E_ACCESS_DENIED				-1203
#define M_E_INVALID_HANDLE				-1204
#define M_E_BAD_FORMAT					-1205
#define M_E_INVALID_ACCESS				-1206
#define M_E_WRITE_PROTECT				-1207
#define M_E_NOT_READY					-1208
#define M_E_SEEK						-1209
#define M_E_WRITE_FAULT					-1210
#define M_E_READ_FAULT					-1211
#define M_E_DISK_FULL					-1212
#define M_E_SHARING_VIOLATION			-1213
#define M_E_LOCK_VIOLATION				-1214

// Metadata Extractor related errors (To be deleted. 06/09/15)
#define M_E_UNKNOWN_ID					-1300
#define M_E_INVALID_ID					-1301
#define M_E_INVALID_LENGTH				-1302
#define M_E_INVALID_DATA				-1303
#define M_E_NOT_SUPPORTED				-1304
#define M_E_OUTOFSCOPE					-1305
#define M_E_REFCOUNT					-1306
#define M_E_EMPTY_TAGS					-1307
#define M_E_DECODE						-1308

// Metadata Extractor related errors (New. ysgkim 06/09/15)
#define M_E_FORMAT_UNKNOWN				-1310
#define M_E_FORMAT_INVALID				-1311
#define M_E_FILE_OPEN					-1312
#define M_E_FILE_READ					-1313
#define M_E_FILE_SEEK					-1314
#define M_E_FILE_WRITE					-1315
#define M_E_FILE_CLOSE					-1316

// MintCore errors
#define M_E_INVALID_USERSCHEME			-1400
#define M_E_INVALID_DB					-1401
#define M_E_INVALID_CLUSTERING			-1402
	
// ADAdmin errors	
#define	M_E_NOT_GUARANTEE_SPM			-1600

// Hardware errors
#define M_E_DEV_NOT_READY				-2000
#define M_E_DEV_EXTERNAL_ERR			-2001
#define M_E_DEV_NOT_SUPPORTED			-2002
#define M_E_DEV_NOT_AVAILABLE			-2003
#define M_E_DEV_BUSY					-2004
#define M_E_DEV_INVALID_STATE			-2005


// DB errors

#define M_E_EXTERN_ERROR				-1

/** @brief ��� ������ ���˺� ������ ���ⶮ�� ��˾ ��˧ */
#define M_E_END_OF_RECORD				777

/** @brief  �輳�� �Ѷ��˶� ��˼ ���������˺� �˶��� �������� */
#define M_E_PATH_NOT_VALID				-13010

/** @brief  ���������˺��� �����׵ذ� ��˺�� �˰̴�� ʱ�� */
#define M_E_DB_NOT_FOUND				M_E_PATH_NOT_VALID

/** @brief  �ⶹ���� ���˹��� ���Ĵ�� �ⶹ���� ID�� �е���� �ؼ� ʱ�� */
#define M_E_TXN_NOT_FOUND				-11008

/** @brief  ���˹�˾ �����׵ذ� ����˺�� �˷� ������ �˶�̩ ���˹��� ������*/
#define M_E_TABLE_DUPLICATED			-12001

/** @brief  �ױ�̩ ���˹� ��̩�溷 ­�� �˶��� �ڸ����� ��˦*/
#define M_E_FIELD_DUPLICATED			-12002

/** @brief  �ʹ���� �����׵ذ� ����˺�� �˷� ������*/
#define M_E_INDEX_DUPLICATED			-12003

/** @brief  ������ �˶�̩ ���������˺��� �˷� ������ */
#define M_E_DB_DUPLICATED				-12006

/** @brief  multicolumn index�溷 ������ �ڸ��� */
#define M_E_INDEX_COLUMN_DUPLICATED		-12008

/** @brief  ��̩�� ���㰪˾ ��˦��ʿ �ײ� �ĵ������� ��̩��� ʱ˼ ���� �ӵٴ�*/
#define M_E_PARAM_NOT_VALID				-13001

/** @brief  ��̩ ��� ʱ˼ data type˾ ��˦������*/
#define M_E_DATATYPE_NOT_VALID			-13007

/** @brief  ��̩ ��� ʱ˼ Operator�� ��˦������*/
#define M_E_OPERATOR_NOT_VALID			-13008

/** @brief  ���˹����� ���ô� */
#define M_E_TABLE_NAME_NOT_VALID		-13025

/** @brief  ­�ŷ��� ���ô� */
#define M_E_FIELD_NAME_NOT_VALID		-13026

/** @brief  �ʹ������ ���ô� */
#define M_E_INDEX_NAME_NOT_VALID		-13027

/** @brief  ������ �� �ز� ���˹� �״� ���㶦 ������*/
#define M_E_NUM_OF_TABLE_OVERFLOW		-14001

/** @brief  �� ���˹��� ����� �ز� �״� ­�� ���㶦 ������*/
#define M_E_NUM_OF_FIELD_OVERFLOW		-14002

/** @brief  �˷� ���� ���� ���˴��� ��˦�����̺� �� �˺� ������ �� �� ���� */ 
#define M_E_NUM_OF_CURSOR_OVERFLOW		-14005

/** @brief  Query̩ condition ���㰡 �״� ���㶦 ����*/ 
#define M_E_NUM_OF_COND_OVERFLOW		-14006

/** @brief  �� ���˹��� ����� �ز� �״� �ʹ�� ���㶦 ������*/
#define M_E_NUM_OF_INDEX_OVERFLOW		-14007

/** @brief  �� �ʹ���� ���ܴ� �� �ز� ����̩ �״� ���㶦 ������*/
#define M_E_NUM_OF_INDEX_COLUMN_OVERFLOW	-14008

/** @brief  ���������˺��� �����׵ذ� ����˺�� ���Ա����� ������ */
#define M_E_PERMISSION_NOT_ENOUGH		-15004

/** @brief  �ͳ� �ҵ�̩ DB Instance�� �˷� ��� ���� */
#define M_E_DB_IN_USE					-16005

/** @brief  DB̮ Library̩ ���������� �⵹�� Open�� �� ���� */
#define M_E_DB_INCOMPATIBLE				-16006

/** @brief  ��� �������� ���� �ⶹ������ �ز� ��˧ */
#define M_E_ACTIVE_TXN_EXIST			-17007	


///////////////////////////////////////////
// MCScanner ��� ���

/** ��̩��� ʱ˼ ����� ���� �輳 */
#define M_E_UNKNOWN_MEDIA_TYPE		-40002

/** DB�� �ͳ� ʭ������ �����׼� ʱ�� ��˧ */
#define M_E_NOT_EXIST_ITEM			-40004

/** Create�� �����׼� ʱ�� */
#define	M_E_NOT_CREATED				-40006

/** �˷� ���� ������ ���� ������ ̰��� */
#define M_E_END_OF_DETECTION		-40007

#define M_E_EXIST_SAME_ITEM			-40008


//////////////////////////////////////////////////////////////////////////
// ���������� ���	-5xxxx�˵�

/** @brief  ��ã�� ���ͱ�̩ �Ѻ϶� ���ʸ��� */
#define M_E_NULL_POINTER				-50100

// MCImageEx ��� ���	-500xx�˵�

/** �麬̩ �ͱ��� ����� ��������*/
#define M_E_IMAGE_SIZE_NEGATIVE				-50001

/** �麬���갡 �����׼� ʱ��*/
#define M_E_IMAGE_BUFFER_NULL				-50002

/** �����麬���갡 �����׼� ʱ��*/
#define M_E_IMAGE_SHRINK_BUFFER_NULL		-50003

/** Grey�麬���갡 �����׼� ʱ��*/
#define M_E_IMAGE_GREY_BUFFER_NULL			-50004

/** ʲ�� �ֲ� �麬��������*/
#define M_E_UNKOWN_IMAGE_TYPE				-50005

/** ���麬���갡 �ֲ��� ���麬���궦 ��˦�׵ذ� ������*/
#define M_E_NONE_COLOR_IMAGE_TYPE			-50006

/** �麬���������� ���˱��� ����˾ ������*/
#define M_E_DIFF_IMAGE_TYPE					-50007

/**< �麬̩ �ͱ��� ��ӿͱ���� ���� */
#define M_E_IMAGE_SIZE_SMALL				-50008

// �Ѱ������� ���	-502xx�˵�
/**
 * @brief ��˱����
 */
#define M_E_TERMINATEFACEDETECT				-50201

// �Ѱ��ͻ���� ���	-503xx�˵�

/**
 @brief �ӵ��ĵ������� ���ô�
 */
#define M_E_INIT_PARAM						-50301
/**
 @brief ����� �����׼� ʱ��
 */
#define M_E_NON_FEATURE						-50302
/**
 @brief ��������(PersonPhoto.alb)�� �����׼� ʱ��
 */
#define M_E_NOT_EXIST_PHOTO_FILE			-50303
/**
 @brief �е��������� �����ز� ����(ClusterInfo.dat)�� �����׼� ʱ��
 */
#define M_E_NOT_EXIST_CLUSTER_FILE			-50304
/**
 @brief ��񼳸� �����ز� ����(feature.dat)�� ���ô���� ���� �е��� ������ �ֳ�
 */
#define M_E_NOT_NEW_PHOTO					-50305
/**
 @brief ���ᾡ���溷 ��²��
 */
#define M_E_EXTRACT_PUPIL_FAILURE			-50306
/**
 @brief ���ᾡ���溷 ��²��
 */
#define M_E_CORRECTION_FAILURE				-50307
/**
 @brief �������갡 �ֳ�.
 */
#define	M_E_DATA_FILE_NOT_FOUND				-50308
/**
 @brief �������� �����׼� ʱ��.
 */
#define M_E_WITHOUT_INIT_ENGIN				-50309
/**
 @brief �Ѱ��� �����׼� ʱ����.
 */
#define M_E_NO_FACE							-50310
/**
 @brief �е����������� �ۼ���� ʱʾ��.
 */
#define M_E_INIT_CLUSTERLIST				-50311
/**
 @brief �е����������� �ۼ���� ʱʾ��.
 */
#define M_E_FEADFEATURE_FUNC				-50312
/**
 @brief �����ܺ�Ѻϰ� NULL�� ���� �س�.
 */
#define M_E_CALLBACKPROC_NULL				-50313
/**
 @brief �Ѱ������ �ױ��� �ֳ�.
 */
#define M_E_NO_FACE_FEATURE					-50314
/**
 @brief	�ݱ��ۻ��갡 �ۼ���� ʱ��.
 */
#define M_E_NO_QUEUE						-50315

// Highlight��� ���	-504xx�˵�
/** ���� ������ �ڳ��� ��² */
#define M_E_MEMORY_ALLOCATION_FAIL	-40001

#define M_E_CAPTURE_VIDEO			-50500

/**
* @brief ��˱�����嶮 ��͵�
*/
#define	MC_ASSERT_ERROR( nErrorCode )	if ( nErrorCode != M_E_SUCCESS ) \
											return nErrorCode;
/**
* @brief ��˱�����嶮 ��͵�
*/
#define	MC_CHECK_ERROR( nCheckErrorCode, nRetErrorCode )	if ( nCheckErrorCode != M_E_SUCCESS ) \
																return nRetErrorCode;

#endif // __MFERROR_H__





