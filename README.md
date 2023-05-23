#BCO Ana

Run
$ ./main file1.root file2.root ...

To load all the hits from file1.root, file2.root, ... into a map that keeps track of the bco,
bco_full, and number of hits at those times. This map is used to fill a root tree which is written
to a file, by default, in

/home/phnxrc/INTT/commissioning_5_23/bco_files/bco_hits

with a filename completed with the exact date (down to the second) when the executable was ran. The
program outputs the fullpath name of the file it created when it finishes.

It displays progress bars with estimates of completion times in case files are long (though it will
read a file in less than several seconds).
