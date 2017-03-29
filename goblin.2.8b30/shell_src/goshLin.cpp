
//  This file forms part of the GOBLIN C++ Class Library.
//
//  Initially written by Christian Fremuth-Paeger, December 2002
//
//  Copying, compiling, distribution and modification
//  of this source code is permitted only in accordance
//  with the GOBLIN general licence information.

/// \file   goshLin.cpp
/// \brief  Tcl shell command procedures for LP and MIP objects

#include "gosh.h"


int Goblin_Ilp_Cmd (ClientData clientData,Tcl_Interp* interp,
    int argc,_CONST_QUAL_ char* argv[])
{
    mipInstance* XLP = reinterpret_cast<mipInstance*>(clientData);

    if (setjmp(goblinThreadData[Goblin_MyThreadIndex()].jumpBuffer) != 0)
    {
        return Goblin_Propagate_Exception(interp);
    }
    else try
    {
        if (argc==2 && strcmp(argv[1],"delete")==0)
        {
            Tcl_DeleteCommand(interp,argv[0]);
            interp->result = "";
            return TCL_OK;
        }

        if (argc==2 && strcmp(argv[1],"reset")==0)
        {
            XLP -> ResetBasis();
            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"write")==0)
        {
            if (argc>4)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            mipInstance::TLPFormat f = mipInstance::GOB_FORMAT;

            if (argc>3)
            {
                if (strcmp(argv[2],"lp")==0) f = mipInstance::LP_FORMAT;
                if (strcmp(argv[2],"mps")==0) f = mipInstance::MPS_FORMAT;
                if (strcmp(argv[2],"cplex")==0) f = mipInstance::MPS_CPLEX;
                if (strcmp(argv[2],"bas")==0) f = mipInstance::BAS_CPLEX;
                if (strcmp(argv[2],"basis")==0) f = mipInstance::BAS_CPLEX;
            }

            XLP -> Write(argv[argc-1],f);

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"read")==0)
        {
            if (argc!=4)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            if (strcmp(argv[2],"bas")==0 || strcmp(argv[2],"basis")==0)
            {
                XLP -> ReadBASFile(argv[3]);
                interp->result = "";
                return TCL_OK;
            }

            if (strcmp(argv[2],"mps")==0 || strcmp(argv[2],"problem")==0)
            {
                if (XLP->K()>0 || XLP->L()>0)
                {
                    interp->result = "Instance must be initial";
                    return TCL_ERROR;
                }

                XLP -> ReadMPSFile(argv[3]);
                interp->result = "";
                return TCL_OK;
            }

            sprintf(interp->result,"Unknown option: %s read %s",
                argv[0],argv[2]);
            return TCL_ERROR;
        }

        if (strcmp(argv[1],"#rows")==0 || strcmp(argv[1],"#restrictions")==0)
        {
            if (argc!=2)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            sprintf(interp->result,"%lu",static_cast<unsigned long>(XLP->K()));
            return TCL_OK;
        }

        if (strcmp(argv[1],"#columns")==0 || strcmp(argv[1],"#variables")==0)
        {
            if (argc!=2)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            sprintf(interp->result,"%lu",static_cast<unsigned long>(XLP->L()));
            return TCL_OK;
        }

        if (strcmp(argv[1],"info")==0)
        {
            if (argc!=3)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            if (strcmp(argv[2],"-mipObject")==0)
            {
                interp->result = "1";
                return TCL_OK;
            }

            // Do not complain, message 'info' is interpreted again
            return Goblin_Generic_Cmd(XLP,interp,argc,argv);
        }

        if (strcmp(argv[1],"orientation")==0)
        {
            if (argc!=2)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            mipFactory *theMipFactory = (mipFactory*)CT->pMipFactory;

            if (theMipFactory->Orientation()==mipFactory::ROW_ORIENTED)
                interp->result = "row";
            else interp->result = "column";

            return TCL_OK;
        }

        if (strcmp(argv[1],"direction")==0)
        {
            if (argc!=2)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            if (XLP->ObjectSense()==managedObject::MAXIMIZE)
                interp->result = "maximize";
            else if (XLP->ObjectSense()==managedObject::MINIMIZE)
                interp->result = "minimize";
            else interp->result = "flat";

            return TCL_OK;
        }

        if (strcmp(argv[1],"maximize")==0)
        {
            if (argc!=2)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            XLP -> SetObjectSense(managedObject::MAXIMIZE);
            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"minimize")==0)
        {
            if (argc!=2)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            XLP -> SetObjectSense(managedObject::MINIMIZE);
            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"invert")==0)
        {
            if (argc!=2)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            XLP -> FlipObjectSense();
            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"nullify")==0)
        {
            if (argc!=2)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            XLP -> SetObjectSense(managedObject::NO_OBJECTIVE);
            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"strip")==0)
        {
            if (argc!=2)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            XLP -> Strip();
            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"resize")==0)
        {
            if (argc!=5)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            TIndex k = TIndex(atol(argv[2]));
            TIndex l = TIndex(atol(argv[3]));
            TIndex nz = TIndex(atol(argv[4]));
            XLP -> Resize(k,l,nz);
            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"coeff")==0 || strcmp(argv[1],"coefficient")==0)
        {
            if (argc!=4)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            TIndex i = TIndex(atol(argv[2]));
            TIndex j = TIndex(atol(argv[3]));
            sprintf(interp->result,"%g", XLP -> Coeff(i,j));
            return TCL_OK;
        }

        if (strcmp(argv[1],"tableau")==0)
        {
            if (argc<3)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            if (strcmp(argv[2],"coeff")==0 || strcmp(argv[2],"coefficient")==0)
            {
                if (argc!=5)
                {
                    WrongNumberOfArguments(interp,argc,argv);
                    return TCL_ERROR;
                }

                TIndex j = TIndex(atol(argv[3]));
                TIndex i = TIndex(atol(argv[4]));
                sprintf(interp->result,"%g", XLP->Tableau(j,i));
                return TCL_OK;
            }

            sprintf(interp->result,"Unknown option: %s tableau %s",argv[0],argv[2]);
            return TCL_ERROR;
        }

        if (strcmp(argv[1],"inverse")==0)
        {
            if (argc<3)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            if (strcmp(argv[2],"coeff")==0 || strcmp(argv[2],"coefficient")==0)
            {
                if (argc!=5)
                {
                    WrongNumberOfArguments(interp,argc,argv);
                    return TCL_ERROR;
                }

                TIndex i = TIndex(atol(argv[3]));
                TIndex j = TIndex(atol(argv[4]));
                sprintf(interp->result,"%g", XLP->BaseInverse(i,j));
                return TCL_OK;
            }

            sprintf(interp->result,"Unknown option: %s inverse %s",argv[0],argv[2]);
            return TCL_ERROR;
        }

        if (strcmp(argv[1],"feasible")==0)
        {
            if (argc!=3)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            if (strcmp(argv[2],"primal")==0)
            {
                sprintf(interp->result,"%d",XLP->PrimalFeasible());
                return TCL_OK;
            }

            if (strcmp(argv[2],"dual")==0)
            {
                sprintf(interp->result,"%d",XLP->DualFeasible());
                return TCL_OK;
            }

            sprintf(interp->result,"Unknown option: %s feasible %s",argv[0],argv[2]);
            return TCL_ERROR;
        }

        if (strcmp(argv[1],"set")==0)
        {
            if (strcmp(argv[2],"coeff")==0 || strcmp(argv[2],"coefficient")==0)
            {
                if (argc!=6)
                {
                    WrongNumberOfArguments(interp,argc,argv);
                    return TCL_ERROR;
                }

                TIndex i = TIndex(atol(argv[3]));
                TIndex j = TIndex(atol(argv[4]));
                TFloat a = TFloat(atof(argv[5]));
                XLP -> SetCoeff(i,j,a);
                interp->result = "";
                return TCL_OK;
            }

            if (strcmp(argv[2],"index")==0)
            {
                if (argc!=6)
                {
                    WrongNumberOfArguments(interp,argc,argv);
                    return TCL_ERROR;
                }

                TIndex i = TIndex(atol(argv[3]));
                TIndex j = TIndex(atol(argv[4]));

                if (strcmp(argv[5],"upper")==0)
                    XLP -> SetIndex(i,j,mipInstance::UPPER);
                else XLP -> SetIndex(i,j,mipInstance::LOWER);

                interp->result = "";
                return TCL_OK;
            }
        }

        if (strcmp(argv[1],"pivot")==0)
        {
            if (strcmp(argv[2],"row")==0 || strcmp(argv[2],"restriction")==0)
            {
                TRestr i = XLP-> PivotRow();

                if (i==NoRestr) interp->result = "*";
                else sprintf(interp->result,"%ld",i);

                return TCL_OK;
            }

            if (strcmp(argv[2],"direction")==0)
            {
                TRestr i = XLP-> PivotRow();
                mipInstance::TLowerUpper tp = XLP-> PivotDirection();

                if (i==NoRestr) interp->result = "";
                else
                {
                    if (tp==mipInstance::LOWER)
                        interp->result = "lower";
                    else interp->result = "upper";
                }

                return TCL_OK;
            }

            if (strcmp(argv[2],"column")==0 || strcmp(argv[2],"variable")==0)
            {
                TVar i =XLP-> PivotColumn();

                if (i==NoVar) interp->result = "*";
                else sprintf(interp->result,"%ld",i);

                return TCL_OK;
            }

            if (argc!=5)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            TIndex i = TIndex(atol(argv[2]));
            TIndex j = TIndex(atol(argv[3]));

            mipInstance::TLowerUpper tp = mipInstance::UPPER;

            if (strcmp(argv[4],"lower")==0) tp = mipInstance::LOWER;

            XLP -> Pivot(i,j,tp);
            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"objective")==0)
        {
            if (argc==2 || strcmp(argv[2],"primal")==0)
            {
                sprintf(interp->result,"%g", XLP->ObjVal());
                return TCL_OK;
            }

            if (argc!=3)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            if (strcmp(argv[2],"dual")==0)
            {
                interp->result = "Not implemented yet";
                return TCL_OK;
            }

            sprintf(interp->result,"Unknown option: %s objective %s",argv[0],argv[2]);
            return TCL_ERROR;
        }

        if (strcmp(argv[1],"solve")==0)
        {
            if (argc!=3)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            if (strcmp(argv[2],"lp")==0)
            {
                XLP -> SolveLP();
                interp->result = "";
                return TCL_OK;
            }

            if (strcmp(argv[2],"primal")==0)
            {
                XLP -> SolvePrimal();
                interp->result = "";
                return TCL_OK;
            }

            if (strcmp(argv[2],"dual")==0)
            {
                XLP -> SolveDual();
                interp->result = "";
                return TCL_OK;
            }

            if (strcmp(argv[2],"mixed")==0 || strcmp(argv[2],"mip")==0)
            {
                XLP -> SolveMIP();
                interp->result = "";
                return TCL_OK;
            }

            sprintf(interp->result,"Unknown option: %s solve %s",argv[0],argv[2]);
            return TCL_ERROR;
        }

        if (strcmp(argv[1],"start")==0)
        {
            if (argc!=3)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            if (strcmp(argv[2],"primal")==0)
            {
                XLP -> StartPrimal();
                interp->result = "";
                return TCL_OK;
            }

            if (strcmp(argv[2],"dual")==0)
            {
                XLP -> StartDual();
                interp->result = "";
                return TCL_OK;
            }

            sprintf(interp->result,"Unknown option: %s start %s",argv[0],argv[2]);
            return TCL_ERROR;
        }

        if (strcmp(argv[1],"dual")==0)
        {
            if (argc!=3)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            mipInstance* YLP = XLP -> DualForm();

            Tcl_CreateCommand(interp,const_cast<char*>(argv[2]),
                Goblin_Ilp_Cmd,reinterpret_cast<ClientData>(YLP),
                (Tcl_CmdDeleteProc *)Goblin_Delete_Ilp);

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"standard")==0)
        {
            if (argc!=3)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            mipInstance* YLP = XLP -> StandardForm();

            Tcl_CreateCommand(interp,const_cast<char*>(argv[2]),
                Goblin_Ilp_Cmd,reinterpret_cast<ClientData>(YLP),
                (Tcl_CmdDeleteProc *)Goblin_Delete_Ilp);

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"canonical")==0)
        {
            if (argc!=3)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            mipInstance* YLP = XLP -> CanonicalForm();

            Tcl_CreateCommand(interp,const_cast<char*>(argv[2]),
                Goblin_Ilp_Cmd,reinterpret_cast<ClientData>(YLP),
                (Tcl_CmdDeleteProc *)Goblin_Delete_Ilp);

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"row")==0 || strcmp(argv[1],"restriction")==0)
            return Goblin_Row_Cmd(XLP,interp,argc,argv);

        if (strcmp(argv[1],"column")==0 || strcmp(argv[1],"variable")==0)
            return Goblin_Variable_Cmd(XLP,interp,argc,argv);

        return Goblin_Generic_Cmd(XLP,interp,argc,argv);
    }
    catch (...)
    {
        return Goblin_Propagate_Exception(interp);
    }

    return TCL_OK;
}


int Goblin_Row_Cmd (mipInstance *XLP,Tcl_Interp* interp,int argc,
    _CONST_QUAL_ char* argv[]) throw(ERRejected,ERRange)
{
    if (argc<4)
    {
        WrongNumberOfArguments(interp,argc,argv);
        return TCL_ERROR;
    }

    if (strcmp(argv[2],"insert")==0)
    {
        if (argc!=5)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        TFloat l = TFloat(atof(argv[3]));
        TFloat u = TFloat(atof(argv[4]));

        sprintf(interp->result,"%ld",XLP->AddRestr(l,u));
        return TCL_OK;
    }

    if (strcmp(argv[2],"index")==0)
    {
        if (argc!=4)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        TRestr i = XLP->RestrIndex((char*)argv[3]);

        if (i==NoRestr) interp->result = "*";
        else sprintf(interp->result,"%ld",i);

        return TCL_OK;
    }

    TIndex i = TIndex(atol(argv[2]));

    if (strcmp(argv[3],"cancel")==0)
    {
        XLP -> DeleteRestr(i);
        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[3],"ubound")==0)
    {
        if (XLP->UBound(i)==InfFloat) sprintf(interp->result,"*");
        else sprintf(interp->result,"%g",XLP->UBound(i));

        return TCL_OK;
    }

    if (strcmp(argv[3],"lbound")==0)
    {
        if (XLP->LBound(i)==-InfFloat) sprintf(interp->result,"*");
        else sprintf(interp->result,"%g",XLP->LBound(i));

        return TCL_OK;
    }

    if (strcmp(argv[3],"label")==0)
    {
        sprintf(interp->result,"%s",
            XLP->RestrLabel(i,managedObject::OWNED_BY_RECEIVER));
        return TCL_OK;
    }

    if (strcmp(argv[3],"type")==0)
    {
        if (XLP->RestrType(i)==mipInstance::NON_BASIC)
            interp->result = "non_basic";
        if (XLP->RestrType(i)==mipInstance::BASIC_UB)
            interp->result = "upper";
        if (XLP->RestrType(i)==mipInstance::BASIC_LB)
            interp->result = "lower";
        if (XLP->RestrType(i)==mipInstance::RESTR_CANCELED)
            interp->result = "canceled";

        return TCL_OK;
    }

    if (strcmp(argv[3],"index")==0)
    {
        if (XLP->RevIndex(i)==NoIndex) sprintf(interp->result,"*");
        else sprintf(interp->result,"%ld",XLP->RevIndex(i));

        return TCL_OK;
    }

    if (strcmp(argv[3],"value")==0)
    {
        if (argc!=5)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        if (strcmp(argv[4],"lower")==0)
        {
            sprintf(interp->result,"%g",XLP->Y(i,mipInstance::LOWER));
            return TCL_OK;
        }

        if (strcmp(argv[4],"upper")==0)
        {
            sprintf(interp->result,"%g",XLP->Y(i,mipInstance::UPPER));
            return TCL_OK;
        }

        sprintf(interp->result,"Unknown option: %s row %ld value %s",
            argv[0],i,argv[4]);
        return TCL_ERROR;
    }

    if (strcmp(argv[3],"slack")==0)
    {
        if (argc!=5)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        if (strcmp(argv[4],"lower")==0)
        {
            sprintf(interp->result,"%g",XLP->Slack(i,mipInstance::LOWER));
            return TCL_OK;
        }

        if (strcmp(argv[4],"upper")==0)
        {
            sprintf(interp->result,"%g",XLP->Slack(i,mipInstance::UPPER));
            return TCL_OK;
        }

        sprintf(interp->result,"Unknown option: %s redbound %ld value %s",
            argv[0],i,argv[4]);
        return TCL_ERROR;
    }

    if (strcmp(argv[3],"set")==0)
    {
        if (argc!=6)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        if (strcmp(argv[4],"ubound")==0)
        {
            TFloat uu = InfFloat;

            if (strcmp(argv[5],"*")!=0) uu = TFloat(atof(argv[5]));

            XLP -> SetUBound(i,uu);
            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[4],"lbound")==0)
        {
            TFloat ll = -InfFloat;

            if (strcmp(argv[5],"*")!=0) ll = TFloat(atof(argv[5]));

            XLP -> SetLBound(i,ll);
            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[4],"label")==0)
        {
            XLP -> SetRestrLabel(i,(char*)argv[5],managedObject::OWNED_BY_SENDER);
            interp->result = "";
            return TCL_OK;
        }

        sprintf(interp->result,"Unknown option: %s row %s set %s",argv[0],argv[2],argv[4]);
        return TCL_ERROR;
    }

    sprintf(interp->result,"Unknown option: %s row %s",argv[0],argv[2]);
    return TCL_ERROR;
}


int Goblin_Variable_Cmd (mipInstance *XLP,Tcl_Interp* interp,int argc,
    _CONST_QUAL_ char* argv[]) throw(ERRejected,ERRange)
{
    if (argc<4)
    {
        WrongNumberOfArguments(interp,argc,argv);
        return TCL_ERROR;
    }

    if (strcmp(argv[2],"insert")==0)
    {
        if (argc!=6)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        TFloat l = TFloat(atof(argv[4]));
        TFloat u = TFloat(atof(argv[5]));

        mipInstance::TVarType tp = mipInstance::VAR_FLOAT;

        if (strcmp(argv[3],"integer")==0) tp = mipInstance::VAR_INT;
        else
        {
            if (strcmp(argv[3],"float")!=0)
            {
                interp->result = "Unknown variable type";
                return TCL_ERROR;
            }
        }

        sprintf(interp->result,"%ld",XLP->AddVar(l,u,tp));
        return TCL_OK;
    }

    if (strcmp(argv[2],"index")==0)
    {
        if (argc!=4)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        TVar i = XLP->VarIndex((char*)argv[3]);

        if (i==NoVar) interp->result = "*";
        else sprintf(interp->result,"%ld",i);

        return TCL_OK;
    }

    TIndex i = TIndex(atol(argv[2]));

    if (strcmp(argv[3],"cancel")==0)
    {
        XLP -> DeleteVar(i);
        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[3],"urange")==0)
    {
        if (XLP->URange(i)==InfFloat) sprintf(interp->result,"*");
        else sprintf(interp->result,"%g",XLP->URange(i));

        return TCL_OK;
    }


    if (strcmp(argv[3],"lrange")==0)
    {
        if (XLP->LRange(i)==-InfFloat) sprintf(interp->result,"*");
        else sprintf(interp->result,"%g",XLP->LRange(i));

        return TCL_OK;
    }

    if (strcmp(argv[3],"cost")==0)
    {
        sprintf(interp->result,"%g",XLP->Cost(i));
        return TCL_OK;
    }

    if (strcmp(argv[3],"type")==0)
    {
        if (XLP->VarType(i)==mipInstance::VAR_INT)
            interp->result = "integer";
        if (XLP->VarType(i)==mipInstance::VAR_FLOAT)
            interp->result = "float";
        if (XLP->VarType(i)==mipInstance::VAR_CANCELED)
            interp->result = "canceled";

        return TCL_OK;
    }

    if (strcmp(argv[3],"label")==0)
    {
        sprintf(interp->result,"%s",
            XLP->VarLabel(i,managedObject::OWNED_BY_RECEIVER));
        return TCL_OK;
    }

    if (strcmp(argv[3],"index")==0)
    {
        sprintf(interp->result,"%ld",XLP->Index(i));
        return TCL_OK;
    }

    if (strcmp(argv[3],"value")==0)
    {
        sprintf(interp->result,"%g",XLP->X(i));
        return TCL_OK;
    }

    if (strcmp(argv[3],"mark")==0)
    {
        if (strcmp(argv[4],"int")==0 || strcmp(argv[4],"integer")==0)
        {
            XLP -> SetVarType(i,mipInstance::VAR_INT);
            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[4],"float")==0)
        {
            XLP -> SetVarType(i,mipInstance::VAR_FLOAT);
            interp->result = "";
            return TCL_OK;
        }

        sprintf(interp->result,"Unknown option: %s variable %s mark %s",argv[0],argv[2],argv[4]);
        return TCL_ERROR;
    }

    if (strcmp(argv[3],"set")==0)
    {
        if (argc!=6)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        if (strcmp(argv[4],"urange")==0)
        {
            TFloat uu = InfFloat;

            if (strcmp(argv[5],"*")!=0) uu = TFloat(atof(argv[5]));

            XLP -> SetURange(i,uu);
            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[4],"lrange")==0)
        {
            TFloat ll = -InfFloat;
            if (strcmp(argv[5],"*")!=0) ll = TFloat(atof(argv[5]));
            XLP -> SetLRange(i,ll);
            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[4],"cost")==0)
        {
            TFloat cc = TFloat(atof(argv[5]));
            XLP -> SetCost(i,cc);
            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[4],"label")==0)
        {
            XLP -> SetVarLabel(i,(char*)argv[5],managedObject::OWNED_BY_SENDER);
            interp->result = "";
            return TCL_OK;
        }

        sprintf(interp->result,"Unknown option: %s variable %s set %s",argv[0],argv[2],argv[4]);
        return TCL_ERROR;
    }

    sprintf(interp->result,"Unknown option: %s variable %s",argv[0],argv[2]);
    return TCL_ERROR;
}


int Goblin_Delete_Ilp (ClientData clientData) throw()
{
    mipInstance* XLP = reinterpret_cast<mipInstance*>(clientData);
    goblinController* thisContext = &(XLP->Context());

    if (destroyThread) return TCL_OK;

    if (thisContext==CT)
    {
        int savedTraceLevel = thisContext->traceLevel;
        thisContext->traceLevel = 0;
        delete XLP;
        thisContext->traceLevel = savedTraceLevel;
    }
    else
    {
        thisContext->traceLevel = 0;
        delete XLP;
        if (!thisContext->IsReferenced()) delete thisContext;
    }

    return TCL_OK;
}
