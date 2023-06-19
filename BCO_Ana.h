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
    const static std::size_t FLX = 8;
    const static std::size_t CHN = 14;
    const static std::size_t CHP = 26;

    const static std::size_t BCO = 128;

    std::string bco_branch_name = "bco";
    std::string bco_full_branch_name = "bco_full";
    std::string flx_branch_name = "pid";
    std::string chn_branch_name = "module";
    std::string chp_branch_name = "chip_id";

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
    Map_t bco_map[FLX][CHN][CHP];
    Long64_t bco_arr[FLX][CHN][CHP][BCO] = {0};

public:
    BCO_Ana();
    ~BCO_Ana();

    void InitProgress();
    void ShowProgress(std::size_t const&, std::size_t const&);
    void ShowFinished();

    int ReadFile(std::string const&, std::string const& = "tree");
    int WriteFile(std::string const& = "/bbox/commissioning/INTT/hit_files/", std::string const& = "tree");
};

#endif//BCO_ANA_H
