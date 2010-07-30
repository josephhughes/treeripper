/*
 * treeripper
 * A program for converting images of phylogenies into  tree files.
 * Copyright (C) 2010 Joseph Hughes <Joseph.Hughes@glasgow.ac.uk>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307, USA.
 */
 //c++ -o TreeRipper TreeRipper.cpp `Magick++-config --cppflags --cxxflags --ldflags --libs`

#include <Magick++.h>
#include "ntree.h"
#include "profile.h"

#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <sstream>
#include <time.h>
#include <vector>

#define FILENAME_SIZE 256 // maximum file name length
/**
 * @var  vector <NTree> NTreeVector
 * @brief A vector of NTree's, used to store the input trees
 *
 */
typedef vector <NTree> NTreeVector;


using namespace std;
using namespace Magick;
int main(int argc,char **argv) {
	if (argc<3){
		std::cout << "Usage: mprog.exe <input filename> <output filename>" << std::endl;
		return 1;
	}
    // get options from command line
    char fname[FILENAME_SIZE];
    strcpy(fname,argv[1]);
    char ofname[FILENAME_SIZE];
    strcpy(ofname,argv[2]);
    // check file exists
    FILE* file=fopen(fname, "r");
    if (file !=NULL){
      fclose(file);
      file=NULL;
    }else{
      cerr << "File \"" << fname << "\" does not exist." << endl;
      exit(0);
    }
    std::ofstream of(ofname, std::ios::binary|std::ios::out);

	try {
		time_t start,end,begin;
		time (&begin);
        Image master(fname);// IpsPage09.jpg ImageColor.jpg LandPlant4.jpg
        Image second = master;
		std::ofstream log("./tmp.log", std::ios::binary|std::ios::out);        
		// get size of image
        int cols= second.columns();
      	int rows= second.rows();
      	log << "Conversion of image to black and white ..." << endl;
//		cout << "Image width: " << cols << endl;
//		cout << "Image length: " << rows << endl;
		// convert to black and white
 		time (&start);
      	second.quantizeColorSpace( GRAYColorspace );
        second.quantizeColors( 2 );
      	second.quantizeDither( false );
      	second.quantize( );
		//second.write("ImageB&W.jpg"); //<<<<<<<<<<<<<<<<<<<<<<<<
		time (&end);
  		double dif = difftime (end,start);
  		printf ("It took %.2lf seconds to convert the image to Black and White.\n", dif );
  		
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//		
		//determine the most common line thickness									//
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//		
		string pixel_sample;
		int slice_location = 0.5 * cols;
		//cout << "Position of the slice " << slice_location << endl;
		
		//checking the line thickness and putting it in an array data[]
		time (&start);
		log << "Determining line thickness of branches ..." << endl;
		int line_thickness = 0;
		string black = "#000000000000"; 
		string white = "#FFFFFFFFFFFF";
		int counter;
		int no_lines = 0;
		list<int> lines;
  		list<int>::iterator p;
  		list<int>::iterator u;
		for (counter = 0; counter < rows; ++counter) {
			pixel_sample = second.pixelColor(slice_location,counter);
			//cout << pixel_sample << endl;
			if ( black.compare(pixel_sample) == 0 ) {
				++line_thickness;
			} if ( white.compare(pixel_sample) == 0 && line_thickness>0) {
				//cout << line_thickness << endl;
				lines.push_back( line_thickness );
				++no_lines;
				line_thickness = 0;
			}	
		}
		//cout << "Orignal list of line thicknesses: ";
  		//for (p = lines.begin(); p != lines.end(); p++){
    	//	cout << *p << " ";
  		//	cout << endl;
  		//}
		lines.sort();
		list<int> unique_lines;
		unique_lines = lines;
		unique_lines.unique(); // remove consecutive duplicates
		int highest_count = 0;
		int thickest_line = 0;
		//count the number of times a unique line thickness is found
  		//cout << "Modified list of line thicknesses (with duplicates removed): ";
  		for(u = unique_lines.begin(); u != unique_lines.end(); u++){
    		//cout << *u << " ";
 			counter = 0;
 			//count the number of times an element from u is found in p (the original list)
  			for (p = lines.begin(); p != lines.end(); p++){
  				if (*u == *p){
  					++counter;
  				}
  			}
  			//cout << counter << endl;
  			if (counter > highest_count){
  				highest_count = counter;
  				thickest_line = *u;
  			}
  				
  		}
//		cout << "The thickest line of " << thickest_line << " pixels is found " << highest_count << " times" << endl;
		log << "Conversion of image size in progress ..." << endl;
	//	if (thickest_line > 2){
			int scaling_parameter = (2 * 100) / thickest_line; //scaling down the line thickness to 2 pixels using the most commonly found line thickness
			//cout << scaling_parameter << endl; 
			ostringstream scaling_proportion;
			scaling_proportion << scaling_parameter << "%"; //concatenate string
//			cout << scaling_proportion.str() << endl; 
			second.scale( scaling_proportion.str() );
		
			//convert image to black and white   
   			second.quantizeColorSpace( GRAYColorspace );
        	second.quantizeColors( 2 );
      		second.quantizeDither( false );
      		second.quantize( );
   //   	}
		//second.write("ImageScaled.txt");
		
		
		time (&end);
  		dif = difftime (end,start);
  		printf ("It took %.2lf seconds to rescale the image and convert the image to Black and White.\n", dif );

		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//	
		//<<get horizontal lines of 8pix and vertical lines of 6 pix into an array<<//
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//			
		time (&start);
		int orig_scaled_cols= second.columns();
      	int orig_scaled_rows= second.rows();
      	int larger_scols=orig_scaled_cols+10;
      	int larger_srows=orig_scaled_rows+10;
      	Image white_edge(Geometry (larger_scols,larger_srows), Color("white"));
      	white_edge.composite(second, 0, 0, OverCompositeOp);
      	second = white_edge;
      	int scaled_cols= second.columns();
      	int scaled_rows= second.rows();

//		cout << "Image width after scaling: " << scaled_cols << endl;
//		cout << "Image length after scaling: " << scaled_rows << endl;
		//don't need to write it into an image, just write into a 2d vector (easier than 2d array)
		std::ofstream out_file("out.dat", std::ios::binary|std::ios::out|std::ios::app);
		vector<vector<int> > img (scaled_cols, vector<int> ( scaled_rows ) );
		vector<vector<int> > new_img (scaled_cols, vector<int> ( scaled_rows ) );
  		for ( int y = 0; y < scaled_rows; y++ ) {
    		for ( int x = 0; x < scaled_cols; x++ ){
    			img[x][y] = 0;
    			new_img[x][y] = 0;
    			pixel_sample = second.pixelColor(x,y);
    			if ( black.compare(pixel_sample) == 0 ){
    				img[x][y] = 1;
    			}
    		}
  		}
  		//write out to text file
  		/*for ( int y = 0; y < scaled_rows; y++ ) {
    		for ( int x = 0; x < scaled_cols; x++ ){
      			out_file << img[x][y] ;
    			
  			}
  			out_file <<'\n';
		}*/
  		log << "Detection of horizontal lines in progress ..." << endl;
  		//only putting horizontql lines of length 8pix into the 2d vector
		int xcoord=0;
		int ycoord=0;
		for (ycoord = 0;  ycoord < scaled_rows; ++ycoord){
			for (xcoord = 0;  xcoord < (scaled_cols-8); ++xcoord){
				int line_score = 0;
				for (int i=0; i<8; ++i){
					line_score = line_score + img[xcoord+i][ycoord];
					//cout << "line score " << line_score << endl;
					
				}
				if (line_score == 8){
					int j=9;
					while (img[xcoord+j][ycoord] == 1 && (xcoord+j)<scaled_cols){
						++j;
						//cout << "repetition in while loop " << j << endl;
					}
					//cout << "Line from " << xcoord << " x " << ycoord << " of " << j << " length " << endl;
					for (int n=0; n<j ; ++n){
						new_img[xcoord+n][ycoord] = 1;
					}
					xcoord = xcoord + j;
				}
				
			}
		}
		log << "Detection of vertical lines in progress ..." << endl;
		//only putting vertical lines of length 6pix into the 2d vector
		for (xcoord = 0;  xcoord < scaled_cols; ++xcoord){
			for (ycoord = 0;  ycoord < (scaled_rows-6); ++ycoord){
				int line_score = 0;
				for (int i=0; i<6; ++i){
					line_score = line_score + img[xcoord][ycoord+i];
					//cout << "line score " << line_score << endl;
				}
				if (line_score == 6){
					int j=7;
					while (img[xcoord][ycoord+j] == 1 && (ycoord+j)<scaled_rows){
						++j;
						//cout << "repetition in while loop " << j << endl;
					}
					//cout << "Line from " << xcoord << " x " << ycoord << " of " << j << " length " << endl;
					for (int n=0; n<j ; ++n){
						new_img[xcoord][ycoord+n] = 1;
					}
					ycoord = ycoord + j;
				}
				
			}
		}

		//print out of image to txt file to check all is OK		
		/*for (ycoord = 0;  ycoord < scaled_rows; ++ycoord){
			for (xcoord = 0;  xcoord < scaled_cols; ++xcoord){
				out_file << new_img[xcoord][ycoord]	;
			}
			out_file << endl;
		}*/
		time (&end);
  		dif = difftime (end,start);
  		printf ("It took %.2lf seconds to get line fragments and store them in new 2d vector.\n", dif );

		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
		//>>>>>>>>>removing black pixels surrounded by a white box from the array   //
		//>>>>>>>>>start with a box of 62 pix                                       //
		//>>>>>>>>>and decrease the size of the box by half each iteration          //
		//>>>>>>>>>stop when the box is 3 pix width                                 //
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
		time (&start);
		log << "Cleaning image. Step 1 ..." << endl;
		int box_size = 62 ; //whys 62? it worked, larger numbers gave a segmentation fault
		for (box_size = 62; box_size > 3; box_size = box_size/2){
//			cout << "box size " << box_size << endl;
			for (ycoord = 0;  ycoord < (scaled_rows-62); ++ycoord){
				for (xcoord = 0;  xcoord < (scaled_cols-62); ++xcoord){
					int box_edge_score = 0;
					for (int i=0; i<=box_size; ++i){
						box_edge_score = box_edge_score + new_img[xcoord][ycoord+i] + new_img[xcoord+i][ycoord] + new_img[xcoord+box_size][ycoord+i] + new_img[xcoord+i][ycoord+box_size];
					}
					//if (box_edge_score > 0){
					//	cout << xcoord << " x " << ycoord << " : " << box_edge_score << endl;
					//}
					if (box_edge_score == 0){
						//cout << xcoord << " x " << ycoord << " : " << box_edge_score << endl;
						for (int i=0; i<=box_size; ++i){
							for (int j=0; j<=box_size; ++j){
								new_img[xcoord+j][ycoord+i] = 0;
							}
						}
					}
				}
			}
			//box_size = box_size/4;
		}
	/*	
		//print out the image in jpg format
		Image box_img = second;		
		for (ycoord = 0;  ycoord < scaled_rows; ++ycoord){ //no pixel 0,0
			for (xcoord = 0;  xcoord < scaled_cols; ++xcoord){
				//out_file << new_img[xcoord][ycoord]	;
				if (new_img[xcoord][ycoord]==1){
					//cout << xcoord << "  " << ycoord << endl;
					box_img.pixelColor(xcoord,ycoord,"black");
				}else {
					box_img.pixelColor(xcoord,ycoord,"white");
				}
			}
			//out_file << endl;
		}
		box_img.write("BoxImage.jpg");
  	*/	
		time (&end);
  		dif = difftime (end,start);
  		printf ("It took %.2lf seconds to remove black pixels surrounded by a white box.\n", dif );

		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
		//>>>>>>>>>>>>>>>>>>>>>>>>removing lines floating around etc....		
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
		
		time (&start);
		log << "Cleaning image. Step 2 ..." << endl;
		//remove line that is stuck to y axis (this is to be able to do the edge to bracket conversion later)
		for (ycoord = 0;  ycoord < (scaled_rows-7); ++ycoord){
			if (new_img[0][ycoord]==1){
				new_img[0][ycoord]=0;
			}
		}
		//scanning right and downwards for patterns
		for (ycoord = 0;  ycoord < (scaled_rows-10); ++ycoord){
			for (xcoord = 0;  xcoord < (scaled_cols-10); ++xcoord){
				//score of each vertical and horizontal line 
				int h1 = 0;
				int h2 = 0;
				int h3 = 0;
				int h4 = 0;
				int h5 = 0;
				int h6 = 0;
				int h7 = 0;
				int h8 = 0;
				int h9 = 0;
				int h10 = 0;
				int v1 = 0;
				int v2 = 0;
				int v3 = 0;
				int v4 = 0;
				int v5 = 0;
				int v6 = 0;
				int v7 = 0;
				int v8 = 0;
				int v9 = 0;
				int v10 = 0;
				
				for (int i=0; i<10; ++i){
					h1 = h1 + new_img[xcoord+i][ycoord];
					h2 = h2 + new_img[xcoord+i][ycoord+1];
					h3 = h3 + new_img[xcoord+i][ycoord+2];
					h4 = h4 + new_img[xcoord+i][ycoord+3];
					h5 = h5 + new_img[xcoord+i][ycoord+4];
					h6 = h6 + new_img[xcoord+i][ycoord+5];
					h7 = h7 + new_img[xcoord+i][ycoord+6];
					h8 = h8 + new_img[xcoord+i][ycoord+7];
					h9 = h9 + new_img[xcoord+i][ycoord+8];
					h10 = h10 + new_img[xcoord+i][ycoord+9];

				}
				for (int j=0; j<10; ++j){
					v1 = v1 + new_img[xcoord][ycoord+j];
					v2 = v2 + new_img[xcoord+1][ycoord+j];
					v3 = v3 + new_img[xcoord+2][ycoord+j];
					v4 = v4 + new_img[xcoord+3][ycoord+j];
					v5 = v5 + new_img[xcoord+4][ycoord+j];
					v6 = v6 + new_img[xcoord+5][ycoord+j];
					v7 = v7 + new_img[xcoord+6][ycoord+j];
					v8 = v8 + new_img[xcoord+7][ycoord+j];
					v9 = v9 + new_img[xcoord+8][ycoord+j];
					v10 = v10 + new_img[xcoord+9][ycoord+j];

					}
				//for vertical lines of 8pix 
				if (h1==0 && v1==0 && v10==0){
					for (int i=0; i<10; ++i){
						for (int j=0; j<10; ++j){
							new_img[xcoord+i][ycoord+j] = 0;
						}
					}
				}
				// removing horizontal lines of 8pix 
				if (v1==0 && h1==0 && h10==0){
					for (int i=0; i<10; ++i){
						for (int j=0; j<10; ++j){
							new_img[xcoord+i][ycoord+j] = 0;
						}
					}
				}
				//removing pixels from corners and other small protruding pixels
				stringstream s1;
				for (int j=0; j<3; ++j){
					for (int i=0; i<3; ++i){
						s1 << new_img[xcoord+i][ycoord+j];
					}
				}
				string img_pattern=s1.str();
				//removing 1 pixel corners
				string pattern[] = {"000011001", "000110010", "010110000", "010011000", "000011010", "000000010", "000011000" }; // pimple sticking up 000000010
				for (int p=0; p<7; ++p){
					if (img_pattern.compare(pattern[p]) == 0){
						//cout << "corner match " << img_pattern << endl; 
						for (int j=0; j<3; ++j){
							for (int i=0; i<3; ++i){
								new_img[xcoord+i][ycoord+j] = 0;
							}
						}
					}
				}
				//removing extra pixel from corner (x4 ways)
				string corner_pix = "100110111";
				if (img_pattern.compare(corner_pix) == 0){
					//cout << "corner match " << img_pattern << endl; 
					for (int j=0; j<2; ++j){
						for (int i=1; i<3; ++i){
							new_img[xcoord+i][ycoord+j] = 0;
						}
					}
				}
				
				corner_pix = "111011001";
				if (img_pattern.compare(corner_pix) == 0){
//					cout << "corner match " << img_pattern << " " << xcoord << " " << ycoord << endl; 
					new_img[xcoord+1][ycoord+1] = 0;
				}
				
				corner_pix = "111110100";
				if (img_pattern.compare(corner_pix) == 0){
					//cout << "corner match " << img_pattern << endl; 
					for (int j=1; j<3; ++j){
						for (int i=1; i<3; ++i){
							new_img[xcoord+i][ycoord+j] = 0;
						}
					}
				}
				corner_pix = "001011111";
				if (img_pattern.compare(corner_pix) == 0){
					//cout << "corner match " << img_pattern << endl; 
					for (int j=0; j<2; ++j){
						for (int i=0; i<2; ++i){
							new_img[xcoord+i][ycoord+j] = 0;
						}
					}
				}
				//removing pimples
				stringstream pimple;
				for (int j=0; j<5; ++j){
					for (int i=0; i<3; ++i){
						pimple << new_img[xcoord+i][ycoord+j];
					}
				}
				string pimple_pattern=pimple.str();
				string pimple_pix[] = {"100100110100100", "100110110110100", "100100110110100", "100110110100100"}; 
				for (int p=0; p<4; ++p){
					if (pimple_pattern.compare(pimple_pix[p]) == 0){
						//cout << "pimple match " << pimple_pattern << endl; 
						for (int j=0; j<5; ++j){
							for (int i=1; i<3; ++i){
								new_img[xcoord+i][ycoord+j] = 0;
							}
						}
					}
				}	
				//scanning down for vertical block stuck to a line
				stringstream block;
				for (int i=0; i<6; ++i){
					block << new_img[xcoord+i][ycoord];
				}
				for (int j=1; j<3 ; ++j){
					block << new_img[xcoord][ycoord+j];
					block << new_img[xcoord+4][ycoord+j];
					block << new_img[xcoord+5][ycoord+j];
				}
				string vertblock_pattern=block.str();
				string vertblock[] = {"000001011011", "100000110110"}; 
				for (int p=0; p<2; ++p){
					if (vertblock_pattern.compare(vertblock[p]) == 0){
						//cout << "vertical block going down " << vertblock_pattern << endl; 
						for (int j=0; j<3; ++j){
							for (int i=1; i<5; ++i){
								new_img[xcoord+i][ycoord+j] = 0;
							}
						}
					}
				}
				string top_right_corner[] = {"011111110", "001001110", "011011110"}; 
				for (int p=0; p<3; ++p){
					if (img_pattern.compare(top_right_corner[p]) == 0){
						for (int j=0; j<2; ++j){
							for (int i=0; i<2; ++i){
								new_img[xcoord+i][ycoord+j] = 1;
							}
						}
					}
				}
				//2 pix and 1 pix corner
				string corner_pattern[] = {"110001001", "110111011", "110011011", "100001001", "110111001"}; //added "110001001" and needs to be put 1st
				for (int p=0; p<5; ++p){
					if (img_pattern.compare(corner_pattern[p]) == 0){
						for (int j=1; j<3; ++j){
							for (int i=0; i<2; ++i){
								new_img[xcoord+i][ycoord+j] = 1;
							}
						}
					}
				}
				stringstream corner1pix;
				for (int j=0; j<2; ++j){
					for (int i=0; i<2; ++i){
						corner1pix << new_img[xcoord+i][ycoord+j];
					}
				}
				string corner1pix_string=corner1pix.str();
				string corner1pix_pattern[] = {"1001", "0110"}; 
				for (int p=0; p<2; ++p){
					if (corner1pix_string.compare(corner1pix_pattern[p]) == 0){
						for (int j=0; j<2; ++j){
							new_img[xcoord][ycoord+j] = 1;
						}
					}
				}
				
				//scanning to repair broken lines
				string broken_line_pattern[] = {"000110001", "000110011", "000110111"}; // for extending horizontal lines
				for (int p=0; p<3; ++p){
					if (img_pattern.compare(broken_line_pattern[p]) == 0){
						for (int j=1; j<3; ++j){
							for (int i=0; i<3; ++i){
								new_img[xcoord+i][ycoord+j] = 1;
							}
						}
					}
				}
				string broken_both_sides[] = {"001110000", "011110000", "111110000", "111110000"}; 
				// 111\;110\;000 and ^111\;110\;000\; for extending horizontal lines
				for (int p=0; p<4; ++p){
					if (img_pattern.compare(broken_both_sides[p]) == 0){
						for (int j=0; j<2; ++j){
							for (int i=0; i<3; ++i){
								new_img[xcoord+i][ycoord+j] = 1;
							}
						}
					}
				}
				string brokencorner = "100001001";
				if (img_pattern.compare(brokencorner) == 0){
					new_img[xcoord+1][ycoord+1] = 1;
				}
				string verybrokencorner = "100000001";
				if (img_pattern.compare(verybrokencorner) == 0){
					for (int j=0; j<3; ++j){
						new_img[xcoord][ycoord+j] = 1;
					}
					for (int i=0; i<3; ++i){
						new_img[xcoord+i][ycoord+2] = 1;
					}
				}
				stringstream s;
				for (int j=0; j<4; ++j){
					for (int i=0; i<3; ++i){
						s << new_img[xcoord+i][ycoord+j];
					}
				}
				string s_pattern=s.str();
				string extrightdown[] = {"000110110111", "000110111111", "111110110000", "111111110000"}; 
				for (int p=0; p<4; ++p){
					if (s_pattern.compare(extrightdown[p]) == 0){
						for (int j=1; j<3; ++j){
							for (int i=0; i<3; ++i){
								new_img[xcoord+i][ycoord+j] = 1;
							}
						}
					}
				}
				//scanning down and right for extending vertical lines
				//scanning down to extend vertical lines
				stringstream vertext;
				for (int i=0; i<3; ++i){
					for (int j=0; j<3; ++j){
						vertext << new_img[xcoord+i][ycoord+j];
					}
				}
				string vert_pattern=vertext.str();
				string extvertdown1[] = {"000110111", "100110111"}; 
				for (int p=0; p<2; p++){
					if (vert_pattern.compare(extvertdown1[p]) == 0){
						for (int i=1; i<3; ++i){
							for (int j=0; j<3; ++j){
								new_img[xcoord+i][ycoord+j] = 1;
							}
						}
					}
				}
				string extvertdown2 = "111110000"; 
				if (vert_pattern.compare(extvertdown2) == 0){
					for (int i=0; i<2; ++i){
						for (int j=0; j<3; ++j){
							new_img[xcoord+i][ycoord+j] = 1;
						}
					}
				}
				string extvertdown3 = "010011001";
				if (vert_pattern.compare(extvertdown3) == 0){
					for (int j=0; j<3; ++j){
						new_img[xcoord+1][ycoord+j] = 1;
					}
				}
				string extvertdown4 = "110011000";
				if (vert_pattern.compare(extvertdown4) == 0){
					for (int j=0; j<3; ++j){
						new_img[xcoord][ycoord+j] = 1;
						new_img[xcoord+1][ycoord+j] = 1;
					}
				}
				string exthor1[] = {"011011001", "011011010"};
				for (int p=0; p<2; ++p){
					if (vert_pattern.compare(exthor1[p]) == 0){
//						cout << " MATCH Horizontal 1 " << endl;
						for (int i=0; i<3; ++i){
							for (int j=1; j<3; ++j){
								new_img[xcoord+i][ycoord+j] = 1;
							}
						}
					}
				}
				string exthor2[] = {"110110100", "110110010"};
				for (int p=0; p<2; ++p){
					if (vert_pattern.compare(exthor2[p]) == 0){
//						cout << " MATCH Horizontal 2 " << endl;
						for (int i=0; i<3; ++i){
							for (int j=0; j<2; ++j){
								new_img[xcoord+i][ycoord+j] = 1;
							}
						}
					}
				}
				//scanning down for vertical lines (4 by 2 pattern)
				stringstream m;
				for (int i=0; i<4; ++i){
					for (int j=0; j<2; ++j){
						m << new_img[xcoord+i][ycoord+j];
					}
				}
				string m_pattern=m.str();
				string extvertdown5[] = {"11101000", "11111000"} ; 
				for (int p=0; p<2; ++p){
					if (m_pattern.compare(extvertdown5[p]) == 0){
						//cout << " MATCH DOWN " << endl;
						for (int i=0; i<3; ++i){
							for (int j=0; j<2; ++j){
								new_img[xcoord+i][ycoord+j] = 1;
							}
						}
					}
				}
				string extvertdown6[] = {"00101011", "00101111"} ;
				for (int p=0; p<2; ++p){
					if (m_pattern.compare(extvertdown6[p]) == 0){
						//cout << " MATCH DOWN On the Left " << endl;
						for (int i=1; i<4; ++i){
							for (int j=0; j<2; ++j){
								new_img[xcoord+i][ycoord+j] = 1;
							}
						}
					}
				}

			}
		}
		log << "Cleaning image. Step 3 ..." << endl;
		//scanning left and upwards for patterns
		for (ycoord = (scaled_rows-10);  ycoord > 0 ; --ycoord){
			for (xcoord = (scaled_cols-10);  xcoord >0; --xcoord){
				//score of each vertical and horizontal line 
				int h1 = 0;
				int h2 = 0;
				int h3 = 0;
				int h4 = 0;
				int h5 = 0;
				int h6 = 0;
				int h7 = 0;
				int h8 = 0;
				int h9 = 0;
				int h10 = 0;
				int v1 = 0;
				int v2 = 0;
				int v3 = 0;
				int v4 = 0;
				int v5 = 0;
				int v6 = 0;
				int v7 = 0;
				int v8 = 0;
				int v9 = 0;
				int v10 = 0;
				
				for (int i=0; i<10; ++i){
					h1 = h1 + new_img[xcoord+i][ycoord];
					h2 = h2 + new_img[xcoord+i][ycoord+1];
					h3 = h3 + new_img[xcoord+i][ycoord+2];
					h4 = h4 + new_img[xcoord+i][ycoord+3];
					h5 = h5 + new_img[xcoord+i][ycoord+4];
					h6 = h6 + new_img[xcoord+i][ycoord+5];
					h7 = h7 + new_img[xcoord+i][ycoord+6];
					h8 = h8 + new_img[xcoord+i][ycoord+7];
					h9 = h9 + new_img[xcoord+i][ycoord+8];
					h10 = h10 + new_img[xcoord+i][ycoord+9];
				}
				for (int j=0; j<10; ++j){
					v1 = v1 + new_img[xcoord][ycoord+j];
					v2 = v2 + new_img[xcoord+1][ycoord+j];
					v3 = v3 + new_img[xcoord+2][ycoord+j];
					v4 = v4 + new_img[xcoord+3][ycoord+j];
					v5 = v5 + new_img[xcoord+4][ycoord+j];
					v6 = v6 + new_img[xcoord+5][ycoord+j];
					v7 = v7 + new_img[xcoord+6][ycoord+j];
					v8 = v8 + new_img[xcoord+7][ycoord+j];
					v9 = v9 + new_img[xcoord+8][ycoord+j];
					v10 = v10 + new_img[xcoord+9][ycoord+j];
				}
				//horizontal lines have already been removed by scanning right and downwards, we can't do it here because of the tips
				//for vertical lines of 12pix or smaller 
				if (h10==0 && v1==0 && v10==0){
					for (int i=0; i<10; ++i){
						for (int j=0; j<10; ++j){
							new_img[xcoord+i][ycoord+j] = 0;
						}
					}
				}
				stringstream block;
				for (int j=0; j<2 ; ++j){
					block << new_img[xcoord][ycoord+j];
					block << new_img[xcoord+4][ycoord+j];
					block << new_img[xcoord+5][ycoord+j];
				}
				for (int i=0; i<6; ++i){
					block << new_img[xcoord+i][ycoord+2];
				}
				string vertblock_pattern=block.str();
				string vertblock[] = {"011011000001", "110110100000", "000011000001"}; //000011000001 this is for the stuck
				for (int p=0; p<3; ++p){
					if (vertblock_pattern.compare(vertblock[p]) == 0){
						//cout << "vertical block going up " << vertblock_pattern << " " << xcoord << " " << ycoord << endl; 
						for (int j=0; j<3; ++j){
							for (int i=1; i<5; ++i){
								new_img[xcoord+i][ycoord+j] = 0;
							}
						}
					}
				}
				stringstream s;
				for (int j=0; j<3; ++j){
					for (int i=0; i<3; ++i){
						s << new_img[xcoord+i][ycoord+j];
					}
				}
				string img_pattern=s.str();
				string extleft1 = "000011111"; 
				if (img_pattern.compare(extleft1) == 0){
					for (int j=1; j<3; ++j){
						for (int i=0; i<3; ++i){
							new_img[xcoord+i][ycoord+j] = 1;
						}
					}
				}
				string extleft2 = "111011000"; 
				if (img_pattern.compare(extleft2) == 0){
					for (int j=0; j<2; ++j){
						for (int i=0; i<3; ++i){
							new_img[xcoord+i][ycoord+j] = 1;
						}
					}
				}
				string extup1="100110110"; 
				if (img_pattern.compare(extup1) == 0){
					for (int j=0; j<3; ++j){
						for (int i=0; i<2; ++i){
							new_img[xcoord+i][ycoord+j] = 1;
						}
					}
				}
				string extup2="001011011"; 
				if (img_pattern.compare(extup2) == 0){
					for (int j=0; j<3; ++j){
						for (int i=1; i<3; ++i){
							new_img[xcoord+i][ycoord+j] = 1;
						}
					}
				}
			
				stringstream m;
				for (int i=0; i<2; ++i){
					for (int j=0; j<4; ++j){
						m << new_img[xcoord+i][ycoord+j];
					}
				}
				string img_m=m.str();
				string extleft3[] = {"00010111", "00110111"} ; 
				for (int p=0; p<2; ++p){
					if (img_m.compare(extleft3[p]) == 0){
						//cout << " Left MATCH " << endl;
						for (int i=0; i<2; ++i){
							for (int j=1; j<4; ++j){
								new_img[xcoord+i][ycoord+j] = 1;
							}
						}
					}
				}
				string extleft4[] = {"10001110", "11001110"} ; 
				for (int p=0; p<2; ++p){
					if (img_m.compare(extleft4[p]) == 0){
						//cout << " Left MATCH below" << endl;
						for (int i=0; i<2; ++i){
							for (int j=0; j<3; ++j){
								new_img[xcoord+i][ycoord+j] = 1;
							}
						}
					}
				}
				string top_right_corner[] = {"011111110", "001001110", "011011110"}; 
				for (int p=0; p<3; ++p){
					if (img_pattern.compare(top_right_corner[p]) == 0){
						for (int j=0; j<2; ++j){
							for (int i=0; i<2; ++i){
								new_img[xcoord+i][ycoord+j] = 1;
							}
						}
					}
				}
				//2 pix and 1 pix corner
				string corner_pattern[] = {"110001001", "110111011", "110011011", "100001001", "110111001"}; //added "110001001" and needs to be put 1st
				for (int p=0; p<5; ++p){
					if (img_pattern.compare(corner_pattern[p]) == 0){
						for (int j=1; j<3; ++j){
							for (int i=0; i<2; ++i){
								new_img[xcoord+i][ycoord+j] = 1;
							}
						}
					}
				}
				stringstream corner1pix;
				for (int j=0; j<2; ++j){
					for (int i=0; i<2; ++i){
						corner1pix << new_img[xcoord+i][ycoord+j];
					}
				}
				string corner1pix_string=corner1pix.str();
				string corner1pix_pattern[] = {"1001", "0110"}; 
				for (int p=0; p<2; ++p){
					if (corner1pix_string.compare(corner1pix_pattern[p]) == 0){
						for (int j=0; j<2; ++j){
							new_img[xcoord][ycoord+j] = 1;
						}
					}
				}
				
				//scanning to repair broken lines
				string broken_line_pattern[] = {"000110001", "000110011", "000110111"}; // for extending horizontal lines
				for (int p=0; p<3; ++p){
					if (img_pattern.compare(broken_line_pattern[p]) == 0){
						for (int j=1; j<3; ++j){
							for (int i=0; i<3; ++i){
								new_img[xcoord+i][ycoord+j] = 1;
							}
						}
					}
				}
				string broken_both_sides[] = {"001110000", "011110000", "111110000", "111110000"}; 
				// 111\;110\;000 and ^111\;110\;000\; for extending horizontal lines
				for (int p=0; p<4; ++p){
					if (img_pattern.compare(broken_both_sides[p]) == 0){
						for (int j=0; j<2; ++j){
							for (int i=0; i<3; ++i){
								new_img[xcoord+i][ycoord+j] = 1;
							}
						}
					}
				}
				string brokencorner = "100001001";
				if (img_pattern.compare(brokencorner) == 0){
					new_img[xcoord+1][ycoord+1] = 1;
				}
				string verybrokencorner = "100000001";
				if (img_pattern.compare(verybrokencorner) == 0){
					for (int j=0; j<3; ++j){
						new_img[xcoord][ycoord+j] = 1;
					}
					for (int i=0; i<3; ++i){
						new_img[xcoord+i][ycoord+2] = 1;
					}
				}
				stringstream m1;
				for (int j=0; j<2; ++j){
					for (int i=0; i<4; ++i){
						m1 << new_img[xcoord+i][ycoord+j];
					}
				}
				string img_m1=m1.str();
				string extendup[] = {"10001110", "00010111"} ; 
				for (int p=0; p<2; ++p){
					if (img_m1.compare(extendup[p]) == 0){
						//cout << "UP MATCH " << endl;
						for (int j=0; j<2; ++j){
							for (int i=1; i<3; ++i){
								new_img[xcoord+i][ycoord+j] = 1;
							}
						}
					}
				}
				stringstream m2;
				for (int j=0; j<2; ++j){
					for (int i=0; i<6; ++i){
						m2 << new_img[xcoord+i][ycoord+j];
					}
				}
			/*	string img_m2=m1.str();
				string extendup2[] = {"10000111110", "000001011111"} ; 
				for (int p=0; p<2; ++p){
					if (img_m2.compare(extendup2[p]) == 0){
						cout << "UP MATCH AGAIN " << endl;
						for (int j=0; j<2; ++j){
							for (int i=1; i<5; ++i){
								new_img[xcoord+i][ycoord+j] = 1;
							}
						}
					}
				}
			*/
			}
		}

		// print out image as a jpg
		Image fourth = second;		
		for (ycoord = 0;  ycoord < scaled_rows; ++ycoord){ //no pixel 0,0
			for (xcoord = 0;  xcoord < scaled_cols; ++xcoord){
				//out_file << new_img[xcoord][ycoord]	;
				if (new_img[xcoord][ycoord]==1){
					//cout << xcoord << "  " << ycoord << endl;
					fourth.pixelColor(xcoord,ycoord,"black");
				}else {
					fourth.pixelColor(xcoord,ycoord,"white");
				}
			}
			//out_file << endl;
		}
		fourth.write("ScaledLines.jpg");		

		time (&end);
  		dif = difftime (end,start);
  		printf ("It took %.2lf seconds to remove lines.\n", dif );
  	

  		
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//  		
		//>>>>>>>>PATCHIN UP>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//	
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
		time (&start);
		
		log << "Patching up lines ..." << endl;
		time (&end);
  		dif = difftime (end,start);
  		printf ("It took %.2lf seconds to patchup corners and lines.\n", dif );
  		
		//printing out edge image in jpg
		Image third = second;
//		cout << "the width of image is " << third.columns() << endl;
//		cout << "the height of image is " << third.rows() << endl;
		
		for (ycoord = 0;  ycoord < scaled_rows; ++ycoord){ //no pixel 0,0
			for (xcoord = 0;  xcoord < scaled_cols; ++xcoord){
				//out_file << new_img[xcoord][ycoord]	;
				if (new_img[xcoord][ycoord]==1){
					//cout << xcoord << "  " << ycoord << endl;
					third.pixelColor(xcoord,ycoord,"black");
				}else {
					third.pixelColor(xcoord,ycoord,"white");
				}
			}
			//out_file << endl;
		}
		
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
		//>>>>>>>>>> EDGE DETECTION AND TRACKING >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
		third.edge(1);
//		third.write("ScaledBlankEdge.jpg");
		log << "Edge detection in progress ..." << endl;
		vector<vector<int> > edge_img (scaled_cols, vector<int> ( scaled_rows ) );
  		for ( int y = 0; y < scaled_rows; y++ ) {
    		for ( int x = 0; x < scaled_cols; x++ ){
    			edge_img[x][y] = 0;
    			pixel_sample = third.pixelColor(x,y);
    			if ( black.compare(pixel_sample) == 0 ){
    				edge_img[x][y] = 1;
    			}
    		}
  		}	
  		
  		stringstream svgstr;
		svgstr << "<?xml version=\"1.0\" standalone=\"no\"?>" << endl;
		svgstr << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"" << endl;
		svgstr << "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" << endl;
		svgstr << "<svg width=\"100%\" height=\"100%\" version=\"1.1\"" << endl;
		svgstr << "xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">" << endl;
		svgstr << "<image xlink:href=\"" << fname << "\" width=\"" << orig_scaled_cols << "\" height=\"" << orig_scaled_rows << "\"/>";
		int tip = 0;
		int xroot = 0;
		int yroot = 0;
		stringstream tree;
		int tip_xcoord[scaled_rows];		
		int tip_ycoord[scaled_cols];	
		int prevx;
		int prevy;
		signed int brchlength;
		log << "Tracking line edges ..." << endl;
	  time (&start);
	  dif = 0;

		for (xcoord = 0;  xcoord < scaled_cols; ++xcoord){
			for (ycoord = 0;  ycoord < scaled_rows; ++ycoord){ 
				if (edge_img[xcoord][ycoord]==0 && dif<300){
					xroot = xcoord;
					yroot = ycoord;
//					cout << "root " << xcoord << " " << ycoord << endl;
					prevx=xcoord;
					prevy=ycoord;
					if (edge_img[xcoord+1][ycoord]==0){
						while (edge_img[xcoord+1][ycoord]==0 && xcoord<scaled_cols && ycoord<scaled_rows && xcoord>=xroot && ycoord>0 && dif<300){
							++xcoord;// go right
						}
//						cout << "begin by going right to " << xcoord << " " << ycoord << endl;
						svgstr << "<line x1=\"" << prevx << "\" y1=\"" << prevy << "\""; 
						svgstr << " x2=\"" << xcoord << "\" y2=\"" << ycoord << "\" style=\"stroke:rgb(255,0,0);stroke-width:1\"/>" << endl;
						second.strokeColor( "red" );
      					second.strokeWidth( 2 );
      					second.draw( DrawableLine( prevx,prevy, xcoord,ycoord ) );
						prevx=xcoord;
						prevy=ycoord;
						tree << "(";
					time (&end);
  	    			dif = difftime (end,start);
// 	    			cout << "time spent in loop " << dif << endl;
                    if (dif>=300) {
                      cerr << "Timed out during contour. Image was probably not cleaned succesfully" << endl;
                      exit(0);
                    }

					}
					while (edge_img[xcoord][ycoord]==0 && xcoord<scaled_cols && ycoord<scaled_rows && xcoord>=xroot && ycoord>0 && dif<300){
						if (edge_img[xcoord][ycoord-1]==0 && xcoord<scaled_cols && ycoord<scaled_rows && xcoord>=xroot && ycoord>0){ //go up
							while (edge_img[xcoord][ycoord-1]==0 && xcoord<scaled_cols && ycoord<scaled_rows && xcoord>=xroot && ycoord>0){
								--ycoord;// go up
							}
//							cout << "gone up " << xcoord << " " << ycoord << endl;
							svgstr << "<line x1=\"" << prevx << "\" y1=\"" << prevy << "\""; 
							svgstr << " x2=\"" << xcoord << "\" y2=\"" << ycoord << "\" style=\"stroke:rgb(255,0,0);stroke-width:1\"/>" << endl;
      						second.draw( DrawableLine( prevx,prevy, xcoord,ycoord ) );

							prevx=xcoord;
							prevy=ycoord;
							if (edge_img[xcoord+1][ycoord]==0 && xcoord<scaled_cols && ycoord<scaled_rows && xcoord>=xroot && ycoord>0){ // and right
								while (edge_img[xcoord+1][ycoord]==0 && xcoord<scaled_cols && ycoord<scaled_rows && xcoord>=xroot && ycoord>0){
									++xcoord; //go right
								}
//								cout << "gone right to " << xcoord << " " << ycoord << endl;
								svgstr << "<line x1=\"" << prevx << "\" y1=\"" << prevy << "\""; 
								svgstr << " x2=\"" << xcoord << "\" y2=\"" << ycoord << "\" style=\"stroke:rgb(255,0,0);stroke-width:1\"/>" << endl;
      							second.draw( DrawableLine( prevx,prevy, xcoord,ycoord ) );
								if (prevx==xcoord && prevy==ycoord){
//									cout << "up and right: breaking at " << xcoord << " " << ycoord << endl;
									break;
								}
								prevx=xcoord;
								prevy=ycoord;
								tree << "(";
							} else if (edge_img[xcoord-1][ycoord]==0 && xcoord<scaled_cols && ycoord<scaled_rows && xcoord>=xroot && ycoord>0 && xcoord>0){ //go left
								while (edge_img[xcoord-1][ycoord]==0 && xcoord<scaled_cols && ycoord<scaled_rows && xcoord>=xroot && ycoord>0 && xcoord>1){
									--xcoord; // go left
								}
//								cout << "gone left to " << xcoord << " " << ycoord << endl;
//								cout << "previous values " << prevx << " " << prevy ;
								svgstr << "<line x1=\"" << prevx << "\" y1=\"" << prevy << "\""; 
								svgstr << " x2=\"" << xcoord << "\" y2=\"" << ycoord << "\" style=\"stroke:rgb(255,0,0);stroke-width:1\"/>" << endl;
      							second.draw( DrawableLine( prevx,prevy, xcoord,ycoord ) );
								if (prevx==xcoord && prevy==ycoord){
//									cout << "up and left: breaking at " << xcoord << " " << ycoord << endl;
									break;
								}
								brchlength=prevx-xcoord;
//								cout << " difference " << brchlength << endl; 
								if (brchlength<0){
								  brchlength=-brchlength;
								}
								
								prevx=xcoord;
								prevy=ycoord;
								tree << "):" << brchlength;
								if (xcoord<=(xroot+1)){ //because of [xcoord-1] and xcoord>1
									svgstr << "<line x1=\"" << prevx << "\" y1=\"" << prevy << "\""; 
									svgstr << " x2=\"" << xcoord << "\" y2=\"" << ycoord << "\" style=\"stroke:rgb(255,0,0);stroke-width:1\"/>" << endl;
      								second.draw( DrawableLine( prevx,prevy, xcoord,ycoord ) );
									brchlength=prevx-xroot;
									if (brchlength<0){
									  brchlength=-brchlength;
									}
									prevx=xcoord;
									prevy=ycoord;
									tree << "):" << brchlength << ";" << endl; // << "end;" << endl;
									xcoord=scaled_cols;
									ycoord=scaled_rows;
									break;
								}
							}
						}
						if (edge_img[xcoord][ycoord+1]==0 && xcoord<scaled_cols && ycoord<scaled_rows && xcoord>=xroot && ycoord>0){//go down
							while (edge_img[xcoord][ycoord+1]==0 && xcoord<scaled_cols && ycoord<scaled_rows && xcoord>=xroot && ycoord>0){
								++ycoord;
							}
//							cout << "gone down " << xcoord << " " << ycoord << endl;
							svgstr << "<line x1=\"" << prevx << "\" y1=\"" << prevy << "\""; 
							svgstr << " x2=\"" << xcoord << "\" y2=\"" << ycoord << "\" style=\"stroke:rgb(255,0,0);stroke-width:1\"/>" << endl;
      						second.draw( DrawableLine( prevx,prevy, xcoord,ycoord ) );

							prevx=xcoord;
							prevy=ycoord;
							if (edge_img[xcoord-1][ycoord]==0 && xcoord<scaled_cols && ycoord<scaled_rows && xcoord>=xroot && ycoord>0){ // and left
								// do array of tipcoordinates);
								++tip;
															
								
								tip_xcoord[tip]=xcoord+1; //to avoid having pixels before the label for OCR in tesseract
								tip_ycoord[tip]=ycoord;// getting the coordinates of the tips to be able to slice names off
								while (edge_img[xcoord-1][ycoord]==0 && xcoord<scaled_cols && ycoord<scaled_rows && xcoord>=xroot && ycoord>0){
									--xcoord; // go left
								}
								brchlength=prevx-xcoord;
							    if (brchlength<0){
							      brchlength=-brchlength;
							    }	
							    tree << tip << ":" << brchlength;
//								cout << "gone left to " << xcoord << " " << ycoord << endl;
//								cout << "previous values " << prevx << " " << prevy ;
								svgstr << "<line x1=\"" << prevx << "\" y1=\"" << prevy << "\""; 
								svgstr << " x2=\"" << xcoord << "\" y2=\"" << ycoord << "\" style=\"stroke:rgb(255,0,0);stroke-width:1\"/>" << endl;
      							second.draw( DrawableLine( prevx,prevy, xcoord,ycoord ) );
								if (prevx==xcoord && prevy==ycoord){
//									cout << "down and left: breaking at " << xcoord << " " << ycoord << endl;
									break;
								}
								prevx=xcoord;
								prevy=ycoord;
								if (xcoord<=xroot){
									svgstr << "<line x1=\"" << prevx << "\" y1=\"" << prevy << "\""; 
									svgstr << " x2=\"" << xcoord << "\" y2=\"" << ycoord << "\" style=\"stroke:rgb(255,0,0);stroke-width:1\"/>" << endl;
      								second.draw( DrawableLine( prevx,prevy, xcoord,ycoord ) );
      								
									brchlength=prevx-xroot;
//									cout << "difference " << brchlength << endl; 
								    if (brchlength<0){
								      brchlength=-brchlength;
								    }																

									prevx=xcoord;
									prevy=ycoord;
									tree << "):" << brchlength << ";" << endl; // << "end;" << endl;
									xcoord=scaled_cols;
									ycoord=scaled_rows;
									break;
								}
							} else if (edge_img[xcoord+1][ycoord]==0 && xcoord<scaled_cols && ycoord<scaled_rows && xcoord>=xroot && ycoord>0){ // go right
								while (edge_img[xcoord+1][ycoord]==0 && xcoord<scaled_cols && ycoord<scaled_rows && xcoord>=xroot && ycoord>0){
									++xcoord; //go right
								}
//								cout << "gone right to " << xcoord << " " << ycoord << endl;
								svgstr << "<line x1=\"" << prevx << "\" y1=\"" << prevy << "\""; 
								svgstr << " x2=\"" << xcoord << "\" y2=\"" << ycoord << "\" style=\"stroke:rgb(255,0,0);stroke-width:1\"/>" << endl;
      							second.draw( DrawableLine( prevx,prevy, xcoord,ycoord ) );
								if (prevx==xcoord && prevy==ycoord){
//									cout << "down and right: breaking at " << xcoord << " " << ycoord << endl;
									break;
								}
								prevx=xcoord;															
								prevy=ycoord;
								tree << ",";
							}
						}
					    time (&end);
  	    			    dif = difftime (end,start);
// 	    			    cout << "time spent in loop " << dif << endl;
                        if (dif>=300) {
                          cerr << "Timed out during contour. Image was probably not cleaned succesfully" << endl;
                          exit(0);
                        }

					}
					
				}// closes the "if (edge_img[xcoord][ycoord]==0 && dif<300){" statement
				 
			}
		}
	
		svgstr << "</svg>" << endl;	
		string tree_file=tree.str();

		//need to find a way to clean up the taxa labels data_label contains all the position information for the gettree.php
		stringstream data_label;
		data_label << tree_file;

		std::ofstream tree_out("tree.phy", std::ios::binary|std::ios::out);	
		tree_out << tree_file;
		tree_out.close();
		//second.write("Contour.jpg");
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
		//<<<<<<<<<<<<<<<<<<<<<<<<<<< croping the names <<<<<<<<<<<<<<<<<<<<<<<<<<<<//
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
		stringstream nexustree;
		nexustree << "#NEXUS" << endl << "Begin TREES;" << endl << "\tTranslate" << endl;
			
		//create a white image the size of the original master image
		Image txt_img(Geometry (cols,rows), Color("white"));
		float scale_up ;
		scale_up = (float) cols/orig_scaled_cols;
		//float scale_down = (float) scaled_cols / cols;
//		cout << " scale " << scale_up << endl;
		
		//second line
		int totalwidth = 0;
		int prevheight = 0;
		int maxheight = 0;
		int prevscaled_height = 0;
		int maxscaled_height = 0;
		int totalscaled_width = 0;
		int totalheight = 0;
		int totalscaled_height = 0;
		log << "OCR of tree leaves ..." << endl;
		for (int i = 1;  i <= tip; ++i){		
			totalwidth = cols - tip_xcoord[i]*scale_up + totalwidth;
			totalheight = rows - tip_ycoord[i]*scale_up + totalheight;
			prevheight = (((tip_ycoord[i+1]*scale_up+tip_ycoord[i]*scale_up)/2)-((tip_ycoord[i]*scale_up+tip_ycoord[i-1]*scale_up)/2)) ;
			if (maxheight < prevheight){
				maxheight = prevheight;
			}
			prevscaled_height =  (((tip_ycoord[i+1]+tip_ycoord[i])/2)-((tip_ycoord[i]+tip_ycoord[i-1])/2));
			if (maxscaled_height < prevscaled_height){
				maxscaled_height = prevscaled_height;
			}
			totalscaled_width = orig_scaled_cols - tip_xcoord[i] + totalscaled_width;
			totalscaled_height = orig_scaled_rows - tip_ycoord[i] + totalscaled_height;
		}		
		data_label << cols << "; " << rows << "; " << totalwidth/tip << "; " << totalheight/tip << "; " ;//maxheight << "; " ;
		data_label << orig_scaled_cols << "; " << orig_scaled_rows << "; " << totalscaled_height/tip << "; " ; //maxscaled_height << "; " 
		data_label << totalscaled_width/tip << "; " << tip << "; " << endl;
		
		//data labels for the rest of the lines
		for (int i = 1;  i <= tip; ++i){		
//			cout << "tip " << i << " xcoord " << tip_xcoord[i] << " ycoord " << tip_ycoord[i] << endl;
			int yOffset = ((tip_ycoord[i-1]*scale_up+tip_ycoord[i]*scale_up)/2);
			int xOffset = tip_xcoord[i]*scale_up;
			int width = cols - tip_xcoord[i]*scale_up;
			int height = ((tip_ycoord[i+1]*scale_up+tip_ycoord[i]*scale_up)/2)-((tip_ycoord[i]*scale_up+tip_ycoord[i-1]*scale_up)/2);
			int xbox = tip_xcoord[i];
			int ybox = ((tip_ycoord[i-1]+tip_ycoord[i])/2);
			int box_height = ((tip_ycoord[i+1]+tip_ycoord[i])/2)-((tip_ycoord[i]+tip_ycoord[i-1])/2);
			int box_width = scaled_cols - tip_xcoord[i];
			Image taxa_label = master;

			// Crop the image to specified size (width, height, xOffset, yOffset)
			if (i==1){
				yOffset=1;
				height = (tip_ycoord[i+1]*scale_up+tip_ycoord[i]*scale_up)/2;
				ybox = 1;
				box_height = (tip_ycoord[i+1]+tip_ycoord[i])/2;
			}
			if (i==tip){
				height=rows-((tip_ycoord[i]*scale_up+tip_ycoord[i-1]*scale_up)/2);
				box_height = orig_scaled_rows-(tip_ycoord[i]+tip_ycoord[i-1])/2;
			}
			//cout << "width " << width << " height " << height << " xOffset " << xOffset << " yOffset " << yOffset << endl;
			//make the box a little bit bigger seems to improve the OCR and reduces choping of lables
			//yOffset=yOffset-2;
			//height=height+4;
			taxa_label.crop( Geometry(width,height, xOffset, yOffset) );
			ostringstream file_label;
			file_label << "label.tif" ;//<< i << ".jpg";
			taxa_label.compressType(Magick::NoCompression);
			taxa_label.magick("TIFF");
			taxa_label.depth(8);
			taxa_label.write(file_label.str() );
			
			//std::system ("tesseract /Users/jhughes/userContent/label.tif /Users/jhughes/userContent/label");
			//cout << "tesseract /Users/jhughes/userContent/label.tif /Users/jhughes/userContent/label" << endl;
			system("/usr/local/bin/tesseract label.tif label");
			string line;
			ifstream taxa_name ("label.txt");
			if (taxa_name.is_open()){
   				getline (taxa_name,line);
   				line.erase(std::remove(line.begin(), line.end(), '>'), line.end());
   				line.erase(std::remove(line.begin(), line.end(), '<'), line.end());
   				line.erase(std::remove(line.begin(), line.end(), '"'), line.end());
   				line.erase(std::remove(line.begin(), line.end(), '`'), line.end());
   				line.erase(std::remove(line.begin(), line.end(), '\''), line.end());
   				line.erase(std::remove(line.begin(), line.end(), '?'), line.end());
   				line.erase(std::remove(line.begin(), line.end(), '}'), line.end());
   				line.erase(std::remove(line.begin(), line.end(), '{'), line.end());
   				line.erase(std::remove(line.begin(), line.end(), ';'), line.end());
    			cout << line << endl;
    			if (i<tip){
    			  nexustree << "\t" << i << "\t'" << line << "'," << endl;
    			}else{
    			  nexustree << "\t" << i << "\t'" << line << "'" << endl << ";" << endl;
    			}
      			//data_label << line << endl;
    			taxa_name.close();
  			}
  			else cout << "Unable to open file";
  			system ("rm label.jpg label.tif label.txt");
			//data_label << "labelid=" << i << " taxaname=" << line << " xcoord=" << xOffset << " ycoord=" << yOffset;
			//data_label << " boxwidth=" << width << " boxheight=" << height << endl;
			data_label << i << "; " << line << "; " << xOffset << "; " << yOffset;
			data_label << "; " << width << "; " << height << "; " << xbox << "; " << ybox << "; ";
			data_label << box_width << "; " << box_height << "; " << endl;

			taxa_label.border("1x1+0+0");
			txt_img.composite(taxa_label, xOffset, yOffset, OverCompositeOp);
		}
		//txt_img.write("TextImage.jpg");
		log << "Creating final output file ..." << endl;		
		string svg_file=svgstr.str();
		std::ofstream svg_out("tmp.svg", std::ios::binary|std::ios::out);
		svg_out << svg_file;
		svg_out.close();
		string img_txt=data_label.str();
		std::ofstream imgtxt_out("tmp.txt", std::ios::binary|std::ios::out);
		imgtxt_out << img_txt;
		imgtxt_out.close();
		nexustree << "TREE	TREERIPPER = " <<  tree_file << endl << endl << "End;" << endl;
		
		Profile<NTree> t;
        if (!t.ReadTrees (nexustree))
        {
          cerr << "Failed to read trees, bailing out" << endl;
          exit(0);
        }
        t.ShowTrees (cout);  
        of << nexustree.str();
		of.close();
		time (&end);
  		dif = difftime (end,begin);
		printf ("It took %.2lf seconds since the very beginning of processing.\n", dif );
		
	}
	catch( exception &error_ )
    {
    	cout << "Caught exception: " << error_.what() << endl;
      	return 1;
    }
	return 0;
}