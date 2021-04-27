#!/bin/bash
#SBATCH --job-name=436
#SBATCH --output=/scratch/user/kevin.weston/APEX/result/436.txt
#SBATCH --time=12:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=6
#SBATCH --mem=6G

export DAN_POLICY=2
python3 /scratch/user/kevin.weston/APEX/server/main.py 436 /scratch/user/kevin.weston/APEX/weights/436.hdf5 8 16 &
sleep 120
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/436.cactusADM-1804B.trace.gz
