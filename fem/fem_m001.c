/*
   File name: fem_m001.c
   Date:      2003/04/16 21:01
   Author:    Jiri Brozovsky

   Copyright (C) 2003 

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   in a file called COPYING along with this program; if not, write to
   the Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA
   02139, USA.

	FEM Solver - Material 000 (empty)
*/

#include "fem_mat.h"

int mat001_test(long mPos) 
{ 
	/* Some datatest needed ;-) */
	return(AF_OK); 
}


int addMat_001(void)
{
	int rv = AF_OK;
	static long  type      = 1 ;
	static long  num       = 7 ;
	static long  val[7]    = {MAT_DENS, MAT_EX, MAT_NU, MAT_ALPHA, MAT_KXX, MAT_C, MAT_PRICE};
	static long  num_rp    = 6 ;
	static long  val_rp[6] = {MAT_DENS, MAT_EX, MAT_NU, MAT_ALPHA, MAT_KXX, MAT_C};

	if (type != femAddMat(type)) {return(AF_ERR_VAL);}
	Mat[type].num = num ;
	Mat[type].val = val ;
	Mat[type].num_rp = num_rp ;
	Mat[type].val_rp = val_rp ;

	Mat[type].test = mat001_test;
	return(rv);
}

/* end of fem_e001.c */
