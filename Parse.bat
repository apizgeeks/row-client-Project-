flex -olex.cpp string.l

@echo off
rem *** Make sure Bison can find these files
rem move bison.simple . >nul
rem move bison.hairy  . >nul

rem *** Run Bison to generate the parser
bison --defines --verbose -o parse.cpp string.y

rem *** Put the files back
rem move bison.simple .. >nul
rem move bison.hairy .. >nul

rem *** Rename parse.cpp.h to lexsymb.h
if exist lexsymb.h del lexsymb.h
ren parse.cpp.h lexsymb.h

pause

:End