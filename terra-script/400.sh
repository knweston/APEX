#!/bin/bash
#SBATCH --job-name=400
#SBATCH --output=/scratch/user/kevin.weston/APEX/result/400.txt
#SBATCH --time=24:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=6
#SBATCH --mem=6G

export DAN_POLICY=2
export SERVER_CONF_FILE=/scratch/user/kevin.weston/APEX/server/config/400.ini
python3 /scratch/user/kevin.weston/APEX/server/main.py 400 /scratch/user/kevin.weston/APEX/weights/400.hdf5 /scratch/user/kevin.weston/APEX/server/config/400.ini 8 16 &
sleep 120
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/400.perlbench-50B.trace.gz
