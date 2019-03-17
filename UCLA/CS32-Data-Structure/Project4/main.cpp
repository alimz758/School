//
//  main.cpp
//  Project 4
//
//  Created by ali mirabzadeh on 3/6/19.
//  Copyright © 2019 AMZ Development. All rights reserved.
//

//#include <iostream>
//#include "Trie.h"
//#include "vector"
////#include "provided.h"
//#include <iostream>
//#include <fstream> // needed to open files
//#include <string>
//#include <vector>
//using namespace std;
//int main()
//{
//    //==================Genome Tester ====================
//
////        // Specify the full path and name of the gene data file on your hard drive.
////        string filename = "/Users/alimirabzadeh/desktop/Project 4/data/Halorubrum_californiense.txt";
////        // Open the data file and get a ifstream object that can be used to read its
////        // contents.
////        ifstream strm(filename);
////        if (!strm)
////        {
////            cout << "Cannot open " << filename << endl;
////            return 1;
////        }
////        vector<Genome> vg;
////        bool success = Genome::load(strm, vg); // Load the data via the stream.
////        string frag;
////        if (success)
////        {
////            cout << "Loaded " << vg.size() << " genomes successfully:" << endl;
////            for (int k = 0; k != vg.size(); k++)
////            {
////                cout << "Name: " << k << " "<<vg[k].name() << endl;
////                //cout << "Seq: ";
////              // vg[k].length();
////                cout <<"Seq: " << k << " ";
////                vg[k].extract(0, frag.length()-1, frag);
////                for(int i=0; i<frag.length();i++)
////                    cout <<frag[i];
////            }
////            cout<<endl;
////        }
////        else
////            cout << "Error loading genome data" << endl;
// // destructor for ifstream closes the file
////    string s="abcd";
////    cout << s.substr(0,2);
//    
//    
//    //======================= Trie Testet=========
   // Trie<int> trie; // This is like std::multimap<std::string,int> trie;
//    trie.insert("HI", 9); // GATTACA à {42}
//    trie.insert("HI", 17);
//    trie.insert("HIT", 1);
//    trie.insert("TIP", 1);
//    trie.insert("HIT", 2);
//    trie.insert("HIP", 100);
//    trie.insert("HIP", 200);
//    trie.insert("HAT", 7);
//    trie.insert("HAT", 8);
//    trie.insert("HAT", 9);
//    trie.insert("TAX", 1000);
//    trie.insert("HA", 14);
//    trie.insert("TAP", 19);
//    trie.insert("TAP", 6);
//    trie.insert("TAP", 32);
 //   string seq="ACGTACGTAAAACCCCGGGGTTTTNANANANANA";
//    //    1. Add the genome to a collection of genomes (e.g., a vector or list) held by the GenomeMatcher object.
//
  // int start=0,index =0;
//    //2. Index the DNA sequences of the newly-added genome by adding every substring of length minSearchLength (the value that was passed into your constructor) of
//    // that genome's DNA sequence into a Trie maintained by the GenomeMatcher.
//
//    //while can extract the right seq, insert
//    //cout << seq<<endl;
//        //cout << seq;
        //insert the fragment with the corresponding index
//        for(int i= 0 ;  i+10 <seq.length();i++)
//        {
//            //cout << seq.substr(i,10) <<endl;
//            trie.insert(seq.substr(i,10), index);
//            index++;
//        }
//
////    trie.insert("GATTACA", 17); // GATTACA à {42, 17}
////    trie.insert("GATTACA", 42); // GATTACA à {42, 17, 42}
//   // trie.insert("GCTTACA", 30);
////    trie.insert("ACTA", 2);
////    trie.insert("CGATAGCTA", 5);
//
//
//    vector<int> result=  trie.find("AAAACCCCGG", true);
//    for(int i=0; i<result.size();i++)
//    {
//        cout<< result[i] <<endl;
//    }
//}
#include "provided.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <cstdlib>
using namespace std;

// Change the string literal in this declaration to be the path to the
// directory that contains the genome data files we provide, e.g.,
// "Z:/CS32/Geenomics/data" or "/Users/fred/cs32/Geenomics/data"

const string PROVIDED_DIR = "/Users/alimirabzadeh/desktop/Project 4/data/";

const string providedFiles[] = {
    "Ferroplasma_acidarmanus.txt",
    "Halobacterium_jilantaiense.txt",
    "Halorubrum_chaoviator.txt",
    "Halorubrum_californiense.txt",
    "Halorientalis_regularis.txt",
    "Halorientalis_persicus.txt",
    "Ferroglobus_placidus.txt",
    "Desulfurococcus_mucosus.txt"
};

void createNewLibrary(GenomeMatcher*& library)
{
    cout << "Enter minimum search length (3-100): ";
    string line;
    getline(cin, line);
    int len = atoi(line.c_str());
    if (len < 3 || len > 100)
    {
        cout << "Invalid prefix size." << endl;
        return;
    }
    delete library;
    library = new GenomeMatcher(len);
}

void addOneGenomeManually(GenomeMatcher* library)
{
    cout << "Enter name: ";
    string name;
    getline(cin, name);
    if (name.empty())
    {
        cout << "Name must not be empty." << endl;
        return;
    }
    cout << "Enter DNA sequence: ";
    string sequence;
    getline(cin, sequence);
    if (sequence.empty())
    {
        cout << "Sequence must not be empty." << endl;
        return;
    }
    if (sequence.find_first_not_of("ACGTNacgtn") != string::npos)
    {
        cout << "Invalid character in DNA sequence." << endl;
        return;
    }
    for (char ch : sequence)
        ch = toupper(ch);
    library->addGenome(Genome(name, sequence));
}

bool loadFile(string filename, vector<Genome>& genomes)
{
    ifstream inputf(filename);
    if (!inputf)
    {
        cout << "Cannot open file: " << filename << endl;
        return false;
    }
    if (!Genome::load(inputf, genomes))
    {
        cout << "Improperly formatted file: " << filename << endl;
        return false;
    }
    return true;
}

void loadOneDataFile(GenomeMatcher* library)
{
    string filename;
    cout << "Enter file name: ";
    getline(cin, filename);
    if (filename.empty())
    {
        cout << "No file name entered." << endl;
        return;
    }
    vector<Genome> genomes;
    if (!loadFile(filename, genomes))
        return;
    for (const auto& g : genomes)
        library->addGenome(g);
    cout << "Successfully loaded " << genomes.size() << " genomes." << endl;
}

void loadProvidedFiles(GenomeMatcher* library)
{
    for (const string& f : providedFiles)
    {
        vector<Genome> genomes;
        if (loadFile(PROVIDED_DIR + "/" + f, genomes))
        {
            for (const auto& g : genomes)
                library->addGenome(g);
            cout << "Loaded " << genomes.size() << " genomes from " << f << endl;
        }
    }
}

void findGenome(GenomeMatcher* library, bool exactMatch)
{
    if (exactMatch)
        cout << "Enter DNA sequence for which to find exact matches: ";
    else
        cout << "Enter DNA sequence for which to find exact matches and SNiPs: ";
    string sequence;
    getline(cin, sequence);
    int minLength = library->minimumSearchLength();
    if (sequence.size() < minLength)
    {
        cout << "DNA sequence length must be at least " << minLength << endl;
        return;
    }
    cout << "Enter minimum sequence match length: ";
    string line;
    getline(cin, line);
    int minMatchLength = atoi(line.c_str());
    if (minMatchLength > sequence.size())
    {
        cout << "Minimum match length must be at least the sequence length." << endl;
        return;
    }
    vector<DNAMatch> matches;
    if (!library->findGenomesWithThisDNA(sequence, minMatchLength, exactMatch, matches))
    {
        cout << "No ";
        if (exactMatch)
            cout << " matches";
        else
            cout << " matches or SNiPs";
        cout << " of " << sequence << " were found." << endl;
        return;
    }
    cout << matches.size();
    if (exactMatch)
        cout << " matches";
    else
        cout << " matches and/or SNiPs";
    cout << " of " << sequence << " found:" << endl;
    for (const auto& m : matches)
        cout << "  length " << m.length << " position " << m.position << " in " << m.genomeName << endl;
}

bool getFindRelatedParams(double& pct, bool& exactMatchOnly)
{
    cout << "Enter match percentage threshold (0-100): ";
    string line;
    getline(cin, line);
    pct = atof(line.c_str());
    if (pct < 0  ||  pct > 100)
    {
        cout << "Percentage must be in the range 0 to 100." << endl;
        return false;
    }
    cout << "Require (e)xact match or allow (S)NiPs (e or s): ";
    getline(cin, line);
    if (line.empty() || (line[0] != 'e' && line[0] != 's'))
    {
        cout << "Response must be e or s." << endl;
        return false;
    }
    exactMatchOnly = (line[0] == 'e');
    return true;
}

void findRelatedGenomesManual(GenomeMatcher* library)
{
    cout << "Enter DNA sequence: ";
    string sequence;
    getline(cin, sequence);
    int minLength = library->minimumSearchLength();
    if (sequence.size() < minLength)
    {
        cout << "DNA sequence length must be at least " << minLength << endl;
        return;
    }
    double pctThreshold;
    bool exactMatchOnly;
    if (!getFindRelatedParams(pctThreshold, exactMatchOnly))
        return;

    vector<GenomeMatch> matches;
    library->findRelatedGenomes(Genome("x", sequence), minLength, exactMatchOnly, pctThreshold, matches);
    if (matches.empty())
    {
        cout << "    No related genomes were found" << endl;
        return;
    }
    cout << "    " << matches.size() << " related genomes were found:" << endl;
    cout.setf(ios::fixed);
    cout.precision(2);
    for (const auto& m : matches)
        cout << " " << setw(6) << m.percentMatch << "%  " << m.genomeName << endl;
}

void findRelatedGenomesFromFile(GenomeMatcher* library)
{
    string filename;
    cout << "Enter name of file containing one or more genomes to find matches for: ";
    getline(cin, filename);
    if (filename.empty())
    {
        cout << "No file name entered." << endl;
        return;
    }
    vector<Genome> genomes;
    if (!loadFile(filename, genomes))
        return;
    double pctThreshold;
    bool exactMatchOnly;
    if (!getFindRelatedParams(pctThreshold, exactMatchOnly))
        return;

    int minLength = library->minimumSearchLength();
    for (const auto& g : genomes)
    {
        vector<GenomeMatch> matches;
        library->findRelatedGenomes(g, 2 * minLength, exactMatchOnly, pctThreshold, matches);
        cout << "  For " << g.name() << endl;
        if (matches.empty())
        {
            cout << "    No related genomes were found" << endl;
            continue;
        }
        cout << "    " << matches.size() << " related genomes were found:" << endl;
        cout.setf(ios::fixed);
        cout.precision(2);
        for (const auto& m : matches)
            cout << "     " << setw(6) << m.percentMatch << "%  " << m.genomeName << endl;
    }
}

void showMenu()
{
    cout << "        Commands:" << endl;
    cout << "         c - create new genome library      s - find matching SNiPs" << endl;
    cout << "         a - add one genome manually        r - find related genomes (manual)" << endl;
    cout << "         l - load one data file             f - find related genomes (file)" << endl;
    cout << "         d - load all provided data files   ? - show this menu" << endl;
    cout << "         e - find matches exactly           q - quit" << endl;
}

int main()
{
    const int defaultMinSearchLength = 10;

    cout << "Welcome to the Gee-nomics test harness!" << endl;
    cout << "The genome library is initially empty, with a default minSearchLength of " << defaultMinSearchLength << endl;
    showMenu();

    GenomeMatcher* library = new GenomeMatcher(defaultMinSearchLength);

    for (;;)
    {
        cout << "Enter command: ";
        string command;
        if (!getline(cin, command))
            break;
        if (command.empty())
            continue;
        switch(tolower(command[0]))
        {
            default:
                cout << "Invalid command " << command << endl;
                break;
            case 'q':
                delete library;
                return 0;
            case '?':
                showMenu();
                break;
            case 'c':
                createNewLibrary(library);
                break;
            case 'a':
                addOneGenomeManually(library);
                break;
            case 'l':
                loadOneDataFile(library);
                break;
            case 'd':
                loadProvidedFiles(library);
                break;
            case 'e':
                findGenome(library, true);
                break;
            case 's':
                findGenome(library, false);
                break;
            case 'r':
                findRelatedGenomesManual(library);
                break;
            case 'f':
                findRelatedGenomesFromFile(library);
                break;
        }
    }
}
