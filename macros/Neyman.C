#include "TH2D.h"
#include "TH1D.h"
#include "TRandom3.h"
#include "TStyle.h"
#include "TCanvas.h"
using namespace std;

void Neyman(){
  cout<<"We study the Rayleigh distribution x*exp(-x^2/2) with parameter sigma = 1"<<endl;

  TH2D* hist2D = new TH2D("hist2D","hist2D",
			  201, 0, 4,
			  201, 0, 4);

  TH1D* hist1D = new TH1D("hist1D","hist1D",
			  201, 0, 4);

  //This defines the Rayleigh distribution to generate numbers from
  TF1 *f1 = new TF1("f1","x*exp(-x*x/2.)",0,4);		

  int Nmeas = 5;
  int Nexp  = 100000;

  double sigma_experiment = 1.;
  
  double sigma_true;
  double sigma_best;
  //double sigma = 1.;

  
  // loop through different sigma_true
  for(int i = 0; i <= 400; i++){
    sigma_true = double(i)/100.;

    for(int e = 0; e < Nexp; e++){
      sigma_best = 0.;
      // loop through measurements in experiment
      for(int m = 0; m < Nmeas; m++){
	double r = f1->GetRandom();
	sigma_best += r*r;
      }

      // our "measurement" for sigma best fit
      sigma_best = sqrt(sigma_best / (2.*double(Nmeas)));
      
      hist2D->Fill(sigma_true, sigma_best);
    }
  }

  // assume we do experiment and measure sigma_experiment

  // which bin in 2D histogram on y-axis matches measurement?
  int ibin = hist2D->GetYaxis()->FindBin(sigma_experiment);

  // Fill 1D histogram with slice from 2D corresponding to measurement
  for(int i = 0; i < 201; i++){
  hist1D->SetBinContent(i+1, hist2D->GetBinContent(i+1, ibin));
  }

  // normalize as probability distribution
  hist1D->Scale(1./hist1D->Integral());
  

  // some formating settings
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);

  TCanvas* can0 = (TCanvas*) new TCanvas("c0","c0",
					 450, 400);
  double hlo = 0.15;
  double hhi = 0.2;
  double hbo = 0.15;
  double hto = 0.07;
  can0->SetLeftMargin(hlo);
  can0->SetRightMargin(hhi);
  can0->SetBottomMargin(hbo);
  can0->SetTopMargin(hto);
  can0->SetGridx();
  can0->SetGridy();
  can0->cd();
  can0->Draw();
  
  hist2D->Draw("colz");
  hist2D->GetXaxis()->CenterTitle();
  hist2D->GetXaxis()->SetTitleSize(0.05);
  hist2D->GetYaxis()->CenterTitle();
  hist2D->GetYaxis()->SetTitleSize(0.05);
  hist2D->GetXaxis()->SetTitleOffset(1.1);
  hist2D->GetYaxis()->SetTitleOffset(1.2);
  hist2D->GetXaxis()->SetTitle("#sigma true");
  hist2D->GetYaxis()->SetTitle("#sigma meas");


  //////////////////////////////////////////

  TCanvas* can1 = (TCanvas*) new TCanvas("c1","c1",
					 450, 400);
  
  can1->SetLeftMargin(0.15);
  can1->SetRightMargin(0.1);
  can1->SetBottomMargin(hbo);
  can1->SetTopMargin(hto);
  can1->SetGridx();
  can1->SetGridy();
  can1->cd();
  can1->Draw();

  hist1D->SetLineColor(kBlue+2);
  hist1D->SetLineWidth(3);
  hist1D->SetFillColor(kBlue);
  hist1D->SetFillStyle(3004);
  hist1D->Draw("hist");

  hist1D->GetXaxis()->CenterTitle();
  hist1D->GetXaxis()->SetTitleSize(0.05);
  hist1D->GetYaxis()->CenterTitle();
  hist1D->GetYaxis()->SetTitleSize(0.05);
  hist1D->GetXaxis()->SetTitleOffset(1.1);
  hist1D->GetYaxis()->SetTitleOffset(1.3);
  hist1D->GetXaxis()->SetTitle("#sigma");
  hist1D->GetYaxis()->SetTitle("P( #sigma | #sigma_{meas})");
}
