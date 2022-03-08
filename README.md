# GeomStudy
Development area for code to study the as-built, in-memory Geant4 geometry for Mu2e.  The code may eventually be merged into Mu2e Offline or the repo may be moved to the Mu2e GitHub instance.

The main code body is called from inside an art Analyzer module but it could be moved to the Mu2eG4 and/or Mu2eG4MT modules and enabled/disabled with run-time configuration.

The code looks at the G4PhysicalVolumeStore, G4LogicalVolumeStore, G4SolidStore.  It produces:
1. Summary information about each object in each store
2. A heirarchical description of the geometry.
3. Warnings about duplicate names in a store; these are not illegal within G4 but it can confuse user code that expects unique names.
4. Warnings about illegal configurations, such as identical planes within a G4PolyCone.
5. Warnings for inconsistent parent-child relationships - there should not be any.