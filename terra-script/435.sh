#!/bin/bash
#SBATCH --job-name=435
#SBATCH --output=/scratch/user/kevin.weston/APEX/result/435.txt
#SBATCH --time=12:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=6
#SBATCH --mem=6G

export DAN_POLICY=2
python3 /scratch/user/kevin.weston/APEX/server/main.py 435 /scratch/user/kevin.weston/APEX/weights/435.hdf5 8 16 &
sleep 120
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/435.gromacs-111B.trace.gz
