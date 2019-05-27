@echo off
cd Program
ls
rm dectree
make clean
make
cd ..

REM set INSTANCES=p01.txt p02.txt p03.txt p04.txt p05.txt p06.txt p07.txt p08.txt p09.txt p10.txt 
REM set INSTANCES=p03.txt p04.txt p05.txt p06.txt p07.txt p08.txt p09.txt p10.txt 
set INSTANCES=p40.txt p43.txt p51.txt p52.txt p54.txt
REM set INSTANCES=p07.txt
(for %%a in (%INSTANCES%) do (
   echo %%a
   echo %%a >> "Solutions/output.txt"
   REM del "./Solutions/results.csv"
   "Program/dectree.exe" "Datasets/%%a" -depth 4 -sol "Solutions/%%a" -seed 0
))
