# Introduction #

TreeRipper is a command line c++ program for the fully-automated recognition of multifurcating phylogenetic trees.


# Prerequisites #

The current version of TreeRipper opens tree-image files in the formats PNG, JPG/JPEG, GIF, TIFF or PDF.
  * The tree needs to have the root on the left and leaves on the right.
  * Horizontal branches.
  * The tree constitutes a dark foreground on a light homogenous background (no background boxes or shading).
  * The tree must be bi- or multifurcating (not a network)
  * The inner nodes are branching points between lines and have no circles, rectangles, etc. inscribed.
  * Tip leaves must have branch lengths greater than 0.