// @(#)root/matrix:$Name:  $:$Id: TMatrixDUtils.cxx,v 1.12 2002/10/25 11:19:02 rdm Exp $
// Author: Fons Rademakers   03/11/97

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// Matrix utility classes.                                              //
//                                                                      //
// This file defines utility classes for the Linear Algebra Package.    //
// The following classes are defined here:                              //
//   TMatrixDAction                                                     //
//   TMatrixDPosAction                                                  //
//   TLazyMatrixD                                                       //
//   THaarMatrixD                                                       //
//   THilbertMatrixD                                                    //
//   TMatrixDRow                                                        //
//   TMatrixDColumn                                                     //
//   TMatrixDDiag                                                       //
//   TMatrixDPivoting                                                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TVectorD.h"
#include "TMatrixD.h"
#include "TClass.h"

Double_t TMatrixDRow::fgErr;
Double_t TMatrixDColumn::fgErr;
Double_t TMatrixDDiag::fgErr;
Double_t TMatrixDFlat::fgErr;


ClassImp(TLazyMatrixD)
ClassImp(TMatrixDRow)
ClassImp(TMatrixDColumn)
ClassImp(TMatrixDDiag)
ClassImp(TMatrixDFlat)

//______________________________________________________________________________
void TMatrixDRow::operator=(Double_t val)
{
   // Assign val to every element of the matrix row.

   if (!fMatrix->IsValid()) {
      Error("operator=", "matrix not initialized");
      return;
   }

   Double_t *rp = fPtr;                    // Row ptr
   for ( ; rp < fPtr + fMatrix->fNelems; rp += fInc)
      *rp = val;
}

//______________________________________________________________________________
void TMatrixDRow::operator+=(const TMatrixDRow &r)
{
   // Add to every element of the matrix row the
   // corresponding element of row r.

   if (!fMatrix->IsValid()) {
      Error("operator+=", "matrix not initialized");
      return;
   }
   if (!r.fMatrix->IsValid()) {
      Error("operator+=", "matrix r not initialized");
      return;
   }
   if (fMatrix->fColLwb != r.fMatrix->fColLwb) {
      Error("operator+=", "matrices have different column indices");
      return;
   }
   if (fMatrix->fNcols != r.fMatrix->fNcols) {
      Error("operator+=", "matrices have different # of columns");
      return;
   }

   Double_t *rp1 = fPtr;
   Double_t *rp2 = r.fPtr;
   for ( ; rp1 < fPtr + fMatrix->fNelems; rp1 += fInc, rp2 += r.fInc)
      *rp1 += *rp2;
}

//______________________________________________________________________________
void TMatrixDRow::operator*=(const TMatrixDRow &r)
{
   // Multiply every element of the matrix row with the
   // corresponding element of row r.

   if (!fMatrix->IsValid()) {
      Error("operator*=", "matrix not initialized");
      return;
   }
   if (!r.fMatrix->IsValid()) {
      Error("operator*=", "matrix r not initialized");
      return;
   }
   if (fMatrix->fColLwb != r.fMatrix->fColLwb) {
      Error("operator*=", "matrices have different column indices");
      return;
   }
   if (fMatrix->fNcols != r.fMatrix->fNcols) {
      Error("operator*=", "matrices have different # of columns");
      return;
   }

   Double_t *rp1 = fPtr;
   Double_t *rp2 = r.fPtr;
   for ( ; rp1 < fPtr + fMatrix->fNelems; rp1 += fInc, rp2 += r.fInc)
      *rp1 *= *rp2;
}

//______________________________________________________________________________
void TMatrixDRow::operator+=(Double_t val)
{
   // Add val to every element of the matrix row.

   if (!fMatrix->IsValid()) {
      Error("operator+=", "matrix not initialized");
      return;
   }

   Double_t *rp = fPtr;                    // Row ptr
   for ( ; rp < fPtr + fMatrix->fNelems; rp += fInc)
      *rp += val;
}

//______________________________________________________________________________
void TMatrixDRow::operator*=(Double_t val)
{
   // Multiply every element of the matrix row with val.

   if (!fMatrix->IsValid()) {
      Error("operator*=", "matrix not initialized");
      return;
   }

   Double_t *rp = fPtr;                    // Row ptr
   for ( ; rp < fPtr + fMatrix->fNelems; rp += fInc)
      *rp *= val;
}

//______________________________________________________________________________
void TMatrixDRow::operator=(const TVectorD &vec)
{
   // Assign a vector to a matrix row. The vector is considered row-vector
   // to allow the assignment in the strict sense.

   if (!fMatrix->IsValid()) {
      Error("operator=", "matrix not initialized");
      return;
   }
   if (!vec.IsValid()) {
      Error("operator=", "vector not initialized");
      return;
   }

   if (fMatrix->fColLwb != vec.fRowLwb || fMatrix->fNcols != vec.fNrows) {
      Error("operator=", "transposed vector cannot be assigned to a row of the matrix");
      return;
   }

   Double_t *rp = fPtr;                          // Row ptr
   Double_t *vp = vec.fElements;                 // Vector ptr
   for ( ; rp < fPtr + fMatrix->fNelems; rp += fInc)
      *rp = *vp++;

   Assert(vp == vec.fElements + vec.fNrows);
}

//______________________________________________________________________________
void TMatrixDRow::Streamer(TBuffer &R__b)
{
   // Stream an object of class TMatrixDRow.

   if (R__b.IsReading()) {
      UInt_t R__s, R__c;
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c);
      TMatrixDRow::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
      fPtr = &(fMatrix->fIndex[0][fRowInd]);
   } else {
      TMatrixDRow::Class()->WriteBuffer(R__b,this);
   }
}

//______________________________________________________________________________
void TMatrixDColumn::operator=(Double_t val)
{
   // Assign val to every element of the matrix column.

   if (!fMatrix->IsValid()) {
      Error("operator=", "matrix not initialized");
      return;
   }

   Double_t *cp = fPtr;                    // Column ptr
   while (cp < fPtr + fMatrix->fNrows)
      *cp++ = val;
}

//______________________________________________________________________________
void TMatrixDColumn::operator+=(const TMatrixDColumn &c)
{
   // Add to every element of the matrix column the
   // corresponding element of column r.

   if (!fMatrix->IsValid()) {
      Error("operator+=", "matrix not initialized");
      return;
   }
   if (!c.fMatrix->IsValid()) {
      Error("operator+=", "matrix c not initialized");
      return;
   }
   if (fMatrix->fRowLwb != c.fMatrix->fRowLwb) {
      Error("operator+=", "matrices have different row indices");
      return;
   }
   if (fMatrix->fNrows != c.fMatrix->fNrows) {
      Error("operator+=", "matrices have different # of rows");
      return;
   }

   Double_t *cp1 = fPtr;
   Double_t *cp2 = c.fPtr;
   while (cp1 < fPtr + fMatrix->fNrows)
      *cp1++ += *cp2++;
}

//______________________________________________________________________________
void TMatrixDColumn::operator*=(const TMatrixDColumn &c)
{
   // Multiply every element of the matrix column with the
   // corresponding element of column c.

   if (!fMatrix->IsValid()) {
      Error("operator*=", "matrix not initialized");
      return;
   }
   if (!c.fMatrix->IsValid()) {
      Error("operator*=", "matrix c not initialized");
      return;
   }
   if (fMatrix->fRowLwb != c.fMatrix->fRowLwb) {
      Error("operator*=", "matrices have different row indices");
      return;
   }
   if (fMatrix->fNrows != c.fMatrix->fNrows) {
      Error("operator*=", "matrices have different # of rows");
      return;
   }

   Double_t *cp1 = fPtr;
   Double_t *cp2 = c.fPtr;
   while (cp1 < fPtr + fMatrix->fNrows)
      *cp1++ *= *cp2++;
}

//______________________________________________________________________________
void TMatrixDColumn::operator+=(Double_t val)
{
   // Add val to every element of the matrix column.

   if (!fMatrix->IsValid()) {
      Error("operator+=", "matrix not initialized");
      return;
   }

   Double_t *cp = fPtr;                    // Column ptr
   while (cp < fPtr + fMatrix->fNrows)
      *cp++ += val;
}

//______________________________________________________________________________
void TMatrixDColumn::operator*=(Double_t val)
{
   // Multiply every element of the matrix column with val.

   if (!fMatrix->IsValid()) {
      Error("operator*=", "matrix not initialized");
      return;
   }

   Double_t *cp = fPtr;                    // Column ptr
   while (cp < fPtr + fMatrix->fNrows)
      *cp++ *= val;
}

//______________________________________________________________________________
void TMatrixDColumn::operator=(const TVectorD &vec)
{
   // Assign a vector to a matrix column.

   if (!fMatrix->IsValid()) {
      Error("operator=", "matrix not initialized");
      return;
   }
   if (!vec.IsValid()) {
      Error("operator=", "vector not initialized");
      return;
   }

   if (fMatrix->fRowLwb != vec.fRowLwb || fMatrix->fNrows != vec.fNrows) {
      Error("operator=", "vector cannot be assigned to a column of the matrix");
      return;
   }

   Double_t *cp = fPtr;                       // Column ptr
   Double_t *vp = vec.fElements;              // Vector ptr
   while (cp < fPtr + fMatrix->fNrows)
      *cp++ = *vp++;

   Assert(vp == vec.fElements + vec.fNrows);
}

//______________________________________________________________________________
void TMatrixDColumn::Streamer(TBuffer &R__b)
{
   // Stream an object of class TMatrixDColumn.

   if (R__b.IsReading()) {
      UInt_t R__s, R__c;
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c);
      TMatrixDColumn::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
      fPtr = &(fMatrix->fIndex[fColInd][0]);
   } else {
      TMatrixDColumn::Class()->WriteBuffer(R__b,this);
   }
}

//______________________________________________________________________________
void TMatrixDDiag::operator=(Double_t val)
{
   // Assign val to every element of the matrix diagonal.

   if (!fMatrix->IsValid()) {
      Error("operator=", "matrix not initialized");
      return;
   }

   Double_t *dp = fPtr;                // Diag ptr
   Int_t i;
   for (i = 0; i < fNdiag; i++, dp += fInc)
      *dp = val;
}

//______________________________________________________________________________
void TMatrixDDiag::operator+=(const TMatrixDDiag &d)
{
   // Add to every element of the matrix diagonal the
   // corresponding element of diagonal d.

   if (!fMatrix->IsValid()) {
      Error("operator+=", "matrix not initialized");
      return;
   }
   if (!d.fMatrix->IsValid()) {
      Error("operator+=", "matrix d not initialized");
      return;
   }
   if (fNdiag != d.fNdiag) {
      Error("operator+=", "matrices have different diagonal length");
      return;
   }

   Double_t *dp1 = fPtr;
   Double_t *dp2 = d.fPtr;
   Int_t i;
   for (i = 0; i < fNdiag; i++, dp1 += fInc, dp2 += d.fInc)
      *dp1 += *dp2;
}

//______________________________________________________________________________
void TMatrixDDiag::operator*=(const TMatrixDDiag &d)
{
   // Multiply every element of the matrix diagonal with the
   // corresponding element of diagonal d.

   if (!fMatrix->IsValid()) {
      Error("operator*=", "matrix not initialized");
      return;
   }
   if (!d.fMatrix->IsValid()) {
      Error("operator*=", "matrix d not initialized");
      return;
   }
   if (fNdiag != d.fNdiag) {
      Error("operator*=", "matrices have different diagonal length");
      return;
   }

   Double_t *dp1 = fPtr;
   Double_t *dp2 = d.fPtr;
   Int_t i;
   for (i = 0; i < fNdiag; i++, dp1 += fInc, dp2 += d.fInc)
      *dp1 *= *dp2;
}

//______________________________________________________________________________
void TMatrixDDiag::operator+=(Double_t val)
{
   // Assign val to every element of the matrix diagonal.

   if (!fMatrix->IsValid()) {
      Error("operator+=", "matrix not initialized");
      return;
   }

   Double_t *dp = fPtr;                // Diag ptr
   Int_t i;
   for (i = 0; i < fNdiag; i++, dp += fInc)
      *dp += val;
}

//______________________________________________________________________________
void TMatrixDDiag::operator*=(Double_t val)
{
   // Multiply every element of the matrix diagonal with val.

   if (!fMatrix->IsValid()) {
      Error("operator*=", "matrix not initialized");
      return;
   }

   Double_t *dp = fPtr;                // Diag ptr
   Int_t i;
   for (i = 0; i < fNdiag; i++, dp += fInc)
      *dp *= val;
}

//______________________________________________________________________________
void TMatrixDDiag::operator=(const TVectorD &vec)
{
   // Assign a vector to the matrix diagonal.

   if (!fMatrix->IsValid()) {
      Error("operator=", "matrix not initialized");
      return;
   }
   if (!vec.IsValid()) {
      Error("operator=", "vector not initialized");
      return;
   }

   if (fNdiag != vec.fNrows) {
      Error("operator=", "vector cannot be assigned to the matrix diagonal");
      return;
   }

   Double_t *dp = fPtr;                       // Diag ptr
   Double_t *vp = vec.fElements;              // Vector ptr
   for ( ; vp < vec.fElements + vec.fNrows; dp += fInc)
      *dp = *vp++;

   Assert(dp < fPtr + fMatrix->fNelems + fInc);
}

//______________________________________________________________________________
void TMatrixDDiag::Streamer(TBuffer &R__b)
{
   // Stream an object of class TMatrixDDiag.

   if (R__b.IsReading()) {
      UInt_t R__s, R__c;
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c);
      TMatrixDDiag::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
      fPtr = &(fMatrix->fElements[0]);
   } else {
      TMatrixDDiag::Class()->WriteBuffer(R__b,this);
   }
}

//______________________________________________________________________________
void TMatrixDFlat::operator=(Double_t val)
{
   // Assign val to every element of the matrix.

   if (!fMatrix->IsValid()) {
      Error("operator=", "matrix not initialized");
      return;
   }

   Double_t *fp = fPtr;
   while (fp < fPtr + fMatrix->fNelems)
      *fp++ = val;
}

//______________________________________________________________________________
void TMatrixDFlat::operator+=(const TMatrixDFlat &mf)
{
   // Add to every element of the matrix the
   // corresponding element of matrix mf.

   if (AreCompatible(*fMatrix,*mf.fMatrix)) {
      Error("operator*=", "matrices are not compatible");
      return;
   }

   Double_t *fp1 = fPtr;
   Double_t *fp2 = mf.fPtr;
   while (fp1 < fPtr + fMatrix->fNelems)
      *fp1++ += *fp2++;
}

//______________________________________________________________________________
void TMatrixDFlat::operator*=(const TMatrixDFlat &mf)
{
   // Multiply every element of the matrix with the
   // corresponding element of diagonal mf.

   if (AreCompatible(*fMatrix,*mf.fMatrix)) {
      Error("operator*=", "matrices are not compatible");
      return;
   }

   Double_t *fp1 = fPtr;
   Double_t *fp2 = mf.fPtr;
   while (fp1 < fPtr + fMatrix->fNelems)
      *fp1++ *= *fp2++;
}

//______________________________________________________________________________
void TMatrixDFlat::operator+=(Double_t val)
{
   // Add val to every element of the matrix.

   if (!fMatrix->IsValid()) {
      Error("operator+=", "matrix not initialized");
      return;
   }

   Double_t *fp = fPtr;
   while (fp < fPtr + fMatrix->fNelems)
      *fp++ += val;
}

//______________________________________________________________________________
void TMatrixDFlat::operator*=(Double_t val)
{
   // Multiply every element of the matrix with val.

   if (!fMatrix->IsValid()) {
      Error("operator*=", "matrix not initialized");
      return;
   }

   Double_t *fp = fPtr;
   while (fp < fPtr + fMatrix->fNelems)
      *fp++ *= val;
}

//______________________________________________________________________________
void TMatrixDFlat::operator=(const TVectorD &vec)
{
   // Assign a vector to the matrix. The matrix is
   // traversed column-wise

   if (!fMatrix->IsValid()) {
      Error("operator=", "matrix not initialized");
      return;
   }
   if (!vec.IsValid()) {
      Error("operator=", "vector not initialized");
      return;
   }

   if (fMatrix->fNelems != vec.fNrows) {
      Error("operator=", "vector cannot be assigned to the matrix");
      return;
   }

   Double_t *fp = fPtr;
   Double_t *vp = vec.fElements;
   while (fp < fPtr + fMatrix->fNelems)
      *fp++ = *vp++;
}

//______________________________________________________________________________
void TMatrixDFlat::Streamer(TBuffer &R__b)
{
   // Stream an object of class TMatrixDFlat.

   if (R__b.IsReading()) {
      UInt_t R__s, R__c;
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c);
      TMatrixDFlat::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
      fPtr = &(fMatrix->fElements[0]);
   } else {
      TMatrixDFlat::Class()->WriteBuffer(R__b,this);
   }
}

//______________________________________________________________________________
TMatrixDPivoting::TMatrixDPivoting(const TMatrixD &m)
    : TMatrixD(m), fRowIndex(new Index_t[fNrows]), fPivotRow(0), fPivotCol(0)
{
   Assert(fRowIndex != 0);

   Index_t rp = fElements;               // Fill in the row_index
   for (Index_t *rip = fRowIndex; rip < fRowIndex+fNrows; )
      *rip++ = rp++;
}

//______________________________________________________________________________
TMatrixDPivoting::~TMatrixDPivoting()
{
   delete [] fRowIndex;
}

//______________________________________________________________________________
void TMatrixDPivoting::PickUpPivot()
{
   // Pick up a pivot, an element with the largest abs value from yet
   // not-pivoted rows and cols

   Double_t max_elem = -1;                // Abs value of the largest element
   Index_t *prpp = 0;                   // Position of the pivot in fRowIndex
   Index_t *pcpp = 0;                   // Position of the pivot in fIndex

   Int_t col_odd = 0;                   // Parity of the current column

   for (Index_t *cpp = fIndex; cpp < fIndex + fNcols; cpp++) {
      const Double_t *cp = *cpp;          // Column pointer for the curr col
      if (cp == 0)                      // skip over already pivoted col
         continue;
      Int_t row_odd = 0;                // Parity of the current row
      for (Index_t *rip = fRowIndex; rip < fRowIndex + fNrows; rip++, cp++)
         if (*rip) {                    // only if the row hasn't been pivoted
            const Double_t v = *cp;
            if (TMath::Abs(v) > max_elem) {
               max_elem = TMath::Abs(v); // Note the local max of col elements
               fPivotValue = v;
               prpp = rip;
               pcpp = cpp;
               fPivotOdd = row_odd ^ col_odd;
            }
            row_odd ^= 1;                // Toggle parity for the next row
         }
      col_odd ^= 1;
   }

   if (max_elem < 0 || prpp == 0 || pcpp == 0)
      Error("PickUpPivot", "all the rows and columns have been already pivoted and eliminated");

   fPivotRow = *prpp, *prpp = 0;
   fPivotCol = *pcpp, *pcpp = 0;
}

//______________________________________________________________________________
Double_t TMatrixDPivoting::PivotingAndElimination()
{
   // Perform pivoting and gaussian elemination, return the pivot value
   // times pivot parity. The procedure places zeros to the fPivotRow of
   // all not yet pivoted columns. A[i,j] -= A[i,pivot_col]/pivot*A[pivot_row,j]

   PickUpPivot();

   if (fPivotValue == 0)
      return 0;

   Assert(fPivotRow != 0 && fPivotCol != 0);

   Double_t *pcp;                         // Pivot column pointer
   const Index_t *rip;                  // Current ptr in row_index
                                        // Divide the pivoted column by pivot
   for (pcp = fPivotCol, rip = fRowIndex; rip < fRowIndex+fNrows; pcp++, rip++)
     if (*rip)                          // Skip already pivoted rows
        *pcp /= fPivotValue;

   // Eliminate all the elements from the pivot_row in all not pivoted columns
   const Double_t *prp = fPivotRow;
   for (const Index_t *cpp = fIndex; cpp < fIndex + fNcols; cpp++, prp += fNrows) {
      Double_t *cp = *cpp;                // A[*,j]
      if (cp == 0)                      // skip over already pivoted col
         continue;
      const Double_t fac = *prp;        // fac = A[pivot_row,j]
                                        // Do elimination stepping over pivoted rows
      for (pcp = fPivotCol, rip = fRowIndex; rip < fRowIndex+fNrows; pcp++, cp++, rip++)
         if (*rip)
            *cp -= *pcp * fac;
   }

   return fPivotOdd ? -fPivotValue : fPivotValue;
}

//______________________________________________________________________________
void MakeHaarMatrixD(TMatrixD &m)
{
   // Create an orthonormal (2^n)*(no_cols) Haar (sub)matrix, whose columns
   // are Haar functions. If no_cols is 0, create the complete matrix with
   // 2^n columns. Example, the complete Haar matrix of the second order is:
   // column 1: [ 1  1  1  1]/2
   // column 2: [ 1  1 -1 -1]/2
   // column 3: [ 1 -1  0  0]/sqrt(2)
   // column 4: [ 0  0  1 -1]/sqrt(2)
   // Matrix m is assumed to be zero originally.

   if (!m.IsValid()) {
      Error("MakeHaarMatrixD", "matrix not initialized");
      return;
   }

   if (m.fNcols > m.fNrows || m.fNcols <= 0) {
      Error("MakeHaarMatrixD", "matrix not right shape");
      return;
   }

   Double_t *cp = m.fElements;
   Double_t *m_end = m.fElements + (m.fNrows*m.fNcols);
   Int_t i;

   Double_t norm_factor = 1/TMath::Sqrt((Double_t)m.fNrows);

   // First column is always 1 (up to normalization)
   for ( i = 0; i < m.fNrows; i++)
      *cp++ = norm_factor;

   // The other functions are kind of steps: stretch of 1 followed by the
   // equally long stretch of -1. The functions can be grouped in families
   // according to their order (step size), differing only in the location
   // of the step
   Int_t step_length = m.fNrows/2;
   while (cp < m_end && step_length > 0) {
      for (Int_t step_position = 0; cp < m_end && step_position < m.fNrows;
           step_position += 2*step_length, cp += m.fNrows) {
         Double_t *ccp = cp + step_position;
         for (i = 0; i < step_length; i++)
            *ccp++ = norm_factor;
         for (i = 0; i < step_length; i++)
            *ccp++ = -norm_factor;
      }
      step_length /= 2;
      norm_factor *= TMath::Sqrt(2.0);
   }

   Assert(step_length != 0 || cp == m_end);
   Assert(m.fNrows != m.fNcols || step_length == 0);
}

//______________________________________________________________________________
THaarMatrixD::THaarMatrixD(Int_t order, Int_t no_cols)
    : TLazyMatrixD(1<<order, no_cols == 0 ? 1<<order : no_cols)
{
   Assert(order > 0 && no_cols >= 0);
}

//______________________________________________________________________________
void THaarMatrixD::FillIn(TMatrixD &m) const
{
   MakeHaarMatrixD(m);
}

//______________________________________________________________________________
void MakeHilbertMatrixD(TMatrixD &m)
{
   // Make a Hilbert matrix. Hilb[i,j] = 1/(i+j-1), i,j=1...max, OR
   // Hilb[i,j] = 1/(i+j+1), i,j=0...max-1 (matrix need not be a square one).
   // The matrix is traversed in the natural (that is, column by column) order.

   if (!m.IsValid()) {
      Error("MakeHilbertMatrixD", "matrix not initialized");
      return;
   }

   if (m.fNrows <= 0 || m.fNcols <= 0) {
      Error("MakeHilbertMatrixD", "matrix not right shape");
      return;
   }

   Double_t *cp = m.fElements;
   Int_t i, j;

   for (j = 0; j < m.fNcols; j++)
      for (i = 0; i < m.fNrows; i++)
         *cp++ = 1./(i+j+1);
}

//______________________________________________________________________________
THilbertMatrixD::THilbertMatrixD(Int_t no_rows, Int_t no_cols)
    : TLazyMatrixD(no_rows, no_cols)
{
   Assert(no_rows > 0 && no_cols > 0);
}

//______________________________________________________________________________
THilbertMatrixD::THilbertMatrixD(Int_t row_lwb, Int_t row_upb, Int_t col_lwb, Int_t col_upb)
    : TLazyMatrixD(row_lwb, row_upb, col_lwb, col_upb)
{
   Assert(row_upb-row_lwb+1 > 0 && col_upb-col_lwb+1 > 0);
}

//______________________________________________________________________________
void THilbertMatrixD::FillIn(TMatrixD &m) const
{
   MakeHilbertMatrixD(m);
}
