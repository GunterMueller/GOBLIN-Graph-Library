
# Unit test for the construction of forbidden minors in non-series-parallel graphs

set testTargetFilters \
    [list [file join planar not_series_parallel "*.gob"] \
          [file join planar not_outerplanar     "outerplanarN*.gob"] ]

set testTargetFormat "gob"

set listOfTestSteps {
    "Disturb embedding"             DisturbEmbedding        \
    "Series-parallel recognition"   TestSeriesParallelity   \
    "Export to GIF"                 TestGifExport           \
}

proc DisturbEmbedding {testObject testOutput} {
    $testObject reorder incidences -shuffle

    return 0
}

proc TestSeriesParallelity {testObject testOutput} {
    $testObject layout configure -arcColourMode 1

    set state [catch {$testObject seriesParallel -embedding -layout -minor \
        -source [$testObject source] -target [$testObject target] }]

    return $state
}

proc TestGifExport {testObject testOutput} {
    ExportAndZip $testObject "$testOutput.gif" gif bitmap

    return 0
}
