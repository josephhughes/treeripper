TreeRipper is a command line c++ program to fully automate the recognition of multifurcating phylogenetic trees. The program accepts an image (jpg, pdf, tiff and others) as an input and performs a number of cleaning up steps on the image to detect lines, remove node labels,  patch-up broken lines and corners and detect line edges. The edge contour is then determined to detect the branch tips and the topology of the tree. Optical Character Recognition (OCR) using tesseract-ocr is then used to determine the tip labels.

**Reference:**

_If using any of the data from here, I would appreciate it if you cited the following_

Hughes, J. (2010). TreeRipper web application: towards a fully automated optical tree recognition software. BMC Bioinformatics [doi:10.1186/1471-2105-12-178](http://www.biomedcentral.com/1471-2105/12/178)