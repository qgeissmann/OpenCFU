[Setup]
DefaultDirName={pf32}\OpenCFU
OutputDir=..\
MinVersion=5.1
AppName=OpenCFU
AppVerName=3.8.8
LanguageDetectionMethod=none
AppID={{7807E304-090C-4F99-8371-1993E17C7568}
AppCopyright=Quentin Geissmann
ChangesAssociations=true
SetupIconFile=..\data\icons\opencfu.ico
PrivilegesRequired=poweruser
ShowLanguageDialog=no
DefaultGroupName=OpenCFU
UsePreviousGroup=true
LicenseFile=..\COPYING

[Files]
Source: ..\data\*.xml; DestDir: {app}\data
Source: ..\data\icons\opencfu.ico; DestDir: {app}\data\icons
Source: ..\data\banner.png; DestDir: {app}\data
Source: ..\data\logo.png; DestDir: {app}\data

Source: ..\data\icons\24x24\opencfu.png; DestDir: {app}\data\icons\24x24\apps
Source: ..\data\icons\48x48\opencfu.png; DestDir: {app}\data\icons\48x48\apps
Source: ..\data\icons\64x64\opencfu.png; DestDir: {app}\data\icons\64x64\apps
Source: ..\data\icons\128x128\opencfu.png; DestDir: {app}\data\icons\128x128\apps



Source: ..\opencfu.exe; DestDir: {app}\bin

Source: C:\MinGW\bin\libstdc++-6.dll; DestDir: {app}\bin
Source: C:\MinGW\bin\libgomp-1.dll; DestDir: {app}\bin
Source: C:\MinGW\bin\pthreadGC2.dll; DestDir: {app}\bin

Source: C:\opencv-2.4.5\build\bin\libopencv_core245.dll; DestDir: {app}\bin
Source: C:\opencv-2.4.5\build\bin\libopencv_highgui245.dll; DestDir: {app}\bin
Source: C:\opencv-2.4.5\build\bin\libopencv_imgproc245.dll; DestDir: {app}\bin
Source: C:\opencv-2.4.5\build\bin\libopencv_ml245.dll; DestDir: {app}\bin

; put the gtk+ runtime together
Source: C:\gtk+\bin\libgtk*.dll ; DestDir: {app}\bin
Source: C:\gtk+\bin\libgdk*.dll ; DestDir: {app}\bin

Source: C:\gtk+\bin\libpango*.dll ; DestDir: {app}\bin
Source: C:\gtk+\bin\libcairo*.dll ; DestDir: {app}\bin
Source: C:\gtk+\bin\libatk*.dll ; DestDir: {app}\bin
Source: C:\gtk+\bin\libgio*.dll ; DestDir: {app}\bin
Source: C:\gtk+\bin\libglib*.dll ; DestDir: {app}\bin
Source: C:\gtk+\bin\libsigc*.dll ; DestDir: {app}\bin
Source: C:\gtk+\bin\libgmodule*.dll ; DestDir: {app}\bin
Source: C:\gtk+\bin\libgobject*.dll ; DestDir: {app}\bin
Source: C:\gtk+\bin\libgthread*.dll ; DestDir: {app}\bin
Source: C:\gtk+\bin\libexpat*.dll ; DestDir: {app}\bin
Source: C:\gtk+\bin\libfontconfig*.dll ; DestDir: {app}\bin
Source: C:\gtk+\bin\libpng14*.dll ; DestDir: {app}\bin
Source: C:\gtk+\bin\intl.dll ; DestDir: {app}\bin
Source: C:\gtk+\bin\freetype6.dll ; DestDir: {app}\bin
Source: C:\gtk+\bin\zlib1.dll ; DestDir: {app}\bin
Source: C:\gtk+\bin\libgcc*.dll ; DestDir: {app}\bin

Source: C:\gtk+\etc\pango\* ; DestDir: {app}\etc\pango\
Source: C:\gtk+\etc\gtk-2.0\*; DestDir: {app}\etc\gtk-2.0
Source: C:\gtk+\share\themes\MS-Windows\gtk-2.0\*; DestDir: {app}\share\themes\MS-Windows\gtk-2.0
Source: C:\gtk+\lib\gtk-2.0\2.10.0\engines\*; DestDir: {app}\lib\gtk-2.0\2.10.0\engines
Source: C:\gtk+\share\themes\MS-Windows\gtk-2.0\*; DestDir: {app}\share\themes\MS-Windows\gtk-2.0


[Icons]
Name: {group}\OpenCFU; Filename: {app}\bin\opencfu.exe; IconFilename: {app}\data\icons\opencfu.ico
Name: {group}\Uninstall OpenCFU; Filename: {app}\unins000.exe
Name: {group}\Website; Filename: http://opencfu.sourceforge.net/
Name: {group}\PLoS ONE publication; Filename: http://www.plosone.org/article/info%3Adoi%2F10.1371%2Fjournal.pone.0054072
