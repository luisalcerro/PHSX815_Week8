 #include "Math/Minimizer.h"
 #include "Math/Factory.h"
 #include "Math/Functor.h"
 #include "TRandom2.h"
 #include "TError.h"
 #include <iostream>




///////Function definition///////
 double Cone(const double *xx )
 {
   const Double_t x = xx[0];
   const Double_t y = xx[1];
   const Double_t tmp1 = (x-5.)*(x-5.);
   const Double_t tmp2 = y*y;
   return sqrt(tmp1 + tmp2) - 100.;
 }
  
 int NumericalMinimization(const char * minName = "Minuit",
                           const char *algoName = "Migrad" ,
                           int randomSeed = -1)
 {
    // create minimizer giving a name and a name (optionally) for the specific
    // algorithm
    // possible choices are:
    //     minName                  algoName
    // Minuit /Minuit2             Migrad, Simplex,Combined,Scan  (default is Migrad)
    //  Minuit2                     Fumili2
    //  Fumili
    //  GSLMultiMin                ConjugateFR, ConjugatePR, BFGS,
    //                              BFGS2, SteepestDescent
    //  GSLMultiFit
    //   GSLSimAn
    //   Genetic

   
    ROOT::Math::Minimizer* minimum =
       ROOT::Math::Factory::CreateMinimizer(minName, algoName);
  
    // set tolerance , etc...
    minimum->SetMaxFunctionCalls(1000000); // for Minuit/Minuit2
    minimum->SetMaxIterations(10000);  // for GSL
    minimum->SetTolerance(0.001);
    minimum->SetPrintLevel(1);
  
    // create function wrapper for minimizer
    // a IMultiGenFunction type
    ROOT::Math::Functor f(&Cone,2);
    double step[2] = {0.01,0.01};
    // starting point
  
    double variable[2] = { -1.,1.2};
    if (randomSeed >= 0) {
       TRandom2 r(randomSeed);
       variable[0] = r.Uniform(-20,20);
       variable[1] = r.Uniform(-20,20);
    }
  
    minimum->SetFunction(f);
  
    // Set the free variables to be minimized !
    minimum->SetVariable(0,"x",variable[0], step[0]);
    minimum->SetVariable(1,"y",variable[1], step[1]);
  
    // do the minimization
    minimum->Minimize();
    std::cout<<"#############################"
	     <<"#############################"<<endl
	     <<"#############################"
	     <<"#############################"<<endl
	     <<"We are minimizing the cone z = ((x-5)^2+y^2)^(1/2)-100"<<endl<<endl
	     <<"The expected result is f(5,0) = -100"<<endl<<endl;
    const double *xs = minimum->X();
    std::cout << "Minimum: f(" << xs[0] << "," << xs[1] << "): "
              << minimum->MinValue()  << std::endl;
    
    // expected minimum is 0
    if ( minimum->MinValue()  < 1.E-4  && f(xs) < 1.E-4)
       std::cout << "Minimizer " << minName << " - " << algoName
                 << "   converged to the right minimum" << std::endl;
    else {
       std::cout << "Minimizer " << minName << " - " << algoName
                 << "   failed to converge !!!" << std::endl;
       Error("NumericalMinimization","fail to converge");
    }

/////////////PLOT THE CONE ////////////////////////////////

   TCanvas *c = new TCanvas("c","Graph2D example",0,0,600,400);
   Double_t x, y, z;
   Int_t np = 200000;
   TGraph2D *dt = new TGraph2D();
   dt->SetTitle("Cone; X ; Y ; Z ");
   TRandom *r = new TRandom();
   for (Int_t N=0; N<np; N++) {
      x = r->Uniform(3.,7.);
      y = r->Uniform(-2.,2.);
      z = sqrt((x-5.)*(x-5.)+y*y) - 100.;
      dt->SetPoint(N,x,y,z);
   }
   gStyle->SetPalette(51);
   dt->Draw("surf1");  
    return 0;
 }





