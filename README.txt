INSTALLATION

TreeRipper has a number of dependencies. First you will need to install the Magick++ library. 
Magick++ is included as part of ImageMagick source releases and may be retrieved here:
http://www.imagemagick.org/script/download.php
It is recommended to install ImageMagick for MacosX using Macport as it installs all the 
delgate libraries.

Secondly, you will need tesseract-ocr which can be found here:
http://code.google.com/p/tesseract-ocr/

On Mac OS X, tesseract depends on libpng which itself needs zlib to compile. You will find them here:
http://www.libpng.org/pub/png/libpng.html
http://www.zlib.net/

Finally, to compile TreeRipper:
cd src
make

To use TreeRipper:
treeripper inputimage.jpg outputtree.nex

On Windows, matters are made slightly easier as Windows executables exist for tesseract. 


Further information about the program can be found here:
http://josephhughes.github.io/treeripper
