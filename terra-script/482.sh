#!/bin/bash
#SBATCH --job-name=482
#SBATCH --output=/scratch/user/kevin.weston/APEX/result/482.txt
#SBATCH --time=24:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=6
#SBATCH --mem=6G

export DAN_POLICY=2
export SERVER_CONF_FILE=/scratch/user/kevin.weston/APEX/server/config/482.ini
# python3 /scratch/user/kevin.weston/APEX/server/main.py 482 /scratch/user/kevin.weston/APEX/weights/482.hdf5 /scratch/user/kevin.weston/APEX/server/config/482.ini 8 16 &
python3 /scratch/user/kevin.weston/APEX/server/main.py 482 none /scratch/user/kevin.weston/APEX/server/config/482.ini 8 16 &
sleep 120
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/482.sphinx3-417B.trace.gz