//  File newSDLib.cpp

//  Copyright (c) 2013
//  Author: Renzo Dani
//  Version: 1.0
//    
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#include <SD.h>
#include "config.h"
#include "newSDLib.h"


int initialSDCard()
{
	if (!SD.begin(SD_CS_PIN)) {
	Serial.println("initialization failed!");
	return 0;
  }
  Serial.println("initialization done.");
  return 1;
	  
}




