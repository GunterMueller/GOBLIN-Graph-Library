
# Unit test for the staircase drawing code

set testTargetFilters \
    [list [file join planar regular "*.gob"] ]

set testTargetFormat "gob"

set listOfTestSteps {
    "Disturb embedding" DisturbEmbedding        \
    "Test planarity"    TestPlanarity           \
    "Convex drawing"    TestStaircaseDrawing    \
    "Export to GIF"     TestGifExport           \
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

proc TestStaircaseDrawing {testObject testOutput} {
    global goblinMethOrthoRefine
    set goblinMethOrthoRefine 4

    $testObject facetSeparation -turnLeft DEP_OBJECT
    DEP_OBJECT layout orthogonal -planar -spacing 10

    return 0
}

proc TestGifExport {testObject testOutput} {
    global goblinXZoom goblinYZoom ZipEnabled

    DEP_OBJECT layout configure \
        -nodeColourMode 2 -arcColourMode 0
    set goblinXZoom 100
    set goblinYZoom 100
    set ZipEnabled 0

    ExportAndZip DEP_OBJECT "$testOutput.rotated.gif" gif bitmap

    DEP_OBJECT delete

    return 0
}
