/*   Do *not* directly modify this file.  It was    */
/*   generated by the Configuration Tool; any  */
/*   changes risk being overwritten.                */

/* INPUT EE3.cdb */

/*  Include Header Files  */
#include <std.h>
#include <prd.h>
#include <hst.h>
#include <swi.h>
#include <tsk.h>
#include <log.h>
#include <sem.h>
#include <sts.h>

#ifdef __cplusplus
extern "C" {
#endif

extern far PRD_Obj prdNdk;
extern far HST_Obj RTA_fromHost;
extern far HST_Obj RTA_toHost;
extern far SWI_Obj PRD_swi;
extern far SWI_Obj KNL_swi;
extern far TSK_Obj TSK_idle;
extern far TSK_Obj tskNetWorkMain;
extern far TSK_Obj tskJPGEncoder;
extern far TSK_Obj tskAcmdBDmain;
extern far LOG_Obj LOG_system;
extern far LOG_Obj trace;
extern far SEM_Obj sem_ConDataReady;
extern far SEM_Obj sem_TrgDataReady;
extern far SEM_Obj sem_HfconDataReady;
extern far SEM_Obj sem_HfrBinconDataRdy;
extern far SEM_Obj sem_CalSpeed;
extern far SEM_Obj sem_LiveStreamDataReady;
extern far SEM_Obj sem_JpegReady;
extern far SEM_Obj sem_LiveStreamDataReady_Another;
extern far SEM_Obj sem_AcmdBDReady;
extern far SEM_Obj sem_FcontoDecReady;
extern far SEM_Obj sem_DpDataReady1;
extern far SEM_Obj sem_DpDataReady2;
extern far SEM_Obj sem_SendVideoReady;
extern far SEM_Obj sem_DecClientReady;
extern far SEM_Obj sem_CmdDataReady;
extern far SEM_Obj sem_UdplistenReady;
extern far SEM_Obj sem_DataRunTimeN;
extern far SEM_Obj sem_SendRoadInfReady;
extern far STS_Obj IDL_busyObj;


#ifdef __cplusplus
}
#endif /* extern "C" */
