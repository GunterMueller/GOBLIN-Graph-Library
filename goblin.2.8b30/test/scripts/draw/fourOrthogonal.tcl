
# Unit test for the Kandinsky layout tool

set testTargetFilters \
    [list [file join planar not_planar "deg_*_3_conn*.gob"] \
          [file join planar not_planar "deg_*_4_conn*.gob"] ]

set testTargetFormat "gob"

set listOfTestSteps {
    "4-Orthogonal Drawing"      Test4OrthogonalDrawing      \
    "Get Width"                 GetWidth                    \
    "Get Height"                GetHeight                   \
    "Export to GIF"             TestGifExport               \
}

proc Test4OrthogonalDrawing {testObject testOutput} {
    global goblinMethOrthoRefine
    set goblinMethOrthoRefine 4

    $testObject layout orthogonal -small -spacing 10

    return 0
}

proc GetWidth {testObject testOutput} {
    return [expr [$testObject layout boundingBox info -coordinate 0 -gridSize] -2]
}

proc GetHeight {testObject testOutput} {
    return [expr [$testObject layout boundingBox info -coordinate 1 -gridSize] -2]
}

proc TestGifExport {testObject testOutput} {
    ExportAndZip $testObject "$testOutput.gif" gif bitmap

    return 0
}
