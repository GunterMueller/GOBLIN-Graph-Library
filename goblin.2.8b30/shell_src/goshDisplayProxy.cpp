
//  This file forms part of the GOBLIN C++ Class Library.
//
//  Initially written by Christian Fremuth-Paeger, March 2009
//
//  Copying, compiling, distribution and modification
//  of this source code is permitted only in accordance
//  with the GOBLIN general licence information.

/// \file   goshDisplayProxy.cpp
/// \brief  Tcl shell command procedures for visualization of library objects

#include "gosh.h"
#include "graphDisplayProxy.h"


int Goblin_Graph_Display_Proxy_Cmd (ClientData clientData,Tcl_Interp* interp,
    int argc,_CONST_QUAL_ char* argv[])
{
    graphDisplayProxy* DP = reinterpret_cast<graphDisplayProxy*>(clientData);

    if (setjmp(goblinThreadData[Goblin_MyThreadIndex()].jumpBuffer) != 0)
    {
        return Goblin_Propagate_Exception(interp);
    }
    else if (argc==1)
    {
        interp->result = "Missing object command";
        return TCL_ERROR;
    }
    else try
    {
        if (strcmp(argv[1],"synchronize")==0)
        {
            DP -> Synchronize();
            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"info")==0)
        {
            if (strcmp(argv[2],"-canvasWidth")==0)
            {
                sprintf(interp->result,"%ld",DP->CanvasWidth());
                return TCL_OK;
            }

            if (strcmp(argv[2],"-canvasHeight")==0)
            {
                sprintf(interp->result,"%ld",DP->CanvasHeight());
                return TCL_OK;
            }

            if (strcmp(argv[2],"-canvasArrowSize")==0)
            {
                sprintf(interp->result,"%g",DP->CanvasArrowSize());
                return TCL_OK;
            }

            sprintf(interp->result,"Unknown option: %s",argv[2]);
            return TCL_ERROR;
        }

        if (strcmp(argv[1],"node")==0)
        {
            if (argc==2)
            {
                interp->result = "Missing node index";
                return TCL_ERROR;
            }

            TNode v = TNode(atol(argv[2]));

            if (argc<4)
            {
                interp->result = "Missing command option";
                return TCL_ERROR;
            }

            if (strcmp(argv[3],"info")==0)
            {
                if (strcmp(argv[4],"-mapped")==0)
                {
                    sprintf(interp->result,"%s",DP->IsNodeMapped(v) ? "1" : "0");
                    return TCL_OK;
                }

                if (strcmp(argv[4],"-canvasWidth")==0)
                {
                    sprintf(interp->result,"%ld",DP->CanvasNodeWidth(v));
                    return TCL_OK;
                }

                if (strcmp(argv[4],"-canvasHeight")==0)
                {
                    sprintf(interp->result,"%ld",DP->CanvasNodeHeight(v));
                    return TCL_OK;
                }

                if (strcmp(argv[4],"-label")==0)
                {
                    DP -> CompoundNodeLabel(interp->result,256,v);
                    return TCL_OK;
                }

                if (strcmp(argv[4],"-colour")==0)
                {
                    DP -> CanvasNodeColour(interp->result,v);
                    return TCL_OK;
                }

                sprintf(interp->result,"Unknown option: %s",argv[4]);
                return TCL_ERROR;
            }

            sprintf(interp->result,"Unknown option: %s",argv[3]);
            return TCL_ERROR;
        }

        if (strcmp(argv[1],"arc")==0)
        {
            if (argc==2)
            {
                interp->result = "Missing arc index";
                return TCL_ERROR;
            }

            TArc a = TNode(atol(argv[2]));

            if (argc<4)
            {
                interp->result = "Missing command option";
                return TCL_ERROR;
            }

            if (strcmp(argv[3],"info")==0)
            {
                if (strcmp(argv[4],"-mapped")==0)
                {
                    sprintf(interp->result,"%s",DP->IsArcMapped(a) ? "1" : "0");
                    return TCL_OK;
                }

                if (strcmp(argv[4],"-canvasPortX")==0)
                {
                    sprintf(interp->result,"%ld",DP->CanvasCXOfPort(a));
                    return TCL_OK;
                }

                if (strcmp(argv[4],"-canvasPortY")==0)
                {
                    sprintf(interp->result,"%ld",DP->CanvasCYOfPort(a));
                    return TCL_OK;
                }

                if (strcmp(argv[4],"-label")==0)
                {
                    DP -> CompoundArcLabel(interp->result,256,a);
                    return TCL_OK;
                }

                if (strcmp(argv[4],"-colour")==0)
                {
                    DP -> CanvasArcColour(interp->result,a);
                    return TCL_OK;
                }

                if (strcmp(argv[4],"-width")==0)
                {
                    sprintf(interp->result,"%ld",DP->CanvasArcWidth(a));
                    return TCL_OK;
                }

                if (strcmp(argv[4],"-dash")==0)
                {
                    const char* dashMode[] = {""," -dash ."," -dash -"," -dash -."};
                    sprintf(interp->result,"%s",dashMode[DP->CanvasArcDashMode(a)]);
                    return TCL_OK;
                }

                if (strcmp(argv[4],"-arrows")==0)
                {
                    switch (DP->ArrowDirections(a))
                    {
                        case ARROW_BOTH:
                        {
                            interp->result = "both";
                            return TCL_OK;
                        }
                        case ARROW_FORWARD:
                        {
                            interp->result = "last";
                            return TCL_OK;
                        }
                        case ARROW_BACKWARD:
                        {
                            interp->result = "first";
                            return TCL_OK;
                        }
                        case ARROW_NONE:
                        {
                            interp->result = "none";
                            return TCL_OK;
                        }
                    }

                    sprintf(interp->result,"Unknown arrow display mode: %d",DP->ArrowDirections(a));
                    return TCL_OK;
                }

                sprintf(interp->result,"Unknown option: %s",argv[4]);
                return TCL_ERROR;
            }

            sprintf(interp->result,"Unknown option: %s",argv[3]);
            return TCL_ERROR;
        }

        if (strcmp(argv[1],"layoutPoint")==0)
        {
            if (argc==2)
            {
                interp->result = "Missing layout point index";
                return TCL_ERROR;
            }

            TNode p = TNode(atol(argv[2]));

            if (argc<4)
            {
                interp->result = "Missing command option";
                return TCL_ERROR;
            }

            if (strcmp(argv[3],"info")==0)
            {
                if (strcmp(argv[4],"-canvasX")==0)
                {
                    sprintf(interp->result,"%ld",DP->CanvasCXOfPoint(p));
                    return TCL_OK;
                }

                if (strcmp(argv[4],"-canvasY")==0)
                {
                    sprintf(interp->result,"%ld",DP->CanvasCYOfPoint(p));
                    return TCL_OK;
                }

                sprintf(interp->result,"Unknown option: %s",argv[4]);
                return TCL_ERROR;
            }

            if (strcmp(argv[3],"placeAt")==0)
            {
                if (argc<6)
                {
                    interp->result = "Missing coordinate values";
                    return TCL_ERROR;
                }

                DP -> PlaceLayoutPoint(p,atol(argv[4]),atol(argv[5]));
                interp->result = "";
                return TCL_OK;
            }

            sprintf(interp->result,"Unknown option: %s",argv[3]);
            return TCL_ERROR;
        }

        if (strcmp(argv[1],"labelAnchorPoint")==0)
        {
            if (argc==2)
            {
                interp->result = "Missing arc index";
                return TCL_ERROR;
            }

            TArc a = TArc(atol(argv[2]));

            if (argc<4)
            {
                interp->result = "Missing command option";
                return TCL_ERROR;
            }

            if (strcmp(argv[3],"info")==0)
            {
                if (strcmp(argv[4],"-canvasX")==0)
                {
                    sprintf(interp->result,"%ld",DP->CanvasCXOfArcLabelAnchor(a));
                    return TCL_OK;
                }

                if (strcmp(argv[4],"-canvasY")==0)
                {
                    sprintf(interp->result,"%ld",DP->CanvasCYOfArcLabelAnchor(a));
                    return TCL_OK;
                }

                sprintf(interp->result,"Unknown option: %s",argv[4]);
                return TCL_ERROR;
            }

            if (strcmp(argv[3],"placeAt")==0)
            {
                if (argc<6)
                {
                    interp->result = "Missing coordinate values";
                    return TCL_ERROR;
                }

                DP -> PlaceArcLabelAnchor(a,atol(argv[4]),atol(argv[5]));
                interp->result = "";
                return TCL_OK;
            }

            sprintf(interp->result,"Unknown option: %s",argv[3]);
            return TCL_ERROR;
        }

        sprintf(interp->result,"Unknown option: %s",argv[1]);
        return TCL_ERROR;
    }
    catch (...)
    {
        return Goblin_Propagate_Exception(interp);
    }

    return TCL_OK;
}


int Goblin_Delete_Graph_Display_Proxy (ClientData clientData) throw()
{
    graphDisplayProxy* DP = reinterpret_cast<graphDisplayProxy*>(clientData);
    delete DP;

    return TCL_OK;
}
