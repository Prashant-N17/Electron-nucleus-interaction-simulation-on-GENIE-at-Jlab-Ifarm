#include <TFile.h>
#include <TTree.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>

// Target mass (example: Carbon-12 nucleus)
constexpr double m_target = 0.938272; // GeV/c^2

int main(int argc, char **argv) {
    std::string filename;

    // Get filename
    if (argc < 2) {
        std::cout << "Enter input .gst.root file: ";
        std::cin >> filename;
    } else {
        filename = argv[1];
    }

    // Open file and tree
    TFile file(filename.c_str());
    if (file.IsZombie()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return 1;
    }
    TTree *gst = (TTree*)file.Get("gst");
    if (!gst) {
        std::cerr << "Tree 'gst' not found in file!" << std::endl;
        return 2;
    }

    // Lepton variables
    Double_t Ev, El, pxl, pyl, pzl;

    // Hadrons (dynamic array, max size large enough for practical use e.g. 20)
    const int maxNF = 32;
    Int_t nf;
    Double_t Ef[maxNF], pxf[maxNF], pyf[maxNF], pzf[maxNF];
    Int_t pdgf[maxNF];

    // Set branch addresses
    gst->SetBranchAddress("Ev",  &Ev);
    gst->SetBranchAddress("El",  &El);
    gst->SetBranchAddress("pxl", &pxl);
    gst->SetBranchAddress("pyl", &pyl);
    gst->SetBranchAddress("pzl", &pzl);
    gst->SetBranchAddress("nf",  &nf);
    gst->SetBranchAddress("Ef",  Ef);
    gst->SetBranchAddress("pxf", pxf);
    gst->SetBranchAddress("pyf", pyf);
    gst->SetBranchAddress("pzf", pzf);
    gst->SetBranchAddress("pdgf", pdgf);
    Ev = 4.021;
    // Output
    std::ofstream csv("missing_mass2.csv");
    csv << "missing_mass\n";

    Long64_t nEntries = gst->GetEntries();

    for (Long64_t i = 0; i < nEntries; ++i) {
        gst->GetEntry(i);

        // Find unique pi+ in event
        int idx_pion = -1;
        for (int j = 0; j < nf; ++j) {
            if (pdgf[j] == 211) {
                idx_pion = j;
                break; // found the pi+
            }
        }
        if (idx_pion == -1) {
            std::cerr << "Warning: No pi+ found in entry " << i << std::endl;
            continue;
        }

        // Pion kinematics
        double Ef_pion  = Ef[idx_pion];
        double pxf_pion = pxf[idx_pion];
        double pyf_pion = pyf[idx_pion];
        double pzf_pion = pzf[idx_pion];

        // Final-state system: lepton + pion+
        double E_FS  = El + Ef_pion;
        double px_FS = pxl + pxf_pion;
        double py_FS = pyl + pyf_pion;
        double pz_FS = pzl + pzf_pion;
        std::cout << "Event " << i << ": "
       << "Ev=" << Ev << ", m_target=" << m_target
       << ", El=" << El << ", Ef_pion=" << Ef_pion
       << ", pxl=" << pxl << ", pyl=" << pyl << ", pzl=" << pzl
       << ", pxf_pion=" << pxf_pion << ", pyf_pion=" << pyf_pion << ", pzf_pion=" << pzf_pion
       << std::endl;
        
        std::cout << "E_FS=" << E_FS << ", px_FS=" << px_FS
        << ", py_FS=" << py_FS << ", pz_FS=" << pz_FS << std::endl;
        // As before (assuming incoming along z)
        double term1 =    m_target + Ev - E_FS;
        double mm2 = term1*term1 - (px_FS*px_FS + py_FS*py_FS + (Ev - pz_FS)*(Ev - pz_FS));
        double mm = mm2 > 0 ? std::sqrt(mm2) : 0.0;
        std::cout << "mm =" << mm << std::endl;

        csv << mm << '\n';
    }

    csv.close();
    std::cout << "Done! Output written to missing_mass2.csv\n";
    return 0;
}
