SegFit
=========

SegFit is a C++ implementation of SegFit Algorithm


Usage
----
Curently, the software are only tested under Linux (specifically, Ubuntu)


1. first make sure ```gfortran``` is installed.

2. Then run ```cmake .``` in the folder. If no error happened, the executable is ```./bin/segfit```

3. three videos' viewcount series are provied in ```./testdata/``` folder. You can run

   ```bash
   
   mkdir output
   ./bin/segfit --videoID ./testdata/videoID.test --viewcount ./testdata/viewcount.test -l 735 -o ./output/

   ```
   
   to run the test. The command above inludes all the necessary parameters needed to run SegFit:
   
   a)  ```--videoID``` :  the videoID file, containing the videoID line by line.
   
   b)  ```--viewcount``` : the viewcount data file, containing the corresponding (w.r.t videoID file) viewcount every line.
   
   c)  ```-l``` : the length of the sequence to process. Will process only the first ```l``` data in each line of viewcount file.
   
   d) ```-o``` : the output folder. (currently use two-level folder hashed by videoID). The output file format is: every line represents a phase. The meanings of the number of every line are: "starting index", "ending index", "a", "b", "c"

   Please use ```./bin/segfit --help to see more info``` 

