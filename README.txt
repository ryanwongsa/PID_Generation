UCT Honours Project

Data from Simulations is in the file ToUseData.txt.
- Event stuff can mostly be ignored
- Ignore particles that have no data in, some simulation error occured
- Each track indicates a particle and will have their particle type indicated under (electron or pion).
- Each track will have at most 6 tracklets which is separated by '----'

Data Extraction Instructions
1. aliroot run1.C > samples.txt
2. java ExtractFirstMacro [creates dataformated.txt]
3. sh reading.sh [creates finaldata.txt]
4. java ExtractFinalData [creates tousedata.txt]

OR

1. sh OneMacroToRule.sh

Creating a PID Generator for ALICE using Neural Networks.

Developed By: Ryan Wong
