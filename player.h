//  File player.h

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

#ifndef __PLAYER_H
#define __PLAYER_H


#define READ_BUF_LEN  32

void pressed(char presseChar);
void scanKey();
void AvailableProcessorTime();
int playFile();

#endif

