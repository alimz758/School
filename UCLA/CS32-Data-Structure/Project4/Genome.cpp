#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <istream>
#include <fstream> // needed to open files
#include "Trie.h"
using namespace std;

class GenomeImpl
{
public:
    GenomeImpl(const string& nm, const string& sequence);
    static bool load(istream& genomeSource, vector<Genome>& genomes);
    int length() const;
    ~GenomeImpl();
    string name() const;
    bool extract(int position, int length, string& fragment) const;
private:
    string m_name,
           m_sequence;
};

GenomeImpl::GenomeImpl(const string& nm, const string& sequence)
{
    /*
     The Genome constructor must initialize a new Genome object with the indicated name
     and DNA sequence. You implementation may assume the sequence contains at least one
     character, and all characters in the sequence are upper case A, C, T, G, or N (we'll explain
     N later). It should run in O(S) time, where S is the length of the longer string.
     */
    m_name=nm;
    m_sequence=sequence;
}
GenomeImpl:: ~GenomeImpl()
{
    //===================TO-DO=====================
    
}

bool GenomeImpl::load(istream& genomeSource, vector<Genome>& genomes)
{
    //while there is something in the file to read
    string s, name, seq;
    seq="";
    name="";
    //read each line in the file
    while(getline(genomeSource,s))
    {
//        Extract the genome name from a line in the file that begins with a greater than sign; everything following the greater-than
        //sign (and excluding the newline at the end of the line) is the name, so for the line
        //if the first char of the current line has starts with '>'
       // cout<< s << endl;
        if(s[0]=='>')
        {
           //when both name and seq got filled
           if(seq!="" && name!="")
           {
               //cout <<"seq:" <<seq <<endl;
               //create a new Genome with name and seq and push it to genomes
               genomes.push_back(Genome(name,seq));
               
                //make seq and name empty whenever found a new name;
               seq="";
               name="";
           }
            //if no char after the '>', return false
            if(s[1]==' ')
                return false;
            //ignore '>' from the beginning of the line, then copy the rest into name
            name=s.substr(1);
            //cout << name;
        }
        //if there is no base line after the name line, return false
        else if(s=="")
            return false;
//      Extract the sequence of DNA bases for that name from the file into one concatenated string. The bases are on the lines following the name up to but not including the next line starting with a greater-than sign (or the end of the file
        else 
        {
            for (int j=0; j<s.length();j++)
            {
                if(toupper(s[j])=='A' || toupper(s[j]) == 'C' || toupper(s[j]) == 'G'|| toupper(s[j]) == 'T' || toupper(s[j])=='N')
                    seq+=s[j];
                else
                    return false;
            }
        }
    }
    //push the last object
    genomes.push_back(Genome(name,seq));
    return true;
}

int GenomeImpl::length() const
{
//    The length() method returns the complete length of the DNA sequence, specifically the
//    total count of As, Cs, Ts, Gs (and Ns) found in the sequence held by the object. This
//    method must run in O(1) time
    return m_sequence.size();
}

string GenomeImpl::name() const
{
    return m_name;
}

bool GenomeImpl::extract(int position, int length, string& fragment) const
{
    //you try to extract a string that goes past the end of the genome
    if(position<0 || position+length> m_sequence.length())
        return false;
    fragment=m_sequence.substr(position, length);
    return true;
}

//******************** Genome functions ************************************

// These functions simply delegate to GenomeImpl's functions.
// You probably don't want to change any of this code.

Genome::Genome(const string& nm, const string& sequence)
{
    m_impl = new GenomeImpl(nm, sequence);
}

Genome::~Genome()
{
    delete m_impl;
}

Genome::Genome(const Genome& other)
{
    m_impl = new GenomeImpl(*other.m_impl);
}

Genome& Genome::operator=(const Genome& rhs)
{
    GenomeImpl* newImpl = new GenomeImpl(*rhs.m_impl);
    delete m_impl;
    m_impl = newImpl;
    return *this;
}

bool Genome::load(istream& genomeSource, vector<Genome>& genomes)
{
    return GenomeImpl::load(genomeSource, genomes);
}

int Genome::length() const
{
    return m_impl->length();
}

string Genome::name() const
{
    return m_impl->name();
}

bool Genome::extract(int position, int length, string& fragment) const
{
    return m_impl->extract(position, length, fragment);
}
