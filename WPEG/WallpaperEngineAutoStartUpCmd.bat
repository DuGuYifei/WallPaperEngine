set TARGET='%~dp0WallpaperEngineStart.exe'
set SHORTCUT='%USERPROFILE%\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup\WallpaperEngineStart.lnk'
set PWS=powershell.exe -ExecutionPolicy Bypass -NoLogo -NonInteractive -NoProfile

%PWS% -Command "$ws=New-Object -ComObject WScript.Shell;$s=$ws.CreateShortcut(%SHORTCUT%); $S.TargetPath=%TARGET%; $S.Save()"