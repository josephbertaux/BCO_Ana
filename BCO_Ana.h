#ifndef BCO_ANA_H
#define BCO_ANA_H

#include <cstdio>
#include <cstddef>
#include <cstdint>
#include <cstdlib>

#include <iomanip>
#include <iostream>
#include <chrono>
#include <map>
#include <string>

#include "TFile.h"
#include "TTree.h"

class BCO_Ana
{
protected:
    std::string bco_branch_name = "bco";
    std::string bco_full_branch_name = "bco_full";

    std::size_t progress_bar_current;
    std::size_t progress_bar_width = 20;
    std::chrono::high_resolution_clock::time_point progress_time;

    int is_offset = 1;

    struct BCO_s
    {
        int bco;
        Long64_t bco_full;
        Long64_t hits;
    };
    typedef std::map<Long64_t, struct BCO_s> Map_t;
    Map_t bco_hits;

public:
    BCO_Ana();
    ~BCO_Ana();

    void InitProgress();
    void ShowProgress(std::size_t const&, std::size_t const&);
    void ShowFinished();

    int ReadFile(std::string const&, std::string const& = "tree");
    int WriteFile(std::string const& = "/home/phnxrc/INTT/commissioning_5_23/bco_files/", std::string const& = "tree");
};

#endif//BCO_ANA_H
