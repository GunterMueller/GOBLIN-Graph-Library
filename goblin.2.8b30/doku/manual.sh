#!/bin/sh

cd doku;

if (! test -e manual.dvi) then
    latex manual.tex;
    latex manual.tex;
fi

latex manual.tex;
makeindex -q manual.idx;
dvips -q -o ../mgoblin.$1.ps manual;
dvipdfm -l -o ../mgoblin.$1.pdf manual.dvi

cd ..
