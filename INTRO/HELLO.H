#define CLS_CLIENT               "HelloClass"

#define MYM_SETNAME              (WM_USER)

typedef struct _NAMEDLGINFO {
   USHORT usSzStruct;
   CHAR achName[64];
} NAMEDLGINFO, *PNAMEDLGINFO;

MRESULT EXPENTRY nameDlgProc(HWND hwndWnd,
                             ULONG ulMsg,
                             MPARAM mpParm1,
                             MPARAM mpParm2);
MRESULT EXPENTRY windowProc(HWND hwndWnd,
                            ULONG ulMsg,
                            MPARAM mpParm1,
                            MPARAM mpParm2);
INT main(VOID);
