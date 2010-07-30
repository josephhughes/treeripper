
 
// $Id: stsupport2.cpp,v 1.15 2002/03/22 12:52:09 jcotton Exp $

/**
 * @file stsupport2.cpp
 *
 * stsupport
 *
 */
#include "ntree.h"
#include "profile.h"

//addede JAC 18/03/04 for Support
#include <iterator>
#include <list>
#include <set>
#include <fstream>
#include <algorithm>
#include <numeric>

#ifdef __GNUC__
	#include <strstream>
#else
	#include <sstream>
#endif
#include <ctime>

#if __MWERKS__
	#if macintosh
		// Metrowerks support for Macintosh command line interface
		#include <console.h>
	#endif
#endif

#ifndef min
    #define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif
#ifndef max
    #define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#define MAJOR_VERSION "0"
#define MINOR_VERSION "1"
#define MINI_VERSION "0"

/*#include "mincut_st.h"
#include "strong_components.h"*/


// Modified SQUID code to handle command line options
#include "getoptions.h"
#define FILENAME_SIZE 256		// Maximum file name length

// Program options
static struct opt_s OPTIONS[] = {

	{ "-b", true, ARG_INT },
	{ "-v", true, ARG_NONE },
};

#define NOPTIONS (sizeof(OPTIONS) / sizeof(struct opt_s))

static char usage[] = "Usage: stsupport [-options] <tree-file> <outfile>\n\
\n\
  Available options: \n\
     -v             show version information\n\
     -b n           set verbosity level\n\
   	 ";


//typedef set<node, less<node> > NodeSet;

// Debugging/verbose flags

bool bAll				= false; //Invesigates all splits, not just on the STree
bool bVerbose			= false; // Write Verbose junk to cout

/**
 * @var  vector <NTree> NTreeVector
 * @brief A vector of NTree's, used to store the input trees
 *
 */
typedef vector <NTree> NTreeVector;


//------------------------------------------------------------------------------
int main (int argc, char **argv)
{

#if __MWERKS__
#if macintosh
    argc = ccommand(&argv);
#endif
#endif

    // Parse options
    // Heavily borrowed from the squid library
    char *optname;
    char *optarg;
    int   optind;

   
	bVerbose			= false; // Write Verbose junk to cout
	bAll				= false;
	int support_verbose = 0;
	
	while (Getopt(argc, argv, OPTIONS, NOPTIONS, usage,
                  &optind, &optname, &optarg))
    {
        if (strcmp(optname, "-b") == 0) {  support_verbose = atoi(optarg);
            if (support_verbose > 2) cout << "Writing verbose information" << endl;}
		if (strcmp(optname, "-v") == 0)
        {
            cout << "STSSupport " << MAJOR_VERSION << "." MINOR_VERSION << "." << MINI_VERSION << endl;
            cout << "(c)2004 James A. Cotton" << endl;
            cout << "distributed under GPL etc." << endl;
                        exit(EXIT_SUCCESS);
        }
	}
	
    if (argc - optind != 2)
    {
        cerr << "Incorrect number of arguments:" << usage << endl;
        exit (0);
    }
	
	
    // Get options from command line
    char fname[FILENAME_SIZE];
    strcpy( fname, argv[optind++] );
	
	char ofname[FILENAME_SIZE];
	strcpy( ofname, argv[optind++] );
	
    // Check file exists
    FILE* file = fopen( fname, "r" );
    if( file != NULL )
    {
        fclose(file);
        file = NULL;
    }
    else
    {
        cerr << "File \"" << fname << "\" does not exist." << endl;
        exit (0);
    }
	
	
   // cout << "STSupport " << MAJOR_VERSION << "." MINOR_VERSION << "." << MINI_VERSION << endl;

    ifstream f (fname);

	ofstream of (ofname);

    Profile<NTree> p;

    if (!p.ReadTrees (f))
    {
        cerr << "Failed to read trees, bailing out" << endl;
        exit(0);
    }

	f.close();
	of.close();
  

  
    return 0;
}
