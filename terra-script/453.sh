#!/bin/bash
#SBATCH --job-name=453
#SBATCH --output=/scratch/user/kevin.weston/APEX/result/453.txt
#SBATCH --time=24:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=6
#SBATCH --mem=6G

export DAN_POLICY=2
export SERVER_CONF_FILE=/scratch/user/kevin.weston/APEX/server/config/453.ini
# python3 /scratch/user/kevin.weston/APEX/server/main.py 453 /scratch/user/kevin.weston/APEX/weights/453.hdf5 /scratch/user/kevin.weston/APEX/server/config/453.ini 8 16 &
python3 /scratch/user/kevin.weston/APEX/server/main.py 453 none /scratch/user/kevin.weston/APEX/server/config/453.ini 8 16 &
sleep 120
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/453.povray-576B.trace.gz
