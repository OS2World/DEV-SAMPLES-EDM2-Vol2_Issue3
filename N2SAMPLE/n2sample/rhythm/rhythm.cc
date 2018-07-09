///////////////////////////////////////////////
// rhythm - waveaudio playlist sample (EDM/2)
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


// prototypes 

void mci_err(ULONG);
void mmio_err(ULONG);


//
// mmaudioheader
//

class mmaudioheader {
    MMAUDIOHEADER mmah;
public:
    mmaudioheader();
    MMAUDIOHEADER* get_addr() { return &mmah; }
    LONG           get_size() { return sizeof(mmah); }
};


mmaudioheader::mmaudioheader()
{
    char* p = (char*) &mmah;
    for (int i=0; i<sizeof(mmah); i++) p[i] = 0;
}


//
// mem_wav: a waveaudio file loaded into memory
//

class mem_wav {
    HMMIO  hmmio;
    PSZ    bptr;
    ULONG  bsize;
    ULONG  SamplesPerSec;
    USHORT BitsPerSample;
public:
    mem_wav(char*);
    ~mem_wav();
    PSZ    get_bptr() { return bptr; }
    ULONG  get_bsize() { return bsize; }
    ULONG  get_SamplesPerSec() { return SamplesPerSec; }
    USHORT get_BitsPerSample() { return BitsPerSample; }
};


mem_wav::mem_wav(char* name)
{
    // open the file

    hmmio = mmioOpen(name, 0, MMIO_READ);


    // get header

    mmaudioheader mmah;

    ULONG BytesRead = 0;

    ULONG rc = mmioGetHeader(hmmio, mmah.get_addr(), mmah.get_size(),
                             &BytesRead, 0, 0);

    if (rc != MMIO_SUCCESS) mmio_err(rc);


    // get some infos about the waveaudio file

    SamplesPerSec = mmah.get_addr()->mmXWAVHeader.WAVEHeader.ulSamplesPerSec;
    BitsPerSample = mmah.get_addr()->mmXWAVHeader.WAVEHeader.usBitsPerSample;

    bsize = mmah.get_addr()->mmXWAVHeader.XWAVHeaderInfo.ulAudioLengthInBytes;
    bptr  = new CHAR[bsize];


    // read file

    rc = mmioRead(hmmio, bptr, bsize);
 
    if (rc == MMIO_ERROR) mmio_err(rc);

    cout << "[file " << name
         << ": read" << setw(7) << rc
         << " bytes of" << setw(7) << bsize << "]\n";


    // close file

    rc = mmioClose(hmmio, 0);
    if (rc != MMIO_SUCCESS) mmio_err(rc);
}


mem_wav::~mem_wav()
{
    delete[] bptr;
}


//
// ple: a playlist entry
//

struct ple {
    ULONG operation;
    ULONG operand1;
    ULONG operand2;
    ULONG operand3;
};


//
// playlist: a waveaudio playlist
//

class playlist {
    ple*     pl;
    int      size, used;
    mem_wav& m1, m2, m3, m4, m5, s1, p1;
    void setup();
public:
    playlist(mem_wav&, mem_wav&, mem_wav&, mem_wav&,
             mem_wav&, mem_wav&, mem_wav&, int);
    ~playlist();
    ple* get_addr() { return pl; }
    int add(ULONG =0, ULONG =0, ULONG =0, ULONG =0);
    int add_branch(ULONG);
    int add_call(ULONG);
    int add_data(mem_wav&);
    int add_exit();
    int add_return();
};


playlist::playlist(mem_wav& M1, mem_wav& M2, mem_wav& M3,
                   mem_wav& M4, mem_wav& M5, mem_wav& S1, mem_wav& P1,
                   int Size)
    : m1(M1), m2(M2), m3(M3), m4(M4), m5(M5), s1(S1), p1(P1),
      size(Size)
{
    if (size < 1) cerr << "error: wrong playlist size!\n";

    pl = new ple[size];
    used = 0;

    setup();
}


playlist::~playlist()
{
    delete[] pl;
}


int playlist::add(ULONG op=0, ULONG opd1=0, ULONG opd2=0, ULONG opd3=0)
{
    if (used >= size) {
        cerr << "error: playlist is too small!\n";
        return -1;
    }

    pl[used].operation = op;
    pl[used].operand1  = opd1;
    pl[used].operand2  = opd2;
    pl[used].operand3  = opd3;

    return used++;
}


int playlist::add_branch(ULONG addr)
{
    return add(BRANCH_OPERATION, 0, addr);
}


int playlist::add_call(ULONG addr)
{
    return add(CALL_OPERATION, 0, addr);
}


int playlist::add_data(mem_wav& mw)
{
    return add(DATA_OPERATION, ULONG(mw.get_bptr()), mw.get_bsize());
}


int playlist::add_exit()
{
    return add(EXIT_OPERATION);
}


int playlist::add_return()
{
    return add(RETURN_OPERATION);
}


void playlist::setup()
{
                  add_branch(70);

    ULONG Intro = add_data(m1);
                  add_data(m1);
                  add_data(m1);
                  add_return();

    ULONG A10a =  add_data(m2);
                  add_data(s1);
                  add_data(p1);
                  add_data(m3);
                  add_data(m4);
                  add_data(m5);
                  add_data(m4);
                  add_data(m5);
                  add_data(m4);
                  add_data(m5);
                  add_return();

    ULONG A10 =   add_data(m2);
                  add_data(s1);
                  add_data(p1);
                  add_data(m5);
                  add_data(m4);
                  add_data(m5);
                  add_data(m4);
                  add_data(m5);
                  add_data(m4);
                  add_data(m5);
                  add_return();

    ULONG B10 =   add_data(m2);
                  add_data(s1);
                  add_data(p1);
                  add_data(m1);
                  add_data(m1);
                  add_data(m1);
                  add_data(m1);
                  add_data(m1);
                  add_data(m1);
                  add_data(m1);
                  add_return();

    ULONG C10 =   add_data(m2);
                  add_data(s1);
                  add_data(p1);
                  add_data(m2);
                  add_data(m2);
                  add_data(m2);
                  add_data(m2);
                  add_data(m2);
                  add_data(m2);
                  add_data(m2);
                  add_return();

    ULONG A6 =    add_data(m2);
                  add_data(s1);
                  add_data(p1);
                  add_data(m5);
                  add_data(m4);
                  add_data(m5);
                  add_return();

    ULONG B6 =    add_data(m2);
                  add_data(s1);
                  add_data(p1);
                  add_data(m1);
                  add_data(m1);
                  add_data(m1);
                  add_return();

    ULONG C6 =    add_data(m2);
                  add_data(s1);
                  add_data(p1);
                  add_data(m2);
                  add_data(m2);
                  add_data(m2);
                  add_return();
// #70
                  add_call(Intro);
                  add_call(A10a);
                  add_call(B10);
                  add_call(A10);
                  add_call(B10);
                  add_call(A10);
                  add_call(C10);
                  add_call(A6);
                  add_call(B6);
                  add_call(A6);
                  add_call(C10);
                  add_call(A10);
                  add_call(C10);
                  add_call(A10);
                  add_call(A10);
                  add_call(A10);
                  add_call(C10);
                  add_call(A6);
                  add_call(C6);
                  add_call(A6);
                  add_call(B10);
                  add_data(s1);
                  add_exit();
}


//
// waveaudio: a waveaudio device
//

class waveaudio {
    MCI_OPEN_PARMS mop;
public:
    waveaudio(playlist&, mem_wav&);
    ~waveaudio();
    void play();
};


waveaudio::waveaudio(playlist& pl, mem_wav& mw)
{
    // open device

    mop.hwndCallback   = 0;
    mop.usDeviceID     = 0;
    mop.pszDeviceType  = MCI_DEVTYPE_WAVEFORM_AUDIO_NAME;
    mop.pszElementName = PSZ(pl.get_addr());

    ULONG rc = mciSendCommand(0,
                              MCI_OPEN,                        // open message
                              MCI_WAIT | MCI_OPEN_SHAREABLE |  // message flags
                              MCI_OPEN_PLAYLIST,
                              &mop,                            // parameters
                              0);

    if (rc != MCIERR_SUCCESS) mci_err(rc);


    // set device parameters

    MCI_WAVE_SET_PARMS wsp;

    wsp.hwndCallback    = 0;
    wsp.ulSamplesPerSec = mw.get_SamplesPerSec();
    wsp.usBitsPerSample = mw.get_BitsPerSample();

    rc = mciSendCommand(mop.usDeviceID,
                        MCI_SET,
                        MCI_WAIT |
                        MCI_WAVE_SET_SAMPLESPERSEC |
                        MCI_WAVE_SET_BITSPERSAMPLE,
                        &wsp,
                        0);

    if (rc != MCIERR_SUCCESS) mci_err(rc);
}


waveaudio::~waveaudio()
{
    // close device

    MCI_GENERIC_PARMS mgp;

    mgp.hwndCallback = 0;

    ULONG rc = mciSendCommand(mop.usDeviceID,
                              MCI_CLOSE,
                              MCI_WAIT,
                              &mgp,
                              0);

    if (rc != MCIERR_SUCCESS) mci_err(rc);
}


void waveaudio::play()
{
    // play the playlist

    MCI_PLAY_PARMS mpp;

    mpp.hwndCallback = 0;

    ULONG rc = mciSendCommand(mop.usDeviceID,
                              MCI_PLAY,
                              MCI_WAIT,
                              &mpp,
                              0);

    if (rc != MCIERR_SUCCESS) mci_err(rc);
}


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
// main
//

int main()
{
    cout << "rhythm -- a Rhythm Generator\n\n";


    // load waveaudio files into memory

    cout << "loading waveaudio files into memory ...\n\n";

    mem_wav m1("m1.wav");
    mem_wav m2("m2.wav");
    mem_wav m3("m3.wav");
    mem_wav m4("m4.wav");
    mem_wav m5("m5.wav");
    mem_wav s1("s1.wav");
    mem_wav p1("p1.wav");


    // set up playlist

    cout << "\nsetting up playlist ...\n\n";

    playlist pl(m1, m2, m3, m4, m5, s1, p1, 100);


    // play playlist

    cout << "and ... go!   (you should pump up the volume :-)\n\n";

    waveaudio wav(pl, m1);
    wav.play();
     

    // that's all folks!

    cout << "... done. yeah!\n";

    return 0;
}

