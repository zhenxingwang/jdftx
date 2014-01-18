/*-------------------------------------------------------------------
Copyright 2013 Deniz Gunceler, Ravishankar Sundararaman

This file is part of JDFTx.

JDFTx is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

JDFTx is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with JDFTx.  If not, see <http://www.gnu.org/licenses/>.
-------------------------------------------------------------------*/

#ifndef JDFTX_ELECTRONIC_SCF_H
#define JDFTX_ELECTRONIC_SCF_H

#include <electronic/common.h>
#include <electronic/Everything.h>
#include <electronic/BandMinimizer.h>
#include <electronic/operators.h>

//! Self-Consistent Iteration for residual minimization
class SCF
{
public:
	SCF(Everything& e);
		
	//! Minimizes residual to achieve self-consistency
	void minimize();
private:
	Everything& e;
	
	matrix overlap; //! Overlap matrix of density/potential residuals
	std::vector<DataRptrCollection> pastVariables, pastResiduals; //!< History
	RealKernel kerkerMix, diisMetric; //!< convolution kernels for kerker preconditioning and the DIIS overlap metric
	
	//! Updates fillings and recomputes filling energies
	void updateFillings();
	
	//! Mix n,tau or Vscloc,Vtau
	void mixPlain();
	
	//! Uses direct inversion in the iterative subspace to extrapolate to a new density/potential
	void mixDIIS();
	
	DataRptrCollection getVariable() const; //!< get the current variables (density / potential, with kinetic components if required)
	void setVariable(DataRptrCollection); //!< set the current variables (density / potential, with kinetic components if required)
	
	double eigDiffRMS(const std::vector<diagMatrix>&, const std::vector<diagMatrix>&) const; //!< weigted RMS difference between two sets of eigenvalues
	void eigenShiftInit(); //!< initialize and check eigenShifts
	void eigenShiftApply(bool reverse);  //!< apply eigenshifts if false, and undo them if true

	//! Applies the single particle constraint on Vscloc
	void single_particle_constraint(double sp_constraint);
	
};

#endif