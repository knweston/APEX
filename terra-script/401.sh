#!/bin/bash
#SBATCH --job-name=401
#SBATCH --output=/scratch/user/kevin.weston/APEX/result/401.txt
#SBATCH --time=48:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=6
#SBATCH --mem=6G

export DAN_POLICY=2
export SERVER_CONF_FILE=/scratch/user/kevin.weston/APEX/server/config/401.ini
python3 /scratch/user/kevin.weston/APEX/server/main.py 401 /scratch/user/kevin.weston/APEX/weights/401.hdf5 /scratch/user/kevin.weston/APEX/server/config/401.ini 8 16 &
# python3 /scratch/user/kevin.weston/APEX/server/main.py 401 none /scratch/user/kevin.weston/APEX/server/config/401.ini 8 16 &
sleep 900
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/401.bzip2-226B.trace.gz
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/401.bzip2-226B.trace.gz
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/401.bzip2-226B.trace.gz
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/401.bzip2-226B.trace.gz
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/401.bzip2-226B.trace.gz
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/401.bzip2-226B.trace.gz
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/401.bzip2-226B.trace.gz
