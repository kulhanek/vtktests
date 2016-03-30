#include "moleculreader.h"
#include <openbabel/obconversion.h>
#include <openbabel/mol.h>
#include <iostream>
#include <fstream>

using namespace std;
using namespace OpenBabel;

MoleculReader::MoleculReader()
{
}
void MoleculReader::readToMol(const char* fileName){
    //OBFormat *inFormat;
    ifstream ifstr(fileName);


    OBConversion conv;
    mol.Clear();

//    OBFormat* inFormat = conv.FormatFromExt(fileName);
    conv.SetInFormat("xyz"/*inFormat*/);
    conv.Read(&mol,&ifstr);
   



}
OBMol & MoleculReader::getMol(){
   return mol;

}

