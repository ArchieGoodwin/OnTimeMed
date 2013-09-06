/** 
 * @file
 * @brief		ÊïË±¿¸´Å º·ºçÁÄËÎ
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

/** @brief ºå½Ó Àµ»°»¤ ³óËËº¬ Àµ»°ÂÚ ËèÀâ¶®°¡ ÊÖË¾ °ÒË§ */
#define M_E_END_OF_RECORD				777

/** @brief  ¼è¼³´ô ´Ñ¶ÂÀË¶® Â÷Ë¼ ´çËËÀ°¹ÕËËº÷ ËË¶§ËË ¹¢¼³ÃúÂÜ */
#define M_E_PATH_NOT_VALID				-13010

/** @brief  ´çËËÀ°¹ÕËËº÷¶¦ ±¶¸ÒÂ×µØ°Ö ÃÕËº±ı ¸Ë°Ì´ó¼è Ê±ËÁ */
#define M_E_DB_NOT_FOUND				M_E_PATH_NOT_VALID

/** @brief  Àâ¶¹¼öºÈ Á¡ËË¹¹Ëæ ´İËÄ´ó²÷ Àâ¶¹¼öºÈ ID°¡ ´Ğµâ´óÊÌ ËØ¼è Ê±ËÁ */
#define M_E_TXN_NOT_FOUND				-11008

/** @brief  Á¡ËË¹¹Ë¾ »¶ºÂÂ×µØ°Ö Â×ÊîËº±ı ËË·ç ´´ËÎÂÙ ËË¶§Ì© Á¡ËË¹¹ËË ¼Á¼õÂÜ*/
#define M_E_TABLE_DUPLICATED			-12001

/** @brief  Â×±ıÌ© Á¡ËË¹¹ ¼³Ì©Ëæº· Â­´Å ËË¶§ËË ¼Ú¸ô´óÊÌ º¡Ë¦*/
#define M_E_FIELD_DUPLICATED			-12002

/** @brief  ËÍ´èº÷¶¦ »¶ºÂÂ×µØ°Ö Â×ÊîËº±ı ËË·ç ¼Á¼õÂÜ*/
#define M_E_INDEX_DUPLICATED			-12003

/** @brief  ´´ËÎÂÙ ËË¶§Ì© ´çËËÀ°¹ÕËËº÷°¡ ËË·ç ¼Á¼õÂÜ */
#define M_E_DB_DUPLICATED				-12006

/** @brief  multicolumn indexËæº· ¿¨µÑËË ¼Ú¸ô´ö */
#define M_E_INDEX_COLUMN_DUPLICATED		-12008

/** @brief  ¼³Ì©´ô º¬ºã°ªË¾ º¡Ë¦ÃÍÊ¿ Â×²÷ ÁÄµ¹·çÀ¾Ëæ ¼³Ì©´ó¼è Ê±Ë¼ °ªËË ËÓµÙ´ö*/
#define M_E_PARAM_NOT_VALID				-13001

/** @brief  ¼³Ì© ´ó¼è Ê±Ë¼ data typeË¾ º¡Ë¦Â×ÊîËÁ*/
#define M_E_DATATYPE_NOT_VALID			-13007

/** @brief  ¼³Ì© ´ó¼è Ê±Ë¼ Operator¶¦ º¡Ë¦Â×ÊîËÁ*/
#define M_E_OPERATOR_NOT_VALID			-13008

/** @brief  Á¡ËË¹¹··ËË »ù·Ã´ö */
#define M_E_TABLE_NAME_NOT_VALID		-13025

/** @brief  Â­´Å··ËË »ù·Ã´ö */
#define M_E_FIELD_NAME_NOT_VALID		-13026

/** @brief  ËÍ´èº÷··ËË »ù·Ã´ö */
#define M_E_INDEX_NAME_NOT_VALID		-13027

/** @brief  »¶ºÂÂÚ ºã ËØ²÷ Á¡ËË¹¹ ¾×´İ ±¶ºã¶¦ ²¿ÊİËÁ*/
#define M_E_NUM_OF_TABLE_OVERFLOW		-14001

/** @brief  ÂÙ Á¡ËË¹¹ËË °¡¼ìºã ËØ²÷ ¾×´İ Â­´Å ±¶ºã¶¦ ²¿ÊİËÁ*/
#define M_E_NUM_OF_FIELD_OVERFLOW		-14002

/** @brief  ËË·ç ·º´Ç ¿¤º· ±·¾Ë´ÉËË º¡Ë¦¼ÚËËÊÌº· ³ó ËËº¬ ¿¤º·¶¦ Êâ ºã ÊÖËÁ */ 
#define M_E_NUM_OF_CURSOR_OVERFLOW		-14005

/** @brief  QueryÌ© condition ±¶ºã°¡ ¾×´İ ±¶ºã¶¦ ²¿ËÁ*/ 
#define M_E_NUM_OF_COND_OVERFLOW		-14006

/** @brief  ÂÙ Á¡ËË¹¹ËË °¡¼ìºã ËØ²÷ ¾×´İ ËÍ´èº÷ ±¶ºã¶¦ ²¿ÊİËÁ*/
#define M_E_NUM_OF_INDEX_OVERFLOW		-14007

/** @brief  ÂÙ ËÍ´èº÷Ëæ ÁäÂÜ´õ ºã ËØ²÷ ¿¨µÑÌ© ¾×´İ ±¶ºã¶¦ ²¿ÊİËÁ*/
#define M_E_NUM_OF_INDEX_COLUMN_OVERFLOW	-14008

/** @brief  ´çËËÀ°¹ÕËËº÷¶¦ »¶ºÂÂ×µØ°Ö Â×ÊîËº±ı »¨ÃÔ±îÂÙËË ¹¢¼ÀÂÜ */
#define M_E_PERMISSION_NOT_ENOUGH		-15004

/** @brief  ÃÍ³ë °ÒµáÌ© DB Instance°¡ ËË·ç ÊâµØ ËØËÁ */
#define M_E_DB_IN_USE					-16005

/** @brief  DBÌ® LibraryÌ© º¼¼³°ª´ÉËË ³âµ¹º· OpenÂÚ ºã ÊÖËÁ */
#define M_E_DB_INCOMPATIBLE				-16006

/** @brief  Âï¼õ ´´»õ¼ÚËÍ ±½»¦ Àâ¶¹¼öºÈËË ËØ²÷ °ÒË§ */
#define M_E_ACTIVE_TXN_EXIST			-17007	


///////////////////////////////////////////
// MCScanner ±ãµÚ Êïµù

/** ¼³Ì©´ó¼è Ê±Ë¼ ·ç´ÑÊÌ À°ËÓ ¼è¼³ */
#define M_E_UNKNOWN_MEDIA_TYPE		-40002

/** DBËæ ÃÍ³ë Ê­ËËÁ¥ËË ¼Á¼õÂ×¼è Ê±²÷ °ÒË§ */
#define M_E_NOT_EXIST_ITEM			-40004

/** Create¶¦ ºãÃÔÂ×¼è Ê±ËÁ */
#define	M_E_NOT_CREATED				-40006

/** ËË·ç ·º´Ç ÁÄËÎËæ ´İÂÙ °Áº¡°¡ Ì°µê´ö */
#define M_E_END_OF_DETECTION		-40007

#define M_E_EXIST_SAME_ITEM			-40008


//////////////////////////////////////////////////////////////////////////
// º¡¼ê°Á»°±ãµÚ Êïµù	-5xxxx±ËµÛ

/** @brief  ·ËÃ£ÂÙ ±¨ÊÍ±¨Ì© ¼ÑºÏ¶¦ ²¿°Ê¸ÊËÁ */
#define M_E_NULL_POINTER				-50100

// MCImageEx ±ãµÚ Êïµù	-500xx±ËµÛ

/** Êéº¬Ì© ¿Í±¨°¡ ¹¢ºãµá ´óÊŞËØËÁ*/
#define M_E_IMAGE_SIZE_NEGATIVE				-50001

/** Êéº¬»ôµê°¡ ¼Á¼õÂ×¼è Ê±ËÁ*/
#define M_E_IMAGE_BUFFER_NULL				-50002

/** ¾¢ºÏÊéº¬»ôµê°¡ ¼Á¼õÂ×¼è Ê±ËÁ*/
#define M_E_IMAGE_SHRINK_BUFFER_NULL		-50003

/** GreyÊéº¬»ôµê°¡ ¼Á¼õÂ×¼è Ê±ËÁ*/
#define M_E_IMAGE_GREY_BUFFER_NULL			-50004

/** Ê²ºã ÊÖ²÷ Êéº¬»ôµêÂô»¥*/
#define M_E_UNKOWN_IMAGE_TYPE				-50005

/** »°Êéº¬»ôµê°¡ ÊÖ²÷´ç »°Êéº¬»ôµê¶¦ ¶®Ë¦Â×µØ°Ö »¤´ªÂÜ*/
#define M_E_NONE_COLOR_IMAGE_TYPE			-50006

/** Êéº¬»ôµêÂô»¥ËË ½ÓËË±ı²÷ ¼¿»õË¾ »¤´ªÂÜ*/
#define M_E_DIFF_IMAGE_TYPE					-50007

/**< Êéº¬Ì© ¿Í±¨°¡ Áì¼Ó¿Í±¨¸ó³Ş »õËÁ */
#define M_E_IMAGE_SIZE_SMALL				-50008

// ÊÑ°í°Á¾¥±ãµÚ Êïµù	-502xx±ËµÛ
/**
 * @brief ÊïË±¿¸´Å
 */
#define M_E_TERMINATEFACEDETECT				-50201

// ÊÑ°íËÍ»¥±ãµÚ Êïµù	-503xx±ËµÛ

/**
 @brief ËÓµÙÁÄµ¹¸¡À¾°¡ »ù·Ã´ö
 */
#define M_E_INIT_PARAM						-50301
/**
 @brief Àã¼ñËË ¼Á¼õÂ×¼è Ê±ËÁ
 */
#define M_E_NON_FEATURE						-50302
/**
 @brief º¡¼êÁÄËÎ(PersonPhoto.alb)ËË ¼Á¼õÂ×¼è Ê±ËÁ
 */
#define M_E_NOT_EXIST_PHOTO_FILE			-50303
/**
 @brief ¿Ğµ¹º÷À¾¼³¸ó°¡ ´ÉÊÌËØ²÷ ÁÄËÎ(ClusterInfo.dat)ËË ¼Á¼õÂ×¼è Ê±ËÁ
 */
#define M_E_NOT_EXIST_CLUSTER_FILE			-50304
/**
 @brief Àã¼ñ¼³¸ó°¡ ´ÉÊÌËØ²÷ ÁÄËÎ(feature.dat)ËË »ù·Ã´óÊî°º±ı »¯µá ´ĞµâÂÙ º¡¼êËË ÊÖ³Ş
 */
#define M_E_NOT_NEW_PHOTO					-50305
/**
 @brief ´´°á¾¡¾¥Ëæº· »¨Â²ÂÜ
 */
#define M_E_EXTRACT_PUPIL_FAILURE			-50306
/**
 @brief ´´°á¾¡¾¥Ëæº· »¨Â²ÂÜ
 */
#define M_E_CORRECTION_FAILURE				-50307
/**
 @brief º¡¼­»ôµê°¡ ÊÖ³Ş.
 */
#define	M_E_DATA_FILE_NOT_FOUND				-50308
/**
 @brief Ëè¼ê½ó±¨Ãù¶¦ ¼êÃÔÂ×¼è Ê±ËÁ.
 */
#define M_E_WITHOUT_INIT_ENGIN				-50309
/**
 @brief ÊÑ°íËË ¼Á¼õÂ×¼è Ê±²÷³Ş.
 */
#define M_E_NO_FACE							-50310
/**
 @brief ¿Ğµ¹º÷À¾·»µâËË ½Û¼¿´ó¼è Ê±Ê¾ËÁ.
 */
#define M_E_INIT_CLUSTERLIST				-50311
/**
 @brief ¿Ğµ¹º÷À¾·»µâËË ½Û¼¿´ó¼è Ê±Ê¾ËÁ.
 */
#define M_E_FEADFEATURE_FUNC				-50312
/**
 @brief ¿»¹ÊÂÜºã¼ÑºÏ°¡ NULLµá ´óÊŞ ËØ³Ş.
 */
#define M_E_CALLBACKPROC_NULL				-50313
/**
 @brief ÊÑ°íÀã¼ñËË Â×±ı´ª ÊÖ³Ş.
 */
#define M_E_NO_FACE_FEATURE					-50314
/**
 @brief	´İ±¨µÛ»ôµê°¡ ½Û¼¿´ó¼è Ê±ËÁ.
 */
#define M_E_NO_QUEUE						-50315

// Highlight±ãµÚ Êïµù	-504xx±ËµÛ
/** ´´¼¬ ¸¡·º¶® ÂÚ³ëËæ »¨Â² */
#define M_E_MEMORY_ALLOCATION_FAIL	-40001

#define M_E_CAPTURE_VIDEO			-50500

/**
* @brief ÊïË±°Áº¡½å¶® ¶ë¿Íµá
*/
#define	MC_ASSERT_ERROR( nErrorCode )	if ( nErrorCode != M_E_SUCCESS ) \
											return nErrorCode;
/**
* @brief ÊïË±°Áº¡½å¶® ¶ë¿Íµá
*/
#define	MC_CHECK_ERROR( nCheckErrorCode, nRetErrorCode )	if ( nCheckErrorCode != M_E_SUCCESS ) \
																return nRetErrorCode;

#endif // __MFERROR_H__





