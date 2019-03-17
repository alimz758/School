#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "Trie.h"
using namespace std;


//Your GenomeMatcher implementation must use your Trie class template in the
//implementation of all data structures that hold DNA sequences. It may use any other STL
//container classes so long as these are not used to hold DNA sequence data (i.e., strings of
//As, Cs, Ts, Gs, or Ns). It may also use any functions from <algorithm>.

//template <typename ValueType>
class GenomeMatcherImpl
{
public:
    GenomeMatcherImpl(int minSearchLength);
    ~GenomeMatcherImpl();
    void addGenome(const Genome& genome);
    int minimumSearchLength() const;
    bool findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const;
    bool findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const;
private:
    Trie<int> trieNode;//Trie map to store Trie and position of Genome
    vector<Genome> genomeCollection;
    int m_minSearchLength;
    vector<DNAMatch> findMatchHelper(string fragment, int startPos, int minLength, vector<DNAMatch> matches, bool exactMatch ) const
    {
       
        //iterate through each Genome in the library
        for (vector<Genome> ::const_iterator  it= genomeCollection.begin() ; it!= genomeCollection.end();it++)
        {
            int i=0;
            string fragmentFromCurrentGenome="";
            //for each object make the longest match empty  in the  beginning
            string longestMatchFound="";
            if(exactMatch==true)
            {
                while(it->extract(startPos, minLength+i, fragmentFromCurrentGenome) && minLength+i<=fragment.length())
                {
                    //if the subfragment that has been found in the Trie matches with the  current substring in the current genome
                    if(fragment.substr(0,minLength+i)==fragmentFromCurrentGenome)
                    {
                        if(fragmentFromCurrentGenome>longestMatchFound)
                            longestMatchFound=fragmentFromCurrentGenome;
                    }
                    i++;
                }
            }
            if(exactMatch==false)
            {
                int counter=0;
                string temp="";
                if(it->extract(startPos, fragment.length(), fragmentFromCurrentGenome))
                {
                    if(fragmentFromCurrentGenome[0]!=fragment[0])
                        break;
                    temp+=fragment[0];
                    for(int i=1; i<fragment.length();i++)
                    {
                        if(fragmentFromCurrentGenome[i]!=fragment[i])
                        {
                            counter++;
                            if(counter==2)
                                break;
                        }
                        temp+=fragment[i];
                        if(temp.length()>=minLength)
                        {
                            if(temp>longestMatchFound)
                                longestMatchFound=temp;
                        }
                        
                    }
                }
            }
            //extract a fragment from the sequence of the current library, from the minLength size to fragment's size
           //by the end of the loop, the
            //if no matches got found for the current Genome, that means there was no longest match, so don't push anything to the object
            if(longestMatchFound=="")
                continue;
            DNAMatch newMatch;
            newMatch.genomeName=it->name();
            newMatch.length=longestMatchFound.size();
            newMatch.position=startPos;
            matches.push_back(newMatch);
        }
        return matches;
    }
};

//template <typename ValueType>
GenomeMatcherImpl::GenomeMatcherImpl(int minSearchLength)
{
//    The GenomeMatcher constructor takes a single argument which specifies the minimum
//    length of a DNA sequence that the user can later search for within the genome library.
//    (Your constructor should save this into a data member for later use.) For example, if the
//    user were to pass in a value of six for the minimum search length, then the shortest DNA
//    sequence the user could search for using the findGenomesWithThisDNA() method would
//    be six bases long, e.g. “GATTAC”. Attempting to search for shorter sequences like
//    “GATTA” or “ACTG” would result in findGenomesWithThisDNA() returning false.
//    This method must run in O(1) time
    m_minSearchLength=minSearchLength;
    //trieNode=new Trie<int>;
}
GenomeMatcherImpl:: ~GenomeMatcherImpl()
{
    //====================== TO-DO ============================
    //destructing
    //2
    //trieNode.reset();
}

void GenomeMatcherImpl::addGenome(const Genome& genome)
{
    string seq;
//    1. Add the genome to a collection of genomes (e.g., a vector or list) held by the GenomeMatcher object.
    genomeCollection.push_back(genome);
    int start=0,index =0;
    //2. Index the DNA sequences of the newly-added genome by adding every substring of length minSearchLength (the value that was passed into your constructor) of
   // that genome's DNA sequence into a Trie maintained by the GenomeMatcher.
    
    //while can extract the right seq, insert
    if(genome.extract(start, genome.length(), seq))
    {
        
        //insert the fragment with the corresponding index
        for(int i= 0 ;  minimumSearchLength()+i <=seq.length();i++)
        {
            trieNode.insert(seq.substr(i,minimumSearchLength()), index);
            index++;
        }
    }
}
int GenomeMatcherImpl::minimumSearchLength() const
{
    return m_minSearchLength;
}
bool GenomeMatcherImpl::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
    //to store the object that has the longest match
    //longest fragment
    vector<int> posFounder;
//    The findGenomesWIthThisDNA() method must return false if
    //1. fragment's length is less than minimumLength, or
    if(fragment.length() < minimumLength)
        return false;
//        2. minimumLength is less than the minSearchLength value passed to the GnomeMatcher's constructor, or
    if( minimumLength < minimumSearchLength())
        return false;
    //counter that passes substring of fragment, e.g fragment= "ABCDEF" , minimumLength=3
    string subFragment=fragment.substr(0,minimumSearchLength());
    //looks through the trie and return a vector containing the position in where found the smallest subString of fragment
    //since if the smallest can be found we can find out the starting position
    posFounder=trieNode.find(subFragment, exactMatchOnly);
    //   3. There are no matches between fragment and any segment of any genome in the GenomeMatcher object's librarya
    if(posFounder.size()==0)
        return false;
    //for each starting position that has been found call the helper function to verify the Genome object and put it in the matches vector
    for(int i=0; i< posFounder.size();i++)
        matches=findMatchHelper(fragment, posFounder[i], minimumLength, matches, exactMatchOnly);
    //At this point matches have been found, so return true;
    return true;
}
bool GenomeMatcherImpl::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    int startingPos;
    //The method also must return false if the value fragmentMatchLength is less than the value of minSearchLength passed into the GenomeMatcher constructor.
    if(fragmentMatchLength< minimumSearchLength())
        return false;
    //For each such sequence
    vector<DNAMatch> genomeMatches={};
    string querySeq, libSeq;
    double percentageP;
    int numOfSeq= query.length()/fragmentMatchLength;
   // 2. Search for the extracted sequence across all genomes in the library (using findGenomesWithThisDNA()), allowing SNiP matches if exactMatchOnly is false).
    //We will consider sequences of length fragmentMatchLength from the querygenome starting at positions 0, 1* fragmentMatchLength,2* fragmentMatchLength,
   // etc. (e.g., if fragmentMatchLength were 12, the start positions would be 0, 12, 24, 36).
    //For each genome g in the library that contained at least one matching sequence from the query genome
    for (vector<Genome> :: const_iterator it = genomeCollection.begin() ; it!=genomeCollection.end(); it++)
    {
        int incrementCounterForEachMatchedGenome=0;
       
        genomeMatches.clear();//clear genomeMatches for each Genome library
        for (int i =0 ; i*fragmentMatchLength<query.length(); i++)
        {
            startingPos=i * fragmentMatchLength;
            querySeq="";
            //  1. Extract that sequence from the queried genome with the starting position.
            query.extract(startingPos, fragmentMatchLength, querySeq);
            //cout << querySeq<<endl;
            if(findGenomesWithThisDNA(querySeq, fragmentMatchLength, exactMatchOnly, genomeMatches))
            {
                for (vector<DNAMatch> :: const_iterator iter= genomeMatches.begin(); iter!= genomeMatches.end(); iter++)
                {
                    if(it->name()== iter->genomeName)
                    {
                        incrementCounterForEachMatchedGenome++;
                        break;
                    }
                }
            }
        }
        percentageP=incrementCounterForEachMatchedGenome/numOfSeq;
        if(percentageP>matchPercentThreshold)
        {
            GenomeMatch genomeMatch;
            genomeMatch.genomeName=it->name();
            genomeMatch.percentMatch=percentageP;
            results.push_back(genomeMatch);
        }
    }
    return true;
}

//******************** GenomeMatcher functions ********************************

// These functions simply delegate to GenomeMatcherImpl's functions.
// You probably don't want to change any of this code.

GenomeMatcher::GenomeMatcher(int minSearchLength)
{
    m_impl = new GenomeMatcherImpl(minSearchLength);
}

GenomeMatcher::~GenomeMatcher()
{
    delete m_impl;
}

void GenomeMatcher::addGenome(const Genome& genome)
{
    m_impl->addGenome(genome);
}

int GenomeMatcher::minimumSearchLength() const
{
    return m_impl->minimumSearchLength();
}

bool GenomeMatcher::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
    return m_impl->findGenomesWithThisDNA(fragment, minimumLength, exactMatchOnly, matches);
}

bool GenomeMatcher::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    return m_impl->findRelatedGenomes(query, fragmentMatchLength, exactMatchOnly, matchPercentThreshold, results);
}
