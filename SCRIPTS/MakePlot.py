#/usr/bin/python3.6

import ROOT
import sys, argparse, os
import numpy as np
import pandas as pd

def BeautifyGraph(graph, color, markerStyle):
    graph.SetMarkerStyle(markerStyle)
    graph.SetMarkerColor(color)
    graph.SetLineColor(color)
    graph.SetLineWidth(2)
    graph.SetMarkerSize(1.5)
    graph.GetXaxis().SetLabelSize(0.065)
    graph.GetYaxis().SetLabelSize(0.065)
    graph.GetXaxis().SetTitleSize(0.07)
    graph.GetYaxis().SetTitleSize(0.07)
    graph.GetYaxis().CenterTitle()
    return graph

def draw(p):
    if (p == "pT"):
        files = ["/volatile/halld/home/ksuresh/ePIC-EIC-2024/ProductionSimulations/epic_craterlake_18x275_63cm_ilayer_23cm_out-fit_pT_res.csv",
                "/volatile/halld/home/ksuresh/ePIC-EIC-2024/ProductionSimulations/epic_craterlake_18x275_63cm_ilayer_25cm_out-fit_pT_res.csv",
                "/volatile/halld/home/ksuresh/ePIC-EIC-2024/ProductionSimulations/epic_craterlake_18x275_63cm_ilayer_27cm_out-fit_pT_res.csv",
                "/volatile/halld/home/ksuresh/ePIC-EIC-2024/ProductionSimulations/epic_craterlake_18x275_63cm_ilayer_29cm_out-fit_pT_res.csv",
                "/volatile/halld/home/ksuresh/ePIC-EIC-2024/ProductionSimulations/epic_craterlake_18x275_63cm_ilayer_32cm_out-fit_pT_res.csv",
                "/volatile/halld/home/ksuresh/ePIC-EIC-2024/ProductionSimulations/epic_craterlake_18x275_63cm_ilayer_35cm_out-fit_pT_res.csv"
                ]
    elif (p == "p"):
        files = ["/volatile/halld/home/ksuresh/ePIC-EIC-2024/ProductionSimulations/epic_craterlake_18x275_63cm_ilayer_23cm_out-fit_mom_res.csv",
                 "/volatile/halld/home/ksuresh/ePIC-EIC-2024/ProductionSimulations/epic_craterlake_18x275_63cm_ilayer_25cm_out-fit_mom_res.csv",
                 "/volatile/halld/home/ksuresh/ePIC-EIC-2024/ProductionSimulations/epic_craterlake_18x275_63cm_ilayer_27cm_out-fit_mom_res.csv",
                 "/volatile/halld/home/ksuresh/ePIC-EIC-2024/ProductionSimulations/epic_craterlake_18x275_63cm_ilayer_29cm_out-fit_mom_res.csv",
                 "/volatile/halld/home/ksuresh/ePIC-EIC-2024/ProductionSimulations/epic_craterlake_18x275_63cm_ilayer_32cm_out-fit_mom_res.csv",
                 "/volatile/halld/home/ksuresh/ePIC-EIC-2024/ProductionSimulations/epic_craterlake_18x275_63cm_ilayer_35cm_out-fit_mom_res.csv"
        ]
    else:
        print ("Invalid option. Choose either pT or p")
        return
    titles = ["Interlayer d = 23cm",
              "Interlayer d = 25cm",
              "Interlayer d = 27cm (default)",
              "Interlayer d = 29cm",
              "Interlayer d = 32cm",
              "Interlayer d = 35cm"
              ]
    
    grErrs = []
    colors = [ROOT.kBlue, ROOT.kRed, ROOT.kGreen, ROOT.kBlack, ROOT.kMagenta, ROOT.kOrange]
    for i in range(len(files)):
        df = pd.read_csv(files[i])
        pT = df[p]
        # set pT values to -99 for x range outside of 1.4 and 3.8
        res = df["Resolution"]
        if (p == "pT"):
            res = np.where(pT < 1.4, -99, res)
            res = np.where(pT > 3.8, -99, res)
        print (res)
        xerr = df["Error"]
        yerr = df["ResolutionError"]
        gr = ROOT.TGraphErrors(len(pT), np.array(pT), np.array(res), np.array(xerr), np.array(yerr))
        gr = BeautifyGraph(gr, colors[i], 20)
        grErrs.append(gr)
    
    c = ROOT.TCanvas("c", "Resolution Studies", 800, 600)
    c.cd()
    c.SetBottomMargin(0.15)
    legend = ROOT.TLegend(0.1, 0.15, 0.9, 0.3)
    legend.SetNColumns(3)
    for i in range(len(grErrs)):
        if i == 0:
            grErrs[i].Draw("APE")
            grErrs[i].SetTitle("Resolution Studies")
            grErrs[i].GetXaxis().SetTitle("p_{T} [GeV]")
            grErrs[i].GetYaxis().SetTitle("Resolution")
            grErrs[i].GetYaxis().SetRangeUser(0, 0.2)
        else:
            grErrs[i].Draw("PE")
        legend.AddEntry(grErrs[i], titles[i], "pe")
    
    legend.Draw("SAME")
    print ("Saving plot to ResolutionStudies.pdf")
    if (p == "pT"):
        c.SaveAs("ResolutionStudies_pT.pdf")
    elif (p == "p"):
        c.SaveAs("ResolutionStudies.pdf")
    
    # lets make a Ratio plot with respect to the default configuration
    
    legend2 = ROOT.TLegend(0.1, 0.1, 0.9, 0.25)
    legend2.SetNColumns(3)
    grErrorRatios = []
    base = grErrs[2]
    for i in range(len(grErrs)):
        grRatio = ROOT.TGraphErrors()
        grRatio.SetName("grRatio for "+titles[i])
        ratioY = pd.read_csv(files[i])["Resolution"]/pd.read_csv(files[2])["Resolution"]
        err_ratioY = ratioY*np.sqrt((pd.read_csv(files[i])["ResolutionError"]/pd.read_csv(files[i])["Resolution"])**2 + (pd.read_csv(files[2])["ResolutionError"]/pd.read_csv(files[2])["Resolution"])**2)
        xs = pd.read_csv(files[i])[p]
        err_xs = pd.read_csv(files[i])["Error"]
        grRatio = ROOT.TGraphErrors(len(xs), np.array(xs), np.array(ratioY), np.array(err_xs), np.array(err_ratioY))
        grRatio = BeautifyGraph(grRatio, colors[i], 20)
        grErrorRatios.append(grRatio)
    
    c2 = ROOT.TCanvas("c2", "Resolution Studies", 800, 600)
    c2.cd()
    c2.SetBottomMargin(0.15)
    for i in range(len(grErrorRatios)):
        if i == 0:
            grErrorRatios[i].Draw("APE")
            grErrorRatios[i].SetTitle("Resolution Ratio w.r.t default configuration 27cm")
            grErrorRatios[i].GetXaxis().SetTitle("p_{T} [GeV]")
            grErrorRatios[i].GetYaxis().SetTitle("Resolution Ratio")
            grErrorRatios[i].GetYaxis().SetRangeUser(0.5, 1.5)
        else:
            grErrorRatios[i].Draw("PE")
        legend2.AddEntry(grErrorRatios[i], titles[i], "pe")
    legend2.Draw("SAME")
    print ("Saving plot to ResolutionRatios.pdf")
    if (p == "pT"):
        c2.SaveAs("ResolutionRatios_pT.pdf")
    elif (p == "p"):
        c2.SaveAs("ResolutionRatios.pdf")
    
        
if __name__ == "__main__":
    draw("pT")