
/*
 * This file was generated by the SOM Compiler.
 * FileName: maprog.c.
 * Generated using:
 *     SOM Precompiler spc: 1.22
 *     SOM Emitter EMITC: 1.24
 */

/*
 * 
 * 
 *   Module Name: MAPROG (Multiple Argument Program)
 * 
 *   OS/2 Work Place Shell Sample Program
 * 
 * 
 */

#define MAProg_Class_Source
#include "maprog.ih"
#include "maprog.ph"
/*
 *  from WPProgram
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// The methods listed here are for use by the class, and not really by the
// objects themselves. hmod is necessary to get the icon and title from the
// resource file, and szTitle is just a place to store the name once it's
// read.

HMODULE hmod;
char    szTitle[100];

typedef struct _info {
   PDRAGINFO pDragInfo;
   MAProg* somSelf;
   char **argument;
   ULONG arguments;
} info;

void _Optlink startThread(void *pArgument)
{
   char **argument = ((info *)pArgument)->argument;
   ULONG arguments = ((info *)pArgument)->arguments;
   MAProg* somSelf = ((info *)pArgument)->somSelf;
   PDRAGINFO pDragInfo = ((info *)pArgument)->pDragInfo;
   PPROGDETAILS pProgDetails = NULL;
   ULONG size = 0;

   if (_wpQueryProgDetails(somSelf, NULL, &size))
      pProgDetails = (PPROGDETAILS)_wpAllocMem(somSelf, size, FALSE);

   if (_wpQueryProgDetails(somSelf, pProgDetails,  &size)) {

      int i;
      char *paramList = (char *)_wpAllocMem(somSelf,
                                            size + arguments*3 + strlen(pProgDetails->pszParameters),
                                            FALSE);
      char *parPos = strstr(pProgDetails->pszParameters, "%*");


        // This simple algorithm only understands the "%*" way of passing
        // arguments, and the working directory isn't even used. A cleverer
        // algorithm is left as an exercise to the reader :-)

      if (paramList && parPos) {
         strncpy(paramList, 
                 pProgDetails->pszParameters,
                 parPos - pProgDetails->pszParameters);
         paramList[parPos - pProgDetails->pszParameters] = '\0';
         for (i = 0; i < arguments; i++) {
            strcat(paramList, "\"");
            strcat(paramList, argument[i]);
            strcat(paramList, "\" ");
            _wpFreeMem(somSelf, (PBYTE)argument[i]);
            argument[i] = NULL;
         } /* endfor */
         _wpFreeMem(somSelf, (PBYTE)argument);
         argument = NULL;
         strcat(paramList, parPos+2);
         pProgDetails->pszParameters = paramList;
         WinStartApp(NULLHANDLE, pProgDetails, paramList, NULL, SAF_STARTCHILDAPP);
      }
      _wpFreeMem(somSelf, (PBYTE)paramList);
      paramList = NULL;
   } /* endif */

   _wpFreeMem(somSelf, (PBYTE)pProgDetails);
   _wpFreeMem(somSelf, (PBYTE)pArgument);
}

/*
 * 
 *    Methods from the WPObject class
 * 
 */




/* 
 *
 *  The parent method starts the associated program once for every item
 *  dropped. We want to start the program once, with all the items added as
 *  parameters.
 *  Just like when answering DM_DROP messages, this method must return 0.
 */

SOM_Scope MRESULT   SOMLINK maprog_wpDrop(MAProg *somSelf,
                                          HWND hwndCnr,
                                          PDRAGINFO pdrgInfo,
                                          PDRAGITEM pdrgItem)
{
   info *pi = (info *)_wpAllocMem(somSelf, sizeof(info), FALSE);
   int i = 0, size = 0;
   ULONG arguments = DrgQueryDragitemCount(pdrgInfo);
   char** argument = (char **)_wpAllocMem(somSelf,
                                          arguments * sizeof(char *),
                                          FALSE);

   /* MAProgData *somThis = MAProgGetData(somSelf); */
   MAProgMethodDebug("MAProg","maprog_wpDrop");

   if (!DrgAccessDraginfo(pdrgInfo)||
       DrgQueryDragitemPtr(pdrgInfo, 0) != pdrgItem) // Yucky way to make
                                                     // sure we only deal
                                                     // with this method
                                                     // once/drop
       
      return 0;


   // Now that we have the information, copy it quickly and leave the
   // message queue to the ones needing it. We'll do the real job in
   // another thread, to save message queue time.

   for (i=0; i < arguments; i++) {
      char *s = NULL;                   
      PDRAGITEM p = DrgQueryDragitemPtr(pdrgInfo, i);
      int lenContainer = DrgQueryStrNameLen(p->hstrContainerName);
      int lenSource = DrgQueryStrNameLen(p->hstrSourceName);

      argument[i] = (char *)_wpAllocMem(somSelf,
                                        lenContainer + lenSource +1,
                                        FALSE);
      size+= lenContainer + lenSource;

      if (argument[i]) {
         DrgQueryStrName(p->hstrContainerName,
                         lenContainer+1,
                         argument[i]);
         DrgQueryStrName(p->hstrSourceName,
                         lenSource+1,
                         argument[i]+lenContainer);
      } /* endif */
   } /* endfor */
   pi->somSelf = somSelf;
   pi->pDragInfo = pdrgInfo;
   pi->arguments = arguments;
   pi->argument = argument;
   _beginthread(startThread, NULL, 4096, (PVOID)pi);
   DrgDeleteDraginfoStrHandles(pdrgInfo);
   DrgFreeDraginfo(pdrgInfo);
   return 0;
}

/*
 *
 * Load the name from the resource file. If it, for whatever reason can't be
 * found, call the parent method and return its name.
 * Return the pointer to the string.
 */

#undef SOM_CurrentClass
#define SOM_CurrentClass SOMMeta
SOM_Scope PSZ   SOMLINK maprogM_wpclsQueryTitle(M_MAProg *somSelf)
{
    /* M_MAProgData *somThis = M_MAProgGetData(somSelf); */
    M_MAProgMethodDebug("M_MAProg","maprogM_wpclsQueryTitle");

    if (!WinLoadString(WinQueryAnchorBlock(HWND_DESKTOP), 
                       _clsQueryModuleHandle(somSelf),
                       ID_TITLE,
                       sizeof(szTitle), szTitle))
        strcpy(szTitle, parent_wpclsQueryTitle(somSelf));

    return szTitle;
}



/*
 *
 *  Fill the INCONINFO structure with the necessary data so that the WPS
 *  can load the icon and display it when needed.
 *  Return the size of the ICONINFO structure.
 */

#undef SOM_CurrentClass
#define SOM_CurrentClass SOMMeta
SOM_Scope ULONG   SOMLINK maprogM_wpclsQueryIconData(M_MAProg *somSelf,
		PICONINFO pIconInfo)
{
   APIRET rc;

    /* M_MAProgData *somThis = M_MAProgGetData(somSelf); */
    M_MAProgMethodDebug("M_MAProg","maprogM_wpclsQueryIconData");

    if (pIconInfo)
    {
       /*
        *   fill in icon information
        */
       pIconInfo->fFormat = ICON_RESOURCE;
       pIconInfo->hmod= _clsQueryModuleHandle(somSelf);
       pIconInfo->resid= ID_ICON;
    }
    return (sizeof(ICONINFO));
}


/*
 *
 *  Return the module handle of the DLL, so the icon and title can be
 *  loaded from the resource file.
 */

#undef SOM_CurrentClass
#define SOM_CurrentClass SOMMeta
SOM_Scope HMODULE   SOMLINK maprogM_clsQueryModuleHandle(M_MAProg *somSelf)
{
    APIRET rc;

    /* M_MAProgData *somThis = M_MAProgGetData(somSelf); */
    M_MAProgMethodDebug("M_MAProg","maprogM_clsQueryModuleHandle");

   /*
    *   Make sure we already have module handle
    */

    if (hmod == NULLHANDLE)
    {
       zString zsPathName;
       /*
        *   Retrieve registered pathname of our module (DLL) and query the
        *   module handle.
        */
       zsPathName = _somLocateClassFile(SOMClassMgrObject,
                                        SOM_IdFromString("MAProg"),
                                        MAProg_MajorVersion,
                                        MAProg_MinorVersion);
       rc = DosQueryModuleHandle( zsPathName, &hmod);
       if (rc)
       {
          DebugBox("maprogM_wpclsInitData", "Failed to load module");
          return;
       }

    }   /* end if (hmod == NULLHANDLE) */

    return (hmod);
}
