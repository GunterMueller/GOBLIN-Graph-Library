
# Unit test for the planar combinatorial embedding codes,
# in particular the export of K_5 and K_3,3 minors

set testTargetFilters \
    [list [file join planar not_planar "*.gob"] ]

set testTargetFormat "gob"

set listOfTestSteps {
    "Disturb embedding" DisturbEmbedding    \
    "Test planarity"    TestPlanarity       \
    "Export to GIF"     TestGifExport       \
}

proc DisturbEmbedding {testObject testOutput} {
    $testObject reorder incidences -shuffle

    return 0
}

proc TestPlanarity {testObject testOutput} {
    global goblinMethPlanarity
    set goblinMethPlanarity 1

    $testObject layout configure -arcColourMode 1

    return [$testObject info -planar]
}

proc TestGifExport {testObject testOutput} {
    global goblinXZoom goblinYZoom ZipEnabled

    set goblinXZoom 100
    set goblinYZoom 100
    set ZipEnabled 0

    $testObject layout orthogonal -spacing 10

    ExportAndZip $testObject "$testOutput.gif" gif bitmap

    return 0
}
