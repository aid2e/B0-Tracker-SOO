
/*
 *
 * Calculates the resolution function for a given set of parameters.
 * 
 * @param x The input value.
 * @param params The array of parameters.
 *               params[0] - amplitude1: The amplitude of the first Gaussian.
 *               params[1] - mean: The mean value of the distribution.
 *               params[2] - sigma1: The standard deviation of the first Gaussian.
 *               params[3] - ratio: The ratio of the amplitudes of the two Gaussians.
 *               params[4] - sigma2: The standard deviation of the second Gaussian.
 * @return The value of the resolution function at the given input value.
*/

void BeautifyGraphError(TGraphErrors *gr, 
int color, int style = 1, int width = 2, 
int markerStyle = 22, int markerSize = 2
)
{
    gr->SetLineColor(color);
    gr->SetLineStyle(style);
    gr->SetLineWidth(width);
    gr->SetMarkerStyle(markerStyle);
    gr->SetMarkerSize(markerSize);
    gr->SetMarkerColor(color);
    gr->SetFillColor(color);
}

Double_t resolution_func(Double_t *x, Double_t *params) {
    // Extract parameters
    Double_t amplitude1 = params[0];
    Double_t mean = params[1];
    Double_t sigma1 = params[2];
    Double_t ratio = params[3];
    Double_t sigma2 = params[4];

    // Calculate the first Gaussian
    Double_t gauss1 = amplitude1 * TMath::Exp(-0.5 * TMath::Power((x[0] - mean) / sigma1, 2));

    // Calculate the amplitude of the second Gaussian
    Double_t amplitude2 = amplitude1 * ratio;

    // Calculate the second Gaussian
    Double_t gauss2 = amplitude2 * TMath::Exp(-0.5 * TMath::Power((x[0] - mean) / sigma2, 2));

    // Return the sum of the two Gaussians
    return gauss1 + gauss2;
}


/**
 * Plots the momentum and pT resolutions for protons in a given file.
 *
 * @param FileName The name of the input file.
 * @param outFileSig The output file signature (default is "out").
 */
void PlotMomentum(TString FileName, TString outFileSig = "")
{
    gStyle->SetTitleSize(0.08);
    gStyle->SetOptFit(11);

    TFile *f = new TFile(FileName.Data(),"READ");
    //if outFileSig is empty, replace .root with _out-fit.root
    if (outFileSig == "") outFileSig = FileName.ReplaceAll(".root", "_out-fit");
    TFile *outFile = new TFile(outFileSig + ".root", "RECREATE");
    outFile->mkdir("momentumFits");
    outFile->mkdir("pTResolutions");
    outFile->mkdir("momentumResols");

    outFile->cd("momentumFits");

    float _momMin = 80.0;
    float _momMax = 100.0;
    float MomRange = _momMax - _momMin;
    float MomResol = 1.0;
    const int nMomBins = (int)(MomRange/MomResol);
    float _pTMin = 0.20; // original 0.20;
    float _pTMax = 5.00; // original 5.00;
    float pTResol = 0.10;
    const int npTBins = (int)((_pTMax - _pTMin)/pTResol);
    float MomMin[nMomBins], MomMax[nMomBins];
    float pTMin[npTBins], pTMax[npTBins];
    for (int i=0; i<nMomBins; i++)
    {
        MomMin[i] = _momMin + i*MomResol;
        MomMax[i] = _momMin + (i+1)*MomResol;
    }
    for (int i=0; i<npTBins; i++)
    {
        pTMin[i] = _pTMin + i*pTResol;
        pTMax[i] = _pTMin + (i+1)*pTResol;
    }
    TH1F *h_mom_res[nMomBins];
    TH1F *h_pT_res[npTBins];
    TCanvas *canvasMom[nMomBins];
    TCanvas *canvaspT[npTBins];
    TF1 *f_mom_res[nMomBins];
    TF1 *f_pT_res[npTBins];
    float dev = 5;
    float mean = 0.;
    float sigma = 0.2;
    float resols[nMomBins];
    float resols_err[nMomBins];
    float A1;
    float A2;
    float sigma1;
    float sigma2;
    float pT_dev = 5;
    float pT_mean = 0.;
    float pT_sigma = 0.2;
    float pT_resols[npTBins];
    float pT_resols_err[npTBins];
    float pT_A1;
    float pT_A2;
    float pT_sigma1;
    float pT_sigma2;
    for (int i=0; i<nMomBins; i++)
    {
        h_mom_res[i] = (TH1F*)f->Get(Form("momentumFits/h_mom_res_%d",i));
        h_mom_res[i]->SetTitle(Form("protons %.2f < p < %.2f [GeV];#vec{p^{gen}};#frac{#Delta p}{p}", MomMin[i], MomMax[i]));
        h_mom_res[i]->Rebin(4);
        canvasMom[i] = new TCanvas(Form("canvasMom_%d",i), Form("canvasMom_%d",i));
        sigma = h_mom_res[i]->GetRMS();
        
        f_mom_res[i] = new TF1(Form("f_mom_res_%d",i),resolution_func,-dev*sigma,dev*sigma, 5);
        //f_mom_res[i]->SetParameters(h_mom_res[i]->GetMaximum()*0.8,mean,0.1*sigma, h_mom_res[i]->GetMaximum()*0.1,mean,0.5*sigma);
        f_mom_res[i]->SetParameters(h_mom_res[i]->GetMaximum()*0.8,mean,0.1, 0.5, 0.5*sigma);
        h_mom_res[i]->Fit(Form("f_mom_res_%d",i),"R");

        A1 = f_mom_res[i]->GetParameter(0);
        A2 = f_mom_res[i]->GetParameter(3);
        sigma1 = f_mom_res[i]->GetParameter(2);
        sigma2 = f_mom_res[i]->GetParameter(5);
        //resols[i] = (A1*sigma1 + A2*sigma2)/(A1 + A2);// (A1*sigma1 + A2*sigma2) / (A1 + A2)
        //resols_err[i] = (A1/(A1+A2))*f_mom_res[i]->GetParError(2) + (A2/(A1+A2))*f_mom_res[i]->GetParError(5);
        //resols_err[i]*=(f_mom_res[i]->GetChisquare()/f_mom_res[i]->GetNDF());
        resols[i] = h_mom_res[i]->GetRMS();
        resols_err[i] = h_mom_res[i]->GetRMSError();

    }
    for (int i=0; i<npTBins; i++)
    {
        h_pT_res[i] = (TH1F*)f->Get(Form("pTResolutions/h_pT_res_%d",i));
        h_pT_res[i]->Rebin(4);
        h_pT_res[i]->SetTitle(Form("protons %.2f < p_{T} < %.2f [GeV];#vec{p_{T}^{gen}};#frac{#Delta p_{T}}{p_{T}}", pTMin[i], pTMax[i]));
        canvaspT[i] = new TCanvas(Form("canvaspT_%d",i), Form("canvaspT_%d",i));
        pT_sigma = h_pT_res[i]->GetRMS();

        f_pT_res[i] = new TF1(Form("f_pT_res_%d",i),"gaus+gaus(3)",-pT_dev*pT_sigma,pT_dev*pT_sigma);
        f_pT_res[i]->SetParameters(h_pT_res[i]->GetMaximum()*0.8,pT_mean,0.1*pT_sigma, h_pT_res[i]->GetMaximum()*0.1,pT_mean,0.5*pT_sigma);
        h_pT_res[i]->Fit(Form("f_pT_res_%d",i),"R");

        pT_A1 = f_pT_res[i]->GetParameter(0);
        pT_A2 = f_pT_res[i]->GetParameter(3);
        pT_sigma1 = f_pT_res[i]->GetParameter(2);
        pT_sigma2 = f_pT_res[i]->GetParameter(5);
        pT_resols[i] = h_pT_res[i]->GetRMS();//(pT_A1*pT_sigma1 + pT_A2*pT_sigma2)/(pT_A1 + pT_A2);// (A1*sigma1 + A2*sigma2) / (A1 + A2)
        //pT_resols_err[i] = (pT_A1/(pT_A1+pT_A2))*f_pT_res[i]->GetParError(2) + (pT_A2/(pT_A1+pT_A2))*f_pT_res[i]->GetParError(5);
        //pT_resols_err[i]*=(f_pT_res[i]->GetChisquare()/f_pT_res[i]->GetNDF());  
        pT_resols_err[i] = h_pT_res[i]->GetRMSError();

    }
    for (int i = 0; i<nMomBins; i++)
    {
        canvasMom[i]->cd();
        h_mom_res[i]->Draw();
        canvasMom[i]->Write();
        if (i==0) canvasMom[i]->Print(outFileSig + "_mom_res.pdf(");
        else if (i==nMomBins-1) canvasMom[i]->Print(outFileSig + "_mom_res.pdf)");
        else canvasMom[i]->Print(outFileSig + "_mom_res.pdf");
    }
    outFile->cd("pTResolutions");
    for (int i = 0; i<npTBins; i++)
    {
        canvaspT[i]->cd();
        h_pT_res[i]->Draw();
        canvaspT[i]->Write();
        if (i==0) canvaspT[i]->Print(outFileSig + "_pT_res.pdf(");
        else if (i==npTBins-1) canvaspT[i]->Print(outFileSig + "_pT_res.pdf)");
        else canvaspT[i]->Print(outFileSig + "_pT_res.pdf");
    }

    
    float centerMomBins[nMomBins];
    float centerMomBinErrs[nMomBins];
    for (int i=0; i<nMomBins; i++)
    {
        centerMomBins[i] = (MomMin[i] + MomMax[i])/2;
        centerMomBinErrs[i] = (MomMax[i] - MomMin[i])/2;
    }
    float centerpTBins[npTBins];
    float centerpTBinErrs[npTBins];
    for (int i=0; i<npTBins; i++)
    {
        centerpTBins[i] = (pTMin[i] + pTMax[i])/2;
        centerpTBinErrs[i] = (pTMax[i] - pTMin[i])/2;
    }
    TGraphErrors *gr_res = new TGraphErrors(nMomBins, centerMomBins, resols, centerMomBinErrs, resols_err);
    gr_res->SetName(outFileSig + "_res");
    gr_res->SetTitle("Weighted p Resolution in Bins of p;#vec{p^{gen}};#frac{#sigma_{p}}{p}");
    gr_res->GetXaxis()->SetTitleSize(0.075);
    gr_res->GetYaxis()->SetTitleSize(0.075);
    gr_res->GetXaxis()->SetLabelSize(0.06);
    gr_res->GetYaxis()->SetLabelSize(0.06);
    gr_res->SetMaximum(0.4); // original gr_res->SetMaximum(gr_res->GetMaximum()*1.1); 
    gr_res->SetMinimum(0.0);
    TCanvas *c_res = new TCanvas(outFileSig + "_canvas_res", outFileSig + "_canvas_res", 1200, 800);
    outFile->cd("../momentumResols");
    c_res->cd();
    c_res->SetLeftMargin(0.2);
    c_res->SetBottomMargin(0.2);
    gr_res->SetMarkerStyle(20);
    gr_res->SetMarkerColor(kRed);
    gr_res->Draw("APE");
    c_res->SaveAs(outFileSig + "_weightedpres_inbinsofp.png");
    gr_res->Write();
    c_res->Write();
    c_res->Print(outFileSig + "_weightedpres_inbinsofp.pdf");

    

    TGraphErrors *gr_res_pT = new TGraphErrors(npTBins, centerpTBins, pT_resols, centerpTBinErrs, pT_resols_err);
    gr_res_pT->SetName(outFileSig + "_res_pT");
    gr_res_pT->SetTitle("Weighted pT Resolution in Bins of pT;#vec{p_{T}^{gen}};#frac{#sigma_{p_{T}}}{p_{T}}");
    gr_res_pT->GetXaxis()->SetTitleSize(0.075);
    gr_res_pT->GetYaxis()->SetTitleSize(0.075);
    gr_res_pT->GetXaxis()->SetLabelSize(0.06);
    gr_res_pT->GetYaxis()->SetLabelSize(0.06);
    gr_res_pT->SetMaximum(0.4); // gr_res_pT->SetMaximum(gr_res_pT->GetMaximum()*1.1); 
    gr_res_pT->SetMinimum(0.0);
    TCanvas *c_res_pT = new TCanvas(outFileSig + "_canvas_res_pT", outFileSig + "_canvas_res_pT", 1200, 800);
    c_res_pT->cd();
    c_res_pT->SetLeftMargin(0.2);
    c_res_pT->SetBottomMargin(0.2);
    gr_res_pT->SetMarkerStyle(20);
    gr_res_pT->SetMarkerColor(kRed);
    gr_res_pT->Draw("APE");
    c_res_pT->SaveAs(outFileSig + "_weightedpTres_bypTbin.png");
    gr_res_pT->Write();
    
    // Clone the histogram
    TH1* clonedHist = (TH1*)gr_res_pT->Clone();
    clonedHist->SetName("comparison_res_pT"); // Set a new name for the cloned histogram

    // Now you have gr_res_pT and clonedHist with different names, you can write both to the file
    clonedHist->Write();
    c_res_pT->Write();
    c_res_pT->Print(outFileSig + "_weightedpTres_bypTbin.pdf");
    


    outFile->Close();

    // Let us write the output from pT resolutions into a csv file
    ofstream pTResFile;
    pTResFile.open(outFileSig + "_pT_res.csv");
    pTResFile << "pT,Resolution,Error,ResolutionError" << endl;
    for (int i = 0; i < npTBins; i++)
    {
        pTResFile << centerpTBins[i] << "," << pT_resols[i] << "," << centerpTBinErrs[i] << "," << pT_resols_err[i] << endl;
    }
    pTResFile.close();

    // Let us write the output from momentum resolutions into a csv file
    ofstream momResFile;
    momResFile.open(outFileSig + "_mom_res.csv");
    momResFile << "p,Resolution,Error,ResolutionError" << endl;
    for (int i = 0; i < nMomBins; i++)
    {
        momResFile << centerMomBins[i] << "," << resols[i] << "," << centerMomBinErrs[i] << "," << resols_err[i] << endl;
    }
    momResFile.close();

    // compute weighted averages across all bins
    float totalMomRes = 0;
    float totalMomResErr = 0;

    for (int i = 0; i < nMomBins; i++)
    {
        totalMomRes += resols[i] / (resols_err[i] * resols_err[i]);
        totalMomResErr += 1 / (resols_err[i] * resols_err[i]);
    }

    totalMomRes /= totalMomResErr;
    totalMomResErr = sqrt(1 / totalMomResErr);

    // compute weighted averages across all pT bins
    float totalpTRes = 0;
    float totalpTResErr = 0;

    for (int i = 0; i < npTBins; i++)
    {
        if(pT_resols[i] == 0) continue;
        totalpTRes += pT_resols[i] / (pT_resols_err[i] * pT_resols_err[i]);
        totalpTResErr += 1 / (pT_resols_err[i] * pT_resols_err[i]);
    }

    totalpTRes /= totalpTResErr;
    totalpTResErr = sqrt(1 / totalpTResErr);

    // write this out as a json file

    ofstream momResJsonFile;
    momResJsonFile.open(outFileSig + "_mom_res.json");
    momResJsonFile << "{" << endl;
    momResJsonFile << "\"weightedMomentumResoltion\": " << totalMomRes << "," << endl;
    momResJsonFile << "\"weightedMomentumResolutionError\": " << totalMomResErr << "," << endl;
    momResJsonFile << "\"weightedpTResoltion\": " << totalpTRes << "," << endl;
    momResJsonFile << "\"weightedpTResolutionError\": " << totalpTResErr << endl;
    momResJsonFile << "}" << endl;

}
