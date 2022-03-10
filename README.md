# GeomStudy
Development area for code to study the as-built, in-memory Geant4 geometry for Mu2e.  The code may eventually be merged into Mu2e Offline or the repo may be moved to the Mu2e GitHub instance.

The main code body is called from inside an art Analyzer module but it could be moved to the Mu2eG4 and/or Mu2eG4MT modules and enabled/disabled with run-time configuration.

The code looks at the G4PhysicalVolumeStore, G4LogicalVolumeStore, G4SolidStore.  It produces:
1. Summary information about each object in each store
2. A heirarchical description of the geometry.
3. Warnings about duplicate names in a store; these are not illegal within G4 but it can confuse user code that expects unique names.
4. Warnings about illegal configurations, such as identical planes within a G4PolyCone.
5. Warnings for inconsistent parent-child relationships - there should not be any.

This is known to work with versions of Offline v10_10_01.  To use this, cd to a clean working directory and have a clean environment.

<pre>
setup mu2e
muse link Offline v10_10_01
git clone https://github.com/kutschke/GeomStudy
git clone https://github.com/Mu2e/Production
muse setup -1
muse build -j 8
mu2e -c GeomStudy/fcl/geomStudy01.fcl >&  geomStudy01.log.1
</pre>

If you want to build against a new prebuilt version of Offline, you can do the following
in your existing muse working area.  Start with a clean shell.

<pre>
setup mu2e
rm -rf link build
muse link Offline vxx_yy_zz
git clone https://github.com/kutschke/GeomStudy
# you may wish to cd into Production and do git pull and/or checkout a new tag
muse setup -1
muse build -j 8
mu2e -c GeomStudy/fcl/geomStudy01.fcl >&  geomStudy01.log.1
</pre>


Alternatively, to build both Offline and GeomStudy, do the following:
<pre>
setup mu2e
git clone https://github.com/kutschke/GeomStudy
git clone https://github.com/Mu2e/Production
git clone https://github.com/Mu2e/Offline
# optionally checkout tags of the 3 repositories
muse setup -1
muse build -j 8
mu2e -c GeomStudy/fcl/geomStudy01.fcl >&  geomStudy01.log.1
</pre>

To add GeomStudy to an existing build of Offline, start in a clean terminal window
and cd to the existing Muse working directory (ie the directory that contains
Offline as a subdirectory).  Then:

<pre>
setup mu2e
git clone https://github.com/kutschke/GeomStudy
# optionally checkout a tag of GeomStudy
muse setup -1
muse build -j 8
mu2e -c GeomStudy/fcl/geomStudy01.fcl >&  geomStudy01.log.1
</pre>
The reason that you need to start in a clean terminal window
is that when you run muse setup, Muse looks for repositories
to bulid. If you add a new repository after you have run muse setup,
then Muse will not know that it's there.
