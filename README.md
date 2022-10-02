# GeomStudy
Development area for code to study the as-built, in-memory Geant4 geometry for Mu2e.  The code may eventually be merged into Mu2e Offline or the repo may be moved to the Mu2e GitHub instance.
It's still in development and contains a lot of debug printout.

The main code body is called from inside an art Analyzer module but it could be moved to the Mu2eG4 and/or Mu2eG4MT modules and enabled/disabled with run-time configuration.

The code looks at the G4PhysicalVolumeStore, G4LogicalVolumeStore, G4SolidStore.  It produces:
1. Summary information about each object in each store
2. A heirarchical description of the geometry.
3. Warnings about duplicate names in a store; these are not illegal within G4 but we have sometimes had user code that expected unique names.
4. Warnings about illegal configurations, such as identical planes within a G4PolyCone.
5. Warnings for inconsistent parent-child relationships - there should not be any.

This is known to work with versions of Offline v10_18_00 but is likely to work with more recnet versions.
The examples choose whichever Offline Musing is current at the time you run the ```muse backing``` command.
To follow this example, cd to a clean working directory and have a clean environment.

<pre>
setup mu2e
muse backing Offline
git clone https://github.com/kutschke/GeomStudy
# optionally checkout a tag of GeomStudy
muse setup
muse build -j 8
mu2e -c GeomStudy/fcl/geomStudy01.fcl >&  geomStudy01.log.1
</pre>

The build will take about 10 seconds if the /cvmfs local cache is loaded and about 30 seconds if the cache needs to be loaded.
Running the job will take about 1:15 (m:ss) on a lightly loaded  mu2egpvm machine if the /cvmfs cache is loaded and about 2:30 (m:ss)
if the cache needs to be loaded.

Alternatively, to build both Offline and GeomStudy, do the following:
<pre>
setup mu2e
git clone https://github.com/kutschke/GeomStudy
git clone https://github.com/Mu2e/Production
git clone https://github.com/Mu2e/Offline
# optionally checkout tags of the 3 repositories
muse setup
muse build -j N
mu2e -c GeomStudy/fcl/geomStudy01.fcl >&  geomStudy01.log.1
</pre>

where a reasonable choice for N is 24 on mu2ebuild01 and 8 on the mu2egpvm machines.

The job produces the log file plus 4 output files:

| Filename | Description |
| --- | --- |
| debug.txt             | Debuging and development printout, including info about duplicate names |
| solids.txt            | Summary of all solids |
| volumeDetails.txt     | Details of each physical volume |
| volumeHierarchy.txt   | Formatted description fo the volume hierarchy |

