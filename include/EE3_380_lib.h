/**
 * \file      	EE3_380_lib.h
 * \author    	Leo Bai
 * \version   	1.08
 * \date      	2012.08.01
 * \brief     	RMVA380SC �����DSP����ͷ�ļ�������ϵͳ����ʹ�õĺ궨��ͽṹ�嶨�塣
 * \note		���ļ��е��κ����ݽ�����ѯ���������û��Զ�����ģ������޷���֤ϵͳ��ȷ����
**/


/***DSPģ��ע��궨��***/
#ifndef	_EE3_LIB_H
#define	_EE3_LIB_H

/***��ѧ�л������궨��***/
#define SWITCH_TO_GENERAL_FILTER 0
#define SWITCH_TO_POLARIZER_FILTER 1

/***��������ź궨��***/
#define	DETECT_LIGHT_REGION_1	1u
#define	DETECT_LIGHT_REGION_2	2u
#define	DETECT_LIGHT_REGION_3	3u
#define	DETECT_LIGHT_REGION_4	4u
#define	DETECT_LIGHT_REGION_5	5u

/***�������ģʽ�궨��***/
#define	CAMERA_MODE_TRG	0u
#define	CAMERA_MODE_FCON	1u
#define	CAMERA_MODE_HFR	2u
#define	CAMERA_MODE_HFRBIN	3u

/***IO��ź궨��***/
#define	PROGRAMMABLEI_IO_1	0u
#define	PROGRAMMABLEI_IO_2	1u
#define	PROGRAMMABLEI_IO_3	2u
#define	PROGRAMMABLEI_IO_4	3u
#define	PROGRAMMABLEI_IO_5	4u
#define	PROGRAMMABLEI_IO_6	5u
#define	PROGRAMMABLEI_IO_7	6u
#define	PROGRAMMABLEI_IO_8	7u
#define	PROGRAMMABLEI_IO_9	8u
#define	PROGRAMMABLEI_IO_10	9u
#define	PROGRAMMABLEI_IO_11	10u
#define	PROGRAMMABLEI_IO_12	11u

/***Cache ���÷�ʽ�궨��***/
#define	EE3_L1_CACHE_SIZE_0K	(0x00000000u)
#define	EE3_L1_CACHE_SIZE_4K	(0x00000001u)
#define	EE3_L1_CACHE_SIZE_8K	(0x00000002u)
#define	EE3_L1_CACHE_SIZE_16K	(0x00000003u)
#define	EE3_L1_CACHE_SIZE_32K	(0x00000004u)
#define	EE3_L2_CACHE_SIZE_0K	(0x00000000u)
#define	EE3_L2_CACHE_SIZE_32K	(0x00000001u)
#define	EE3_L2_CACHE_SIZE_64K	(0x00000002u)
#define	EE3_L2_CACHE_SIZE_128K	(0x00000003u)
#define	EE3_L2_CACHE_SIZE_256K	(0x00000004u)
#define	EE3_L2_CACHE_SIZE_MAX	(0x00000005u)

/***RGB�ɼ����и�ʽ�궨��***/
#define RGB_ARRAY_FORMATIS_BGR				0
#define RGB_ARRAY_FORMATIS_RGB				1

//��ǰͼ�񴫸��������зֱ���
#define FINE_ROW						1088u	 //1088u	  
#define FINE_COL						1920u    //1920u  

/***ͼ����ǿ�ȼ��궨��***/
#define IMG_ENHANCEMENT_NONE					0x00u 
#define IMG_ENHANCEMENT_LEVEL1 				0x01u 
#define IMG_ENHANCEMENT_LEVEL2 				0x02u 
#define IMG_ENHANCEMENT_LEVEL3 				0x03u

/***����������ʱ��ģʽ�궨��***/
#define EE3_380_DRCLK_NORMAL				0			//fps = 15
#define	EE3_380_DRCLK_HF					0x01u		//fps = 25

/***CCD PGA RANGE�궨��***/
#define	PGA_LOWLIMIT						0.0
#define PGA_HIGHLIMIT						36.0

/***JPG����������YUV��ʽ�궨��***/
#define	XDM_YUV_420P						1u
#define	XDM_YUV_422ILE						4u

/***����ģʽ�µĴ���Դ��ʾ�궨��***/
#define	TRGSUR_IO1							0u
#define	TRGSUR_IO2							1u
#define	TRGSUR_IO3							2u
#define	TRGSUR_IO4							3u
#define	TRGSUR_IO5							4u
#define	TRGSUR_IO6							5u
#define	TRGSUR_IO7							6u
#define	TRGSUR_IO8							7u
#define	TRGSUR_IO9							8u
#define	TRGSUR_IO10							9u
#define	TRGSUR_IO11							10u
#define	TRGSUR_IO12							11u
#define	TRGSUR_SOFT							12u

/***ͼ����ת��ʾ�궨��***/
#define	IMGR_DEGREE0						0u
#define	IMGR_DEGREE90						1u
#define	IMGR_DEGREE270						2u


/***H.264��Ƶ��ѹ�������ͼ��ֱ��ʺ궨��***/
/***********	Resolution Configuration of RMVA310SC/RMVA310SN/RMVA310SP H.264 Video Streaming  ***********/
#define	ENC_1600_1216					0x05u															
#define	ENC_800_544						0x04u														
#define	ENC_1600_1216_2_1080P			0x1du											
#define	ENC_1600_1216_2_720P			0x1eu											
#define	ENC_1600_1216_2_D1				0x59u													
#define	ENC_1600_1216_2_CIF				0x45u												
#define	ENC_800_544_2_1080P				0x1bu														
#define	ENC_800_544_2_720P				0x1cu														
#define	ENC_800_544_2_D1				0x58u														
#define	ENC_800_544_2_CIF				0x44u	
#define	ENC_310S_DS_1080P_CIF			0x85u													//˫��������ʱ
#define ENC_310S_DS_FR_CIF				0x71u													//˫��������ʱ
/***********	Resolution Configuration of RMVA385SC/RMVA385SP H.264 Video Streaming  ***********/
#define	ENC_1600_1216					0x05u															
#define	ENC_800_544						0x04u														
#define	ENC_1600_1216_2_1080P			0x1du											
#define	ENC_1600_1216_2_720P			0x1eu											
#define	ENC_1600_1216_2_D1				0x59u													
#define	ENC_1600_1216_2_CIF				0x45u
#define	ENC_800_600						0x6cu
#define	ENC_800_600_2_1080P				0x6du
#define	ENC_800_600_2_720P				0x6eu
#define	ENC_800_600_2_D1				0x6fu
#define	ENC_800_600_2_CIF				0x70u
#define	ENC_385S_DS_1080P_CIF			0x85u													//˫��������ʱ
#define ENC_385S_DS_FR_CIF				0x71u													//˫��������ʱ
/***********	Resolution Configuration of RMVA380SC/RMVA380SP H.264 Video Streaming  ***********/
#define	ENC_1920_1088					0x07u																
#define	ENC_960_544						0x06u																	
#define	ENC_1920_1088_2_1080P			0x21u													
#define	ENC_1920_1088_2_720P			0x22u													
#define	ENC_1920_1088_2_D1				0x5bu														
#define	ENC_1920_1088_2_CIF				0x47u														
#define	ENC_960_544_2_1080P				0x1fu														
#define	ENC_960_544_2_720P				0x20u														
#define	ENC_960_544_2_D1				0x5au														
#define	ENC_960_544_2_CIF				0x46u
#define ENC_380S_DS_1080P_CIF			0x86u													//˫��������ʱ													
/***********	Resolution Configuration of RMVA340SC/RMVA340SP H.264 Video Streaming  ***********/
#define	ENC_2352_1760					0x13u														
#define	ENC_1168_880					0x14u														
#define	ENC_2352_1760_2_1600_1200		0x3fu												
#define	ENC_2352_1760_2_1080P			0x39u																	
#define	ENC_2352_1760_2_720P			0x3au													
#define	ENC_2352_1760_2_D1				0x67u														
#define	ENC_2352_1760_2_CIF				0x53u														
#define	ENC_1168_880_2_1080P			0x3bu													
#define	ENC_1168_880_2_720P				0x3cu														
#define	ENC_1168_880_2_D1				0x68u														
#define	ENC_1168_880_2_CIF				0x54u				
#define	ENC_340S_DS_FR_CIF				0x87u													//˫��������ʱ
#define	ENC_340S_DS_1080P_CIF			0x88u													//˫��������ʱ										
/***********	Resolution Configuration of RMVA335SC/RMVA335SN/RMVA335SP H.264 Video Streaming  ***********/
#define	ENC_2448_2048					0x12u														
#define	ENC_1216_1024					0x08u														
#define	ENC_2448_2048_2_1600_1200		0x3eu												
#define	ENC_2448_2048_2_1080P			0x37u													
#define	ENC_2448_2048_2_720P			0x38u													
#define	ENC_2448_2048_2_D1				0x66u														
#define	ENC_2448_2048_2_CIF				0x52u														
#define	ENC_1216_1024_2_1080P			0x23u													
#define	ENC_1216_1024_2_720P			0x24u													
#define	ENC_1216_1024_2_D1				0x5cu														
#define	ENC_1216_1024_2_CIF				0x48u				
#define	ENC_335S_DS_FR_CIF				0x73u													//˫��������ʱ
#define	ENC_335S_DS_1080P_CIF			0x72u													//˫��������ʱ										
/***********	Resolution Configuration of RMVA360SC/RMVA360SP/RMVA361SC/RMVA361SP H.264 Video Streaming  ***********/
#define	ENC_3744_1408					0x0cu
#define	ENC_3744_1408_2_1080P			0x2bu	
#define	ENC_3744_1408_2_720P			0x2cu
#define	ENC_3744_1408_2_D1				0x60u
#define	ENC_3744_1408_2_CIF				0x4cu																						
#define	ENC_1888_1408					0x11u														
#define	ENC_944_704						0x0au															
#define	ENC_1888_1408_2_1600_1200		0x3du												
#define	ENC_1888_1408_2_1080P			0x35u												
#define	ENC_1888_1408_2_720P			0x36u												
#define	ENC_1888_1408_2_D1				0x65u													
#define	ENC_1888_1408_2_CIF				0x51u													
#define	ENC_944_704_2_1080P				0x27u													
#define	ENC_944_704_2_720P				0x28u													
#define	ENC_944_704_2_D1				0x5eu													
#define	ENC_944_704_2_CIF				0x4au	
#define	ENC_1888_1408_2_1600_1200		0x3du																
#define	ENC_3776_1408_2_1600_1216		0x69u
#define	ENC_3776_1408_2_1600_1200		0x6au
#define	ENC_1888_1408_2_1600_1216		0x6bu
#define	ENC_360S_DS_FR_CIF				0x89u													//˫��������ʱ
#define	ENC_360S_DS_1080P_CIF			0x8au													//˫������ʱ								
/***********	Resolution Configuration of RMVA320SC/RMVA320SN/RMVA320SP  H.264 Video Streaming  ***********/
#define	ENC_1280_960					0x01u																			
#define ENC_1280_960_2_1600_1200		0x74u
#define	ENC_1280_960_2_1080P			0x15u																	
#define	ENC_1280_960_2_720P				0x16u 
#define	ENC_1280_960_2_D1				0x55u																				
#define	ENC_1280_960_2_CIF				0x41u																			
#define	ENC_640_480						0x75u
#define	ENC_640_480_2_1280_720			0x76u
#define	ENC_640_480_2_720_576			0x77u
#define	ENC_640_480_2_352_288			0x78u
#define	ENC_320S_DS_FR_CIF				0x8Bu													//˫��������ʱ
#define	ENC_320S_DS_1080P_CIF			0x8Cu													//˫��������ʱ	
/***********	Resolution Configuration of RMVA370SN/RMVA370SC/RMVA370SP  H.264 Video Streaming  ***********/
#define	ENC_1920_1440					0x79u	
#define	ENC_1920_1440_2_1600_1200		0x7Au
#define	ENC_1920_1440_2_1920_1080		0x7Bu
#define	ENC_1920_1440_2_1280_720		0x7Cu
#define	ENC_1920_1440_2_D1				0x7Du
#define	ENC_1920_1440_2_CIF				0x7Eu
#define	ENC_960_720						0x7Fu
#define	ENC_960_720_2_1600_1200			0x80u
#define	ENC_960_720_2_1920_1080			0x81u
#define	ENC_960_720_2_1280_720			0x82u
#define	ENC_960_720_2_D1				0x83u
#define	ENC_960_720_2_CIF				0x84u
#define	ENC_370S_DS_FR_CIF				0x8Du													//˫��������ʱ
#define	ENC_370S_DS_1080P_CIF			0x8Eu													//˫��������ʱ	

/***********	Reserved Resolution Configuration of H.264 Video Streaming  ***********/
#define	ENC_1344_256				0x02u																					
#define	ENC_1344_1024				0x03u																					
#define	ENC_2432_2048				0x09u																					
#define	ENC_1856_1408				0x0bu																				
#define	ENC_800_608					0x0du																					
#define	ENC_1632_1216				0x0eu																					
#define	ENC_3264_2432				0x0fu																				
#define	ENC_3776_1408				0x10u																					

#define	ENC_1344_256_2_1080P		0x17u																		
#define	ENC_1344_256_2_720P			0x18u																			
#define	ENC_1344_1024_2_1080P		0x19u																		
#define	ENC_1344_1024_2_720P		0x1au																	
#define	ENC_2432_2048_2_1080P		0x25u																  	
#define	ENC_2432_2048_2_720P		0x26u																		
#define	ENC_1856_1408_2_1080P		0x29u																		
#define	ENC_1856_1408_2_720P		0x2au																								
																	
#define	ENC_800_608_2_1080P			0x2du																			
#define	ENC_800_608_2_720P			0x2eu																			
#define	ENC_1632_1216_2_1080P		0x2fu																		
#define	ENC_1632_1216_2_720P		0x30u																		
#define	ENC_3264_2432_2_1080P		0x31u																	
#define	ENC_3264_2432_2_720P		0x32u																	
#define	ENC_3776_1408_2_1080P		0x33u																	
#define	ENC_3776_1408_2_720P		0x34u																	
#define	ENC_2432_2048_2_1600_1200	0x40u																	

#define	ENC_1344_256_2_CIF			0x42u																		
#define	ENC_1344_1024_2_CIF			0x43u																								
#define	ENC_2432_2048_2_CIF			0x49u																			
#define	ENC_1856_1408_2_CIF			0x4bu																			
																		
#define	ENC_800_608_2_CIF			0x4du																			
#define	ENC_1632_1216_2_CIF			0x4eu																		
#define	ENC_3264_2432_2_CIF			0x4fu																		
#define	ENC_3776_1408_2_CIF			0x50u																		

#define	ENC_1344_256_2_D1			0x56u																			
#define	ENC_1344_1024_2_D1			0x57u																			
#define	ENC_2432_2048_2_D1			0x5du																			
#define	ENC_1856_1408_2_D1			0x5fu																			
																			
#define	ENC_800_608_2_D1			0x61u																			
#define	ENC_1632_1216_2_D1			0x62u																		
#define	ENC_3264_2432_2_D1			0x63u																			
#define	ENC_3776_1408_2_D1			0x64u																		 

/***ѹ����OSD��ʾ�궨��***/
#define ENC_OSD_SHOW_ENABLE					0x00u
#define ENC_OSD_SHOW_DISABLE				0x01u

/***ѹ����ץ��֡�Ƿ�ѹ��264�����ϴ��궨��***/
#define ENC_TRGF_UPLOAD						0x01u
#define ENC_TRGF_THROWAWAY					0x00u

//ѹ����֡�ʵȼ�
#define	ENC_FR_LEVEL_0						0u
#define	ENC_FR_LEVEL_1						1u
#define	ENC_FR_LEVEL_2						2u
#define	ENC_FR_LEVEL_3						3u
#define	ENC_FR_LEVEL_4						4u
#define	ENC_FR_LEVEL_5						5u
#define	ENC_FR_LEVEL_6						6u
#define	ENC_FR_LEVEL_7						7u

/***DM648��OSD�����궨��***/
#define	OSD_COLOR_RED						0u
#define	OSD_COLOR_YELLOW					1u
#define OSD_COLOR_WHITE						2u
#define OSD_COLOR_ORANGE					3u
#define OSD_COLOR_GREEN						4u
#define OSD_COLOR_BLUE						5u
#define OSD_COLOR_DEEP_SKY_BLUE				6u
#define OSD_COLOR_PURPLE					7u
#define OSD_COLOR_BLACK						8u
#define	OSD_ZOOM_X1							0u	//16*16
#define	OSD_ZOOM_X2							1u	//32*32
#define	OSD_ZOOM_X3							2u	//64*64
#define OSD_ERROR_COLOR_SELECT				1u
#define OSD_ERROR_LOCATION_SET				2u
#define OSD_ERROR_IMG_FORMAT				3u
#define OSD_ERROR_WORD_SIZE_SELECT			4u

/***��Դͬ���Ľ���Ƶ�ʺ궨��***/
#define	POWERSYN_ACFREQ_50HZ				0x00
#define	POWERSYN_ACFREQ_60HZ				0x01

/***ѹ����������÷���ֵ����***/
#define	ENC_CONFIG_OK						0u
#define	ENC_CONFIG_RESOL_ERR				-1
#define	ENC_OSD_LEN_ERR						-2
#define	ENC_OSD_FONT1X						1u
#define	ENC_OSD_FONT2X						2u
#define	ENC_OSD_FONT3X						3u
#define	ENC_OSD_FONT4X						4u
#define	ENC_OSD_FONT5X						5u

/***DMAϵͳ�������ȼ��궨��***/
#define DMA_SYS_PRI_LEVEL0					0x00u
#define DMA_SYS_PRI_LEVEL1					0x01u
#define DMA_SYS_PRI_LEVEL2					0x02u
#define DMA_SYS_PRI_LEVEL3					0x03u
#define DMA_SYS_PRI_LEVEL4					0x04u

/***DMA���ȼ��궨��***/
#define EDMA_OPT_PRI_URGENT					0x00u
#define EDMA_OPT_PRI_HIGH					0x01u
#define EDMA_OPT_PRI_MEDIUM					0x02u
#define EDMA_OPT_PRI_LOW					0x03u

/***QDMA���ȼ��궨��***/
#define QDMA_OPT_PRI_URGENT					0x00u
#define QDMA_OPT_PRI_HIGH					0x01u
#define QDMA_OPT_PRI_MEDIUM					0x02u
#define QDMA_OPT_PRI_LOW					0x03u

/***�����������ƿ�������***/
#define	OC_FLASH_0							0x00u
#define	OC_FLASH_1							0x01u

//������ȫ�ֱ�������ģʽ��Ӧ�����ż���
#define TRGFCON_SCALE_LEVEL_0				0u   //1920*1088
#define TRGFCON_SCALE_LEVEL_1				1u   //960*544

/***���ڲ����ʶ���***/
#define	Roseek_RS232_BAUDRATE_4800			1u
#define	Roseek_RS232_BAUDRATE_9600			2u
#define	Roseek_RS232_BAUDRATE_19200			3u
#define	Roseek_RS232_BAUDRATE_38400			4u
#define	Roseek_RS232_BAUDRATE_57600			5u
#define	Roseek_RS232_BAUDRATE_115200		6u
#define	Roseek_RS485_BAUDRATE_4800			1u
#define	Roseek_RS485_BAUDRATE_9600			2u
#define	Roseek_RS485_BAUDRATE_19200			3u
#define	Roseek_RS485_BAUDRATE_38400			4u
#define	Roseek_RS485_BAUDRATE_57600			5u
#define	Roseek_RS485_BAUDRATE_115200		6u
#define	Roseek_RS232_PORT1					0u
#define	Roseek_RS232_PORT2					1u

/***���ں�����������ֵ�궨��***/
#define	UART_SEND_OK						0u
#define	UART_FIFO_FULL						1u
#define	UART_SENDDATA_OVERSIZE				2u
#define	UART_PORTIDX_ERROR					3u

/***ѹ����ͼ��������ͺ궨��***/
#define	ENC_JPG								0xffd8ffd9u
#define	ENC_H264							0xffd6ffd7u

/***��ȡѹ�������÷���ֵ�궨��***/
#define	ENC_INF_OK							0
#define	ENC_INF_DTYPE_ERR					-1
#define	ENC_INF_DLEN_ERR					-2
#define	ENC_DSTADD_ERR						-3

/***֧�ֵ�SD�����ͺ궨��***/
#define	SD_2G								0x00u// ����Ϊ2GSD��
#define	SDHC_4G								0x01u// ����Ϊ4GSDHC��
#define	SDHC_8G								0x02u// ����Ϊ8GSDHC��
#define	SDHC_16G							0x03u// ����Ϊ16GSDHC��
#define	SDHC_32G							0x04u// ����Ϊ32GSDHC��

/***DAT����������궨��***/
#define	DAT_CHANNEL_ID_ERR					1u
#define	DAT_CHANNEL_ADD_ERR					2u
#define	DAT_CHANNEL_BUSY					3u
#define	DAT_CHANNEL_DLEN_ERR				4u
#define	DAT_TYPE_ERR						5u
#define	DAT_OP_OK							0u

/***DAT�����ȴ����ͺ궨��***/
#define	DAT_WAITALL							0u
#define	DAT_WAITNONE						1u

/***DAT����2D�������ͺ궨��***/
#define	DAT_1D2D							0u
#define	DAT_2D1D							1u
#define	DAT_2D2D							2u

#ifndef _ROSEEK_MODULE_00
#define _ROSEEK_MODULE_00
#endif
extern void Roseek_Module_Reg_00(Uint8* pVerInf);//Main lib
#ifndef _ROSEEK_MODULE_01
#define _ROSEEK_MODULE_01
#endif
extern void Roseek_Module_Reg_01(Uint8* pVerInf);//FGPA
#ifndef _ROSEEK_MODULE_02
#define _ROSEEK_MODULE_02
#endif
extern void Roseek_Module_Reg_02(Uint8* pVerInf);//ENC

/**************************************************************************/
typedef struct ModuleVersionInf
{
	Uint32 ui32Version; //ģ��汾��Ϣ������Ϊʮ��������105 ����1.05 �汾
	Char strVerInf[160]; //ģ����Ϣ����������Ϊ�ַ���
}RoseekModuleVersionInf;
/**************************************************************************/
 

//���Ͷ���
typedef int   DAT_HANDLE;

/**************************************************************************/
// TrgMode->trigger mode; FconMode->fine continuous mode
enum Roseek_SensorRunMode{ TrgMode, FconMode };
enum Roseek_ImgAcquisition_Format{ RGB_ARRAY, YUV_ARRAY, BAYER_ARRAY };
enum Roseek_WhiteBalance_environment { DAYLIGHT, FLUORESCENCE, WITHOUTWB };
/**************************************************************************/

/**************************************************************************/
//����IO���Խṹ��
typedef struct SPIOAtbStruct{
	Uint8	ui8IsOutput;				//�Ƿ�����, 1->output, 0->input
	////////��������////////
	Uint8	ui8DigFilteringLevel;		//������������0-20��21������Ӧ0-20���룬one step = 1 ms
	Uint8	ui8IsTrgSource;				//�Ƿ�Ϊץ�ĵĴ���Դ��1->����Դ��0->�Ǵ���Դ
	Uint8	ui8TrgEdgePolarity;			//������������, 0->�½��ش���, 1->�����ش���, 2->˫�ؾ�����
	Uint8	ui8EnableInt;				//�����ж�ʹ�ܣ�1->����0->��ֹ
	Uint8	ui8PwrSynPulsIn;			//��Դͬ���������룬�½��ش���
	Uint8	ui8InOpRsved_2;				//��������
	Uint8	ui8InOpRsved_3;				//��������
	///////�������/////////
	Uint8	ui8OutOption;
										//0->һ������������ƽ���û�����
										//1->����ģʽ�����ͬ�����������ģʽ������ɵ�����󶨳���(IO���)�ҽ��ɶ�Ӧһ����������ӦIO����ָ��Ϊ����Դ��֧����Ӳ����(�ߵ�ƽ����)
										//2->����ģʽ�����ͬ�����������ģʽ������ɵ�����󶨳���(IO���)�ҽ��ɶ�Ӧһ����������ӦIO����ָ��Ϊ����Դ��֧����Ӳ����(�͵�ƽ����)
										//3->����ģʽ�����ͬ������������ģʽ������ɵ����������Ϊ�����ͬ���������������������е�һ������һ��������˸����֧����������ƣ�֧����Ӳ����(�ߵ�ƽ����)
										//4->����ģʽ�����ͬ������������ģʽ������ɵ����������Ϊ�����ͬ���������������������е�һ������һ��������˸����֧����������ƣ�֧����Ӳ����(�͵�ƽ����)
										//5->ͨ�������ͬ�����������ɵ�����Ӧ����ģʽ(�ߵ�ƽ����)
										//6->ͨ�������ͬ�����������ɵ�����Ӧ����ģʽ(�͵�ƽ����)
										//7->����ģʽ�����ͬ�����������Ӧ����������ɵ�����Ӧ��������Ӳ����(�ߵ�ƽ����)
										//8->����ģʽ�����ͬ�����������Ӧ����������ɵ�����Ӧ��������Ӳ����(�ߵ�ƽ����)
										//9->LED��Ƶ��ͬ�����������Ӧ����������ɵ�����Ӧ����ģʽ���ڴ���ģʽ�ع�ʱ��ͬ�����(�ߵ�ƽ����)
										//10->LED��Ƶ��ͬ�����������Ӧ����������ɵ�����Ӧ����ģʽ���ڴ���ģʽ�ع�ʱ��ͬ�����(�͵�ƽ����)
										//11->LEDȫƵ��ͬ�����������Ӧ����������ɵ�����Ӧ����ģʽ���ڴ���ģʽ�ع�ʱ��ͬ�����(�ߵ�ƽ����)
										//12->LEDȫƵ��ͬ�����������Ӧ����������ɵ�����Ӧ����ģʽ���ڴ���ģʽ�ع�ʱ��ͬ�����(�͵�ƽ����)
										//13->����ģʽ�����ͬ�����������Ӧ����������ɵ�(�ߵ�ƽ����)
										//14->����ģʽ�����ͬ�����������Ӧ����������ɵ�(�͸ߵ�ƽ����)
										//15->����ģʽ�����ͬ�����������ɵ�(�ߵ�ƽ����)
										//16->����ģʽ�����ͬ�����������ɵ�(�͵�ƽ����)
										//17->�ع�ͬ��ź���?�ߵ�ƽ��Ч)
										//18-31����
	Uint8	ui8SoftTrgFlashEnable;		//��Ϊ����ģʽ�����ͬ�����(15-16),ʹ���ź�,1Ϊʹ��,0Ϊ��ֹ
	//�����ͬ�����ѡ��
	Uint8	ui8FlashOutOption;			//��Ϊ����ģʽ�ĵ�����ģʽ(1-4)����Ӧ�����Ƶ�IO���(������)�����ڶ�Ӧ��IOΪ����Դ���������Ч
	Uint8	ui8FlashAlterState;			//��Ϊ����ģʽ����Ӧ��״̬(����0/1����״̬)
	Uint8	ui8SLStgFlashEnable;		//����ģʽ�ĵ�����ģʽ����������Ƿ�ʹ��
	Uint8	ui8OutOpRsved_2;			//��������
	Uint8	ui8OutOpRsved_3;			//��������
	Uint8	ui8OutOpRsved_4;			//��������
}RoseekSPIOAtbStruct;
/**************************************************************************/

/**************************************************************************/
//�����������Խṹ�壨new��
typedef struct ISOOutAtbStruct{
	Uint8    ui8OutOption;
	Uint8    ui8SoftTrgFlashEnable;
	Uint8    ui8FlashOutOption;
	Uint8    ui8FlashAlterState;
	Uint8    ui8SLStgFlashEnable;
	Uint8    ui8OutOpRsved_1;
	Uint8    ui8OutOpRsved_2;
	Uint8    ui8OutOpRsved_3;
}RoseekISOOutAtbStruct;
/**************************************************************************/

/**************************************************************************/
//ͼ�������Զ����ڲ����ṹ��
typedef struct ImgYMeanAutoAdjParams{
	Bool	bIsAllowBrightnessAdj;		//�Ƿ�������ӿ����Զ����ڣ�TRUE:����; FALSE:������
	Uint32  ui32PIShutterWidthHigh_us;	//���ӿ����Զ����ڵĿ���ʱ�����ޣ���λΪ΢��uS
	Uint32  ui32CurrentShutterWidth_us;	//��ǰ���ӿ��ſ���ʱ�䣬��λΪ΢��uS
	float	fCurrentAfePga_db;			//��ǰ����������ź��ڲ����棬��λΪdb
	float	fPIPgaHigh_db;				//����������ź��ڲ������Զ��������ޣ���λΪdb
	Uint32  ui32PICurYmeanThreshold;	//ͼ�������Զ�������׼�ο�ֵ
	Uint32  ui32PIYmeanHighThreshold;	//ͼ�����Ȳ�����ڷ�Χ��?
	Uint32	ui32PIYmeanLowThreshold;	//ͼ�����Ȳ�����ڷ�Χ����
}RoseekImgYMeanAutoAdjParams;
/**************************************************************************/

/**************************************************************************/
//���ͨ�ÿ������ṹ��
typedef struct EagleEye2GeneralCtrlStruct{
	Bool	bRunWhitebalance;							//�Ƿ�Ե�ǰ�ɼ���ͼ����а�ƽ�����
	Bool	bAllowAutoTrgAfterModeSwitch;				//ģʽ�л�������ģʽ���Ƿ������Զ�����ץȡһ��ͼ��
	Bool	bIsEnableImgLUT;							//�Ƿ�ʹ��ͼ��LUTӳ��任
	Bool	bIsEnableWatchDog;							//�Ƿ�ʹ�ܿ��Ź�
	Uint32	ui32WDTValue;								//���Ź���ʱ��ֵ(1~256��)
	float	fCurSignalGain;								//��ǰ����������ź�����
	enum Roseek_SensorRunMode CurRunModeFlag;				//�����ǰ����������ģʽ
	enum Roseek_ImgAcquisition_Format CurImgFormat;		//�����ǰͼ��ɼ��ĸ�ʽ
}RoseekGeneralCtrlStruct;
/**************************************************************************/

/**************************************************************************/
//SD����Ϣ�ṹ��
typedef struct RS_SD_INFSTRUCT
{
	Uint8	ui8Card_type;		/* �������� */
	Uint32	ui32Block_num;		/* ���п������ */
	Uint32	ui32Block_len;		/* ���Ŀ鳤��(��λ:�ֽ�) */
}rs_sd_infstruct;
/**************************************************************************/

/**************************************************************************/
//���õ��Ŀ��еı���
extern RoseekGeneralCtrlStruct	g_CameraGeneralCtrlParams;
extern RoseekImgYMeanAutoAdjParams	g_TrgBrightnessAdj_Params;
extern RoseekImgYMeanAutoAdjParams	g_FconBrightnessAdj_Params;
extern rs_sd_infstruct g_SDInf;	/* SD����Ϣ�ṹ����� */
extern volatile float fWBCof_R;//1.3392
extern volatile float fWBCof_G;//1.0
extern volatile float fWBCof_B;//1.4032
/**************************************************************************/

/******************************��������************************************/
extern	void	Roseek_Open_Imagechannel();
extern	void	Roseek_Close_Imagechannel();
extern	void	Roseek_Enable_ExpSynInt();
extern	void	Roseek_Disable_ExpSynInt();
extern	void	Roseek_Grab_Single_Frame();
extern	Bool	Roseek_Set_TrgShutter_Time( Uint32 );
extern	Bool	Roseek_Set_FConShutter_Time( Uint32 );
extern	void	Roseek_Mode_Switch_Ctrl( enum Roseek_SensorRunMode, Uint32 );
extern	void	Roseek_Enable_RunModeSwitch_AutoTrg( Bool );
extern	void	Roseek_Enable_TrgEnableInt();
extern	void	Roseek_Disable_TrgEnableInt();
extern	void	Roseek_Set_Afe_Pga( enum Roseek_SensorRunMode, float );
extern	void	Roseek_Set_WBEvr( enum Roseek_WhiteBalance_environment );
extern	void	Roseek_Set_WBCof( float, float, float );
extern	void	Roseek_Set_Image_LUT( Uint8*, Uint32 );
extern	void	Roseek_Disable_Image_LUT( );
extern	void	Roseek_Enable_Image_LUT( );
extern	void	Roseek_Resume_Image_LUT();
extern	void	Roseek_Set_GammaValue( float );
extern	Bool	Roseek_Is_TrgEnableInt();
extern	Bool	Roseek_Is_TrgEnable();
extern	void	Roseek_Set_ImgAcquisition_Format( enum Roseek_ImgAcquisition_Format );
extern	void	Roseek_SetTrgShutterTimeCtrlMode( Bool );
extern	Bool	Roseek_GetTrgShutterTimeCtrlMode();
extern	Bool	Roseek_Set_MIBlockParam( Uint32, Uint32, Uint32, Uint32, Uint32 );
extern	Bool	Roseek_Get_MIBlockYRGBMean( Uint32, Uint32* );
extern	int		Roseek_Get_TrgSurFlag();
extern	void	Roseek_Set_FlashSynWidth( Uint32 );
extern	void	Roseek_DetectIllum_Adj( enum Roseek_SensorRunMode, Uint32 );
extern	Bool	Roseek_Set_ImgRotateMode( Uint32 );
extern	Uint32	Roseek_Get_ImgRotateMode();
extern	enum	Roseek_SensorRunMode Roseek_Get_CurRunMode();
extern	enum	Roseek_ImgAcquisition_Format Roseek_Get_CurImgFormat();
extern	void	Roseek_ImgAcqChannel_Config( enum Roseek_SensorRunMode,
									 		 enum Roseek_ImgAcquisition_Format,
									 		 Uint8*,
									 		 Uint32 );
extern	void	Roseek_Clear_ImgAcqChannel();
extern	void	Roseek_Set_RGBArray_Seq( Uint32 );
extern	void	Roseek_Set_TrgFconMode_ScaleLevel( enum Roseek_SensorRunMode, Uint32 );
extern	Uint32	Roseek_Get_TrgFconMode_ScaleLevel();
extern	void	Roseek_Set_DMASysPriority( Uint32 );
extern	DAT_HANDLE Roseek_DAT_Open( Uint32 );
extern	int		Roseek_DAT_Close( DAT_HANDLE );
extern	int		Roseek_DAT_Copy( DAT_HANDLE, void*, void*, Uint16 );
extern	int		Roseek_DAT_Copy2d( DAT_HANDLE, Uint32, void*, void*, Uint16, Uint16, Uint16 );
extern	void	Roseek_DAT_Wait( DAT_HANDLE, Uint32 );
extern	void	Roseek_Enable_PowerSyn( Uint32 );
extern	void	Roseek_Disable_PowerSyn();
extern	void	Roseek_Set_PowerSyn_Phase( Uint32 );
extern	void  Roseek_Set_ColorSaturation_Param( float ); 
extern	void	Roseek_WhiteBalance();
extern	void	Roseek_Set_ImgEnhancement_Level(Uint32);
extern	int		Roseek_PIOInteInfConfig( Uint8* );
extern	int		Roseek_SPIOInfConfig( Uint8, Uint8* );
extern	Bool	Roseek_Set_Pio_Outvalue( Uint8, Uint8 );
extern	void	Roseek_Set_FlashPreSyn( Uint32 );
extern	void	Roseek_Enable_TrgFlashlamp_Syn_Out();
extern	void	Roseek_Enable_FconFlashlamp_Syn_Out();
extern	void	Roseek_Disable_TrgFlashlamp_Syn_Out();
extern	void	Roseek_Disable_FconFlashlamp_Syn_Out();
extern	int		Roseek_Config_OC_FlashCtrl( Uint32, Uint32 );
extern	Bool	Roseek_Enable_PIOInt( Uint8 );
extern	Bool	Roseek_Disable_PIOInt( Uint8 );
extern	void	Roseek_Get_PIOIntInf( Uint32* );
extern	Uint32	Roseek_GetPIOState();
extern	Uint32	Roseek_GetPIOIntTime();
extern	Uint8	Roseek_OSD(	Uint8	*pui8Info,					 
							Uint8	*pOutputTime,
							Uint32	ui32IsAllowOutputTime, 
							Uint32	ui32Xraw, 
							Uint32	ui32Yraw, 
							Uint32	ui32ColorSelect,
							Uint32	ui32ImgRowNum ,
							Uint32	ui32ImgColNum,
							Uint32	ui32ImgFormat,
							Uint8	*pui8YBuf,
							Uint8	*pui8CrBuf,	
							Uint8	*pui8CbBuf,
							Uint32	ui32WordZoom,
							Uint32	ui32Reserved1,
							Uint32	ui32Reserved2,
							Uint32	ui32Reserved3);
extern int		Roseek_JPGENC_Encode( Uint8* pui8OutputBufs[2], Uint8* pui8InputBufs[3], Uint32 ui32YUVFormat, Uint32 ui32Quality, Uint32 ui32ImgWidth, Uint32 ui32ImgHeight);
extern int		Roseek_JPGENC_Init( Uint32 ui32ImgWidthMax, Uint32 ui32ImgHeightMax );
extern	Bool	Roseek_Flash_Burn( Uint8*, Uint32, Uint32 );
extern	Bool	Roseek_Flash_Read( Uint8*, Uint32, Uint32 );
extern	void	Roseek_RS232_Open( Uint32 );
extern	void	Roseek_RS232_Close( Uint32 );
extern	void	Roseek_RS232_SetBaudrate( Uint32, Uint32 );
extern	int		Roseek_RS232_WriteData( Uint32, char*, Uint32 );
extern	void	Roseek_RS232_ReadBuf( Uint32, char*, Uint32 );
extern	Bool	Roseek_SetRS232RcvBufDepth( Uint32, Uint32 );
extern	void	Roseek_RS485_Open();
extern	void	Roseek_RS485_Close();
extern	void	Roseek_RS485_SetBaudrate( Uint32 );
extern	int		Roseek_RS485_WriteData( char*, Uint32 );
extern	void	Roseek_RS485_ReadBuf( char*, Uint32 );
extern	Bool	Roseek_SetRS485RcvBufDepth( Uint32 );
extern	Bool	Roseek_IsRS232_1_RcvReady();
extern	Bool	Roseek_IsRS232_2_RcvReady();
extern	Bool	Roseek_IsRS485RcvReady();
extern	Uint32	Roseek_GetRS232_1_Recvcnt();
extern	Uint32	Roseek_GetRS232_2_Recvcnt();
extern	Uint32	Roseek_GetRS485_Recvcnt();
extern	int		Roseek_SD_Initialize();
extern	int		Roseek_SD_WriteMultiBlock( Uint32, Uint32, Uint8* );
extern	int		Roseek_SD_EraseBlock( Uint32, Uint32 );
extern	int		Roseek_SD_ReadMultiBlock( Uint32, Uint32, Uint8*, Uint32 );
extern	void	Roseek_Open_ENCDataChannel();
extern	void	Roseek_Close_ENCDataChannel();
extern	void	Roseek_Set_EncFRLevel( Uint32 );
extern	int		Roseek_Set_EncResolution( Uint32 );
extern	int		Roseek_Set_EncOSDInf( char* );
extern	void	Roseek_Set_EncCodeRate( Uint32 );
extern	void	Roseek_Set_EncOSD_ShowOp( Uint32, Uint32, Uint32, Uint32, Uint32, Uint32, Uint32, Uint32, Uint32, Uint32 );
extern	Bool	Roseek_IsENCCodeStream_Rdy();
extern 	Bool	Roseek_Encd_TransOver();
extern	void	Roseek_Set_EncOrgSys_Restore();
extern	void	Roseek_Set_EncTFTransOp( Uint32 );
extern	int		Roseek_ENCDataAcq_Config( Uint32*, Uint32*, Uint8* );
extern	void	Roseek_Set_EncIFInterval( Uint32 );//�ؼ�֡��� Ĭ����֡��  
extern	void	Roseek_Set_EncOSD_ShowOp( Uint32, Uint32, Uint32, Uint32, Uint32, Uint32, Uint32, Uint32, Uint32, Uint32 );
extern	void	Roseek_Save_IP_Address( Uint32 );
extern	Uint32	Roseek_Read_IP_Address();
extern	void	Roseek_Save_MAC_Address( Uint8* );
extern	void	Roseek_Read_MAC_Address( Uint8* );
extern	Uint32	Roseek_GetTickCount();
extern	void	Roseek_Reset();
extern	float	Roseek_Read_CurTemperature();
extern	Bool	Roseek_RTC_SetTime( Uint8* );
extern	void	Roseek_RTC_ReadTime( Uint8* );
extern	void	Roseek_Waitusec( Uint32 );
extern	void	Roseek_Watchdog_Open();
extern	void	Roseek_Watchdog_Close();
extern	void	Roseek_Set_WatchdogTimer( Uint32 );
extern	void	Roseek_KickWatchdog();
extern	void	Roseek_Enable_AutoKickWatchdog();
extern	void	Roseek_Disable_AutoKickWatchdog();
extern	void	Roseek_Initialize_SysBufs();
extern	void	Roseek_Get_CPU_ID( Uint8* );
extern  void	Roseek_Drive_LF_Switch(Uint32);
extern	void	Roseek_Get_ENCCodeRate( Uint32* pCodeRate );
extern	int 	Roseek_Set_SubCodeRate( Uint32	ui32SubCodeRate);
extern	void 	Roseek_Get_SubCodeRate( Uint32* pui32SubCodeRate);
extern	int 	Roseek_Set_EncCIFOSDInf( char* pszBuf );
/**************************************************************************/

#endif


