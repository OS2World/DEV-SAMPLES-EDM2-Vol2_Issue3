/* The bitmap of a menu entry having style MIS_BITMAP is loaded from
   the main executable.  This function loads it from the resource DLL.

   Input:  hwndFrame    Handle of the frame window owning the menu
           hMod         Module handle of the DLL containing the bitmap
                        resource
           idMenu       ID of the menu item (must have style MIS_BITMAP)
           idBitmap     ID of the bitmap
*/

void load_menu_bitmap (HWND hwndFrame, HMODULE hmod,
                       USHORT idMenu, USHORT idBitmap)
{
  HPS hps;
  HWND hwndMenu;
  HBITMAP hbm;

  hps = WinGetScreenPS (HWND_DESKTOP);
  hwndMenu = WinWindowFromID (hwndFrame, FID_MENU);
  hbm = GpiLoadBitmap (hps, hmod, idBitmap, 0, 0);
  WinSendMsg (hwndMenu, MM_SETITEMHANDLE,
              MPFROMSHORT (idMenu), MPFROMLONG (hbm));
  WinReleasePS (hps);
}
