//////////////////////////////////////////
// mmiofmts - MMPM/2 mmio formats sample
//
// using emx 0.8h, mm4emx 1.0
//
//
// Marc E.E. van Woerkom, 2/94
//


#include <os2.h>

#define  INCL_OS2MM
#include <os2me.h>

#include <iostream.h>
#include <iomanip.h>


//
// mmformatinfo
//

class mmformatinfo {
    MMFORMATINFO mmfi;
public:
    mmformatinfo(FOURCC IOProc=0);
    MMFORMATINFO* get_addr() { return &mmfi; }
    LONG          get_NameLength() { return mmfi.lNameLength; }
    PSZ           get_DefaultFormatExt() { return mmfi.szDefaultFormatExt; }
    FOURCC        get_IOProc() { return mmfi.fccIOProc; }
};


mmformatinfo::mmformatinfo(FOURCC IOProc=0)
{
    char* p = (char*) &mmfi;
    for (int i=0; i<sizeof(mmfi); i++) p[i] = 0;

    mmfi.fccIOProc = IOProc;
}


//
// mmio_err: translate MMIO error code into a string
//

void mmio_err(ULONG rc)
{
    cerr << "MMIO error: ";

    char* s;

    switch (rc) {
    case MMIO_SUCCESS:
        s = "SUCCESS (huh?)";
        break;
    case MMIOERR_UNBUFFERED:
        s = "UNBUFFERD";
        break;
    case MMIOERR_INVALID_HANDLE:
        s = "INVALID HANDLE";
        break;
    case MMIOERR_INVALID_PARAMETER:
        s = "INVALID PARAMETER";
        break;
    case MMIOERR_READ_ONLY_FILE:
        s = "READ ONLY FILE";
        break;
    case MMIOERR_WRITE_ONLY_FILE:
        s = "WRITE ONLY FILE";
        break;
    case MMIOERR_WRITE_FAILED:
        s = "WRITE FAILED";
        break;
    case MMIOERR_READ_FAILED:
        s = "READ FAILED";
        break;
    case MMIOERR_SEEK_FAILED:
        s = "SEEK FAILED";
        break;
    case MMIOERR_NO_FLUSH_NEEDED:
        s = "NO FLUSH NEEDED";
        break;
    case MMIOERR_OUTOFMEMORY:
        s = "OUT OF MEMORY";
        break;
    case MMIOERR_CANNOTEXPAND:
        s = "CANNOT EXPAND";
        break;
    case MMIOERR_FREE_FAILED:
        s = "FREE FAILED";
        break;
    case MMIOERR_CHUNKNOTFOUND:
        s = "CHUNK NOT FOUND";
        break;
    case MMIO_ERROR:
        s = "ERROR";
        break;
    case MMIO_WARNING:
        s = "WARNING";
        break;
    case MMIO_CF_FAILURE:
        s = "CF FAILURE";
        break;
    default:
        cerr << rc;
        s = " (hmm...)";
    }

    cerr << s << "\n";
}


//
// main procedure
//
//
// WARNING: The MMPM/2 stops working on my system
//          if I use a string as argument for the FOURCC mask
//          which is none of the registered ones! (e.g. wuff)
//
//          Looks like a MMPM/2 bug to me.
//          (A major confusion of mmio.dll?)
//

int main(int argc, char* argv[])
{
    cout << "mmiofmts -- MMPM/2 MMIO Formats\n\n";


    // parse args

    FOURCC mask = 0;

    if (argc>1) {
        mask = mmioStringToFOURCC(argv[1], MMIO_TOUPPER);

        cout << "mask in use is [";
        char* p = (char*) &mask;
        for (int i=0; i<4; i++) cout << p[i];
        cout << "]\n\n";
    }


    // query # of IOProcedures

    mmformatinfo mmfi_spec(mask);

    ULONG NumFormats = 0;

    ULONG rc = mmioQueryFormatCount(mmfi_spec.get_addr(),
                                    &NumFormats,
                                    0,
                                    0);

    if (rc != MMIO_SUCCESS) {
        mmio_err(rc);
        return 1;
    }

    cout << "formats supported: " << NumFormats;

    if (!NumFormats) return 0;


    // get formats

    mmformatinfo* mmfip = new mmformatinfo[NumFormats];

    ULONG FormatsRead = 0;

    rc = mmioGetFormats(mmfi_spec.get_addr(),
                        NumFormats,
                        mmfip,
                        &FormatsRead,
                        0,
                        0);

    if (rc != MMIO_SUCCESS) {
        mmio_err(rc);
        return 2;
    }

    cout << "  (" << FormatsRead << " formats read)\n\n";


    // print information

    cout << "no.   4-cc   name                                     leng  extn\n\n";

    for (int i=0; i<NumFormats; i++) {
        cout.setf(ios::right, ios::adjustfield);
        cout << setw(2) << i+1 << ":  [";

        FOURCC IOProc = mmfip[i].get_IOProc();
        char* p = (char*) &IOProc;

        for (int j=0; j<4; j++) cout << p[j];
        cout << "]  ";

        LONG NameLength = mmfip[i].get_NameLength();

        if (NameLength) {
            PSZ name = new CHAR[NameLength+1];
            LONG BytesRead = 0;

            rc = mmioGetFormatName(mmfip[i].get_addr(),
                                   name,
                                   &BytesRead,
                                   0,
                                   0);

            name[NameLength] = 0;

            if (rc != MMIO_SUCCESS) {
                mmio_err(rc);
                cout << "     ";
            }
            else {
                cout.setf(ios::left, ios::adjustfield);
                cout << setw(40) << name << " ("
                     << setw(2) << BytesRead;
            }

            delete[] name;
        }
        else
            cout << "-" << setw(43) << "( 0";

        cout.setf(ios::left, ios::adjustfield);
        cout << ")  ."
             << setw(3) << mmfip[i].get_DefaultFormatExt() << "\n";
    }

    delete[] mmfip;


    // that's all folks!

    return 0;
}

