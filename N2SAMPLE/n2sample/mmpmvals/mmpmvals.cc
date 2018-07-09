///////////////////////////////////////////
// mmpmvals - MMPM/2 system values sample
//
// using emx 0.8h, mm4emx 1.0
//
//
// Marc E.E. van Woerkom, 2/94
//


#include <os2.h>

#define  INCL_OS2MM
#define  INCL_MCIOS2
#include <os2me.h>

#include <iostream.h>


//
// main
//

int main()
{
    cout << "mmpmvals -- MMPM/2 System Values\n\n";

    BOOL ClosedCaption;
    mciQuerySysValue(MSV_CLOSEDCAPTION, &ClosedCaption);
    cout << "MSV_CLOSEDCAPTION  :  " << int(ClosedCaption) << "\n";

    ULONG MasterVolume;
    mciQuerySysValue(MSV_MASTERVOLUME, &MasterVolume);
    cout << "MSV_MASTERVOLUME   :  " << int(MasterVolume) << "\n";

    ULONG Headphones;
    mciQuerySysValue(MSV_HEADPHONES, &Headphones);
    cout << "MSV_HEADPHONES     :  " << int(Headphones) << "\n";

    ULONG Speakers;
    mciQuerySysValue(MSV_SPEAKERS, &Speakers);
    cout << "MSV_SPEAKERS       :  " << int(Speakers) << "\n";
 
    CHAR WorkPath[CCHMAXPATH];
    mciQuerySysValue(MSV_WORKPATH, WorkPath);
    cout << "MSV_WORKPATH       :  " << WorkPath << "\n";
    
    ULONG SysqOsValue;
    mciQuerySysValue(MSV_SYSQOSVALUE, &SysqOsValue);
    cout << "MSV_SYSQOSVALUE    :  " << int(SysqOsValue) << "\n";

    ULONG SysqOsErrorFlag;
    mciQuerySysValue(MSV_SYSQOSERRORFLAG, &SysqOsErrorFlag);
    cout << "MSV_SYSQOSERRORFLAG:  " << int(SysqOsErrorFlag) << "\n";


    // that's all folks!

    return 0;
}

