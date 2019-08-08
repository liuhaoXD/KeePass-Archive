ECHO OFF
CLS

IF EXIST ..\Debug\KeePass.exe COPY /B ..\Debug\KeePass.exe /B ..\Distrib\KeePass.exe
IF EXIST ..\Release\KeePass.exe COPY /B ..\Release\KeePass.exe /B ..\Distrib\KeePass.exe

DEL ..\Distrib\KeePass.html
COPY /B KeePass.html /B ..\Distrib\KeePass.html

DEL ..\Distrib\License.html
COPY /B License.html /B ..\Distrib\License.html

DEL ..\Distrib\KeePass.ini
COPY /B KeePass.ini /B ..\Distrib\KeePass.ini

DEL Bulgarian.lng
Util\CprLang.exe BulgarianSrc.lng Bulgarian.lng

DEL Czech.lng
Util\CprLang.exe CzechSrc.lng Czech.lng

DEL Dutch.lng
Util\CprLang.exe DutchSrc.lng Dutch.lng

DEL Estonian.lng
Util\CprLang.exe EstonianSrc.lng Estonian.lng

DEL French.lng
Util\CprLang.exe FrenchSrc.lng French.lng

DEL German.lng
Util\CprLang.exe GermanSrc.lng German.lng

DEL Korean.lng
Util\CprLang.exe KoreanSrc.lng Korean.lng

DEL Portuguese.lng
Util\CprLang.exe PortugueseSrc.lng Portuguese.lng

DEL Swedish.lng
Util\CprLang.exe SwedishSrc.lng Swedish.lng

ECHO.
ECHO Have you updated the language information fields?

DEL ..\Debug\German.lng
COPY /B German.lng /B ..\Debug\German.lng > NUL
DEL ..\Debug\French.lng
COPY /B French.lng /B ..\Debug\French.lng > NUL
DEL ..\Debug\Dutch.lng
COPY /B Dutch.lng /B ..\Debug\Dutch.lng > NUL
DEL ..\Debug\Swedish.lng
COPY /B Swedish.lng /B ..\Debug\Swedish.lng > NUL
DEL ..\Debug\Korean.lng
COPY /B Korean.lng /B ..\Debug\Korean.lng > NUL
DEL ..\Debug\Czech.lng
COPY /B Czech.lng /B ..\Debug\Czech.lng > NUL
DEL ..\Debug\Estonian.lng
COPY /B Estonian.lng /B ..\Debug\Estonian.lng > NUL
DEL ..\Debug\Portuguese.lng
COPY /B Portuguese.lng /B ..\Debug\Portuguese.lng > NUL
DEL ..\Debug\Bulgarian.lng
COPY /B Bulgarian.lng /B ..\Debug\Bulgarian.lng > NUL

DEL ..\Release\German.lng
COPY /B German.lng /B ..\Release\German.lng > NUL
DEL ..\Release\French.lng
COPY /B French.lng /B ..\Release\French.lng > NUL
DEL ..\Release\Dutch.lng
COPY /B Dutch.lng /B ..\Release\Dutch.lng > NUL
DEL ..\Release\Swedish.lng
COPY /B Swedish.lng /B ..\Release\Swedish.lng > NUL
DEL ..\Release\Korean.lng
COPY /B Korean.lng /B ..\Release\Korean.lng > NUL
DEL ..\Release\Czech.lng
COPY /B Czech.lng /B ..\Release\Czech.lng > NUL
DEL ..\Release\Estonian.lng
COPY /B Estonian.lng /B ..\Release\Estonian.lng > NUL
DEL ..\Release\Portuguese.lng
COPY /B Portuguese.lng /B ..\Release\Portuguese.lng > NUL
DEL ..\Release\Bulgarian.lng
COPY /B Bulgarian.lng /B ..\Release\Bulgarian.lng > NUL

COPY /B German.lng /B Util\LangRef.src > NUL
