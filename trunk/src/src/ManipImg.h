#ifndef MANIPIMG_H_
#define MANIPIMG_H_

/*
 * From ImageMagick:
 */
#include <Magick++.h>

#include <iostream>
/*
 * Takes an image object and splits it by detecting white lines of at least 5px that span
 * left to right or top to bottom accross the image. Assumes that the largest image is a phylogeny.
 * Returns the largest image as an image object.
 */

Image splitImg(original) {
  int cols= original.columns();
  int rows= original.rows();
  cout << "Image width: " << cols << endl;
  cout << "Image length: " << rows << endl;
 };

/*
void collapseOnTree(TreeTemplate<Node>  & tree, float value) {
	vector<Node *> innerNodes = tree.getInnerNodes();
    for (unsigned i=0;i<innerNodes.size(); i++)
    {
       	if ((innerNodes[i])->hasFather() && ((innerNodes[i])->hasBranchProperty(TreeTools::BOOTSTRAP)) && (dynamic_cast<const Number<double> *>((innerNodes[i])->getBranchProperty(TreeTools::BOOTSTRAP))->getValue()<value))
       	{
        	collapseEdge(tree, innerNodes[i]);
        }
    }
}
*/

#endif /*MANIPIMG_H_*/
