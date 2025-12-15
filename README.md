Please reference JLAB websites and help desk for **setting your account at ifarm at JLAB**. I use Group e1206107 under hallC for the task.
I recommend **VDI access** for working on JLab computers:
https://jlab.servicenowservices.com/sp?sys_kb_id=dec16b0ddb7f0410ee4a3889fc961944&id=kb_article_view&sysparm_rank=1&sysparm_tsqueryId=beebdd561b8ae290a2c7db1de54bcb7f

Once you install it, they give you a windows machine by default, so you can **email helpdesk@jlab.org and ask for CUE RHEL9 machine** to be added.

**Open a terminal, and ssh scilogin.jlab.org**
**Enter your pin+MFA**
**Ssh ifarm**
**Enter your password**
**You can work on /work/hallc/e1206107**

To setup GENIE in a personal computer might be a hectic and irritating task. 

The way I used GENIE was by **setting up the below container in my IFARM account at JLAB.**
/cvmfs/oasis.opensciencegrid.org/mis/apptainer/current/bin/apptainer shell --shell=/bin/bash -B /cvmfs,/work/hallc/e1206107/<location of setup_GENIE.sh file> --ipc --pid /cvmfs/singularity.opensciencegrid.org/fermilab/fnal-dev-sl7:latest

and** sourcing the following files in order.** 
-source /cvmfs/fermilab.opensciencegrid.org/products/genie/bootstrap_genie_ups.sh
-source /cvmfs/fermilab.opensciencegrid.org/products/common/etc/setups 
-source setup_GENIE.sh https://github.com/e4nu/Generator-NuHepMC.git hepmc3-Q2_04 false false $GENIE/config/ true    (make sure of existence of setup_GENIE.sh in the directory of execution of the command)

Your GENIE environment is now set up.


To submit multiple jobs at IFARM, you will need wrapper codes with location of actual code to run on the farm. 
The submit_geven_swif2.sh and submit_gntpc_swif2.sh are the wrapper texts to be used when creating events and converting those events in ghep format to gst format that can be easily analysed.

swif2 command package is used to keep track of your jobs at ifarm. The codes provided in the repo make use of the swif2 package to submit jobs at ifarm.
The code does not create a workflow. I create workflows manually using (swif2 create -workflow <workflow name>). Make sure to check the workflow name in the main code to match it.
One can change the code as necessary to make it flexible.

**The core operation**
**create splines using the following command:**    gmkspl -p 11 -t 1000010020     -e 4.021     -o e4021_deu.xml   --tune GEM21_11a_00_000 --event-generator-list EMDIS
                                                          (probe)   (target) (energy/range)  (spline file output, user defined name)

**Generate events using the splines**:    gevgen -n 1000000 -p 11 -t 1000060120 -e 4021 --tune GEM21_11a_00_000 --event-generator-list EMDIS --cross-sections e4012_c12.xml -o gntp.1001.ghep.root
(on ifarm, type **bash submit_gevgen_swif2.sh**)                                                                                          (reference the xml file, using proper location if different from the current location)

**convert to gst format**: gntpc -f gst -i gntp.1.ghep.root -o gntp.1.gst.root
(in the Generator folder, the splines along with .status files are stored. remove the .status files and run **bash submit_gntpc_swif2.sh**)

**Combine all the generated gst files to a single file**:     
hadd [output_file_name] [input_files]

**run root**: root

(I was having some difficulty writing the code. I do the following task manually:)
**put filters:**

TFile *fin = new TFile("<combined large gst file>");
TTree *t = (TTree*)fin->Get("gst");
TFile *fout = new TFile("<filtered file name, user defined>", "RECREATE");
TTree *t2 = t->CopyTree("(nfpip==1 && nfpi0==0 && nfpim==0) && ((1.071<El && El<1.309) && (2.5137<Ef[1] && Ef[1]<3.0723)) && (0.868112<cthl && cthl<0.900622) && (0.975993<cthf[1] && cthf[1]<0.988809)");
t2->Write();
fout->Close();
fin->Close();




