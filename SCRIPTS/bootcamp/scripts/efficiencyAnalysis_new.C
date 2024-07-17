#include <fstream>

void efficiencyAnalysis_new(TString FileName, TString outputDirectory = "./"){  
  // Ensure the output directory path ends with a slash
  //if (!outputDirectory.EndsWith("/")) outputDirectory += "/";
  
  // Construct the full path for the output file
  TString outputPath = outputDirectory + "EfficiencyAnalysis.root";

  // Set output file for the histograms
  TFile *ofile = TFile::Open(outputPath,"RECREATE");
  // Analysis code will go here
  // Set up input file chain
  TChain *mychain = new TChain("events");
  mychain->Add(FileName);

  // Initialize reader
  TTreeReader tree_reader(mychain);

  // Get Particle Information
  TTreeReaderArray<int> partGenStat(tree_reader, "MCParticles.generatorStatus");
  TTreeReaderArray<float> partMomX(tree_reader, "MCParticles.momentum.x");
  TTreeReaderArray<float> partMomY(tree_reader, "MCParticles.momentum.y");
  TTreeReaderArray<float> partMomZ(tree_reader, "MCParticles.momentum.z");
  TTreeReaderArray<int> partPdg(tree_reader, "MCParticles.PDG");

  // Get Reconstructed Track Information
  TTreeReaderArray<float> trackMomX(tree_reader, "ReconstructedChargedParticles.momentum.x");
  TTreeReaderArray<float> trackMomY(tree_reader, "ReconstructedChargedParticles.momentum.y");
  TTreeReaderArray<float> trackMomZ(tree_reader, "ReconstructedChargedParticles.momentum.z");

  // Get Associations Between MCParticles and ReconstructedChargedParticles
  TTreeReaderArray<unsigned int> recoAssoc(tree_reader, "ReconstructedChargedParticleAssociations.recID");
  TTreeReaderArray<unsigned int> simuAssoc(tree_reader, "ReconstructedChargedParticleAssociations.simID");
    
  // Define Histograms
  TH1D *partEta = new TH1D("partEta","#eta of Thrown Charged Particles; #eta", 120, -6, 6);
  TH1D *matchedPartEta = new TH1D("matchedPartEta","#eta of Thrown Charged Particles That Have Matching Track; #eta", 120, -6, 6);
  TH1D* partMom = new TH1D("partMom", "Momentum of Thrown Charged Particles (truth); P(GeV/c)", 150, 0, 150);
  TH1D* matchedPartMom = new TH1D("matchedPartMom", "Momentum of Thrown Charged Particles (truth), with matching track; P(GeV/c)", 150, 0, 150);
  TH1D* partPhi = new TH1D("partPhi", "#phi of Thrown Charged Particles (truth); #phi(rad)", 320, -3.2, 3.2);
  TH1D* matchedPartPhi = new TH1D("matchedPartPhi", "#phi of Thrown Charged Particles (truth), with matching track; #phi(rad)", 320, -3.2, 3.2);

  TH2D* partPEta = new TH2D("partPEta", "P vs #eta of Thrown Charged Particles; P(GeV/c); #eta", 150, 0, 150, 120, -6, 6);
  TH2D* matchedPartPEta = new TH2D("matchedPartPEta", "P vs #eta of Thrown Charged Particles, with matching track; P(GeV/c); #eta", 150, 0, 150, 120, -6, 6);
  TH2D* partPhiEta = new TH2D("partPhiEta", "#phi vs #eta of Thrown Charged Particles; #phi(rad); #eta", 160, -3.2, 3.2, 120, -6, 6);
  TH2D* matchedPartPhiEta = new TH2D("matchedPartPhiEta", "#phi vs #eta of Thrown Charged Particles; #phi(rad); #eta", 160, -3.2, 3.2, 120, -6, 6);
    
  TH1D *matchedPartTrackDeltaEta = new TH1D("matchedPartTrackDeltaEta","#Delta#eta Between Matching Thrown and Reconstructed Charged Particle; #Delta#eta", 100, -0.25, 0.25);
  TH1D *matchedPartTrackDeltaPhi = new TH1D("matchedPartTrackDeltaPhi","#Detla #phi Between Matching Thrown and Reconstructed Charged Particle; #Delta#phi", 200, -0.2, 0.2);
  TH1D *matchedPartTrackDeltaR = new TH1D("matchedPartTrackDeltaR","#Delta R Between Matching Thrown and Reconstructed Charged Particle; #Delta R", 300, 0, 0.3);
  TH1D *matchedPartTrackDeltaMom = new TH1D("matchedPartTrackDeltaMom","#Delta P Between Matching Thrown and Reconstructed Charged Particle; #Delta P", 200, -10, 10);
  TH1D *matchedPartTrackMomRes = new TH1D("matchedPartTrackMomRes","#Delta P/P Between Matching Thrown and Reconstructed Charged Particle; #Delta P/P", 200, -10, 10); 
  TH1D *matchedPartTrackTransMomRes = new TH1D("matchedPartTrackTransMomRes","#Delta P_{T}/P_{T} Between Matching Thrown and Reconstructed Charged Particle; #Delta P_{T}/P_{T}", 200, -10, 10);   

  // Define some histograms for our efficiencies
  TH1D *TrackEff_Eta = new TH1D("TrackEff_Eta", "Tracking efficiency as fn of #eta; #eta; Eff(%)", 120, -6, 6); 
  TH1D *TrackEff_Mom = new TH1D("TrackEff_Mom", "Tracking efficiency as fn of P; P(GeV/c); Eff(%)", 150, 0, 150); 
  TH1D *TrackEff_Phi = new TH1D("TrackEff_Phi", "Tracking efficiency as fn of #phi; #phi(rad); Eff(%)", 320, -3.2, 3.2);

  // 2D Efficiencies
  TH2D* TrackEff_PEta = new TH2D("TrackEff_PEta", "Tracking efficiency as fn of P and #eta; P(GeV/c); #eta", 150, 0, 150, 120, -6, 6);
  TH2D* TrackEff_PhiEta = new TH2D("TrackEff_PhiEta", "Tracking efficiency as fn of #phi and #eta; #phi(rad); #eta", 160, -3.2, 3.2, 120, -6, 6);

  // All charged particle histos
  TH1D *ChargedEta = new TH1D("ChargedEta", "#eta of all charged particles; #eta", 120, -6, 6);
  TH1D *ChargedPhi = new TH1D("ChargedPhi", "#phi of all charged particles; #phi (rad)", 120, -3.2, 3.2);
  TH1D *ChargedP = new TH1D("ChargedP", "P of all charged particles; P(GeV/c)", 150, 0, 150);
  
  while(tree_reader.Next()) { // Loop over events

    for(unsigned int i=0; i<partGenStat.GetSize(); i++) // Loop over thrown particles
      {
	if(partGenStat[i] == 1) // Select stable thrown particles
	  {
	    int pdg = TMath::Abs(partPdg[i]);
        if(pdg == -2212 || pdg == 2212)
      // changing this to just pions, for now
      //if(pdg == -211 || pdg == 211)
	    //if(pdg == 11 || pdg == 13 || pdg == -211 || pdg == 211 || pdg == 321 || pdg == 2212) // Look at charged particles (electrons, muons, pions, kaons, protons)
	      {
		TVector3 trueMom(partMomX[i],partMomY[i],partMomZ[i]);
        TVector2 trueTransMom(partMomX[i],partMomY[i]); // new

        //float truePT = trueTransMom.Mod(); // new

		float trueEta = trueMom.PseudoRapidity();
		float truePhi = trueMom.Phi();
	    
		partEta->Fill(trueEta);
		partPhi->Fill(truePhi);
		partMom->Fill(trueMom.Mag());
		partPEta->Fill(trueMom.Mag(), trueEta);
		partPhiEta->Fill(truePhi, trueEta);

		// Loop over associations to find matching ReconstructedChargedParticle
		for(unsigned int j=0; j<simuAssoc.GetSize(); j++)
		  {
		    if(simuAssoc[j] == i) // Find association index matching the index of the thrown particle we are looking at
		      {
			TVector3 recMom(trackMomX[recoAssoc[j]],trackMomY[recoAssoc[j]],trackMomZ[recoAssoc[j]]); // recoAssoc[j] is the index of the matched ReconstructedChargedParticle
            TVector2 recTransMom(trackMomX[recoAssoc[j]],trackMomY[recoAssoc[j]]); // new
			

            // Check the distance between the thrown and reconstructed particle
			float deltaEta = trueEta - recMom.PseudoRapidity();
			float deltaPhi = TVector2::Phi_mpi_pi(truePhi - recMom.Phi());
			float deltaR = TMath::Sqrt(deltaEta*deltaEta + deltaPhi*deltaPhi);
			float deltaMom = ((trueMom.Mag()) - (recMom.Mag()));
            float momRes = ((recMom.Mag()) - (trueMom.Mag()))/(trueMom.Mag());
            float momPTRes = ((recTransMom.Mod()) - (trueTransMom.Mod()))/(trueTransMom.Mod()); // new

			matchedPartTrackDeltaEta->Fill(deltaEta);
			matchedPartTrackDeltaPhi->Fill(deltaPhi);
			matchedPartTrackDeltaR->Fill(deltaR);
			matchedPartTrackDeltaMom->Fill(deltaMom);
            matchedPartTrackMomRes->Fill(momRes);
            matchedPartTrackTransMomRes->Fill(momPTRes);

			matchedPartEta->Fill(trueEta); // Plot the thrown eta if a matched ReconstructedChargedParticle was found
			matchedPartPhi->Fill(truePhi);
			matchedPartMom->Fill(trueMom.Mag());

			matchedPartPEta->Fill(trueMom.Mag(), trueEta);
			matchedPartPhiEta->Fill(truePhi, trueEta);
	
		      }
		  }// End loop over associations
	      } // End PDG check
	  } // End stable particles condition
      } // End loop over thrown particles
    // Loop over all charged particles and fill some histograms of kinematics quantities
    for(unsigned int k=0; k<trackMomX.GetSize(); k++){ // Loop over all charged particles, thrown or not
      
      TVector3 CPartMom(trackMomX[k], trackMomY[k], trackMomZ[k]);

      float CPartEta = CPartMom.PseudoRapidity();
      float CPartPhi = CPartMom.Phi();

      ChargedEta->Fill(CPartEta);
      ChargedPhi->Fill(CPartPhi);
      ChargedP->Fill(CPartMom.Mag());
      
    } // End loop over all charged particles
  } // End loop over events

  // Take the ratio of the histograms above to get our efficiency plots
  TrackEff_Eta->Divide(matchedPartEta, partEta, 1, 1, "b");
  TrackEff_Mom->Divide(matchedPartMom, partMom, 1, 1, "b");
  TrackEff_Phi->Divide(matchedPartPhi, partPhi, 1, 1, "b");
  TrackEff_PEta->Divide(matchedPartPEta, partPEta, 1, 1, "b");
  TrackEff_PhiEta->Divide(matchedPartPhiEta, partPhiEta, 1, 1, "b");
  
  // to get the # tracks not reconstructed / total # tracks
  double totalTracks = (partMom->GetEntries())*1.0;
  double totalTracksNotReconstructed = totalTracks - (matchedPartMom->GetEntries())*1.0;
  double kfEff = totalTracksNotReconstructed / totalTracks;

  // Calculate and print mean for matchedPartTrackMomRes
  double meanMomRes = matchedPartTrackMomRes->GetMean();
  // Calculate and print mean for matchedPartTrackTransMomRes
  double meanTransMomRes = matchedPartTrackTransMomRes->GetMean();

  
  TString outputPath2 = outputDirectory + "output.txt";

  std::ofstream outFile(outputPath2);
  // Check if the file was successfully opened
  if (outFile.is_open()) {
    // Write the desired output to the file
    outFile << meanTransMomRes << " " << kfEff << std::endl;

    // Close the file
    outFile.close();
  } else {
    std::cout << "Unable to open file for writing." << std::endl;
  }


/*
  // Print out (until I know how to capture it)
  cout << meanTransMomRes << " " << kfEff << endl;
*/
/*  // Print out (until I know how to capture it)
  //cout << "Mean of matchedPartTrackMomRes: " << meanMomRes << endl;
  //cout << "Kalman Filter Efficiency: " << kfEff << endl;
  cout << meanMomRes << " " << kfEff << endl;*/


/* 
// Calculate and print mean and standard deviation for partEta
  double meanEta = partEta->GetMean();
  double stdDevEta = partEta->GetStdDev();
  cout << "Mean of partEta: " << meanEta << endl;
  cout << "Standard Deviation of partEta: " << stdDevEta << endl;

*/

  ofile->Write(); // Write histograms to file
  ofile->Close(); // Close output file
}