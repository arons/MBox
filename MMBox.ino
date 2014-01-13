//  Author: Renzo Dani
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

#include <Arduino.h>
#include <avr/io.h>
#include "config.h"
#include "player.h"
#include "vs10xx.h"

#include "newSDLib.h"

void setup()
{
  Serial.begin(9600);

  InitSPI();

  InitIOForVs10xx();

  Mp3Reset();
  if (!initialSDCard())
    return;

  RED_LED_ON();//light red LED
}


void loop()
{ 
  playFile(); 
}



