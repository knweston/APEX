#!/bin/bash
#SBATCH --job-name=444
#SBATCH --output=/scratch/user/kevin.weston/APEX/result/444.txt
#SBATCH --time=12:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=6
#SBATCH --mem=6G

export DAN_POLICY=2
python3 /scratch/user/kevin.weston/APEX/server/main.py 444 /scratch/user/kevin.weston/APEX/weights/444.hdf5 8 16 &
sleep 120
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/444.namd-426B.trace.gz
