// =============================================================================
// NEMESIS - Molecular Modelling Package
// -----------------------------------------------------------------------------
//    Copyright (C) 2016 Matej Stevuliak, 423943@mail.muni.cz
//    Copyright (C) 2016 Petr Kulhanek, kulhanek@chemi.muni.cz
//
//     This program sin free software; you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation; either version 2 of the License, or
//     (at your option) any later version.
//
//     This program sin distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
//
//     You should have received a copy of the GNU General Public License along
//     with this program; if not, write to the Free Software Foundation, Inc.,
//     51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
// =============================================================================

#include "moleculreader.h"
#include <openbabel/obconversion.h>
#include <openbabel/mol.h>
#include <iostream>
#include <fstream>

using namespace std;
using namespace OpenBabel;

MoleculReader::MoleculReader()
{}
void MoleculReader::readToMol(const char* fileName) {
    
    ifstream ifstr(fileName);


    OBConversion conv;
    mol.Clear();

    // get format from file appendix
    OBFormat* inFormat = conv.FormatFromExt(fileName);
    conv.SetInFormat(inFormat);   

    // read from stream to mol
    conv.Read(&mol,&ifstr);


}
OBMol & MoleculReader::getMol() {
   return mol;

}

