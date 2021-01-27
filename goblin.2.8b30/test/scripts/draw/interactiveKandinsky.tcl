
# Unit test for the Kandinsky layout tool

set testTargetFilters \
    [list [file join planar deg3 "*.gob"] \
          [file join planar deg4 "*.gob"] \
          [file join planar deg_high "*.gob"] ]

set testTargetFormat "gob"

set listOfTestSteps {
    "Combinatorial Embedding"   TestCombinatorialEmbedding  \
    "Kandinski Drawing"         TestKandinskyDrawing        \
    "Export to GIF"             TestGifExport               \
}

proc TestCombinatorialEmbedding {testObject testOutput} {
    return [$testObject reorder incidences -planar]
}

proc TestKandinskyDrawing {testObject testOutput} {
    global goblinMethOrthoRefine
    set goblinMethOrthoRefine 4

    $testObject layout orthogonal -spacing 10

    return 0
}

proc TestGifExport {testObject testOutput} {
    ExportAndZip $testObject "$testOutput.gif" gif bitmap

    return 0
}
