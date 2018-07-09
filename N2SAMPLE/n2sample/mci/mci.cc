//////////////////////////////////////////
// mci -- an interpreter for MCI scripts
//
// using emx 0.8h, mm4emx 1.0
//
//     based on a C program by
//     John J. McDonough
//
// Marc E.E. van Woerkom, 2/94
//


#include <iostream.h>
#include <fstream.h>

#include <os2.h>

#define  INCL_OS2MM
#define  INCL_MCIOS2
#include <os2me.h>


//
// this sample demonstrates the following MMPM/2 API's:
//
//     mciSendString()      send a MCI command string, receive an answer
//     mciGetErrorString()  look up the error string for the error code
//

int main(int argc, char* argv[])
{
    if ( argc != 2 ) {
        cerr << "usage: mci <filename>\n";
        return 1;
    }

    ifstream infile(argv[1]);
    if (!infile) {
        cerr << "error: can't open input file " << argv[1] << '\n';
        return 2;
    }

    int line = 0;

    const bsize = 128;
    char buff[bsize];

    while (!infile.eof()) {
        char c = infile.peek();  // peek one char forward

        if (infile.good()) {

            infile.get(buff, bsize, '\n');

            cout.width(3);
            cout << ++line << ": [" << buff << "]\n";

            if (c != '#') {

                const rsize = 128;
                char rbuff[rsize];
                for (int i=0; i<rsize; i++) rbuff[i] = 0;

                ULONG rc = mciSendString(buff,   // buffer with MCI string
                                         rbuff,  // return buffer
                                         rsize,  // rbuff size
                                         0,      // no callback window handle
                                         0);     // no user parameter

                if (rc == MCIERR_SUCCESS) {
                    if (rbuff[0])
                        cout << "      -> " << rbuff << "\n\n";
                }
                else {
                    ULONG rc2 = mciGetErrorString(rc,      // error code
                                                  rbuff,   // return buffer
                                                  rsize);  // rbuff size

                    if (rc2 == MCIERR_SUCCESS)
                        cerr << "      -> MCI error: " << rbuff << "\n\n";
                    else
                        cerr << "      -> error #" << rc << " has occured!\n\n";
                }
            }
        }

        infile.get(c);  // eat the \r after the \n
    }

    return 0;
}

