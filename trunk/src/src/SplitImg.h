#ifndef EXTREES_H_
#define EXTREES_H_

/*
 * From PhylLib:
 */
#include <Phyl/Tree.h>
#include <Phyl/TreeTemplate.h>
#include <Phyl/TreeTools.h>
#include <Phyl/Newick.h>
#include <Phyl/TreeTools.h>
#include <Phyl/TreeTemplateTools.h>
#include <Utils/Number.h>
#include <Phyl/Node.h>


/*
 * All Bio++ functions are also in a namespace, so we'll use it:
 */
using namespace bpp;


void collapseEdge(TreeTemplate<Node>  & tree,Node * on) {
	 Node * temp = (* on).getFather();
	 Node * newNode;
	 unsigned i_max = (* on).getNumberOfSons();           
	 for (unsigned i=0; i< i_max; i++)
	 {
		 newNode=(* on).getSon(0);   // we take always the first son...it's always a different one
		 (* on).removeSon(*((* on).getSon(0)));
		 temp->addSon( * newNode);
	 }
	 (* temp).removeSon( *on);
	 if((* temp).getNumberOfSons()==1 && (((* temp).hasFather())))   {  //degree ==2... not so good! we have to collapse again
	 	collapseEdge(tree, temp);
	 	if(((* temp).getSon(0))->hasDistanceToFather() && ( temp)->hasDistanceToFather())
	 		(* (* temp).getSon(0)).setDistanceToFather(((* temp).getSon(0))->getDistanceToFather() + ( temp)->getDistanceToFather());
	 }
 };


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

#endif /*EXTREES_H_*/
