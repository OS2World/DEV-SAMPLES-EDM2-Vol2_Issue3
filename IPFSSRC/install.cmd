/* installation program */

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

file =directory()"\maprog.dll";
result = SysRegisterObjectClass("MAProg", file)
if result then
say "Multiple Argument Program class registered"
else
say "Failed to register class"

