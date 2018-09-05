#!/bin/bash

#SBATCH --mail-user=mperkins@georgiasouthern.edu
#SBATCH --mail-type=ALL

#SBATCH --ntasks=4
####SBATCH --ntasks-per-core=1


module load python/python-3.5.2
module load system/openmpi-1.10.2

mpiexec -n 4 python script.py
