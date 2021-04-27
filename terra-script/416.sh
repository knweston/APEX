#!/bin/bash
#SBATCH --job-name=416
#SBATCH --output=/scratch/user/kevin.weston/APEX/result/416.txt
#SBATCH --time=24:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=6
#SBATCH --mem=6G

export DAN_POLICY=2
export SERVER_CONF_FILE=/scratch/user/kevin.weston/APEX/server/config/416.ini
# python3 /scratch/user/kevin.weston/APEX/server/main.py 416 /scratch/user/kevin.weston/APEX/weights/416.hdf5 /scratch/user/kevin.weston/APEX/server/config/416.ini 8 16 &
python3 /scratch/user/kevin.weston/APEX/server/main.py 416 none /scratch/user/kevin.weston/APEX/server/config/416.ini 8 16 &
sleep 120
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/416.gamess-875B.trace.gz
