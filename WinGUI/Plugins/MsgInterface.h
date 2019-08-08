/*
  KeePass Password Safe - The Open-Source Password Manager
  Copyright (C) 2003-2007 Dominik Reichl <dominik.reichl@t-online.de>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef ___KEEPASS_MESSAGE_INTERFACE_H___
#define ___KEEPASS_MESSAGE_INTERFACE_H___

#include "KeePluginDef.h"

void KPMI_SetMainDialog(void *pMainPwSafeDlg);

C_FN_SHARE DWORD_PTR KP_API KP_Call(DWORD dwCode, LPARAM lParamW, LPARAM lParamL, LPARAM lParamM);
C_FN_SHARE DWORD_PTR KP_API KP_Query(DWORD dwCode, LPARAM lParam);

#endif // ___KEEPASS_MESSAGE_INTERFACE_H___