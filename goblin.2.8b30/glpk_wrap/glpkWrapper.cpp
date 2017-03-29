
/*
**  This file forms part of the GOBLIN C++ Class Library.
**
**  Written by Bernhard Schmidt and Christian Fremuth-Paeger, August 2003.
**
**  Copying, compiling, distribution and modification
**  of this source code is permitted only in accordance
**  with the GOBLIN general licence information.
*/


#include "glpkWrapper.h"
#include "glpk.h"
#include "fileImport.h"
#include "moduleGuard.h"


// -----------------------------------------------------------------------------


glpkWrapper::glpkWrapper(TRestr k,TVar l,TIndex nz,TObjectSense dir,
    goblinController& CT) throw()  : mipInstance(CT)
{
    lp = lpx_create_prob();
    lpx_set_class(lp,LPX_MIP);
    index = NULL;
    rowIndex = NULL;
    objSense = NO_OBJECTIVE;

    SetObjectSense(dir);
}


glpkWrapper::glpkWrapper(const char* impFileName,
    goblinController& thisContext) throw(ERFile,ERParse) :
    mipInstance(thisContext)
{
    lp=lpx_create_prob();
    lpx_set_class(lp,LPX_MIP);
    index = NULL;
    rowIndex = NULL;
    objSense = NO_OBJECTIVE;

    #if defined(_TIMERS_)

    CT.globalTimer[TimerIO] -> Enable();

    #endif

    LogEntry(LOG_IO,"Loading native LP...");
    if (!CT.logIO && CT.logMem) LogEntry(LOG_MEM,"Loading native LP...");

    goblinImport F(impFileName,CT);

    F.Scan("mixed_integer");

    F.Scan("rows");
    TIndex* nrows = F.GetTIndexTuple(1);
    delete[] nrows;

    F.Scan("columns");
    TIndex* ncols = F.GetTIndexTuple(1);

    F.Scan("size");
    TIndex* nzero = F.GetTIndexTuple(1);
    delete[] nzero;

    F.Scan("pivot");
    TIndex* pPivot = F.GetTIndexTuple(3);

    if (pPivot[0]==NoIndex)
    {
        pivotRow = NoRestr;
        pivotColumn = NoVar;
        pivotDir = LOWER;
    }
    else
    {
        pivotRow = pPivot[0];
        pivotColumn = pPivot[1];
        pivotDir = TLowerUpper(pPivot[2]);
    }

    delete[] pPivot;

    ReadVarValues(&F,ncols[0]);
    delete[] ncols;

    // Variable values are released by the following operations
    TFloat* swapVarValue = varValue;
    TVar swapNumVars = numVars;
    varValue = NULL;

    F.Scan("rowvis");
    char* pDummy = F.GetCharTuple(0);
    delete[] pDummy;

    F.Scan("colvis");
    pDummy = F.GetCharTuple(0);
    delete[] pDummy;

    F.Scan("configure");
    F.ReadConfiguration();

    F.Scan(); // mixed_integer

    mipInstance::ReadMPSFile(F.Stream());

    InitBasis();

    mipInstance::ReadBASFile(F.Stream());

    int l = strlen(impFileName)-4;
    char* tmpLabel = new char[l+1];
    memcpy(tmpLabel,impFileName,l);
    tmpLabel[l] = 0;
    SetLabel(tmpLabel);
    delete[] tmpLabel;
    CT.SetMaster(Handle());

    varValue = swapVarValue;
    numVars = swapNumVars;

    #if defined(_TIMERS_)

    CT.globalTimer[TimerIO] -> Disable();

    #endif
}


glpkWrapper::~glpkWrapper() throw()
{
    if (lp) lpx_delete_prob(lp);
    if (index) delete[] index;
    if (rowIndex) delete[] rowIndex;
}


// *************************************************************** //
//                      File Input and Output                      //
// *************************************************************** //


void glpkWrapper::ReadMPSFile(char* file) throw()
{
    lp=lpx_read_mps(file);
}


void glpkWrapper::ReadCPLEX_LP(char* file) throw()
{
    lp = lpx_read_lpt(file);
}


void glpkWrapper::ReadGNU_MP_Model(char* model,char* data,char* output)
    throw()
{
    lp = lpx_read_model(model,data,output);
}


void glpkWrapper::ReadBASFile(char* file) throw(ERParse,ERRejected)
{
    if (lp) lpx_read_bas(lp,file);
}


void glpkWrapper::WriteMPSFile(char* file,TLPFormat)
    const throw(ERFile,ERRejected)
{
    if (lp) lpx_write_mps(lp,file);
}


void glpkWrapper::WriteCPLEX_LP(char* file) const throw()
{
    if (lp) lpx_write_lpt(lp,file);
}


void glpkWrapper::WriteBASFile(char* file,TLPFormat)
    const throw(ERFile,ERRejected)
{
    if (lp) lpx_write_bas(lp,file);
}


void glpkWrapper::WriteSimplexSolution(char* file) const throw()
{
    lpx_print_sol(lp,file);
}


// *************************************************************** //
//                      Instance Manipulation                      //
// *************************************************************** //


TVar glpkWrapper::AddVar(TFloat lb,TFloat ub,TFloat cost,TVarType type)
    throw(ERRejected)
{
    ReleaseVarValues();

    lpx_add_cols(lp,1);

    int n = lpx_get_num_cols(lp);

    if (lb==-InfFloat)
    {
        if (ub==InfFloat) lpx_set_col_bnds(lp,n,LPX_FR,0,0);
        else lpx_set_col_bnds(lp,n,LPX_UP,0,ub);
    }
    else
    {
        if (ub==InfFloat)
        {
            lpx_set_col_bnds(lp,n,LPX_LO,lb,0);
        }
        else
        {
            if (lb==ub)
                    lpx_set_col_bnds(lp,n,LPX_FX,lb,ub);
            else lpx_set_col_bnds(lp,n,LPX_DB,lb,ub);
        }
    }

    lpx_set_col_coef(lp,n,cost);

    if (type==VAR_INT)
    {
        if (lpx_get_class(lp)==LPX_LP) lpx_set_class(lp,LPX_MIP);

        lpx_set_col_kind(lp,n,LPX_IV);
    }

    if (index)
    {
        index = (TRestr *)GoblinRealloc(index,n*sizeof(TRestr));
        index[n-1] = K()+n-1;
    }

    return n-1;
}


TRestr glpkWrapper::AddRestr(TFloat lb,TFloat ub) throw(ERRejected)
{
    lpx_add_rows(lp,1);

    int n = lpx_get_num_rows(lp);

    if (lb==-InfFloat)
    {
        if (ub==InfFloat)
            lpx_set_row_bnds(lp,n,LPX_FR,0,0);
        else lpx_set_row_bnds(lp,n,LPX_UP,0,ub);
    }
    else
    {
        if (ub==InfFloat) lpx_set_row_bnds(lp,n,LPX_LO,lb,0);
        else
        {
            if (lb==ub) lpx_set_row_bnds(lp,n,LPX_FX,lb,ub);
            else lpx_set_row_bnds(lp,n,LPX_DB,lb,ub);
        }
    }

    if (rowIndex)
    {
        rowIndex = (TRestr *)GoblinRealloc(rowIndex,n*sizeof(TRestr));
        rowIndex[n-1] = n-1;
    }

    return n-1;
}


void glpkWrapper::DeleteVar(TVar j) throw(ERRange,ERRejected)
{
    #if defined(_FAILSAVE_)

    if (j>=TVar(lpx_get_num_cols(lp))) NoSuchVar("DeleteVar",j);

    #endif

    ReleaseVarValues();

    lpx_unmark_all(lp);
    lpx_mark_col(lp,j+1,1);
    lpx_del_items(lp);
}


void glpkWrapper::DeleteRestr(TRestr i) throw(ERRange)
{
    #if defined(_FAILSAVE_)

    if (i>=TRestr(lpx_get_num_rows(lp))) NoSuchRestr("DeleteRestr",i);

    #endif

    lpx_unmark_all(lp);
    lpx_mark_row(lp,i+1,1);
    lpx_del_items(lp);
}


void glpkWrapper::SetLRange(TVar i,TFloat lb) throw(ERRange)
{
    #if defined(_FAILSAVE_)

    if (i>=L()) NoSuchVar("SetLRange",i);

    #endif

    if (lb==-InfFloat)
    {
        if (URange(i)==InfFloat)
                    lpx_set_col_bnds(lp,i+1,LPX_FR,0,0);
        else lpx_set_col_bnds(lp,i+1,LPX_UP,0,URange(i));
    }
    else
    {
        if (lb==URange(i)) lpx_set_col_bnds(lp,i+1,LPX_FX,lb,lb);
        else
        {
            if (URange(i)==InfFloat)
                        lpx_set_col_bnds(lp,i+1,LPX_LO,lb,0);
            else lpx_set_col_bnds(lp,i+1,LPX_DB,lb,URange(i));
        }
    }
}


void glpkWrapper::SetURange(TVar i,TFloat ub) throw(ERRange)
{
    #if defined(_FAILSAVE_)

    if (i>=L()) NoSuchVar("SetURange",i);

    #endif

    if (ub==InfFloat)
    {
        if (LRange(i)==-InfFloat)
                    lpx_set_col_bnds(lp,i+1,LPX_FR,0,0);
        else lpx_set_col_bnds(lp,i+1,LPX_LO,LRange(i),0);
    }
    else
    {
        if (ub==LRange(i)) lpx_set_col_bnds(lp,i+1,LPX_FX,ub,ub);
        else
        {
            if (LRange(i)==-InfFloat)
                        lpx_set_col_bnds(lp,i+1,LPX_UP,0,ub);
            else lpx_set_col_bnds(lp,i+1,LPX_DB,LRange(i),ub);
        }
    }
}


void glpkWrapper::SetUBound(TRestr i,TFloat ub) throw(ERRange)
{
    #if defined(_FAILSAVE_)

    if (i>=K()) NoSuchRestr("SetUBound",i);

    #endif

    if (ub==InfFloat)
    {
        if (LBound(i)==-InfFloat)
                    lpx_set_row_bnds(lp,i+1,LPX_FR,0,0);
        else lpx_set_row_bnds(lp,i+1,LPX_LO,LBound(i),0);
    }
    else
    {
        if (ub==LBound(i)) lpx_set_row_bnds(lp,i+1,LPX_FX,ub,ub);
        else
        {
            if (LBound(i)==-InfFloat)
                        lpx_set_row_bnds(lp,i+1,LPX_UP,0,ub);
            else lpx_set_row_bnds(lp,i+1,LPX_DB,LBound(i),ub);
        }
    }
}

void glpkWrapper::SetLBound(TRestr i,TFloat lb) throw(ERRange)
{
    #if defined(_FAILSAVE_)

    if (i>=K()) NoSuchRestr("SetLBound",i);

    #endif

    if (lb==-InfFloat)
    {
        if (UBound(i)==InfFloat)
                    lpx_set_row_bnds(lp,i+1,LPX_FR,0,0);
        else lpx_set_row_bnds(lp,i+1,LPX_UP,0,UBound(i));
    }
    else
    {
        if (lb==UBound(i)) lpx_set_row_bnds(lp,i+1,LPX_FX,lb,lb);
        else
        {
            if (UBound(i)==InfFloat)
                        lpx_set_row_bnds(lp,i+1,LPX_LO,lb,0);
            else lpx_set_row_bnds(lp,i+1,LPX_DB,lb,UBound(i));
        }
    }
}


void glpkWrapper::SetCost(TVar var,TFloat cost) throw(ERRange)
{
    #if defined(_FAILSAVE_)

    if (var>=L()) NoSuchVar("SetCost",var);
    if (cost>=InfFloat || cost<=-InfFloat)
        Error(ERR_RANGE,"SetCost","Finite cost coefficients required");

    #endif

    lpx_set_col_coef(lp,var+1,cost);
}


void glpkWrapper::SetVarType(TVar var,TVarType type) throw(ERRange)
{
    #if defined(_FAILSAVE_)

    if (var>=L()) NoSuchVar("SetVarType",var);

    #endif

    if (type==VAR_INT && lpx_get_class(lp)==LPX_LP)
    lpx_set_class(lp,LPX_MIP);

    if (type==VAR_INT && lpx_get_col_kind(lp,var+1)==LPX_CV)
            lpx_set_col_kind(lp,var+1,LPX_IV);

    if (type==VAR_FLOAT && lpx_get_col_kind(lp,var+1)==LPX_IV)
            lpx_set_col_kind(lp,var+1,LPX_CV);
}


void glpkWrapper::SetVarLabel(TVar var,char* name,TOwnership own)
    throw(ERRange,ERRejected)
{
    #if defined(_FAILSAVE_)

    if (var>=L()) NoSuchVar("SetVarLabel",var);

    #endif

    lpx_set_col_name(lp,var+1,name);
}


void glpkWrapper::SetRestrLabel(TRestr row,char* name,TOwnership own)
   throw(ERRange,ERRejected)
{
    #if defined(_FAILSAVE_)

    if (row>=K()) NoSuchRestr("SetRestrLabel",row);

    #endif

    lpx_set_row_name(lp,row+1,name);
}


void glpkWrapper::SetObjectSense(TObjectSense dir) throw()
{
    if (dir==MINIMIZE) lpx_set_obj_dir(lp,LPX_MIN);
    if (dir==MAXIMIZE) lpx_set_obj_dir(lp,LPX_MAX);
    if (dir==NO_OBJECTIVE)
    {
        for (TVar i=1;i<=L();i++)
            lpx_set_col_coef(lp,i,0);
    }

    objSense = dir;
}


void glpkWrapper::SetRow(TRestr i,TVar len,TVar* index,double* val)
    throw(ERRange)
{
    #if defined(_FAILSAVE_)

    if (i>=K()) NoSuchRestr("SetRow",i);

    #endif

    for (TVar u=0;u<len;u++)
    {
        indexBuffer[u+1] = int(index[u]+1);
        valueBuffer[u+1] = val[u];
    }

    lpx_set_mat_row(lp,int(i+1),int(len),indexBuffer,valueBuffer);
}


void glpkWrapper::SetColumn(TVar j,TRestr len,TRestr* index,double* val)
    throw(ERRange)
{
    #if defined(_FAILSAVE_)

    if (j>=L()) NoSuchVar("SetCol",j);

    #endif

    for (TRestr u=0;u<len;u++)
    {
        indexBuffer[u+1] = int(index[u]+1);
        valueBuffer[u+1] = val[u];
    }

    lpx_set_mat_col(lp,int(j+1),int(len),indexBuffer,valueBuffer);
}


void glpkWrapper::SetCoeff(TRestr i,TVar j,TFloat aa)
    throw(ERRange)
{
    #if defined(_FAILSAVE_)

    if (i>=K())  NoSuchRestr("Coeff",i);
    if (j>=L())  NoSuchVar("Coeff",j);

    if (aa>=InfFloat || aa<=-InfFloat)
        Error(ERR_RANGE,"SetCoeff","Finite matrix coefficients required");

    #endif

    int nz = lpx_get_mat_col(lp,int(j+1),indexBuffer,valueBuffer);

    int u = 1;
    for (;u<=nz && indexBuffer[u];u++)
    {
        if (indexBuffer[u]==int(i+1))
        {
            valueBuffer[u] = aa;
            break;
        }
    }

    if (u>nz || indexBuffer[u]==0)
    {
        if (aa==0) return;

        indexBuffer[u] = i+1;
        valueBuffer[u] = aa;
        indexBuffer[u+1] = 0;
        nz++;
    }

    lpx_set_mat_col(lp,int(j+1),nz,indexBuffer,valueBuffer);
}


// *************************************************************** //
//                      Retrieval Operations                       //
// *************************************************************** //


TRestr glpkWrapper::K() const throw()
{
    return lpx_get_num_rows(lp);
}


TVar glpkWrapper::L() const throw()
{
    return lpx_get_num_cols(lp);
}


TIndex glpkWrapper::NZ() const throw()
{
    return lpx_get_num_nz(lp);
}


TFloat glpkWrapper::Cost(TVar j) const throw(ERRange)
{
    #if defined(_FAILSAVE_)

    if (j>= L()) NoSuchVar("Cost",j);

    #endif

    return lpx_get_col_coef(lp,j+1);
}


TVar glpkWrapper::GetRow(TRestr i,TRestr* index,double* val)
    const throw(ERRange)
{
    #if defined(_FAILSAVE_)

    if (TRestr(i)>=K()) NoSuchRestr("GetRow",i);

    #endif

    int nz = lpx_get_mat_row(lp,int(i+1),indexBuffer,valueBuffer);

    for (int u=0;u<nz;u++)
    {
        index[u] = TIndex(indexBuffer[u+1]-1);
        val[u] = valueBuffer[u+1];
    }

    return TIndex(nz);
}


TRestr glpkWrapper::GetColumn(TVar i,TRestr* index,double* val)
    const throw(ERRange)
{
    #if defined(_FAILSAVE_)

    if (i>= L()) NoSuchVar("GetColumn",i);

    #endif

    int nz = lpx_get_mat_col(lp,int(i+1),indexBuffer,valueBuffer);

    for (int u=0;u<nz;u++)
    {
        index[u] = TRestr(indexBuffer[u+1]-1);
        val[u] = valueBuffer[u+1];
    }

    return TRestr(nz);
}


TFloat glpkWrapper::LRange(TVar i) const throw(ERRange)
{
    #if defined(_FAILSAVE_)

    if (i>= L()) NoSuchVar("LRange",i);

    #endif

    int typx = 0;
    double lb,ub = 0;

    lpx_get_col_bnds(lp,i+1,&typx,&lb,&ub);

    if (typx==LPX_FR || typx==LPX_UP) return -InfFloat;

    return lb;
}


TFloat glpkWrapper::URange(TVar i) const throw(ERRange)
{
    #if defined(_FAILSAVE_)

    if (i>= L()) NoSuchVar("URange",i);

    #endif

    int typx = 0;
    double lb,ub = 0;

    lpx_get_col_bnds(lp,i+1,&typx,&lb,&ub);

    if (typx==LPX_FR || typx==LPX_LO) return InfFloat;

    return ub;
}


TFloat glpkWrapper::LBound(TRestr i) const throw(ERRange)
{
    #if defined(_FAILSAVE_)

    if (i>=K()+L()) NoSuchRestr("LBound",i);

    #endif

    if (i>=K()) return LRange(i-K());

    int typx = 0;
    double lb,ub = 0;

    lpx_get_row_bnds(lp,i+1,&typx,&lb,&ub);

    if (typx==LPX_FR || typx== LPX_UP) return -InfFloat;

    return lb;
}


TFloat glpkWrapper::UBound(TRestr i) const throw(ERRange)
{
    #if defined(_FAILSAVE_)

    if (i>=K()+L()) NoSuchRestr("UBound",i);

    #endif

    if (i>=K()) return URange(i-K());

    int typx = 0;
    double lb,ub = 0;

    lpx_get_row_bnds(lp,i+1,&typx,&lb,&ub);

    if(typx==LPX_FR || typx== LPX_LO) return InfFloat;

    return ub;
}


glpkWrapper::TVarType glpkWrapper::VarType(TVar i) const throw(ERRange)
{
    #if defined(_FAILSAVE_)

    if (i>=L()) NoSuchVar("VarType",i);

    #endif

    if (lpx_get_class(lp)==LPX_LP) return VAR_FLOAT;

    int type = lpx_get_col_kind(lp,i+1);

    if (type==LPX_IV)
                return VAR_INT;
    else return VAR_FLOAT;
}


static char thisVarLabel[20] = "";

char* glpkWrapper::VarLabel(TVar i,TOwnership tp) const throw(ERRange)
{
    #if defined(_FAILSAVE_)

    if (i>=L()) NoSuchVar("VarLabel",i);

    #endif

    char* label= lpx_get_col_name(lp,i+1);

    if (label==NULL)
    {
        sprintf(thisVarLabel,"%ld",L());
        int len = strlen(thisVarLabel);

        sprintf(thisVarLabel,"x%*.*ld",len,len,i+1);
    }
    else sprintf(thisVarLabel,"%s",label);

    if (tp==OWNED_BY_RECEIVER) return thisVarLabel;

    char* retLabel = new char[strlen(thisVarLabel)+1];
    strcpy(retLabel,thisVarLabel);

    return retLabel;
}


static char thisRestrLabel[20] = "";

char* glpkWrapper::RestrLabel(TRestr i,TOwnership tp) const throw(ERRange)
{
    #if defined(_FAILSAVE_)

    if (i>=K()) NoSuchRestr("RestrLabel",i);

    #endif

    char* label = lpx_get_row_name(lp,i+1);

    if (label==NULL)
    {
        sprintf(thisRestrLabel,"%ld",K());
        int len = strlen(thisRestrLabel);

        sprintf(thisRestrLabel,"r%*.*ld",len,len,i+1);
    }
    else sprintf(thisRestrLabel,"%s",label);

    if (tp==OWNED_BY_RECEIVER) return thisRestrLabel;

    char* retLabel = new char[strlen(thisRestrLabel)+1];
    strcpy(retLabel,thisRestrLabel);

    return retLabel;
}


glpkWrapper::TObjectSense glpkWrapper::ObjectSense() const throw()
{
    return objSense;
    /*
    int dir = lpx_get_obj_dir(lp);

    if (dir==LPX_MIN) return MINIMIZE;
    else return MAXIMIZE;
    */
}


TFloat glpkWrapper::Coeff(TRestr i,TVar j)
    const throw(ERRange)
{
    #if defined(_FAILSAVE_)

    if (i>=K())  NoSuchRestr("Coeff",i);
    if (j>=L())  NoSuchVar("Coeff",j);

    #endif

    int nz = lpx_get_mat_col(lp,j+1,indexBuffer,valueBuffer);

    for (int u=1;u<=nz && indexBuffer[u];u++)
    {
        if (indexBuffer[u]==int(i+1)) return valueBuffer[u];
    }

    return 0;
}


// *************************************************************** //
//                    Basic dependent information                  //
// *************************************************************** //


glpkWrapper::TRestrType glpkWrapper::RestrType(TRestr row)
    const throw(ERRange)
{
    #if defined(_FAILSAVE_)

    if (row>=K()+L()) NoSuchRestr("RestrType",row);

    #endif

    int type = 0;

    if (row<K())
        lpx_get_row_info(lp,row+1,&type,NULL,NULL);
    else lpx_get_col_info(lp,row-K()+1,&type,NULL,NULL);

    TRestrType ret = BASIC_LB;

    if (type==LPX_BS) ret = NON_BASIC;
    else if (type==LPX_NU) ret = BASIC_UB;

    return ret;
}


TRestr glpkWrapper::Index(TVar i)
    const throw(ERRange)
{
    #if defined(_FAILSAVE_)

    if (i>=L())  NoSuchVar("Index",i);

    #endif

    if (index==NULL) InitIndex();

    return index[i];
}


TRestr glpkWrapper::RowIndex(TRestr i)
    const throw(ERRange)
{
    #if defined(_FAILSAVE_)

    if (i>=K())  NoSuchRestr("Index",i);

    #endif

    if (index==NULL) InitIndex();

    return rowIndex[i];
}


TVar glpkWrapper::RevIndex(TRestr i)
    const throw(ERRange)
{
    #if defined(_FAILSAVE_)

    if (i>=K()+L())  NoSuchRestr("RevIndex",i);

    #endif

    if (index==NULL) InitIndex();

    for (TVar j=0;j<L();j++)
        if (index[j]==i) return j;

    return NoRestr;
}


void glpkWrapper::SetIndex(TRestr i,TVar j,TLowerUpper rt)
    throw(ERRange,ERRejected)
{
    #if defined(_FAILSAVE_)

    if (i>=K()+L())  NoSuchRestr("SetIndex",i);
    if (j>=L())  NoSuchVar("SetIndex",j);

    #endif

    TRestr k = Index(j);

    SetRestrType(i,rt);

    if (k==i) return;

    index[j] = i;

    // Update row index (but only if a structural restriction enters the basis)

    if (i<K()) rowIndex[i] = K()+j;

    if (k==NoRestr) return;

    // Release old mate k of j from basis

    if (i<K())
    {
        if (k<K())
            lpx_set_row_stat(lp,k+1,LPX_BS);
        else lpx_set_col_stat(lp,k-K()+1,LPX_BS);
    }
}


void glpkWrapper::SetRestrType(TRestr i,TLowerUpper rt)
    throw(ERRange,ERRejected)
{
    #if defined(_FAILSAVE_)

    if (i>=K()+L())  NoSuchRestr("SetRestrType",i);

    #endif

    int type = LPX_NL;

    if (rt==UPPER) type = LPX_NU;

    if (i<K())
        lpx_set_row_stat(lp,i+1,type);
    else lpx_set_col_stat(lp,i-K()+1,type);
}


void glpkWrapper::ReleaseIndex()
    const throw()
{
    if (index)
    {
        delete[] index;
        index = NULL;
    }

    if (rowIndex)
    {
        delete[] rowIndex;
        rowIndex = NULL;
    }
}


void glpkWrapper::InitIndex()
    const throw()
{
    if (index) return;

    int tag = 0;
    int k = lpx_get_num_rows(lp);
    int l = lpx_get_num_cols(lp);

    index = new TRestr[l];
    rowIndex = new TRestr[k];

    // If possible, match rows with the corresponding artificial column

    for (int i=1;i<=k;i++)
    {
        lpx_get_row_info(lp,i,&tag,NULL,NULL);

        if (tag==LPX_BS) rowIndex[i-1] = TRestr(i-1);
        else rowIndex[i-1] = NoRestr;
    }

    // If possible, match variables with the corresponding variable bound
    // Match the remaining rows and variables in an arbitrary order

    TVar j = 0;

    for (int i=1;i<=l;i++)
    {
        lpx_get_col_info(lp,i,&tag,NULL,NULL);

        if (tag!=LPX_BS) index[i-1] = k+i-1;
        else
        {
            while (int(j)<k && rowIndex[j]!=NoRestr) j++;

            if (int(j)>=k)
            {
                InternalError("InitIndex","Inconsistent basis data");
            }

            index[i-1] = j;
            rowIndex[j] = TRestr(i-1);
        }
    }
}


void glpkWrapper::InitBasis() const throw()
{
    lpx_std_basis(lp);

    ReleaseIndex();
}


void glpkWrapper::EvaluateBasis() throw(ERRejected)
{
    lpx_warm_up(lp);

    ReleaseIndex();
}


TFloat glpkWrapper::X(TVar var) const throw(ERRange)
{
    #if defined(_FAILSAVE_)

    if (var>= L()) NoSuchVar("X",var);

    #endif

    double value = 0;

    lpx_warm_up(lp);
    lpx_get_col_info(lp,int(var+1),NULL,&value,NULL);

    return value;
}


TFloat glpkWrapper::Y(TRestr row,TLowerUpper loUp) const throw(ERRange)
{
    #if defined(_FAILSAVE_)

    if (row>=K()+L()) NoSuchRestr("Y",row);

    #endif

    lpx_warm_up(lp);

    int type = 0;
    double value = 0;

    if (row<K())
        lpx_get_row_info(lp,int(row+1),&type,NULL,&value);
    else lpx_get_col_info(lp,int(row-K()+1),&type,NULL,&value);

    if (type==LPX_BS) return 0;

    if (type==LPX_NF
    || (type==LPX_NS && loUp==LOWER)
    || (type==LPX_NL && loUp==LOWER)
    || (type==LPX_NU && loUp==UPPER) ) return value;

    return 0;
}


TIndex glpkWrapper::TableauRow(TIndex k,TIndex* index,double* val)
    const throw(ERRange)
{
    #if defined(_FAILSAVE_)

    if (TRestr(k)>= L()) NoSuchRestr("TableauRow",k);

    #endif

    if (lpx_get_status(lp)==LPX_UNDEF && lpx_warm_up(lp)!=LPX_E_OK)
    {
        InitBasis();
        lpx_warm_up(lp);
    }

    int nz = lpx_eval_tab_row(lp,int(k+1),indexBuffer,valueBuffer);

    for (int u=0;u<nz;u++)
    {
        index[u] = TIndex(indexBuffer[u+1]-1);
        val[u] = valueBuffer[u+1];
    }

    return TIndex(nz);
}


TIndex glpkWrapper::TableauColumn(TIndex k,TIndex* index,double* val)
    const throw(ERRange)
{
    #if defined(_FAILSAVE_)

    if (TRestr(k)>=L()+K()) NoSuchRestr("TableauCol",k);

    #endif

    if (lpx_get_status(lp)==LPX_UNDEF && lpx_warm_up(lp)!=LPX_E_OK)
    {
        InitBasis();
        lpx_warm_up(lp);
    }

    int nz = lpx_eval_tab_col(lp,int(k+1),indexBuffer,valueBuffer);

    for (int u=0;u<nz;u++)
    {
        index[u] = TIndex(indexBuffer[u+1]-1);
        val[u] = valueBuffer[u+1];
    }

    return TIndex(nz);
}


TFloat glpkWrapper::Tableau(TIndex j,TIndex i) const throw(ERRange,ERRejected)
{
    #if defined(_FAILSAVE_)

    if (i>=K()+L()) NoSuchVar("Tableau",i);
    if (j>=K()+L()) NoSuchVar("Tableau",j);

    if (RevIndex(j)!=NoRestr) 
        Error(ERR_REJECTED,"Tableau","Non-basic column");
    if (RevIndex(i)==NoRestr) 
        Error(ERR_REJECTED,"Tableau","Basic column");

    #endif

    if (lpx_get_status(lp)==LPX_UNDEF && lpx_warm_up(lp)!=LPX_E_OK)
    {
        InitBasis();
        lpx_warm_up(lp);
    }

    int nz = lpx_eval_tab_row(lp,int(j+1),indexBuffer,valueBuffer);

    for (int u=0;u<nz;u++)
        if (int(i) == indexBuffer[u+1]-1) return valueBuffer[u+1];

    return 0;
}


TFloat glpkWrapper::BaseInverse(TIndex j,TIndex i)
    const throw(ERRange,ERRejected)
{
    #if defined(_FAILSAVE_)

    if (i>=K()) NoSuchRestr("BaseInverse",i);
    if (j>=K()+L()) NoSuchRestr("BaseInverse",j);

    if (RevIndex(j)!=NoVar) 
        Error(ERR_REJECTED,"BaseInverse","Non-basic row");

    #endif

    if (lpx_get_status(lp)==LPX_UNDEF && lpx_warm_up(lp)!=LPX_E_OK)
    {
        InitBasis();
        lpx_warm_up(lp);
    }

    indexBuffer[1] = int(i)+1;
    valueBuffer[1] = 1;

    int nz = lpx_transform_col(lp,1,indexBuffer,valueBuffer);

    for (int u=0;u<nz;u++)
        if (int(j) == indexBuffer[u+1]-1) return valueBuffer[u+1];

    return 0;
}


void glpkWrapper::Pivot(TIndex j,TIndex i,TLowerUpper rt)
    throw(ERRange,ERRejected)
{
    #if defined(_FAILSAVE_)

    if (i>=K()+L()) NoSuchVar("Pivot",i);
    if (j>=K()+L()) NoSuchVar("Pivot",j);

    if (RevIndex(i)==NoVar) 
        Error(ERR_REJECTED,"Pivot","Basic column");

    #endif

    if (!index) InitIndex();

    TRestr k = 0;
    for (;k<K() && rowIndex[k]!=j;k++) {};

    #if defined(_FAILSAVE_)

    if (k==K()) 
        Error(ERR_REJECTED,"Pivot","Non-basic column");

    #endif

    rowIndex[k] = i;

    // Add column i to basis

    if (i<K())
    lpx_set_row_stat(lp,i+1,LPX_BS);
    else lpx_set_col_stat(lp,i-K()+1,LPX_BS);

    // Release column j from basis

    int type = LPX_NL;

    if (rt==UPPER) type = LPX_NU;

    if (j<K())
        lpx_set_row_stat(lp,j+1,type);
    else lpx_set_col_stat(lp,j-K()+1,type);

    // Update index[]

    if (j>=K()) index[j-K()] = j;
    if (i>=K()) index[i-K()] = k;

    for (TRestr l=0;l<L();l++)
        if (index[l]>=K()) index[l] = K()+l;
}


TIndex glpkWrapper::TransformRow(TIndex len,TIndex* index,double* val)
{
    for (TIndex u=0;u<len;u++)
    {
        indexBuffer[u+1] = int(index[u]+1);
        valueBuffer[u+1] = val[u];
    }

    int nz = lpx_transform_row(lp,len,indexBuffer,valueBuffer);

    for (TIndex u=0;u<len;u++)
    {
        index[u] = TIndex(indexBuffer[u+1]-1);
        val[u] = valueBuffer[u+1];
    }

    return TIndex(nz);
}


// *************************************************************** //
//                           Control Parameters                    //
// *************************************************************** //


void glpkWrapper::ResetParameters()
{
    lpx_reset_parms(lp);
}


void glpkWrapper::SetIntegerParameter(int parameter,int value)
{
    lpx_set_int_parm(lp,parameter,value);
}


void glpkWrapper::SetRealParameter(int parameter ,double value)
{
   lpx_set_real_parm(lp,parameter,value);
}


int glpkWrapper::GetIntegerParameter(int parameter)
{
    return lpx_get_int_parm(lp,parameter);
}


double glpkWrapper::GetRealParameter(int parameter)
{
    return lpx_get_real_parm(lp,parameter);
}


// *************************************************************** //
//                           Algorithms                            //
// *************************************************************** //


TFloat glpkWrapper::SolveLP() throw(ERRejected)
{
    moduleGuard M(ModGLPK,*this,"Calling GLPK plugin...");

    int ret = 0;

    if (CT.methLP<3)
    {
        LogEntry(LOG_METH,"(Simplex method)");
        ret = lpx_simplex(lp);

        if (ret == LPX_E_FAULT)
        {
            LogEntry(LOG_RES,"...Initial basis is invalid");
            LogEntry(LOG_METH,"Restarting with default basis...");

            ResetBasis();
            ret = lpx_simplex(lp);
        }
    }
    else
    {
        LogEntry(LOG_METH,"(Interior point method)");
        ret = lpx_interior(lp);
    }

    ReleaseIndex();
    InitIndex();

    int state = lpx_get_status(lp);

    if (state==LPX_NOFEAS)
    {
        M.Shutdown(LOG_RES,"...Problem is infeasible");

        if (ObjectSense()==MAXIMIZE)
             return -InfFloat;
        else return InfFloat;
    }

    if (state==LPX_UNBND)
    {
        M.Shutdown(LOG_RES,"...Problem is unbounded");

        if (ObjectSense()==MAXIMIZE)
             return -InfFloat;
        else return InfFloat;
    }

    TFloat obj = lpx_get_obj_val(lp);

    sprintf(CT.logBuffer,"...Optimal Objective Value: %g",obj);
    M.Shutdown(LOG_RES,CT.logBuffer);

    return obj;
}


TFloat glpkWrapper::SolveMIP() throw(ERRejected)
{
    return mipInstance::SolveMIP();

    moduleGuard M(ModGLPK,*this,"Calling GLPK plugin...");
    LogEntry(LOG_METH,"(Simplex branch and bound)");

    int ret = lpx_simplex(lp);

    int state = lpx_get_status(lp);

    if (state==LPX_NOFEAS)
    {
        M.Shutdown(LOG_RES,"...LP relaxation is infeasible");
        return InfFloat;
    }

    ReleaseIndex();

    LogEntry(LOG_METH,"Start branching...");

    ret = lpx_integer(lp);

    if (ret==LPX_E_FAULT)
    {
        Error(ERR_REJECTED,"SolveMIP","Bounds and objective must be integral");
    }

    state = lpx_get_mip_stat(lp);

    if (state==LPX_I_NOFEAS || state==LPX_I_UNDEF)
    {
        M.Shutdown(LOG_RES,"...Problem is infeasible");
        return InfFloat;
    }

    TFloat obj = TFloat(lpx_get_mip_obj(lp));

    sprintf(CT.logBuffer,"...Optimal Objective Value: %g",obj);
    M.Shutdown(LOG_RES,CT.logBuffer);

    return obj;
}


glpkFactory::glpkFactory() throw() : managedObject(goblinDefaultContext)
{
}

mipInstance* glpkFactory::NewInstance(TRestr k,TVar l,TIndex nz,TObjectSense dir,
    goblinController& thisContext) const throw(ERRange)
{
    return new glpkWrapper(k,l,nz,dir,thisContext);
}


mipInstance* glpkFactory::ReadInstance(const char* impFileName,goblinController& thisContext)
    const throw(ERRange)
{
    return new glpkWrapper(impFileName,thisContext);
}


glpkFactory::~glpkFactory() throw()
{
}


unsigned long glpkFactory::Size() const throw()
{
    return sizeof(glpkFactory);
}


char* glpkFactory::Authors() const throw()
{
    return "Andrew Makhorin";
}


int glpkFactory::MajorRelease() const throw()
{
    return 4;
}


int glpkFactory::MinorRelease() const throw()
{
    return 0;
}


char* glpkFactory::PatchLevel() const throw()
{
    return "";
}


char* glpkFactory::BuildDate() const throw()
{
    return "";
}

char* glpkFactory::License() const throw()
{
    return "GPL";
}


mipFactory::TLPOrientation glpkFactory::Orientation() const throw()
{
    return ROW_ORIENTED;
}
