; �ýű�ʹ�� HM VNISEdit �ű��༭���򵼲���

; ��װ�����ʼ���峣��
!define PRODUCT_NAME "iSCAN"
!define PRODUCT_VERSION "1.0"
!define PRODUCT_PUBLISHER "Kyle Zeng, Inc."
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\iSCAN.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

SetCompressor lzma

; ------ MUI �ִ����涨�� (1.67 �汾���ϼ���) ------
!include "MUI.nsh"

; MUI Ԥ���峣��
!define MUI_ABORTWARNING
!define MUI_ICON "Resource\exe.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; ��ӭҳ��
!insertmacro MUI_PAGE_WELCOME
; ���Э��ҳ��
!insertmacro MUI_PAGE_LICENSE "Licence.txt"
; ��װĿ¼ѡ��ҳ��
!insertmacro MUI_PAGE_DIRECTORY
; ��װ����ҳ��
!insertmacro MUI_PAGE_INSTFILES
; ��װ���ҳ��
!define MUI_FINISHPAGE_RUN "$INSTDIR\iSCAN.exe"
!insertmacro MUI_PAGE_FINISH

; ��װж�ع���ҳ��
!insertmacro MUI_UNPAGE_INSTFILES

; ��װ�����������������
!insertmacro MUI_LANGUAGE "SimpChinese"

; ��װԤ�ͷ��ļ�
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS
; ------ MUI �ִ����涨����� ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "iSCAN_Setup.exe"
InstallDir "$PROGRAMFILES\iSCAN"
InstallDirRegKey HKLM "${PRODUCT_UNINST_KEY}" "UninstallString"
ShowInstDetails show
ShowUnInstDetails show

Section "MainSection" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File "..\..\Desktop\Test\chartdir60.dll"
  CreateDirectory "$SMPROGRAMS\iSCAN"
  CreateShortCut "$SMPROGRAMS\iSCAN\iSCAN.lnk" "$INSTDIR\iSCAN.exe"
  CreateShortCut "$DESKTOP\iSCAN.lnk" "$INSTDIR\iSCAN.exe"
  File "..\..\Desktop\Test\D3Dcompiler_47.dll"
  SetOutPath "$INSTDIR\iconengines"
  File "..\..\Desktop\Test\iconengines\qsvgicon.dll"
  SetOutPath "$INSTDIR\imageformats"
  File "..\..\Desktop\Test\imageformats\qgif.dll"
  File "..\..\Desktop\Test\imageformats\qicns.dll"
  File "..\..\Desktop\Test\imageformats\qico.dll"
  File "..\..\Desktop\Test\imageformats\qjpeg.dll"
  File "..\..\Desktop\Test\imageformats\qsvg.dll"
  File "..\..\Desktop\Test\imageformats\qtga.dll"
  File "..\..\Desktop\Test\imageformats\qtiff.dll"
  File "..\..\Desktop\Test\imageformats\qwbmp.dll"
  File "..\..\Desktop\Test\imageformats\qwebp.dll"
  SetOutPath "$INSTDIR"
  File "..\..\Desktop\Test\iSCAN.exe"
  File "..\..\Desktop\Test\libEGL.dll"
  File "..\..\Desktop\Test\libGLESV2.dll"
  File "..\..\Desktop\Test\libusb-1.0.dll"
  File "..\..\Desktop\Test\opengl32sw.dll"
  SetOutPath "$INSTDIR\platforms"
  File "..\..\Desktop\Test\platforms\qwindows.dll"
  SetOutPath "$INSTDIR"
  File "..\..\Desktop\Test\Qt5Charts.dll"
  File "..\..\Desktop\Test\Qt5Core.dll"
  File "..\..\Desktop\Test\Qt5Gui.dll"
  File "..\..\Desktop\Test\Qt5SerialPort.dll"
  File "..\..\Desktop\Test\Qt5Sql.dll"
  File "..\..\Desktop\Test\Qt5Svg.dll"
  File "..\..\Desktop\Test\Qt5Widgets.dll"
  SetOutPath "$INSTDIR\sqldrivers"
  File "..\..\Desktop\Test\sqldrivers\qsqlite.dll"
  File "..\..\Desktop\Test\sqldrivers\qsqlmysql.dll"
  File "..\..\Desktop\Test\sqldrivers\qsqlodbc.dll"
  File "..\..\Desktop\Test\sqldrivers\qsqlpsql.dll"
  SetOutPath "$INSTDIR\styles"
  File "..\..\Desktop\Test\styles\qwindowsvistastyle.dll"
  SetOutPath "$INSTDIR\translations"
  File "..\..\Desktop\Test\translations\qt_ar.qm"
  File "..\..\Desktop\Test\translations\qt_bg.qm"
  File "..\..\Desktop\Test\translations\qt_ca.qm"
  File "..\..\Desktop\Test\translations\qt_cs.qm"
  File "..\..\Desktop\Test\translations\qt_da.qm"
  File "..\..\Desktop\Test\translations\qt_de.qm"
  File "..\..\Desktop\Test\translations\qt_en.qm"
  File "..\..\Desktop\Test\translations\qt_es.qm"
  File "..\..\Desktop\Test\translations\qt_fi.qm"
  File "..\..\Desktop\Test\translations\qt_fr.qm"
  File "..\..\Desktop\Test\translations\qt_gd.qm"
  File "..\..\Desktop\Test\translations\qt_he.qm"
  File "..\..\Desktop\Test\translations\qt_hu.qm"
  File "..\..\Desktop\Test\translations\qt_it.qm"
  File "..\..\Desktop\Test\translations\qt_ja.qm"
  File "..\..\Desktop\Test\translations\qt_ko.qm"
  File "..\..\Desktop\Test\translations\qt_lv.qm"
  File "..\..\Desktop\Test\translations\qt_pl.qm"
  File "..\..\Desktop\Test\translations\qt_ru.qm"
  File "..\..\Desktop\Test\translations\qt_sk.qm"
  File "..\..\Desktop\Test\translations\qt_uk.qm"
SectionEnd

Section -AdditionalIcons
  SetOutPath $INSTDIR
  CreateShortCut "$SMPROGRAMS\iSCAN\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\iSCAN.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\iSCAN.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd

/******************************
 *  �����ǰ�װ�����ж�ز���  *
 ******************************/

Section Uninstall
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\translations\qt_uk.qm"
  Delete "$INSTDIR\translations\qt_sk.qm"
  Delete "$INSTDIR\translations\qt_ru.qm"
  Delete "$INSTDIR\translations\qt_pl.qm"
  Delete "$INSTDIR\translations\qt_lv.qm"
  Delete "$INSTDIR\translations\qt_ko.qm"
  Delete "$INSTDIR\translations\qt_ja.qm"
  Delete "$INSTDIR\translations\qt_it.qm"
  Delete "$INSTDIR\translations\qt_hu.qm"
  Delete "$INSTDIR\translations\qt_he.qm"
  Delete "$INSTDIR\translations\qt_gd.qm"
  Delete "$INSTDIR\translations\qt_fr.qm"
  Delete "$INSTDIR\translations\qt_fi.qm"
  Delete "$INSTDIR\translations\qt_es.qm"
  Delete "$INSTDIR\translations\qt_en.qm"
  Delete "$INSTDIR\translations\qt_de.qm"
  Delete "$INSTDIR\translations\qt_da.qm"
  Delete "$INSTDIR\translations\qt_cs.qm"
  Delete "$INSTDIR\translations\qt_ca.qm"
  Delete "$INSTDIR\translations\qt_bg.qm"
  Delete "$INSTDIR\translations\qt_ar.qm"
  Delete "$INSTDIR\styles\qwindowsvistastyle.dll"
  Delete "$INSTDIR\sqldrivers\qsqlpsql.dll"
  Delete "$INSTDIR\sqldrivers\qsqlodbc.dll"
  Delete "$INSTDIR\sqldrivers\qsqlmysql.dll"
  Delete "$INSTDIR\sqldrivers\qsqlite.dll"
  Delete "$INSTDIR\Qt5Widgets.dll"
  Delete "$INSTDIR\Qt5Svg.dll"
  Delete "$INSTDIR\Qt5Sql.dll"
  Delete "$INSTDIR\Qt5SerialPort.dll"
  Delete "$INSTDIR\Qt5Gui.dll"
  Delete "$INSTDIR\Qt5Core.dll"
  Delete "$INSTDIR\Qt5Charts.dll"
  Delete "$INSTDIR\platforms\qwindows.dll"
  Delete "$INSTDIR\opengl32sw.dll"
  Delete "$INSTDIR\libusb-1.0.dll"
  Delete "$INSTDIR\libGLESV2.dll"
  Delete "$INSTDIR\libEGL.dll"
  Delete "$INSTDIR\iSCAN.exe"
  Delete "$INSTDIR\imageformats\qwebp.dll"
  Delete "$INSTDIR\imageformats\qwbmp.dll"
  Delete "$INSTDIR\imageformats\qtiff.dll"
  Delete "$INSTDIR\imageformats\qtga.dll"
  Delete "$INSTDIR\imageformats\qsvg.dll"
  Delete "$INSTDIR\imageformats\qjpeg.dll"
  Delete "$INSTDIR\imageformats\qico.dll"
  Delete "$INSTDIR\imageformats\qicns.dll"
  Delete "$INSTDIR\imageformats\qgif.dll"
  Delete "$INSTDIR\iconengines\qsvgicon.dll"
  Delete "$INSTDIR\D3Dcompiler_47.dll"
  Delete "$INSTDIR\chartdir60.dll"

  Delete "$SMPROGRAMS\iSCAN\Uninstall.lnk"
  Delete "$DESKTOP\iSCAN.lnk"
  Delete "$SMPROGRAMS\iSCAN\iSCAN.lnk"

  RMDir "$SMPROGRAMS\iSCAN"
  RMDir "$INSTDIR\translations"
  RMDir "$INSTDIR\styles"
  RMDir "$INSTDIR\sqldrivers"
  RMDir "$INSTDIR\platforms"
  RMDir "$INSTDIR\imageformats"
  RMDir "$INSTDIR\iconengines"

  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd

#-- ���� NSIS �ű��༭�������� Function ���α�������� Section ����֮���д���Ա��ⰲװ�������δ��Ԥ֪�����⡣--#

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "��ȷʵҪ��ȫ�Ƴ� $(^Name) ���������е������" IDYES +2
  Abort
FunctionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) �ѳɹ��ش����ļ�����Ƴ���"
FunctionEnd
