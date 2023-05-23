#include "BCO_Ana.h"

int main(int argc, char** argv)
{
    BCO_Ana bco_ana;

    printf("\n");
    for(int i = 1; i < argc; ++i)
    {
        printf("Reading file %3d of %3d\n", i, argc - 1);
        bco_ana.ReadFile(argv[i]);
    }

    printf("\n");
    bco_ana.WriteFile();

    return 0;
}
