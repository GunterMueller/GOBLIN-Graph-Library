
// --------------------------------------------------------------------------
//  Example code for loading a library object from file, using the general
//  file import interface.
// --------------------------------------------------------------------------


#include <abstractMixedGraph.h>


int main(int ParamCount,const char* ParamStr[])
{
    // Read an occasional "-format xxx" command line parameter.
    // By default, apply the Dimacs "edge" format
    int formatIndex = goblinDefaultContext.FindParam(ParamCount,ParamStr,"-format");
    const char* formatString =
        (formatIndex>0 && formatIndex<ParamCount-2) ?
        ParamStr[(formatIndex+1)] : "goblin";

    managedObject* X =
        goblinDefaultContext.ImportByFormatName(ParamStr[ParamCount-1],formatString);

    if (!X)
    {
        printf("Unable to load from file \"%s\"\n", ParamStr[ParamCount-1]);
        return -1;
    }

    if (!X->IsGraphObject())
    {
        printf("File \"%s\" does not specify a graph object\n", ParamStr[ParamCount-1]);
        return -1;
    }

    if (dynamic_cast<abstractMixedGraph*>(X)->IsPlanar())
    {
        printf("Input graph is planar\n");
        return 1;
    }

    printf("Input graph is non-planar\n");
    return 0;
}
