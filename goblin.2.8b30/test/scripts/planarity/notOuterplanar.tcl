
# Unit test for the construction of forbidden minors in non-outerplanar graphs

set testTargetFilters \
    [list [file join planar not_outerplanar "*.gob"] ]

set testTargetFormat "gob"

set listOfTestSteps {
    "Outerplanar recognition"       TestOuterplanarity   \
    "Export to GIF"                 TestGifExport           \
}

proc TestOuterplanarity {testObject testOutput} {
    $testObject layout configure -arcColourMode 1
    $testObject reorder incidences -outerplanar

    return 0
}

proc TestGifExport {testObject testOutput} {
    ExportAndZip $testObject "$testOutput.gif" gif bitmap

    return 0
}
