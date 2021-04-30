#!/bin/bash
#SBATCH --job-name=458
#SBATCH --output=/scratch/user/kevin.weston/APEX/result/458.txt
#SBATCH --time=48:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=6
#SBATCH --mem=6G

export DAN_POLICY=2
export SERVER_CONF_FILE=/scratch/user/kevin.weston/APEX/server/config/458.ini
python3 /scratch/user/kevin.weston/APEX/server/main.py 458 /scratch/user/kevin.weston/APEX/weights/458.hdf5 /scratch/user/kevin.weston/APEX/server/config/458.ini 8 16 &
# python3 /scratch/user/kevin.weston/APEX/server/main.py 458 none /scratch/user/kevin.weston/APEX/server/config/458.ini 8 16 &
sleep 600
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/458.sjeng-767B.trace.gz
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/458.sjeng-767B.trace.gz
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/458.sjeng-767B.trace.gz
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/458.sjeng-767B.trace.gz
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/458.sjeng-767B.trace.gz
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/458.sjeng-767B.trace.gz
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/458.sjeng-767B.trace.gz
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/458.sjeng-767B.trace.gz
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/458.sjeng-767B.trace.gz
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/458.sjeng-767B.trace.gz
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/458.sjeng-767B.trace.gz
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/458.sjeng-767B.trace.gz
