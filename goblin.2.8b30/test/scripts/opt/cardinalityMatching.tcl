
# Unit test for the cardinality matching solver

set testTargetFilters \
    [list [file join planar tiles "*.gob"] ]

set testTargetFormat "gob"

set listOfTestSteps {
    "Instance generation"   TestInstanceGeneration  \
    "#Nodes"                GetNNodes               \
    "Cardinality matching"  TestCardinalityMatching \
    "Export to GIF"         TestGifExport           \
}

proc TestInstanceGeneration {testObject testOutput} {
    goblin openGrid -rows 10 -columns 17 -square DEP_OBJECT
    $testObject reorder incidences -geometric
    DEP_OBJECT merge -facets [$testObject handle]
    DEP_OBJECT configure -nodeDemand 3 -upperBound 1

    return 1
}

proc GetNNodes {testObject testOutput} {
    return [DEP_OBJECT #nodes]
}

proc TestCardinalityMatching {testObject testOutput} {
    global goblinMethMXF goblinMethMaxBalFlow goblinMethBNS

    set goblinMethMXF 1
    set goblinMethMaxBalFlow 5
    set goblinMethBNS 0
    DEP_OBJECT maximumMatching

    return [DEP_OBJECT info -cardinality]
}

proc TestGifExport {testObject testOutput} {
    # This exports the minimum cost perfect matching provided by the subgraph labels,
    # and the Gallai-Edmonds partition given by the node colours

    global goblinXZoom goblinYZoom ZipEnabled

    set goblinXZoom 100
    set goblinYZoom 100
    set ZipEnabled 0

    DEP_OBJECT layout configure \
        -arcColourMode  0  -arcLabelFormat  ""  -arcWidthMode   2  \
        -arcWidthMin    1  -arcWidthMax      5  -arcStippleMode 5  \
        -nodeSize     100  -nodeLabelFormat ""  -nodeColourMode 2  \
        -nodeShapeMode  1  -bendSpacing      5

    ExportAndZip DEP_OBJECT "$testOutput.gif" gif bitmap

    DEP_OBJECT delete

    return 0
}
