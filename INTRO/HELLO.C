#define INCL_WINDIALOGS
#define INCL_WINENTRYFIELDS
#define INCL_WINLISTBOXES
#define INCL_WINSYS
#define INCL_WINWINDOWMGR
#include <os2.h>
#include <stdio.h>
#include "hello.h"
#include "hellorc.h"

MRESULT EXPENTRY nameDlgProc(HWND hwndWnd,
                             ULONG ulMsg,
                             MPARAM mpParm1,
                             MPARAM mpParm2)
{
   PNAMEDLGINFO pndiInfo;

   pndiInfo=(PNAMEDLGINFO)WinQueryWindowPtr(hwndWnd,0);              // @1

   switch (ulMsg) {
   case WM_INITDLG:                                                  // @2
      {
         HAB habAnchor;
         HWND hwndLb;
         CHAR achText[64];

         pndiInfo=(PNAMEDLGINFO)PVOIDFROMMP(mpParm2);                // @3
         WinSetWindowPtr(hwndWnd,0,(PVOID)pndiInfo);                 // @4

         WinSendDlgItemMsg(hwndWnd,                                  // @5
                           DNAME_EF_NAME,
                           EM_SETTEXTLIMIT,
                           MPFROMSHORT(sizeof(pndiInfo->achName)),
                           0);

         habAnchor=WinQueryAnchorBlock(hwndWnd);
         hwndLb=WinWindowFromID(hwndWnd,DNAME_LB_NAMELIST);

         WinLoadString(habAnchor,                                    // @6
                       NULLHANDLE,
                       STR_TOM,
                       sizeof(achText),
                       achText);

         WinInsertLboxItem(hwndLb,LIT_END,achText);                  // @7

         WinLoadString(habAnchor,
                       NULLHANDLE,
                       STR_DICK,
                       sizeof(achText),
                       achText);

         WinInsertLboxItem(hwndLb,LIT_END,achText);

         WinLoadString(habAnchor,
                       NULLHANDLE,
                       STR_HARRY,
                       sizeof(achText),
                       achText);

         WinInsertLboxItem(hwndLb,LIT_END,achText);
      }
      break;
   case WM_CONTROL:                                                  // @8
      switch (SHORT1FROMMP(mpParm1)) {
      case DNAME_LB_NAMELIST:
         switch (SHORT2FROMMP(mpParm1)) {
         case LN_SELECT:                                             // @9
            {
               HWND hwndLb;
               SHORT sIndex;

               hwndLb=WinWindowFromID(hwndWnd,DNAME_LB_NAMELIST);

               sIndex=WinQueryLboxSelectedItem(hwndLb);
               WinQueryLboxItemText(hwndLb,
                                    sIndex,
                                    pndiInfo->achName,
                                    sizeof(pndiInfo->achName));

               WinSetDlgItemText(hwndWnd,
                                 DNAME_EF_NAME,
                                 pndiInfo->achName);
            }
            break;
         case LN_ENTER:                                              // @10
            WinPostMsg(hwndWnd,WM_COMMAND,MPFROMSHORT(DID_OK),0);
            break;
         default:
            return WinDefDlgProc(hwndWnd,ulMsg,mpParm1,mpParm2);     // @11
         } /* endswitch */
         break;
      default:
         return WinDefDlgProc(hwndWnd,ulMsg,mpParm1,mpParm2);
      } /* endswitch */
      break;
   case WM_COMMAND:
      switch (SHORT1FROMMP(mpParm1)) {
      case DID_OK:                                                   // @12
         WinDismissDlg(hwndWnd,TRUE);
         break;
      case DID_CANCEL:                                               // @13
         WinDismissDlg(hwndWnd,FALSE);
         break;
      default:
         return WinDefDlgProc(hwndWnd,ulMsg,mpParm1,mpParm2);
      } /* endswitch */
      break;
   default:
      return WinDefDlgProc(hwndWnd,ulMsg,mpParm1,mpParm2);
   } /* endswitch */

   return MRFROMSHORT(FALSE);
}

MRESULT EXPENTRY windowProc(HWND hwndWnd,
                            ULONG ulMsg,
                            MPARAM mpParm1,
                            MPARAM mpParm2)
{
   PNAMEDLGINFO pndiInfo;

   pndiInfo=(PNAMEDLGINFO)WinQueryWindowPtr(hwndWnd,0);

   switch (ulMsg) {
   case WM_PAINT:
      {
         HPS hpsPaint;
         RECTL rclPaint;
         HAB habAnchor;
         CHAR achFmt[64];
         CHAR achText[256];

         hpsPaint=WinBeginPaint(hwndWnd,NULLHANDLE,&rclPaint);

         WinFillRect(hpsPaint,&rclPaint,SYSCLR_WINDOW);

         habAnchor=WinQueryAnchorBlock(hwndWnd);

         if (pndiInfo!=NULL) {                                       // @14
            WinLoadString(habAnchor,
                          NULLHANDLE,
                          STR_HELLOWITHNAME,
                          sizeof(achFmt),
                          achFmt);
            sprintf(achText,achFmt,pndiInfo->achName);
         } else {
            WinLoadString(habAnchor,
                          NULLHANDLE,
                          STR_HELLONONAME,
                          sizeof(achText),
                          achText);
         } /* endif */

         WinQueryWindowRect(hwndWnd,&rclPaint);

         WinDrawText(hpsPaint,
                     -1,
                     achText,
                     &rclPaint,
                     CLR_BLACK,
                     0,
                     DT_CENTER|DT_VCENTER);

         WinEndPaint(hpsPaint);
      }
      break;
   case MYM_SETNAME:                                                 // @15
      WinSetWindowPtr(hwndWnd,0,PVOIDFROMMP(mpParm1));
      break;
   default:
      return WinDefWindowProc(hwndWnd,ulMsg,mpParm1,mpParm2);
   } /* endswitch */

   return MRFROMSHORT(FALSE);
}

INT main(VOID)
{
   HAB habAnchor;
   HMQ hmqQueue;
   NAMEDLGINFO ndiInfo;
   ULONG ulCreate;
   HWND hwndFrame;
   HWND hwndClient;
   BOOL bLoop;
   QMSG qmMsg;

   habAnchor=WinInitialize(0);
   hmqQueue=WinCreateMsgQueue(habAnchor,0);

   ndiInfo.usSzStruct=sizeof(ndiInfo);

   if (!WinDlgBox(HWND_DESKTOP,                                      // @16
                  HWND_DESKTOP,
                  nameDlgProc,
                  NULLHANDLE,
                  DLG_NAME,
                  (PVOID)&ndiInfo)) {
      goto EXIT_PROC;                                                // @17
   } /* endif */

   WinRegisterClass(habAnchor,
                    CLS_CLIENT,
                    windowProc,
                    CS_SIZEREDRAW,
                    sizeof(PVOID));

   ulCreate=FCF_SYSMENU | FCF_TITLEBAR | FCF_MINMAX | FCF_SIZEBORDER |
               FCF_SHELLPOSITION | FCF_TASKLIST | FCF_ICON;

   hwndFrame=WinCreateStdWindow(HWND_DESKTOP,
                                WS_VISIBLE,
                                &ulCreate,
                                CLS_CLIENT,
                                "Hello World Sample",
                                0,
                                NULLHANDLE,
                                RES_CLIENT,
                                &hwndClient);
   if (hwndFrame!=NULLHANDLE) {
      WinSetFocus(HWND_DESKTOP,hwndFrame);                           // @18

      WinSendMsg(hwndClient,MYM_SETNAME,MPFROMP(&ndiInfo),0);        // @19

      bLoop=WinGetMsg(habAnchor,&qmMsg,NULLHANDLE,0,0);
      while (bLoop) {
         WinDispatchMsg(habAnchor,&qmMsg);
         bLoop=WinGetMsg(habAnchor,&qmMsg,NULLHANDLE,0,0);
      } /* endwhile */

      WinDestroyWindow(hwndFrame);
   } /* endif */

   EXIT_PROC:
   WinDestroyMsgQueue(hmqQueue);
   WinTerminate(habAnchor);
   return 0;
}
