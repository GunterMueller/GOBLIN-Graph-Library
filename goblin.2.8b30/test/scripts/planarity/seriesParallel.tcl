
# Unit test for the recognition, embedding and drawing of series-parallel graphs

set testTargetFilters \
    [list [file join planar series_parallel "*.gob"] ]

set testTargetFormat "gob"

set listOfTestSteps {
    "Disturb embedding"         DisturbEmbedding        \
    "Series-parallel drawing"   TestSeriesParallelity   \
    "Export to GIF"             TestGifExport           \
    "Dual graph construction"   TestDuality             \
}

proc DisturbEmbedding {testObject testOutput} {
    $testObject reorder incidences -shuffle

    return 0
}

proc TestSeriesParallelity {testObject testOutput} {
    $testObject seriesParallel -embedding -layout

    return 0
}

proc TestGifExport {testObject testOutput} {
    global goblinXZoom goblinYZoom

    set goblinXZoom 100
    set goblinYZoom 100

    ExportAndZip $testObject "$testOutput.gif" gif bitmap

    return 0
}

proc TestDuality {testObject testOutput} {
    $testObject dualGraph DEP_OBJECT
    DEP_OBJECT delete

    return 0
}
