
/*
**  This file forms part of the GOBLIN C++ Class Library.
**
**  Written by Christian Fremuth-Paeger, September 2003.
**  
**  Copying, compiling, distribution and modification
**  of this source code is permitted only in accordance
**  with the GOBLIN general licence information.
*/             


#include "gosh.h"
#include "glpk.h"
#include "glpkWrapper.h"


extern "C" int Glpk_print_hook(void *info, char *msg)
{
    CT -> LogEntry(LOG_METH2,NoHandle,msg);

    return 1;
}


extern "C" int Glpk_Init(Tcl_Interp *interp)
{
    CT -> LogEntry(LOG_SHELL,NoHandle,"Starting GLPK 4.0 plugin...");

    if (Tcl_PkgRequire(interp,"goblin","2.6",0)==NULL)
    {
        sprintf(interp->result,"GOBLIN must be loaded before the GLPK plugin");
        return TCL_ERROR;
    }

    CT -> LogEntry(LOG_SHELL,NoHandle,"");

    CT -> ReleaseLPModule();

    mipFactory* tmpPtr = new glpkFactory();
    goblinController::pMipFactory = reinterpret_cast<char*>(tmpPtr);

    char version[10];
    sprintf(version,"%d.%d",4,0);
    Tcl_PkgProvide(interp,"glpk",version);

    LIBENV* env = lib_env_ptr();
    env->print_hook = &Glpk_print_hook;

    return TCL_OK;
}
