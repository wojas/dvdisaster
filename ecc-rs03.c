/*  dvdisaster: Additional error correction for optical media.
 *  Copyright (C) 2004-2017 Carsten Gnoerlich.
 *
 *  Email: carsten@dvdisaster.org  -or-  cgnoerlich@fsfe.org
 *  Project homepage: http://www.dvdisaster.org
 *
 *  This file is part of dvdisaster.
 *
 *  dvdisaster is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  dvdisaster is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with dvdisaster. If not, see <http://www.gnu.org/licenses/>.
 */

#include "dvdisaster.h"

#include "rs03-includes.h"

/***
 *** Method registration
 ***/

static void destroy(Method*);


void register_rs03(void)
{  Method *method = g_malloc0(sizeof(Method));

   method->ckSumClosure = g_malloc0(sizeof(RS03CksumClosure));

   /*** Standard infomation and methods */ 

   strncpy(method->name, "RS03", 4);
   method->menuEntry = g_strdup(_("Multithreaded RS codec (RS03)"));
   method->description = g_strdup(_("Multithreaded Reed-Solomon codec for error correction files and augmented images"));
   method->create  = RS03Create;
   method->fix     = RS03Fix;
   method->verify  = RS03Verify;

   /*** Linkage to rs03-common.c */

   method->expectedImageSize = RS03ExpectedImageSize;
   method->getCrcBuf         = RS03GetCrcBuf;

   /*** Linkage to rs03-recognize.c */

   method->recognizeEccFile  = RS03RecognizeFile;
   method->recognizeEccImage = RS03RecognizeImage;

   /*** Linkage to rs03-window.c */

   method->createCreateWindow = CreateRS03EncWindow;
   method->createFixWindow    = CreateRS03FixWindow;

   method->resetCreateWindow = ResetRS03EncWindow;
   method->resetFixWindow    = ResetRS03FixWindow;

   method->createPrefsPage   = CreateRS03PrefsPage;
   method->resetPrefsPage    = ResetRS03PrefsPage;
   method->readPreferences   = ReadRS03Preferences;

   /*** Linkage to rs03-verify.c */

   method->createVerifyWindow = CreateRS03VerifyWindow;
   method->resetVerifyWindow  = ResetRS03VerifyWindow;

   /*** Register ourself */

   method->destroy = destroy;

   RegisterMethod(method);
}

static void destroy(Method *method)
{  RS03Widgets *wl = (RS03Widgets*)method->widgetList;
   RS03CksumClosure *csc = (RS03CksumClosure*)method->ckSumClosure;

   if(csc->lay)
      g_free(csc->lay);
   g_free(method->ckSumClosure);

   if(wl)
   {  if(wl->fixCurve) FreeCurve(wl->fixCurve);

      if(wl->cmpSpiral)
	FreeSpiral(wl->cmpSpiral);

      if(wl->cmpLayout)
	g_object_unref(wl->cmpLayout);

      g_free(wl);
   }
}

