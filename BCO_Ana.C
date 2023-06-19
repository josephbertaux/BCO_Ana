//#include <filesystem>
#include <iostream>
#include <string>


std::string bco_branch = "bco_full";
std::string bco_cut = "ladder == 9 && chip_id != 21 && chan_id != 126";

std::string path = "/home/phnxrc/INTT/commissioning_5_23/bco_files/";

void BCO_Ana(std::string const& f)
{
    //std::filesystem::file_time_type* time = nullptr;
    //std::filesystem::directory_entry oldest;
    //for(std::filesystem::directory_entry const& itr : std::filesystem::directory_iterator(path))
    //{
    //    if(!itr.is_regular_file())continue;

    //    if(!time)
    //    {
    //        time = new std::filesystem::file_time_type;
    //        *time = itr.last_write_time();
    //    }

    //    if(itr.last_write_time() >= *time)
    //    {
    //        *time = itr.last_write_time();
    //        oldest = itr;
    //    }
    //}
    //if(time)delete time;

    //std::cout << oldest.path().filename() << std::endl;

    std::string filename = f;
    TFile* file = TFile::Open(filename.c_str(), "READ");
    TTree* tree = (TTree*)file->Get("tree");

    while(filename.find("/") != std::string::npos)
    {
        filename = filename.substr(filename.find("/")+1);
    }
    filename = filename.substr(0, filename.find("."));

    TCanvas* cnvs = new TCanvas(Form("%s_cnvs", filename.c_str()), Form("%s_cnvs", filename.c_str()));
    cnvs->cd();

    int bco = 0;
    Long64_t bco_full = 0;

    int evt = 0;
    int chip = 0;
    int chan = 0;

    tree->SetBranchAddress("bco_full", &bco_full);
    tree->SetBranchAddress("bco", &bco);

    tree->SetBranchAddress("event", &evt);
    tree->SetBranchAddress("chip_id", &chip);
    tree->SetBranchAddress("chan_id", &chan);

    tree->GetEntry(0);
    Long64_t min = bco_full;
    tree->GetEntry(tree->GetEntriesFast()-1);
    Long64_t max = bco_full;

    TH1* hist[30] = {nullptr};
    for(int e = 0; e < 30; ++e)
    {
        hist[e] = new TH1D(Form("%s_hist_%d", filename.c_str(), e), Form("%s_hist_%d", filename.c_str(), e), 128, 0, 127);
    }
    for(Long64_t n = 0; n < tree->GetEntriesFast(); ++n)
    {
        tree->GetEntry(n);

        if(chip == 21 && chan == 126)continue;

        if(evt >= 30)continue;
        hist[evt]->Fill(bco);
    }
    for(int e = 0; e < 30; ++e)
    {
        hist[e]->SetLineColor(e);
        hist[e]->Draw("same");
    }

    //tree->Draw(Form("%s>>%s_hist", bco_branch.c_str(), filename.c_str()), bco_cut.c_str());
    //TH1* hist = (TH1*)gROOT->FindObject(Form("%s_hist", filename.c_str()));

    //tree->SetMarkerStyle(20);
    //tree->SetMarkerSize(2);
    //tree->Draw("hits_v_bco_combined:bco_combined");

    cnvs->Show();

    filename = path + filename + ".png";
    std::cout << filename << std::endl;
    //cnvs->SaveAs(filename.c_str());
}
