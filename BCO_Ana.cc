#include "BCO_Ana.h"

BCO_Ana::BCO_Ana()
{
    //Do nothing
}

BCO_Ana::~BCO_Ana()
{
    //Do nothing
}

void BCO_Ana::InitProgress()
{
    progress_bar_current = 0;
    progress_time = std::chrono::high_resolution_clock::now();
}

void BCO_Ana::ShowProgress(std::size_t const& p, std::size_t const& P)
{
    if(P < p)return;
    if(p * progress_bar_width < P * progress_bar_current)return;

    while(P * progress_bar_current < p * progress_bar_width)++progress_bar_current;

    std::size_t s = 0;
    double current_time = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - progress_time).count();

    printf("\r");
    printf("\t[");
    while(s < progress_bar_current)
    {
        printf(">"); 
        ++s;
    }
    while(s < progress_bar_width)
    {
        printf(" ");
        ++s;
    }
    printf("]\t");

    printf("%0.2fs (ETA %0.2fs)%8s", current_time, current_time * P / p, " ");
    fflush(stdout);
}

void BCO_Ana::ShowFinished()
{
    std::size_t s = 0;
    double current_time = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - progress_time).count();

    printf("\r");
    printf("\t[");
    while(s < progress_bar_width)
    {
        printf(">"); 
        ++s;
    }
    printf("]\t");
    printf("%0.2fs (DONE) %80s", current_time, "\n");
}

int BCO_Ana::ReadFile(std::string const& file_name, std::string const& tree_name)
{
    if(file_name.empty())
    {
        printf("BCO_Ana::ReadFile\n");
        printf("Argument 'file_name' is empty string\n");

        return 1;
    }
    TFile* file = TFile::Open(file_name.c_str(), "READ");
    if(!file)
    {
        printf("BCO_Ana::ReadFile\n");
        printf("Could not open file:\n");
        printf("\t%s\n", file_name.c_str());

        return 1;
    }

    if(tree_name.empty())
    {
        printf("BCO_Ana::ReadFile\n");
        printf("Argument 'tree_name' is empty string\n");

        return 1;
    }
    TTree* tree = (TTree*)file->Get(tree_name.c_str());
    if(!tree)
    {
        printf("BCO_Ana::ReadFile\n");
        printf("Could not get tree:\n");
        printf("\t%s\n", tree_name.c_str());

        return 1;
    }

    int bco = 0;
    Long64_t bco_full = 0;
    int flx = 0;
    int chn = 0;
    int chp = 0;

    tree->SetBranchAddress(bco_branch_name.c_str(), &bco);
    tree->SetBranchAddress(bco_full_branch_name.c_str(), &bco_full);
    tree->SetBranchAddress(flx_branch_name.c_str(), &flx);
    tree->SetBranchAddress(chn_branch_name.c_str(), &chn);
    tree->SetBranchAddress(chp_branch_name.c_str(), &chp);

    Long64_t bco_hit = 0;

    InitProgress();
    for(Long64_t n = 0; n < tree->GetEntriesFast(); ++n)
    {
        tree->GetEntry(n);

        switch(is_offset)
        {
            case 0:
                bco_hit = (bco_full & (Long64_t)0xffffffffffffff80) + (bco & 0x000000000000007f);
                break;
            default:
                bco_hit = bco_full + bco;
                break;
        }

        flx -= 3001;
        if(bco_map[flx][chn][chp].find(bco_hit) == bco_map[flx][chn][chp].end())
        {
            bco_map[flx][chn][chp][bco_hit] = (struct BCO_s){.bco = bco, .bco_full = bco_full, .hits = 0};
        }
        ++(bco_map[flx][chn][chp][bco_hit].hits);
        ++(bco_arr[flx][chn][chp][bco % BCO]);

        ShowProgress(n, tree->GetEntriesFast());
    }
    ShowFinished();

    file->Close();

    return 0;
}

int BCO_Ana::WriteFile(std::string const& file_dir, std::string const& tree_name)
{
    if(file_dir.empty())
    {
        printf("BCO_Ana::WriteFile\n");
        printf("Argument 'file_dir' is empty string\n");

        return 1;
    }

    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    std::stringstream ss;

    ss << file_dir;
    ss << "bco_hits_";
    ss << std::put_time(&tm, "%d_%m_%Y_%H_%M_%S");
    ss << ".root";

    TFile* file = TFile::Open(ss.str().c_str(), "RECREATE");
    if(!file)
    {
        printf("BCO_Ana::WriteFile\n");
        printf("Could not create file:\n");
        printf("\t%s\n", ss.str().c_str());

        return 1;
    }

    printf("Writing to file:\n");
    printf("\t%s\n", ss.str().c_str());

    int bco;
    Long64_t bco_full;
    Long64_t bco_combined;
    Long64_t hits_v_bco_combined;
    Long64_t hits_v_bco;

    int flx = 0;
    int chn = 0;
    int chp = 0;

    TTree* tree = nullptr;
    Map_t::const_iterator itr;
   
    int flag = 1;
    tree = new TTree(Form("tree", flx, chn, chp), Form("tree", flx, chn, chp));
    tree->SetDirectory(file);

    tree->Branch("bco", &bco);
    tree->Branch("bco_full", &bco_full);
    tree->Branch("bco_combined", &bco_combined);
    tree->Branch("hits_v_bco_combined", &hits_v_bco_combined);
    tree->Branch("hits_v_bco", &hits_v_bco);

    tree->Branch("felix", &flx);
    tree->Branch("felix_channel", &chn);
    tree->Branch("chp", &chp);

    InitProgress();
    while(true)
    {
        for(itr = bco_map[flx][chn][chp].begin(); itr != bco_map[flx][chn][chp].end(); ++itr)
        {
            bco = itr->second.bco;
            bco_full = itr->second.bco_full;
            bco_combined = itr->first;

            hits_v_bco_combined = itr->second.hits;
            hits_v_bco = bco_arr[flx][chn][chp][bco % 128];

            tree->Fill();
        }

        ShowProgress(flx * CHN * CHP + chn * CHP + chp, FLX * CHN * CHP);

        ++chp;
        if(chp < CHP)continue;
        chp = 0;

        ++chn;
        if(chn < CHN)continue;
        chn = 0;

        ++flx;

        if(flx < FLX)continue;
        flx = 0;

        break;
    }

    tree->Write();
    file->Write();
    file->Close();

    ShowFinished();

    return 0;
}
