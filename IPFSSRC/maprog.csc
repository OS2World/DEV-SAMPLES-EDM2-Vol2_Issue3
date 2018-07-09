/******************************************************************************
*
*  Module Name: MAPROG (Multiple Argument Program)
*
*  Simple OS/2 Work Place Shell Program
*
******************************************************************************/

include <wppgm.sc>      // We need the definition of WPProgram, so it can be
                        // inherited from.

#******************************************************************************
#   The new class
#******************************************************************************

class: MAProg,
       external stem   = maprog,
       local,
       external prefix = maprog_,
       classprefix     = maprogM_,
       major version   = 1,
       minor version   = 1;

--
-- DESCRIPTION:
--
--    This is a program object that understands several files being dropped
--    on it.
--
--    An instance of this class can be created as a Workplace object.
--

#******************************************************************************
#   After having specified some of the basics of the class, we need to go
#   into details. First, what class is the parent. Be careful with name
#   confusions. The include files of the classes are shortened to fir the
#   FAT file name convention. The real names of the classes are not.
#******************************************************************************

parent: WPProgram;

#******************************************************************************
#   Now it may be a good thing to send some private details to the .C file
#   that will be created. In this case, little is needed, but a shorthand
#   way of fireing up a message box is always nice to have, for debugging
#   purposes.
#******************************************************************************

passthru: C.ph;

#ifdef DEBUG
    #define DebugBox(title, text) \
                  WinMessageBox(HWND_DESKTOP,   \
                                HWND_DESKTOP,   \
                                (PSZ) text,     \
                                (PSZ) title,    \
                                20,             \
                                MB_OK | MB_INFORMATION | MB_MOVEABLE)
#else
    #define DebugBox(title, text)
#endif

endpassthru;   /* C.ph */

#******************************************************************************
#   Now, the details necessary for the implementation.
#******************************************************************************

passthru: C.ih;

   #define INCL_WIN
   #define INCL_DOS
   #define INCL_WINSTDDRAG
   #define INCL_DOSSESMGR

   #include <os2.h>

   #define INCL_WPCLASS
   #define INCL_WPFOLDER

   #include <pmwp.h>

   #define ID_TITLE           100       // the title string identifier
   #define ID_ICON            101       // the icon identifier


endpassthru;   /* .ih */

#******************************************************************************
#   Now the stuff that might be neat later. In this case, nothing at all...
#******************************************************************************

passthru: C.h, after;


endpassthru;   /* C.h */

#******************************************************************************
#   If we needed instance data, this would be the place for it, in a
#   data:       xxxx
#   statement.
#   We don't need any instance data beyond what already exists in the parent
#   class, though.
#******************************************************************************


#******************************************************************************
#   Making a new class would be worthless unless as a minimum some of the
#   methods of the parent class are to be overridden. New methods might also
#   be added. In this case, however, no new INSTANCE methods are needed.
#******************************************************************************

methods:

#******************************************************************************
#   The only instance method necessary to override to perform what we want,
#   is the wpDrop method. wpOpen should be overridden in a more serious
#   class, but this is just an example.
#******************************************************************************

-- from WPProgram

override wpDrop;
##
## Syntax: MRESULT wpDrop(HWND hwndCnr,       // the origin of the drag op.
##                        PDRAGINFO pdrgInfo, // normal DM_DROP info
##                        PDRAGITEM pdrgItem) // One of the items...
--
-- The parent method starts the associated program once for every item
-- dropped. We want to start the program once, with all the items added as
-- parameters.
-- Just like when answering DM_DROP messages, this method must return 0.



#******************************************************************************
#   No new INSTANCE methods were needed, but one new CLASS method is. There
#   must be a way to get the module handle so the icon can be retieved.
#******************************************************************************

HMODULE  clsQueryModuleHandle (), class;
##
## Syntax: HMODULE clsQueryModuleHandle()
--
-- Return the module handle of the DLL, so the icon and titme can be
-- loaded from the resource file.


#******************************************************************************
#   Specify class methods being overridden
#******************************************************************************

override wpclsQueryTitle, class; 
##
## Syntax: PSZ wpclsQueryTitle() // Should be overridden by ALL classes that
##                               // have a template, so that the template can
##                               // be given a name.
--
-- Load the name from the resource file. If it, for whatever reason can't be
-- found, call the parent method and return its name.
-- Return the pointer to the string.



override wpclsQueryIconData, class;
##
## Syntax: ULONG wpclsQueryIconData(PICONINFO pIconInfo)
##
## Should be overridden by ALL classes that have an icon of their own.
--
-- Fill the INCONINFO structure with the necessary data so that the WPS
-- can load the icon and display it when needed.
-- Return the size of the ICONINFO structure.


