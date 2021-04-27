#!/bin/bash
#SBATCH --job-name=470
#SBATCH --output=/scratch/user/kevin.weston/APEX/result/470.txt
#SBATCH --time=24:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=6
#SBATCH --mem=6G

export DAN_POLICY=2
export SERVER_CONF_FILE=/scratch/user/kevin.weston/APEX/server/config/470.ini
# python3 /scratch/user/kevin.weston/APEX/server/main.py 470 /scratch/user/kevin.weston/APEX/weights/470.hdf5 /scratch/user/kevin.weston/APEX/server/config/470.ini 8 16 &
python3 /scratch/user/kevin.weston/APEX/server/main.py 470 none /scratch/user/kevin.weston/APEX/server/config/470.ini 8 16 &
sleep 120
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/470.lbm-1274B.trace.gz