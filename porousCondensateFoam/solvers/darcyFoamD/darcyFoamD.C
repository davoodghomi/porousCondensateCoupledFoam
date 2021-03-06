/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | foam-extend: Open Source CFD
   \\    /   O peration     | Version:     4.0
    \\  /    A nd           | Web:         http://www.foam-extend.org
     \\/     M anipulation  | For copyright notice see file Copyright
-------------------------------------------------------------------------------
License
    This file is part of foam-extend.

    foam-extend is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    foam-extend is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with foam-extend.  If not, see <http://www.gnu.org/licenses/>.


Application
    darcyFoamD

Description
    Stationary solver for incompressible single-phase flow in porous medium

Developers
    Hamid Naderan
    Davood Ghomi

\*---------------------------------------------------------------------------*/


#include "fvCFD.H"
#include "wellcase.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include   "setRootCase.H"
    #include   "createTime.H"
    #include   "createMesh.H"
    #include   "readGravitationalAcceleration.H"
    #include   "createFields.H"
    # include  "createK_member.H"
    #include   "createWellbores.H"

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "\nStarting time loop\n" << endl;

    while (runTime.run())
    {
            # include "choosewellplan.H" 
            # include "setDeltaT.H"
            # include "readPositivePressureControls.H"
             
            runTime++;
            Info<< "Time = " << runTime.timeName() << nl << endl;
	    fvScalarMatrix pEqn
                (
                    fvm::laplacian(-Mf,p) + fvc::div(phig) + fvm::Sp(coefImplicitSourcePro1*coefPConstantPro1*WPro1/volumePro1,p)
                   + fvm::Sp(coefImplicitSourceInj1*coefPConstantInj1*WInj1/volumeInj1,p) - coefExplicitSourcePro1*explicitSourceTermPro1  
                   - coefExplicitSourceInj1*explicitSourceTermInj1  
                );
        
            pEqn.setReference(pRefCell, pRefValue);
	    pEqn.solve();
            U = ( -M & fvc::grad(p) ) + (M & g) * rho;
	    U.correctBoundaryConditions();
            //  surfaceScalarField phip("phip",Mf & mesh.Sf());
            
            runTime.write();
            Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
            << "  ClockTime = " << runTime.elapsedClockTime() << " s"
            << nl << endl;
    }

    return 0;
}

// ************************************************************************* //
