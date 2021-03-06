/*
   File name: fem_gfnt.c
   Date:      2003/11/17 16:20
   Author:    Jiri Brozovsky

   Copyright (C) 2003 Jiri Brozovsky

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
  
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.
  
   You should have received a copy of the GNU Library General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
   USA.

	 Graphics output for "fem" (pre|post)processor - main file

	 $Id: fem_gfnt.c,v 1.5 2003/12/15 21:19:28 jirka Exp $
*/

#include "fem_gfx.h"
#ifdef _USE_GFX_

#ifdef USE_GLUT
#include <GL/glut.h>
#else
unsigned char fontB[]= {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x3c, 0x3c, 0x3c, 0x18, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x66, 0x66, 0x66, 0x00,
0x00, 0x00, 0x00, 0x00, 0x6c, 0x6c, 0xfe, 0x6c, 0x6c, 0x6c, 0xfe, 0x6c, 0x6c, 0x00, 0x00, 0x00,
0x00, 0x00, 0x18, 0x18, 0x7c, 0xc6, 0x86, 0x06, 0x06, 0x7c, 0xc0, 0xc2, 0xc6, 0x7c, 0x18, 0x18,
0x00, 0x00, 0x00, 0x00, 0x86, 0xc6, 0x60, 0x30, 0x18, 0x0c, 0xc6, 0xc2, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x76, 0xcc, 0xcc, 0xcc, 0xdc, 0x76, 0x38, 0x6c, 0x6c, 0x38, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x30, 0x30, 0x30, 0x00,
0x00, 0x00, 0x00, 0x00, 0x0c, 0x18, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0c, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x30, 0x18, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x18, 0x30, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x3c, 0xff, 0x3c, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x7e, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x30, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x02, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x38, 0x6c, 0xc6, 0xc6, 0xd6, 0xd6, 0xc6, 0xc6, 0x6c, 0x38, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x38, 0x18, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xfe, 0xc6, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06, 0xc6, 0x7c, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0x06, 0x06, 0x06, 0x3c, 0x06, 0x06, 0xc6, 0x7c, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x1e, 0x0c, 0x0c, 0x0c, 0xfe, 0xcc, 0x6c, 0x3c, 0x1c, 0x0c, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0x06, 0x06, 0x06, 0xfc, 0xc0, 0xc0, 0xc0, 0xfe, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0xfc, 0xc0, 0xc0, 0x60, 0x38, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0c, 0x06, 0x06, 0xc6, 0xfe, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x78, 0x0c, 0x06, 0x06, 0x06, 0x7e, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x30, 0x18, 0x18, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x06, 0x0c, 0x18, 0x30, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x0c, 0x18, 0x30, 0x60, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x0c, 0xc6, 0xc6, 0x7c, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x7c, 0xc0, 0xdc, 0xde, 0xde, 0xde, 0xc6, 0xc6, 0x7c, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xfe, 0xc6, 0xc6, 0x6c, 0x38, 0x10, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xfc, 0x66, 0x66, 0x66, 0x66, 0x7c, 0x66, 0x66, 0x66, 0xfc, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x3c, 0x66, 0xc2, 0xc0, 0xc0, 0xc0, 0xc0, 0xc2, 0x66, 0x3c, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xf8, 0x6c, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x6c, 0xf8, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xfe, 0x66, 0x62, 0x60, 0x68, 0x78, 0x68, 0x62, 0x66, 0xfe, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xf0, 0x60, 0x60, 0x60, 0x68, 0x78, 0x68, 0x62, 0x66, 0xfe, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x3a, 0x66, 0xc6, 0xc6, 0xde, 0xc0, 0xc0, 0xc2, 0x66, 0x3c, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xfe, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x3c, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x78, 0xcc, 0xcc, 0xcc, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x1e, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xe6, 0x66, 0x66, 0x6c, 0x78, 0x78, 0x6c, 0x66, 0x66, 0xe6, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xfe, 0x66, 0x62, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0xf0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xd6, 0xfe, 0xfe, 0xee, 0xc6, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xce, 0xde, 0xfe, 0xf6, 0xe6, 0xc6, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xf0, 0x60, 0x60, 0x60, 0x60, 0x7c, 0x66, 0x66, 0x66, 0xfc, 0x00, 0x00,
0x00, 0x00, 0x0e, 0x0c, 0x7c, 0xde, 0xd6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xe6, 0x66, 0x66, 0x66, 0x6c, 0x7c, 0x66, 0x66, 0x66, 0xfc, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc6, 0x06, 0x0c, 0x38, 0x60, 0xc6, 0xc6, 0x7c, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x3c, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x5a, 0x7e, 0x7e, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x10, 0x38, 0x6c, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x6c, 0xee, 0xfe, 0xd6, 0xd6, 0xd6, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xc6, 0xc6, 0x6c, 0x7c, 0x38, 0x38, 0x7c, 0x6c, 0xc6, 0xc6, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x3c, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x66, 0x66, 0x66, 0x66, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xfe, 0xc6, 0xc2, 0x60, 0x30, 0x18, 0x0c, 0x86, 0xc6, 0xfe, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x3c, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3c, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x02, 0x06, 0x0e, 0x1c, 0x38, 0x70, 0xe0, 0xc0, 0x80, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x3c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x3c, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc6, 0x6c, 0x38, 0x10,
0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x30, 0x30,
0x00, 0x00, 0x00, 0x00, 0x76, 0xcc, 0xcc, 0xcc, 0x7c, 0x0c, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x7c, 0x66, 0x66, 0x66, 0x66, 0x6c, 0x78, 0x60, 0x60, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc0, 0xc0, 0xc0, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x76, 0xcc, 0xcc, 0xcc, 0xcc, 0x6c, 0x3c, 0x0c, 0x0c, 0x1c, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc0, 0xc0, 0xfe, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xf0, 0x60, 0x60, 0x60, 0x60, 0xf0, 0x60, 0x64, 0x6c, 0x38, 0x00, 0x00,
0x00, 0x78, 0xcc, 0x0c, 0x7c, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0x76, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xe6, 0x66, 0x66, 0x66, 0x66, 0x76, 0x6c, 0x60, 0x60, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x3c, 0x18, 0x18, 0x18, 0x18, 0x18, 0x38, 0x00, 0x18, 0x18, 0x00, 0x00,
0x00, 0x3c, 0x66, 0x66, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x0e, 0x00, 0x06, 0x06, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xe6, 0x66, 0x6c, 0x78, 0x78, 0x6c, 0x66, 0x60, 0x60, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x3c, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x38, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xc6, 0xd6, 0xd6, 0xd6, 0xd6, 0xfe, 0xec, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0xdc, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0xf0, 0x60, 0x60, 0x7c, 0x66, 0x66, 0x66, 0x66, 0x66, 0xdc, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x1e, 0x0c, 0x0c, 0x7c, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0x76, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xf0, 0x60, 0x60, 0x60, 0x66, 0x76, 0xdc, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0x0c, 0x38, 0x60, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x1c, 0x36, 0x30, 0x30, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x10, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x76, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x18, 0x3c, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x6c, 0xfe, 0xd6, 0xd6, 0xd6, 0xc6, 0xc6, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xc6, 0x6c, 0x38, 0x38, 0x38, 0x6c, 0xc6, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0xf8, 0x0c, 0x06, 0x7e, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xfe, 0xc6, 0x60, 0x30, 0x18, 0xcc, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x0e, 0x18, 0x18, 0x18, 0x18, 0x70, 0x18, 0x18, 0x18, 0x0e, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x70, 0x18, 0x18, 0x18, 0x18, 0x0e, 0x18, 0x18, 0x18, 0x70, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdc, 0x76, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xc6, 0xc6, 0xc6, 0x6c, 0x38, 0x10, 0x00, 0x00, 0x00, 0x00,
};
#endif

#ifdef USE_GLUT
void *MyGlutFont = GLUT_BITMAP_HELVETICA_12;
#endif

#ifdef USE_GL2PS
int   afFontPlotWidth = 16 ;
#endif



/** Plots string to OpenGL space
	@param Str   = string
	@param X,Y,Z = coordinates
	@param RGB[4] = color, if NULL, default color is used
 */
void femPlotString(char *Str, double X, double Y, double Z, double *RGB)
{
#ifdef USE_GLUT
	int i;
#endif

  /* black on paper, white on screen (maybe) */
  if (plotProp.PlotToFile == 1)
	{	
		if (RGB == NULL) { glColor4f(0,0,0,1); }
		
    glRasterPos3f(X,Y,Z);
		
#ifdef USE_GL2PS
    gl2psText(Str,"Times",afFontPlotWidth);
#endif
	}
  else
  {		
		if (RGB == NULL) 
		{
			glColor4f( plotProp.textColor[0], plotProp.textColor[1],
			           plotProp.textColor[2], plotProp.textColor[3]);
		}
	}

    glRasterPos3f(X,Y,Z);

#ifdef USE_GLUT
    for (i=0; i<strlen(Str); i++)
    {
      glutBitmapCharacter(MyGlutFont,Str[i]);
	  }
#else
    while (*Str) 
    { 
      if ((*Str)>=32)
      {
        glBitmap(8, 16, 0.0f, 0.0f, 9.0f, 0.0f, fontB+(((*Str++)-32)<<4)); 
      }
    }
#endif
}

/** Plots string to OpenGL space
	@param Str   = string
	@param RGB[4] = color, if NULL, default color is used
 */
void femPlotStringHere(char *Str, double *RGB)
{
#ifdef USE_GLUT
	int i;
#endif

  /* black on paper, white on screen (maybe) */
  if (plotProp.PlotToFile == 1)
	{	
		if (RGB == NULL) {glColor4f(0,0,0,1);}
#ifdef USE_GL2PS
    gl2psText(Str,"Times",afFontPlotWidth);
#endif
	}
  else
  {		
		if (RGB == NULL) 
		{
			glColor4f( plotProp.textColor[0], plotProp.textColor[1],
			           plotProp.textColor[2], plotProp.textColor[3]);
		}
	}

#ifdef USE_GLUT
  for (i=0; i<strlen(Str); i++)
  {
    glutBitmapCharacter(MyGlutFont,Str[i]);
	}
#else
  while (*Str) 
  { 
    if ((*Str)>=32)
    {
      glBitmap(8, 16, 0.0f, 0.0f, 9.0f, 0.0f, fontB+(((*Str++)-32)<<4)); 
    }
  }
#endif
}


/** Plots double to OpenGL space
	@param Val   = double
	@param RGB[4] = color, if NULL, default color is used
 */
void femPlotDoubleHere(double Val, double *RGB)
{
	char Str[FEM_STR_LEN+1];

#if 0
	sprintf(Str,"%e",Val);
#else
#endif
	sprintf(Str,"%1.5e",Val);
  femPlotStringHere(Str, RGB);
}



/** Plots integer to OpenGL space
	@param Val   = integer
	@param X,Y,Z = coordinates
	@param RGB[4] = color, if NULL, default color is used
 */
void femPlotInt(int Val, double X, double Y, double Z, double *RGB)
{
	char Str[FEM_STR_LEN+1];

	sprintf(Str,"%i",Val);
  femPlotString(Str, X, Y, Z,RGB);
}

/** Plots double to OpenGL space
	@param Val   = double
	@param X,Y,Z = coordinates
	@param RGB[4] = color, if NULL, default color is used
 */
void femPlotDouble(double Val, double X, double Y, double Z, double *RGB)
{
	char Str[FEM_STR_LEN+1];

	sprintf(Str,"%e",Val);
  femPlotString(Str, X, Y, Z,RGB);
}

#endif
/* end of fem_gfnt.c */
