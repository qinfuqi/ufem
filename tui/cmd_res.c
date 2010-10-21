/*
   File name: cmd_res.c
   Date:      2010/10/18 09:68
   Author:    Jiri Brozovsky

   Copyright (C) 2010 Jiri Brozovsky

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

	 FEM User Interface - commands related to results

*/

#include "fem_tui.h"
#include "cint.h"
#include "fdb_fem.h"
#include "fdb_geom.h"
#include "fdb_edef.h"
#include "fdb_res.h"

extern FILE *fdbPrnFile ;  /* file pointer */
extern long  fdbOutputFormat ;
extern char *fdbPrnFileName ;
extern long  fdbPrnAutoName ;
extern long  fdbPrnAppendFile ;
extern char *fdbPrnViewCommand;

/** Prints reaction solution: "prrs"
 * @param cmd command
 * @return status
 */
int func_fem_prrs(char *cmd)
{
	int     rv   = AF_OK ;

	FEM_TEST_POSTPROCESSOR

  rv = fdbResListReacts(fdbPrnFile);

	return ( tuiCmdReact(cmd, rv) ) ;
}

/** Prints DOF solution: "prdof"
 * @param cmd command
 * @return status
 */
int func_fem_prdof(char *cmd)
{
	int     rv   = AF_OK ;

	FEM_TEST_POSTPROCESSOR

  rv = fdbResListDOFSolu(fdbPrnFile);

	return ( tuiCmdReact(cmd, rv) ) ;
}


/** Prints element solution: "pres,item1[,ttem2,..]"
 * @param cmd command
 * @return status
 */
int func_fem_pres(char *cmd)
{
	int     rv   = AF_OK ;
  long   *fld = NULL;
  long    len = 0 ;
  long    i ;

	FEM_TEST_POSTPROCESSOR

  len = ciParNum(cmd) - 1 ;

  if (len < 1)
  {
    fprintf(msgout,"[E] %s!\n",
        _("At least one result type must be specified"));
	  return ( tuiCmdReact(cmd, AF_ERR_EMP) ) ;
  }

  if ((fld=(long *)malloc(len*sizeof(long))) == NULL)
  {
    fprintf(msgout,"[E] %s!\n",
        _("Out of memory for results"));
	  return ( tuiCmdReact(cmd, AF_ERR_MEM) ) ;
  }

  for (i=0; i<len; i++) { fld[i] = ciGetParInt(cmd, i+1) ; }

  rv = fdbResListElemSolu(fdbPrnFile, fld, len);

  free(fld); fld = NULL ;

	return ( tuiCmdReact(cmd, rv) ) ;
}


/** Prints AVERAGED nodal solution: "prns,item1[,ttem2,..]"
 * @param cmd command
 * @return status
 */
int func_fem_prns(char *cmd)
{
	int     rv   = AF_OK ;
  long   *fld = NULL;
  long    len = 0 ;
  long    i ;

	FEM_TEST_POSTPROCESSOR

  len = ciParNum(cmd) - 1 ;

  if (len < 1)
  {
    fprintf(msgout,"[E] %s!\n",
        _("At least one result type must be specified"));
	  return ( tuiCmdReact(cmd, AF_ERR_EMP) ) ;
  }

  if ((fld=(long *)malloc(len*sizeof(long))) == NULL)
  {
    fprintf(msgout,"[E] %s!\n",
        _("Out of memory for results"));
	  return ( tuiCmdReact(cmd, AF_ERR_MEM) ) ;
  }

  for (i=0; i<len; i++) { fld[i] = ciGetParInt(cmd, i+1) ; }

  rv = fdbResListAvSolu(fdbPrnFile, fld, len);

  free(fld); fld = NULL ;

	return ( tuiCmdReact(cmd, rv) ) ;
}

/** Sets sets output format: "outform,(plain|tex|html)"
 * @param cmd command
 * @return status
 */
int func_fem_set_output(char *cmd)
{
	int     rv   = AF_OK ;
  char   *name = NULL  ;

  if (ciParNum(cmd) > 1)
  {
    if ( (name = ciGetParStr(cmd,1)) != NULL )
    {
      ciStrCompr(name);
      if (strlen(name) > 0)
      {
        switch (name[0])
        {
          case 'l':
          case 'L':
          case 't':
          case 'T': fdbOutputFormat = FDB_FORMAT_LTX; break;
          case 'h':
          case 'H': fdbOutputFormat = FDB_FORMAT_HTML; break;
          default:  fdbOutputFormat = FDB_FORMAT_TEXT; break;
        }
      }
      else
      {
        fdbOutputFormat = FDB_FORMAT_TEXT ;
      }
    }
    else
    {
      fdbOutputFormat = FDB_FORMAT_TEXT ;
    }
    free(name) ; name = NULL ;
  }
  else
  {
    fdbOutputFormat = FDB_FORMAT_TEXT ;
  }

	return ( tuiCmdReact(cmd, rv) ) ;
}


/** Sets if output file name is autogenerated: "outauto,(yes|no)"
 * Empty filename means stdout
 * @param cmd command
 * @return status
 */
int func_fem_set_outauto(char *cmd)
{
	int     rv   = AF_OK ;

  if (ciParNum(cmd) > 1)
  {
    if (ciGetParInt(cmd, 1) == 1)
    {
      if (fdbPrnFileName != NULL)
      {
        free(fdbPrnFileName);
        fdbPrnFileName = NULL ;
      }
    }

    fdbPrnAutoName = AF_YES ;
    fdbPrnFile = NULL ;
    fdbPrnAppendFile = AF_NO ;
  }
	return ( tuiCmdReact(cmd, rv) ) ;
}

/** Sets output file name: "outfile[,filename]"
 * Empty filename means stdout
 * @param cmd command
 * @return status
 */
int func_fem_set_outfile(char *cmd)
{
	int     rv   = AF_OK ;

  if (fdbPrnFileName != NULL)
  {
    free(fdbPrnFileName);
    fdbPrnAutoName = AF_YES ;
    fdbPrnFileName = NULL ;
    fdbPrnAppendFile = AF_YES ;
  }

  if (ciParNum(cmd) <= 1)
  {
    fdbPrnFile = stdout ;
  }
  else
  {
    if (ciGetParStr(cmd, 1) != NULL)
    {
      fdbPrnFileName = ciGetParStr(cmd, 1) ;
      fdbPrnAutoName = AF_NO ;
      fdbPrnFile = NULL ;
      fdbPrnAppendFile = AF_NO ;
    }
    else
    {
      fdbPrnFile = stdout ;
    }
  }

	return ( tuiCmdReact(cmd, rv) ) ;
}

/** Sets output file viewer: "outview[,filename]"
 * Empty filename means no viewer
 * @param cmd command
 * @return status
 */
int func_fem_set_outview(char *cmd)
{
	int     rv   = AF_OK ;

  if (fdbPrnViewCommand != NULL)
  {
    free(fdbPrnViewCommand);
    fdbPrnViewCommand = NULL ;
  }

  if (ciParNum(cmd) > 1)
  {
    if (ciGetParStr(cmd, 1) != NULL)
    {
      fdbPrnViewCommand = ciGetParStr(cmd, 1) ;
    }
  }

	return ( tuiCmdReact(cmd, rv) ) ;
}

/* ----------------------------------------- */

/** Compute 2D vector scale: "tensorscale,plane,x,y,z,lines,radius,division"
 * @param cmd command
 * @return status
 */
int func_fem_comp_vec_scale2D (char *cmd)
{
	int    rv      = AF_OK ;
	long   plane   = 1;
	double px      = 0.0 ;
	double py      = 0.0 ;
	double pz      = 0.0 ;
	long   lines   = 2;
	double radius  = 1.0;
	long   linediv = 1;
	double a       = 0.0 ;
	double b       = 0.0 ;
	double angle   = 0.0 ;
	long   mat     = 0 ;

	FEM_TEST_POSTPROCESSOR

	if (ciParNum(cmd) < 8)
	{
		fprintf(msgout,"[E] %s: \"%s,%s\"!\n",
			 	_("All parameters are required:"),
				ciGetParStr(cmd, 0),
        "plane,x,y,z,lines,radius,division");
		return ( tuiCmdReact(cmd, AF_ERR_EMP) ) ;
	}
	
	plane   = ciGetParInt(cmd, 1); 
	px      = ciGetParDbl(cmd, 2); 
	py      = ciGetParDbl(cmd, 3); 
	pz      = ciGetParDbl(cmd, 4); 
	lines   = ciGetParInt(cmd, 5); 
	radius  = ciGetParDbl(cmd, 6); 
	linediv = ciGetParInt(cmd, 7); 

	rv = fdbTS2D_tensor_scale(plane,
	 	                     px, 
												 py, 
												 pz, 
												 lines, 
												 radius, 
												 linediv,
												 &a,
												 &b,
												 &angle,
												 &mat);
  
	return ( tuiCmdReact(cmd, rv) ) ;
}

/** Compute 2D vector scale: "tensoraniso,plane,x,y,z,step,nstep,material,lines,radius,division"
 * @param cmd command
 * @return status
 */
int func_fem_comp_vec_scale_aniso2D (char *cmd)
{
	int    rv      = AF_OK ;
	long   plane   = 1;
	double px      = 0.0 ;
	double py      = 0.0 ;
	double pz      = 0.0 ;
	double dist    = 1.0 ;
	long   ndist   = 1 ;
	long   lines   = 2;
	double radius  = 1.0;
	long   linediv = 1;
	long   mat     = 1 ;
	double a       = 0.0 ;
	double b       = 0.0 ;
	double angle   = 0.0 ;
	double aniso   = 0.0 ;

	FEM_TEST_POSTPROCESSOR

	if (ciParNum(cmd) < 11)
	{
		fprintf(msgout,"[E] %s: \"%s,%s\"!\n",
			 	_("All parameters are required:"),
				ciGetParStr(cmd, 0),
        "plane,x,y,z,step,nsteps,material,lines,radius,division");
		return ( tuiCmdReact(cmd, AF_ERR_EMP) ) ;
	}
	
	plane   = ciGetParInt( cmd,  1 ); 
	dist    = ciGetParDbl( cmd,  2 ); 
	ndist   = ciGetParInt( cmd,  3 ); 
	mat     = ciGetParInt( cmd,  4 ); 
	px      = ciGetParDbl( cmd,  5 ); 
	py      = ciGetParDbl( cmd,  6 ); 
	pz      = ciGetParDbl( cmd,  7 ); 
	lines   = ciGetParInt( cmd,  8 ); 
	radius  = ciGetParDbl( cmd,  9 ); 
	linediv = ciGetParInt( cmd, 10 ); 

	rv = fdbTS2D_global_tensor_scale(plane,
	 	                     px, 
												 py, 
												 pz, 
												 dist,
												 ndist,
												 mat,
												 lines, 
												 radius, 
												 linediv,
												 &a,
												 &b,
												 &angle,
												 &aniso);

  fprintf(stdout,"[ ] %s: %s = %e, %s = %e\n", 
      _("Tensor Scale Data"),
      _("Angle"), angle, 
      _("Anisotrophy"), aniso);
  
	return ( tuiCmdReact(cmd, rv) ) ;
}

/** Fill variable with selections status (1=selected, 0=unselected): "fillvarissel,[n|e|d|f|el|k|ge|l|a|v],id"
 * @param cmd command
 * @return status
 */
int func_fill_var_is_selected(char *cmd)
{
  int rv  = AF_OK ;
  int   i ;
  char *var = NULL ;
  char *dir = NULL ;
	char  value[CI_STR_LEN];
  long  id, type, type_id ;

	for (i=0; i<CI_STR_LEN; i++) { value[i] = '\0' ; }
  
  if (ciParNum(cmd) < 4)
	{
		fprintf(msgout,"[E] %s: \"%s,%s\"!\n",
			 	_("All parameters are required:"),
				ciGetParStrNoExpand(cmd, 0),
        "variable,entity_type,entity_number");
		return ( tuiCmdReact(cmd, AF_ERR_EMP) ) ;
	}

  if (strlen(var = ciGetParStrNoExpand(cmd, 1)) < 1) 
	{
		free(var);
		fprintf(msgout,"[E] %s!\n", 
				_("Invalid variable"));
		return(tuiCmdReact(cmd, AF_ERR_VAL));
	}
  
  if (strlen(dir = ciGetParStrNoExpand(cmd, 2)) < 1) 
	{
		free(dir);
    rv = AF_ERR_VAL ;
		fprintf(msgout,"[E] %s!\n", _("Invalid item name"));
		goto memFree;
	}

  if ((id = ciGetParInt(cmd,3)) <1)
  {
    rv = AF_ERR_VAL ;
		fprintf(msgout,"[E] %s!\n", _("Invalid item number"));
		goto memFree;
  }
 
  switch(dir[0])
  {
    case 'n': type = NODE ; type_id = NODE_ID ; break ;
    case 'e': type = ELEM ; type_id = ELEM_ID ;
              if (strlen(dir) > 1)
              {
                if (dir[1] == 'l') 
                { 
                  type = ELOAD ; 
                  type_id = ELOAD_ID ; 
                }
              }
              break ;
    case 'd': type = NDISP ; type_id = NDISP_ID ; break ;
    case 'f': type = NLOAD ; type_id = NLOAD_ID ; break ;
    case 'l': 
    case 'a': 
    case 'v': 
    case 'g': type = ENTITY ; type_id = ENTITY_ID ; break ;
    case 'k': type = KPOINT ; type_id = KPOINT_ID ; break ;
    default: 
            rv = AF_ERR_VAL ;
		        fprintf(msgout,"[E] %s!\n", _("Invalid data type"));
            goto memFree ;
            break;
  }

	sprintf(value,"%li",fdb_user_get_sel(type, type_id, id) );
	rv = ciAddVar(var, value) ;
  
memFree:
  free(dir); dir = NULL ;
  free(var); var = NULL ;

	return ( tuiCmdReact(cmd, rv) ) ;
}


/** Fill variable number of non-repeating results: "fillvareresnumnrep,elem_id,result"
 * @param cmd command
 * @return status
 */
int func_fill_var_eres_num_nrep(char *cmd)
{
  int rv  = AF_OK ;
  int   i  ;
  char *var = NULL ;
  long id   = -1 ;
  long eres  = -1 ;
	char value[CI_STR_LEN];

	FEM_TEST_POSTPROCESSOR

	for (i=0; i<CI_STR_LEN; i++) { value[i] = '\0' ; }

  if (ciParNum(cmd) < 4)
	{
		fprintf(msgout,"[E] %s: \"%s,%s\"!\n",
			 	_("All parameters are required:"),
				ciGetParStrNoExpand(cmd, 0),
        "variable,element,result");
		return ( tuiCmdReact(cmd, AF_ERR_EMP) ) ;
	}

  if (strlen(var = ciGetParStrNoExpand(cmd, 1)) < 1) 
	{
		free(var);
		fprintf(msgout,"[E] %s!\n", 
				_("Invalid variable"));
		return(tuiCmdReact(cmd, AF_ERR_VAL));
	}

  if ((id = ciGetParInt(cmd,2)) <1)
  {
    rv = AF_ERR_VAL ;
		fprintf(msgout,"[E] %s!\n", _("Invalid element"));
		goto memFree;
  }
  
  if ((eres = ciGetParInt(cmd, 3)) < 1) 
	{
    rv = AF_ERR_VAL ;
		fprintf(msgout,"[E] %s!\n", _("Invalid result type"));
		goto memFree;
	}
  
	sprintf(value,"%li",fdb_user_get_eres_no_rep_num(id, eres) );
	rv = ciAddVar(var, value) ;
  
memFree:
  free(var); var = NULL ;
	return ( tuiCmdReact(cmd, rv) ) ;
}

/** Fill variable number of repeating results: "fillvareresnumrep,elem_id,result"
 * @param cmd command
 * @return status
 */
int func_fill_var_eres_num_rep(char *cmd)
{
  int rv  = AF_OK ;
  int   i  ;
  char *var = NULL ;
  long id   = -1 ;
  long eres  = -1 ;
	char value[CI_STR_LEN];

	FEM_TEST_POSTPROCESSOR

	for (i=0; i<CI_STR_LEN; i++) { value[i] = '\0' ; }

  if (ciParNum(cmd) < 4)
	{
		fprintf(msgout,"[E] %s: \"%s,%s\"!\n",
			 	_("All parameters are required:"),
				ciGetParStrNoExpand(cmd, 0),
        "variable,element,result");
		return ( tuiCmdReact(cmd, AF_ERR_EMP) ) ;
	}

  if (strlen(var = ciGetParStrNoExpand(cmd, 1)) < 1) 
	{
		free(var);
		fprintf(msgout,"[E] %s!\n", 
				_("Invalid variable"));
		return(tuiCmdReact(cmd, AF_ERR_VAL));
	}

  if ((id = ciGetParInt(cmd,2)) <1)
  {
    rv = AF_ERR_VAL ;
		fprintf(msgout,"[E] %s!\n", _("Invalid element"));
		goto memFree;
  }
  
  if ((eres = ciGetParInt(cmd, 3)) < 1) 
	{
    rv = AF_ERR_VAL ;
		fprintf(msgout,"[E] %s!\n", _("Invalid result type"));
		goto memFree;
	}
  
	sprintf(value,"%li",fdb_user_get_eres_rep_num(id, eres) );
	rv = ciAddVar(var, value) ;
  
memFree:
  free(var); var = NULL ;
	return ( tuiCmdReact(cmd, rv) ) ;
}

/** Fill variable with element result value: "fillvarnres,var,node_id,dof"
 * @param cmd command
 * @return status
 */
int func_fill_var_nres(char *cmd)
{
  int rv  = AF_OK ;
  long i ;
  char *var = NULL ;
  long id   = -1 ;
  long pos = -1 ;
  long dof ;
	char value[CI_STR_LEN];

	FEM_TEST_POSTPROCESSOR

	for (i=0; i<CI_STR_LEN; i++) { value[i] = '\0' ; }
  
  if (ciParNum(cmd) < 4)
	{
		fprintf(msgout,"[E] %s: \"%s,%s\"!\n",
			 	_("All parameters are required:"),
				ciGetParStrNoExpand(cmd, 0),
        "variable,node,dof_type");
		return ( tuiCmdReact(cmd, AF_ERR_EMP) ) ;
	}

  if (strlen(var = ciGetParStrNoExpand(cmd, 1)) < 1) 
	{
		free(var);
		fprintf(msgout,"[E] %s!\n", 
				_("Invalid variable"));
		return(tuiCmdReact(cmd, AF_ERR_VAL));
	}

  if ((id = ciGetParInt(cmd,2)) <1)
  {
    rv = AF_ERR_VAL ;
		fprintf(msgout,"[E] %s!\n", _("Invalid node"));
		goto memFree;
  }

  if ((fdbInputCountInt(NODE, NODE_ID, id, &pos)) <= 0) 
  { 
    rv = AF_ERR_VAL ;
		fprintf(msgout,"[E] %s: %li!\n", _("Node not found"),id);
		goto memFree;
  }

  
  if ((dof = ciGetParInt(cmd, 3)) < 1) 
	{
    rv = AF_ERR_VAL ;
		fprintf(msgout,"[E] %s!\n", _("Invalid DOF type"));
		goto memFree;
	}

  if ((dof <=0) || (dof > KNOWN_DOFS))
  {
    rv = AF_ERR_VAL ;
		fprintf(msgout,"[E] %s!\n", _("Invalid DOF type"));
		goto memFree;
  }

	sprintf(value,"%e", fdbResNodeGetVal(pos, dof));
	rv = ciAddVar(var, value) ;
  
memFree:
  free(var); var = NULL ;
	return ( tuiCmdReact(cmd, rv) ) ;
}



/** Fill variable with element result value : "fillvareres,var,elem_id,result,number(0=non-repeating, 1..n=repeating)"
 * @param cmd command
 * @return status
 */
int func_fill_var_eres(char *cmd)
{
  int rv  = AF_OK ;
  int   i  ;
  char *var = NULL ;
  long id   = -1 ;
  long eres  = -1 ;
	long ipos = 0 ;
  long  pos = -1;
	char value[CI_STR_LEN];

	FEM_TEST_POSTPROCESSOR

	for (i=0; i<CI_STR_LEN; i++) { value[i] = '\0' ; }

  if (ciParNum(cmd) < 5)
	{
		fprintf(msgout,"[E] %s: \"%s,%s\"!\n",
			 	_("All parameters are required:"),
				ciGetParStrNoExpand(cmd, 0),
        "variable,element,result,position");
		return ( tuiCmdReact(cmd, AF_ERR_EMP) ) ;
	}

  if (strlen(var = ciGetParStrNoExpand(cmd, 1)) < 1) 
	{
		free(var);
		fprintf(msgout,"[E] %s!\n", 
				_("Invalid variable"));
		return(tuiCmdReact(cmd, AF_ERR_VAL));
	}

  if ((id = ciGetParInt(cmd,2)) <1)
  {
    rv = AF_ERR_VAL ;
		fprintf(msgout,"[E] %s!\n", _("Invalid element"));
		goto memFree;
  }

	if ((fdbInputCountInt(ELEM, ELEM_ID, id, &pos)) < 0) 
  { 
    rv = AF_ERR_VAL ;
		fprintf(msgout,"[E] %s: %li!\n", _("Element not found"),id);
		goto memFree;
  }
  
  if ((eres = ciGetParInt(cmd, 3)) < 1) 
	{
    rv = AF_ERR_VAL ;
		fprintf(msgout,"[E] %s!\n", _("Invalid result type"));
		goto memFree;
	}

 
  if ((ipos = ciGetParInt(cmd, 4)) < 1) 
	{
    rv = AF_ERR_VAL ;
		fprintf(msgout,"[E] %s!\n", _("Invalid result type"));
		goto memFree;
	}
  
	sprintf(value,"%e", (double)fdb_user_get_eres(id, eres, ipos, 0));
	rv = ciAddVar(var, value) ;
  
memFree:
  free(var); var = NULL ;
	return ( tuiCmdReact(cmd, rv) ) ;
}

/** Fill variable with maximum element result value : "fillvareresmax,elem_id,result,number"
 * @param cmd command
 * @return status
 */
int func_fill_var_eres_max(char *cmd)
{
  int rv  = AF_OK ;
  int   i  ;
  char *var = NULL ;
  long id   = -1 ;
  long eres  = -1 ;
	char value[CI_STR_LEN];

	FEM_TEST_POSTPROCESSOR

	for (i=0; i<CI_STR_LEN; i++) { value[i] = '\0' ; }

  if (ciParNum(cmd) < 4)
	{
		fprintf(msgout,"[E] %s: \"%s,%s\"!\n",
			 	_("All parameters are required:"),
				ciGetParStrNoExpand(cmd, 0),
        "variable,element,result");
		return ( tuiCmdReact(cmd, AF_ERR_EMP) ) ;
	}

  if (strlen(var = ciGetParStrNoExpand(cmd, 1)) < 1) 
	{
		free(var);
		fprintf(msgout,"[E] %s!\n", 
				_("Invalid variable"));
		return(tuiCmdReact(cmd, AF_ERR_VAL));
	}

  if ((id = ciGetParInt(cmd,2)) <1)
  {
    rv = AF_ERR_VAL ;
		fprintf(msgout,"[E] %s!\n", _("Invalid element"));
		goto memFree;
  }
  
  if ((eres = ciGetParInt(cmd, 3)) < 1) 
	{
    rv = AF_ERR_VAL ;
		fprintf(msgout,"[E] %s!\n", _("Invalid result type"));
		goto memFree;
	}
  
	sprintf(value,"%li", (long)fdb_user_get_eres(id, eres, 0, 1));
	rv = ciAddVar(var, value) ;
  
memFree:
  free(var); var = NULL ;
	return ( tuiCmdReact(cmd, rv) ) ;
}

/** Fill variable with minimum element result value : "fillvareresmin,elem_id,result"
 * @param cmd command
 * @return status
 */
int func_fill_var_eres_min(char *cmd)
{
  int rv  = AF_OK ;
  int   i  ;
  char *var = NULL ;
  long id   = -1 ;
  long eres  = -1 ;
	char value[CI_STR_LEN];

	FEM_TEST_POSTPROCESSOR

	for (i=0; i<CI_STR_LEN; i++) { value[i] = '\0' ; }

  if (ciParNum(cmd) < 4)
	{
		fprintf(msgout,"[E] %s: \"%s,%s\"!\n",
			 	_("All parameters are required:"),
				ciGetParStrNoExpand(cmd, 0),
        "variable,element,result");
		return ( tuiCmdReact(cmd, AF_ERR_EMP) ) ;
	}

  if (strlen(var = ciGetParStrNoExpand(cmd, 1)) < 1) 
	{
		free(var);
		fprintf(msgout,"[E] %s!\n", 
				_("Invalid variable"));
		return(tuiCmdReact(cmd, AF_ERR_VAL));
	}

  if ((id = ciGetParInt(cmd,2)) <1)
  {
    rv = AF_ERR_VAL ;
		fprintf(msgout,"[E] %s!\n", _("Invalid element"));
		goto memFree;
  }
  
  if ((eres = ciGetParInt(cmd, 3)) < 1) 
	{
    rv = AF_ERR_VAL ;
		fprintf(msgout,"[E] %s!\n", _("Invalid result type"));
		goto memFree;
	}
 
	sprintf(value,"%li", (long)fdb_user_get_eres(id, eres, 0, 2));
	rv = ciAddVar(var, value) ;
  
memFree:
  free(var); var = NULL ;
	return ( tuiCmdReact(cmd, rv) ) ;
}


/** Fill variable with average element result value : "fillvareresaver,elem_id,result"
 * @param cmd command
 * @return status
 */
int func_fill_var_eres_aver(char *cmd)
{
  int rv  = AF_OK ;
  int   i  ;
  char *var = NULL ;
  long id   = -1 ;
  long eres  = -1 ;
	char value[CI_STR_LEN];

	FEM_TEST_POSTPROCESSOR

	for (i=0; i<CI_STR_LEN; i++) { value[i] = '\0' ; }

  if (ciParNum(cmd) < 4)
	{
		fprintf(msgout,"[E] %s: \"%s,%s\"!\n",
			 	_("All parameters are required:"),
				ciGetParStrNoExpand(cmd, 0),
        "variable,element,result");
		return ( tuiCmdReact(cmd, AF_ERR_EMP) ) ;
	}

  if (strlen(var = ciGetParStrNoExpand(cmd, 1)) < 1) 
	{
		free(var);
		fprintf(msgout,"[E] %s!\n", 
				_("Invalid variable"));
		return(tuiCmdReact(cmd, AF_ERR_VAL));
	}

  if ((id = ciGetParInt(cmd,2)) <1)
  {
    rv = AF_ERR_VAL ;
		fprintf(msgout,"[E] %s!\n", _("Invalid element"));
		goto memFree;
  }
  
  if ((eres = ciGetParInt(cmd, 3)) < 1) 
	{
    rv = AF_ERR_VAL ;
		fprintf(msgout,"[E] %s!\n", _("Invalid result type"));
		goto memFree;
	}
 
	sprintf(value,"%e", fdb_user_get_eres(id, eres, 0, 3));
	rv = ciAddVar(var, value) ;
  
memFree:
  free(var); var = NULL ;
	return ( tuiCmdReact(cmd, rv) ) ;
}

/** Fill variable with maximal element result value : "fillvareresselmax,result"
 * @param cmd command
 * @return status
 */
int func_fill_var_eres_max_from_all(char *cmd)
{
  int rv  = AF_OK ;
  int   i  ;
  char *var = NULL ;
  long eres  = -1 ;
	char value[CI_STR_LEN];
  double max, min ;

	FEM_TEST_POSTPROCESSOR

	for (i=0; i<CI_STR_LEN; i++) { value[i] = '\0' ; }

  if (ciParNum(cmd) < 3)
	{
		fprintf(msgout,"[E] %s: \"%s,%s\"!\n",
			 	_("All parameters are required:"),
				ciGetParStrNoExpand(cmd, 0),
        "variable,result");
		return ( tuiCmdReact(cmd, AF_ERR_EMP) ) ;
	}

  if (strlen(var = ciGetParStrNoExpand(cmd, 1)) < 1) 
	{
		free(var);
		fprintf(msgout,"[E] %s!\n", 
				_("Invalid variable"));
		return(tuiCmdReact(cmd, AF_ERR_VAL));
	}

  if ((eres = ciGetParInt(cmd, 2)) < 1) 
	{
    rv = AF_ERR_VAL ;
		fprintf(msgout,"[E] %s!\n", _("Invalid result type"));
		goto memFree;
	}
  
  if ((rv=fdbResMaxMinElem(eres, &max, &min)) != AF_OK) {goto memFree;}

	sprintf(value,"%e", max);
	rv = ciAddVar(var, value) ;
  
memFree:
  free(var); var = NULL ;
	return ( tuiCmdReact(cmd, rv) ) ;
}

/** Fill variable with maximal element result value : "fillvareresselmin,result"
 * @param cmd command
 * @return status
 */
int func_fill_var_eres_min_from_all(char *cmd)
{
  int rv  = AF_OK ;
  int   i  ;
  char *var = NULL ;
  long eres  = -1 ;
	char value[CI_STR_LEN];
  double max, min ;

	FEM_TEST_POSTPROCESSOR

	for (i=0; i<CI_STR_LEN; i++) { value[i] = '\0' ; }

  if (ciParNum(cmd) < 3)
	{
		fprintf(msgout,"[E] %s: \"%s,%s\"!\n",
			 	_("All parameters are required:"),
				ciGetParStrNoExpand(cmd, 0),
        "variable,result");
		return ( tuiCmdReact(cmd, AF_ERR_EMP) ) ;
	}

  if (strlen(var = ciGetParStrNoExpand(cmd, 1)) < 1) 
	{
		free(var);
		fprintf(msgout,"[E] %s!\n", 
				_("Invalid variable"));
		return(tuiCmdReact(cmd, AF_ERR_VAL));
	}

  if ((eres = ciGetParInt(cmd, 2)) < 1) 
	{
    rv = AF_ERR_VAL ;
		fprintf(msgout,"[E] %s!\n", _("Invalid result type"));
		goto memFree;
	}
  
  if ((rv=fdbResMaxMinElem(eres, &max, &min)) != AF_OK) {goto memFree;}

	sprintf(value,"%e", min);
	rv = ciAddVar(var, value) ;
  
memFree:
  free(var); var = NULL ;
	return ( tuiCmdReact(cmd, rv) ) ;
}


/* ****************************************************
 *
 * PATH OPERATIONS
 *
 */

/** Creates new (empty) path: "path [,number, name]"
 * @param cmd command
 * @return status
 */
int func_path_create(char *cmd)
{
  int rv = AF_OK ;
  int num  = -1 ;
  int i ;

	FEM_TEST_POSTPROCESSOR

  if (ciParNum(cmd) > 1)
  {
    num = ciGetParInt(cmd,1) ;
    if ((num < 1)|| (num > PATH_NUM))
    {
		  fprintf(msgout,"[w] %s!\n", _("Invalid path number"));
      num = -1 ;
    }
    else
    {
      /* check if exist - if yes then delete old path */
      if (femPath[num-1].len > 0)
      {
        resPathFree(num-1);
      }
    }
  }
	else
	{
		fprintf(msgout,"[E] %s!\n", _("Incomplete command - must be: path,[number],name"));
	      return ( tuiCmdReact(cmd, AF_ERR_VAL) ) ;
	}

  if (num == -1)  /* new path number */
  {
    for (i=0; i<PATH_NUM; i++)
    {
      if (femPath[i].len <= 0)
      {
        num = i+1 ;
        break ;
      }
    }

    if( num == -1)
    {
		  fprintf(msgout,"[E] %s!\n", _("Path space is full - new path is not possible"));
	    return ( tuiCmdReact(cmd, AF_ERR_MEM) ) ;
    }
  }

  if (resPathAlloc(num-1) != AF_OK)
  {
		fprintf(msgout,"[E] %s!\n", _("No memory for path data"));
	  return ( tuiCmdReact(cmd, AF_ERR_MEM) ) ;
  }

  /* test path name: */
  if (ciParNum(cmd) > 2)
  {
    if (strlen(ciGetParStr(cmd,2)) > 0)
    {
      strncpy(femPath[num-1].desc,ciGetParStr(cmd,2), 255);
    }
    else
    {
      sprintf(femPath[num-1].desc, "PATH%i",num) ;
    }
  }
  else { sprintf(femPath[num-1].desc, "PATH%i",num) ; }

  femActivePath = num - 1 ;

  fprintf(msgout,"[ ] %s: %i, %s\n", _("New path created"),
      num, femPath[num-1].desc);

	return ( tuiCmdReact(cmd, rv) ) ;
}

/** Deletes a path: "pathdel,number"
 * @param cmd command
 * @return status
 */
int func_path_delete(char *cmd)
{
  int num  = -1 ;
  int i ;

	FEM_TEST_POSTPROCESSOR

  if (ciParNum(cmd) < 2)
  {
		fprintf(msgout,"[E] %s!\n", _("Path number have to be specified: pathdel,number"));
	  return ( tuiCmdReact(cmd, AF_ERR_VAL) ) ;
  }

  if (ciTestStringALL(cmd,1) == AF_YES)
  {
    for (i=0; i<PATH_NUM; i++)
    {
      resPathFree(i);
      fprintf(msgout,"[i] %s: %i\n", _("Path deleted:"),i+1);
    }
    femActivePath = -1 ;
	  return ( tuiCmdReact(cmd, AF_OK) ) ;
  }

  num = ciGetParInt(cmd,1) ;
  if ((num < 1)&& (num > PATH_NUM))
  {
	  fprintf(msgout,"[w] %s!\n", _("Invalid path number"));
	  return ( tuiCmdReact(cmd, AF_ERR_VAL) ) ;
  }

  resPathFree(num-1);
  femActivePath = num-2;
  fprintf(msgout,"[i] %s: %i\n", _("Path deleted:"),num-1);

	return ( tuiCmdReact(cmd, AF_OK) ) ;
}


/** Adds node to path: "pn,node"
 * @param cmd command
 * @return status
 */
int func_path_node_new(char *cmd)
{
  int num  = -1 ;

	FEM_TEST_POSTPROCESSOR

  if (femActivePath < 0)
  {
		fprintf(msgout,"[E] %s!\n", _("Path must be defined first"));
	  return ( tuiCmdReact(cmd, AF_ERR_VAL) ) ;
  }

  if (ciParNum(cmd) < 2)
  {
		fprintf(msgout,"[E] %s!\n", _("Node number have to be specified: pn,node"));
	  return ( tuiCmdReact(cmd, AF_ERR_VAL) ) ;
  }
  num = ciGetParInt(cmd,1) ;

  if ((num <1) || (num > fdbInputFindMaxInt(NODE, NODE_ID)))
  {
		fprintf(msgout,"[E] %s!\n", _("Invalid node"));
	  return ( tuiCmdReact(cmd, AF_ERR_VAL) ) ;
  }
  else
  {
    return(tuiCmdReact(cmd,resPathAddNode(femActivePath, num)));
  }

	return ( tuiCmdReact(cmd, AF_OK) ) ;
}

/** List paths: "pathlist,from,to"
 * @param cmd command
 * @return status
 */
int func_path_list(char *cmd)
{
  int from, to ;

	FEM_TEST_POSTPROCESSOR

  from = 1 ;
  to   = PATH_NUM ;

  if (ciParNum(cmd) < 2)
  {
    return ( tuiCmdReact(cmd, femPathList(fdbPrnFile, 0, PATH_NUM)));
  }
  else
  {
    if (ciParNum(cmd) > 1) 
    { 
      from = ciGetParInt(cmd,1) ;
      if (from > PATH_NUM) {from = PATH_NUM;}
      if (from < 1) {from = 1;}
    }
    if (ciParNum(cmd) > 2) 
    { 
      to = ciGetParInt(cmd,1) ;
      if (to > PATH_NUM) {to = PATH_NUM;}
      if (to < 1) {to = 1;}
      if (to < from) {to = from;}
    }
  }

  return ( tuiCmdReact(cmd, femPathList(fdbPrnFile, from-1, to-1)));
}

/** List paths: "prpath,res1[,res2,res3,res4,res5,res6]"
 * @param cmd command
 * @return status
 */
int func_path_print_res(char *cmd)
{
  long res_type[6] ;
  long res_len = 0 ;
  int i ;

	FEM_TEST_POSTPROCESSOR


  if (ciParNum(cmd) < 2)
  {
		 fprintf(msgout,"[E] %s!\n", _("At least one result type must be given: prpath,res_type"));
	   return ( tuiCmdReact(cmd, AF_ERR_VAL) ) ;
  }
  
  for (i=0; i<6; i++)
  {
    if (ciParNum(cmd) > i+1) 
    {
      res_type[i] = ciGetParInt(cmd,i+1); 
      res_len++;
    }
  }

  return ( tuiCmdReact(cmd, femPathResPrn(fdbPrnFile, femActivePath, res_type, res_len )));
}

/* end of cmd_res.c */
