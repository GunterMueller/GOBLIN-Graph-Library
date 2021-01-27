
# Unit test for the computation of minimum weigth edge covers

set testTargetFilters \
    [list [file join planar tiles "*.gob"] ]

set testTargetFormat "gob"

set listOfTestSteps {
    "Instance generation"   TestInstanceGeneration  \
    "#Nodes"                GetNNodes               \
    "#Arcs"                 GetNArcs                \
    "Edge cover"            TestEdgeCover           \
    "Perfect Matching"      Test1Factor             \
    "Export to GIF"         TestGifExport           \
    "Check Bounds"          CheckBounds             \
}

proc TestInstanceGeneration {testObject testOutput} {
    global goblinRandMin goblinRandMax

    goblin openGrid -rows 7 -columns 10 -square DEP_OBJECT
    $testObject reorder incidences -geometric
    DEP_OBJECT merge -facets [$testObject handle]

    set goblinRandMin 1
    set goblinRandMax 9
    DEP_OBJECT random -edgeLength
    DEP_OBJECT configure -nodeDemand 1 -upperBound 1 -metricType 0

    return 1
}

proc GetNNodes {testObject testOutput} {
    return [DEP_OBJECT #nodes]
}

proc GetNArcs {testObject testOutput} {
    return [DEP_OBJECT #edges]
}

proc TestEdgeCover {testObject testOutput} {
    global goblinMethMCF goblinMethMCFST LowerBound

    set goblinMethMCF 1
    set goblinMethMCFST 2

    set LowerBound [DEP_OBJECT edgeCover]
    return $LowerBound
}

proc Test1Factor {testObject testOutput} {
    global goblinMethCandidates goblinMethMCF goblinMethMCFST goblinMethFailSave UpperBound

    set goblinMethFailSave 1
    set goblinMethCandidates -1
    set goblinMethMCF 5
    set goblinMethMCFST 2

    if {[expr [DEP_OBJECT #nodes]%2]==1} {
        return 0
    }

    DEP_OBJECT minimumCostMatching
    DEP_OBJECT extract matching

    set UpperBound [DEP_OBJECT info -subgraphWeight]
    return $UpperBound
}

proc TestGobExport {testObject testOutput} {
    global ZipEnabled
    set ZipEnabled 0

    ExportAndZip DEP_OBJECT "$testOutput.gob" gob object

    return 0
}

proc CheckBounds {testObject testOutput} {
    global LowerBound UpperBound

    if {$LowerBound>$UpperBound} {error "Relaxation error"}

    return [expr $UpperBound-$LowerBound]
}

proc TestGifExport {testObject testOutput} {
    # This exports the minimal edge cover given by the edge colour attribute,
    # and the minimum cost perfect matching provided by the subgraph labels

    global goblinXZoom goblinYZoom ZipEnabled

    set goblinXZoom 100
    set goblinYZoom 100
    set ZipEnabled 0

    DEP_OBJECT layout configure \
        -arcColourMode  1  -arcLabelFormat   "#4"  -arcWidthMode   2  \
        -arcWidthMin    1  -arcWidthMax        5  -arcStippleMode 0  \
        -nodeSize     100  -nodeLabelFormat   ""  -nodeColourMode 0  \
        -nodeShapeMode  1  -arcVisibilityMode  4  -bendSpacing    5

    ExportAndZip DEP_OBJECT "$testOutput.gif" gif bitmap

    DEP_OBJECT delete

    return 0
}
