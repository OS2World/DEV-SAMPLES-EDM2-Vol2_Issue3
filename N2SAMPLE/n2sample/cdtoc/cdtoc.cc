////////////////////////////////
// cdtoc - audio CD toc sample
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
#include <iomanip.h>


//
// mci_err: translate the MCI return code into an error string
//

void mci_err(ULONG rc)
{
    const rsize = 128;
    char rbuff[rsize];

    ULONG rc2 = mciGetErrorString(rc,      // error code
                                  rbuff,   // return buffer
                                  rsize);  // rbuff size

    if (rc2 == MCIERR_SUCCESS)
        cerr << "MCI error: " << rbuff << "\n\n";
    else
        cerr << "error #" << rc << " has occured!\n\n";
}


//
// print_mmtime: print time given in MMTIME as hh.mm.ss
//

void print_mmtime(ULONG mmtime)
{
    ULONG hms = HMSFROMMM(mmtime);

    // hms packing is: |--|ss|mm|hh|

    int hour = int(ULONG_LWLB(hms));
    int min  = int(ULONG_LWHB(hms));
    int sec  = int(ULONG_HWLB(hms));

    if (hour)
        cout << setw(4) << setfill('0')
             << hour << '.'
             << setfill('0');
    else
        cout << setfill(' ');  // I believe this shouldn't be neccessary

    cout << setw(2) << min << '.';

    cout << setw(2) << setfill('0')
         << sec
         << setfill(' ');       // this neither
}


//
// main
//

int main()
{
    cout << "cdtoc -- Audio CD Table of Contents\n\n";


    // open the audio CD device

    MCI_OPEN_PARMS mop;

    mop.hwndCallback = 0;
    mop.usDeviceID = 0;
    mop.pszDeviceType = MCI_DEVTYPE_CD_AUDIO_NAME;
    mop.pszElementName = 0;

    ULONG rc = mciSendCommand(0,
                              MCI_OPEN,                       // open message
                              MCI_WAIT | MCI_OPEN_SHAREABLE,  // message flags
                              &mop,                           // parameters
                              0);

    if (rc != MCIERR_SUCCESS) {
        mci_err(rc);
        return 1;
    }


    // ask for the table of contents

    const MAXTOCRECS = 99;
    MCI_TOC_REC mtr[MAXTOCRECS];

    MCI_TOC_PARMS mtp;
 
    mtp.hwndCallback = 0;
    mtp.pBuf = mtr;
    mtp.ulBufSize = sizeof(mtr);

    rc = mciSendCommand(mop.usDeviceID,  // device ID
                        MCI_GETTOC,      // get toc message
                        MCI_WAIT,        // message flags
                        &mtp,            // parameters
                        0);

    if (rc != MCIERR_SUCCESS) mci_err(rc);


    // close the device

    MCI_GENERIC_PARMS mgp;
    mgp.hwndCallback = 0;

    ULONG rc2 = mciSendCommand(mop.usDeviceID, MCI_CLOSE, MCI_WAIT, &mgp, 0);
 
    if (rc2 != MCIERR_SUCCESS) mci_err(rc2);


    // now show the TOC, if been successful

    if (rc == MCIERR_SUCCESS) {
        int i = 0;

        while (mtr[i].TrackNum) {
            cout << "Track" << setw(3);
            cout << int(mtr[i].TrackNum)
                 << ":  Length ";
            print_mmtime(mtr[i].ulEndAddr - mtr[i].ulStartAddr);
            cout << "  [";
            print_mmtime(mtr[i].ulStartAddr);
            cout << " to ";
            print_mmtime(mtr[i].ulEndAddr);
            cout << "]  Control " << int(mtr[i].Control)
                 << ", Country " << mtr[i].usCountry
                 << ", Owner " << mtr[i].ulOwner
                 << ", #" << mtr[i].ulSerialNum << "\n";
            i++;
        }
    }
    

    // that's all folks!

    return 0;
}

