
# Unit test for the visibility representation method applied to graphs with maximum degree 3

set testTargetFilters \
    [list [file join planar deg4 "*.gob"] \
          [file join planar deg3 "*.gob"] \
          [file join planar regular "*.gob"] \
          [file join planar deg_high "*.gob"] ]

set testTargetFormat "gob"

set listOfTestSteps {
    "Disturb embedding"         DisturbEmbedding                \
    "Test planarity"            TestPlanarity                   \
    "4-Planar drawing"          TestSmallNodeDrawing            \
    "Get Width"                 GetWidth                        \
    "Get Height"                GetHeight                       \
    "Export to GIF"             TestGifExport                   \
}

proc DisturbEmbedding {testObject testOutput} {
    $testObject reorder incidences -shuffle

    return 0
}

proc TestPlanarity {testObject testOutput} {
    global goblinMethPlanarity
    set goblinMethPlanarity 1

    return [$testObject reorder incidences -planar]
}

proc TestSmallNodeDrawing {testObject testOutput} {
    global goblinMethOrthoRefine
    set goblinMethOrthoRefine 4

    $testObject facetSeparation DEP_OBJECT
    DEP_OBJECT layout orthogonal -small -spacing 10

    return 0
}

proc GetWidth {testObject testOutput} {
    return [expr [DEP_OBJECT layout boundingBox info -coordinate 0 -gridSize] -2]
}

proc GetHeight {testObject testOutput} {
    return [expr [DEP_OBJECT layout boundingBox info -coordinate 1 -gridSize] -2]
}

proc TestGifExport {testObject testOutput} {
    global goblinXZoom goblinYZoom ZipEnabled

    DEP_OBJECT layout configure -nodeColourMode 2 -arcColourMode 0
    set goblinXZoom 100
    set goblinYZoom 100
    set ZipEnabled 0

    ExportAndZip DEP_OBJECT "$testOutput.separated.gif" gif bitmap

    DEP_OBJECT delete

    return 0
}
