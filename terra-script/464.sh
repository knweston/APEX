#!/bin/bash
#SBATCH --job-name=464
#SBATCH --output=/scratch/user/kevin.weston/APEX/result/464.txt
#SBATCH --time=48:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=6
#SBATCH --mem=6G

export DAN_POLICY=2
export SERVER_CONF_FILE=/scratch/user/kevin.weston/APEX/server/config/464.ini
python3 /scratch/user/kevin.weston/APEX/server/main.py 464 /scratch/user/kevin.weston/APEX/weights/464.hdf5 /scratch/user/kevin.weston/APEX/server/config/464.ini 8 16 &
# python3 /scratch/user/kevin.weston/APEX/server/main.py 464 none /scratch/user/kevin.weston/APEX/server/config/464.ini 8 16 &
sleep 900
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/464.h264ref-97B.trace.gz
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/464.h264ref-97B.trace.gz
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/464.h264ref-97B.trace.gz
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/464.h264ref-97B.trace.gz
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/464.h264ref-97B.trace.gz
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/464.h264ref-97B.trace.gz
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/464.h264ref-97B.trace.gz
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/464.h264ref-97B.trace.gz
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/464.h264ref-97B.trace.gz
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/464.h264ref-97B.trace.gz
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/464.h264ref-97B.trace.gz
/scratch/user/kevin.weston/APEX/exclusiu /scratch/user/kevin.weston/traces/464.h264ref-97B.trace.gz
