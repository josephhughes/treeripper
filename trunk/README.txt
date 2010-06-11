INSTALLATION

TreeRipper has a number of dependencies. First you will need to install the Magick++ library. 
Magick++ is included as part of ImageMagick source releases and may be retrieved here:
http://www.imagemagick.org/script/download.php

Secondly, you will need tesseract-ocr which can be found here:
http://code.google.com/p/tesseract-ocr/

On Mac OS X, tesseract depends on libpng which itself needs zlib to compile. You will find them here:
http://www.libpng.org/pub/png/libpng.html
http://www.zlib.net/

Finally, to compile TreeRipper:
c++ -o TreeRipper TreeRipper.cpp `Magick++-config --cppflags --cxxflags --ldflags --libs

On Windows, matters are made slightly easier as Windows executables exist for tesseract. 