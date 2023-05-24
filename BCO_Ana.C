//#include <filesystem>
#include <iostream>
#include <string>

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

    TCanvas* cnvs = new TCanvas(filename.c_str(), filename.c_str());
    cnvs->cd();
    cnvs->SetLogy();

    tree->SetMarkerStyle(20);
    tree->SetMarkerSize(1);
    tree->SetMarkerColor(kRed);

    tree->Draw("hits_v_bco:bco");
    cnvs->Show();

    filename = path + filename + ".png";
    std::cout << filename << std::endl;
}
