/* ============================================================================
 * Copyright (c) 2011, Michael A. Jackson (BlueQuartz Software)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the name of Michael A. Jackson nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
///////////////////////////////////////////////////////////////////////////////
// This code was partly written under US Air Force Contract FA8650-07-D-5800
///////////////////////////////////////////////////////////////////////////////

#include "AngReader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>


#include "AngConstants.h"


#define PI_OVER_2f       1.57079632679489661f
#define THREE_PI_OVER_2f 4.71238898038468985f
#define TWO_PIf          6.28318530717958647f
#define ONE_PIf          3.14159265358979323f

#define kBufferSize 1024


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AngReader::AngReader() :
m_UserOrigin(Ebsd::Ang::NoOrientation),
m_FileName(""),
m_NumberOfElements(0),
m_ManageMemory(true)
{
  // Init all the arrays to NULL
  m_Phi1 = NULL;
  m_Phi = NULL;
  m_Phi2 = NULL;
  m_Iq = NULL;
  m_Ci = NULL;
  m_PhaseData = NULL;
  m_X = NULL;
  m_Y = NULL;
  m_SEMSignal = NULL;
  m_Fit = NULL;

  m_NumFields = 8;
  // Initialize the map of header key to header value
  m_Headermap[Ebsd::Ang::TEMPIXPerUM] = AngHeaderEntry<float>::NewEbsdHeaderEntry(Ebsd::Ang::TEMPIXPerUM);
  m_Headermap[Ebsd::Ang::XStar] = AngHeaderEntry<float>::NewEbsdHeaderEntry(Ebsd::Ang::XStar);
  m_Headermap[Ebsd::Ang::YStar] = AngHeaderEntry<float>::NewEbsdHeaderEntry(Ebsd::Ang::YStar);
  m_Headermap[Ebsd::Ang::ZStar] = AngHeaderEntry<float>::NewEbsdHeaderEntry(Ebsd::Ang::ZStar);
  m_Headermap[Ebsd::Ang::WorkingDistance] = AngHeaderEntry<float>::NewEbsdHeaderEntry(Ebsd::Ang::WorkingDistance);
  m_Headermap[Ebsd::Ang::Grid] = AngStringHeaderEntry::NewEbsdHeaderEntry(Ebsd::Ang::Grid);
  m_Headermap[Ebsd::Ang::XStep] = AngHeaderEntry<float>::NewEbsdHeaderEntry(Ebsd::Ang::XStep);
  m_Headermap[Ebsd::Ang::YStep] = AngHeaderEntry<float>::NewEbsdHeaderEntry(Ebsd::Ang::YStep);
  m_Headermap[Ebsd::Ang::ZStep] = AngHeaderEntry<float>::NewEbsdHeaderEntry(Ebsd::Ang::ZStep); // NOT actually in the file>::NewEbsdHeaderEntry(); , but may be needed
  m_Headermap[Ebsd::Ang::ZPos] = AngHeaderEntry<float>::NewEbsdHeaderEntry(Ebsd::Ang::ZPos); // NOT actually in the file>::NewEbsdHeaderEntry(); , but may be needed
  m_Headermap[Ebsd::Ang::ZMax] = AngHeaderEntry<float>::NewEbsdHeaderEntry(Ebsd::Ang::ZMax); // NOT actually in the file>::NewEbsdHeaderEntry(); , but may be needed
  m_Headermap[Ebsd::Ang::NColsOdd] = AngHeaderEntry<int>::NewEbsdHeaderEntry(Ebsd::Ang::NColsOdd);
  m_Headermap[Ebsd::Ang::NColsEven] = AngHeaderEntry<int>::NewEbsdHeaderEntry(Ebsd::Ang::NColsEven);
  m_Headermap[Ebsd::Ang::NRows] = AngHeaderEntry<int>::NewEbsdHeaderEntry(Ebsd::Ang::NRows);
  m_Headermap[Ebsd::Ang::Operator] = AngStringHeaderEntry::NewEbsdHeaderEntry(Ebsd::Ang::Operator);
  m_Headermap[Ebsd::Ang::SampleId] = AngStringHeaderEntry::NewEbsdHeaderEntry(Ebsd::Ang::SampleId);
  m_Headermap[Ebsd::Ang::ScanId] = AngStringHeaderEntry::NewEbsdHeaderEntry(Ebsd::Ang::ScanId);


  // Give these values some defaults
  setNumOddCols(-1);
  setNumEvenCols(-1);
  setNumRows(-1);
}

// -----------------------------------------------------------------------------
//  Clean up any Memory that was allocated for this class
// -----------------------------------------------------------------------------
AngReader::~AngReader()
{
  deletePointers();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AngReader::initPointers(size_t numElements)
{
  size_t numBytes = numElements * sizeof(float);
  m_Phi1 = allocateArray<float > (numElements);
  m_Phi = allocateArray<float > (numElements);
  m_Phi2 = allocateArray<float > (numElements);
  m_Iq = allocateArray<float > (numElements);
  m_Ci = allocateArray<float > (numElements);
  m_PhaseData = allocateArray<int> (numElements);
  m_X = allocateArray<float > (numElements);
  m_Y = allocateArray<float > (numElements);
  m_SEMSignal = allocateArray<float > (numElements);
  m_Fit = allocateArray<float > (numElements);

  ::memset(m_Phi1, 0, numBytes);
  ::memset(m_Phi, 0, numBytes);
  ::memset(m_Phi2, 0, numBytes);
  ::memset(m_Iq, 0, numBytes);
  ::memset(m_Ci, 0, numBytes);
  ::memset(m_PhaseData, 0, numBytes);
  ::memset(m_X, 0, numBytes);
  ::memset(m_Y, 0, numBytes);
  ::memset(m_SEMSignal, 0, numBytes);
  ::memset(m_Fit, 0, numBytes);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AngReader::deletePointers()
{
  this->deallocateArrayData<float > (m_Phi1);
  this->deallocateArrayData<float > (m_Phi);
  this->deallocateArrayData<float > (m_Phi2);
  this->deallocateArrayData<float > (m_Iq);
  this->deallocateArrayData<float > (m_Ci);
  this->deallocateArrayData<int > (m_PhaseData);
  this->deallocateArrayData<float > (m_X);
  this->deallocateArrayData<float > (m_Y);
  this->deallocateArrayData<float > (m_SEMSignal);
  this->deallocateArrayData<float > (m_Fit);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int AngReader::readHeaderOnly()
{
  int err = 1;
  char buf[kBufferSize];
  std::ifstream in(m_FileName.c_str());
  m_headerComplete = false;
  if (!in.is_open())
  {
    std::cout << "Ang file could not be opened: " << m_FileName << std::endl;
    return -100;
  }

  m_CompleteHeader.clear();
  m_Phases.clear();

  while (!in.eof() && !m_headerComplete)
  {
    ::memset(buf, 0, kBufferSize);
    in.getline(buf, kBufferSize);
    parseHeaderLine(buf, kBufferSize);
    m_CompleteHeader.append(buf);
  }
  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int AngReader::readFile()
{
  int err = 1;
  char buf[kBufferSize];
  m_headerComplete = false;
  std::ifstream in(m_FileName.c_str());

  if (!in.is_open())
  {
    std::cout << "Ang file could not be opened: " << m_FileName << std::endl;
    return -100;
  }

  m_CompleteHeader.clear();
  m_Phases.clear();

  while (!in.eof() && !m_headerComplete)
  {
    ::memset(buf, 0, kBufferSize);
    in.getline(buf, kBufferSize);
    parseHeaderLine(buf, kBufferSize);
    if (m_headerComplete == false) {
      m_CompleteHeader.append(buf);
    }
  }

  // Delete any currently existing pointers
  deletePointers();
  // Initialize new pointers
  size_t numElements = 0;

  std::string grid = getGrid();

  int nOddCols = getNumOddCols();
  int nEvenCols = getNumEvenCols();
  int nRows = getNumRows();

  if (nRows < 1)
  {
    return -200;
  }
  else if (grid.find(Ebsd::Ang::SquareGrid) == 0)
  {
   // if (nCols > 0) { numElements = nRows * nCols; }
    if (nOddCols > 0) { numElements = nRows * nOddCols;/* nCols = nOddCols;*/ }
    else if (nEvenCols > 0) { numElements = nRows * nEvenCols; /* nCols = nEvenCols; */ }
    else { numElements = 0; }
  }
  else if (grid.find(Ebsd::Ang::HexGrid) == 0)
  {
    std::cout << "Ang Files with Hex Grids Are NOT currently supported." << std::endl;
    return -400;
  }
  else // Grid was not set
  {
    return -300;
  }

  initPointers(numElements);

  if (NULL == m_Phi1 || NULL == m_Phi || NULL == m_Phi2 || NULL == m_Iq || NULL == m_SEMSignal || NULL == m_Ci || NULL == m_PhaseData || m_X == NULL || m_Y == NULL)
  {
    return -1;
  }

  //double progress = 0.0;
  size_t totalDataRows = nRows * nEvenCols;
  size_t counter = 0;
  for(int row = 0; row < nRows && in.eof() == false; ++row)
  {
    for(int col = 0; col < nEvenCols && in.eof() == false; ++col)
    {
      this->readData(buf, nEvenCols, col, nRows, row, counter);
      // Read the next line of data
      in.getline(buf, kBufferSize);
      ++counter;
    }
  }


  if (counter != totalDataRows && in.eof() == true)
  {

    std::cout << "Premature End Of File reached.\n"
        << m_FileName
        << "\nNumRows=" << nRows << " nEvenCols=" << nEvenCols
        << "\ncounter=" << counter << " totalDataRows=" << totalDataRows
        << "\nTotal Data Points Read=" << counter << std::endl;
  }
  if (m_NumFields < 10)
  {
    this->deallocateArrayData<float > (m_Fit);
  }
  if (m_NumFields < 9)
  {
    this->deallocateArrayData<float > (m_SEMSignal);
  }
  return err;
}

// -----------------------------------------------------------------------------
//  Read the Header part of the ANG file
// -----------------------------------------------------------------------------
void AngReader::parseHeaderLine(char* buf, size_t length)
{
  if (buf[0] != '#')
  {
    m_headerComplete = true;
    return;
  }
  // Start at the first character and walk until you find another non-space character
  size_t i = 1;
  while(buf[i] == ' ')
  {
    ++i;
  }
  size_t wordStart = i;
  size_t wordEnd = i+1;
  while(1)
  {
    if (buf[i] == 45 || buf[i] == 95) { ++i; } // "-" or "_" character
    else if (buf[i] >= 65 && buf[i] <=90) { ++i; } // Upper case alpha character
    else if (buf[i] >= 97 && buf[i] <=122) {++i; } // Lower case alpha character
    else { break;}
  }
  wordEnd = i;

  std::string word( &(buf[wordStart]), wordEnd - wordStart);

  if (word.size() == 0)
  {
    return;
  }

  // If the word is "Phase" then we need to construct a "Phase" class and
  //  store all the meta data for the phase into that class. When we are done
  //  parsing data for the phase then stick the Phase instance into the header
  //  map or stick it into a vector<Phase::Pointer> and stick the vector into
  //  the map under the "Phase" key
  if (word.compare(Ebsd::Ang::Phase) == 0)
  {
    m_CurrentPhase = AngPhase::New();
    m_CurrentPhase->parsePhase(buf, wordEnd, length);
    // Parsing the phase is complete, now add it to the vector of Phases
    m_Phases.push_back(m_CurrentPhase);
  }
  else if (word.compare(Ebsd::Ang::MaterialName) == 0 && m_CurrentPhase.get() != NULL)
  {
    m_CurrentPhase->parseMaterialName(buf, wordEnd, length);
  }
  else if (word.compare(Ebsd::Ang::Formula) == 0 && m_CurrentPhase.get() != NULL)
  {
    m_CurrentPhase->parseFormula(buf, wordEnd, length);
  }
  else if (word.compare(Ebsd::Ang::Info) == 0 && m_CurrentPhase.get() != NULL)
  {
    m_CurrentPhase->parseInfo(buf, wordEnd, length);
  }
  else if (word.compare(Ebsd::Ang::Symmetry) == 0 && m_CurrentPhase.get() != NULL)
  {
    m_CurrentPhase->parseSymmetry(buf, wordEnd, length);
  }
  else if (word.compare(Ebsd::Ang::LatticeConstants) == 0 && m_CurrentPhase.get() != NULL)
  {
    m_CurrentPhase->parseLatticeConstants(buf, wordEnd, length);
  }
  else if (word.compare(Ebsd::Ang::NumberFamilies) == 0 && m_CurrentPhase.get() != NULL)
  {
    m_CurrentPhase->parseNumberFamilies(buf, wordEnd, length);
  }
  else if (word.compare(Ebsd::Ang::HKLFamilies) == 0 && m_CurrentPhase.get() != NULL)
  {
    m_CurrentPhase->parseHKLFamilies(buf, wordEnd, length);
  }
  else if (word.compare(Ebsd::Ang::Categories) == 0 && m_CurrentPhase.get() != NULL)
  {
    m_CurrentPhase->parseCategories(buf, wordEnd, length);
  }
  else
  {
    EbsdHeaderEntry::Pointer p = m_Headermap[word];
    if (NULL == p.get())
    {
      std::cout << "---------------------------" << std::endl;
      std::cout << "Could not find header entry for key'" << word << "'" << std::endl;
      std::string upper(word);
      std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
      std::cout << "#define ANG_" << upper << "     \"" << word << "\"" << std::endl;
      std::cout << "const std::string " << word << "(ANG_" << upper << ");" << std::endl;

      std::cout << "angInstanceProperty(AngHeaderEntry<float>. float, " << word << "Ebsd::Ang::" << word << std::endl;
      std::cout << "m_Headermap[Ebsd::Ang::" << word << "] = AngHeaderEntry<float>::NewEbsdHeaderEntry(Ebsd::Ang::" << word << ");" << std::endl;
      return;
    }
    else
    {
      p->parseValue(buf, wordEnd, length);
    }
  }

}

// -----------------------------------------------------------------------------
//  Read the data part of the ANG file
// -----------------------------------------------------------------------------
void AngReader::readData(const std::string &line,
                         int nCols, int col, int nRows, int row, size_t i)
{
  /* When reading the data there should be at least 8 cols of data. There may even
   * be 10 columns of data. The column names should be the following:
   * phi1
   * phi
   * phi2
   * x pos
   * y pos
   * image quality
   * confidence index
   * phase
   * SEM Signal
   * Fit of Solution
   *
   * Some TSL ang files do NOT have all 10 columns. Assume these are lacking the last
   * 2 columns and all the other columns are the same as above.
   */
  float p1, p, p2, x, y, iqual, conf, semSignal, fit;
  int ph;
  size_t offset = 0;
  m_NumFields = sscanf(line.c_str(), "%f %f %f %f %f %f %f %d %f %f", &p1, &p,&p2, &x, &y, &iqual, &conf, &ph, &semSignal, &fit);


  // Do we transform the data
  if (m_UserOrigin == Ebsd::Ang::UpperRightOrigin)
  {
    offset = (row*nCols)+((nCols-1)-col);
    if (p1 - PI_OVER_2f < 0.0)
    {
      p1 = p1 + THREE_PI_OVER_2f;
    }
    else
    {
      p1 = p1 - PI_OVER_2f;
    }
  }
  else if (m_UserOrigin == Ebsd::Ang::UpperLeftOrigin)
  {
    if (p1 + PI_OVER_2f > TWO_PIf)
    {
      p1 = p1 - THREE_PI_OVER_2f;
    }
    else
    {
      p1 = p1 + PI_OVER_2f;
    }
    if (p + ONE_PIf > TWO_PIf)
    {
      p = p - ONE_PIf;
    }
    else
    {
      p = p + ONE_PIf;
    }
  }
  else if (m_UserOrigin == Ebsd::Ang::LowerLeftOrigin)
  {
    offset = (((nRows-1)-row)*nCols)+col;
    if (p1 + PI_OVER_2f > TWO_PIf)
    {
      p1 = p1 - THREE_PI_OVER_2f;
    }
    else
    {
      p1 = p1 + PI_OVER_2f;
    }
  }
  else if (m_UserOrigin == Ebsd::Ang::LowerRightOrigin)
  {
    offset = (((nRows-1)-row)*nCols)+((nCols-1)-col);
  }

  if (m_UserOrigin == Ebsd::Ang::NoOrientation)
  {
    // If the user/programmer sets "NoOrientation" then we simply read the data
    // from the file and copy the values into the arrays without any regard for
    // the true X and Y positions in the grid. We are simply trying to keep the
    // data as close to the original as possible.
    offset = i;
  }
  m_Phi1[offset] = p1;
  m_Phi[offset] = p;
  m_Phi2[offset] = p2;
  m_Iq[offset] = iqual;
  m_Ci[offset] = conf;
  m_PhaseData[offset] = ph;
  m_X[offset] = x;
  m_Y[offset] = y;
  if (m_NumFields > 8) {
    m_SEMSignal[offset] = semSignal;
  }
  if (m_NumFields > 9)
  {
    m_Fit[offset] = fit;
  }
}

