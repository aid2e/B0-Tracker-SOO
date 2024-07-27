void SimpleAnalysis(TString FileName, TString outFileName = "out.root")
{
    gROOT->Reset();
    TFile *f = new TFile(FileName.Data(),"READ");
    TFile *outFile = new TFile(outFileName.Data(),"RECREATE");
    outFile->mkdir("basicPlots");
    outFile->mkdir("momentumFits");
    outFile->mkdir("pTResolutions");
    TTree *events = (TTree*)f->Get("events");
    int NEVENTS = events->GetEntries();
    TTreeReader tr(events);

    TTreeReaderArray<float> gen_px(tr,"GeneratedParticles.momentum.x");
    TTreeReaderArray<float> gen_py(tr,"GeneratedParticles.momentum.y");
    TTreeReaderArray<float> gen_pz(tr,"GeneratedParticles.momentum.z");
    TTreeReaderArray<float> gen_energy(tr,"GeneratedParticles.energy");
    TTreeReaderArray<float> gen_m(tr,"GeneratedParticles.mass");
    TTreeReaderArray<int> gen_pdgid(tr,"GeneratedParticles.PDG");

    TTreeReaderArray<float> rec_px(tr,"ReconstructedChargedParticles.momentum.x");
    TTreeReaderArray<float> rec_py(tr,"ReconstructedChargedParticles.momentum.y");
    TTreeReaderArray<float> rec_pz(tr,"ReconstructedChargedParticles.momentum.z");
    TTreeReaderArray<float> rec_energy(tr,"ReconstructedChargedParticles.energy");
    TTreeReaderArray<float> rec_m(tr,"ReconstructedChargedParticles.mass");
    TTreeReaderArray<int> rec_pdgid(tr,"ReconstructedChargedParticles.PDG");
    TTreeReaderArray<float> rec_charge(tr,"ReconstructedChargedParticles.charge");

    TH1D* matchedPartMom = new TH1D("matchedPartMom", "Momentum of Thrown Charged Particles (truth), with matching track; P(GeV/c)", 20, 80, 100);
    TH1D* matchedPartTransMom = new TH1D("matchedPartTransMom", "Transverse Momentum of Thrown Charged Particles (truth), with matching track; P_{T}(GeV/c)", 48, 0.2, 5.0);
    TH1D *matchedPartTrackMomRes = new TH1D("matchedPartTrackMomRes","#Delta P/P Between Matching Thrown and Reconstructed Charged Particle; #Delta P/P", 200, -0.5, 0.5); 
    TH1D *matchedPartTrackTransMomRes = new TH1D("matchedPartTrackTransMomRes","#Delta P_{T}/P_{T} Between Matching Thrown and Reconstructed Charged Particle; #Delta P_{T}/P_{T}", 200, -0.5, 0.5);   

    
    float _momMin = 80.0;
    float _momMax = 100.0;
    float MomRange = _momMax - _momMin;
    float MomResol = 1.0;
    int nMomBins = (int)(MomRange/MomResol);
    float MomMin[nMomBins], MomMax[nMomBins];
    

    float _pTMin = 0.20;
    float _pTMax = 5.00;
    float pTRange = _pTMax - _pTMin;
    float pTResol = 0.10;
    int npTBins = (int)(pTRange/pTResol);
    float pTMin[npTBins], pTMax[npTBins];
    
    cout << "Number of bins in MOmRage is " << nMomBins << endl;
    for (int i=0; i<nMomBins; i++)
    {
        MomMin[i] = _momMin + i*MomResol;
        MomMax[i] = _momMin + (i+1)*MomResol;
        cout << MomMin[i] << " " << MomMax[i] << endl;
    }
    for (int i=0; i<npTBins; i++)
    {
        pTMin[i] = _pTMin + i*pTResol;
        pTMax[i] = _pTMin + (i+1)*pTResol;
    }
    TH1F* h_mom_res[nMomBins];
    TH1F* h_pT_res[npTBins];
    //TF1 *f_mom_res[nMomBins];
    
    for (int j=0; j<nMomBins; j++)
    {
        // the extent is _momMin + _momMax*MomResol < p < _momMin + (_momMax+1)*MomResol
        float _min = _momMin + j*MomResol;
        float _max = _momMin + (j+1)*MomResol;
        h_mom_res[j] = new TH1F(Form("h_mom_res_%d",j),Form("%10.2f #leq p < %10.2f;p;Counts",_min, _max),1000,-0.5, 0.5);
        cout << h_mom_res[j]->GetName() << endl;
    }
    

    for (int i=0; i<npTBins; i++)
    {
        // the extent is _pTMin + _pTMax*pTResol < p < _pTMin + (_pTMax+1)*pTResol
        float _min = _pTMin + i*pTResol;
        float _max = _pTMin + (i+1)*pTResol;
        h_pT_res[i] = new TH1F(Form("h_pT_res_%d",i),Form("%10.2f #leq #p_{T} < %10.2f;p_{T};Counts",_min, _max),1000,-0.5, 0.5);
        cout << h_pT_res[i]->GetName() << endl;
    }
    int ev = 0;
    while (tr.Next())
    {
        if (ev+1%100==0) {cout << "Event: " << ev << \
        ", gen_pdg size: " << gen_pdgid.GetSize() << \
        ", recon_pdg size:" << rec_pdgid.GetSize() << endl;}

        if (gen_pdgid.GetSize()!=rec_pdgid.GetSize()) {continue;}
        for (int i=0; i<gen_pdgid.GetSize(); i++)
        {
            //if (gen_pdgid[i]!=rec_pdgid[i] || gen_pdgid[i]!=2212) {continue;}
            if ((rec_m[i] > 1.0 ) || (rec_m[i] < 0.85) || rec_charge[i]<0.9 || rec_charge[i]>1.1 || gen_pdgid[i]!=2212) {continue;}
            float gen_mom = sqrt(gen_px[i]*gen_px[i] + gen_py[i]*gen_py[i] + gen_pz[i]*gen_pz[i]);
            float rec_mom = sqrt(rec_px[i]*rec_px[i] + rec_py[i]*rec_py[i] + rec_pz[i]*rec_pz[i]);
            
            matchedPartMom->Fill(rec_mom);
            matchedPartTrackMomRes->Fill((rec_mom-gen_mom)/gen_mom);
    

            for (int j=0; j<nMomBins; j++)
            {
                if (gen_mom>=MomMin[j] && gen_mom<MomMax[j])
                {
                    h_mom_res[j]->Fill((rec_mom-gen_mom)/gen_mom);
                }
            }
            
            float gen_pT = sqrt(gen_px[i]*gen_px[i] + gen_py[i]*gen_py[i]);
            float rec_pT = sqrt(rec_px[i]*rec_px[i] + rec_py[i]*rec_py[i]);
            matchedPartTransMom->Fill(rec_pT);
            matchedPartTrackTransMomRes->Fill((rec_pT-gen_pT)/gen_pT);

            for (int j=0; j<npTBins; j++)
            {
                if (gen_pT>=pTMin[j] && gen_pT<pTMax[j])
                {
                    h_pT_res[j]->Fill((rec_pT-gen_pT)/gen_pT);
                }
            }
            
        }
        ev++;

    }

    //TCanvas *c = new TCanvas();
    //c->cd();
    //h_mom_res->Draw();
    //c->SaveAs("mom_res.png");
    outFile->cd("basicPlots");
    matchedPartMom->Write();
    matchedPartTransMom->Write();
    matchedPartTrackMomRes->Write();
    matchedPartTrackTransMomRes->Write();

    TCanvas *c = new TCanvas();
    c->cd();
    matchedPartMom->Draw();
    c->SaveAs(FileName + "matchedPartMom.png");
    matchedPartTransMom->Draw();
    c->SaveAs(FileName + "matchedPartTransMom.png");
    matchedPartTrackMomRes->Draw();
    c->SaveAs(FileName + "matchedPartTrackMomRes.png");
    matchedPartTrackTransMomRes->Draw();
    c->SaveAs(FileName + "matchedPartTrackTransMomRes.png");


    outFile->cd("momentumFits");
    for (int i=0; i<nMomBins; i++)
    {
        h_mom_res[i]->Write();
    }
    outFile->cd("pTResolutions");
    for (int i=0; i<npTBins; i++)
    {
        h_pT_res[i]->Write();
    }

    
    outFile->Close();

}
