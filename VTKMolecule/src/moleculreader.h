#ifndef MOLECULREADER_H
#define MOLECULREADER_H

#include <stdio.h>
#include <openbabel/mol.h>


class MoleculReader
{
public:
    MoleculReader();
    OpenBabel::OBMol & getMol();
    void readToMol(const char* fileName);

private:
    OpenBabel::OBMol mol;
};

#endif // MOLECULREADER_H
