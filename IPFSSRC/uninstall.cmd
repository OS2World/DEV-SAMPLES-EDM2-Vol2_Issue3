/* deinstallation program */

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

result = SysDeregisterObjectClass('MAProg')
if result then
say "Multiple Argument Program deinstalled";
else
say "Failed to uninstall class";
