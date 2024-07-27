#include <vector>
#include <fstream>
#include <string>

/*
This code takes a text file as input, where each line contains the name of a ROOT file 
containing a TGraph object with the Pt resolution as a function of Pt. The code overlays
each of the graphs, changing the marker style and color for each graph, and adds a legend.

Assuming you have run ./eic-shell and are in the directory PlotTest, you can run:
root -l -b -q 'OverlayList.C("fileList.txt")'

where fileList.txt is a text file containing the names of the ROOT files to overlay.
*/


void OverlayList(const std::string& fileName) {
    // Read the file names from the text file
    std::ifstream inputFile(fileName);
    if (!inputFile) {
        std::cerr << "Error opening file: " << fileName << std::endl;
        return;
    }

    std::vector<std::string> fileNames;
    std::string line;
    while (std::getline(inputFile, line)) {
        fileNames.push_back(line);
    }

    // Create a canvas
    TCanvas* c = new TCanvas("c", "Overlay Graphs", 800, 600);
    c->SetTopMargin(0.1);
    c->SetBottomMargin(0.14);
    c->SetLeftMargin(0.175);
    c->SetRightMargin(0.1);
    // Set the maximum value of the y-axis 
    // Variable to hold the first graph for axis titles
    TGraph* firstGraph = nullptr;
    bool isFirstGraph = true;
    TGraph* graph = nullptr;

    // Create a legend
    TLegend* legend = new TLegend(0.5, 0.5, 0.9, 0.9); // Adjust the position and size of the legend as needed

    legend->SetTextSize(0.04);

    // Define a vector of ROOT color codes
    std::vector<int> colors = {kRed, kBlue, kGreen, kOrange, kCyan, kYellow, kGray, kViolet};
    int filecount = -1;

    // Loop over the file names
    for (const auto& fileName : fileNames) {
        // Load the graph from the file (assuming the graph is named 'graph' in each file)
        TFile* file = TFile::Open(fileName.c_str());
        if (!file || file->IsZombie()) {
            std::cerr << "Error opening file: " << fileName << std::endl;
            continue;
        }
        
        file->GetObject("comparison_res_pT", graph); // Adjust "graph" if your graph has a different name
        if (!graph) {
            std::cerr << "Graph not found in file: " << fileName << std::endl;
            file->Close();
            continue;
        }

        // Use the first graph to set axis titles
        if (isFirstGraph == true) {
            firstGraph = graph;
            isFirstGraph = false;
            firstGraph->GetXaxis()->SetTitle("p_{T}^{gen}");
            firstGraph->GetYaxis()->SetTitle("#frac{#Delta p_{T}}{p_{T}}");
            firstGraph->GetXaxis()->SetTitleSize(0.05);
            firstGraph->GetXaxis()->SetRangeUser(1, 3.5);
            firstGraph->GetYaxis()->SetTitleSize(0.05);
            firstGraph->GetYaxis()->SetRangeUser(0, 0.3); // Set the range from 0 to 0.3
        }
        
        // Set unique marker type and color for each graph
        ++filecount;
        int markerColor = colors[filecount % colors.size()];
        graph->SetMarkerColor(markerColor);
        if (filecount < colors.size()) {
            graph->SetMarkerStyle(20);
        }
        else{
            graph->SetMarkerStyle(22);
        }
        //graph->SetMarkerStyle(filecount < colors.size() ? 20 + (filecount % 4) : 21);
        

        // Add the graph to the legend with the corresponding marker color
        //legend->AddEntry(graph, fileName.c_str(), "p");
        if (filecount == 0) { legend->AddEntry(graph, "Using Default XML", "p"); }// new
        if (filecount == 1)legend->AddEntry(graph, "Set Z via Custom XML", "p"); //new
        // Draw the graph
        // Use "AP" for the first graph to draw the axes, and "P" for subsequent graphs to avoid redrawing the axes
        graph->Draw(fileNames.front() == fileName ? "AP" : "P SAME");

        file->Close();
    }

    // Draw the legend
    legend->Draw();
    
    c->SaveAs("comparison.png");
}

