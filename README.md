# srSim

A short and long read simulator for next-generation sequencing (NGS) data. 

The read simulator can be used for the development or testing of NGS aligners and 
assemblers.

Installation
----

Use the included config.txt file to customize. Compile using the included 
Makefile. Then run using:

=======
```bash
./srSim config.txt
```

Configuration File
----
    verbose        Print detailed runtime information
    genomeLength        Length of the genome to simulate in nucleotides
    genome              Genome name to create reads. If the filename exists, a genome will not be created
    avgRead             Average length for 454 pyrosequencing is 533
    coverage            Average coverage for the data
    machine             Machine used to sequence the data (454, illumina)
    qvdata              QV data set filename to use
    mutation            Can nucleotides be mutated? (true/false)
    qvScore             Default QV score for non-mutated simulations
    printQV             Should the program print a QV score?
    reverseComplement   Do you want ~50% of the reads to be reverse complimented?
    threads             Number of threads to utilize


Version
----

0.1
